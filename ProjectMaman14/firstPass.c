
#include "firstPass.h"
#include "program.h"

int errorFirst=0;/*global variable the defined if there are any error int the first pass 0- there are no 1-there are*/

/*  Prints an error message in red text to stderr and sets the errorFirst flag.*/
void printError(char *printText)
{
    fprintf(stderr,"\033[0;31m Invalid : %s \033[0m\n",printText); /* Red */
    errorFirst=1;

}
/*  Prints the binary representation of an integer.*/
void printBin(int x)
{
    unsigned int mask = 1<<11;
    while (mask != 0)
    {
        if (x & mask)
            putchar('1');
        else
            putchar('0');

        mask = mask >> 1;
    }
}
/*Prints the content of the code image in binary and its type*/
void printarray(LABEL *head,cidodProp progArr[],int L)
{
    LABEL *temp =head;
    int i;
    printf("\nThe code image:\n");
    printf("Binary code      type\n");
    for (i = 0; i < L; i++)
    {
        printf("%d: ", i);
        printBin(progArr[i].code);
        printf("  ,%d\n", progArr[i].type);
    }
}


/*check if the label name is already exist in the label linked list if yes return 1 else return 0*/
int checkExistLabel(LABEL **head,char *name)
{
    LABEL *temp=*head;
    while(temp!=NULL)
    {
        if (strcmp(temp->labelName,name) == 0)
        {
            return 1;
        }
        temp=temp->next;
    }
    return 0;
}
/*insert node to the list- function to the label that need to decode in the next pass*/
void insertHeadToDecode(LABEL ** headToDecode, int lineNum,  char *labelName, int type)
{

    LABEL* newLabel = (LABEL*)malloc(sizeof(LABEL));
    LABEL * cur=*headToDecode;
    char *ezer;
    int i=0;
    newLabel->lineNum = lineNum;
    ezer = (char*)malloc(strlen(labelName) + 1);
    if (newLabel==NULL||ezer==NULL)
    {
        printf("Error in memory allocated!");
        exit(0);
    }
    while(i< strlen(labelName)&& labelName[i]!='\n')
    {
        ezer[i]= labelName[i];
        i++;
    }
    ezer[i]='\0';
    strcpy(newLabel->labelName, ezer);
    newLabel->labelName[sizeof(newLabel->labelName) - 1] = '\0';
    newLabel->type = type;
    newLabel->next = NULL;
    if (*headToDecode == NULL)
    {
        *headToDecode = newLabel;
    } else {
        while (cur->next != NULL)
        {
            cur = cur->next;
        }
        cur->next = newLabel;
    }
}
/* Inserts a node for a label into the label linked list.*/
void insertNode(LABEL ** head, int lineNum,  char *labelName, int type)
{

    LABEL* newLabel = (LABEL*)malloc(sizeof(LABEL));
    LABEL * cur=*head;
    newLabel->lineNum = lineNum;
    strncpy(newLabel->labelName, labelName, sizeof(newLabel->labelName) - 1);
    newLabel->labelName[sizeof(newLabel->labelName) - 1] = '\0'; /*Ensure null-termination-check it with miki*/
    newLabel->type = type;
    newLabel->next = NULL;
    if (checkExistLabel(head, labelName))
    {
        printError("The label  is already exist");
    }
    else
    {
        if (*head == NULL)
        {
            *head = newLabel;

        } else
        {

            while (cur->next != NULL)
            {
                cur = cur->next;
            }
            cur->next = newLabel;
        }
    }
}
/* Generates machine code for target operands in the instruction by different mion methods .*/
short int cidodSourceMion(Operand source, Operand target,LABEL **headToDecode,cidodProp *progArr,int *L,int *ic,int *dc) /*change to pointer^^^^^^^^^^*/
{
    short  coding = 0;
    if (source.mion == 5)
    {
        coding = coding | (source.num << 7);
        progArr[*L].code = coding;
        progArr[*L].type = 1;

    }
    else if (source.mion == 1)
    {
        coding = coding | (source.num << 2);
    }
    (*ic)++;
    (*L)++;
    if (target.mion == 5)
    {
        if (source.mion == 5)
        {
            coding = coding | (target.num << 2);
            progArr[(*L) - 1].code = coding;
        }
        else
        {
            coding = 0;
            coding = coding | (target.num << 2);
            progArr[*L].code = coding;
            progArr[*L].type = 1;

            (*ic)++;
            (*L)++;
        }
    }
    else if (target.mion == 1)
    {
        coding = 0;
        coding = coding | (target.num << 2);
        progArr[*L].code = coding;
        progArr[*L].type = 1;
        (*ic)++;
        (*L)++;
    }
    else if (target.mion == 3)
    {
        coding = 0;
        progArr[*L].code = coding;
        progArr[*L].type = 1;
        insertHeadToDecode(headToDecode,*L,target.labelName,0);
        (*ic)++;
        (*L)++;
    }
    return coding;
}
/*Generates machine code for target operands */
short int kidodTargetMion(Operand target,LABEL **headToDecode,cidodProp *progArr,int *L,int *ic, int *dc)
{
    /*combine with the source function, think about the source when its null*/
    short int coding = 0;
    if (target.mion == 5 || target.mion == 1)
    {
        coding = coding | (target.num << 2);
    }
    else if(target.mion==3){
        insertHeadToDecode(headToDecode,*L,target.labelName,0);
    }
    progArr[*L].code = coding;
    progArr[*L].type = 1;
    (*ic)++;
    (*L)++;
    return coding;
}
/*Handles the ".data" directive, converting data values to machine code*/
int DirectiveData(char *sourceCode,cidodProp *progArr,int *L, int *ic, int *dc)
{
    char *dataNum,*ptr,*source = NULL;
    int num,countData=0,i=0,countComma=0,sourceLen= (int) (source);
    source = (char *)malloc(sourceLen= + 1);
    strcpy(source,sourceCode);
    dataNum= strtok(sourceCode,", ");
    if (source == NULL)
    {
        printf("Error in memory allocated!");
        exit(0);
        return -1;
    }
    if (dataNum)
    {
        while (dataNum)
        {
            num = (int)strtol(dataNum, &ptr, 10);
            /*check if there are some char that are not integer in the dataNum */
            if(strcmp(ptr, "") == 0|| strcmp(ptr,"\n")==0)
            {
                /*Check if the numbers are in the appropriate range for 12 bits*/
                if (num<2048&&num>-2049)
                {
                    progArr[*L].code=num;
                    progArr[*L].type=0;
                    (*L)++;
                    (*dc)++;
                }
                else
                {
                    printError("There is an invalid number in the input, the number range should be between -2048 to 2047 ");
                }
            }
            else{
                printError("The number is invalid, there are invalid characters");
            }
            dataNum = strtok(NULL, ", ");
            countData++;
        }
        /*Check if the number of commas matches the number of numbers*/
        while (i< sourceLen)
        {
            if (source[i]==',')
            {
                countComma++;
            }
            i++;
        }
    }
    else
    {
        printError("No data after operation data");
    }
    free(source);
    if(countComma>=countData)
    {
        printError("There are unnecessary commas in the sentence");
    }
    /*add check if the first and last char are "" and check if there are more conditions in the instructors ^^^^^*/
    return 0;
}
/*Handles the ".string" directive, converting string characters to machine code.*/
int DirectiveString(char *sourceCode,cidodProp *progArr,int *L, int *ic, int *dc)
{
    int i, len = 0;
    char *ch;
    len = (int)strlen(sourceCode);


    for (i = 0; i < len ; i++)
    {
        if(sourceCode[i]=='\n'){
            printError("In the line \".string\" the word does not end with \"");
        }
        progArr[*L].code = sourceCode[i];
        progArr[*L].type = 0;
        (*L)++;
        (*dc)++;
    }
    ch= strtok(NULL," \"");
    progArr[*L].code = 0;
    progArr[*L].type = 0;
    (*L)++;
    (*dc)++;
    return 0;
}
/*Generates machine code for one-operand instructions*/
int oneOperand(int act, char *wwsline,LABEL **headToDecode,cidodProp *progArr,int *L,int *ic,int *dc)
{
    int operand_num_temp=0;
    short int coding = 0;
    Operand opr_target;
    coding = (act << 5);
    opr_target = oprandType(wwsline); /*oprandType(wwsline,&opr_tearget);*/
    if (opr_target.mion==1)
    {
        /*If there is an error and the type of addressing method does not match the type of operation, the row will not enter the coding table*/
        if ((act>3&&act<6)||(act>6&&act<12)||act==13)
        {
            printError("There is an addressing method that does not fit the type of operation");
            return 1;
        }
    }
    wwsline= strtok(NULL," \n\t ");
    if (wwsline!=NULL)
    {
        printError("In the input file there is a line with a directive that should have one operand but there are more");
    }
    operand_num_temp = opr_target.mion;
    coding = coding | (operand_num_temp << 2);
    progArr[*L].code = coding;
    progArr[*L].type = 1;
    (*L)++;
    (*ic)++;
    kidodTargetMion(opr_target,headToDecode,progArr,L,ic,dc);
    return 0;
}

/* Generates machine code for two-operand instructions.*/
int twoOperands(int act, char *wwsline,LABEL **headToDecode,cidodProp *progArr,int *L,int *ic, int *dc)
{
    int operand_num_temp;
    char *token, chOpsource[MAX_LABEL], chOptarget[MAX_LABEL], *start;
    short int coding = 0;
    Operand oprSource, oprTarget;
    token = strtok(wwsline, ",");
    if (token)
    {
        start = token;
        while (*start == ' ' || *start == '\t')
        {
            start++;
        }
        strcpy(chOpsource, start);
        token = strtok(NULL, ",");
        if (token)
        {
            /*Trim leading spaces and tabs from optarget*/
            start = token;
            while (*start == ' ' || *start == '\t')
            {
                start++;
            }
            strcpy(chOptarget, start);
            token= strtok(NULL,", ");
            if (token!=NULL)
            {
                printError("In the input file there is a line with a directive that should have two operand but there are more");
            }
        }
        else
        {
            strcpy(chOptarget, ""); /*Set optarget as an empty string if no second word is found*/
            printError("In the input file there is a line with a directive that should have two operand but there is less");
        }
    }
    else
    {
        printError("In the input file there is a line with a directive that should have two operand but there is less");
    }


    coding = coding | (act << 5);
    oprSource = oprandType(chOpsource);
    /*If there is an error and the type of addressing method does not match the type of operation, the row will not enter the coding table*/
    if (oprSource.mion!=3&&act==6)
    {
        printError("There is an addressing method that does not fit the type of operation");
        return 1;
    }
    operand_num_temp = oprSource.mion;
    coding = coding | (operand_num_temp << 9);
    oprTarget = oprandType(chOptarget);
    operand_num_temp = oprTarget.mion;
    if(act!=1&&operand_num_temp==1)
    {
        printError("There is an addressing method that does not fit the type of operation");
        return 1;
    }
    coding = coding | (operand_num_temp << 2);
    progArr[*L].code = coding;
    progArr[*L].type = 1;
    (*ic)++;
    (*L)++;
    cidodSourceMion(oprSource, oprTarget,headToDecode,progArr,L,ic,dc);
    return 0;
}

Operand oprandType(char *op)
{
    char* endptr,*opWS;/*opWS- op without spcaes*/
    Operand opr;
    int i=0, num1=0;
    opWS= strtok(op," \n\t");
    if (op[0] == '@')
    {
        if (strlen(opWS) != 3)
        {
            printError("error in operand type @r is too long");
        }
        if (op[1] == 'r')
        {
            if (op[2] >= '0' && op[2] <= '7') /*check lenght^^^^^^^^^^^*/
            {
                opr.num = op[2] - '0';
                opr.mion = 5;
                return opr;
            }
            else
            {
                printError(" register: The name of the register does not exist, registers between @r0 and @r7");
            }
        }
        else
        {
            printError(" register: name not exist");
        }
    }
        /*else if (atoi(op))*/ /*^^^^^^^^^^^^^^^^^^^^^^^^why isdigit not good*/
    else if ((op[0] >= '0' && op[0] <= '9') || op[0] == '-'||op[0]=='+')
    {
         num1 =  (int)strtoll(op, &endptr, 10);
        if (isspace(*endptr)|| strcmp(endptr,"") == 0) {
            opr.num=num1;
            opr.mion = 1;
        } else
        {
            printError("invalid number");
            opr.num=0;
            opr.mion = 1;
        }
        return opr;
    }
    else
    {
        opr.num = 0;
        opr.mion = 3;
        strcpy(opr.labelName, op);
        return opr;
    }
    return opr;
}


/*Determines the type of sentence (instruction or directive) and processes it accordingly.*/
/*Parameters: line - String containing the assembly code line.
head - Pointer to the label list.
headUnFind - Pointer to the label table for unresolved labels.
progArr - Array of kidodProp structs to store generated code.*/
int typeSen(char *line, LABEL **head,LABEL **headToDecode,cidodProp *progArr,int *L,int *ic,int *dc)
{
    char *wwsline, *token;
    short act;
    int countQue=0,i=0,lenWord;
    wwsline= strchr(line,':');
    if (wwsline)/*check if there is label by the check if there is : in the sentences*/
    {
        wwsline = strtok(line, ":");
        if (strlen(wwsline)<31)
        {
            if (actionArray( wwsline) == -1)
            {
                insertNode(head, *L + 100, wwsline, 0);
                token = wwsline;
                token = strtok(NULL, " \n");
            }
            else
            {
                printError("Label name can not be an action name");
            }

        }
        else
        {
            printError("The max lenght for label name is 31");
        }

    }
    else
    {
        token = strtok(line, " \n");
    }
    if(!token)
    {
        printError("Missing details");
    }

    act = actionArray(token);
    switch (act)
    {
        /*first group mov, cmp,add, sub, lea 0, 1, 2, 3,6 */
        case 0:
        case 1:
        case 2:
        case 3:
        case 6:
            wwsline = strtok(NULL, "\n");
            twoOperands(act, wwsline,headToDecode,progArr,L,ic,dc);
            break;
            /*second group not, clr, inc, jmp, bne, red, prn, jsr 4,5,7,9,10,11,12,13*/
        case 4:
        case 5:
        case 7:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            wwsline = strtok(NULL, " ");
            oneOperand(act, wwsline,headToDecode,progArr,L,ic,dc);
            break;
        case 8:
        case 14:
        case 15:
            progArr[*L].code = 0|(act << 5);
            progArr[*L].type = 1;
            (*ic)++;
            (*L)++;
            break;
        case 16:
            token = strtok(NULL, "\n");
            if(token==NULL)
            {
                printError("There is no word after the action .string");
            }
            else
            {
                lenWord= strlen(token);
                for(i=0;i<lenWord;i++)
                {
                    if (token[i]=='\"')
                    {
                        countQue++;
                    }
                }
                if (countQue>2)
                {
                    printError("The line format \".string\" does not match its definition, there are too many quotation marks");
                }
                else if(token&&token[0]!='\"')
                {
                    printError("Error inputting the .string command, the word does not begin with \"");
                }
                else
                {
                    wwsline = strtok(token, "\"");
                    DirectiveString(wwsline,progArr,L,ic,dc);
                }
            }
            break;
        case 17:
            wwsline = strtok(NULL, " ");
            DirectiveData(wwsline,progArr,L,ic,dc);
            break;
        case 18:
            externFlag=1;
            break;
        case 19:
            entryFlag=1;
            wwsline = strtok(NULL, " ");
            break;

        default:
            printError("The input has an undefined instruction");
            break;
    }
    return 1;
}

/*check when the number of the action*/
short actionArray(char *ch)
{
    char *actArr[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop", ".string", ".data", ".extern", ".entry"};
    short i = 0;
    for (i = 0; i < sizeof(actArr) / sizeof(char *); i++)
    {
        if (!strcmp(ch, actArr[i]))
        {
            return i;
        }
    }

    return -1;
}

/*Performs the first pass of the assembler, processing the assembly source code and generating the label and code arrays.
Parameters: fileName - Name of the source code file.*/
int firstppass(char *fileName)
{
    int ic=0, L=0, dc=0;
    LABEL *headToDecode=NULL;/*label that need to get*/
    LABEL *head=NULL;
    char *strError,*copytext,line[MAX_LINE];
    int isLenght=0,i=0;
    char i_str[20];
    FILE *copy;
    char *copyName = stringTwoStrings(fileName, ".am");
    cidodProp progArr[MEMORY_ARRAY_SIZE];
    copy = fopen(copyName, "r");
    while (fgets(line, sizeof(line), copy) != NULL)
    {

        for (i = 0; i < MAX_LINE; ++i)
        {
            if ( line[i] =='\n')
            {
                isLenght=1;
                break;
            }
        }
        if (isLenght)
        {
            typeSen(line, &head,&headToDecode,progArr,&L,&ic,&dc);
        }
        else
        {
            sprintf(i_str, "%d", i);
            strError= stringTwoStrings("line: ",i_str);
            strError= stringTwoStrings(strError," is too long, the max is 81 chars per line");
            printError( strError);
        }

    }
    printarray(head, progArr,L);
    printarray(headToDecode,progArr,L);
    if(!errorFirst)
    {
        secondPass(&head,&headToDecode,fileName,L,progArr);
    }
    return 0;
}
