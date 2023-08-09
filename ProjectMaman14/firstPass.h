#ifndef __FIRSTPASS__
#define __FIRSTPASS__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "secondPass.h"


#define MAX_LINE 80


typedef struct Operand
{
    int num;
    int mion;
    char labelName[100];
} Operand;




void printError(char *printText);
void insertNode(LABEL ** head, int lineNum,  char *labelName, int type);
short int kidodSourceMion(Operand source, Operand target,LABEL **headUnFind) ;
short int kidodTargetMion(Operand targert,LABEL **headUnFind);
int DirectiveData(char *sourceCode);
int DirectiveString(char *sourceCode);
int oneOperand(int act, char *wwsline,LABEL **headUnFind);
int twoOperands(int act, char *wwsline,LABEL **headUnFind);
Operand oprandType(char *op);
int typeSen(char *line, LABEL **head,LABEL **headUnFind);
short actionArray(char *ch);
int firstppass(char *fileName);


#endif
