/* The content of this file was generated using the C profile of libCellML 0.5.0. */

#pragma once

#include <stddef.h>

extern const char VERSION[];
extern const char LIBCELLML_VERSION[];

extern const size_t CONSTANT_COUNT;
extern const size_t COMPUTED_CONSTANT_COUNT;
extern const size_t ALGEBRAIC_COUNT;

typedef struct {
    char name[5];
    char units[15];
    char component[17];
} VariableInfo;

extern const VariableInfo CONSTANT_INFO[];
extern const VariableInfo COMPUTED_CONSTANT_INFO[];
extern const VariableInfo ALGEBRAIC_INFO[];

double * createConstantsArray();
double * createComputedConstantsArray();
double * createAlgebraicArray();

void deleteArray(double *array);

void initialiseVariables(double *constants, double *computedConstants, double *algebraic);
void computeComputedConstants(double *constants, double *computedConstants);
void computeVariables(double *constants, double *computedConstants, double *algebraic);