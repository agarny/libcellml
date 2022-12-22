/* The content of this file was generated using the C profile of libCellML 0.4.0. */

#include "model.constant.h"

#include <math.h>
#include <stdlib.h>

const char VERSION[] = "0.3.2";
const char LIBCELLML_VERSION[] = "0.4.0";

const size_t STATE_COUNT = 4;
const size_t VARIABLE_COUNT = 18;

const VariableInfo VOI_INFO = {"time", "millisecond", "environment", VARIABLE_OF_INTEGRATION};

const VariableInfo STATE_INFO[] = {
    {"m", "dimensionless", "sodium_channel_m_gate", STATE},
    {"h", "dimensionless", "sodium_channel_h_gate", STATE},
    {"n", "dimensionless", "potassium_channel_n_gate", STATE},
    {"V", "millivolt", "membrane", STATE}
};

const VariableInfo VARIABLE_INFO[] = {
    {"i_Stim", "microA_per_cm2", "membrane", ALGEBRAIC},
    {"E_L", "millivolt", "leakage_current", COMPUTED_CONSTANT},
    {"i_L", "microA_per_cm2", "leakage_current", ALGEBRAIC},
    {"E_Na", "millivolt", "sodium_channel", COMPUTED_CONSTANT},
    {"i_Na", "microA_per_cm2", "sodium_channel", ALGEBRAIC},
    {"alpha_m", "per_millisecond", "sodium_channel_m_gate", ALGEBRAIC},
    {"beta_m", "per_millisecond", "sodium_channel_m_gate", ALGEBRAIC},
    {"alpha_h", "per_millisecond", "sodium_channel_h_gate", ALGEBRAIC},
    {"beta_h", "per_millisecond", "sodium_channel_h_gate", ALGEBRAIC},
    {"E_K", "millivolt", "potassium_channel", COMPUTED_CONSTANT},
    {"i_K", "microA_per_cm2", "potassium_channel", ALGEBRAIC},
    {"alpha_n", "per_millisecond", "potassium_channel_n_gate", ALGEBRAIC},
    {"beta_n", "per_millisecond", "potassium_channel_n_gate", ALGEBRAIC},
    {"Cm", "microF_per_cm2", "membrane", EXTERNAL},
    {"E_R", "millivolt", "membrane", CONSTANT},
    {"g_L", "milliS_per_cm2", "leakage_current", CONSTANT},
    {"g_Na", "milliS_per_cm2", "sodium_channel", CONSTANT},
    {"g_K", "milliS_per_cm2", "potassium_channel", CONSTANT}
};

double * createStatesArray()
{
    return (double *) malloc(STATE_COUNT*sizeof(double));
}

double * createVariablesArray()
{
    return (double *) malloc(VARIABLE_COUNT*sizeof(double));
}

void deleteArray(double *array)
{
    free(array);
}

void initialiseVariables(double voi, double *states, double *variables, ExternalVariable externalVariable)
{
    variables[14] = 0.0;
    variables[15] = 0.3;
    variables[16] = 120.0;
    variables[17] = 36.0;
    states[0] = 0.05;
    states[1] = 0.6;
    states[2] = 0.325;
    states[3] = 0.0;
    variables[13] = externalVariable(voi, states, variables, 13);
}

void computeComputedConstants(double *variables)
{
    variables[1] = variables[14]-10.613;
    variables[3] = variables[14]-115.0;
    variables[9] = variables[14]+12.0;
}

void computeRates(double voi, double *states, double *rates, double *variables, ExternalVariable externalVariable)
{
    variables[5] = 0.1*(states[3]+25.0)/(exp((states[3]+25.0)/10.0)-1.0);
    variables[6] = 4.0*exp(states[3]/18.0);
    rates[0] = variables[5]*(1.0-states[0])-variables[6]*states[0];
    variables[7] = 0.07*exp(states[3]/20.0);
    variables[8] = 1.0/(exp((states[3]+30.0)/10.0)+1.0);
    rates[1] = variables[7]*(1.0-states[1])-variables[8]*states[1];
    variables[11] = 0.01*(states[3]+10.0)/(exp((states[3]+10.0)/10.0)-1.0);
    variables[12] = 0.125*exp(states[3]/80.0);
    rates[2] = variables[11]*(1.0-states[2])-variables[12]*states[2];
    variables[0] = ((voi >= 10.0) && (voi <= 10.5))?-20.0:0.0;
    variables[13] = externalVariable(voi, states, variables, 13);
    variables[2] = variables[15]*(states[3]-variables[1]);
    variables[10] = variables[17]*pow(states[2], 4.0)*(states[3]-variables[9]);
    variables[4] = variables[16]*pow(states[0], 3.0)*states[1]*(states[3]-variables[3]);
    rates[3] = -(-variables[0]+variables[4]+variables[10]+variables[2])/variables[13];
}

void computeVariables(double voi, double *states, double *rates, double *variables, ExternalVariable externalVariable)
{
    variables[2] = variables[15]*(states[3]-variables[1]);
    variables[4] = variables[16]*pow(states[0], 3.0)*states[1]*(states[3]-variables[3]);
    variables[5] = 0.1*(states[3]+25.0)/(exp((states[3]+25.0)/10.0)-1.0);
    variables[6] = 4.0*exp(states[3]/18.0);
    variables[7] = 0.07*exp(states[3]/20.0);
    variables[8] = 1.0/(exp((states[3]+30.0)/10.0)+1.0);
    variables[10] = variables[17]*pow(states[2], 4.0)*(states[3]-variables[9]);
    variables[11] = 0.01*(states[3]+10.0)/(exp((states[3]+10.0)/10.0)-1.0);
    variables[12] = 0.125*exp(states[3]/80.0);
    variables[13] = externalVariable(voi, states, variables, 13);
}
