# The content of this file was generated using the Python profile of libCellML 0.5.0.

from enum import Enum
from math import *


__version__ = "0.5.0"
LIBCELLML_VERSION = "0.5.0"

CONSTANT_COUNT = 1
COMPUTED_CONSTANT_COUNT = 2
ALGEBRAIC_COUNT = 0


class VariableType(Enum):
    CONSTANT = 0
    COMPUTED_CONSTANT = 1
    ALGEBRAIC = 2


CONSTANT_INFO = [
    {"name": "k", "units": "mM", "component": "constants", "type": VariableType.CONSTANT}
]

COMPUTED_CONSTANT_INFO = [
    {"name": "x", "units": "mM", "component": "main", "type": VariableType.COMPUTED_CONSTANT},
    {"name": "y", "units": "M", "component": "main", "type": VariableType.COMPUTED_CONSTANT}
]

ALGEBRAIC_INFO = [
]


def create_variables_array():
    return [nan]*VARIABLE_COUNT


def initialise_variables(constants):
    constants[0] = 123.0


def compute_computed_constants(constants, computed_constants):
    computed_constants[0] = constants[0]+constants[0]
    computed_constants[1] = 0.001*constants[0]+0.001*constants[0]


def compute_variables(constants, computed_constants, algebraic):
    pass
