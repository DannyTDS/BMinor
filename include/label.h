#ifndef LABEL_H
#define LABEL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Labels: used to mark jpl addresses of jump instructions */
int label_create();     // create a label
const char* label_name(int label);     // return literal label name

#endif