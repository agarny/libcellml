#include <math.h>

void initializeModel(double *states, double *variables)
{
    states[3] = 0.0;
    variables[0] = 1.0;
    variables[1] = 0.0;
    variables[2] = 0.3;
    variables[3] = 120.0;
    states[1] = 0.6;
    states[0] = 0.05;
    variables[4] = 36.0;
    states[2] = 0.325;
}

void computeConstantEquations(double *variables)
{
    variables[6] = variables[1]-10.613;
    variables[8] = variables[1]-115.0;
    variables[14] = variables[1]+12.0;
}

void computeRateEquations(double voi, double *states, double *rates, double *variables)
{
    variables[10] = 0.1*(states[3]+25.0)/(exp((states[3]+25.0)/10.0)-1.0);
    variables[11] = 4.0*exp(states[3]/18.0);
    rates[0] = variables[10]*(1.0-states[0])-variables[11]*states[0];
    variables[12] = 0.07*exp(states[3]/20.0);
    variables[13] = 1.0/(exp((states[3]+30.0)/10.0)+1.0);
    rates[1] = variables[12]*(1.0-states[1])-variables[13]*states[1];
    variables[16] = 0.01*(states[3]+10.0)/(exp((states[3]+10.0)/10.0)-1.0);
    variables[17] = 0.125*exp(states[3]/80.0);
    rates[2] = variables[16]*(1.0-states[2])-variables[17]*states[2];
    variables[5] = ((voi >= 10.0) && (voi <= 10.5))?-20.0:0.0;
    variables[7] = variables[2]*(states[3]-variables[6]);
    variables[15] = variables[4]*pow(states[2], 4.0)*(states[3]-variables[14]);
    variables[9] = variables[3]*pow(states[0], 3.0)*states[1]*(states[3]-variables[8]);
    rates[3] = -(-variables[5]+variables[9]+variables[15]+variables[7])/variables[0];
}

void computeAlgebraicEquations(double voi, double *states, double *rates, double *variables)
{
}