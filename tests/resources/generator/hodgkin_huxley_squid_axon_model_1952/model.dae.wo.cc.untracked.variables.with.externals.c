/* The content of this file was generated using the C profile of libCellML 0.6.2. */

#include "model.dae.wo.cc.untracked.variables.with.externals.h"

#include <math.h>
#include <stdlib.h>

const char VERSION[] = "0.6.0";
const char LIBCELLML_VERSION[] = "0.6.2";

const size_t STATE_COUNT = 3;
const size_t CONSTANT_COUNT = 0;
const size_t COMPUTED_CONSTANT_COUNT = 0;
const size_t ALGEBRAIC_COUNT = 0;
const size_t EXTERNAL_COUNT = 3;

const VariableInfo VOI_INFO = {"time", "millisecond", "environment"};

const VariableInfo STATE_INFO[] = {
    {"h", "dimensionless", "sodium_channel_h_gate"},
    {"m", "dimensionless", "sodium_channel_m_gate"},
    {"n", "dimensionless", "potassium_channel_n_gate"}
};

const VariableInfo CONSTANT_INFO[] = {
};

const VariableInfo COMPUTED_CONSTANT_INFO[] = {
};

const VariableInfo ALGEBRAIC_INFO[] = {
};

const VariableInfo EXTERNAL_INFO[] = {
    {"V", "millivolt", "membrane"},
    {"i_Na", "microA_per_cm2", "sodium_channel"},
    {"alpha_n", "per_millisecond", "potassium_channel_n_gate"}
};

double * createStatesArray()
{
    double *res = (double *) malloc(STATE_COUNT*sizeof(double));

    for (size_t i = 0; i < STATE_COUNT; ++i) {
        res[i] = NAN;
    }

    return res;
}

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

typedef struct {
    double voi;
    double *states;
    double *rates;
    double *constants;
    double *computedConstants;
    double *algebraic;
    double *externals;
} RootFindingInfo;

extern void nlaSolve(void (*objectiveFunction)(double *, double *, void *),
                     double *u, size_t n, void *data);

void objectiveFunction6(double *u, double *f, void *data)
{
    double voi = ((RootFindingInfo *) data)->voi;
    double *states = ((RootFindingInfo *) data)->states;
    double *rates = ((RootFindingInfo *) data)->rates;
    double *constants = ((RootFindingInfo *) data)->constants;
    double *computedConstants = ((RootFindingInfo *) data)->computedConstants;
    double *algebraic = ((RootFindingInfo *) data)->algebraic;
    double *externals = ((RootFindingInfo *) data)->externals;

    rates[1] = u[0];

    f[0] = rates[1]-(sodium_channel_m_gate_alpha_m*(1.0-states[1])-sodium_channel_m_gate_beta_m*states[1])-0.0;
}

void findRoot6(double voi, double *states, double *rates, double *constants, double *computedConstants, double *algebraic, double *externals)
{
    RootFindingInfo rfi = { voi, states, rates, constants, computedConstants, algebraic, externals };
    double u[1];

    u[0] = rates[1];

    nlaSolve(objectiveFunction6, u, 1, &rfi);

    rates[1] = u[0];
}

void objectiveFunction9(double *u, double *f, void *data)
{
    double voi = ((RootFindingInfo *) data)->voi;
    double *states = ((RootFindingInfo *) data)->states;
    double *rates = ((RootFindingInfo *) data)->rates;
    double *constants = ((RootFindingInfo *) data)->constants;
    double *computedConstants = ((RootFindingInfo *) data)->computedConstants;
    double *algebraic = ((RootFindingInfo *) data)->algebraic;
    double *externals = ((RootFindingInfo *) data)->externals;

    rates[0] = u[0];

    f[0] = rates[0]-(sodium_channel_h_gate_alpha_h*(1.0-states[0])-sodium_channel_h_gate_beta_h*states[0])-0.0;
}

void findRoot9(double voi, double *states, double *rates, double *constants, double *computedConstants, double *algebraic, double *externals)
{
    RootFindingInfo rfi = { voi, states, rates, constants, computedConstants, algebraic, externals };
    double u[1];

    u[0] = rates[0];

    nlaSolve(objectiveFunction9, u, 1, &rfi);

    rates[0] = u[0];
}

void objectiveFunction13(double *u, double *f, void *data)
{
    double voi = ((RootFindingInfo *) data)->voi;
    double *states = ((RootFindingInfo *) data)->states;
    double *rates = ((RootFindingInfo *) data)->rates;
    double *constants = ((RootFindingInfo *) data)->constants;
    double *computedConstants = ((RootFindingInfo *) data)->computedConstants;
    double *algebraic = ((RootFindingInfo *) data)->algebraic;
    double *externals = ((RootFindingInfo *) data)->externals;

    rates[2] = u[0];

    f[0] = rates[2]-(externals[2]*(1.0-states[2])-potassium_channel_n_gate_beta_n*states[2])-0.0;
}

void findRoot13(double voi, double *states, double *rates, double *constants, double *computedConstants, double *algebraic, double *externals)
{
    RootFindingInfo rfi = { voi, states, rates, constants, computedConstants, algebraic, externals };
    double u[1];

    u[0] = rates[2];

    nlaSolve(objectiveFunction13, u, 1, &rfi);

    rates[2] = u[0];
}

void initialiseVariables(double *states, double *rates, double *constants, double *computedConstants, double *algebraic)
{
    states[0] = 0.6;
    states[1] = 0.05;
    states[2] = 0.325;
    rates[0] = 0.0;
    rates[1] = 0.0;
    rates[2] = 0.0;
}

void computeComputedConstants(double *constants, double *computedConstants)
{
}

void computeRates(double voi, double *states, double *rates, double *constants, double *computedConstants, double *algebraic, double *externals, ExternalVariable externalVariable)
{
    externals[0] = externalVariable(voi, states, rates, constants, computedConstants, algebraic, externals, 0);
    findRoot6(voi, states, rates, constants, computedConstants, algebraic, externals);
    findRoot9(voi, states, rates, constants, computedConstants, algebraic, externals);
    externals[2] = externalVariable(voi, states, rates, constants, computedConstants, algebraic, externals, 2);
    findRoot13(voi, states, rates, constants, computedConstants, algebraic, externals);
}

void computeVariables(double voi, double *states, double *rates, double *constants, double *computedConstants, double *algebraic, double *externals, ExternalVariable externalVariable)
{
    externals[0] = externalVariable(voi, states, rates, constants, computedConstants, algebraic, externals, 0);
    externals[2] = externalVariable(voi, states, rates, constants, computedConstants, algebraic, externals, 2);
    externals[1] = externalVariable(voi, states, rates, constants, computedConstants, algebraic, externals, 1);
}