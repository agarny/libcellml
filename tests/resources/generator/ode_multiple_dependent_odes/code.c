/* The content of this file was generated using the C profile of libCellML 0.2.0. */

#include <math.h>
#include <stddef.h>
#include <stdlib.h>

const char LIBCELLML_VERSION[] = "0.2.0";

const size_t STATE_COUNT = 2;
const size_t VARIABLE_COUNT = 1;

typedef struct {
    char name[8];
    char units[14];
    char component[9];
} VariableInfo;

const VariableInfo VOI_INFO = {"t", "second", "my_x_ode"};

const VariableInfo STATE_INFO[] = {
    {"x", "dimensionless", "my_x_ode"},
    {"y", "dimensionless", "my_y_ode"}
};

const VariableInfo VARIABLE_INFO[] = {
    {"epsilon", "dimensionless", "my_y_ode"}
};

double * createStatesArray()
{
    return (double *) malloc(2 * sizeof(double));
}

double * createVariablesArray()
{
    return (double *) malloc(1 * sizeof(double));
}

void deleteArray(double *array)
{
    free(array);
}

void initializeConstants(double *states, double *variables)
{
    states[0] = -2.0;
    states[1] = 0.0;
    variables[0] = 1.0;
}

void computeComputedConstants(double *variables)
{
}

void computeRates(double voi, double *states, double *rates, double *variables)
{
    rates[0] = states[1]*1.0;
    rates[1] = (variables[0]*(1.0-pow(states[0], 2.0))*states[1]-states[0])*1.0;
}

void computeVariables(double voi, double *states, double *rates, double *variables)
{
}
