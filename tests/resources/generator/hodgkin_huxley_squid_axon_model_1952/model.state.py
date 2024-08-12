# The content of this file was generated using the Python profile of libCellML 0.5.0.

from enum import Enum
from math import *


__version__ = "0.5.0"
LIBCELLML_VERSION = "0.5.0"

STATE_COUNT = 3
CONSTANT_COUNT = 5
COMPUTED_CONSTANT_COUNT = 3
ALGEBRAIC_COUNT = 10
EXTERNAL_COUNT = 1


class VariableType(Enum):
    VARIABLE_OF_INTEGRATION = 0
    STATE = 1
    CONSTANT = 2
    COMPUTED_CONSTANT = 3
    ALGEBRAIC = 4
    EXTERNAL = 5


VOI_INFO = {"name": "time", "units": "millisecond", "component": "environment", "type": VariableType.VARIABLE_OF_INTEGRATION}

STATE_INFO =     {"name": "V", "units": "millivolt", "component": "membrane", "type": VariableType.STATE},
    {"name": "h", "units": "dimensionless", "component": "sodium_channel_h_gate", "type": VariableType.STATE},
    {"name": "n", "units": "dimensionless", "component": "potassium_channel_n_gate", "type": VariableType.STATE}


CONSTANT_INFO = [
    {"name": "Cm", "units": "microF_per_cm2", "component": "membrane", "type": VariableType.CONSTANT},
    {"name": "E_R", "units": "millivolt", "component": "membrane", "type": VariableType.CONSTANT},
    {"name": "g_L", "units": "milliS_per_cm2", "component": "leakage_current", "type": VariableType.CONSTANT},
    {"name": "g_Na", "units": "milliS_per_cm2", "component": "sodium_channel", "type": VariableType.CONSTANT},
    {"name": "g_K", "units": "milliS_per_cm2", "component": "potassium_channel", "type": VariableType.CONSTANT}
]

COMPUTED_CONSTANT_INFO = [
    {"name": "E_L", "units": "millivolt", "component": "leakage_current", "type": VariableType.COMPUTED_CONSTANT},
    {"name": "E_Na", "units": "millivolt", "component": "sodium_channel", "type": VariableType.COMPUTED_CONSTANT},
    {"name": "E_K", "units": "millivolt", "component": "potassium_channel", "type": VariableType.COMPUTED_CONSTANT}
]

ALGEBRAIC_INFO = [
    {"name": "i_Stim", "units": "microA_per_cm2", "component": "membrane", "type": VariableType.ALGEBRAIC},
    {"name": "i_L", "units": "microA_per_cm2", "component": "leakage_current", "type": VariableType.ALGEBRAIC},
    {"name": "i_K", "units": "microA_per_cm2", "component": "potassium_channel", "type": VariableType.ALGEBRAIC},
    {"name": "i_Na", "units": "microA_per_cm2", "component": "sodium_channel", "type": VariableType.ALGEBRAIC},
    {"name": "alpha_m", "units": "per_millisecond", "component": "sodium_channel_m_gate", "type": VariableType.ALGEBRAIC},
    {"name": "beta_m", "units": "per_millisecond", "component": "sodium_channel_m_gate", "type": VariableType.ALGEBRAIC},
    {"name": "alpha_h", "units": "per_millisecond", "component": "sodium_channel_h_gate", "type": VariableType.ALGEBRAIC},
    {"name": "beta_h", "units": "per_millisecond", "component": "sodium_channel_h_gate", "type": VariableType.ALGEBRAIC},
    {"name": "alpha_n", "units": "per_millisecond", "component": "potassium_channel_n_gate", "type": VariableType.ALGEBRAIC},
    {"name": "beta_n", "units": "per_millisecond", "component": "potassium_channel_n_gate", "type": VariableType.ALGEBRAIC}
]

EXTERNAL_INFO = [
    {"name": "m", "units": "dimensionless", "component": "sodium_channel_m_gate", "type": VariableType.EXTERNAL}
]


def leq_func(x, y):
    return 1.0 if x <= y else 0.0


def geq_func(x, y):
    return 1.0 if x >= y else 0.0


def and_func(x, y):
    return 1.0 if bool(x) & bool(y) else 0.0


def create_states_array():
    return [nan]*STATE_COUNT


def create_variables_array():
    return [nan]*VARIABLE_COUNT


def initialise_variables(voi, states, rates, constants, external_variable):
    constants[0] = 1.0
    constants[1] = 0.0
    constants[2] = 0.3
    constants[3] = 120.0
    constants[4] = 36.0
    states[0] = 0.0
    states[1] = 0.6
    states[2] = 0.325
    algebraic[4] = external_variable(voi, states, rates, variables, 4)


def compute_computed_constants(constants, computed_constants):
    computed_constants[0] = constants[1]-10.613
    computed_constants[1] = constants[1]-115.0
    computed_constants[2] = constants[1]+12.0


def compute_rates(voi, states, rates, constants, computed_constants, algebraic, external_variable):
    algebraic[0] = -20.0 if and_func(geq_func(voi, 10.0), leq_func(voi, 10.5)) else 0.0
    algebraic[1] = constants[2]*(states[0]-computed_constants[0])
    algebraic[2] = constants[4]*pow(states[2], 4.0)*(states[0]-computed_constants[2])
    algebraic[4] = external_variable(voi, states, rates, variables, 4)
    algebraic[3] = constants[3]*pow(algebraic[4], 3.0)*states[1]*(states[0]-computed_constants[1])
    rates[0] = -(-algebraic[0]+algebraic[3]+algebraic[2]+algebraic[1])/constants[0]
    algebraic[7] = 0.07*exp(states[0]/20.0)
    algebraic[8] = 1.0/(exp((states[0]+30.0)/10.0)+1.0)
    rates[1] = algebraic[7]*(1.0-states[1])-algebraic[8]*states[1]
    algebraic[9] = 0.01*(states[0]+10.0)/(exp((states[0]+10.0)/10.0)-1.0)
    algebraic[10] = 0.125*exp(states[0]/80.0)
    rates[2] = algebraic[9]*(1.0-states[2])-algebraic[10]*states[2]


def compute_variables(voi, states, rates, constants, computed_constants, algebraic, external_variable):
    algebraic[1] = constants[2]*(states[0]-computed_constants[0])
    algebraic[4] = external_variable(voi, states, rates, variables, 4)
    algebraic[3] = constants[3]*pow(algebraic[4], 3.0)*states[1]*(states[0]-computed_constants[1])
    algebraic[5] = 0.1*(states[0]+25.0)/(exp((states[0]+25.0)/10.0)-1.0)
    algebraic[6] = 4.0*exp(states[0]/18.0)
    algebraic[7] = 0.07*exp(states[0]/20.0)
    algebraic[8] = 1.0/(exp((states[0]+30.0)/10.0)+1.0)
    algebraic[2] = constants[4]*pow(states[2], 4.0)*(states[0]-computed_constants[2])
    algebraic[9] = 0.01*(states[0]+10.0)/(exp((states[0]+10.0)/10.0)-1.0)
    algebraic[10] = 0.125*exp(states[0]/80.0)
