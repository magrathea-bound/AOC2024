#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct {
    int row;
    int col;
    int **arr;
} Arr_Rc;

Arr_Rc* data_read(char *input) {
    FILE *reports = fopen(input, "r");
    if (!reports) {
        errno = ENOENT;
        perror("Invalid File");
        return NULL;
    }

    Arr_Rc *data = (Arr_Rc *)malloc(sizeof(Arr_Rc)); 
    data->row = 0;
    data->col= 0;
    char line[1024];
    while(fgets(line, sizeof(line), reports)) {
        data->row++;
        char *token = strtok(line, " ");
        int count = 0;
        while(token != NULL) {
            count++;
            token = strtok(NULL, " ");
        }
        if(count > data->col) {
            data->col = count;
        }
    }

    data->arr = (int **)malloc(data->row * sizeof(int *));
    for(int i = 0; i < data->row; i++) {
        data->arr[i] = (int *)calloc(data->col, sizeof(int));
    }

    rewind(reports);
    int i = 0;
    int level = 0;
    while (fgets(line, sizeof(line), reports)) {
        int j = 0;
        char *token = strtok(line, " ");
        while (token != NULL) {
            sscanf(token, "%d", &level);
            data->arr[i][j] = level;
            token = strtok(NULL, " ");
            j++;
        }
        i++;
    }
    return data;
}

int safe_check(Arr_Rc *reports) {
    int tot = 0;
    int down;
    for(int i = 0; i < reports->row; i++) {
        if(reports->arr[i][0] == reports->arr[i][1]) {
            continue;
        } else if(reports->arr[i][0] < reports->arr[i][1]) {
            down = -1;
        } else {
            down = 1;
        }

        int j = 1;
        while(reports->arr[i][j] != 0 && j < reports->col) {
            int diff = reports->arr[i][j] - reports->arr[i][j-1]; 
            diff *= -1 * down;
            if(diff < 1 || diff > 3) {
                tot -= 1;
                break;
            }
            j++;
        }
        tot += 1;
    }
    return tot;
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        errno = EINVAL;
        perror("Invalid argument");
        return -1;
    }

    Arr_Rc *data = data_read(argv[1]);
    if(!data) {
        perror("Trouble loading data");
        return -1;
    }

    int safe_reports = safe_check(data);
    printf("Number of safe reports: %d\n", safe_reports);
    
    return 0;
}
