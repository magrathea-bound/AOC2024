#include <stdio.h>
#include <errno.h>

int main(int argc, char *argv[]){
    if(argc !== 2){
        perror("Argument Error:")
        return -1
    }

    return 0
}
