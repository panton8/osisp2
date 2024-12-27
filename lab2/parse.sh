#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Использование: $0 <input_file.csv>"
    exit 1
fi

input_file=$1
output_file="queries.sql"

> "$output_file"

IFS=';' read -r -a tables < "$input_file"
IFS=';' read -r -a fields < <(tail -n +2 "$input_file")

table_index=0
data=()

while IFS=',' read -r -a line; do
    for value in "${line[@]}"; do
        data+=("$value")
    done

    if [[ ${data[-1]} == *";"* ]]; then
        num_fields=$(echo ${fields[table_index]} | awk -F, '{print NF}')
        num_values=$(( ${#data[@]} / num_fields ))

        values=""
        for (( i=0; i<num_values; i++ )); do
            start=$(( i * num_fields ))
            end=$(( (i + 1) * num_fields ))

            if [ $i -eq $((num_values - 1)) ]; then
                last_value="${data[@]:$start:$num_fields}"
                last_value="${last_value//;/}"
                values+="($last_value)"
            else
                values+="(${data[@]:$start:$num_fields}),"
            fi

            values+=$'\n'
        done

        values="${values%$'\n'}"

        echo "INSERT INTO ${tables[table_index]} (${fields[table_index]}) VALUES" >> "$output_file"
        echo "$values;" | sed 's/ /, /g' >> "$output_file"
        data=()
        table_index=$((table_index + 1))
    fi
done < <(tail -n +3 "$input_file")

echo "Запросы были успешно сгенерированы и записаны в файл $output_file"
