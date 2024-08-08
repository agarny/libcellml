/* The content of this file was generated using the C profile of libCellML 0.5.0. */

#include "model.h"

#include <math.h>
#include <stdlib.h>

const char VERSION[] = "0.6.0";
const char LIBCELLML_VERSION[] = "0.5.0";

const size_t CONSTANT_COUNT = 0;
const size_t COMPUTED_CONSTANT_COUNT = 0;
const size_t ALGEBRAIC_COUNT = 10;
const size_t EXTERNAL_COUNT = 0;

const VariableInfo VARIABLE_INFO[] = {
    {"v", "fmol_per_sec", "SLC_template3_ss", ALGEBRAIC},
    {"E", "fmol", "SLC_template3_ss", CONSTANT},
    {"P_0", "per_fmol_sec4", "SLC_template3_ss", CONSTANT},
    {"q_Ao", "fmol", "SLC_template3_ss", CONSTANT},
    {"P_1", "per_fmol_sec4", "SLC_template3_ss", CONSTANT},
    {"q_Ai", "fmol", "SLC_template3_ss", CONSTANT},
    {"P_2", "per_fmol_sec3", "SLC_template3_ss", CONSTANT},
    {"P_5", "per_sec3", "SLC_template3_ss", CONSTANT},
    {"P_4", "per_fmol2_sec3", "SLC_template3_ss", CONSTANT},
    {"P_3", "per_fmol_sec3", "SLC_template3_ss", EXTERNAL}
};

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

void initialiseVariables(double *constants, ExternalVariable externalVariable)
{
    constants[0] = 1.1;
    constants[1] = 21262500.0;
    constants[2] = 150.0;
    constants[3] = 3402000.0;
    constants[4] = 2.0;
    constants[5] = 2902500.0;
    constants[6] = 810000.0;
    constants[7] = 247140.0;
    algebraic[1] = externalVariable(variables, 1);
}

void computeComputedConstants(double *constants, double *computedConstants)
{
}

void computeVariables(double *constants, double *computedConstants, double *algebraic, ExternalVariable externalVariable)
{
    algebraic[1] = externalVariable(variables, 1);
    algebraic[0] = constants[0]*(constants[1]*constants[2]-constants[3]*constants[4])/(constants[5]*constants[4]+algebraic[1]*constants[2]+constants[7]*constants[4]*constants[2]+constants[6]);
}
