/* The content of this file was generated using the C profile of libCellML 0.5.0. */

#include "model.h"

#include <math.h>
#include <stdlib.h>

const char VERSION[] = "0.6.0";
const char LIBCELLML_VERSION[] = "0.5.0";

const size_t STATE_COUNT = 1;
const size_t CONSTANT_COUNT = 0;
const size_t COMPUTED_CONSTANT_COUNT = 0;
const size_t ALGEBRAIC_COUNT = 0;
const size_t EXTERNAL_COUNT = 0;

const VariableInfo VOI_INFO = {"t", "second", "my_component", VARIABLE_OF_INTEGRATION};

const VariableInfo STATE_INFO[] = {
    {"x", "dimensionless", "my_component", STATE}
};

const VariableInfo VARIABLE_INFO[] = {
};

double * createStatesArray()
{
    double *res = (double *) malloc(STATE_COUNT*sizeof(double));

    for (size_t i = 0; i < STATE_COUNT; ++i) {
        res[i] = NAN;
    }

    return res;
}

double * createVariablesArray()
{
    double *res = (double *) malloc(VARIABLE_COUNT*sizeof(double));

    for (size_t i = 0; i < VARIABLE_COUNT; ++i) {
        res[i] = NAN;
    }

    return res;
}

void deleteArray(double *array)
{
    free(array);
}

void initialiseVariables(double *states, double *rates, double *constants)
{
    states[0] = 1.0;
}

void computeComputedConstants(double *constants, double *computedConstants)
{
}

void computeRates(double voi, double *states, double *rates, double *constants, double *computedConstants, double *algebraic)
{
    rates[0] = 1.0;
}

void computeVariables(double voi, double *states, double *rates, double *constants, double *computedConstants, double *algebraic)
{
}
