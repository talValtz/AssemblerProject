//
// Created by Tal Valtzer on 02/08/2023.
//

#include "labels.h"


char *str_allocate_cat(char *first_str, char *second_str)
{
    char *str = (char *)malloc(strlen(first_str) + strlen(second_str) + 1);
    strcpy(str, first_str);
    strcat(str, second_str);
    return str;
}
