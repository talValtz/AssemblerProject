
#include "firstPass.h"
#include "program.h"

int errorFirst=0;/*global variable the defined if there are any error int the first pass 0- there are no 1-there are*/


int ic, L, dc;

void printError(char *printText){
    printf("\033[0;31m"); /* Red */
    printf("invalid : %s",printText);
    printf("\033[0m\n");
    errorFirst=1;

}
void printBin(int x)
{
    unsigned int mask = 1<<11;
   /* unsigned int mask = 1 << (sizeof(short int) * 8 - 1);*/
    while (mask != 0)
    {
        if (x & mask)
            putchar('1');
        else
            putchar('0');

        mask = mask >> 1;
    }
}

void printarray(LABEL *head)
{
    LABEL *temp =head;
    int i;
    printf("\nthe code image:\n");
    printf("binarry code      type\n");
    for (i = 0; i < L; i++)
    {
        printf("%d: ", i);
        printBin(progArr[i].code);
        printf("  ,%d", progArr[i].type);
        printf("\n");
    }
    printf("\nthe label     table:      type:  \n");
    while(temp!=NULL){
        printf("%s           %d       %d\n", temp->labelName, temp->lineNum,temp->type);
        temp = temp->next;

    }
    /*if (head != NULL) {
        do {
            printf("%s           %d       %d\n", temp->labelName, temp->lineNum,temp->type);
            temp = temp->next;
        } while (temp != head);
    }*/
    printf("\n");
}



/*check if the label name is already exist in the label linked list if yes return 1 else return 0*/
int checkExistLabel(LABEL **head,char *Name){
    LABEL *temp=*head;
    while(temp!=NULL){
        if (strcmp(temp->labelName,Name) == 0)
            return 1;
        temp=temp->next;
    }
    return 0;
}

void insertNode2(LABEL ** head, int lineNum,  char *labelName, int type) {

    LABEL* newLabel = (LABEL*)malloc(sizeof(LABEL));
    LABEL * cur=*head;
    char *ezer;
    int i=0;
    newLabel->lineNum = lineNum;
    ezer = (char*)malloc(strlen(labelName) + 1);
    for(i=i;i< strlen(labelName)-1;i++){
        ezer[i]= labelName[i];
    }
    ezer[i]='\0';
    /*strncpy(newLabel->labelName, labelName, sizeof(newLabel->labelName) - 1);*/
    strcpy(newLabel->labelName, ezer);
    newLabel->labelName[sizeof(newLabel->labelName) - 1] = '\0'; /*Ensure null-termination-check it with miki*/
    newLabel->type = type;
    newLabel->next = NULL;
        if (*head == NULL) {
            *head = newLabel;
        } else {
            while (cur->next != NULL) {
                cur = cur->next;
            }
            cur->next = newLabel;
        }
}

void insertNode(LABEL ** head, int lineNum,  char *labelName, int type) {

    LABEL* newLabel = (LABEL*)malloc(sizeof(LABEL));
    LABEL * cur=*head;
    newLabel->lineNum = lineNum;
    strncpy(newLabel->labelName, labelName, sizeof(newLabel->labelName) - 1);
    newLabel->labelName[sizeof(newLabel->labelName) - 1] = '\0'; /*Ensure null-termination-check it with miki*/
    newLabel->type = type;
    newLabel->next = NULL;


    if (checkExistLabel(head, labelName)){
        printError("The label  is already exist");
    }
    else{
        if (*head == NULL) {
            *head = newLabel;

        } else {

            while (cur->next != NULL) {
                cur = cur->next;
            }
            cur->next = newLabel;
        }

    }
}

short int kidodSourceMion(Operand source, Operand target,LABEL **headUnFind) /*change to pointer^^^^^^^^^^*/
{
    short  kidod = 0;


    if (source.mion == 5)
    {
        kidod = kidod | (source.num << 7);
        progArr[L].code = kidod;
        progArr[L].type = 1;
        ic++;
        L++;
    }
    else if (source.mion == 1)
    {
        kidod = kidod | (source.num << 2);
        ic++;
        L++;
    }
    else if (source.mion == 3)
    {
        ic++;
        L++;
    }
    if (target.mion == 5)
    {
        if (source.mion == 5)
        {
            kidod = kidod | (target.num << 2);
            progArr[L - 1].code = kidod;
        }
        else
        {
            kidod = 0;
            kidod = kidod | (target.num << 2);
            progArr[L].code = kidod;
            progArr[L].type = 1;

            ic++;
            L++;
        }
    }
    else if (target.mion == 1)
    {
        kidod = 0;
        kidod = kidod | (target.num << 2);
        progArr[L].code = kidod;
        progArr[L].type = 1;
        ic++;
        L++;
    }
    else if (target.mion == 3)
    {
        kidod = 0;
        progArr[L].code = kidod;
        progArr[L].type = 1;
        insertNode2(headUnFind,L,target.labelName,0);
        ic++;
        L++;
    }

    return kidod;
}

short int kidodTargetMion(Operand target,LABEL **headUnFind)
{
    /*combine with the source function, think about the source when its null*/
    short int kidod = 0;
    if (target.mion == 5 || target.mion == 1)
    {
        kidod = kidod | (target.num << 2);
    }
    else if(target.mion==3){
        insertNode2(headUnFind,L,target.labelName,0);
    }
    progArr[L].code = kidod;
    progArr[L].type = 1;
    ic++;
    L++;
    return kidod;
}

int DirectiveData(char *sourceCode)
{
    char *dataNum;
    dataNum= strtok(sourceCode,", ");

    char *ptr;
    int num,countChar=0,i=0;
    /*token=strtok(0," ");*/
    if (dataNum)
    {
        while (dataNum)
        {
            printf("token in brefore Dirctive data function is : %s|\n", dataNum);
            num = (int)strtol(dataNum, &ptr, 10);
            /*check if there are some char that are not integer in the dataNum */
            i=0;
            while(i< strlen(ptr)&&ptr[i]!=','){
                printf("ptr[i] is: %c|",ptr[i]);
                if(!isspace(ptr[i])&&(ptr[i]!=',')){
                    countChar++;
                }
                i++;
            }

            if(!countChar)
            {
                printf("The number(unsigned long integer) is %d\n", num);
                progArr[L].code=num;
                progArr[L].type=0;
                L++;
                dc++;
            }
            else{
                /*error that there are some chars instead of integer^^^^^^^^^^^^*/
            }
            dataNum = strtok(NULL, ", ");
            printf("token in afterDirctive data function is : %s|\n", dataNum);
            printf("String part is |%s|", ptr);
        }
    }
    else
    {
        /*error about null witch mean that there are no data^^^^^^^^^^ */
    }

    /*add check if the first and last char are "" and check if there are more conditions in the instructors ^^^^^*/
    printf("wwsline in Dirctive data function is : %s|\n", sourceCode);
    printf("token in Dirctive data function is : %s|\n", dataNum);
    return 0;
}
int DirectiveString(char *sourceCode)
{
    int i, len = 0;
    len = (int)strlen(sourceCode);
    /*add check if the first and last char are "" and check if there are more conditions in the instructors ^^^^^*/
    printf("wwsline in Dirctive string function is : %s\n", sourceCode);
    for (i = 1; i < len - 2; i++)
    {
        printf("source code binary ^^^^^^^\n");
        printBin(sourceCode[i]);
        printf("\n");
        printf("counterLine : %d    char is: %c^^^^^^^\n", L, sourceCode[i]);
        progArr[L].code = sourceCode[i];
        progArr[L].type = 0;
        L++;
        dc++;
    }
    progArr[L].code = 0;
    progArr[L].type = 0;
    L++;
    dc++;
    return 0;
}
int oneOperand(int act, char *wwsline,LABEL **headUnFind)
{
    int operand_num_temp;
    short int kidod = 0;
    Operand opr_target;
    printf(" wwsline in oneOperand function:%s\n", wwsline);
    /*wwsline = strtok(NULL, " ");*/
    printf(" wwsline in oneOperand function:  %s\n", wwsline);
    kidod = (act << 5);
    opr_target = oprandType(wwsline);
    operand_num_temp = opr_target.mion;
    printf(" operand_num_temp22 %d\n", operand_num_temp);
    printf(" optarget %s\n", wwsline);
    kidod = kidod | (operand_num_temp << 2);
    progArr[L].code = kidod;
    progArr[L].type = 1;

    ic++;
    L++;
    printf("\nkidod: ");
    printBin(kidod);
    printf("\n");
    printBin(kidodTargetMion(opr_target,headUnFind));
    return 0;
}

int twoOperands(int act, char *wwsline,LABEL **headUnFind)
{
    int operand_num_temp;
    char *token, opsource[MAX_LABEL], optarget[MAX_LABEL], *start;
    short int kidod = 0;
    Operand opr_source, opr_target;
    token = strtok(wwsline, ",");
    printf(" token %s\n", token);
    if (token)
    {
        start = token;
        while (*start == ' ' || *start == '\t')
        {
            start++;
        }
        strcpy(opsource, start);
        printf("opsource: %s\n", opsource);

        token = strtok(NULL, ",");
        printf("Token: %s\n", token);
        if (token)
        {
            /*Trim leading spaces and tabs from optarget*/
            start = token;
            while (*start == ' ' || *start == '\t')
            {
                start++;
            }
            strcpy(optarget, start);
            printf("optarget: %s\n", optarget);
        }
        else
        {
            strcpy(optarget, ""); /*Set optarget as an empty string if no second word is found*/
        }
    }

    printf(" opsource %s\n", opsource);
    printf(" optarget %s\n", optarget);

    printf(" wwsline!!! %s\n", wwsline);
    /*opsource = strtok(wwsline, ",");*/
    kidod = kidod | (act << 5);
    opr_source = oprandType(opsource);
    operand_num_temp = opr_source.mion;
    kidod = kidod | (operand_num_temp << 9);

    printf(" act!!! %d\n", act);

    /*if opsource is null thien errorrrr! */
    printf(" opsource %s\n", opsource);
    printf(" operand_num_temp %d\n", operand_num_temp);
    /*optarget = strtok(NULL, "/n");*/
    opr_target = oprandType(optarget);
    operand_num_temp = opr_target.mion;
    printf(" operand_num_temp22 %d\n", operand_num_temp);
    printf(" optarget %s\n", optarget);
    kidod = kidod | (operand_num_temp << 2);
    progArr[L].code = kidod;
    progArr[L].type = 1;
    ic++;
    L++;
    printf("\nkidod: ");
    printBin(kidod);
    printf("\n");
    printf("bin kidod:\n");
    printBin(kidodSourceMion(opr_source, opr_target,headUnFind));

    /*if optarget is null thien errorrrr! */
    return 0;
}
Operand oprandType(char *op)
{
    char* endptr;
    Operand opr;/*check with miki if is it should be with malloc to with null to restart value */
    int i=0, num1=0;

    /*printf("\n%lu\n", op[0]);*/
    if (op[0] == '@')
    {
        if (strlen(op) != 4)
        {
            printf("|%s| error in operand input %lu\n",op, strlen(op));
            printf("error in operand input\n");
        }
        printf("\n@@@@\n");
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
                /*put into error text*/
                printf("\033[0;31m"); /* Red */
                printf("invalid register: name not exist");
                printf("\033[0m\n");
            }
        }
        else
        {
            /*put into error text*/
            printf("\033[0;31m"); /* Red */
            printf("invalid register: name not exist");
            printf("\033[0m\n");
        }
    }
        /*else if (atoi(op))*/ /*^^^^^^^^^^^^^^^^^^^^^^^^why isdigit not good*/
    else if ((op[0] >= '0' && op[0] <= '9') || op[0] == '-'||op[0]=='+')
    {


         num1 =  (int)strtoll(op, &endptr, 10);
        if (isspace(*endptr)) {
            opr.num=num1;
            opr.mion = 1;
        } else {
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


/*check the suntence type Insruction, derctive, and what tro do next*/
int typeSen(char *line, LABEL **head,LABEL **headUnFind)
{

    char *wwsline;
    char *token;
    short act;
    LABEL *lblNode=(LABEL*) malloc(sizeof (LABEL));
    wwsline= strchr(line,':');

    if (wwsline)/*check if there is label by the check if there is : in the sentences*/
    {
        wwsline = strtok(line, ":");
        printf(" wwsline in if %s\n", wwsline);
        insertNode(head,L+100,wwsline,0);
        printf("%s    %d\n", lblNode->labelName,  lblNode->lineNum);
        printf("Line in if : %s  \nwwsline: %s\n", line, wwsline);
        token = wwsline;
        token = strtok(NULL, " \n");
    }
    else
    {
        printf(" wwsline is NULL\n");
        token = strtok(line, " ");
    }
    if (token)
    {
        printf(" tokennnnnnn22: %s\n", token);
    }
    else
    {
        printf(" tokennnnnnn22 is null\n");
    }
    /*if token is null then error ^^^^^^^^^^^^^^^^^^^*/
    act = actionArray(token);
    /*act = act << 5;*/
    printf(" act number : %d\n", act);

    printf("  wwsline22222: %s\n", wwsline);
    printBin(act);
    printf("\n");
    switch (act)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 6:
            wwsline = strtok(NULL, "\n");
            twoOperands(act, wwsline,headUnFind);
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

            oneOperand(act, wwsline,headUnFind);

            break;
        case 8:
        case 14:
        case 15:
            break;
        case 16:
            wwsline = strtok(NULL, " ");
            printf("  wwsline3333: %s\n", wwsline);
            DirectiveString(wwsline);
            break;

        case 17:
            wwsline = strtok(NULL, " ");
            printf(" wwsline44: %s\n", wwsline);
            DirectiveData(wwsline);
            break;
        case 18:
            externFlag=1;
            break;
        case 19:
            entryFlag=1;
            wwsline = strtok(NULL, " ");
          /*  labelDefinition(act, wwsline, (LABEL *) head);*/
            break;

        default:

            printf("\033[0;31m"); /* Red */
            printf("invalid\n");
            printf("\033[0m\n");
            break;
    }
    /*if(act==17||act==16){

    }
    else if(act<16){

    }
    else{
        fprintf(stderr, "invalid");
    }*/
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

/*read line after line*/
int firstppass(char *fileName)
{
    LABEL *headUnfind=NULL;/*label that need to get*/
    LABEL *head=NULL;
    char *strError;
    char line[MAX_LABEL];
    int lenght;
    int isLenght=0,i=0;
    char i_str[20]; // Assuming the number can fit in 20 characters

    FILE *copy;
    copy = fopen("textfile.copytxt", "r");
    while (fgets(line, sizeof(line), copy) != NULL)
    {
        for (i = 0; i < MAX_LABEL; ++i) {
            if ( line[i] =='\n'){
                isLenght=1;
                break;
            }
        }
        if (isLenght){
            typeSen(line, &head,&headUnfind);
        }
        else
        {
            sprintf(i_str, "%d", i);
            strError= str_allocate_cat("line: ",i_str);
            strError= str_allocate_cat(strError," is too long, the max is 81 chars per line");
            printError( strError);
        }

    }
    printarray(head);
    printarray(headUnfind);
    lenght=L;
    if(!errorFirst){
        secondPass(&head,&headUnfind,fileName,lenght);
    }

    return 0;
}
