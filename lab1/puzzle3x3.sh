#!/bin/bash

board=("7" "4" "1" "2" "3" "5" " " "8" "6")

print_board() {
    clear
    echo "--------------"
    echo "| ${board[0]} | ${board[1]} | ${board[2]} |"
    echo "--------------"
    echo "| ${board[3]} | ${board[4]} | ${board[5]} |"
    echo "--------------"
    echo "| ${board[6]} | ${board[7]} | ${board[8]} |"
    echo "--------------"
}

move() {
    local from=$1
    local to=$2
    local temp=${board[$from]}
    board[$from]=${board[$to]}
    board[$to]=$temp
}

empty_pos=6

moves_file="moves.txt"
echo "Ходы игры:" > "$moves_file"

print_board >> "$moves_file"
echo "--------------" >> "$moves_file"

while true; do
    print_board

    if [ "${board[*]}" == "1 2 3 4 5 6 7 8  " ]; then
        echo "Поздравляем! Вы выиграли!"
        exit 0
    fi

    echo "Используйте стрелки для перемещения (q для выхода):"
    read -n 1 key

    case $key in
        q)
            exit 0
            ;;
        A)
            move_pos=$((empty_pos - 3))
            ;;
        B)
            move_pos=$((empty_pos + 3))
            ;;
        C)
            move_pos=$((empty_pos + 1))
            ;;
        D)
            move_pos=$((empty_pos - 1))
            ;;
        *)
            echo "Некорректный ввод! Используйте стрелки для перемещения (q для выхода)."
            continue
            ;;
    esac

    if [ "$move_pos" -ge 0 ] && [ "$move_pos" -le 8 ]; then
        move $empty_pos $move_pos
        empty_pos=$move_pos

        print_board >> "$moves_file"
        echo "--------------" >> "$moves_file"
    else
        echo "Невозможно переместить число. Попробуйте еще раз."
        sleep 1
    fi
done
