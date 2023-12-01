#include "label.h"

int label_cnt = 0;

int label_create() {
    /* Keeps updating a global label counter */
    return label_cnt++;
}

const char* label_name(int label) {
    char name[BUFSIZ];
    sprintf(name, ".L%d", label);
    return strdup(name);
}