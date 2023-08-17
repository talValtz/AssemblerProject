#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_LABEL 31
#define MEMORY_ARRAY_SIZE 1024

typedef struct LABEL
{
    int lineNum;
    char labelName[31];
    int type; /*0-none, 1-entry,2-extern*/
    struct LABEL *next;
} LABEL;
typedef struct cidodProp
{
    unsigned short code;
    int type; /*0-directive(hanhia), 1-instruction(horaa)*/
} cidodProp;

char *stringTwoStrings(char *first_str, char *second_str);
int externFlag,entryFlag;
