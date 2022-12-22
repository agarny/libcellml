/* The content of this file was generated using the C profile of libCellML 0.4.0. */

#include "model.h"

#include <math.h>
#include <stdlib.h>

const char VERSION[] = "0.3.2";
const char LIBCELLML_VERSION[] = "0.4.0";

const size_t VARIABLE_COUNT = 3;

const VariableInfo VARIABLE_INFO[] = {
    {"x", "mM", "main", COMPUTED_CONSTANT},
    {"y", "M", "main", COMPUTED_CONSTANT},
    {"k", "mM", "constants", CONSTANT}
};

double * createVariablesArray()
{
    return (double *) malloc(VARIABLE_COUNT*sizeof(double));
}

void deleteArray(double *array)
{
    free(array);
}

void initialiseVariables(double *variables)
{
    variables[2] = 123.0;
}

void computeComputedConstants(double *variables)
{
    variables[0] = variables[2]+variables[2];
    variables[1] = 0.001*variables[2]+0.001*variables[2];
}

void computeVariables(double *variables)
{
}
