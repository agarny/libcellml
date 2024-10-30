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

TEST(GeneratorTrackedVariables, noModelOrVariable)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("generator/hodgkin_huxley_squid_axon_model_1952/model.cellml"));
    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    auto analyserModel = analyser->model();
    auto generator = libcellml::Generator::create();

    EXPECT_FALSE(generator->isTrackedVariable(nullptr));
    EXPECT_FALSE(generator->isUntrackedVariable(nullptr));

    EXPECT_FALSE(generator->trackVariable(nullptr));
    EXPECT_FALSE(generator->untrackVariable(nullptr));

    EXPECT_FALSE(generator->trackAllConstants(nullptr));
    EXPECT_FALSE(generator->untrackAllConstants(nullptr));

    EXPECT_FALSE(generator->trackAllComputedConstants(nullptr));
    EXPECT_FALSE(generator->untrackAllComputedConstants(nullptr));

    EXPECT_FALSE(generator->trackAllAlgebraic(nullptr));
    EXPECT_FALSE(generator->untrackAllAlgebraic(nullptr));

    EXPECT_FALSE(generator->trackAllVariables(nullptr));
    EXPECT_FALSE(generator->untrackAllVariables(nullptr));

    EXPECT_EQ(size_t(0), generator->trackedConstantCount(nullptr));
    EXPECT_EQ(size_t(0), generator->untrackedConstantCount(nullptr));

    EXPECT_EQ(size_t(0), generator->trackedComputedConstantCount(nullptr));
    EXPECT_EQ(size_t(0), generator->untrackedComputedConstantCount(nullptr));

    EXPECT_EQ(size_t(0), generator->trackedAlgebraicCount(nullptr));
    EXPECT_EQ(size_t(0), generator->untrackedAlgebraicCount(nullptr));

    EXPECT_EQ(size_t(0), generator->trackedVariableCount(nullptr));
    EXPECT_EQ(size_t(0), generator->untrackedVariableCount(nullptr));
}

TEST(GeneratorTrackedVariables, tracAndkUntrackVariableOfIntegration)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("generator/hodgkin_huxley_squid_axon_model_1952/model.cellml"));
    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    auto analyserModel = analyser->model();
    auto generator = libcellml::Generator::create();

    auto variable = model->component("environment")->variable("time");

    EXPECT_TRUE(generator->isTrackedVariable(analyserModel->variable(variable)));
    EXPECT_FALSE(generator->isUntrackedVariable(analyserModel->variable(variable)));

    EXPECT_EQ(size_t(18), generator->trackedVariableCount(analyserModel));
    EXPECT_EQ(size_t(0), generator->untrackedVariableCount(analyserModel));

    EXPECT_FALSE(generator->untrackVariable(analyserModel->variable(variable)));

    EXPECT_TRUE(generator->isTrackedVariable(analyserModel->variable(variable)));
    EXPECT_FALSE(generator->isUntrackedVariable(analyserModel->variable(variable)));

    EXPECT_EQ(size_t(18), generator->trackedVariableCount(analyserModel));
    EXPECT_EQ(size_t(0), generator->untrackedVariableCount(analyserModel));

    EXPECT_FALSE(generator->trackVariable(analyserModel->variable(variable)));

    EXPECT_EQ(size_t(18), generator->trackedVariableCount(analyserModel));
    EXPECT_EQ(size_t(0), generator->untrackedVariableCount(analyserModel));
}

TEST(GeneratorTrackedVariables, trackAndUntrackStateVariable)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("generator/hodgkin_huxley_squid_axon_model_1952/model.cellml"));
    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    auto analyserModel = analyser->model();
    auto generator = libcellml::Generator::create();

    auto variable = model->component("membrane")->variable("V");

    EXPECT_TRUE(generator->isTrackedVariable(analyserModel->variable(variable)));
    EXPECT_FALSE(generator->isUntrackedVariable(analyserModel->variable(variable)));

    EXPECT_EQ(size_t(18), generator->trackedVariableCount(analyserModel));
    EXPECT_EQ(size_t(0), generator->untrackedVariableCount(analyserModel));

    EXPECT_FALSE(generator->untrackVariable(analyserModel->variable(variable)));

    EXPECT_TRUE(generator->isTrackedVariable(analyserModel->variable(variable)));
    EXPECT_FALSE(generator->isUntrackedVariable(analyserModel->variable(variable)));

    EXPECT_EQ(size_t(18), generator->trackedVariableCount(analyserModel));
    EXPECT_EQ(size_t(0), generator->untrackedVariableCount(analyserModel));

    EXPECT_FALSE(generator->trackVariable(analyserModel->variable(variable)));

    EXPECT_EQ(size_t(18), generator->trackedVariableCount(analyserModel));
    EXPECT_EQ(size_t(0), generator->untrackedVariableCount(analyserModel));
}

TEST(GeneratorTrackedVariables, trackAndUntrackConstant)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("generator/hodgkin_huxley_squid_axon_model_1952/model.cellml"));
    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    auto analyserModel = analyser->model();
    auto generator = libcellml::Generator::create();

    auto variable = model->component("membrane")->variable("Cm");

    EXPECT_TRUE(generator->isTrackedVariable(analyserModel->variable(variable)));
    EXPECT_FALSE(generator->isUntrackedVariable(analyserModel->variable(variable)));

    EXPECT_EQ(size_t(18), generator->trackedVariableCount(analyserModel));
    EXPECT_EQ(size_t(0), generator->untrackedVariableCount(analyserModel));

    EXPECT_TRUE(generator->untrackVariable(analyserModel->variable(variable)));

    EXPECT_FALSE(generator->isTrackedVariable(analyserModel->variable(variable)));
    EXPECT_TRUE(generator->isUntrackedVariable(analyserModel->variable(variable)));

    EXPECT_EQ(size_t(17), generator->trackedVariableCount(analyserModel));
    EXPECT_EQ(size_t(1), generator->untrackedVariableCount(analyserModel));

    EXPECT_TRUE(generator->trackVariable(analyserModel->variable(variable)));

    EXPECT_EQ(size_t(18), generator->trackedVariableCount(analyserModel));
    EXPECT_EQ(size_t(0), generator->untrackedVariableCount(analyserModel));
}

TEST(GeneratorTrackedVariables, trackAndUntrackComputedConstant)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("generator/hodgkin_huxley_squid_axon_model_1952/model.cellml"));
    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    auto analyserModel = analyser->model();
    auto generator = libcellml::Generator::create();

    auto variable = model->component("leakage_current")->variable("E_L");

    EXPECT_TRUE(generator->isTrackedVariable(analyserModel->variable(variable)));
    EXPECT_FALSE(generator->isUntrackedVariable(analyserModel->variable(variable)));

    EXPECT_EQ(size_t(18), generator->trackedVariableCount(analyserModel));
    EXPECT_EQ(size_t(0), generator->untrackedVariableCount(analyserModel));

    EXPECT_TRUE(generator->untrackVariable(analyserModel->variable(variable)));

    EXPECT_FALSE(generator->isTrackedVariable(analyserModel->variable(variable)));
    EXPECT_TRUE(generator->isUntrackedVariable(analyserModel->variable(variable)));

    EXPECT_EQ(size_t(17), generator->trackedVariableCount(analyserModel));
    EXPECT_EQ(size_t(1), generator->untrackedVariableCount(analyserModel));

    EXPECT_TRUE(generator->trackVariable(analyserModel->variable(variable)));

    EXPECT_EQ(size_t(18), generator->trackedVariableCount(analyserModel));
    EXPECT_EQ(size_t(0), generator->untrackedVariableCount(analyserModel));
}

TEST(GeneratorTrackedVariables, trackAndUntrackAlgebraicVariable)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("generator/hodgkin_huxley_squid_axon_model_1952/model.cellml"));
    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    auto analyserModel = analyser->model();
    auto generator = libcellml::Generator::create();

    auto variable = model->component("membrane")->variable("i_Stim");

    EXPECT_TRUE(generator->isTrackedVariable(analyserModel->variable(variable)));
    EXPECT_FALSE(generator->isUntrackedVariable(analyserModel->variable(variable)));

    EXPECT_EQ(size_t(18), generator->trackedVariableCount(analyserModel));
    EXPECT_EQ(size_t(0), generator->untrackedVariableCount(analyserModel));

    EXPECT_TRUE(generator->untrackVariable(analyserModel->variable(variable)));

    EXPECT_FALSE(generator->isTrackedVariable(analyserModel->variable(variable)));
    EXPECT_TRUE(generator->isUntrackedVariable(analyserModel->variable(variable)));

    EXPECT_EQ(size_t(17), generator->trackedVariableCount(analyserModel));
    EXPECT_EQ(size_t(1), generator->untrackedVariableCount(analyserModel));

    EXPECT_TRUE(generator->trackVariable(analyserModel->variable(variable)));

    EXPECT_EQ(size_t(18), generator->trackedVariableCount(analyserModel));
    EXPECT_EQ(size_t(0), generator->untrackedVariableCount(analyserModel));
}

TEST(GeneratorTrackedVariables, trackAndUntrackExternalVariable)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("generator/hodgkin_huxley_squid_axon_model_1952/model.cellml"));
    auto analyser = libcellml::Analyser::create();

    analyser->addExternalVariable(libcellml::AnalyserExternalVariable::create(model->component("membrane")->variable("V")));

    analyser->analyseModel(model);

    auto analyserModel = analyser->model();
    auto generator = libcellml::Generator::create();

    auto variable = model->component("membrane")->variable("V");

    EXPECT_TRUE(generator->isTrackedVariable(analyserModel->variable(variable)));
    EXPECT_FALSE(generator->isUntrackedVariable(analyserModel->variable(variable)));

    EXPECT_EQ(size_t(18), generator->trackedVariableCount(analyserModel));
    EXPECT_EQ(size_t(0), generator->untrackedVariableCount(analyserModel));

    EXPECT_FALSE(generator->untrackVariable(analyserModel->variable(variable)));

    EXPECT_TRUE(generator->isTrackedVariable(analyserModel->variable(variable)));
    EXPECT_FALSE(generator->isUntrackedVariable(analyserModel->variable(variable)));

    EXPECT_EQ(size_t(18), generator->trackedVariableCount(analyserModel));
    EXPECT_EQ(size_t(0), generator->untrackedVariableCount(analyserModel));

    EXPECT_FALSE(generator->trackVariable(analyserModel->variable(variable)));

    EXPECT_EQ(size_t(18), generator->trackedVariableCount(analyserModel));
    EXPECT_EQ(size_t(0), generator->untrackedVariableCount(analyserModel));
}

TEST(GeneratorTrackedVariables, trackAndUntrackVariableFromOtherModel)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("generator/hodgkin_huxley_squid_axon_model_1952/model.cellml"));
    auto otherModel = parser->parseModel(fileContents("generator/noble_model_1962/model.cellml"));
    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    auto analyserModel = analyser->model();
    auto generator = libcellml::Generator::create();

    auto otherVariable = otherModel->component("membrane")->variable("Cm");

    EXPECT_FALSE(generator->trackVariable(analyserModel->variable(otherVariable)));
    EXPECT_FALSE(generator->untrackVariable(analyserModel->variable(otherVariable)));
}

TEST(GeneratorTrackedVariables, trackAndUntrackAllConstants)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("generator/hodgkin_huxley_squid_axon_model_1952/model.cellml"));
    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    auto analyserModel = analyser->model();
    auto generator = libcellml::Generator::create();

    EXPECT_EQ(size_t(5), generator->trackedConstantCount(analyserModel));
    EXPECT_EQ(size_t(0), generator->untrackedConstantCount(analyserModel));
    EXPECT_EQ(size_t(18), generator->trackedVariableCount(analyserModel));
    EXPECT_EQ(size_t(0), generator->untrackedVariableCount(analyserModel));

    generator->untrackAllConstants(analyserModel);

    EXPECT_EQ(size_t(0), generator->trackedConstantCount(analyserModel));
    EXPECT_EQ(size_t(5), generator->untrackedConstantCount(analyserModel));
    EXPECT_EQ(size_t(13), generator->trackedVariableCount(analyserModel));
    EXPECT_EQ(size_t(5), generator->untrackedVariableCount(analyserModel));

    generator->trackAllConstants(analyserModel);

    EXPECT_EQ(size_t(5), generator->trackedConstantCount(analyserModel));
    EXPECT_EQ(size_t(0), generator->untrackedConstantCount(analyserModel));
    EXPECT_EQ(size_t(18), generator->trackedVariableCount(analyserModel));
    EXPECT_EQ(size_t(0), generator->untrackedVariableCount(analyserModel));
}

TEST(GeneratorTrackedVariables, trackAndUntrackAllComputedConstants)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("generator/hodgkin_huxley_squid_axon_model_1952/model.cellml"));
    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    auto analyserModel = analyser->model();
    auto generator = libcellml::Generator::create();

    EXPECT_EQ(size_t(3), generator->trackedComputedConstantCount(analyserModel));
    EXPECT_EQ(size_t(0), generator->untrackedComputedConstantCount(analyserModel));
    EXPECT_EQ(size_t(18), generator->trackedVariableCount(analyserModel));
    EXPECT_EQ(size_t(0), generator->untrackedVariableCount(analyserModel));

    generator->untrackAllComputedConstants(analyserModel);

    EXPECT_EQ(size_t(0), generator->trackedComputedConstantCount(analyserModel));
    EXPECT_EQ(size_t(3), generator->untrackedComputedConstantCount(analyserModel));
    EXPECT_EQ(size_t(15), generator->trackedVariableCount(analyserModel));
    EXPECT_EQ(size_t(3), generator->untrackedVariableCount(analyserModel));

    generator->trackAllComputedConstants(analyserModel);

    EXPECT_EQ(size_t(3), generator->trackedComputedConstantCount(analyserModel));
    EXPECT_EQ(size_t(0), generator->untrackedComputedConstantCount(analyserModel));
    EXPECT_EQ(size_t(18), generator->trackedVariableCount(analyserModel));
    EXPECT_EQ(size_t(0), generator->untrackedVariableCount(analyserModel));
}

TEST(GeneratorTrackedVariables, trackAndUntrackAllAlgebraicVariables)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("generator/hodgkin_huxley_squid_axon_model_1952/model.cellml"));
    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    auto analyserModel = analyser->model();
    auto generator = libcellml::Generator::create();

    EXPECT_EQ(size_t(10), generator->trackedAlgebraicCount(analyserModel));
    EXPECT_EQ(size_t(0), generator->untrackedAlgebraicCount(analyserModel));
    EXPECT_EQ(size_t(18), generator->trackedVariableCount(analyserModel));
    EXPECT_EQ(size_t(0), generator->untrackedVariableCount(analyserModel));

    generator->untrackAllAlgebraic(analyserModel);

    EXPECT_EQ(size_t(0), generator->trackedAlgebraicCount(analyserModel));
    EXPECT_EQ(size_t(10), generator->untrackedAlgebraicCount(analyserModel));
    EXPECT_EQ(size_t(8), generator->trackedVariableCount(analyserModel));
    EXPECT_EQ(size_t(10), generator->untrackedVariableCount(analyserModel));

    generator->trackAllAlgebraic(analyserModel);

    EXPECT_EQ(size_t(10), generator->trackedAlgebraicCount(analyserModel));
    EXPECT_EQ(size_t(0), generator->untrackedAlgebraicCount(analyserModel));
    EXPECT_EQ(size_t(18), generator->trackedVariableCount(analyserModel));
    EXPECT_EQ(size_t(0), generator->untrackedVariableCount(analyserModel));
}

TEST(GeneratorTrackedVariables, trackAndUntrackAllVariables)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents("generator/hodgkin_huxley_squid_axon_model_1952/model.cellml"));
    auto analyser = libcellml::Analyser::create();

    analyser->analyseModel(model);

    auto analyserModel = analyser->model();
    auto generator = libcellml::Generator::create();

    EXPECT_EQ(size_t(18), generator->trackedVariableCount(analyserModel));
    EXPECT_EQ(size_t(0), generator->untrackedVariableCount(analyserModel));

    generator->untrackAllVariables(analyserModel);

    EXPECT_EQ(size_t(0), generator->trackedVariableCount(analyserModel));
    EXPECT_EQ(size_t(18), generator->untrackedVariableCount(analyserModel));

    generator->trackAllVariables(analyserModel);

    EXPECT_EQ(size_t(18), generator->trackedVariableCount(analyserModel));
    EXPECT_EQ(size_t(0), generator->untrackedVariableCount(analyserModel));
}

enum class TrackingType
{
    VARIABLES,
    CONSTANTS,
    COMPUTED_CONSTANTS,
    ALGEBRAIC
};

void untrack(const libcellml::AnalyserModelPtr &model, const libcellml::GeneratorPtr &generator, TrackingType trackingType)
{
    switch (trackingType) {
    case TrackingType::VARIABLES:
        generator->untrackAllVariables(model);

        break;
    case TrackingType::CONSTANTS:
        generator->untrackAllConstants(model);

        break;
    case TrackingType::COMPUTED_CONSTANTS:
        generator->untrackAllComputedConstants(model);

        break;
    case TrackingType::ALGEBRAIC:
        generator->untrackAllAlgebraic(model);

        break;
    }
}

void hodgkinHuxleySquidAxonModel1952CodeGeneration(bool ode, TrackingType trackingType)
{
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(fileContents(std::string("generator/hodgkin_huxley_squid_axon_model_1952/model") + (ode ? "" : ".dae.wo.cc") + ".cellml"));
    auto analyser = libcellml::Analyser::create();
    auto generator = libcellml::Generator::create();
    std::string modelType = ode ? "model" : "model.dae.wo.cc";
    std::string variableType = (trackingType == TrackingType::VARIABLES) ?
                                   "variables" :
                                   ((trackingType == TrackingType::CONSTANTS) ?
                                        "constants" :
                                        ((trackingType == TrackingType::COMPUTED_CONSTANTS) ?
                                             "computed.constants" :
                                             "algebraic.variables"));

    analyser->analyseModel(model);

    auto analyserModel = analyser->model();

    untrack(analyserModel, generator, trackingType);

    auto profile = generator->profile();

    profile->setInterfaceFileNameString(modelType + ".untracked." + variableType + ".h");

    EXPECT_EQ_FILE_CONTENTS("generator/hodgkin_huxley_squid_axon_model_1952/" + modelType + ".untracked." + variableType + ".h", generator->interfaceCode(analyserModel));
    EXPECT_EQ_FILE_CONTENTS("generator/hodgkin_huxley_squid_axon_model_1952/" + modelType + ".untracked." + variableType + ".c", generator->implementationCode(analyserModel));

    profile = libcellml::GeneratorProfile::create(libcellml::GeneratorProfile::Profile::PYTHON);

    generator->setProfile(profile);

    EXPECT_EQ_FILE_CONTENTS("generator/hodgkin_huxley_squid_axon_model_1952/" + modelType + ".untracked." + variableType + ".py", generator->implementationCode(analyserModel));

    // With some external variables.

    auto potassium_channel_n_gate_alpha_n = model->component("potassium_channel_n_gate")->variable("alpha_n");
    auto external_sodium_channel_i_Na = libcellml::AnalyserExternalVariable::create(model->component("sodium_channel")->variable("i_Na"));

    external_sodium_channel_i_Na->addDependency(potassium_channel_n_gate_alpha_n);
    external_sodium_channel_i_Na->addDependency(model->component("sodium_channel_h_gate")->variable("h"));

    analyser->addExternalVariable(libcellml::AnalyserExternalVariable::create(model->component("membrane")->variable("V")));
    analyser->addExternalVariable(external_sodium_channel_i_Na);
    analyser->addExternalVariable(libcellml::AnalyserExternalVariable::create(potassium_channel_n_gate_alpha_n));

    analyser->analyseModel(model);

    analyserModel = analyser->model();

    untrack(analyserModel, generator, trackingType);

    profile = libcellml::GeneratorProfile::create(libcellml::GeneratorProfile::Profile::C);

    generator->setProfile(profile);

    profile->setInterfaceFileNameString(modelType + ".untracked." + variableType + ".with.externals.h");

    EXPECT_EQ_FILE_CONTENTS("generator/hodgkin_huxley_squid_axon_model_1952/" + modelType + ".untracked." + variableType + ".with.externals.h", generator->interfaceCode(analyserModel));
    EXPECT_EQ_FILE_CONTENTS("generator/hodgkin_huxley_squid_axon_model_1952/" + modelType + ".untracked." + variableType + ".with.externals.c", generator->implementationCode(analyserModel));

    profile = libcellml::GeneratorProfile::create(libcellml::GeneratorProfile::Profile::PYTHON);

    generator->setProfile(profile);

    EXPECT_EQ_FILE_CONTENTS("generator/hodgkin_huxley_squid_axon_model_1952/" + modelType + ".untracked." + variableType + ".with.externals.py", generator->implementationCode(analyserModel));
}

TEST(GeneratorTrackedVariables, hodgkinHuxleySquidAxonModel1952UntrackedConstants)
{
    hodgkinHuxleySquidAxonModel1952CodeGeneration(true, TrackingType::CONSTANTS);
}

TEST(GeneratorTrackedVariables, hodgkinHuxleySquidAxonModel1952UntrackedComputedConstants)
{
    hodgkinHuxleySquidAxonModel1952CodeGeneration(true, TrackingType::COMPUTED_CONSTANTS);
}

TEST(GeneratorTrackedVariables, hodgkinHuxleySquidAxonModel1952UntrackedAlgebraicVariables)
{
    hodgkinHuxleySquidAxonModel1952CodeGeneration(true, TrackingType::ALGEBRAIC);
}

TEST(GeneratorTrackedVariables, hodgkinHuxleySquidAxonModel1952UntrackedVariables)
{
    hodgkinHuxleySquidAxonModel1952CodeGeneration(true, TrackingType::VARIABLES);
}

TEST(GeneratorTrackedVariables, hodgkinHuxleySquidAxonModel1952DaeUntrackedConstants)
{
    hodgkinHuxleySquidAxonModel1952CodeGeneration(false, TrackingType::CONSTANTS);
}

TEST(GeneratorTrackedVariables, hodgkinHuxleySquidAxonModel1952DaeUntrackedComputedConstants)
{
    hodgkinHuxleySquidAxonModel1952CodeGeneration(false, TrackingType::COMPUTED_CONSTANTS);
}

TEST(GeneratorTrackedVariables, hodgkinHuxleySquidAxonModel1952DaeUntrackedAlgebraicVariables)
{
    hodgkinHuxleySquidAxonModel1952CodeGeneration(false, TrackingType::ALGEBRAIC);
}

TEST(GeneratorTrackedVariables, hodgkinHuxleySquidAxonModel1952DaeUntrackedVariables)
{
    hodgkinHuxleySquidAxonModel1952CodeGeneration(false, TrackingType::VARIABLES);
}