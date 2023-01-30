/* The content of this file was generated using the C profile of libCellML 0.4.0. */

#include "model.h"

#include <math.h>
#include <stdlib.h>

const char VERSION[] = "0.3.1";
const char LIBCELLML_VERSION[] = "0.4.0";

const size_t STATE_COUNT = 1;
const size_t VARIABLE_COUNT = 203;

const VariableInfo VOI_INFO = {"t", "second", "my_component", VARIABLE_OF_INTEGRATION};

const VariableInfo STATE_INFO[] = {
    {"x", "dimensionless", "my_component", STATE}
};

const VariableInfo VARIABLE_INFO[] = {
    {"m", "dimensionless", "my_component", CONSTANT},
    {"n", "dimensionless", "my_component", CONSTANT},
    {"o", "dimensionless", "my_component", CONSTANT},
    {"p", "dimensionless", "my_component", CONSTANT},
    {"q", "dimensionless", "my_component", CONSTANT},
    {"r", "dimensionless", "my_component", CONSTANT},
    {"s", "dimensionless", "my_component", CONSTANT},
    {"eqnEq", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnEqCoverageParentheses", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnNeq", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnNeqCoverageParentheses", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnLt", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnLtCoverageParentheses", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnLeq", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnLeqCoverageParentheses", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnGt", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnGtCoverageParentheses", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnGeq", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnGeqCoverageParentheses", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnAnd", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnAndMultiple", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnAndParentheses", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnAndParenthesesLeftPlusWith", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnAndParenthesesLeftPlusWithout", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnAndParenthesesLeftMinusWith", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnAndParenthesesLeftMinusWithout", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnAndParenthesesLeftPower", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnAndParenthesesLeftRoot", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnAndParenthesesRightPlusWith", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnAndParenthesesRightPlusWithout", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnAndParenthesesRightMinusWith", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnAndParenthesesRightMinusWithout", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnAndParenthesesRightPower", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnAndParenthesesRightRoot", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnAndCoverageParentheses", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnOr", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnOrMultiple", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnOrParentheses", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnOrParenthesesLeftPlusWith", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnOrParenthesesLeftPlusWithout", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnOrParenthesesLeftMinusWith", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnOrParenthesesLeftMinusWithout", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnOrParenthesesLeftPower", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnOrParenthesesLeftRoot", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnOrParenthesesRightPlusWith", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnOrParenthesesRightPlusWithout", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnOrParenthesesRightMinusWith", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnOrParenthesesRightMinusWithout", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnOrParenthesesRightPower", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnOrParenthesesRightRoot", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnOrCoverageParentheses", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnXor", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnXorMultiple", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnXorParentheses", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnXorParenthesesLeftPlusWith", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnXorParenthesesLeftPlusWithout", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnXorParenthesesLeftMinusWith", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnXorParenthesesLeftMinusWithout", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnXorParenthesesLeftPower", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnXorParenthesesLeftRoot", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnXorParenthesesRightPlusWith", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnXorParenthesesRightPlusWithout", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnXorParenthesesRightMinusWith", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnXorParenthesesRightMinusWithout", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnXorParenthesesRightPower", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnXorParenthesesRightRoot", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnXorCoverageParentheses", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnNot", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnPlus", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnPlusMultiple", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnPlusParentheses", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnPlusUnary", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnMinus", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnMinusParentheses", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnMinusParenthesesPlusWith", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnMinusParenthesesPlusWithout", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnMinusUnary", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnMinusUnaryParentheses", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnTimes", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnTimesMultiple", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnTimesParentheses", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnTimesParenthesesLeftPlusWith", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnTimesParenthesesLeftPlusWithout", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnTimesParenthesesLeftMinusWith", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnTimesParenthesesLeftMinusWithout", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnTimesParenthesesRightPlusWith", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnTimesParenthesesRightPlusWithout", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnTimesParenthesesRightMinusWith", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnTimesParenthesesRightMinusWithout", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnDivide", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnDivideParentheses", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnDivideParenthesesLeftPlusWith", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnDivideParenthesesLeftPlusWithout", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnDivideParenthesesLeftMinusWith", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnDivideParenthesesLeftMinusWithout", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnDivideParenthesesRightPlusWith", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnDivideParenthesesRightPlusWithout", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnDivideParenthesesRightMinusWith", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnDivideParenthesesRightMinusWithout", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnDivideParenthesesRightTimes", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnDivideParenthesesRightDivide", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnPowerSqrt", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnPowerSqr", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnPowerCube", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnPowerCi", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnPowerParentheses", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnPowerParenthesesLeftPlusWith", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnPowerParenthesesLeftPlusWithout", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnPowerParenthesesLeftMinusWith", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnPowerParenthesesLeftMinusWithout", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnPowerParenthesesLeftTimes", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnPowerParenthesesLeftDivide", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnPowerParenthesesRightPlusWith", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnPowerParenthesesRightPlusWithout", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnPowerParenthesesRightMinusWith", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnPowerParenthesesRightMinusWithout", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnPowerParenthesesRightTimes", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnPowerParenthesesRightDivide", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnPowerParenthesesRightPower", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnPowerParenthesesRightRoot", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnRootSqrt", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnRootSqrtOther", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnRootCube", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnRootCi", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnRootParentheses", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnRootParenthesesLeftPlusWith", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnRootParenthesesLeftPlusWithout", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnRootParenthesesLeftMinusWith", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnRootParenthesesLeftMinusWithout", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnRootParenthesesLeftTimes", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnRootParenthesesLeftDivide", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnRootParenthesesRightPlusWith", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnRootParenthesesRightPlusWithout", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnRootParenthesesRightMinusWith", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnRootParenthesesRightMinusWithout", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnRootParenthesesRightTimes", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnRootParenthesesRightDivide", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnRootParenthesesRightPower", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnRootParenthesesRightRoot", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnAbs", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnExp", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnLn", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnLog", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnLog2", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnLog10", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnLogCi", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnCeiling", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnFloor", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnMin", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnMinMultiple", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnMax", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnMaxMultiple", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnRem", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnSin", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnCos", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnTan", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnSec", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnCsc", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnCot", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnSinh", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnCosh", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnTanh", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnSech", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnCsch", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnCoth", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnArcsin", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnArccos", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnArctan", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnArcsec", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnArccsc", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnArccot", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnArcsinh", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnArccosh", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnArctanh", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnArcsech", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnArccsch", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnArccoth", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnPiecewisePiece", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnPiecewisePieceOtherwise", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnPiecewisePiecePiecePiece", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnPiecewisePiecePiecePieceOtherwise", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnWithPiecewise", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnCnInteger", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnCnDouble", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnCnIntegerWithExponent", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnCnDoubleWithExponent", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnCi", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnTrue", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnFalse", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnExponentiale", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnPi", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnInfinity", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnNotanumber", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnCoverageForPlusOperator", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnCoverageForMinusOperator", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnCoverageForTimesOperator", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnCoverageForDivideOperator", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnCoverageForAndOperator", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnCoverageForOrOperator", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnCoverageForXorOperator", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnCoverageForPowerOperator", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnCoverageForRootOperator", "dimensionless", "my_component", COMPUTED_CONSTANT},
    {"eqnCoverageForMinusUnary", "dimensionless", "my_component", COMPUTED_CONSTANT}
};

double xor(double x, double y)
{
    return (x != 0.0) ^ (y != 0.0);
}

double min(double x, double y)
{
    return (x < y)?x:y;
}

double max(double x, double y)
{
    return (x > y)?x:y;
}

double sec(double x)
{
    return 1.0/cos(x);
}

double csc(double x)
{
    return 1.0/sin(x);
}

double cot(double x)
{
    return 1.0/tan(x);
}

double sech(double x)
{
    return 1.0/cosh(x);
}

double csch(double x)
{
    return 1.0/sinh(x);
}

double coth(double x)
{
    return 1.0/tanh(x);
}

double asec(double x)
{
    return acos(1.0/x);
}

double acsc(double x)
{
    return asin(1.0/x);
}

double acot(double x)
{
    return atan(1.0/x);
}

double asech(double x)
{
    double oneOverX = 1.0/x;

    return log(oneOverX+sqrt(oneOverX*oneOverX-1.0));
}

double acsch(double x)
{
    double oneOverX = 1.0/x;

    return log(oneOverX+sqrt(oneOverX*oneOverX+1.0));
}

double acoth(double x)
{
    double oneOverX = 1.0/x;

    return 0.5*log((1.0+oneOverX)/(1.0-oneOverX));
}

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

void initialiseVariables(double *states, double *variables)
{
    variables[0] = 1.0;
    variables[1] = 2.0;
    variables[2] = 3.0;
    variables[3] = 4.0;
    variables[4] = 5.0;
    variables[5] = 6.0;
    variables[6] = 7.0;
    variables[182] = 123.0;
    variables[183] = 123.456789;
    variables[184] = 123.0e99;
    variables[185] = 123.456789e99;
    variables[187] = 1.0;
    variables[188] = 0.0;
    variables[189] = 2.71828182845905;
    variables[190] = 3.14159265358979;
    variables[191] = INFINITY;
    variables[192] = NAN;
    states[0] = 0.0;
}

void computeComputedConstants(double *variables)
{
    variables[7] = variables[0] == variables[1];
    variables[8] = variables[0]/(variables[1] == variables[1]);
    variables[9] = variables[0] != variables[1];
    variables[10] = variables[0]/(variables[1] != variables[2]);
    variables[11] = variables[0] < variables[1];
    variables[12] = variables[0]/(variables[1] < variables[2]);
    variables[13] = variables[0] <= variables[1];
    variables[14] = variables[0]/(variables[1] <= variables[2]);
    variables[15] = variables[0] > variables[1];
    variables[16] = variables[0]/(variables[1] > variables[2]);
    variables[17] = variables[0] >= variables[1];
    variables[18] = variables[0]/(variables[1] >= variables[2]);
    variables[19] = variables[0] && variables[1];
    variables[20] = variables[0] && variables[1] && variables[2];
    variables[21] = (variables[0] < variables[1]) && (variables[2] > variables[3]);
    variables[22] = (variables[0]+variables[1]) && (variables[2] > variables[3]);
    variables[23] = variables[0] && (variables[1] > variables[2]);
    variables[24] = (variables[0]-variables[1]) && (variables[2] > variables[3]);
    variables[25] = -variables[0] && (variables[1] > variables[2]);
    variables[26] = pow(variables[0], variables[1]) && (variables[2] > variables[3]);
    variables[27] = pow(variables[0], 1.0/variables[1]) && (variables[2] > variables[3]);
    variables[28] = (variables[0] < variables[1]) && (variables[2]+variables[3]);
    variables[29] = (variables[0] < variables[1]) && variables[2];
    variables[30] = (variables[0] < variables[1]) && (variables[2]-variables[3]);
    variables[31] = (variables[0] < variables[1]) && -variables[2];
    variables[32] = (variables[0] < variables[1]) && pow(variables[2], variables[3]);
    variables[33] = (variables[0] < variables[1]) && pow(variables[2], 1.0/variables[3]);
    variables[34] = variables[0]/(variables[1] && variables[2]);
    variables[35] = variables[0] || variables[1];
    variables[36] = variables[0] || variables[1] || variables[2];
    variables[37] = (variables[0] < variables[1]) || (variables[2] > variables[3]);
    variables[38] = (variables[0]+variables[1]) || (variables[2] > variables[3]);
    variables[39] = variables[0] || (variables[1] > variables[2]);
    variables[40] = (variables[0]-variables[1]) || (variables[2] > variables[3]);
    variables[41] = -variables[0] || (variables[1] > variables[2]);
    variables[42] = pow(variables[0], variables[1]) || (variables[2] > variables[3]);
    variables[43] = pow(variables[0], 1.0/variables[1]) || (variables[2] > variables[3]);
    variables[44] = (variables[0] < variables[1]) || (variables[2]+variables[3]);
    variables[45] = (variables[0] < variables[1]) || variables[2];
    variables[46] = (variables[0] < variables[1]) || (variables[2]-variables[3]);
    variables[47] = (variables[0] < variables[1]) || -variables[2];
    variables[48] = (variables[0] < variables[1]) || pow(variables[2], variables[3]);
    variables[49] = (variables[0] < variables[1]) || pow(variables[2], 1.0/variables[3]);
    variables[50] = variables[0]/(variables[1] || variables[2]);
    variables[51] = xor(variables[0], variables[1]);
    variables[52] = xor(variables[0], xor(variables[1], variables[2]));
    variables[53] = xor(variables[0] < variables[1], variables[2] > variables[3]);
    variables[54] = xor(variables[0]+variables[1], variables[2] > variables[3]);
    variables[55] = xor(variables[0], variables[1] > variables[2]);
    variables[56] = xor(variables[0]-variables[1], variables[2] > variables[3]);
    variables[57] = xor(-variables[0], variables[1] > variables[2]);
    variables[58] = xor(pow(variables[0], variables[1]), variables[2] > variables[3]);
    variables[59] = xor(pow(variables[0], 1.0/variables[1]), variables[2] > variables[3]);
    variables[60] = xor(variables[0] < variables[1], variables[2]+variables[3]);
    variables[61] = xor(variables[0] < variables[1], variables[2]);
    variables[62] = xor(variables[0] < variables[1], variables[2]-variables[3]);
    variables[63] = xor(variables[0] < variables[1], -variables[2]);
    variables[64] = xor(variables[0] < variables[1], pow(variables[2], variables[3]));
    variables[65] = xor(variables[0] < variables[1], pow(variables[2], 1.0/variables[3]));
    variables[66] = variables[0]/xor(variables[1], variables[2]);
    variables[67] = !variables[0];
    variables[68] = variables[0]+variables[1];
    variables[69] = variables[0]+variables[1]+variables[2];
    variables[70] = (variables[0] < variables[1])+(variables[2] > variables[3]);
    variables[71] = variables[0];
    variables[72] = variables[0]-variables[1];
    variables[73] = (variables[0] < variables[1])-(variables[2] > variables[3]);
    variables[74] = (variables[0] < variables[1])-(variables[2]+variables[3]);
    variables[75] = (variables[0] < variables[1])-variables[2];
    variables[76] = -variables[0];
    variables[77] = -(variables[0] < variables[1]);
    variables[78] = variables[0]*variables[1];
    variables[79] = variables[0]*variables[1]*variables[2];
    variables[80] = (variables[0] < variables[1])*(variables[2] > variables[3]);
    variables[81] = (variables[0]+variables[1])*(variables[2] > variables[3]);
    variables[82] = variables[0]*(variables[1] > variables[2]);
    variables[83] = (variables[0]-variables[1])*(variables[2] > variables[3]);
    variables[84] = -variables[0]*(variables[1] > variables[2]);
    variables[85] = (variables[0] < variables[1])*(variables[2]+variables[3]);
    variables[86] = (variables[0] < variables[1])*variables[2];
    variables[87] = (variables[0] < variables[1])*(variables[2]-variables[3]);
    variables[88] = (variables[0] < variables[1])*-variables[2];
    variables[89] = variables[0]/variables[1];
    variables[90] = (variables[0] < variables[1])/(variables[3] > variables[2]);
    variables[91] = (variables[0]+variables[1])/(variables[3] > variables[2]);
    variables[92] = variables[0]/(variables[2] > variables[1]);
    variables[93] = (variables[0]-variables[1])/(variables[3] > variables[2]);
    variables[94] = -variables[0]/(variables[2] > variables[1]);
    variables[95] = (variables[0] < variables[1])/(variables[2]+variables[3]);
    variables[96] = (variables[0] < variables[1])/variables[2];
    variables[97] = (variables[0] < variables[1])/(variables[2]-variables[3]);
    variables[98] = (variables[0] < variables[1])/-variables[2];
    variables[99] = (variables[0] < variables[1])/(variables[2]*variables[3]);
    variables[100] = (variables[0] < variables[1])/(variables[2]/variables[3]);
    variables[101] = sqrt(variables[0]);
    variables[102] = pow(variables[0], 2.0);
    variables[103] = pow(variables[0], 3.0);
    variables[104] = pow(variables[0], variables[1]);
    variables[105] = pow(variables[0] <= variables[1], variables[2] >= variables[3]);
    variables[106] = pow(variables[0]+variables[1], variables[2] >= variables[3]);
    variables[107] = pow(variables[0], variables[1] >= variables[2]);
    variables[108] = pow(variables[0]-variables[1], variables[2] >= variables[3]);
    variables[109] = pow(-variables[0], variables[1] >= variables[2]);
    variables[110] = pow(variables[0]*variables[1], variables[2] >= variables[3]);
    variables[111] = pow(variables[0]/variables[1], variables[2] >= variables[3]);
    variables[112] = pow(variables[0] <= variables[1], variables[2]+variables[3]);
    variables[113] = pow(variables[0] <= variables[1], variables[2]);
    variables[114] = pow(variables[0] <= variables[1], variables[2]-variables[3]);
    variables[115] = pow(variables[0] <= variables[1], -variables[2]);
    variables[116] = pow(variables[0] <= variables[1], variables[2]*variables[3]);
    variables[117] = pow(variables[0] <= variables[1], variables[2]/variables[3]);
    variables[118] = pow(variables[0] <= variables[1], pow(variables[2], variables[3]));
    variables[119] = pow(variables[0] <= variables[1], pow(variables[2], 1.0/variables[3]));
    variables[120] = sqrt(variables[0]);
    variables[121] = sqrt(variables[0]);
    variables[122] = pow(variables[0], 1.0/3.0);
    variables[123] = pow(variables[0], 1.0/variables[1]);
    variables[124] = pow(variables[0] < variables[1], 1.0/(variables[3] > variables[2]));
    variables[125] = pow(variables[0]+variables[1], 1.0/(variables[3] > variables[2]));
    variables[126] = pow(variables[0], 1.0/(variables[2] > variables[1]));
    variables[127] = pow(variables[0]-variables[1], 1.0/(variables[3] > variables[2]));
    variables[128] = pow(-variables[0], 1.0/(variables[2] > variables[1]));
    variables[129] = pow(variables[0]*variables[1], 1.0/(variables[3] > variables[2]));
    variables[130] = pow(variables[0]/variables[1], 1.0/(variables[3] > variables[2]));
    variables[131] = pow(variables[0] < variables[1], 1.0/(variables[2]+variables[3]));
    variables[132] = pow(variables[0] < variables[1], 1.0/variables[2]);
    variables[133] = pow(variables[0] < variables[1], 1.0/(variables[2]-variables[3]));
    variables[134] = pow(variables[0] < variables[1], 1.0/-variables[2]);
    variables[135] = pow(variables[0] < variables[1], 1.0/(variables[2]*variables[3]));
    variables[136] = pow(variables[0] < variables[1], 1.0/(variables[2]/variables[3]));
    variables[137] = pow(variables[0] < variables[1], 1.0/pow(variables[2], variables[3]));
    variables[138] = pow(variables[0] < variables[1], 1.0/pow(variables[2], 1.0/variables[3]));
    variables[139] = fabs(variables[0]);
    variables[140] = exp(variables[0]);
    variables[141] = log(variables[0]);
    variables[142] = log10(variables[0]);
    variables[143] = log(variables[0])/log(2.0);
    variables[144] = log10(variables[0]);
    variables[145] = log(variables[0])/log(variables[1]);
    variables[146] = ceil(variables[0]);
    variables[147] = floor(variables[0]);
    variables[148] = min(variables[0], variables[1]);
    variables[149] = min(variables[0], min(variables[1], variables[2]));
    variables[150] = max(variables[0], variables[1]);
    variables[151] = max(variables[0], max(variables[1], variables[2]));
    variables[152] = fmod(variables[0], variables[1]);
    variables[153] = sin(variables[0]);
    variables[154] = cos(variables[0]);
    variables[155] = tan(variables[0]);
    variables[156] = sec(variables[0]);
    variables[157] = csc(variables[0]);
    variables[158] = cot(variables[0]);
    variables[159] = sinh(variables[0]);
    variables[160] = cosh(variables[0]);
    variables[161] = tanh(variables[0]);
    variables[162] = sech(variables[0]);
    variables[163] = csch(variables[0]);
    variables[164] = coth(variables[0]);
    variables[165] = asin(variables[0]);
    variables[166] = acos(variables[0]);
    variables[167] = atan(variables[0]);
    variables[168] = asec(variables[0]);
    variables[169] = acsc(variables[0]);
    variables[170] = acot(variables[0]);
    variables[171] = asinh(variables[0]);
    variables[172] = acosh(variables[0]);
    variables[173] = atanh(variables[0]/2.0);
    variables[174] = asech(variables[0]);
    variables[175] = acsch(variables[0]);
    variables[176] = acoth(2.0*variables[0]);
    variables[177] = (variables[0] > variables[1])?variables[0]:NAN;
    variables[178] = (variables[0] > variables[1])?variables[0]:variables[2];
    variables[179] = (variables[0] > variables[1])?variables[0]:(variables[2] > variables[3])?variables[2]:(variables[4] > variables[5])?variables[4]:NAN;
    variables[180] = (variables[0] > variables[1])?variables[0]:(variables[2] > variables[3])?variables[2]:(variables[4] > variables[5])?variables[4]:variables[6];
    variables[181] = 123.0+((variables[0] > variables[1])?variables[0]:NAN);
    variables[186] = variables[0];
    variables[193] = (variables[0] && variables[1])+((variables[2] > variables[3])?variables[1]:NAN)+variables[4]+(variables[5] && variables[6]);
    variables[194] = (variables[0] && variables[1])-(((variables[2] > variables[3])?variables[1]:NAN)-(variables[4]-((variables[2] > variables[3])?variables[1]:NAN)))-(variables[5] && variables[6]);
    variables[195] = (variables[0] && variables[1])*((variables[2] > variables[3])?variables[1]:NAN)*variables[4]*((variables[2] > variables[3])?variables[1]:NAN)*(variables[5] && variables[6]);
    variables[196] = (variables[0] && variables[1])/(((variables[2] > variables[3])?variables[1]:NAN)/(variables[4]/((variables[2] > variables[3])?variables[1]:NAN)));
    variables[197] = (variables[0] || variables[1]) && xor(variables[0], variables[1]) && ((variables[2] > variables[3])?variables[1]:NAN) && variables[4] && ((variables[2] > variables[3])?variables[1]:NAN) && xor(variables[0], variables[1]) && (variables[0] || variables[1]);
    variables[198] = (variables[0] && variables[1]) || xor(variables[0], variables[1]) || ((variables[2] > variables[3])?variables[1]:NAN) || variables[4] || ((variables[2] > variables[3])?variables[1]:NAN) || xor(variables[0], variables[1]) || (variables[0] && variables[1]);
    variables[199] = xor(variables[0] && variables[1], xor(variables[0] || variables[1], xor((variables[2] > variables[3])?variables[1]:NAN, xor(xor(xor(variables[4], (variables[2] > variables[3])?variables[1]:NAN), variables[0] || variables[1]), variables[0] && variables[1]))));
    variables[200] = pow(variables[0] && variables[1], pow((variables[2] > variables[3])?variables[1]:NAN, pow(pow(variables[4], (variables[2] > variables[3])?variables[1]:NAN), variables[0] && variables[1])));
    variables[201] = pow(pow(pow(variables[0] && variables[1], 1.0/pow((variables[2] > variables[3])?variables[1]:NAN, 1.0/variables[4])), 1.0/((variables[2] > variables[3])?variables[1]:NAN)), 1.0/(variables[0] && variables[1]));
    variables[202] = -(variables[0] && variables[1])+-((variables[2] > variables[3])?variables[1]:NAN);
}

void computeRates(double voi, double *states, double *rates, double *variables)
{
    rates[0] = 1.0;
}

void computeVariables(double voi, double *states, double *rates, double *variables)
{
}