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
    EXPECT_EQ("c = 30.0*x*pow(z, -1.0)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(2)->ast()));
}

TEST(AnalyserSymEngine, rearrangeTrigonometricEquations)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("analyser/symengine/trigonometric.cellml"));

    EXPECT_EQ(size_t(0), parser->issueCount());

    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    EXPECT_EQ(libcellml::AnalyserModel::Type::ALGEBRAIC, analyser->analyserModel()->type());

    EXPECT_EQ("a = 1.0/2.0*(1.0-sin(w))", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(0)->ast()));
    EXPECT_EQ("b = cos(4.0+x)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(1)->ast()));
    EXPECT_EQ("c = 2.0+tan(3.0-y)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(2)->ast()));
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
    EXPECT_EQ("c = y*2.71828182845905", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(2)->ast()));
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
    EXPECT_EQ("d = -1.0/6.0*pow(2.0, 2.0/3.0)*pow(-27.0*w+27.0*pow(pow(w, 2.0), 1.0/2.0), 1.0/3.0)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(3)->ast()));
#else
    EXPECT_EQ("d = -1.0/6.0*pow(2.0, 2.0/3.0)*pow(27.0*pow(pow(w, 2.0), 1.0/2.0)+-27.0*w, 1.0/3.0)", libcellml::Generator::equationCode(analyser->analyserModel()->analyserEquation(3)->ast()));
#endif
}
