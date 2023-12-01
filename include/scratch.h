#ifndef SCRATCH_H
#define SCRATCH_H

int scratch_alloc();                // Allocate a register
void scratch_free(int reg);         // Mark a register as available
const char* scratch_name(int reg);  // Returns literal name of register

#endif