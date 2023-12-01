#include "scratch.h"
#include "utils.h"

int NREG = 7;
int reglist[7] = {0};        // 0: not used. 1: used.

int scratch_alloc() {
    for (int reg=0; reg<NREG; reg++) {
        if (reglist[reg]==0) {
            reglist[reg] = 1;
            return reg;
        }
    }
    error("Codegen error: no free scratch register available");
    exit(FAILURE);
}

void scratch_free(int reg) {
    if (reg < 0 || reg >= NREG) {
        error("Codegen error: attempt to free unknown register %d", reg);
        exit(FAILURE);
    }
    reglist[reg] = 0;
}

const char* scratch_name(int reg) {
    /* rbx, r10-r15 */
    if (reg < 0 || reg >= NREG) {
        error("Codegen error: undefined name for unknown register %d", reg);
        exit(FAILURE);
    } else if (reg == 0) {
        return "rbx";
    } else {
        char name[4];
        sprintf(name, "r%d", reg+9);
        return strdup(name);
    }
}