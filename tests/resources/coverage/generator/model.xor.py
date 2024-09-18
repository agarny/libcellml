# The content of this file was generated using a modified Python profile of libCellML 0.5.0.

from enum import Enum
from math import *


__version__ = "0.5.0.post0"
LIBCELLML_VERSION = "0.5.0"

STATE_COUNT = 1
CONSTANT_COUNT = 7
COMPUTED_CONSTANT_COUNT = 207
ALGEBRAIC_COUNT = 5
EXTERNAL_COUNT = 1

VOI_INFO = {"name": "t", "units": "second", "component": "my_component"}

STATE_INFO = [
    {"name": "x", "units": "dimensionless", "component": "my_component"}
]

CONSTANT_INFO = [
    {"name": "m", "units": "dimensionless", "component": "my_component"},
    {"name": "n", "units": "dimensionless", "component": "my_component"},
    {"name": "o", "units": "dimensionless", "component": "my_component"},
    {"name": "p", "units": "dimensionless", "component": "my_component"},
    {"name": "q", "units": "dimensionless", "component": "my_component"},
    {"name": "r", "units": "dimensionless", "component": "my_component"},
    {"name": "s", "units": "dimensionless", "component": "my_component"}
]

COMPUTED_CONSTANT_INFO = [
    {"name": "eqnEq", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnEqCoverageParentheses", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnNeq", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnNeqCoverageParentheses", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnLt", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnLtCoverageParentheses", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnLeq", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnLeqCoverageParentheses", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnGt", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnGtCoverageParentheses", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnGeq", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnGeqCoverageParentheses", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnAnd", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnAndMultiple", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnAndParentheses", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnAndParenthesesLeftPlusWith", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnAndParenthesesLeftPlusWithout", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnAndParenthesesLeftMinusWith", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnAndParenthesesLeftMinusWithout", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnAndParenthesesLeftPower", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnAndParenthesesLeftRoot", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnAndParenthesesRightPlusWith", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnAndParenthesesRightPlusWithout", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnAndParenthesesRightMinusWith", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnAndParenthesesRightMinusWithout", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnAndParenthesesRightPower", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnAndParenthesesRightRoot", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnAndCoverageParentheses", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnOrParentheses", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnOrParenthesesLeftPlusWith", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnOrParenthesesLeftPlusWithout", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnOrParenthesesLeftMinusWith", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnOrParenthesesLeftMinusWithout", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnOrParenthesesLeftPower", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnOrParenthesesLeftRoot", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnOrParenthesesRightPlusWith", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnOrParenthesesRightPlusWithout", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnOrParenthesesRightMinusWith", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnOrParenthesesRightMinusWithout", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnOrParenthesesRightPower", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnOrParenthesesRightRoot", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnOrCoverageParentheses", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnXor", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnXorMultiple", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnXorParentheses", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnXorParenthesesLeftPlusWith", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnXorParenthesesLeftPlusWithout", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnXorParenthesesLeftMinusWith", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnXorParenthesesLeftMinusWithout", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnXorParenthesesLeftPower", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnXorParenthesesLeftRoot", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnXorParenthesesRightPlusWith", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnXorParenthesesRightPlusWithout", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnXorParenthesesRightMinusWith", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnXorParenthesesRightMinusWithout", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnXorParenthesesRightPower", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnXorParenthesesRightRoot", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnNot", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPlusMultiple", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPlusParentheses", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPlusUnary", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnMinus", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnMinusParentheses", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnMinusParenthesesPlusWith", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnMinusParenthesesPlusWithout", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnMinusParenthesesDirectUnaryMinus", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnMinusParenthesesIndirectUnaryMinus", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnMinusUnary", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnMinusUnaryParentheses", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnTimes", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnTimesMultiple", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnTimesParentheses", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnTimesParenthesesLeftPlusWith", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnTimesParenthesesLeftPlusWithout", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnTimesParenthesesLeftMinusWith", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnTimesParenthesesLeftMinusWithout", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnTimesParenthesesRightPlusWith", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnTimesParenthesesRightPlusWithout", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnTimesParenthesesRightMinusWith", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnTimesParenthesesRightMinusWithout", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnDivide", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnDivideParentheses", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnDivideParenthesesLeftPlusWith", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnDivideParenthesesLeftPlusWithout", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnDivideParenthesesLeftMinusWith", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnDivideParenthesesLeftMinusWithout", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnDivideParenthesesRightPlusWith", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnDivideParenthesesRightPlusWithout", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnDivideParenthesesRightMinusWith", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnDivideParenthesesRightMinusWithout", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnDivideParenthesesRightTimes", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnDivideParenthesesRightDivide", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPowerSqrt", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPowerSqr", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPowerCube", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPowerCi", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPowerParentheses", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPowerParenthesesLeftPlusWith", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPowerParenthesesLeftPlusWithout", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPowerParenthesesLeftMinusWith", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPowerParenthesesLeftMinusWithout", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPowerParenthesesLeftTimes", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPowerParenthesesLeftDivide", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPowerParenthesesRightPlusWith", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPowerParenthesesRightPlusWithout", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPowerParenthesesRightMinusWith", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPowerParenthesesRightMinusWithout", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPowerParenthesesRightTimes", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPowerParenthesesRightDivide", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPowerParenthesesRightPower", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPowerParenthesesRightRoot", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnRootSqrt", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnRootSqrtOther", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnRootSqr", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnRootCube", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnRootCi", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnRootParentheses", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnRootParenthesesLeftPlusWith", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnRootParenthesesLeftPlusWithout", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnRootParenthesesLeftMinusWith", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnRootParenthesesLeftMinusWithout", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnRootParenthesesLeftTimes", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnRootParenthesesLeftDivide", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnRootParenthesesRightPlusWith", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnRootParenthesesRightPlusWithout", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnRootParenthesesRightMinusWith", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnRootParenthesesRightMinusWithout", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnRootParenthesesRightTimes", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnRootParenthesesRightDivide", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnRootParenthesesRightPower", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnRootParenthesesRightRoot", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnAbs", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnExp", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnLn", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnLog", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnLog2", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnLog10", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnLogCi", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnCeiling", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnFloor", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnMin", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnMinMultiple", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnMax", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnMaxMultiple", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnRem", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnSin", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnCos", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnTan", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnSec", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnCsc", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnCot", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnSinh", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnCosh", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnTanh", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnSech", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnCsch", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnCoth", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnArcsin", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnArccos", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnArctan", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnArcsec", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnArccsc", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnArccot", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnArcsinh", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnArccosh", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnArctanh", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnArcsech", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnArccsch", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnArccoth", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPiecewise", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPiecewisePiece", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPiecewisePiece2", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPiecewiseOtherwise", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPiecewisePieceOtherwise", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPiecewisePiecePiecePiece", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPiecewisePiecePiecePiece2", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPiecewisePiecePiecePieceOtherwise", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnWithPiecewise", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnWithPiecewise2", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnCnInteger", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnCnDouble", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnCnIntegerWithExponent", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnCnDoubleWithExponent", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnCi", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnTrue", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnFalse", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnExponentiale", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnPi", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnInfinity", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnNotanumber", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnCoverageForPlusOperator", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnCoverageForPlusOperator2", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnCoverageForMinusOperator", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnCoverageForMinusOperator2", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnCoverageForTimesOperator", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnCoverageForTimesOperator2", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnCoverageForDivideOperator", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnCoverageForDivideOperator2", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnCoverageForAndOperator", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnCoverageForOrOperator", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnCoverageForXorOperator", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnCoverageForPowerOperator", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnCoverageForRootOperator", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnCoverageForMinusUnary", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnCoverageForMinusUnary2", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnComputedConstant1", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnComputedConstant2", "units": "dimensionless", "component": "my_component"}
]

ALGEBRAIC_INFO = [
    {"name": "eqnOr", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnOrMultiple", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnXorCoverageParentheses", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnNlaVariable1", "units": "dimensionless", "component": "my_component"},
    {"name": "eqnNlaVariable2", "units": "dimensionless", "component": "my_component"}
]

EXTERNAL_INFO = [
    {"name": "eqnPlus", "units": "dimensionless", "component": "my_component"}
]


def eq_func(x, y):
    return 1.0 if x == y else 0.0


def neq_func(x, y):
    return 1.0 if x != y else 0.0


def lt_func(x, y):
    return 1.0 if x < y else 0.0


def leq_func(x, y):
    return 1.0 if x <= y else 0.0


def gt_func(x, y):
    return 1.0 if x > y else 0.0


def geq_func(x, y):
    return 1.0 if x >= y else 0.0


def and_func(x, y):
    return 1.0 if bool(x) & bool(y) else 0.0


def or_func(x, y):
    return 1.0 if bool(x) | bool(y) else 0.0


def XOR_FUNC(x, y):
    return 1.0 if bool(x) ^ bool(y) else 0.0


def not_func(x):
    return 1.0 if not bool(x) else 0.0


def min(x, y):
    return x if x < y else y


def max(x, y):
    return x if x > y else y


def sec(x):
    return 1.0/cos(x)


def csc(x):
    return 1.0/sin(x)


def cot(x):
    return 1.0/tan(x)


def sech(x):
    return 1.0/cosh(x)


def csch(x):
    return 1.0/sinh(x)


def coth(x):
    return 1.0/tanh(x)


def asec(x):
    return acos(1.0/x)


def acsc(x):
    return asin(1.0/x)


def acot(x):
    return atan(1.0/x)


def asech(x):
    return acosh(1.0/x)


def acsch(x):
    return asinh(1.0/x)


def acoth(x):
    return atanh(1.0/x)


def create_states_array():
    return [nan]*STATE_COUNT


def create_constants_array():
    return [nan]*CONSTANT_COUNT


def create_computed_constants_array():
    return [nan]*COMPUTED_CONSTANT_COUNT


def create_algebraic_array():
    return [nan]*ALGEBRAIC_COUNT


def create_externals_array():
    return [nan]*EXTERNAL_COUNT


from nlasolver import nla_solve


def objective_function_0(u, f, data):
    voi = data[0]
    states = data[1]
    rates = data[2]
    constants = data[3]
    computed_constants = data[4]
    algebraic = data[5]
    externals = data[6]

    algebraic[3] = u[0]
    algebraic[4] = u[1]

    f[0] = algebraic[3]+algebraic[4]+states[0]-0.0
    f[1] = algebraic[3]-algebraic[4]-(computed_constants[205]+computed_constants[206])


def find_root_0(voi, states, rates, constants, computed_constants, algebraic, externals):
    u = [nan]*2

    u[0] = algebraic[3]
    u[1] = algebraic[4]

    u = nla_solve(objective_function_0, u, 2, [voi, states, rates, constants, computed_constants, algebraic, externals])

    algebraic[3] = u[0]
    algebraic[4] = u[1]


def initialise_variables(states, rates, constants, computed_constants, algebraic):
    states[0] = 0.0
    constants[0] = 1.0
    constants[1] = 2.0
    constants[2] = 3.0
    constants[3] = 4.0
    constants[4] = 5.0
    constants[5] = 6.0
    constants[6] = 7.0
    computed_constants[169] = nan
    computed_constants[179] = 123.0
    computed_constants[180] = 123.456789
    computed_constants[181] = 123.0e99
    computed_constants[182] = 123.456789e99
    computed_constants[184] = 1.0
    computed_constants[185] = 0.0
    computed_constants[186] = 2.71828182845905
    computed_constants[187] = 3.14159265358979
    computed_constants[188] = inf
    computed_constants[189] = nan
    computed_constants[205] = 1.0
    computed_constants[206] = 3.0
    algebraic[3] = 1.0
    algebraic[4] = 2.0


def compute_computed_constants(constants, computed_constants):
    computed_constants[0] = eq_func(constants[0], constants[1])
    computed_constants[1] = constants[0]/eq_func(constants[1], constants[1])
    computed_constants[2] = neq_func(constants[0], constants[1])
    computed_constants[3] = constants[0]/neq_func(constants[1], constants[2])
    computed_constants[4] = lt_func(constants[0], constants[1])
    computed_constants[5] = constants[0]/lt_func(constants[1], constants[2])
    computed_constants[6] = leq_func(constants[0], constants[1])
    computed_constants[7] = constants[0]/leq_func(constants[1], constants[2])
    computed_constants[8] = gt_func(constants[0], constants[1])
    computed_constants[9] = constants[0]/gt_func(constants[2], constants[1])
    computed_constants[10] = geq_func(constants[0], constants[1])
    computed_constants[11] = constants[0]/geq_func(constants[2], constants[1])
    computed_constants[12] = and_func(constants[0], constants[1])
    computed_constants[13] = and_func(constants[0], and_func(constants[1], constants[2]))
    computed_constants[14] = and_func(lt_func(constants[0], constants[1]), gt_func(constants[2], constants[3]))
    computed_constants[15] = and_func(constants[0]+constants[1], gt_func(constants[2], constants[3]))
    computed_constants[16] = and_func(constants[0], gt_func(constants[1], constants[2]))
    computed_constants[17] = and_func(constants[0]-constants[1], gt_func(constants[2], constants[3]))
    computed_constants[18] = and_func(-constants[0], gt_func(constants[1], constants[2]))
    computed_constants[19] = and_func(pow(constants[0], constants[1]), gt_func(constants[2], constants[3]))
    computed_constants[20] = and_func(pow(constants[0], 1.0/constants[1]), gt_func(constants[2], constants[3]))
    computed_constants[21] = and_func(lt_func(constants[0], constants[1]), constants[2]+constants[3])
    computed_constants[22] = and_func(lt_func(constants[0], constants[1]), constants[2])
    computed_constants[23] = and_func(lt_func(constants[0], constants[1]), constants[2]-constants[3])
    computed_constants[24] = and_func(lt_func(constants[0], constants[1]), -constants[2])
    computed_constants[25] = and_func(lt_func(constants[0], constants[1]), pow(constants[2], constants[3]))
    computed_constants[26] = and_func(lt_func(constants[0], constants[1]), pow(constants[2], 1.0/constants[3]))
    computed_constants[27] = constants[0]/and_func(constants[1], constants[2])
    computed_constants[28] = or_func(lt_func(constants[0], constants[1]), gt_func(constants[2], constants[3]))
    computed_constants[29] = or_func(constants[0]+constants[1], gt_func(constants[2], constants[3]))
    computed_constants[30] = or_func(constants[0], gt_func(constants[1], constants[2]))
    computed_constants[31] = or_func(constants[0]-constants[1], gt_func(constants[2], constants[3]))
    computed_constants[32] = or_func(-constants[0], gt_func(constants[1], constants[2]))
    computed_constants[33] = or_func(pow(constants[0], constants[1]), gt_func(constants[2], constants[3]))
    computed_constants[34] = or_func(pow(constants[0], 1.0/constants[1]), gt_func(constants[2], constants[3]))
    computed_constants[35] = or_func(lt_func(constants[0], constants[1]), constants[2]+constants[3])
    computed_constants[36] = or_func(lt_func(constants[0], constants[1]), constants[2])
    computed_constants[37] = or_func(lt_func(constants[0], constants[1]), constants[2]-constants[3])
    computed_constants[38] = or_func(lt_func(constants[0], constants[1]), -constants[2])
    computed_constants[39] = or_func(lt_func(constants[0], constants[1]), pow(constants[2], constants[3]))
    computed_constants[40] = or_func(lt_func(constants[0], constants[1]), pow(constants[2], 1.0/constants[3]))
    computed_constants[41] = constants[0]/or_func(constants[1], constants[2])
    computed_constants[42] = XOR_FUNC(constants[0], constants[1])
    computed_constants[43] = XOR_FUNC(constants[0], XOR_FUNC(constants[1], constants[2]))
    computed_constants[44] = XOR_FUNC(lt_func(constants[0], constants[1]), gt_func(constants[2], constants[3]))
    computed_constants[45] = XOR_FUNC(constants[0]+constants[1], gt_func(constants[2], constants[3]))
    computed_constants[46] = XOR_FUNC(constants[0], gt_func(constants[1], constants[2]))
    computed_constants[47] = XOR_FUNC(constants[0]-constants[1], gt_func(constants[2], constants[3]))
    computed_constants[48] = XOR_FUNC(-constants[0], gt_func(constants[1], constants[2]))
    computed_constants[49] = XOR_FUNC(pow(constants[0], constants[1]), gt_func(constants[2], constants[3]))
    computed_constants[50] = XOR_FUNC(pow(constants[0], 1.0/constants[1]), gt_func(constants[2], constants[3]))
    computed_constants[51] = XOR_FUNC(lt_func(constants[0], constants[1]), constants[2]+constants[3])
    computed_constants[52] = XOR_FUNC(lt_func(constants[0], constants[1]), constants[2])
    computed_constants[53] = XOR_FUNC(lt_func(constants[0], constants[1]), constants[2]-constants[3])
    computed_constants[54] = XOR_FUNC(lt_func(constants[0], constants[1]), -constants[2])
    computed_constants[55] = XOR_FUNC(lt_func(constants[0], constants[1]), pow(constants[2], constants[3]))
    computed_constants[56] = XOR_FUNC(lt_func(constants[0], constants[1]), pow(constants[2], 1.0/constants[3]))
    computed_constants[57] = not_func(constants[0])
    computed_constants[58] = constants[0]+constants[1]+constants[2]
    computed_constants[59] = lt_func(constants[0], constants[1])+gt_func(constants[2], constants[3])
    computed_constants[60] = constants[0]
    computed_constants[61] = constants[0]-constants[1]
    computed_constants[62] = lt_func(constants[0], constants[1])-gt_func(constants[2], constants[3])
    computed_constants[63] = lt_func(constants[0], constants[1])-(constants[2]+constants[3])
    computed_constants[64] = lt_func(constants[0], constants[1])-constants[2]
    computed_constants[65] = constants[0]-(-constants[1])
    computed_constants[66] = constants[0]-(-constants[1]*constants[2])
    computed_constants[67] = -constants[0]
    computed_constants[68] = -lt_func(constants[0], constants[1])
    computed_constants[69] = constants[0]*constants[1]
    computed_constants[70] = constants[0]*constants[1]*constants[2]
    computed_constants[71] = lt_func(constants[0], constants[1])*gt_func(constants[2], constants[3])
    computed_constants[72] = (constants[0]+constants[1])*gt_func(constants[2], constants[3])
    computed_constants[73] = constants[0]*gt_func(constants[1], constants[2])
    computed_constants[74] = (constants[0]-constants[1])*gt_func(constants[2], constants[3])
    computed_constants[75] = -constants[0]*gt_func(constants[1], constants[2])
    computed_constants[76] = lt_func(constants[0], constants[1])*(constants[2]+constants[3])
    computed_constants[77] = lt_func(constants[0], constants[1])*constants[2]
    computed_constants[78] = lt_func(constants[0], constants[1])*(constants[2]-constants[3])
    computed_constants[79] = lt_func(constants[0], constants[1])*-constants[2]
    computed_constants[80] = constants[0]/constants[1]
    computed_constants[81] = lt_func(constants[0], constants[1])/gt_func(constants[3], constants[2])
    computed_constants[82] = (constants[0]+constants[1])/gt_func(constants[3], constants[2])
    computed_constants[83] = constants[0]/gt_func(constants[2], constants[1])
    computed_constants[84] = (constants[0]-constants[1])/gt_func(constants[3], constants[2])
    computed_constants[85] = -constants[0]/gt_func(constants[2], constants[1])
    computed_constants[86] = lt_func(constants[0], constants[1])/(constants[2]+constants[3])
    computed_constants[87] = lt_func(constants[0], constants[1])/constants[2]
    computed_constants[88] = lt_func(constants[0], constants[1])/(constants[2]-constants[3])
    computed_constants[89] = lt_func(constants[0], constants[1])/-constants[2]
    computed_constants[90] = lt_func(constants[0], constants[1])/(constants[2]*constants[3])
    computed_constants[91] = lt_func(constants[0], constants[1])/(constants[2]/constants[3])
    computed_constants[92] = sqrt(constants[0])
    computed_constants[93] = pow(constants[0], 2.0)
    computed_constants[94] = pow(constants[0], 3.0)
    computed_constants[95] = pow(constants[0], constants[1])
    computed_constants[96] = pow(leq_func(constants[0], constants[1]), geq_func(constants[2], constants[3]))
    computed_constants[97] = pow(constants[0]+constants[1], geq_func(constants[2], constants[3]))
    computed_constants[98] = pow(constants[0], geq_func(constants[1], constants[2]))
    computed_constants[99] = pow(constants[0]-constants[1], geq_func(constants[2], constants[3]))
    computed_constants[100] = pow(-constants[0], geq_func(constants[1], constants[2]))
    computed_constants[101] = pow(constants[0]*constants[1], geq_func(constants[2], constants[3]))
    computed_constants[102] = pow(constants[0]/constants[1], geq_func(constants[2], constants[3]))
    computed_constants[103] = pow(leq_func(constants[0], constants[1]), constants[2]+constants[3])
    computed_constants[104] = pow(leq_func(constants[0], constants[1]), constants[2])
    computed_constants[105] = pow(leq_func(constants[0], constants[1]), constants[2]-constants[3])
    computed_constants[106] = pow(leq_func(constants[0], constants[1]), -constants[2])
    computed_constants[107] = pow(leq_func(constants[0], constants[1]), constants[2]*constants[3])
    computed_constants[108] = pow(leq_func(constants[0], constants[1]), constants[2]/constants[3])
    computed_constants[109] = pow(leq_func(constants[0], constants[1]), pow(constants[2], constants[3]))
    computed_constants[110] = pow(leq_func(constants[0], constants[1]), pow(constants[2], 1.0/constants[3]))
    computed_constants[111] = sqrt(constants[0])
    computed_constants[112] = sqrt(constants[0])
    computed_constants[113] = pow(constants[0], 1.0/0.5)
    computed_constants[114] = pow(constants[0], 1.0/3.0)
    computed_constants[115] = pow(constants[0], 1.0/constants[1])
    computed_constants[116] = pow(lt_func(constants[0], constants[1]), 1.0/gt_func(constants[3], constants[2]))
    computed_constants[117] = pow(constants[0]+constants[1], 1.0/gt_func(constants[3], constants[2]))
    computed_constants[118] = pow(constants[0], 1.0/gt_func(constants[2], constants[1]))
    computed_constants[119] = pow(constants[0]-constants[1], 1.0/gt_func(constants[3], constants[2]))
    computed_constants[120] = pow(-constants[0], 1.0/gt_func(constants[2], constants[1]))
    computed_constants[121] = pow(constants[0]*constants[1], 1.0/gt_func(constants[3], constants[2]))
    computed_constants[122] = pow(constants[0]/constants[1], 1.0/gt_func(constants[3], constants[2]))
    computed_constants[123] = pow(lt_func(constants[0], constants[1]), 1.0/(constants[2]+constants[3]))
    computed_constants[124] = pow(lt_func(constants[0], constants[1]), 1.0/constants[2])
    computed_constants[125] = pow(lt_func(constants[0], constants[1]), 1.0/(constants[2]-constants[3]))
    computed_constants[126] = pow(lt_func(constants[0], constants[1]), 1.0/-constants[2])
    computed_constants[127] = pow(lt_func(constants[0], constants[1]), 1.0/(constants[2]*constants[3]))
    computed_constants[128] = pow(lt_func(constants[0], constants[1]), 1.0/(constants[2]/constants[3]))
    computed_constants[129] = pow(lt_func(constants[0], constants[1]), 1.0/pow(constants[2], constants[3]))
    computed_constants[130] = pow(lt_func(constants[0], constants[1]), 1.0/pow(constants[2], 1.0/constants[3]))
    computed_constants[131] = fabs(constants[0])
    computed_constants[132] = exp(constants[0])
    computed_constants[133] = log(constants[0])
    computed_constants[134] = log10(constants[0])
    computed_constants[135] = log(constants[0])/log(2.0)
    computed_constants[136] = log10(constants[0])
    computed_constants[137] = log(constants[0])/log(constants[1])
    computed_constants[138] = ceil(constants[0])
    computed_constants[139] = floor(constants[0])
    computed_constants[140] = min(constants[0], constants[1])
    computed_constants[141] = min(constants[0], min(constants[1], constants[2]))
    computed_constants[142] = max(constants[0], constants[1])
    computed_constants[143] = max(constants[0], max(constants[1], constants[2]))
    computed_constants[144] = fmod(constants[0], constants[1])
    computed_constants[145] = sin(constants[0])
    computed_constants[146] = cos(constants[0])
    computed_constants[147] = tan(constants[0])
    computed_constants[148] = sec(constants[0])
    computed_constants[149] = csc(constants[0])
    computed_constants[150] = cot(constants[0])
    computed_constants[151] = sinh(constants[0])
    computed_constants[152] = cosh(constants[0])
    computed_constants[153] = tanh(constants[0])
    computed_constants[154] = sech(constants[0])
    computed_constants[155] = csch(constants[0])
    computed_constants[156] = coth(constants[0])
    computed_constants[157] = asin(constants[0])
    computed_constants[158] = acos(constants[0])
    computed_constants[159] = atan(constants[0])
    computed_constants[160] = asec(constants[0])
    computed_constants[161] = acsc(constants[0])
    computed_constants[162] = acot(constants[0])
    computed_constants[163] = asinh(constants[0])
    computed_constants[164] = acosh(constants[0])
    computed_constants[165] = atanh(constants[0]/2.0)
    computed_constants[166] = asech(constants[0])
    computed_constants[167] = acsch(constants[0])
    computed_constants[168] = acoth(2.0*constants[0])
    computed_constants[170] = constants[0] if gt_func(constants[0], constants[1]) else nan
    computed_constants[171] = constants[0] if lt_func(constants[0], constants[1]) else nan
    computed_constants[172] = constants[0]
    computed_constants[173] = constants[0] if gt_func(constants[0], constants[1]) else constants[2]
    computed_constants[174] = constants[0] if gt_func(constants[0], constants[1]) else constants[2] if gt_func(constants[2], constants[3]) else constants[4] if gt_func(constants[4], constants[5]) else nan
    computed_constants[175] = constants[0] if lt_func(constants[0], constants[1]) else constants[2] if gt_func(constants[2], constants[3]) else constants[4] if gt_func(constants[4], constants[5]) else nan
    computed_constants[176] = constants[0] if gt_func(constants[0], constants[1]) else constants[2] if gt_func(constants[2], constants[3]) else constants[4] if gt_func(constants[4], constants[5]) else constants[6]
    computed_constants[177] = 123.0+(constants[0] if gt_func(constants[0], constants[1]) else nan)
    computed_constants[178] = 123.0+(constants[0] if lt_func(constants[0], constants[1]) else nan)
    computed_constants[183] = constants[0]
    computed_constants[190] = and_func(constants[0], constants[1])+(constants[1] if gt_func(constants[2], constants[3]) else nan)+constants[4]+and_func(constants[5], constants[6])
    computed_constants[191] = and_func(constants[0], constants[1])+(constants[1] if lt_func(constants[2], constants[3]) else nan)+constants[4]+and_func(constants[5], constants[6])
    computed_constants[192] = and_func(constants[0], constants[1])-((constants[1] if gt_func(constants[2], constants[3]) else nan)-(constants[4]-(constants[1] if gt_func(constants[2], constants[3]) else nan)))-and_func(constants[5], constants[6])
    computed_constants[193] = and_func(constants[0], constants[1])-((constants[1] if lt_func(constants[2], constants[3]) else nan)-(constants[4]-(constants[1] if lt_func(constants[2], constants[3]) else nan)))-and_func(constants[5], constants[6])
    computed_constants[194] = and_func(constants[0], constants[1])*(constants[1] if gt_func(constants[2], constants[3]) else nan)*constants[4]*(constants[1] if gt_func(constants[2], constants[3]) else nan)*and_func(constants[5], constants[6])
    computed_constants[195] = and_func(constants[0], constants[1])*(constants[1] if lt_func(constants[2], constants[3]) else nan)*constants[4]*(constants[1] if lt_func(constants[2], constants[3]) else nan)*and_func(constants[5], constants[6])
    computed_constants[196] = and_func(constants[0], constants[1])/((constants[1] if gt_func(constants[2], constants[3]) else nan)/(constants[4]/(constants[1] if gt_func(constants[2], constants[3]) else nan)))
    computed_constants[197] = and_func(constants[0], constants[1])/((constants[1] if lt_func(constants[2], constants[3]) else nan)/(constants[4]/(constants[1] if lt_func(constants[2], constants[3]) else nan)))
    computed_constants[198] = and_func(or_func(constants[0], constants[1]), and_func(XOR_FUNC(constants[0], constants[1]), and_func(constants[1] if gt_func(constants[2], constants[3]) else nan, and_func(and_func(and_func(constants[4], constants[1] if gt_func(constants[2], constants[3]) else nan), XOR_FUNC(constants[0], constants[1])), or_func(constants[0], constants[1])))))
    computed_constants[199] = or_func(and_func(constants[0], constants[1]), or_func(XOR_FUNC(constants[0], constants[1]), or_func(constants[1] if gt_func(constants[2], constants[3]) else nan, or_func(or_func(or_func(constants[4], constants[1] if gt_func(constants[2], constants[3]) else nan), XOR_FUNC(constants[0], constants[1])), and_func(constants[0], constants[1])))))
    computed_constants[200] = XOR_FUNC(and_func(constants[0], constants[1]), XOR_FUNC(or_func(constants[0], constants[1]), XOR_FUNC(constants[1] if gt_func(constants[2], constants[3]) else nan, XOR_FUNC(XOR_FUNC(XOR_FUNC(constants[4], constants[1] if gt_func(constants[2], constants[3]) else nan), or_func(constants[0], constants[1])), and_func(constants[0], constants[1])))))
    computed_constants[201] = pow(and_func(constants[0], constants[1]), pow(constants[1] if gt_func(constants[2], constants[3]) else nan, pow(pow(constants[4], constants[1] if gt_func(constants[2], constants[3]) else nan), and_func(constants[0], constants[1]))))
    computed_constants[202] = pow(pow(pow(and_func(constants[0], constants[1]), 1.0/pow(constants[1] if gt_func(constants[2], constants[3]) else nan, 1.0/constants[4])), 1.0/(constants[1] if gt_func(constants[2], constants[3]) else nan)), 1.0/and_func(constants[0], constants[1]))
    computed_constants[203] = -and_func(constants[0], constants[1])+-(constants[1] if gt_func(constants[2], constants[3]) else nan)
    computed_constants[204] = -and_func(constants[0], constants[1])+-(constants[1] if lt_func(constants[2], constants[3]) else nan)


def compute_rates(voi, states, rates, constants, computed_constants, algebraic, externals, external_variable):
    rates[0] = 1.0


def compute_variables(voi, states, rates, constants, computed_constants, algebraic, externals, external_variable):
    algebraic[0] = or_func(states[0], states[0])
    algebraic[1] = or_func(states[0], or_func(states[0], constants[0]))
    algebraic[2] = constants[0]/XOR_FUNC(constants[1], states[0])
    externals[0] = external_variable(voi, states, rates, constants, computed_constants, algebraic, externals, 0)
    find_root_0(voi, states, rates, constants, computed_constants, algebraic, externals)