//
// Created by Tal Valtzer on 05/08/2023.
//

#ifndef __SECONDPASS__
#define __SECONDPASS__

#include "labels.h"

void labelDefinition(int act, char *label, LABEL *head);
int secondPass(LABEL **head,LABEL **headUnfind,char *fileName,int L);
#endif //PROJECTMAMAN14_SECONDPASS_H
