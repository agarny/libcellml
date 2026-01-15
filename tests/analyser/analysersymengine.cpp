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

TEST(AnalyserSymEngine, rearrangeAdditiveEquations)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("analyser/symengine/addition.cellml"));

    EXPECT_EQ(size_t(0), parser->issueCount());

    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    EXPECT_EQ(libcellml::AnalyserModel::Type::ALGEBRAIC, analyser->analyserModel()->type());

    EXPECT_EQ("a = 10.0-(w+x)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(0)->ast()));
    EXPECT_EQ("b = 1.0-(2.0-y)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(1)->ast()));
#ifdef _WIN32
    EXPECT_EQ("c = -z-(1.0+x)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(2)->ast()));
    EXPECT_EQ("d = y-w", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(3)->ast()));
#else
    EXPECT_EQ("c = -(1.0+x)-z", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(2)->ast()));
    EXPECT_EQ("d = -w+y", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(3)->ast()));
#endif
}

TEST(AnalyserSymEngine, rearrangeMultiplicativeEquations)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("analyser/symengine/multiplication.cellml"));

    EXPECT_EQ(size_t(0), parser->issueCount());

    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    EXPECT_EQ(libcellml::AnalyserModel::Type::ALGEBRAIC, analyser->analyserModel()->type());

    EXPECT_EQ("a = 4.0*pow(w, -1.0)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(0)->ast()));
    EXPECT_EQ("b = 18.0*y", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(1)->ast()));
    EXPECT_EQ("c = 30.0*pow(z, -1.0)*x", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(2)->ast()));
}

TEST(AnalyserSymEngine, rearrangeTrigonometricEquations)
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

TEST(AnalyserSymEngine, rearrangeEquationsWithConstants)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("analyser/symengine/constants.cellml"));

    EXPECT_EQ(size_t(0), parser->issueCount());

    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    EXPECT_EQ(libcellml::AnalyserModel::Type::ALGEBRAIC, analyser->analyserModel()->type());

    EXPECT_EQ("a = 8.65-x", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(0)->ast()));
    EXPECT_EQ("b = 400000.0*pow(w, -1.0)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(1)->ast()));
    EXPECT_EQ("c = 2.71828182845905*y", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(2)->ast()));
#ifdef __linux__
    EXPECT_EQ("d = -(-3.14159265358979-z)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(3)->ast()));
#else
    EXPECT_EQ("d = -(-z-3.14159265358979)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(3)->ast()));
#endif
    EXPECT_EQ("e = INFINITY-w", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(4)->ast()));
}

TEST(AnalyserSymEngine, rearrangePolynomialEquations)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("analyser/symengine/polynomials.cellml"));

    EXPECT_EQ(size_t(0), parser->issueCount());

    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    EXPECT_EQ(libcellml::AnalyserModel::Type::ALGEBRAIC, analyser->analyserModel()->type());

    EXPECT_EQ("a = 3.0", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(0)->ast()));
    EXPECT_EQ("b = -2.0", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(1)->ast()));
    EXPECT_EQ("c = 3.0", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(2)->ast()));
#ifdef _WIN32
    EXPECT_EQ("d = -1/6.0*pow(2.0, 2/3.0)*pow(-27.0*w+27.0*pow(pow(w, 2.0), 1/2.0), 1/3.0)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(3)->ast()));
#else
    EXPECT_EQ("d = -1.0/6.0*pow(2.0, 2.0/3.0)*pow(27.0*pow(pow(w, 2.0), 1.0/2.0)+-27.0*w, 1.0/3.0)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(3)->ast()));
#endif
}

TEST(AnalyserSymEngine, rearrangeExponentialEquations)
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

TEST(AnalyserSymEngine, rearrangeLogarithmicEquations)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("analyser/symengine/logarithmic.cellml"));

    EXPECT_EQ(size_t(0), parser->issueCount());

    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    EXPECT_EQ(libcellml::AnalyserModel::Type::ALGEBRAIC, analyser->analyserModel()->type());

    EXPECT_EQ("a = 5.0-log(x)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(0)->ast()));
    EXPECT_EQ("b = -pow(log(10.0), -1.0)*(log(10.0)*y-log(3.0))", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(1)->ast()));
    EXPECT_EQ("c = pow(log(2.0), -1.0)*(2.5*log(2.0)-log(z))", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(2)->ast()));
}

TEST(AnalyserSymEngine, rearrangeUncommonArithmeticEquations)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("analyser/symengine/uncommon_arithmetic.cellml"));

    EXPECT_EQ(size_t(0), parser->issueCount());

    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    EXPECT_EQ(libcellml::AnalyserModel::Type::ALGEBRAIC, analyser->analyserModel()->type());

    EXPECT_EQ("a = 2.0-pow(w, 1/2.0)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(0)->ast()));
    EXPECT_EQ("b = pow(w, -1/4.0)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(1)->ast()));
    EXPECT_EQ("c = 3.0*fabs(-x+y)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(2)->ast()));
    EXPECT_EQ("d = w-ceil(0.4+x)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(3)->ast()));
    EXPECT_EQ("e = 1.0+floor(1/2.0*z)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(4)->ast()));
    EXPECT_EQ("f = 1/5.0*min(y, x)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(5)->ast()));
    EXPECT_EQ("g = w*pow(max(z, y), -1.0)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(6)->ast()));
    EXPECT_EQ("h = -fmod(z, w)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(7)->ast()));
}

TEST(AnalyserSymEngine, rearrangeDifferentialEquations)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("analyser/symengine/differential.cellml"));

    EXPECT_EQ(size_t(0), parser->issueCount());

    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    EXPECT_EQ(libcellml::AnalyserModel::Type::ODE, analyser->analyserModel()->type());

    EXPECT_EQ("x = -dy/dt", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(0)->ast()));
}

TEST(AnalyserSymEngine, unrearrangeableEquations)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("analyser/symengine/unrearrangeable.cellml"));

    EXPECT_EQ(size_t(0), parser->issueCount());

    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    EXPECT_EQ(libcellml::AnalyserModel::Type::NLA, analyser->analyserModel()->type());

    EXPECT_EQ("2.0*x1+sin(a)-1.0", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(0)->ast()));
    EXPECT_EQ("csc(4.0+b)-x2-0.0", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(1)->ast()));
    EXPECT_EQ("x1-tanh(3.0-c)-2.0", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(2)->ast()));
    // EXPECT_EQ("sech(d)+x2-1.0", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(3)->ast()));
    EXPECT_EQ("acos(e)-x1-0.5", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(3)->ast()));
    EXPECT_EQ("acot(f+2.0)-x2", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(4)->ast()));
    EXPECT_EQ("x1+asinh(g)-1.0", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(5)->ast()));
    EXPECT_EQ("acsch(h-1.0)-x1-0.0", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(6)->ast()));
    EXPECT_EQ("pow(i, 2.0)-3.0*i-2.0-0.0", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(7)->ast()));
    EXPECT_EQ("log(j)-x1", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(8)->ast()));
    EXPECT_EQ("x2-log10(k)-0.0", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(9)->ast()));
    EXPECT_EQ("exp(l)+x1-3.0", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(10)->ast()));
    EXPECT_EQ("pow(m, 2.5)-30.0", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(11)->ast()));
    EXPECT_EQ("pow(2.0, n)-16.0", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(12)->ast()));
    EXPECT_EQ("x2-((x1 == 0.0)?x1:o)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(13)->ast()));
    EXPECT_EQ("p*exp(p)-x3", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(14)->ast()));
    EXPECT_EQ("fabs(q)-x1-0.0", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(15)->ast()));
    EXPECT_EQ("ceil(r)-5.0", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(16)->ast()));
    EXPECT_EQ("floor(s)+x2-3.0", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(17)->ast()));
    EXPECT_EQ("min(t, x1)-x2", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(18)->ast()));
    EXPECT_EQ("max(u, 2.0)-x1-1.0", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(19)->ast()));
    EXPECT_EQ("fmod(v, 3.0)-x2", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(20)->ast()));
}

TEST(AnalyserSymEngine, breakAlgebraicLoop)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("analyser/symengine/simple_capillary.cellml"));

    EXPECT_EQ(size_t(0), parser->issueCount());

    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    EXPECT_EQ(libcellml::AnalyserModel::Type::ODE, analyser->analyserModel()->type());

    EXPECT_EQ("v_z = v_in-v_y", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(0)->ast()));
    EXPECT_EQ("P_x = P_out+P_R", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(1)->ast()));
    EXPECT_EQ("v_y = -pow(-R*C-C*R_v, -1.0)*(C*(P_out+R*v_in)-q)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(2)->ast()));
    EXPECT_EQ("P_R = v_z*R", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(3)->ast()));
    EXPECT_EQ("P_R_v = v_y*R_v", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(4)->ast()));
    EXPECT_EQ("dq/dt = v_y", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(5)->ast()));
    EXPECT_EQ("P_C = q/C", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(6)->ast()));
}

TEST(AnalyserSymEngine, breakTwoIndependentAlgebraicLoops)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("analyser/symengine/winograd_destexhe_sanchezvives_2008.cellml"));

    EXPECT_EQ(size_t(0), parser->issueCount());

    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    EXPECT_EQ(libcellml::AnalyserModel::Type::ODE, analyser->analyserModel()->type());

    // TODO Add checks for the rearranged equations once implemented.
}

TEST(Analyser, break2dLinearSystem)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("analyser/symengine/linear_system_2d.cellml"));

    EXPECT_EQ(size_t(0), parser->issueCount());

    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    EXPECT_EQ(libcellml::AnalyserModel::Type::ALGEBRAIC, analyser->analyserModel()->type());

    // TODO Add checks for the rearranged equations once implemented.
}

TEST(Analyser, break3dLinearSystem)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("analyser/symengine/linear_system_3d.cellml"));

    EXPECT_EQ(size_t(0), parser->issueCount());

    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    EXPECT_EQ(libcellml::AnalyserModel::Type::ALGEBRAIC, analyser->analyserModel()->type());

    // TODO Add checks for the rearranged equations once implemented.
}

TEST(Analyser, break4dLinearSystem)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("analyser/symengine/linear_system_4d.cellml"));

    EXPECT_EQ(size_t(0), parser->issueCount());

    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    EXPECT_EQ(libcellml::AnalyserModel::Type::ALGEBRAIC, analyser->analyserModel()->type());

    // TODO Add checks for the rearranged equations once implemented.
}