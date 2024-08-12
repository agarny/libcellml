# The content of this file was generated using the Python profile of libCellML 0.5.0.

from enum import Enum
from math import *


__version__ = "0.5.0"
LIBCELLML_VERSION = "0.5.0"

STATE_COUNT = 2
CONSTANT_COUNT = 0
COMPUTED_CONSTANT_COUNT = 0
ALGEBRAIC_COUNT = 2


class VariableType(Enum):
    VARIABLE_OF_INTEGRATION = 0
    STATE = 1
    CONSTANT = 2
    COMPUTED_CONSTANT = 3
    ALGEBRAIC = 4


VOI_INFO = {"name": "t", "units": "ms", "component": "environment", "type": VariableType.VARIABLE_OF_INTEGRATION}

STATE_INFO =     {"name": "y", "units": "mM", "component": "circle_y_implementation", "type": VariableType.STATE},
    {"name": "x", "units": "mM", "component": "circle_x", "type": VariableType.STATE}


CONSTANT_INFO = [
]

COMPUTED_CONSTANT_INFO = [
]

ALGEBRAIC_INFO = [
    {"name": "local_complex_maths", "units": "mM", "component": "circle_sibling", "type": VariableType.ALGEBRAIC},
    {"name": "two_x", "units": "milli_mole", "component": "circle_x_sibling", "type": VariableType.ALGEBRAIC}
]


def create_states_array():
    return [nan]*STATE_COUNT


def create_variables_array():
    return [nan]*VARIABLE_COUNT


def initialise_variables(states, rates, constants):
    states[0] = 1.0
    states[1] = 0.0


def compute_computed_constants(constants, computed_constants):
    pass


def compute_rates(voi, states, rates, constants, computed_constants, algebraic):
    rates[1] = -states[0]*1.0
    rates[0] = states[1]*1.0


def compute_variables(voi, states, rates, constants, computed_constants, algebraic):
    algebraic[0] = states[0]+5.0*states[0]/3.0+1.0*exp(states[0]/2.0)
    algebraic[1] = 2.0*states[1]
