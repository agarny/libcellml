from math import *

def initializeConstants(states, variables):
    states[0] = 0.0

def computeComputedConstants(variables):
    pass

def computeRates(voi, states, rates, variables):
    rates[0] = 1.0

def computeVariables(voi, states, rates, variables):
    variables[0] = 2.0*voi
    variables[1] = 3.0*variables[0]