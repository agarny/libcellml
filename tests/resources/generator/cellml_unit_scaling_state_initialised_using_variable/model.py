# The content of this file was generated using the Python profile of libCellML 0.5.0.

from enum import Enum
from math import *


__version__ = "0.5.0"
LIBCELLML_VERSION = "0.5.0"

STATE_COUNT = 2
CONSTANT_COUNT = 2
COMPUTED_CONSTANT_COUNT = 0
ALGEBRAIC_COUNT = 0


class VariableType(Enum):
    VARIABLE_OF_INTEGRATION = 0
    STATE = 1
    CONSTANT = 2
    COMPUTED_CONSTANT = 3
    ALGEBRAIC = 4


VOI_INFO = {"name": "t", "units": "ms", "component": "environment", "type": VariableType.VARIABLE_OF_INTEGRATION}

STATE_INFO =     {"name": "k1", "units": "mM", "component": "main", "type": VariableType.STATE},
    {"name": "k2", "units": "M", "component": "main", "type": VariableType.STATE}


CONSTANT_INFO = [
    {"name": "k1_cst", "units": "mM", "component": "constants", "type": VariableType.CONSTANT},
    {"name": "k2_cst", "units": "mM", "component": "constants", "type": VariableType.CONSTANT}
]

COMPUTED_CONSTANT_INFO = [
]

ALGEBRAIC_INFO = [
]


def create_states_array():
    return [nan]*STATE_COUNT


def create_variables_array():
    return [nan]*VARIABLE_COUNT


def initialise_variables(states, rates, constants):
    constants[0] = 123.0
    constants[1] = 789.0
    states[0] = constants[0]
    states[1] = 0.001*constants[1]


def compute_computed_constants(constants, computed_constants):
    pass


def compute_rates(voi, states, rates, constants, computed_constants, algebraic):
    rates[0] = 1.23
    rates[1] = 7.89


def compute_variables(voi, states, rates, constants, computed_constants, algebraic):
    pass
