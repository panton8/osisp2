#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_THREADS 8

pthread_mutex_t mergeMutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int *array;
    int size;
} ThreadData;

int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

void *sortFragment(void *arg) {
    ThreadData *data = (ThreadData*)arg;
    qsort(data->array, data->size, sizeof(int), compare);
    pthread_exit(NULL);
}

void merge(int *array, int *temp, int left, int mid, int right) {
    pthread_mutex_lock(&mergeMutex);
    int i = left, j = mid + 1, k = left;
    while (i <= mid && j <= right) {
        if (array[i] <= array[j]) {
            temp[k++] = array[i++];
        } else {
            temp[k++] = array[j++];
        }
    }
    while (i <= mid) {
        temp[k++] = array[i++];
    }
    while (j <= right) {
        temp[k++] = array[j++];
    }
    for (i = left; i <= right; ++i) {
        array[i] = temp[i];
    }
    pthread_mutex_unlock(&mergeMutex);
}

void mergeSort(int *array, int *temp, int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        mergeSort(array, temp, left, mid);
        mergeSort(array, temp, mid + 1, right);
        merge(array, temp, left, mid, right);
    }
}


void multiThreadedSort(int *array, int size, int numThreads) {
    pthread_t threads[numThreads];
    ThreadData threadData[numThreads];


    int fragmentSize = size / numThreads;
    int i;
    for (i = 0; i < numThreads; ++i) {
        threadData[i].array = array + i * fragmentSize;
        threadData[i].size = (i == numThreads - 1) ? (size - i * fragmentSize) : fragmentSize;
        pthread_create(&threads[i], NULL, sortFragment, (void*)&threadData[i]);
    }


    for (i = 0; i < numThreads; ++i) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    srand(time(NULL));

    int size, numThreads;
    printf("Enter the size of the array: ");
    if (scanf("%d", &size) != 1 || size <= 0) {
        printf("Invalid input for array size\n");
        return 1;
    }
    printf("Enter the number of threads: ");
    if (scanf("%d", &numThreads) != 1 || numThreads <= 0 || numThreads > MAX_THREADS) {
        printf("Invalid input for number of threads\n");
        return 1;
    }

    int *array = (int*)malloc(size * sizeof(int));
    if (array == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    printf("Unsorted array:\n");
    for (int i = 0; i < size; ++i) {
        array[i] = rand() % 100;
        printf("%d ", array[i]);
    }
    printf("\n");

    int *temp = (int*)malloc(size * sizeof(int));
    if (temp == NULL) {
        printf("Memory allocation failed\n");
        free(array);
        return 1;
    }

    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    mergeSort(array, temp, 0, size - 1);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double cpu_time_used = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Sorted array:\n");
    for (int i = 0; i < size; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");

    printf("Configuration: Array size = %d, Number of threads = %d\n", size, numThreads);
    printf("Time taken: %f seconds\n", cpu_time_used);

    free(array);
    free(temp);

    return 0;
}
