#ifndef SCRATCH_H
#define SCRATCH_H

int scratch_alloc();                // Allocate a register
int scratch_alloc_xmm();            // Allocate a xmm register
void scratch_free(int reg);         // Mark a register as available
const char* scratch_name(int reg);  // Returns literal name of register

#endif