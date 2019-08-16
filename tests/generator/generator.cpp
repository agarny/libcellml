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

static const std::string EMPTY_STRING;

TEST(Generator, emptyModel)
{
    libcellml::ModelPtr model = std::make_shared<libcellml::Model>();
    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(size_t(0), generator.errorCount());

    EXPECT_EQ(libcellml::Generator::ModelType::UNKNOWN, generator.modelType());

    EXPECT_EQ(size_t(0), generator.stateCount());
    EXPECT_EQ(size_t(0), generator.variableCount());

    EXPECT_EQ(nullptr, generator.variableOfIntegration());
    EXPECT_EQ(nullptr, generator.state(0));
    EXPECT_EQ(nullptr, generator.variable(0));

    EXPECT_EQ(EMPTY_STRING, generator.code());
}

TEST(Generator, initialized_variable_of_integration)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/initialized_variable_of_integration.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    const std::vector<std::string> expectedErrors = {
        "Variable 'time' in component 'my_component' of model 'initialized_variable_of_integration' cannot be both a variable of integration and initialised."};

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(expectedErrors.size(), generator.errorCount());

    for (size_t i = 0; i < generator.errorCount(); ++i) {
        EXPECT_EQ(expectedErrors.at(i), generator.error(i)->description());
        EXPECT_EQ(libcellml::Error::Kind::GENERATOR, generator.error(i)->kind());
    }

    EXPECT_EQ(libcellml::Generator::ModelType::INVALID, generator.modelType());

    EXPECT_EQ(size_t(0), generator.stateCount());
    EXPECT_EQ(size_t(0), generator.variableCount());

    EXPECT_EQ(nullptr, generator.variableOfIntegration());
    EXPECT_EQ(nullptr, generator.state(0));
    EXPECT_EQ(nullptr, generator.variable(0));

    EXPECT_EQ(EMPTY_STRING, generator.code());
}

TEST(Generator, two_variables_of_integration)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/two_variables_of_integration.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    const std::vector<std::string> expectedErrors = {
        "Variable 'time' in component 'main' of model 'two_variables_of_integration' and variable 'other_time' in component 'sub_sub_sub' of model 'two_variables_of_integration' cannot both be a variable of integration."};

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(expectedErrors.size(), generator.errorCount());

    for (size_t i = 0; i < generator.errorCount(); ++i) {
        EXPECT_EQ(expectedErrors.at(i), generator.error(i)->description());
        EXPECT_EQ(libcellml::Error::Kind::GENERATOR, generator.error(i)->kind());
    }

    EXPECT_EQ(libcellml::Generator::ModelType::INVALID, generator.modelType());

    EXPECT_EQ(size_t(0), generator.stateCount());
    EXPECT_EQ(size_t(0), generator.variableCount());

    EXPECT_EQ(nullptr, generator.variableOfIntegration());
    EXPECT_EQ(nullptr, generator.state(0));
    EXPECT_EQ(nullptr, generator.variable(0));

    EXPECT_EQ(EMPTY_STRING, generator.code());
}

TEST(Generator, non_first_order_odes)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/non_first_order_odes.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    const std::vector<std::string> expectedErrors = {
        "The differential equation for variable 'x' in component 'main' of model 'non_first_order_odes' must be of the first order.",
        "The differential equation for variable 'y' in component 'sub' of model 'non_first_order_odes' must be of the first order.",
        "The differential equation for variable 'z' in component 'sub_sub' of model 'non_first_order_odes' must be of the first order."};

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(expectedErrors.size(), generator.errorCount());

    for (size_t i = 0; i < generator.errorCount(); ++i) {
        EXPECT_EQ(expectedErrors.at(i), generator.error(i)->description());
        EXPECT_EQ(libcellml::Error::Kind::GENERATOR, generator.error(i)->kind());
    }

    EXPECT_EQ(libcellml::Generator::ModelType::INVALID, generator.modelType());

    EXPECT_EQ(size_t(0), generator.stateCount());
    EXPECT_EQ(size_t(0), generator.variableCount());

    EXPECT_EQ(nullptr, generator.variableOfIntegration());
    EXPECT_EQ(nullptr, generator.state(0));
    EXPECT_EQ(nullptr, generator.variable(0));

    EXPECT_EQ(EMPTY_STRING, generator.code());
}

TEST(Generator, undefined_variables)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/undefined_variables.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    const std::vector<std::string> expectedErrors = {
        "Variable 'a' in component 'my_component' of model 'undefined_variables' is referenced in an equation, but it is not defined anywhere.",
        "Variable 'b' in component 'my_component' of model 'undefined_variables' is referenced in an equation, but it is not defined anywhere."};

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(expectedErrors.size(), generator.errorCount());

    for (size_t i = 0; i < generator.errorCount(); ++i) {
        EXPECT_EQ(expectedErrors.at(i), generator.error(i)->description());
    }

    EXPECT_EQ(libcellml::Generator::ModelType::INVALID, generator.modelType());

    EXPECT_EQ(size_t(0), generator.stateCount());
    EXPECT_EQ(size_t(0), generator.variableCount());

    EXPECT_EQ(nullptr, generator.variableOfIntegration());
    EXPECT_EQ(nullptr, generator.state(0));
    EXPECT_EQ(nullptr, generator.variable(0));

    EXPECT_EQ(EMPTY_STRING, generator.code());
}

TEST(Generator, variable_initialized_twice)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/variable_initialized_twice.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    const std::vector<std::string> expectedErrors = {
        "Variable 'x' in component 'sub' of model 'variable_initialized_twice' and variable 'x' in component 'main' of model 'variable_initialized_twice' are equivalent and cannot therefore both be initialised."};

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(expectedErrors.size(), generator.errorCount());

    for (size_t i = 0; i < generator.errorCount(); ++i) {
        EXPECT_EQ(expectedErrors.at(i), generator.error(i)->description());
    }

    EXPECT_EQ(libcellml::Generator::ModelType::INVALID, generator.modelType());

    EXPECT_EQ(size_t(0), generator.stateCount());
    EXPECT_EQ(size_t(0), generator.variableCount());

    EXPECT_EQ(nullptr, generator.variableOfIntegration());
    EXPECT_EQ(nullptr, generator.state(0));
    EXPECT_EQ(nullptr, generator.variable(0));

    EXPECT_EQ(EMPTY_STRING, generator.code());
}

TEST(Generator, non_initialized_state)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/non_initialized_state.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    const std::vector<std::string> expectedErrors = {
        "Variable 'x' in component 'my_component' of model 'non_initialized_state' is used in an ODE, but it is not initialised."};

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(expectedErrors.size(), generator.errorCount());

    for (size_t i = 0; i < generator.errorCount(); ++i) {
        EXPECT_EQ(expectedErrors.at(i), generator.error(i)->description());
    }

    EXPECT_EQ(libcellml::Generator::ModelType::UNDERCONSTRAINED, generator.modelType());

    EXPECT_EQ(size_t(0), generator.stateCount());
    EXPECT_EQ(size_t(0), generator.variableCount());

    EXPECT_EQ(nullptr, generator.variableOfIntegration());
    EXPECT_EQ(nullptr, generator.state(0));
    EXPECT_EQ(nullptr, generator.variable(0));

    EXPECT_EQ(EMPTY_STRING, generator.code());
}

TEST(Generator, underconstrained)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/underconstrained.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    const std::vector<std::string> expectedErrors = {
        "Variable 'x' in component 'my_component' of model 'my_model' is not computed."};

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(expectedErrors.size(), generator.errorCount());

    for (size_t i = 0; i < generator.errorCount(); ++i) {
        EXPECT_EQ(expectedErrors.at(i), generator.error(i)->description());
    }

    EXPECT_EQ(libcellml::Generator::ModelType::UNDERCONSTRAINED, generator.modelType());

    EXPECT_EQ(size_t(0), generator.stateCount());
    EXPECT_EQ(size_t(0), generator.variableCount());

    EXPECT_EQ(nullptr, generator.variableOfIntegration());
    EXPECT_EQ(nullptr, generator.state(0));
    EXPECT_EQ(nullptr, generator.variable(0));

    EXPECT_EQ(EMPTY_STRING, generator.code());
}

TEST(Generator, overconstrained)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/overconstrained.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    const std::vector<std::string> expectedErrors = {
        "Variable 'x' in component 'my_component' of model 'my_model' is computed more than once."};

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(expectedErrors.size(), generator.errorCount());

    for (size_t i = 0; i < generator.errorCount(); ++i) {
        EXPECT_EQ(expectedErrors.at(i), generator.error(i)->description());
    }

    EXPECT_EQ(libcellml::Generator::ModelType::OVERCONSTRAINED, generator.modelType());

    EXPECT_EQ(size_t(0), generator.stateCount());
    EXPECT_EQ(size_t(0), generator.variableCount());

    EXPECT_EQ(nullptr, generator.variableOfIntegration());
    EXPECT_EQ(nullptr, generator.state(0));
    EXPECT_EQ(nullptr, generator.variable(0));

    EXPECT_EQ(EMPTY_STRING, generator.code());
}

TEST(Generator, unsuitably_constrained)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/unsuitably_constrained.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    const std::vector<std::string> expectedErrors = {
        "Variable 'x' in component 'my_component' of model 'my_model' is not computed.",
        "Variable 'y' in component 'my_component' of model 'my_model' is computed more than once."};

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(expectedErrors.size(), generator.errorCount());

    for (size_t i = 0; i < generator.errorCount(); ++i) {
        EXPECT_EQ(expectedErrors.at(i), generator.error(i)->description());
    }

    EXPECT_EQ(libcellml::Generator::ModelType::UNSUITABLY_CONSTRAINED, generator.modelType());

    EXPECT_EQ(size_t(0), generator.stateCount());
    EXPECT_EQ(size_t(0), generator.variableCount());

    EXPECT_EQ(nullptr, generator.variableOfIntegration());
    EXPECT_EQ(nullptr, generator.state(0));
    EXPECT_EQ(nullptr, generator.variable(0));

    EXPECT_EQ(EMPTY_STRING, generator.code());
}

TEST(Generator, invalidReplacementString)
{
    const std::vector<std::string> expectedErrors1 = {
        "The template origin comment string does not contain exactly one replacement string 'PLACEHOLDER'.",
        "The template return create array string does not contain exactly one replacement string 'PLACEHOLDER'.",
        "The template state vector size string does not contain exactly one replacement string 'PLACEHOLDER'.",
        "The template variable information entry string does not contain exactly two replacement strings 'PLACEHOLDER'.",
        "The template variable information object string does not contain exactly two replacement strings 'PLACEHOLDER'.",
        "The template variable vector size constant string does not contain exactly one replacement string 'PLACEHOLDER'.",
        "The template verstion string does not contain exactly one replacement string 'PLACEHOLDER'.",
        "The template voi information string does not contain exactly two replacement strings 'PLACEHOLDER'.",
        "Unable to generate code with a profile that is not valid."
    };

    const std::vector<std::string> expectedErrors2 = {
        "The template verstion string does not contain exactly one replacement string 'PLACEHOLDER'.",
        "The template voi information string does not contain exactly two replacement strings 'PLACEHOLDER'.",
        "Unable to generate code with a profile that is not valid."
    };

    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/template_replacement/model.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(size_t(0), generator.errorCount());

    libcellml::GeneratorProfilePtr profile = std::make_shared<libcellml::GeneratorProfile>(libcellml::GeneratorProfile::Profile::PYTHON);

    profile->setTemplateReplacementString("");

    generator.setProfile(profile);

    EXPECT_EQ("", generator.code());

    profile->setTemplateOriginCommentString("This string has no template replacement.");
    profile->setTemplateVariableInformationObjectString("This string doesn't have a placeholder or PLACEHOLDER either!");
    profile->setTemplateReplacementString("PLACEHOLDER");


    generator.clearErrors();
    EXPECT_EQ("", generator.code());

    EXPECT_EQ(9, generator.errorCount());
    for (size_t i = 0; i < generator.errorCount(); ++i) {
        EXPECT_EQ(expectedErrors1.at(i), generator.error(i)->description());
        EXPECT_EQ(libcellml::Error::Kind::GENERATOR, generator.error(i)->kind());
    }

    profile->setTemplateOriginCommentString("This sting has PLACEHOLDER place holder.");
    profile->setTemplateReturnCreatedArrayString("test PLACEHOLDER");
    profile->setTemplateStateVectorSizeConstantString("test state vector PLACEHOLDER");
    profile->setTemplateVariableInformationEntryString("test variable info. entry PLACEHOLDER again PLACEHOLDER");
    profile->setTemplateVariableInformationObjectString("test variable info. object PLACEHOLDER another PLACEHOLDER");
    profile->setTemplateVariableVectorSizeConstantString("test variable vector PLACEHOLDER");
    profile->setTemplateVersionString("This string has four PLACEHOLDER PLACEHOLDER< PLACEHOLDER, PLACEHOLDER text.");
    profile->setTemplateVoiInformationString("test voi PLACEHOLDER");

    generator.clearErrors();
    EXPECT_EQ("", generator.code());

    EXPECT_EQ(3, generator.errorCount());
    for (size_t i = 0; i < generator.errorCount(); ++i) {
        EXPECT_EQ(expectedErrors2.at(i), generator.error(i)->description());
        EXPECT_EQ(libcellml::Error::Kind::GENERATOR, generator.error(i)->kind());
    }
}

TEST(Generator, algebraic_eqn_computed_var_on_rhs)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/algebraic_eqn_computed_var_on_rhs/model.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(size_t(0), generator.errorCount());

    EXPECT_EQ(libcellml::Generator::ModelType::ALGEBRAIC, generator.modelType());

    EXPECT_EQ(size_t(0), generator.stateCount());
    EXPECT_EQ(size_t(2), generator.variableCount());

    EXPECT_EQ(nullptr, generator.variableOfIntegration());
    EXPECT_EQ(nullptr, generator.state(0));
    EXPECT_NE(nullptr, generator.variable(0));
    EXPECT_EQ(nullptr, generator.variable(generator.variableCount()));

    EXPECT_EQ(fileContents("generator/algebraic_eqn_computed_var_on_rhs/code.c"), generator.code());

    libcellml::GeneratorProfilePtr profile = std::make_shared<libcellml::GeneratorProfile>(libcellml::GeneratorProfile::Profile::PYTHON);

    generator.setProfile(profile);

    EXPECT_EQ(fileContents("generator/algebraic_eqn_computed_var_on_rhs/code.py"), generator.code());
}

TEST(Generator, algebraic_eqn_const_var_on_rhs)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/algebraic_eqn_const_var_on_rhs/model.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(size_t(0), generator.errorCount());

    EXPECT_EQ(libcellml::Generator::ModelType::ALGEBRAIC, generator.modelType());

    EXPECT_EQ(size_t(0), generator.stateCount());
    EXPECT_EQ(size_t(2), generator.variableCount());

    EXPECT_EQ(nullptr, generator.variableOfIntegration());
    EXPECT_EQ(nullptr, generator.state(0));
    EXPECT_NE(nullptr, generator.variable(0));
    EXPECT_EQ(nullptr, generator.variable(generator.variableCount()));

    EXPECT_EQ(fileContents("generator/algebraic_eqn_const_var_on_rhs/code.c"), generator.code());

    libcellml::GeneratorProfilePtr profile = std::make_shared<libcellml::GeneratorProfile>(libcellml::GeneratorProfile::Profile::PYTHON);

    generator.setProfile(profile);

    EXPECT_EQ(fileContents("generator/algebraic_eqn_const_var_on_rhs/code.py"), generator.code());
}

TEST(Generator, algebraic_eqn_constant_on_rhs)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/algebraic_eqn_constant_on_rhs/model.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(size_t(0), generator.errorCount());

    EXPECT_EQ(libcellml::Generator::ModelType::ALGEBRAIC, generator.modelType());

    EXPECT_EQ(size_t(0), generator.stateCount());
    EXPECT_EQ(size_t(1), generator.variableCount());

    EXPECT_EQ(nullptr, generator.variableOfIntegration());
    EXPECT_EQ(nullptr, generator.state(0));
    EXPECT_NE(nullptr, generator.variable(0));
    EXPECT_EQ(nullptr, generator.variable(generator.variableCount()));

    EXPECT_EQ(fileContents("generator/algebraic_eqn_constant_on_rhs/code.c"), generator.code());

    libcellml::GeneratorProfilePtr profile = std::make_shared<libcellml::GeneratorProfile>(libcellml::GeneratorProfile::Profile::PYTHON);

    generator.setProfile(profile);

    EXPECT_EQ(fileContents("generator/algebraic_eqn_constant_on_rhs/code.py"), generator.code());
}

TEST(Generator, algebraic_eqn_derivative_on_rhs)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/algebraic_eqn_derivative_on_rhs/model.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(size_t(0), generator.errorCount());

    EXPECT_EQ(libcellml::Generator::ModelType::ODE, generator.modelType());

    EXPECT_EQ(size_t(1), generator.stateCount());
    EXPECT_EQ(size_t(2), generator.variableCount());

    EXPECT_NE(nullptr, generator.variableOfIntegration());
    EXPECT_NE(nullptr, generator.state(0));
    EXPECT_EQ(nullptr, generator.state(generator.stateCount()));
    EXPECT_NE(nullptr, generator.variable(0));
    EXPECT_EQ(nullptr, generator.variable(generator.variableCount()));

    EXPECT_EQ(fileContents("generator/algebraic_eqn_derivative_on_rhs/code.c"), generator.code());

    libcellml::GeneratorProfilePtr profile = std::make_shared<libcellml::GeneratorProfile>(libcellml::GeneratorProfile::Profile::PYTHON);

    generator.setProfile(profile);

    EXPECT_EQ(fileContents("generator/algebraic_eqn_derivative_on_rhs/code.py"), generator.code());
}

TEST(Generator, algebraic_eqn_derivative_on_rhs_one_component)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/algebraic_eqn_derivative_on_rhs_one_component/model.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(size_t(0), generator.errorCount());

    EXPECT_EQ(libcellml::Generator::ModelType::ODE, generator.modelType());

    EXPECT_EQ(size_t(1), generator.stateCount());
    EXPECT_EQ(size_t(2), generator.variableCount());

    EXPECT_NE(nullptr, generator.variableOfIntegration());
    EXPECT_NE(nullptr, generator.state(0));
    EXPECT_EQ(nullptr, generator.state(generator.stateCount()));
    EXPECT_NE(nullptr, generator.variable(0));
    EXPECT_EQ(nullptr, generator.variable(generator.variableCount()));

    EXPECT_EQ(fileContents("generator/algebraic_eqn_derivative_on_rhs_one_component/code.c"), generator.code());

    libcellml::GeneratorProfilePtr profile = std::make_shared<libcellml::GeneratorProfile>(libcellml::GeneratorProfile::Profile::PYTHON);

    generator.setProfile(profile);

    EXPECT_EQ(fileContents("generator/algebraic_eqn_derivative_on_rhs_one_component/code.py"), generator.code());
}

TEST(Generator, algebraic_eqn_state_var_on_rhs)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/algebraic_eqn_state_var_on_rhs/model.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(size_t(0), generator.errorCount());

    EXPECT_EQ(libcellml::Generator::ModelType::ODE, generator.modelType());

    EXPECT_EQ(size_t(1), generator.stateCount());
    EXPECT_EQ(size_t(2), generator.variableCount());

    EXPECT_NE(nullptr, generator.variableOfIntegration());
    EXPECT_NE(nullptr, generator.state(0));
    EXPECT_EQ(nullptr, generator.state(generator.stateCount()));
    EXPECT_NE(nullptr, generator.variable(0));
    EXPECT_EQ(nullptr, generator.variable(generator.variableCount()));

    EXPECT_EQ(fileContents("generator/algebraic_eqn_state_var_on_rhs/code.c"), generator.code());

    libcellml::GeneratorProfilePtr profile = std::make_shared<libcellml::GeneratorProfile>(libcellml::GeneratorProfile::Profile::PYTHON);

    generator.setProfile(profile);

    EXPECT_EQ(fileContents("generator/algebraic_eqn_state_var_on_rhs/code.py"), generator.code());
}

TEST(Generator, algebraic_eqn_state_var_on_rhs_one_component)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/algebraic_eqn_state_var_on_rhs_one_component/model.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(size_t(0), generator.errorCount());

    EXPECT_EQ(libcellml::Generator::ModelType::ODE, generator.modelType());

    EXPECT_EQ(size_t(1), generator.stateCount());
    EXPECT_EQ(size_t(2), generator.variableCount());

    EXPECT_NE(nullptr, generator.variableOfIntegration());
    EXPECT_NE(nullptr, generator.state(0));
    EXPECT_EQ(nullptr, generator.state(generator.stateCount()));
    EXPECT_NE(nullptr, generator.variable(0));
    EXPECT_EQ(nullptr, generator.variable(generator.variableCount()));

    EXPECT_EQ(fileContents("generator/algebraic_eqn_state_var_on_rhs_one_component/code.c"), generator.code());

    libcellml::GeneratorProfilePtr profile = std::make_shared<libcellml::GeneratorProfile>(libcellml::GeneratorProfile::Profile::PYTHON);

    generator.setProfile(profile);

    EXPECT_EQ(fileContents("generator/algebraic_eqn_state_var_on_rhs_one_component/code.py"), generator.code());
}

TEST(Generator, ode_computed_var_on_rhs)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/ode_computed_var_on_rhs/model.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(size_t(0), generator.errorCount());

    EXPECT_EQ(libcellml::Generator::ModelType::ODE, generator.modelType());

    EXPECT_EQ(size_t(1), generator.stateCount());
    EXPECT_EQ(size_t(1), generator.variableCount());

    EXPECT_NE(nullptr, generator.variableOfIntegration());
    EXPECT_NE(nullptr, generator.state(0));
    EXPECT_EQ(nullptr, generator.state(generator.stateCount()));
    EXPECT_NE(nullptr, generator.variable(0));
    EXPECT_EQ(nullptr, generator.variable(generator.variableCount()));

    EXPECT_EQ(fileContents("generator/ode_computed_var_on_rhs/code.c"), generator.code());

    libcellml::GeneratorProfilePtr profile = std::make_shared<libcellml::GeneratorProfile>(libcellml::GeneratorProfile::Profile::PYTHON);

    generator.setProfile(profile);

    EXPECT_EQ(fileContents("generator/ode_computed_var_on_rhs/code.py"), generator.code());
}

TEST(Generator, ode_computed_var_on_rhs_one_component)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/ode_computed_var_on_rhs_one_component/model.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(size_t(0), generator.errorCount());

    EXPECT_EQ(libcellml::Generator::ModelType::ODE, generator.modelType());

    EXPECT_EQ(size_t(1), generator.stateCount());
    EXPECT_EQ(size_t(1), generator.variableCount());

    EXPECT_NE(nullptr, generator.variableOfIntegration());
    EXPECT_NE(nullptr, generator.state(0));
    EXPECT_EQ(nullptr, generator.state(generator.stateCount()));
    EXPECT_NE(nullptr, generator.variable(0));
    EXPECT_EQ(nullptr, generator.variable(generator.variableCount()));

    EXPECT_EQ(fileContents("generator/ode_computed_var_on_rhs_one_component/code.c"), generator.code());

    libcellml::GeneratorProfilePtr profile = std::make_shared<libcellml::GeneratorProfile>(libcellml::GeneratorProfile::Profile::PYTHON);

    generator.setProfile(profile);

    EXPECT_EQ(fileContents("generator/ode_computed_var_on_rhs_one_component/code.py"), generator.code());
}

TEST(Generator, ode_const_var_on_rhs)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/ode_const_var_on_rhs/model.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(size_t(0), generator.errorCount());

    EXPECT_EQ(libcellml::Generator::ModelType::ODE, generator.modelType());

    EXPECT_EQ(size_t(1), generator.stateCount());
    EXPECT_EQ(size_t(1), generator.variableCount());

    EXPECT_NE(nullptr, generator.variableOfIntegration());
    EXPECT_NE(nullptr, generator.state(0));
    EXPECT_EQ(nullptr, generator.state(generator.stateCount()));
    EXPECT_NE(nullptr, generator.variable(0));
    EXPECT_EQ(nullptr, generator.variable(generator.variableCount()));

    EXPECT_EQ(fileContents("generator/ode_const_var_on_rhs/code.c"), generator.code());

    libcellml::GeneratorProfilePtr profile = std::make_shared<libcellml::GeneratorProfile>(libcellml::GeneratorProfile::Profile::PYTHON);

    generator.setProfile(profile);

    EXPECT_EQ(fileContents("generator/ode_const_var_on_rhs/code.py"), generator.code());
}

TEST(Generator, ode_const_var_on_rhs_one_component)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/ode_const_var_on_rhs_one_component/model.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(size_t(0), generator.errorCount());

    EXPECT_EQ(libcellml::Generator::ModelType::ODE, generator.modelType());

    EXPECT_EQ(size_t(1), generator.stateCount());
    EXPECT_EQ(size_t(1), generator.variableCount());

    EXPECT_NE(nullptr, generator.variableOfIntegration());
    EXPECT_NE(nullptr, generator.state(0));
    EXPECT_EQ(nullptr, generator.state(generator.stateCount()));
    EXPECT_NE(nullptr, generator.variable(0));
    EXPECT_EQ(nullptr, generator.variable(generator.variableCount()));

    EXPECT_EQ(fileContents("generator/ode_const_var_on_rhs_one_component/code.c"), generator.code());

    libcellml::GeneratorProfilePtr profile = std::make_shared<libcellml::GeneratorProfile>(libcellml::GeneratorProfile::Profile::PYTHON);

    generator.setProfile(profile);

    EXPECT_EQ(fileContents("generator/ode_const_var_on_rhs_one_component/code.py"), generator.code());
}

TEST(Generator, ode_constant_on_rhs)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/ode_constant_on_rhs/model.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(size_t(0), generator.errorCount());

    EXPECT_EQ(libcellml::Generator::ModelType::ODE, generator.modelType());

    EXPECT_EQ(size_t(1), generator.stateCount());
    EXPECT_EQ(size_t(0), generator.variableCount());

    EXPECT_NE(nullptr, generator.variableOfIntegration());
    EXPECT_NE(nullptr, generator.state(0));
    EXPECT_EQ(nullptr, generator.state(generator.stateCount()));
    EXPECT_EQ(nullptr, generator.variable(0));

    EXPECT_EQ(fileContents("generator/ode_constant_on_rhs/code.c"), generator.code());

    libcellml::GeneratorProfilePtr profile = std::make_shared<libcellml::GeneratorProfile>(libcellml::GeneratorProfile::Profile::PYTHON);

    generator.setProfile(profile);

    EXPECT_EQ(fileContents("generator/ode_constant_on_rhs/code.py"), generator.code());
}

TEST(Generator, ode_constant_on_rhs_one_component)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/ode_constant_on_rhs_one_component/model.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(size_t(0), generator.errorCount());

    EXPECT_EQ(libcellml::Generator::ModelType::ODE, generator.modelType());

    EXPECT_EQ(size_t(1), generator.stateCount());
    EXPECT_EQ(size_t(0), generator.variableCount());

    EXPECT_NE(nullptr, generator.variableOfIntegration());
    EXPECT_NE(nullptr, generator.state(0));
    EXPECT_EQ(nullptr, generator.state(generator.stateCount()));
    EXPECT_EQ(nullptr, generator.variable(0));

    EXPECT_EQ(fileContents("generator/ode_constant_on_rhs_one_component/code.c"), generator.code());

    libcellml::GeneratorProfilePtr profile = std::make_shared<libcellml::GeneratorProfile>(libcellml::GeneratorProfile::Profile::PYTHON);

    generator.setProfile(profile);

    EXPECT_EQ(fileContents("generator/ode_constant_on_rhs_one_component/code.py"), generator.code());
}

TEST(Generator, ode_multiple_dependent_odes)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/ode_multiple_dependent_odes/model.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(size_t(0), generator.errorCount());

    EXPECT_EQ(libcellml::Generator::ModelType::ODE, generator.modelType());

    EXPECT_EQ(size_t(2), generator.stateCount());
    EXPECT_EQ(size_t(1), generator.variableCount());

    EXPECT_NE(nullptr, generator.variableOfIntegration());
    EXPECT_NE(nullptr, generator.state(0));
    EXPECT_EQ(nullptr, generator.state(generator.stateCount()));
    EXPECT_NE(nullptr, generator.variable(0));
    EXPECT_EQ(nullptr, generator.variable(generator.variableCount()));

    EXPECT_EQ(fileContents("generator/ode_multiple_dependent_odes/code.c"), generator.code());

    libcellml::GeneratorProfilePtr profile = std::make_shared<libcellml::GeneratorProfile>(libcellml::GeneratorProfile::Profile::PYTHON);

    generator.setProfile(profile);

    EXPECT_EQ(fileContents("generator/ode_multiple_dependent_odes/code.py"), generator.code());
}

TEST(Generator, ode_multiple_dependent_odes_one_component)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/ode_multiple_dependent_odes_one_component/model.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(size_t(0), generator.errorCount());

    EXPECT_EQ(libcellml::Generator::ModelType::ODE, generator.modelType());

    EXPECT_EQ(size_t(2), generator.stateCount());
    EXPECT_EQ(size_t(1), generator.variableCount());

    EXPECT_NE(nullptr, generator.variableOfIntegration());
    EXPECT_NE(nullptr, generator.state(0));
    EXPECT_EQ(nullptr, generator.state(generator.stateCount()));
    EXPECT_NE(nullptr, generator.variable(0));
    EXPECT_EQ(nullptr, generator.variable(generator.variableCount()));

    EXPECT_EQ(fileContents("generator/ode_multiple_dependent_odes_one_component/code.c"), generator.code());

    libcellml::GeneratorProfilePtr profile = std::make_shared<libcellml::GeneratorProfile>(libcellml::GeneratorProfile::Profile::PYTHON);

    generator.setProfile(profile);

    EXPECT_EQ(fileContents("generator/ode_multiple_dependent_odes_one_component/code.py"), generator.code());
}

TEST(Generator, ode_multiple_odes_with_same_name)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/ode_multiple_odes_with_same_name/model.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(size_t(0), generator.errorCount());

    EXPECT_EQ(libcellml::Generator::ModelType::ODE, generator.modelType());

    EXPECT_EQ(size_t(2), generator.stateCount());
    EXPECT_EQ(size_t(1), generator.variableCount());

    EXPECT_NE(nullptr, generator.variableOfIntegration());
    EXPECT_NE(nullptr, generator.state(0));
    EXPECT_EQ(nullptr, generator.state(generator.stateCount()));
    EXPECT_NE(nullptr, generator.variable(0));
    EXPECT_EQ(nullptr, generator.variable(generator.variableCount()));

    EXPECT_EQ(fileContents("generator/ode_multiple_odes_with_same_name/code.c"), generator.code());

    libcellml::GeneratorProfilePtr profile = std::make_shared<libcellml::GeneratorProfile>(libcellml::GeneratorProfile::Profile::PYTHON);

    generator.setProfile(profile);

    EXPECT_EQ(fileContents("generator/ode_multiple_odes_with_same_name/code.py"), generator.code());
}

TEST(Generator, cellml_mappings_and_encapsulations)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/cellml_mappings_and_encapsulations/model.cellml"));

    //TODO: currently, the parser wrongly reports an error with our model, hence
    //      we "expect" one parsing error. Clearly, no parsing error should be
    //      reported once issue #377 has been addressed.
    EXPECT_EQ(size_t(1), parser.errorCount());

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(size_t(0), generator.errorCount());

    EXPECT_EQ(libcellml::Generator::ModelType::ODE, generator.modelType());

    EXPECT_EQ(size_t(2), generator.stateCount());
    EXPECT_EQ(size_t(2), generator.variableCount());

    EXPECT_NE(nullptr, generator.variableOfIntegration());
    EXPECT_NE(nullptr, generator.state(0));
    EXPECT_EQ(nullptr, generator.state(generator.stateCount()));
    EXPECT_NE(nullptr, generator.variable(0));
    EXPECT_EQ(nullptr, generator.variable(generator.variableCount()));

    EXPECT_EQ(fileContents("generator/cellml_mappings_and_encapsulations/code.c"), generator.code());

    libcellml::GeneratorProfilePtr profile = std::make_shared<libcellml::GeneratorProfile>(libcellml::GeneratorProfile::Profile::PYTHON);

    generator.setProfile(profile);

    EXPECT_EQ(fileContents("generator/cellml_mappings_and_encapsulations/code.py"), generator.code());
}

TEST(Generator, dependent_eqns)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/dependent_eqns/model.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(size_t(0), generator.errorCount());

    EXPECT_EQ(libcellml::Generator::ModelType::ODE, generator.modelType());

    EXPECT_EQ(size_t(1), generator.stateCount());
    EXPECT_EQ(size_t(2), generator.variableCount());

    EXPECT_NE(nullptr, generator.variableOfIntegration());
    EXPECT_NE(nullptr, generator.state(0));
    EXPECT_EQ(nullptr, generator.state(generator.stateCount()));
    EXPECT_NE(nullptr, generator.variable(0));
    EXPECT_EQ(nullptr, generator.variable(generator.variableCount()));

    EXPECT_EQ(fileContents("generator/dependent_eqns/code.c"), generator.code());

    libcellml::GeneratorProfilePtr profile = std::make_shared<libcellml::GeneratorProfile>(libcellml::GeneratorProfile::Profile::PYTHON);

    generator.setProfile(profile);

    EXPECT_EQ(fileContents("generator/dependent_eqns/code.py"), generator.code());
}

TEST(Generator, fabbri_fantini_wilders_severi_human_san_model_2017)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/fabbri_fantini_wilders_severi_human_san_model_2017/model.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(size_t(0), generator.errorCount());

    EXPECT_EQ(libcellml::Generator::ModelType::ODE, generator.modelType());

    EXPECT_EQ(size_t(33), generator.stateCount());
    EXPECT_EQ(size_t(217), generator.variableCount());

    EXPECT_NE(nullptr, generator.variableOfIntegration());
    EXPECT_NE(nullptr, generator.state(0));
    EXPECT_EQ(nullptr, generator.state(generator.stateCount()));
    EXPECT_NE(nullptr, generator.variable(0));
    EXPECT_EQ(nullptr, generator.variable(generator.variableCount()));

    EXPECT_EQ(fileContents("generator/fabbri_fantini_wilders_severi_human_san_model_2017/code.c"), generator.code());

    libcellml::GeneratorProfilePtr profile = std::make_shared<libcellml::GeneratorProfile>(libcellml::GeneratorProfile::Profile::PYTHON);

    generator.setProfile(profile);

    EXPECT_EQ(fileContents("generator/fabbri_fantini_wilders_severi_human_san_model_2017/code.py"), generator.code());
}

TEST(Generator, garny_kohl_hunter_boyett_noble_rabbit_san_model_2003)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/garny_kohl_hunter_boyett_noble_rabbit_san_model_2003/model.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(size_t(0), generator.errorCount());

    EXPECT_EQ(libcellml::Generator::ModelType::ODE, generator.modelType());

    EXPECT_EQ(size_t(15), generator.stateCount());
    EXPECT_EQ(size_t(185), generator.variableCount());

    EXPECT_NE(nullptr, generator.variableOfIntegration());
    EXPECT_NE(nullptr, generator.state(0));
    EXPECT_EQ(nullptr, generator.state(generator.stateCount()));
    EXPECT_NE(nullptr, generator.variable(0));
    EXPECT_EQ(nullptr, generator.variable(generator.variableCount()));

    EXPECT_EQ(fileContents("generator/garny_kohl_hunter_boyett_noble_rabbit_san_model_2003/code.c"), generator.code());

    libcellml::GeneratorProfilePtr profile = std::make_shared<libcellml::GeneratorProfile>(libcellml::GeneratorProfile::Profile::PYTHON);

    generator.setProfile(profile);

    EXPECT_EQ(fileContents("generator/garny_kohl_hunter_boyett_noble_rabbit_san_model_2003/code.py"), generator.code());
}

TEST(Generator, hodgkin_huxley_squid_axon_model_1952)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/hodgkin_huxley_squid_axon_model_1952/model.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(size_t(0), generator.errorCount());

    EXPECT_EQ(libcellml::Generator::ModelType::ODE, generator.modelType());

    EXPECT_EQ(size_t(4), generator.stateCount());
    EXPECT_EQ(size_t(18), generator.variableCount());

    EXPECT_NE(nullptr, generator.variableOfIntegration());
    EXPECT_NE(nullptr, generator.state(0));
    EXPECT_EQ(nullptr, generator.state(generator.stateCount()));
    EXPECT_NE(nullptr, generator.variable(0));
    EXPECT_EQ(nullptr, generator.variable(generator.variableCount()));

    const std::vector<libcellml::GeneratorVariable::Type> expectedTypes = {
        libcellml::GeneratorVariable::Type::CONSTANT,
        libcellml::GeneratorVariable::Type::CONSTANT,
        libcellml::GeneratorVariable::Type::CONSTANT,
        libcellml::GeneratorVariable::Type::CONSTANT,
        libcellml::GeneratorVariable::Type::CONSTANT,
        libcellml::GeneratorVariable::Type::COMPUTED_CONSTANT,
        libcellml::GeneratorVariable::Type::COMPUTED_CONSTANT,
        libcellml::GeneratorVariable::Type::COMPUTED_CONSTANT,
        libcellml::GeneratorVariable::Type::ALGEBRAIC,
        libcellml::GeneratorVariable::Type::ALGEBRAIC,
        libcellml::GeneratorVariable::Type::ALGEBRAIC,
        libcellml::GeneratorVariable::Type::ALGEBRAIC,
        libcellml::GeneratorVariable::Type::ALGEBRAIC,
        libcellml::GeneratorVariable::Type::ALGEBRAIC,
        libcellml::GeneratorVariable::Type::ALGEBRAIC,
        libcellml::GeneratorVariable::Type::ALGEBRAIC,
        libcellml::GeneratorVariable::Type::ALGEBRAIC,
        libcellml::GeneratorVariable::Type::ALGEBRAIC};

    for (size_t i = 0; i < generator.variableCount(); ++i) {
        EXPECT_NE(nullptr, generator.variable(i)->variable());
        EXPECT_EQ(expectedTypes[i], generator.variable(i)->type());
    }

    EXPECT_EQ(fileContents("generator/hodgkin_huxley_squid_axon_model_1952/code.c"), generator.code());

    libcellml::GeneratorProfilePtr profile = std::make_shared<libcellml::GeneratorProfile>(libcellml::GeneratorProfile::Profile::PYTHON);

    generator.setProfile(profile);

    EXPECT_EQ(fileContents("generator/hodgkin_huxley_squid_axon_model_1952/code.py"), generator.code());
}

TEST(Generator, noble_model_1962)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/noble_model_1962/model.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(size_t(0), generator.errorCount());

    EXPECT_EQ(libcellml::Generator::ModelType::ODE, generator.modelType());

    EXPECT_EQ(size_t(4), generator.stateCount());
    EXPECT_EQ(size_t(17), generator.variableCount());

    EXPECT_NE(nullptr, generator.variableOfIntegration());
    EXPECT_NE(nullptr, generator.state(0));
    EXPECT_EQ(nullptr, generator.state(generator.stateCount()));
    EXPECT_NE(nullptr, generator.variable(0));
    EXPECT_EQ(nullptr, generator.variable(generator.variableCount()));

    EXPECT_EQ(fileContents("generator/noble_model_1962/code.c"), generator.code());

    libcellml::GeneratorProfilePtr profile = std::make_shared<libcellml::GeneratorProfile>(libcellml::GeneratorProfile::Profile::PYTHON);

    generator.setProfile(profile);

    EXPECT_EQ(fileContents("generator/noble_model_1962/code.py"), generator.code());
}

TEST(Generator, coverage)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/coverage/model.cellml"));

    EXPECT_EQ(size_t(0), parser.errorCount());

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_EQ(size_t(0), generator.errorCount());

    EXPECT_EQ(libcellml::Generator::ModelType::ODE, generator.modelType());

    EXPECT_EQ(size_t(1), generator.stateCount());
    EXPECT_EQ(size_t(185), generator.variableCount());

    EXPECT_NE(nullptr, generator.variableOfIntegration());
    EXPECT_NE(nullptr, generator.state(0));
    EXPECT_EQ(nullptr, generator.state(generator.stateCount()));
    EXPECT_NE(nullptr, generator.variable(0));
    EXPECT_EQ(nullptr, generator.variable(generator.variableCount()));

    EXPECT_EQ(fileContents("generator/coverage/code.c"), generator.code());

    libcellml::GeneratorProfilePtr profile = std::make_shared<libcellml::GeneratorProfile>();

    generator.setProfile(profile);

    profile->setXorString("^");
    profile->setHasXorOperator(true);

    profile->setPowerString("^^");
    profile->setSquareString("sqr");
    profile->setHasPowerOperator(true);

    profile->setPiecewiseIfString("piecewise(#cond, #if");
    profile->setPiecewiseElseString(", #else)");
    profile->setHasConditionalOperator(false);

    profile->setTemplateOriginCommentString("");

    EXPECT_EQ(fileContents("generator/coverage/codeWithCustomProfile.out"), generator.code());

    profile->setProfile(libcellml::GeneratorProfile::Profile::PYTHON);

    generator.setProfile(profile);

    EXPECT_EQ(fileContents("generator/coverage/code.py"), generator.code());
}

TEST(Generator, coverForIsPiecewiseStatement)
{
    libcellml::Parser parser;
    libcellml::ModelPtr model = parser.parseModel(fileContents("generator/coverage/piecewiseCover.cellml"));

    libcellml::Generator generator;

    generator.processModel(model);

    EXPECT_LT(size_t(2000), generator.code().length());
}
