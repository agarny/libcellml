"""
Python bindings for libCellML

The purpose of libCellML is to create, manipulate, serialise, deserialise,
 validate and instantiate CellML models. In the first instance, all serialising
 and deserialising will be to and from standard strings and using the CellML
 XML format.
"""

import libcellml

from libcellml.analyser import Analyser
from libcellml.analyserequation import AnalyserEquation
from libcellml.analyserequationast import AnalyserEquationAst
from libcellml.analyserexternalvariable import AnalyserExternalVariable
from libcellml.analysermodel import AnalyserModel
from libcellml.analyservariable import AnalyserVariable
from libcellml.annotator import Annotator
from libcellml.component import Component
from libcellml.enums import CellmlElementType, cellmlElementTypeAsString
from libcellml.generator import Generator
from libcellml.generatorprofile import GeneratorProfile
from libcellml.importer import Importer
from libcellml.importsource import ImportSource
from libcellml.issue import Issue
from libcellml.model import Model
from libcellml.parser import Parser
from libcellml.printer import Printer
from libcellml.reset import Reset
from libcellml.types import UnitsItem, VariablePair, AnyCellmlElement
from libcellml.units import Units
from libcellml.validator import Validator
from libcellml.variable import Variable
from libcellml.version import version, versionString


# Convert enumerations generated by SWIG

def convert(base, enum, variables, new_base=None):
    converting = False

    class Object:
        pass

    obj = Object()
    for var in variables:
        if hasattr(base, enum + '_' + var):
            # Swig 3.0.3 and higher
            setattr(obj, var, getattr(base, enum + '_' + var))
            delattr(base, enum + '_' + var)
            converting = True

    if converting:
        setattr(base if new_base is None else new_base, enum, obj)


convert(AnalyserEquation, 'Type', [
    'CONSTANT',
    'COMPUTED_CONSTANT',
    'ODE',
    'NLA',
    'ALGEBRAIC',
    'EXTERNAL',
])
convert(AnalyserEquationAst, 'Type', [
    'EQUALITY',

    # Relational and logical operators.

    'EQ',
    'NEQ',
    'LT',
    'LEQ',
    'GT',
    'GEQ',
    'AND',
    'OR',
    'XOR',
    'NOT',

    # Arithmetic operators.

    'PLUS',
    'MINUS',
    'TIMES',
    'DIVIDE',
    'POWER',
    'ROOT',
    'ABS',
    'EXP',
    'LN',
    'LOG',
    'CEILING',
    'FLOOR',
    'MIN',
    'MAX',
    'REM',

    # Calculus elements.

    'DIFF',

    # Trigonometric operators.

    'SIN',
    'COS',
    'TAN',
    'SEC',
    'CSC',
    'COT',
    'SINH',
    'COSH',
    'TANH',
    'SECH',
    'CSCH',
    'COTH',
    'ASIN',
    'ACOS',
    'ATAN',
    'ASEC',
    'ACSC',
    'ACOT',
    'ASINH',
    'ACOSH',
    'ATANH',
    'ASECH',
    'ACSCH',
    'ACOTH',

    # Piecewise statement.

    'PIECEWISE',
    'PIECE',
    'OTHERWISE',

    # Token elements.

    'CI',
    'CN',

    # Qualifier elements.

    'DEGREE',
    'LOGBASE',
    'BVAR',

    # Constants.

    'TRUE',
    'FALSE',
    'E',
    'PI',
    'INF',
    'NAN',
])
convert(AnalyserModel, 'Type', [
    'UNKNOWN',
    'ODE',
    'DAE',
    'NLA',
    'ALGEBRAIC',
    'INVALID',
    'UNDERCONSTRAINED',
    'OVERCONSTRAINED',
    'UNSUITABLY_CONSTRAINED',
])
convert(AnalyserVariable, 'Type', [
    'VARIABLE_OF_INTEGRATION',
    'STATE',
    'CONSTANT',
    'COMPUTED_CONSTANT',
    'ALGEBRAIC',
    'EXTERNAL',
])
convert(GeneratorProfile, 'Profile', [
    'C',
    'PYTHON',
])
convert(Issue, 'Cause', [
    'COMPONENT',
    'CONNECTION',
    'ENCAPSULATION',
    'IMPORT',
    'MATHML',
    'MODEL',
    'RESET',
    'UNDEFINED',
    'UNITS',
    'VARIABLE',
    'XML',
])
convert(Issue, 'Level', [
    'ERROR',
    'WARNING',
    'HINT',
    'MESSAGE',
])
convert(Issue, 'ReferenceRule', [
    'UNDEFINED',
    'XML_UNEXPECTED_ELEMENT',
    'XML_UNEXPECTED_CHARACTER',
    'XML_UNEXPECTED_NAMESPACE',
    'XML_ATTRIBUTE_HAS_NAMESPACE',
    'XML_ID_ATTRIBUTE',
    'MODEL_ELEMENT',
    'MODEL_NAME',
    'MODEL_NAME_VALUE',
    'MODEL_CHILD',
    'MODEL_MORE_THAN_ONE_ENCAPSULATION',
    'IMPORT_ELEMENT',
    'IMPORT_HREF',
    'IMPORT_HREF_LOCATOR',
    'IMPORT_CHILD',
    'IMPORT_EQUIVALENT_INFOSET',
    'IMPORT_UNITS_ELEMENT',
    'IMPORT_UNITS_NAME',
    'IMPORT_UNITS_NAME_VALUE',
    'IMPORT_UNITS_NAME_UNIQUE',
    'IMPORT_UNITS_UNITS_REFERENCE',
    'IMPORT_UNITS_UNITS_REFERENCE_VALUE',
    'IMPORT_UNITS_UNITS_REFERENCE_VALUE_TARGET',
    'IMPORT_COMPONENT_ELEMENT',
    'IMPORT_COMPONENT_NAME',
    'IMPORT_COMPONENT_NAME_VALUE',
    'IMPORT_COMPONENT_NAME_UNIQUE',
    'IMPORT_COMPONENT_COMPONENT_REFERENCE',
    'IMPORT_COMPONENT_COMPONENT_REFERENCE_VALUE',
    'IMPORT_COMPONENT_COMPONENT_REFERENCE_TARGET',
    'UNITS_ELEMENT',
    'UNITS_NAME',
    'UNITS_NAME_VALUE',
    'UNITS_NAME_UNIQUE',
    'UNITS_STANDARD',
    'UNITS_CHILD',
    'UNIT_ELEMENT',
    'UNIT_UNITS',
    'UNIT_UNITS_REFERENCE',
    'UNIT_UNITS_CIRCULAR_REFERENCE',
    'UNIT_ATTRIBUTE_OPTIONAL',
    'UNIT_ATTRIBUTE_PREFIX_VALUE',
    'UNIT_ATTRIBUTE_MULTIPLIER_VALUE',
    'UNIT_ATTRIBUTE_EXPONENT_VALUE',
    'COMPONENT_ELEMENT',
    'COMPONENT_NAME',
    'COMPONENT_NAME_VALUE',
    'COMPONENT_NAME_UNIQUE',
    'COMPONENT_CHILD',
    'VARIABLE_ELEMENT',
    'VARIABLE_ATTRIBUTE_REQUIRED',
    'VARIABLE_NAME_VALUE',
    'VARIABLE_NAME_UNIQUE',
    'VARIABLE_UNITS_VALUE',
    'VARIABLE_ATTRIBUTE_OPTIONAL',
    'VARIABLE_INTERFACE_VALUE',
    'VARIABLE_INITIAL_VALUE_VALUE',
    'RESET_ELEMENT',
    'RESET_ATTRIBUTE_REQUIRED',
    'RESET_VARIABLE_REFERENCE',
    'RESET_TEST_VARIABLE_REFERENCE',
    'RESET_ORDER_VALUE',
    'RESET_ORDER_UNIQUE',
    'RESET_CHILD',
    'RESET_RESET_VALUE_CHILD',
    'RESET_TEST_VALUE_CHILD',
    'TEST_VALUE_ELEMENT',
    'TEST_VALUE_CHILD',
    'RESET_VALUE_ELEMENT',
    'RESET_VALUE_CHILD',
    'MATH_ELEMENT',
    'MATH_MATHML',
    'MATH_CHILD',
    'MATH_CI_VARIABLE_REFERENCE',
    'MATH_CN_UNITS_ATTRIBUTE',
    'MATH_CN_UNITS_ATTRIBUTE_REFERENCE',
    'MATH_CN_BASE10',
    'MATH_CN_FORMAT',
    'ENCAPSULATION_ELEMENT',
    'ENCAPSULATION_CHILD',
    'COMPONENT_REF_ELEMENT',
    'COMPONENT_REF_COMPONENT_ATTRIBUTE',
    'COMPONENT_REF_COMPONENT_ATTRIBUTE_REFERENCE',
    'COMPONENT_REF_COMPONENT_ATTRIBUTE_UNIQUE',
    'COMPONENT_REF_CHILD',
    'CONNECTION_ELEMENT',
    'CONNECTION_COMPONENT1_ATTRIBUTE',
    'CONNECTION_COMPONENT1_ATTRIBUTE_REFERENCE',
    'CONNECTION_COMPONENT2_ATTRIBUTE',
    'CONNECTION_COMPONENT2_ATTRIBUTE_REFERENCE',
    'CONNECTION_EXCLUDE_SELF',
    'CONNECTION_UNIQUE',
    'CONNECTION_CHILD',
    'MAP_VARIABLES_ELEMENT',
    'MAP_VARIABLES_VARIABLE1_ATTRIBUTE',
    'MAP_VARIABLES_VARIABLE1_ATTRIBUTE_REFERENCE',
    'MAP_VARIABLES_VARIABLE2_ATTRIBUTE',
    'MAP_VARIABLES_VARIABLE2_ATTRIBUTE_REFERENCE',
    'MAP_VARIABLES_UNIQUE',
    'DATA_REPR_IDENTIFIER_AT_LEAST_ONE_ALPHANUM',
    'DATA_REPR_IDENTIFIER_BEGIN_EURO_NUM',
    'DATA_REPR_IDENTIFIER_LATIN_ALPHANUM',
    'INVALID_ARGUMENT',
    'IMPORTER_ERROR_IMPORTING_UNITS',
    'IMPORTER_MISSING_FILE',
    'IMPORTER_MISSING_COMPONENT',
    'IMPORTER_MISSING_UNITS',
    'IMPORTER_NULL_MODEL',
    'IMPORTER_UNDEFINED_MODEL',
    'IMPORTER_UNRESOLVED_IMPORTS',
    'ANALYSER_EQUATION_NOT_EQUALITY_STATEMENT',
    'ANALYSER_UNITS',
    'ANALYSER_UNLINKED_UNITS',
    'ANALYSER_VARIABLE_INITIALISED_MORE_THAN_ONCE',
    'ANALYSER_VARIABLE_NON_CONSTANT_INITIALISATION',
    'ANALYSER_VOI_INITIALISED',
    'ANALYSER_VOI_SEVERAL',
    'ANALYSER_ODE_NOT_FIRST_ORDER',
    'ANALYSER_VARIABLE_UNUSED',
    'ANALYSER_STATE_NOT_INITIALISED',
    'ANALYSER_STATE_RATE_AS_ALGEBRAIC',
    'ANALYSER_VARIABLE_COMPUTED_MORE_THAN_ONCE',
    'ANALYSER_EXTERNAL_VARIABLE_DIFFERENT_MODEL',
    'ANALYSER_EXTERNAL_VARIABLE_VOI',
    'ANALYSER_EXTERNAL_VARIABLE_USE_PRIMARY_VARIABLE',
    'ANNOTATOR_ID_NOT_FOUND',
    'ANNOTATOR_ID_NOT_UNIQUE',
    'ANNOTATOR_NO_MODEL',
    'ANNOTATOR_INCONSISTENT_TYPE',
    'ANNOTATOR_NULL_MODEL',
    'UNSPECIFIED'
])
convert(Units, 'Prefix', [
    'YOTTA',
    'ZETTA',
    'EXA',
    'PETA',
    'TERA',
    'GIGA',
    'MEGA',
    'KILO',
    'HECTO',
    'DECA',
    'DECI',
    'CENTI',
    'MILLI',
    'MICRO',
    'NANO',
    'PICO',
    'FEMTO',
    'ATTO',
    'ZEPTO',
    'YOCTO',
], new_base=Units)
convert(Units, 'StandardUnit', [
    'AMPERE',
    'BECQUEREL',
    'CANDELA',
    'COULOMB',
    'DIMENSIONLESS',
    'FARAD',
    'GRAM',
    'GRAY',
    'HENRY',
    'HERTZ',
    'JOULE',
    'KATAL',
    'KELVIN',
    'KILOGRAM',
    'LITRE',
    'LUMEN',
    'LUX',
    'METRE',
    'MOLE',
    'NEWTON',
    'OHM',
    'PASCAL',
    'RADIAN',
    'SECOND',
    'SIEMENS',
    'SIEVERT',
    'STERADIAN',
    'TESLA',
    'VOLT',
    'WATT',
    'WEBER',
])
convert(Variable, 'InterfaceType', [
    'NONE',
    'PRIVATE',
    'PUBLIC',
    'PUBLIC_AND_PRIVATE',
])

del [convert, libcellml]
