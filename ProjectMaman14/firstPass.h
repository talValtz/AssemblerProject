#ifndef __FIRSTPASS__
#define __FIRSTPASS__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "secondPass.h"



typedef struct Operand
{
    int num;
    int mion;
    char labelName[100];
} Operand;




void printError(char *printText);
void insertNode(LABEL ** head, int lineNum,  char *labelName, int type);
short int cidodSourceMion(Operand source, Operand target,LABEL **headToDecode,cidodProp *progArr,int *L,int *ic,int *dc) ;
short int kidodTargetMion(Operand target,LABEL **headToDecode,cidodProp *progArr,int *L,int *ic, int *dc);
int DirectiveData(char *sourceCode,cidodProp *progArr,int *L, int *ic, int *dc);
int DirectiveString(char *sourceCode,cidodProp *progArr,int *L, int *ic, int *dc);
int oneOperand(int act, char *wwsline,LABEL **headToDecode,cidodProp *progArr,int *L,int *ic, int *dc);
int twoOperands(int act, char *wwsline,LABEL **headToDecode,cidodProp *progArr,int *L,int *ic, int *dc);
Operand oprandType(char *op);
int typeSen(char *line, LABEL **head,LABEL **headToDecode,cidodProp *progArr,int *L,int *ic,int *dc);
short actionArray(char *ch);
int firstppass(char *fileName);


#endif
