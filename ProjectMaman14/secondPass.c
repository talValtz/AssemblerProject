//
// Created by Tal Valtzer on 05/08/2023.
//
#include "secondPass.h"

void  completeTable(LABEL **head,LABEL **headUnfind,int L){
    LABEL *curUnfind=*headUnfind,*cur=*head;
    unsigned int mask = 1 << (sizeof(short int) * 8 - 1);
    int i=0;
    while(curUnfind!=NULL){
        cur=*head;
        while(cur!=NULL){
            if(!strcmp(cur->labelName,curUnfind->labelName)){
                printf("cur:|%s| curUnfind: |%s|\n",cur->labelName,curUnfind->labelName);
                if(curUnfind->type!=2){
                    progArr[curUnfind->lineNum].code=2|((cur->lineNum)<<2);
                    printf("kidod: %d\n",progArr[curUnfind->lineNum].code);
                }

            }
            cur=cur->next;
        }
        curUnfind=curUnfind->next;
    }

    for (i = 0; i < L; i++)
{
mask = 1 << 11;
printf("%d: ", i);
while (mask != 0)
{
if (progArr[i].code & mask)
putchar('1');
else
putchar('0');

mask = mask >> 1;
}
printf("  ,%d", progArr[i].type);
printf("\n");
}


}


void entryFunction(LABEL **head,LABEL **headUnfind,char* line,FILE *ent)
{
    unsigned int mask = 1 << (sizeof(short int) * 8 - 1);
    char *token,*ezer = NULL,number_str[20];;
    int i=0;
    LABEL *temp =*head;
    token= strtok(line," ");
    while(!strcmp(token,".entry")){
        token= strtok(NULL," ");
    }
    ezer = (char*)malloc(strlen(token) + 1);
    for(i=i;i< strlen(token)-1;i++){
        ezer[i]= token[i];
    }
    ezer[i]='\0';
    while(temp!=NULL){
        if(!strcmp(temp->labelName,ezer)){
            i=0;
            temp->type=1;

            snprintf(number_str, sizeof(number_str), "%d", temp->lineNum);


            while (i < strlen(token)-1)
            {
                fputc(token[i], ent);
                i++;
            }
            fputs(" ",ent);
            for (i = 0; number_str[i] != '\0'; i++) {
                fputc(number_str[i], ent);
            }
            fputs("\n",ent);

        }
        temp = temp->next;

    }

    free(ezer);

}

void externFunction(LABEL **head,char* line,FILE *ext)
{
    unsigned int mask = 1 << (sizeof(short int) * 8 - 1);
    char *token,*ezer = NULL,number_str[20];;
    int i=0;
    LABEL *temp =*head;
    token= strtok(line," ");
    while(!strcmp(token,".extern")){
        token= strtok(NULL," ");
    }
    ezer = (char*)malloc(strlen(token) + 1);
    for(i=i;i< strlen(token)-1;i++){
        ezer[i]= token[i];
    }
    ezer[i]='\0';



    while(temp!=NULL){
       if(!strcmp(temp->labelName,ezer)){
           i=0;
           temp->type=2;

           snprintf(number_str, sizeof(number_str), "%d", ((temp->lineNum)+100));
           progArr[temp->lineNum].code=1;

           while (i < strlen(token)-1)
           {
                   fputc(token[i], ext);
               i++;
           }
           fputs(" ",ext);
           for (i = 0; number_str[i] != '\0'; i++) {
               fputc(number_str[i], ext);
           }
           fputs("\n",ext);

       }
        temp = temp->next;

    }

    /*free(ezer);*/

}
void printchar(char ch){
    char newCh;
    if(ch<=25){
        newCh='A'+ch;
    }
    else if(ch<=51){
        newCh='a'+ch-26;
    }
    else if(ch<=61){
        newCh='0'+ch-51;

    }
    else if(ch==62)
    {
        newCh='+';
    }
    else{
        newCh='/';
    }
    printf("%c",newCh);

}
int secondPass(LABEL **head,LABEL **headUnfind,char *fileName,int L)
{
    int i;
    char line[82]="";
    FILE *copy,*ent,*ext;
    char *entC= str_allocate_cat(fileName, ".ent");
    char *extC = str_allocate_cat(fileName, ".ext");
    char *copyName = str_allocate_cat(fileName, ".copytxt");
    copy = fopen(copyName, "r");


    if(externFlag){
        ext= fopen(extC,"w");
    }
    if(entryFlag){
        ent=fopen(entC,"w");

    }
    while (fgets(line, sizeof(line), copy) != NULL)
    {
        if(strstr(line,".extern")){
            printf("in if :)");
            externFunction(headUnfind,line,ext);
        }
        if(strstr(line,".entry")){
            entryFunction(head,headUnfind,line,ent);
        }

    }
   completeTable(head,headUnfind,L);
    for (i = 0;  i<L ; i++) {
        if (progArr[i].type==1){
            printchar((progArr[i].code>>6)&0x3f);/*0x3f*/
            printchar(progArr[i].code&0x3f);/*0x3f*/
            printf("\n");
        }
    }
    printf("---------");
    for (i = 0;  i<L ; i++) {
        if (progArr[i].type==0){
            printchar((progArr[i].code>>6)&0x3f);/*0x3f*/
            printchar(progArr[i].code&0x3f);/*0x3f*/
            printf("\n");
        }
    }






    return 1;
}

/*for (i = 0; i < 26; i++)
{
mask = 1 << (sizeof(short int) * 8 - 1);
printf("%d: ", i);
while (mask != 0)
{
if (progArr[i].code & mask)
putchar('1');
else
putchar('0');

mask = mask >> 1;
}
printf("  ,%d", progArr[i].type);
printf("\n");
}*/