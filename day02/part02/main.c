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
int level_check(int *report, int col, int dampened) {
    int safe = 0;
    int desc;
    switch(dampened) {
        case 0:
            if(report[1] == report[2]) {
                return 0;
            } else if(report[1] < report[2]) {
                desc = -1;
            } else {
                desc = 1;
            }
            break;
        case 1:
            if(report[0] == report[2]) {
                return 0;
            } else if(report[0] < report[2]) {
                desc = -1;
            } else {
                desc = 1;
            }
            break;
        default:
            if(report[0] == report[1]) {
                safe = level_check(report, col, 0);
                if(safe != 0) {
                    return 1;
                }
                safe = level_check(report, col, 1);
                if(safe != 0) {
                    return 1;
                }
                return 0;
            } else if(report[0] < report[1]) {
                desc = -1;
            } else {
                desc = 1;
            }
            break;
    }

    int j;
    switch(dampened) {
        case 0:
            j = 2;
            break;
        default:
            j = 1;
            break;
    }
    int dampAdj = 0;
    while(report[j] != 0 && j < col) {
        if(dampened == j) {
            dampAdj = 1;
            j++;
            continue;
        }
        int diff = report[j] - report[j-1-dampAdj]; 
        diff *= -1 * desc;
        if(dampened < 0 && (diff < 1 || diff > 3)) {
            safe = level_check(report, col, j);
            if(safe != 0) {
                return 1;
            }
            safe = level_check(report, col, j-1);
            if(safe != 0) {
                return 1;
            }
            return 0;
        } else if (diff < 1 || diff > 3) {
            return 0;
        }
        dampAdj = 0;
        j++;
    }
    return 1;
}

void free_rc_array(int **reports, int rows) {
    for(int i = 0; i < rows; i++) {
        free(reports[i]);
    }
    free(reports);
}

int safe_check(Arr_Rc *reports) {
    int tot = 0;
    for(int i = 0; i < reports->row; i++) {
        tot += level_check(reports->arr[i], reports->col, -1);
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
    
    free_rc_array(data->arr, data->row);
    return 0;
}
