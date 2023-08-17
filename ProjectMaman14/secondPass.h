//
// Created by Tal Valtzer on 05/08/2023.
//

#ifndef __SECONDPASS__
#define __SECONDPASS__

#include "labels.h"

void  completeTabel(LABEL **head,LABEL **headToDecode,int L,cidodProp  *progArr);
int secondPass(LABEL **head,LABEL **headToDecode,char *fileName,int L,cidodProp *progArr);
#endif //PROJECTMAMAN14_SECONDPASS_H
