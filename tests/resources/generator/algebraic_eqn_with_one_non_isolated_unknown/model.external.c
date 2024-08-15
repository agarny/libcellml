/* The content of this file was generated using the C profile of libCellML 0.5.0. */

#include "model.external.h"

#include <math.h>
#include <stdlib.h>

const char VERSION[] = "0.6.0";
const char LIBCELLML_VERSION[] = "0.5.0";

const size_t CONSTANT_COUNT = 0;
const size_t COMPUTED_CONSTANT_COUNT = 3;
const size_t ALGEBRAIC_COUNT = 0;
const size_t EXTERNAL_COUNT = 1;

const VariableInfo CONSTANT_INFO[] = {
};

const VariableInfo COMPUTED_CONSTANT_INFO[] = {
    {"b", "dimensionless", "my_algebraic_eqn"},
    {"c", "dimensionless", "my_algebraic_eqn"},
    {"d", "dimensionless", "my_algebraic_eqn"}
};

const VariableInfo ALGEBRAIC_INFO[] = {
};

const VariableInfo EXTERNAL_INFO[] = {
    {"a", "dimensionless", "my_algebraic_eqn"}
};

double * createConstantsArray()
{
    double *res = (double *) malloc(CONSTANT_COUNT*sizeof(double));

    for (size_t i = 0; i < CONSTANT_COUNT; ++i) {
        res[i] = NAN;
    }

    return res;
}

double * createComputedConstantsArray()
{
    double *res = (double *) malloc(COMPUTED_CONSTANT_COUNT*sizeof(double));

    for (size_t i = 0; i < COMPUTED_CONSTANT_COUNT; ++i) {
        res[i] = NAN;
    }

    return res;
}

double * createAlgebraicArray()
{
    double *res = (double *) malloc(ALGEBRAIC_COUNT*sizeof(double));

    for (size_t i = 0; i < ALGEBRAIC_COUNT; ++i) {
        res[i] = NAN;
    }

    return res;
}

double * createExternalsArray()
{
    double *res = (double *) malloc(EXTERNAL_COUNT*sizeof(double));

    for (size_t i = 0; i < EXTERNAL_COUNT; ++i) {
        res[i] = NAN;
    }

    return res;
}

void deleteArray(double *array)
{
    free(array);
}

void initialiseVariables(double *constants, double *algebraic, ExternalVariable externalVariable)
{
    algebraic[0] = externalVariable(variables, 0);
}

void computeComputedConstants(double *constants, double *computedConstants)
{
    computedConstants[0] = 3.0;
    computedConstants[1] = 5.0;
    computedConstants[2] = 7.0;
}

void computeVariables(double *constants, double *computedConstants, double *algebraic, ExternalVariable externalVariable)
{
    algebraic[0] = externalVariable(variables, 0);
}
