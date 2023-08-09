#ifndef __MACRO__
#define __MACRO__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "firstPass.h"


#define MAX 100
typedef struct Macro{
    char name[MAX];
    char content[MAX];
    struct Macro *next;
}Macro;

char *str_allocate_cat(char *first_str, char* second_str);
int copyContentToFile(Macro *cur, FILE *cpy);
int checkMacroName(Macro *head, char line[], FILE *cpy);
int checkInMacro(char line[], Macro **head);
int preProcess(char *fileName);
#endif