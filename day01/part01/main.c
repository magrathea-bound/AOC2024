#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int INPUT_ROWS = 1000;

void swap(int* a, int* b) {
    int t =  *a;
    *a = *b;
    *b = t;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    
    swap(&arr[i + 1], &arr[high]);  
    return i + 1;
}

void quickSort(int arr[], int low, int high) {
    if (low < high){
    int partition_index = partition(arr, low, high);

    quickSort(arr, low, partition_index - 1);
    quickSort(arr, partition_index + 1, high);
    }
}

int main(int argc, char *argv[]) {
    if(argc != 2){
        errno = EINVAL;
        perror("Argument Error");
        return -1;
    };

    FILE *fd = fopen(argv[1], "r");
    if (fd == NULL) {
        perror("Error opening file");
        return -1;
    }
    
    int col1[INPUT_ROWS], col2[INPUT_ROWS];
    int row_count = 0;

    while (fscanf(fd, "%d %d", &col1[row_count], &col2[row_count]) == 2) {
            row_count++;
        }

    fclose(fd);

    quickSort(col1, 0, INPUT_ROWS - 1);
    quickSort(col2, 0, INPUT_ROWS - 1);

    int tot = 0;

    for (int i = 0; i < INPUT_ROWS; i++) {
        tot += abs(col1[i] - col2[i]);
    }

    printf("Total: %d \n", tot);

    return 0;
}
