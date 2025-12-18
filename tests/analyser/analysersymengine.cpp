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

#include "test_utils.h"

#include "gtest/gtest.h"

#include <libcellml>

TEST(Analyser, rearrangeAdditiveEquations)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("analyser/symengine/addition.cellml"));

    EXPECT_EQ(size_t(0), parser->issueCount());

    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    EXPECT_EQ(libcellml::AnalyserModel::Type::ALGEBRAIC, analyser->analyserModel()->type());

    EXPECT_EQ("a = 10.0-(w+x)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(0)->ast()));
    EXPECT_EQ("b = 1.0-(2.0-y)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(1)->ast()));
    EXPECT_EQ("c = -z-(1.0+x)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(2)->ast()));
    EXPECT_EQ("d = y-w", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(3)->ast()));
}

TEST(Analyser, rearrangeMultiplicativeEquations)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("analyser/symengine/multiplication.cellml"));

    EXPECT_EQ(size_t(0), parser->issueCount());

    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    EXPECT_EQ(libcellml::AnalyserModel::Type::ALGEBRAIC, analyser->analyserModel()->type());

    EXPECT_EQ("a = 4.0*pow(w, -1.0)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(0)->ast()));
    EXPECT_EQ("b = 18.0*y", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(1)->ast()));
    EXPECT_EQ("c = 30.0*x*pow(z, -1.0)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(2)->ast()));
}

TEST(Analyser, rearrangeTrigonometricEquations)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("analyser/symengine/trigonometric.cellml"));

    EXPECT_EQ(size_t(0), parser->issueCount());

    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    EXPECT_EQ(libcellml::AnalyserModel::Type::ALGEBRAIC, analyser->analyserModel()->type());

    EXPECT_EQ("a = 1/2.0*(1.0-sin(z1))", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(0)->ast()));
    EXPECT_EQ("b = cos(4.0+z2)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(1)->ast()));
    EXPECT_EQ("c = 2.0+tan(3.0-z3)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(2)->ast()));
    EXPECT_EQ("d = 1/2.0*(1.0-sec(z1))", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(3)->ast()));
    EXPECT_EQ("e = csc(4.0+z2)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(4)->ast()));
    EXPECT_EQ("f = 2.0+cot(3.0-z3)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(5)->ast()));
    EXPECT_EQ("g = 1/2.0*(1.0-sinh(z1))", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(6)->ast()));
    EXPECT_EQ("h = cosh(4.0+z2)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(7)->ast()));
    EXPECT_EQ("i = 2.0+tanh(3.0-z3)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(8)->ast()));
    EXPECT_EQ("j = 1/2.0*(1.0-sech(z1))", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(9)->ast()));
    EXPECT_EQ("k = csch(4.0+z2)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(10)->ast()));
    EXPECT_EQ("l = 2.0+coth(3.0-z3)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(11)->ast()));
    EXPECT_EQ("m = 1/2.0*(1.0-asin(z1))", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(12)->ast()));
    EXPECT_EQ("n = acos(4.0+z2)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(13)->ast()));
    EXPECT_EQ("o = 2.0+atan(3.0-z3)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(14)->ast()));
    EXPECT_EQ("p = 1/2.0*(1.0-asec(z1))", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(15)->ast()));
    EXPECT_EQ("q = acsc(4.0+z2)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(16)->ast()));
    EXPECT_EQ("r = 2.0+acot(3.0-z3)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(17)->ast()));
    EXPECT_EQ("s = 1/2.0*(1.0-asinh(z1))", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(18)->ast()));
    EXPECT_EQ("t = acosh(4.0+z2)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(19)->ast()));
    EXPECT_EQ("u = 2.0+atanh(3.0-z3)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(20)->ast()));
    EXPECT_EQ("v = 1/2.0*(1.0-asech(z1))", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(21)->ast()));
    EXPECT_EQ("w = acsch(4.0+z2)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(22)->ast()));
    EXPECT_EQ("x = 2.0+acoth(3.0-z3)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(23)->ast()));
}

TEST(Analyser, rearrangeEquationsWithConstants)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("analyser/symengine/constants.cellml"));

    EXPECT_EQ(size_t(0), parser->issueCount());

    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    EXPECT_EQ(libcellml::AnalyserModel::Type::ALGEBRAIC, analyser->analyserModel()->type());

    EXPECT_EQ("a = 8.65-x", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(0)->ast()));
    EXPECT_EQ("b = 400000.0*pow(w, -1.0)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(1)->ast()));
    EXPECT_EQ("c = y*2.71828182845905", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(2)->ast()));
    EXPECT_EQ("d = -(-z-3.14159265358979)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(3)->ast()));
    EXPECT_EQ("e = INFINITY-w", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(4)->ast()));
}

TEST(Analyser, rearrangePolynomialEquations)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("analyser/symengine/polynomials.cellml"));

    EXPECT_EQ(size_t(0), parser->issueCount());

    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    EXPECT_EQ(libcellml::AnalyserModel::Type::ALGEBRAIC, analyser->analyserModel()->type());

    EXPECT_EQ("a = -1/3.0*(6.0+15.0*pow(-1.0, 1/3.0))", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(0)->ast()));
    EXPECT_EQ("b = -2.0", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(1)->ast()));
    EXPECT_EQ("c = -1/3.0*(1/2.0*pow(2.0, 2/3.0)*pow(-972.0+pow(947700.0, 1/2.0), 1/3.0)+-9.0*pow(2.0, 1/3.0)*pow(-972.0+pow(947700.0, 1/2.0), -1/3.0))", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(2)->ast()));
    EXPECT_EQ("d = -1/6.0*pow(2.0, 2/3.0)*pow(-27.0*w+27.0*pow(pow(w, 2.0), 1/2.0), 1/3.0)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(3)->ast()));
}

TEST(Analyser, rearrangeExponentialEquations)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("analyser/symengine/exponential.cellml"));

    EXPECT_EQ(size_t(0), parser->issueCount());

    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    EXPECT_EQ(libcellml::AnalyserModel::Type::ALGEBRAIC, analyser->analyserModel()->type());

    EXPECT_EQ("a = pow(2.71828182845905, -(10.0-w))*(1.0+200.0*pow(2.71828182845905, 10.0-w))", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(0)->ast()));
    EXPECT_EQ("b = -(pow(2.71828182845905, 200.0)-pow(2.71828182845905, 10.0))", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(1)->ast()));
}

TEST(Analyser, rearrangeLogarithmicEquations)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("analyser/symengine/logarithmic.cellml"));

    EXPECT_EQ(size_t(0), parser->issueCount());

    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    EXPECT_EQ(libcellml::AnalyserModel::Type::ALGEBRAIC, analyser->analyserModel()->type());

    EXPECT_EQ("a = 5.0-log(x)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(0)->ast()));
    EXPECT_EQ("b = -pow(log(10.0), -1.0)*(y*log(10.0)-log(3.0))", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(1)->ast()));
    EXPECT_EQ("c = pow(log(2.0), -1.0)*(-log(z)+2.5*log(2.0))", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(2)->ast()));
}
