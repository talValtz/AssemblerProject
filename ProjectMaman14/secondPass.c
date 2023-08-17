//
// Created by Tal Valtzer on 05/08/2023.
//
#include "secondPass.h"

void  freeNodes(LABEL **head,LABEL **headToDecode)
{
    LABEL *cur=*head;
    while (*head != NULL)
    {
         cur = *head;
        *head = (*head)->next;
        free(cur);
    }
    while (*headToDecode != NULL)
    {
        cur = *headToDecode;
        *headToDecode = (*headToDecode)->next;
        free(cur);
    }
}
void  completeTabel(LABEL **head,LABEL **headToDecode,int L,cidodProp  *progArr)
{
    LABEL *curToDecode=*headToDecode,*cur=*head;
    unsigned int mask = 1 << (sizeof(short int) * 8 - 1);
    int i=0;
    while(curToDecode!=NULL)
    {
        cur=*head;
        while(cur!=NULL)
        {
            if(!strcmp(cur->labelName,curToDecode->labelName))
            {
                if(curToDecode->type!=2)
                {
                    progArr[curToDecode->lineNum].code=2|((cur->lineNum)<<2);
                    break;
                }

            }
            cur=cur->next;
        }
        curToDecode=curToDecode->next;
    }
    printf("the final table is: \n");

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


void entryFunction(LABEL **head,LABEL **headToDecode,char* line,FILE *ent)
{
    unsigned int mask = 1 << (sizeof(short int) * 8 - 1);
    char *token,number_str[20];;
    int i=0,tokenLen=strlen(token);
    LABEL *temp =*head;
    token= strtok(line," ");
    while (strncmp(token, ".entry", strlen(".entry")) == 0)
    {
        token= strtok(NULL," ");
    }
    token= strtok(token," \n");
    while(temp!=NULL)
    {
        if(!strncmp(temp->labelName,token,tokenLen))
        {
            i=0;
            temp->type=1;
            snprintf(number_str, sizeof(number_str), "%d", temp->lineNum);
            while (i < tokenLen-1)
            {
                fputc(token[i], ent);
                i++;
            }
            fputs(" ",ent);
            for (i = 0; number_str[i] != '\0'; i++)
            {
                fputc(number_str[i], ent);
            }
            fputs("\n",ent);
        }
        temp = temp->next;
    }

}

void externFunction(LABEL **head,char* line,FILE *ext,cidodProp *progArr)
{
    unsigned int mask = 1 << (sizeof(short int) * 8 - 1);
    char *token,number_str[20];;
    int i=0,tokenLen= strlen(token),extLen=strlen(".extern");
    LABEL *temp =*head;
    token= strtok(line," ");
    while(!strncmp(token,".extern",extLen))
    {
        token= strtok(NULL," ");
    }
    token=strtok(token," \n");
    while(temp!=NULL)
    {
       if(!strncmp(temp->labelName,token,tokenLen))
       {
           i=0;
           temp->type=2;
           snprintf(number_str, sizeof(number_str), "%d", ((temp->lineNum)+100));
           progArr[temp->lineNum].code=1;
           while (i < tokenLen-1)
           {
                   fputc(token[i], ext);
               i++;
           }
           fputs(" ",ext);
           for (i = 0; number_str[i] != '\0'; i++)
           {
                 fputc(number_str[i], ext);
           }
           fputs("\n",ext);

       }
        temp = temp->next;

    }

}
char printchar(char ch)
{
    char newCh;
    if(ch<=25)
    {
        newCh='A'+ch;
    }
    else if(ch<=51)
    {
        newCh='a'+ch-26;
    }
    else if(ch<=61)
    {
        newCh='0'+ch-52;
    }
    else if(ch==62)
    {
        newCh='+';
    }
    else
    {
        newCh='/';
    }
    printf("%c",newCh);
    return newCh;

}
/*Executes the second pass of the assembler, processing the .entry and .extern*/
int secondPass(LABEL **head,LABEL **headToDecode,char *fileName,int L,cidodProp *progArr)
{
    int i,errorSecond=0;/* variable the defined if there are any error int the second pass 0- there are no 1-there are*/;
    char line[82]="";
    FILE *copy,*ent=NULL,*ext=NULL,*ob;
    unsigned int mask = 1 << (sizeof(short int) * 8 - 1);
    char *entC= stringTwoStrings(fileName, ".ent");
    char *extC = stringTwoStrings(fileName, ".ext");
    char *obC = stringTwoStrings(fileName, ".ob");
    char *copyName = stringTwoStrings(fileName, ".am");
    copy = fopen(copyName, "r");
    /*Open the files only and there is a statement*/
    if(externFlag){
        ext= fopen(extC,"w");
        if (ext == NULL)
        {
            printf("Error opening extern  file\n");
            return 1;
        }
    }

    if(entryFlag)
    {
        ent=fopen(entC,"w");
        if (ext == NULL)
        {
            printf("Error opening entry file\n");
            return 1;
        }

    }
    while (fgets(line, sizeof(line), copy) != NULL)
    {
        if(strstr(line,".extern"))
        {
            externFunction(headToDecode,line,ext,progArr);
        }
        if(strstr(line,".entry"))
        {
            entryFunction(head,headToDecode,line,ent);
        }

    }
    completeTabel(head,headToDecode,L,progArr);
    freeNodes(head,headToDecode);
    ob= fopen(obC, "w");
    for (i = 0;  i<L ; i++)
    {
        if (progArr[i].type==1)
        {
            fputc(printchar((progArr[i].code>>6)&0x3f), ob);
            fputc(printchar((progArr[i].code)&0x3f), ob);
            fputc('\n',ob);

        }
    }
    for (i = 0;  i<L ; i++)
    {
        if (progArr[i].type==0)
        {
            fputc(printchar((progArr[i].code>>6)&0x3f), ob);
            fputc(printchar((progArr[i].code)&0x3f), ob);
            fputc('\n',ob);
        }
    }
    if (ext != NULL)
    {
        fclose(ext);
    }
    if(ent!=NULL)
    {
        fclose(ent);
    }


    fclose(copy);
    fclose(ob);

    return 1;
}

