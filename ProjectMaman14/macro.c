#include "macro.h"
#include "firstPass.h"

char *str_allocate_cat(char *first_str, char *second_str)
{
    char *str = (char *)malloc(strlen(first_str) + strlen(second_str) + 1);
    strcpy(str, first_str);
    strcat(str, second_str);
    return str;
}

int copyContentToFile(Macro *cur, FILE *cpy)
{
    int i = 0;
    printf("%lu\n", strlen(cur->content));
    fputs(cur->content, cpy);
    /*while (i < strlen(cur->content))
    {

        if (!isspace(cur->content[i]))
        {
            fputc(cur->content[i], cpy);
        }
        if (cur->content[i] == '\n')
        {
            fputc(cur->content[i], cpy);
        }
        i++;
    }*/
    return 0;
}

/*check if line in th einput file the compare to marco exist name*/
int checkMacroName(Macro *head, char line[], FILE *cpy)
{
    Macro *cur = head;
    int i = 0;
    int j;
    while (isspace(line[i]))
    {
        i++;
    }
    j = i;
    while (!isspace(line[j]))
    {
        j++;
    }
    while (cur != NULL)
    {
        if (!strncmp(&line[i], cur->name, j - i))
        {

            copyContentToFile(cur, cpy);
            return 1;
        }
        cur = cur->next;
    }
    return 0;
}

/*function to check if we are iun marco or in the end*/
int checkInMacro(char line[], Macro **head)
{
    char macroName[100];
    Macro *temp = (Macro *)malloc(sizeof(Macro));
    int j;
    int i = 0;
    while (isspace(line[i]))
        i++;
    if (strncmp(&line[i], "mcro", 4) == 0)
    {
        i += 4;
        while (isspace(line[i]))
            i++;
        j = 0;
        while (!isspace(line[i]) && line[i] != '\0')
        {
            macroName[j] = line[i];
            i++;
            j++;
        }
        macroName[j] = '\0';
        printf("The macro name is: %s\n", macroName);
    }
    else
    {
        if (strncmp(&line[i], "endmcro", 7) == 0)
        {
            return 2;
        }

        printf("The line does not contain a valid macro.\n");
    }
    strcpy(temp->name, macroName);
    temp->next = NULL;
    if (*head == NULL)
    {
        *head = temp;
    }
    else
    {
        Macro *last = *head;
        while (last->next != NULL)
        {
            last = last->next;
        }
        last->next = temp;
    }

    return 1;
}

int preProcess(char *fileName) {

    FILE *file, *copy;
    char line[100] = "";
    int isEmptyLine;
    int isInMacro = 0;
    int i;
    Macro *head = NULL;
    char *origin = str_allocate_cat(fileName, ".txt");
    char *copyName = str_allocate_cat(fileName, ".copytxt");

    file = fopen(origin, "r");
    copy = fopen(copyName, "w");
    printf("%s\n", origin);
    printf("%s", copyName);
    if (file == NULL) {
        printf("Error opening files\n");
        return 1;
    }


        while (fgets(line, sizeof(line), file) != NULL)
        {
            if (isInMacro == 2)
            {
                isInMacro = 0;
            }
            else if (isInMacro == 1)
            {
                checkMacroName(head, line, copy);
            }
            if (strncmp(line, "endmcro", 7) == 0)
            {
                printf(":) :(");
                isInMacro=2;
            }

            if (strstr(line, "mcro") != NULL)
            {
                printf("The word '%s' is found in the line: %s", "mcro", line);
                isInMacro = checkInMacro(line, &head);
               /* fgets(line, sizeof(line), file);*/
            }
            /*  printf("%lu\n", strspn(line, "mcro"));*/
            if(isInMacro!=2){
                isEmptyLine = 1;
                for (i = 0; i < strlen(line); i++)
                {
                    if (!isspace(line[i]))
                    {
                        if (line[i] != ';')
                        {

                            isEmptyLine = 0;
                        }
                        break;
                    }
                }
                if (!isEmptyLine && !isInMacro)
                {
                    if (head != NULL)
                    {
                        if (!checkMacroName(head, line, copy))
                        {
                            fputs(line, copy);
                        }
                    }
                    else
                    {
                        fputs(line, copy);
                    }
                }
                if (isInMacro == 1)
                {
                    strncat(head->content, line, strlen(line));

                    printf("The con  is found: %s", head->content);
                }
            }

        }


    fclose(file);
    fclose(copy);
    firstppass(fileName);

    return 0;
}
