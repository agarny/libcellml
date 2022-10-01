# The content of this file was generated using the Python profile of libCellML 0.3.104.

from enum import Enum
from math import *


__version__ = "0.3.0"
LIBCELLML_VERSION = "0.3.104"

STATE_COUNT = 1
VARIABLE_COUNT = 2


class VariableType(Enum):
    VARIABLE_OF_INTEGRATION = 1
    STATE = 2
    CONSTANT = 3
    COMPUTED_CONSTANT = 4
    ALGEBRAIC = 5


VOI_INFO = {"name": "t", "units": "ms", "component": "environment", "type": VariableType.VARIABLE_OF_INTEGRATION}

STATE_INFO = [
    {"name": "k", "units": "mM", "component": "states", "type": VariableType.STATE}
]

VARIABLE_INFO = [
    {"name": "x", "units": "mM", "component": "main", "type": VariableType.ALGEBRAIC},
    {"name": "y", "units": "M", "component": "main", "type": VariableType.ALGEBRAIC}
]


def create_states_array():
    return [nan]*STATE_COUNT


def create_variables_array():
    return [nan]*VARIABLE_COUNT


def initialise_variables(states, variables):
    states[0] = 123.0


def compute_computed_constants(variables):
    pass


def compute_rates(voi, states, rates, variables):
    rates[0] = 1.23


def compute_variables(voi, states, rates, variables):
    variables[0] = states[0]+states[0]
    variables[1] = 0.001*states[0]+0.001*states[0]
