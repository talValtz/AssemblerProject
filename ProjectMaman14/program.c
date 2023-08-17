#include "program.h"

int processfile(char *fileName)
{
    preProcess(fileName);
    return 0;

}
/*The program will receive the names of the input files,
it will display an error message when a file name is too long, and then continue to process the given files.
Using sprintf to convert the value of i to a string is a common and standard way to generate random numeric strings.*/
int main(int argc, char *argv[])
{
    int i;
    char *err,indexStr[10];
    if (argc < 2) {
        fprintf(stderr, "Usage: %s file1 file2 file3 ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    for (i = 1; i < argc; i++)
    {
        if (strlen(argv[i]) > MAX_FILE_NAME)
        {

            sprintf(indexStr, "%d", i);
            err=stringTwoStrings("Filename ", indexStr);
            printError(stringTwoStrings(err,"is too long (max 30 characters)"));
            continue;
        }
        processfile(argv[i]);
    }
    return 0;
}

