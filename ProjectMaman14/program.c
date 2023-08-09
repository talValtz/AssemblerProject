#include "program.h"



int processfile(char *fileName){


    preProcess(fileName);
    return 0;

}

int main(int argc, char *argv[]){
    int i;
    if (argc < 2) {
        fprintf(stderr, "Usage: %s file1 file2 file3 ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    for (i = 1; i < argc; i++) {
        if (strlen(argv[i]) > MAX_FILE_NAME) {
            fprintf(stderr, "Filename %s is too long (max %d characters)\n",
                    argv[i], MAX_FILE_NAME);
            continue;
        }
        processfile(argv[i]);
    }
    return 0;
}

