/*
Copyright libCellML Contributors

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifdef _WIN32
#    define _USE_MATH_DEFINES
#endif

#include "test_utils.h"

#include "gtest/gtest.h"

#include <libcellml>

#include <cmath>

std::string convertDoubleToString(double value)
{
    std::ostringstream strs;
    strs << std::setprecision(std::numeric_limits<double>::digits10) << value;
    return strs.str();
}

TEST(GeneratorProfile, defaultRelationalAndLogicalOperatorValues)
{
    libcellml::GeneratorProfilePtr generatorProfile = std::make_shared<libcellml::GeneratorProfile>();

    EXPECT_EQ(" = ", generatorProfile->assignmentString());
    EXPECT_EQ(" == ", generatorProfile->eqString());
    EXPECT_EQ(" != ", generatorProfile->neqString());
    EXPECT_EQ(" < ", generatorProfile->ltString());
    EXPECT_EQ(" <= ", generatorProfile->leqString());
    EXPECT_EQ(" > ", generatorProfile->gtString());
    EXPECT_EQ(" >= ", generatorProfile->geqString());
    EXPECT_EQ(" && ", generatorProfile->andString());
    EXPECT_EQ(" || ", generatorProfile->orString());
    EXPECT_EQ("xor", generatorProfile->xorString());
    EXPECT_EQ("!", generatorProfile->notString());

    EXPECT_EQ(true, generatorProfile->hasEqOperator());
    EXPECT_EQ(true, generatorProfile->hasNeqOperator());
    EXPECT_EQ(true, generatorProfile->hasLtOperator());
    EXPECT_EQ(true, generatorProfile->hasLeqOperator());
    EXPECT_EQ(true, generatorProfile->hasGtOperator());
    EXPECT_EQ(true, generatorProfile->hasGeqOperator());
    EXPECT_EQ(true, generatorProfile->hasAndOperator());
    EXPECT_EQ(true, generatorProfile->hasOrOperator());
    EXPECT_EQ(false, generatorProfile->hasXorOperator());
    EXPECT_EQ(true, generatorProfile->hasNotOperator());
}

TEST(GeneratorProfile, defaultArithmeticOperatorValues)
{
    libcellml::GeneratorProfilePtr generatorProfile = std::make_shared<libcellml::GeneratorProfile>();

    EXPECT_EQ("+", generatorProfile->plusString());
    EXPECT_EQ("-", generatorProfile->minusString());
    EXPECT_EQ("*", generatorProfile->timesString());
    EXPECT_EQ("/", generatorProfile->divideString());
    EXPECT_EQ("pow", generatorProfile->powerString());
    EXPECT_EQ("sqrt", generatorProfile->squareRootString());
    EXPECT_EQ("", generatorProfile->squareString());
    EXPECT_EQ("fabs", generatorProfile->absoluteValueString());
    EXPECT_EQ("exp", generatorProfile->exponentialString());
    EXPECT_EQ("log", generatorProfile->napierianLogarithmString());
    EXPECT_EQ("log10", generatorProfile->commonLogarithmString());
    EXPECT_EQ("ceil", generatorProfile->ceilingString());
    EXPECT_EQ("floor", generatorProfile->floorString());
    EXPECT_EQ("min", generatorProfile->minString());
    EXPECT_EQ("max", generatorProfile->maxString());
    EXPECT_EQ("fmod", generatorProfile->remString());

    EXPECT_EQ(false, generatorProfile->hasPowerOperator());
}

TEST(GeneratorProfile, defaultTrigonometricOperatorValues)
{
    libcellml::GeneratorProfilePtr generatorProfile = std::make_shared<libcellml::GeneratorProfile>();

    EXPECT_EQ("sin", generatorProfile->sinString());
    EXPECT_EQ("cos", generatorProfile->cosString());
    EXPECT_EQ("tan", generatorProfile->tanString());
    EXPECT_EQ("sec", generatorProfile->secString());
    EXPECT_EQ("csc", generatorProfile->cscString());
    EXPECT_EQ("cot", generatorProfile->cotString());
    EXPECT_EQ("sinh", generatorProfile->sinhString());
    EXPECT_EQ("cosh", generatorProfile->coshString());
    EXPECT_EQ("tanh", generatorProfile->tanhString());
    EXPECT_EQ("sech", generatorProfile->sechString());
    EXPECT_EQ("csch", generatorProfile->cschString());
    EXPECT_EQ("coth", generatorProfile->cothString());
    EXPECT_EQ("asin", generatorProfile->asinString());
    EXPECT_EQ("acos", generatorProfile->acosString());
    EXPECT_EQ("atan", generatorProfile->atanString());
    EXPECT_EQ("asec", generatorProfile->asecString());
    EXPECT_EQ("acsc", generatorProfile->acscString());
    EXPECT_EQ("acot", generatorProfile->acotString());
    EXPECT_EQ("asinh", generatorProfile->asinhString());
    EXPECT_EQ("acosh", generatorProfile->acoshString());
    EXPECT_EQ("atanh", generatorProfile->atanhString());
    EXPECT_EQ("asech", generatorProfile->asechString());
    EXPECT_EQ("acsch", generatorProfile->acschString());
    EXPECT_EQ("acoth", generatorProfile->acothString());
}

TEST(GeneratorProfile, defaultPiecewiseStatementValues)
{
    libcellml::GeneratorProfilePtr generatorProfile = std::make_shared<libcellml::GeneratorProfile>();

    EXPECT_EQ("(#cond)?#if", generatorProfile->conditionalOperatorIfString());
    EXPECT_EQ(":#else", generatorProfile->conditionalOperatorElseString());
    EXPECT_EQ("", generatorProfile->piecewiseIfString());
    EXPECT_EQ("", generatorProfile->piecewiseElseString());

    EXPECT_EQ(true, generatorProfile->hasConditionalOperator());
}

TEST(GeneratorProfile, defaultConstantValues)
{
    libcellml::GeneratorProfilePtr generatorProfile = std::make_shared<libcellml::GeneratorProfile>();

    EXPECT_EQ("1.0", generatorProfile->trueString());
    EXPECT_EQ("0.0", generatorProfile->falseString());
    EXPECT_EQ(convertDoubleToString(exp(1.0)), generatorProfile->eString());
    EXPECT_EQ(convertDoubleToString(M_PI), generatorProfile->piString());
    EXPECT_EQ("1.0/0.0", generatorProfile->infString());
    EXPECT_EQ("sqrt(-1.0)", generatorProfile->nanString());
}

TEST(GeneratorProfile, defaultMathematicalFunctionValues)
{
    libcellml::GeneratorProfilePtr generatorProfile = std::make_shared<libcellml::GeneratorProfile>();

    EXPECT_EQ("", generatorProfile->eqFunctionString());
    EXPECT_EQ("", generatorProfile->neqFunctionString());
    EXPECT_EQ("", generatorProfile->ltFunctionString());
    EXPECT_EQ("", generatorProfile->leqFunctionString());
    EXPECT_EQ("", generatorProfile->gtFunctionString());
    EXPECT_EQ("", generatorProfile->geqFunctionString());
    EXPECT_EQ("", generatorProfile->andFunctionString());
    EXPECT_EQ("", generatorProfile->orFunctionString());
    EXPECT_EQ("double xor(double x, double y)\n"
              "{\n"
              "    return (x != 0.0) ^ (y != 0.0);\n"
              "}\n",
              generatorProfile->xorFunctionString());
    EXPECT_EQ("", generatorProfile->notFunctionString());

    EXPECT_EQ("double min(double x, double y)\n"
              "{\n"
              "    return (x < y)?x:y;\n"
              "}\n",
              generatorProfile->minFunctionString());
    EXPECT_EQ("double max(double x, double y)\n"
              "{\n"
              "    return (x > y)?x:y;\n"
              "}\n",
              generatorProfile->maxFunctionString());

    EXPECT_EQ("double sec(double x)\n"
              "{\n"
              "    return 1.0/cos(x);\n"
              "}\n",
              generatorProfile->secFunctionString());
    EXPECT_EQ("double csc(double x)\n"
              "{\n"
              "    return 1.0/sin(x);\n"
              "}\n",
              generatorProfile->cscFunctionString());
    EXPECT_EQ("double cot(double x)\n"
              "{\n"
              "    return 1.0/tan(x);\n"
              "}\n",
              generatorProfile->cotFunctionString());
    EXPECT_EQ("double sech(double x)\n"
              "{\n"
              "    return 1.0/cosh(x);\n"
              "}\n",
              generatorProfile->sechFunctionString());
    EXPECT_EQ("double csch(double x)\n"
              "{\n"
              "    return 1.0/sinh(x);\n"
              "}\n",
              generatorProfile->cschFunctionString());
    EXPECT_EQ("double coth(double x)\n"
              "{\n"
              "    return 1.0/tanh(x);\n"
              "}\n",
              generatorProfile->cothFunctionString());
    EXPECT_EQ("double asec(double x)\n"
              "{\n"
              "    return acos(1.0/x);\n"
              "}\n",
              generatorProfile->asecFunctionString());
    EXPECT_EQ("double acsc(double x)\n"
              "{\n"
              "    return asin(1.0/x);\n"
              "}\n",
              generatorProfile->acscFunctionString());
    EXPECT_EQ("double acot(double x)\n"
              "{\n"
              "    return atan(1.0/x);\n"
              "}\n",
              generatorProfile->acotFunctionString());
    EXPECT_EQ("double asech(double x)\n"
              "{\n"
              "    double oneOverX = 1.0/x;\n"
              "\n"
              "    return log(oneOverX+sqrt(oneOverX*oneOverX-1.0));\n"
              "}\n",
              generatorProfile->asechFunctionString());
    EXPECT_EQ("double acsch(double x)\n"
              "{\n"
              "    double oneOverX = 1.0/x;\n"
              "\n"
              "    return log(oneOverX+sqrt(oneOverX*oneOverX+1.0));\n"
              "}\n",
              generatorProfile->acschFunctionString());
    EXPECT_EQ("double acoth(double x)\n"
              "{\n"
              "    double oneOverX = 1.0/x;\n"
              "\n"
              "    return 0.5*log((1.0+oneOverX)/(1.0-oneOverX));\n"
              "}\n",
              generatorProfile->acothFunctionString());
}

TEST(GeneratorProfile, defaultMiscellaneousValues)
{
    libcellml::GeneratorProfilePtr generatorProfile = std::make_shared<libcellml::GeneratorProfile>();

    EXPECT_EQ("void freeVector(double *array)\n{\n   free(array);\n}\n", generatorProfile->freeVectorFunctionString());
    EXPECT_EQ("#include <stddef.h>\n#include <stdlib.h>\n#include <math.h>\n", generatorProfile->headerString());

    EXPECT_EQ("voi", generatorProfile->variableOfIntegrationString());

    EXPECT_EQ("states", generatorProfile->statesArrayString());
    EXPECT_EQ("rates", generatorProfile->ratesArrayString());
    EXPECT_EQ("variables", generatorProfile->variablesArrayString());

    EXPECT_EQ("double *createStateVector()\n{\n", generatorProfile->beginCreateStateVectorMethodString());
    EXPECT_EQ("}\n", generatorProfile->endCreateStateVectorMethodString());

    EXPECT_EQ("double *createRateVector()\n{\n", generatorProfile->beginCreateRateVectorMethodString());
    EXPECT_EQ("}\n", generatorProfile->endCreateRateVectorMethodString());

    EXPECT_EQ("double *createVariableVector()\n{\n", generatorProfile->beginCreateVariableVectorMethodString());
    EXPECT_EQ("}\n", generatorProfile->endCreateVariableVectorMethodString());

    EXPECT_EQ("void initializeConstants(double *states, double *variables)\n{\n", generatorProfile->beginInitializeConstantsMethodString());
    EXPECT_EQ("}\n", generatorProfile->endInitializeConstantsMethodString());

    EXPECT_EQ("void computeComputedConstants(double *variables)\n{\n", generatorProfile->beginComputeComputedConstantsMethodString());
    EXPECT_EQ("}\n", generatorProfile->endComputeComputedConstantsMethodString());

    EXPECT_EQ("void computeRates(double voi, double *states, double *rates, double *variables)\n{\n", generatorProfile->beginComputeRatesMethodString());
    EXPECT_EQ("}\n", generatorProfile->endComputeRatesMethodString());

    EXPECT_EQ("void computeVariables(double voi, double *states, double *rates, double *variables)\n{\n", generatorProfile->beginComputeVariablesMethodString());
    EXPECT_EQ("}\n", generatorProfile->endComputeVariablesMethodString());

    EXPECT_EQ("", generatorProfile->emptyMethodString());

    EXPECT_EQ("const struct VARIABLE_INFO STATE_VECTOR_INFORMATION_ARRAY[] = {\n", generatorProfile->beginStateVectorInformationArrayString());
    EXPECT_EQ("};\n", generatorProfile->endStateVectorInformationArrayString());
    EXPECT_EQ("const struct VARIABLE_INFO VARIABLE_VECTOR_INFORMATION_ARRAY[] = {\n", generatorProfile->beginVariableVectorInformationArrayString());
    EXPECT_EQ("};\n", generatorProfile->endVariableVectorInformationArrayString());

    EXPECT_EQ("    ", generatorProfile->indentString());

    EXPECT_EQ("[", generatorProfile->openArrayString());
    EXPECT_EQ("]", generatorProfile->closeArrayString());

    EXPECT_EQ(",", generatorProfile->arrayElementSeparatorString());
    EXPECT_EQ(";", generatorProfile->commandSeparatorString());

    EXPECT_EQ("/* ", generatorProfile->beginCommentString());
    EXPECT_EQ(" */\n", generatorProfile->endCommentString());
}

TEST(GeneratorProfile, defaultTemplateValues)
{
    libcellml::GeneratorProfilePtr generatorProfile = std::make_shared<libcellml::GeneratorProfile>();

    EXPECT_EQ("VALUE", generatorProfile->templateReplacementString());

    EXPECT_EQ("The contents of this file was generated from version VALUE of libCellML.", generatorProfile->templateOriginCommentString());
    EXPECT_EQ("return (double *)malloc(VALUE * sizeof (double));\n", generatorProfile->templateReturnCreatedArrayString());
    EXPECT_EQ("const size_t STATE_VECTOR_SIZE = VALUE;\n", generatorProfile->templateStateVectorSizeConstantString());
    EXPECT_EQ("{\"VALUE\", \"VALUE\"}", generatorProfile->templateVariableInformationEntryString());
    EXPECT_EQ("struct VARIABLE_INFO {\n    char name[VALUE];\n    char units[VALUE];\n};\n", generatorProfile->templateVariableInformationObjectString());
    EXPECT_EQ("const size_t VARIABLE_VECTOR_SIZE = VALUE;\n", generatorProfile->templateVariableVectorSizeConstantString());
    EXPECT_EQ("const char version[] = \"VALUE\";\n", generatorProfile->templateVersionString());
    EXPECT_EQ("const struct VARIABLE_INFO VOI = {\"VALUE\", \"VALUE\"};\n", generatorProfile->templateVoiInformationString());
}

TEST(GeneratorProfile, relationalAndLogicalOperators)
{
    libcellml::GeneratorProfilePtr generatorProfile = std::make_shared<libcellml::GeneratorProfile>();

    const std::string value = "value";
    const bool trueValue = true;
    const bool falseValue = false;

    generatorProfile->setAssignmentString(value);
    generatorProfile->setEqString(value);
    generatorProfile->setNeqString(value);
    generatorProfile->setLtString(value);
    generatorProfile->setLeqString(value);
    generatorProfile->setGtString(value);
    generatorProfile->setGeqString(value);
    generatorProfile->setAndString(value);
    generatorProfile->setOrString(value);
    generatorProfile->setXorString(value);
    generatorProfile->setNotString(value);

    generatorProfile->setHasEqOperator(falseValue);
    generatorProfile->setHasNeqOperator(falseValue);
    generatorProfile->setHasLtOperator(falseValue);
    generatorProfile->setHasLeqOperator(falseValue);
    generatorProfile->setHasGtOperator(falseValue);
    generatorProfile->setHasGeqOperator(falseValue);
    generatorProfile->setHasAndOperator(falseValue);
    generatorProfile->setHasOrOperator(falseValue);
    generatorProfile->setHasXorOperator(trueValue);
    generatorProfile->setHasNotOperator(falseValue);

    EXPECT_EQ(value, generatorProfile->assignmentString());
    EXPECT_EQ(value, generatorProfile->eqString());
    EXPECT_EQ(value, generatorProfile->neqString());
    EXPECT_EQ(value, generatorProfile->ltString());
    EXPECT_EQ(value, generatorProfile->leqString());
    EXPECT_EQ(value, generatorProfile->gtString());
    EXPECT_EQ(value, generatorProfile->geqString());
    EXPECT_EQ(value, generatorProfile->andString());
    EXPECT_EQ(value, generatorProfile->orString());
    EXPECT_EQ(value, generatorProfile->xorString());
    EXPECT_EQ(value, generatorProfile->notString());

    EXPECT_EQ(falseValue, generatorProfile->hasEqOperator());
    EXPECT_EQ(falseValue, generatorProfile->hasNeqOperator());
    EXPECT_EQ(falseValue, generatorProfile->hasLtOperator());
    EXPECT_EQ(falseValue, generatorProfile->hasLeqOperator());
    EXPECT_EQ(falseValue, generatorProfile->hasGtOperator());
    EXPECT_EQ(falseValue, generatorProfile->hasGeqOperator());
    EXPECT_EQ(falseValue, generatorProfile->hasAndOperator());
    EXPECT_EQ(falseValue, generatorProfile->hasOrOperator());
    EXPECT_EQ(trueValue, generatorProfile->hasXorOperator());
    EXPECT_EQ(falseValue, generatorProfile->hasNotOperator());
}

TEST(GeneratorProfile, arithmeticOperators)
{
    libcellml::GeneratorProfilePtr generatorProfile = std::make_shared<libcellml::GeneratorProfile>();

    const std::string value = "value";
    const bool trueValue = true;

    generatorProfile->setPlusString(value);
    generatorProfile->setMinusString(value);
    generatorProfile->setTimesString(value);
    generatorProfile->setDivideString(value);
    generatorProfile->setPowerString(value);
    generatorProfile->setSquareRootString(value);
    generatorProfile->setSquareString(value);
    generatorProfile->setAbsoluteValueString(value);
    generatorProfile->setExponentialString(value);
    generatorProfile->setNapierianLogarithmString(value);
    generatorProfile->setCommonLogarithmString(value);
    generatorProfile->setCeilingString(value);
    generatorProfile->setFloorString(value);
    generatorProfile->setMinString(value);
    generatorProfile->setMaxString(value);
    generatorProfile->setRemString(value);

    generatorProfile->setHasPowerOperator(trueValue);

    EXPECT_EQ(value, generatorProfile->plusString());
    EXPECT_EQ(value, generatorProfile->minusString());
    EXPECT_EQ(value, generatorProfile->timesString());
    EXPECT_EQ(value, generatorProfile->divideString());
    EXPECT_EQ(value, generatorProfile->powerString());
    EXPECT_EQ(value, generatorProfile->squareRootString());
    EXPECT_EQ(value, generatorProfile->squareString());
    EXPECT_EQ(value, generatorProfile->absoluteValueString());
    EXPECT_EQ(value, generatorProfile->exponentialString());
    EXPECT_EQ(value, generatorProfile->napierianLogarithmString());
    EXPECT_EQ(value, generatorProfile->commonLogarithmString());
    EXPECT_EQ(value, generatorProfile->ceilingString());
    EXPECT_EQ(value, generatorProfile->floorString());
    EXPECT_EQ(value, generatorProfile->minString());
    EXPECT_EQ(value, generatorProfile->maxString());
    EXPECT_EQ(value, generatorProfile->remString());

    EXPECT_EQ(trueValue, generatorProfile->hasPowerOperator());
}

TEST(GeneratorProfile, trigonometricOperators)
{
    libcellml::GeneratorProfilePtr generatorProfile = std::make_shared<libcellml::GeneratorProfile>();

    const std::string value = "value";

    generatorProfile->setSinString(value);
    generatorProfile->setCosString(value);
    generatorProfile->setTanString(value);
    generatorProfile->setSecString(value);
    generatorProfile->setCscString(value);
    generatorProfile->setCotString(value);
    generatorProfile->setSinhString(value);
    generatorProfile->setCoshString(value);
    generatorProfile->setTanhString(value);
    generatorProfile->setSechString(value);
    generatorProfile->setCschString(value);
    generatorProfile->setCothString(value);
    generatorProfile->setAsinString(value);
    generatorProfile->setAcosString(value);
    generatorProfile->setAtanString(value);
    generatorProfile->setAsecString(value);
    generatorProfile->setAcscString(value);
    generatorProfile->setAcotString(value);
    generatorProfile->setAsinhString(value);
    generatorProfile->setAcoshString(value);
    generatorProfile->setAtanhString(value);
    generatorProfile->setAsechString(value);
    generatorProfile->setAcschString(value);
    generatorProfile->setAcothString(value);

    EXPECT_EQ(value, generatorProfile->sinString());
    EXPECT_EQ(value, generatorProfile->cosString());
    EXPECT_EQ(value, generatorProfile->tanString());
    EXPECT_EQ(value, generatorProfile->secString());
    EXPECT_EQ(value, generatorProfile->cscString());
    EXPECT_EQ(value, generatorProfile->cotString());
    EXPECT_EQ(value, generatorProfile->sinhString());
    EXPECT_EQ(value, generatorProfile->coshString());
    EXPECT_EQ(value, generatorProfile->tanhString());
    EXPECT_EQ(value, generatorProfile->sechString());
    EXPECT_EQ(value, generatorProfile->cschString());
    EXPECT_EQ(value, generatorProfile->cothString());
    EXPECT_EQ(value, generatorProfile->asinString());
    EXPECT_EQ(value, generatorProfile->acosString());
    EXPECT_EQ(value, generatorProfile->atanString());
    EXPECT_EQ(value, generatorProfile->asecString());
    EXPECT_EQ(value, generatorProfile->acscString());
    EXPECT_EQ(value, generatorProfile->acotString());
    EXPECT_EQ(value, generatorProfile->asinhString());
    EXPECT_EQ(value, generatorProfile->acoshString());
    EXPECT_EQ(value, generatorProfile->atanhString());
    EXPECT_EQ(value, generatorProfile->asechString());
    EXPECT_EQ(value, generatorProfile->acschString());
    EXPECT_EQ(value, generatorProfile->acothString());
}

TEST(GeneratorProfile, piecewiseStatement)
{
    libcellml::GeneratorProfilePtr generatorProfile = std::make_shared<libcellml::GeneratorProfile>();

    const std::string value = "value";
    const bool falseValue = true;

    generatorProfile->setConditionalOperatorIfString(value);
    generatorProfile->setConditionalOperatorElseString(value);
    generatorProfile->setPiecewiseIfString(value);
    generatorProfile->setPiecewiseElseString(value);

    generatorProfile->setHasConditionalOperator(falseValue);

    EXPECT_EQ(value, generatorProfile->conditionalOperatorIfString());
    EXPECT_EQ(value, generatorProfile->conditionalOperatorElseString());
    EXPECT_EQ(value, generatorProfile->piecewiseIfString());
    EXPECT_EQ(value, generatorProfile->piecewiseElseString());

    EXPECT_EQ(falseValue, generatorProfile->hasConditionalOperator());
}

TEST(GeneratorProfile, constants)
{
    libcellml::GeneratorProfilePtr generatorProfile = std::make_shared<libcellml::GeneratorProfile>();

    const std::string value = "value";

    generatorProfile->setTrueString(value);
    generatorProfile->setFalseString(value);
    generatorProfile->setEString(value);
    generatorProfile->setPiString(value);
    generatorProfile->setInfString(value);
    generatorProfile->setNanString(value);

    EXPECT_EQ(value, generatorProfile->trueString());
    EXPECT_EQ(value, generatorProfile->falseString());
    EXPECT_EQ(value, generatorProfile->eString());
    EXPECT_EQ(value, generatorProfile->piString());
    EXPECT_EQ(value, generatorProfile->infString());
    EXPECT_EQ(value, generatorProfile->nanString());
}

TEST(GeneratorProfile, mathematicalFunctions)
{
    libcellml::GeneratorProfilePtr generatorProfile = std::make_shared<libcellml::GeneratorProfile>();

    const std::string value = "value";

    generatorProfile->setEqFunctionString(value);
    generatorProfile->setNeqFunctionString(value);
    generatorProfile->setLtFunctionString(value);
    generatorProfile->setLeqFunctionString(value);
    generatorProfile->setGtFunctionString(value);
    generatorProfile->setGeqFunctionString(value);
    generatorProfile->setAndFunctionString(value);
    generatorProfile->setOrFunctionString(value);
    generatorProfile->setXorFunctionString(value);
    generatorProfile->setNotFunctionString(value);

    generatorProfile->setMinFunctionString(value);
    generatorProfile->setMaxFunctionString(value);

    generatorProfile->setSecFunctionString(value);
    generatorProfile->setCscFunctionString(value);
    generatorProfile->setCotFunctionString(value);
    generatorProfile->setSechFunctionString(value);
    generatorProfile->setCschFunctionString(value);
    generatorProfile->setCothFunctionString(value);
    generatorProfile->setAsecFunctionString(value);
    generatorProfile->setAcscFunctionString(value);
    generatorProfile->setAcotFunctionString(value);
    generatorProfile->setAsechFunctionString(value);
    generatorProfile->setAcschFunctionString(value);
    generatorProfile->setAcothFunctionString(value);

    EXPECT_EQ(value, generatorProfile->eqFunctionString());
    EXPECT_EQ(value, generatorProfile->neqFunctionString());
    EXPECT_EQ(value, generatorProfile->ltFunctionString());
    EXPECT_EQ(value, generatorProfile->leqFunctionString());
    EXPECT_EQ(value, generatorProfile->gtFunctionString());
    EXPECT_EQ(value, generatorProfile->geqFunctionString());
    EXPECT_EQ(value, generatorProfile->andFunctionString());
    EXPECT_EQ(value, generatorProfile->orFunctionString());
    EXPECT_EQ(value, generatorProfile->xorFunctionString());
    EXPECT_EQ(value, generatorProfile->notFunctionString());

    EXPECT_EQ(value, generatorProfile->minFunctionString());
    EXPECT_EQ(value, generatorProfile->maxFunctionString());

    EXPECT_EQ(value, generatorProfile->secFunctionString());
    EXPECT_EQ(value, generatorProfile->cscFunctionString());
    EXPECT_EQ(value, generatorProfile->cotFunctionString());
    EXPECT_EQ(value, generatorProfile->sechFunctionString());
    EXPECT_EQ(value, generatorProfile->cschFunctionString());
    EXPECT_EQ(value, generatorProfile->cothFunctionString());
    EXPECT_EQ(value, generatorProfile->asecFunctionString());
    EXPECT_EQ(value, generatorProfile->acscFunctionString());
    EXPECT_EQ(value, generatorProfile->acotFunctionString());
    EXPECT_EQ(value, generatorProfile->asechFunctionString());
    EXPECT_EQ(value, generatorProfile->acschFunctionString());
    EXPECT_EQ(value, generatorProfile->acothFunctionString());
}

TEST(GeneratorProfile, miscellaneous)
{
    libcellml::GeneratorProfilePtr generatorProfile = std::make_shared<libcellml::GeneratorProfile>();

    const std::string value = "value";

    generatorProfile->setFreeVectorFunctionString(value);
    generatorProfile->setHeaderString(value);

    generatorProfile->setVariableOfIntegrationString(value);

    generatorProfile->setStatesArrayString(value);
    generatorProfile->setRatesArrayString(value);
    generatorProfile->setVariablesArrayString(value);

    generatorProfile->setBeginCreateStateVectorMethodString(value);
    generatorProfile->setEndCreateStateVectorMethodString(value);
    generatorProfile->setBeginCreateRateVectorMethodString(value);
    generatorProfile->setEndCreateRateVectorMethodString(value);
    generatorProfile->setBeginCreateVariableVectorMethodString(value);
    generatorProfile->setEndCreateVariableVectorMethodString(value);

    generatorProfile->setBeginInitializeConstantsMethodString(value);
    generatorProfile->setEndInitializeConstantsMethodString(value);

    generatorProfile->setBeginComputeComputedConstantsMethodString(value);
    generatorProfile->setEndComputeComputedConstantsMethodString(value);

    generatorProfile->setBeginComputeRatesMethodString(value);
    generatorProfile->setEndComputeRatesMethodString(value);

    generatorProfile->setBeginComputeVariablesMethodString(value);
    generatorProfile->setEndComputeVariablesMethodString(value);

    generatorProfile->setEmptyMethodString(value);

    generatorProfile->setBeginStateVectorInformationArrayString(value);
    generatorProfile->setEndStateVectorInformationArrayString(value);
    generatorProfile->setBeginVariableVectorInformationArrayString(value);
    generatorProfile->setEndVariableVectorInformationArrayString(value);

    generatorProfile->setIndentString(value);

    generatorProfile->setOpenArrayString(value);
    generatorProfile->setCloseArrayString(value);

    generatorProfile->setArrayElementSeparatorString(value);
    generatorProfile->setCommandSeparatorString(value);

    generatorProfile->setBeginCommentString(value);
    generatorProfile->setEndCommentString(value);

    EXPECT_EQ(value, generatorProfile->freeVectorFunctionString());
    EXPECT_EQ(value, generatorProfile->headerString());

    EXPECT_EQ(value, generatorProfile->variableOfIntegrationString());

    EXPECT_EQ(value, generatorProfile->statesArrayString());
    EXPECT_EQ(value, generatorProfile->ratesArrayString());
    EXPECT_EQ(value, generatorProfile->variablesArrayString());

    EXPECT_EQ(value, generatorProfile->beginCreateStateVectorMethodString());
    EXPECT_EQ(value, generatorProfile->endCreateStateVectorMethodString());
    EXPECT_EQ(value, generatorProfile->beginCreateRateVectorMethodString());
    EXPECT_EQ(value, generatorProfile->endCreateRateVectorMethodString());
    EXPECT_EQ(value, generatorProfile->beginCreateVariableVectorMethodString());
    EXPECT_EQ(value, generatorProfile->endCreateVariableVectorMethodString());

    EXPECT_EQ(value, generatorProfile->beginInitializeConstantsMethodString());
    EXPECT_EQ(value, generatorProfile->endInitializeConstantsMethodString());

    EXPECT_EQ(value, generatorProfile->beginComputeComputedConstantsMethodString());
    EXPECT_EQ(value, generatorProfile->endComputeComputedConstantsMethodString());

    EXPECT_EQ(value, generatorProfile->beginComputeRatesMethodString());
    EXPECT_EQ(value, generatorProfile->endComputeRatesMethodString());

    EXPECT_EQ(value, generatorProfile->beginComputeVariablesMethodString());
    EXPECT_EQ(value, generatorProfile->endComputeVariablesMethodString());

    EXPECT_EQ(value, generatorProfile->emptyMethodString());

    EXPECT_EQ(value, generatorProfile->beginStateVectorInformationArrayString());
    EXPECT_EQ(value, generatorProfile->endStateVectorInformationArrayString());
    EXPECT_EQ(value, generatorProfile->beginVariableVectorInformationArrayString());
    EXPECT_EQ(value, generatorProfile->endVariableVectorInformationArrayString());

    EXPECT_EQ(value, generatorProfile->indentString());

    EXPECT_EQ(value, generatorProfile->openArrayString());
    EXPECT_EQ(value, generatorProfile->closeArrayString());

    EXPECT_EQ(value, generatorProfile->arrayElementSeparatorString());
    EXPECT_EQ(value, generatorProfile->commandSeparatorString());

    EXPECT_EQ(value, generatorProfile->beginCommentString());
    EXPECT_EQ(value, generatorProfile->endCommentString());
}

TEST(GeneratorProfile, templateValues)
{
    libcellml::GeneratorProfilePtr generatorProfile = std::make_shared<libcellml::GeneratorProfile>();

    const std::string value = "placeholder";

    generatorProfile->setTemplateReplacementString(value);

    EXPECT_EQ(value, generatorProfile->templateReplacementString());

    generatorProfile->setTemplateOriginCommentString(value);
    generatorProfile->setTemplateReturnCreatedArrayString(value);
    generatorProfile->setTemplateStateVectorSizeConstantString(value);
    generatorProfile->setTemplateVariableInformationEntryString(value);
    generatorProfile->setTemplateVariableInformationObjectString(value);
    generatorProfile->setTemplateVariableVectorSizeConstantString(value);
    generatorProfile->setTemplateVersionString(value);
    generatorProfile->setTemplateVoiInformationString(value);

    EXPECT_EQ(value, generatorProfile->templateOriginCommentString());
    EXPECT_EQ(value, generatorProfile->templateReturnCreatedArrayString());
    EXPECT_EQ(value, generatorProfile->templateStateVectorSizeConstantString());
    EXPECT_EQ(value, generatorProfile->templateVariableInformationEntryString());
    EXPECT_EQ(value, generatorProfile->templateVariableInformationObjectString());
    EXPECT_EQ(value, generatorProfile->templateVariableVectorSizeConstantString());
    EXPECT_EQ(value, generatorProfile->templateVersionString());
    EXPECT_EQ(value, generatorProfile->templateVoiInformationString());
}
