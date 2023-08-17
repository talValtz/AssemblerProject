#include "macro.h"
#include "firstPass.h"
/*Allocates memory and concatenates two strings.*/
char *stringTwoStrings(char *first_str, char *second_str)
{
    char *str = (char *)malloc(strlen(first_str) + strlen(second_str) + 1);
    strcpy(str, first_str);
    strcat(str, second_str);
    return str;
}
/*Copies the content of a Macro structure to a specified file.*/
int copyContentToFile(Macro *cur, FILE *cpy)
{
    int i = 0;
    fputs(cur->content, cpy);

    return 0;
}

/* * Checks if a line in the input file contains a macro name and replaces it with the corresponding macro content.*/
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

/*function to check if we are in marco or in the end, Checks if the current line is part of a macro definition and stores the macro name for later processing.
*/
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
    }
    else
    {
        if (strncmp(&line[i], "endmcro", 7) == 0)
        {
            return 2;
        }

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


/*Performs macro processing on an input assembly code file.
  It identifies macro definitions, replaces their occurrences, and then performs the first pass on the processed code.*/
int preProcess(char *fileName) {

    FILE *file, *copy;
    char line[100] = "";
    int isEmptyLine;
    int isInMacro = 0;
    int i;
    Macro *head = NULL;
    char *origin = stringTwoStrings(fileName, ".as");
    char *copyName = stringTwoStrings(fileName, ".am");

    file = fopen(origin, "r");
    copy = fopen(copyName, "w");

    if (file == NULL) {
        return 1;
    }


        while (fgets(line, sizeof(line), file) != NULL)
        {
            if (isInMacro == 2)/*if isInMacro its mean that its the end of macro*/
            {
                isInMacro = 0;
            }
            else if (isInMacro == 1)
            {
                checkMacroName(head, line, copy);
            }
            if (strncmp(line, "endmcro", 7) == 0)
            {
                isInMacro=2;
            }
            if (isInMacro == 1)
            {
                strncat(head->content, line, strlen(line));
            }

            if (strstr(line, "mcro") != NULL)
            {
                isInMacro = checkInMacro(line, &head);
            }
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
            }
        }


    fclose(file);
    fclose(copy);
    firstppass(fileName);

    return 0;
}
