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

#include "libcellml/analyser.h"

#include <symengine/derivative.h>
#include <symengine/eval_double.h>
#include <symengine/simplify.h>
#include <symengine/solve.h>
#include <symengine/subs.h>

#include "libcellml/analyserequation.h"
#include "libcellml/analyserexternalvariable.h"
#include "libcellml/generatorprofile.h"
#include "libcellml/validator.h"

#include "analyser_p.h"
#include "analyserequation_p.h"
#include "analyserequationast_p.h"
#include "analyservariable_p.h"
#include "commonutils.h"
#include "generator_p.h"
#include "xmlutils.h"

namespace libcellml {

AnalyserInternalVariablePtr AnalyserInternalVariable::create(const VariablePtr &variable)
{
    auto res = std::make_shared<AnalyserInternalVariable>();

    res->setVariable(variable);

    return res;
}

void AnalyserInternalVariable::setVariable(const VariablePtr &variable,
                                           bool checkInitialValue)
{
    if (checkInitialValue && !variable->initialValue().empty()) {
        // The variable has an initial value, so it can either be a constant, an
        // algebraic variable (that needs to be computed using an NLA solver),
        // or a state. For now, all we know is that it is initialised.

        mType = Type::INITIALISED;

        mInitialisingVariable = variable;
    }

    mVariable = variable;
}

void AnalyserInternalVariable::makeVoi()
{
    mType = Type::VARIABLE_OF_INTEGRATION;
}

void AnalyserInternalVariable::makeState()
{
    switch (mType) {
    case Type::UNKNOWN:
        mType = Type::SHOULD_BE_STATE;

        break;
    case Type::INITIALISED:
        mType = Type::STATE;

        break;
    default:
        // Other types we don't care about.

        break;
    }
}

void AnalyserInternalVariable::makeConstant()
{
    mType = Type::CONSTANT;
}

AnalyserInternalEquationPtr AnalyserInternalEquation::create(const ComponentPtr &component)
{
    auto res = std::make_shared<AnalyserInternalEquation>();

    res->mAst = AnalyserEquationAst::create();
    res->mComponent = component;

    return res;
}

AnalyserInternalEquationPtr AnalyserInternalEquation::create(const AnalyserInternalVariablePtr &variable)
{
    auto res = std::make_shared<AnalyserInternalEquation>();

    res->mComponent = owningComponent(variable->mVariable);

    res->addUnknownVariable(variable);

    return res;
}

void AnalyserInternalEquation::addVariable(const AnalyserInternalVariablePtr &variable)
{
    if (mVariablesSet.insert(variable.get()).second) {
        mVariables.push_back(variable);
        mAllVariables.push_back(variable);
    }
}

AnalyserInternalVariablePtrs::iterator AnalyserInternalEquation::removeVariable(const AnalyserInternalVariablePtr &variable)
{
    mAllVariables.erase(std::remove(mAllVariables.begin(), mAllVariables.end(), variable), mAllVariables.end());
    mVariablesSet.erase(variable.get());

    return mVariables.erase(std::remove(mVariables.begin(), mVariables.end(), variable), mVariables.end());
}

void AnalyserInternalEquation::addStateVariable(const AnalyserInternalVariablePtr &stateVariable)
{
    if (mStateVariablesSet.insert(stateVariable.get()).second) {
        mStateVariables.push_back(stateVariable);
        mAllVariables.push_back(stateVariable);
    }
}

AnalyserInternalVariablePtrs::iterator AnalyserInternalEquation::removeStateVariable(const AnalyserInternalVariablePtr &stateVariable)
{
    mAllVariables.erase(std::remove(mAllVariables.begin(), mAllVariables.end(), stateVariable), mAllVariables.end());
    mStateVariablesSet.erase(stateVariable.get());

    return mStateVariables.erase(std::remove(mStateVariables.begin(), mStateVariables.end(), stateVariable), mStateVariables.end());
}

void AnalyserInternalEquation::addUnknownVariable(const AnalyserInternalVariablePtr &unknownVariable)
{
    if (mUnknownVariablesSet.insert(unknownVariable.get()).second) {
        mUnknownVariables.push_back(unknownVariable);
    }
}

AnalyserInternalVariablePtrs::iterator AnalyserInternalEquation::removeUnknownVariable(const AnalyserInternalVariablePtr &unknownVariable)
{
    mUnknownVariablesSet.erase(unknownVariable.get());

    return mUnknownVariables.erase(std::remove(mUnknownVariables.begin(), mUnknownVariables.end(), unknownVariable), mUnknownVariables.end());
}

bool AnalyserInternalEquation::isKnownVariable(const AnalyserInternalVariablePtr &variable)
{
    return variable->mType != AnalyserInternalVariable::Type::UNKNOWN;
}

bool AnalyserInternalEquation::isKnownStateVariable(const AnalyserInternalVariablePtr &stateVariable)
{
    return stateVariable->mIsKnownStateVariable;
}

bool AnalyserInternalEquation::hasKnownVariables(const AnalyserInternalVariablePtrs &variables)
{
    return std::any_of(variables.begin(), variables.end(), [](const auto &v) {
        return isKnownVariable(v);
    });
}

bool AnalyserInternalEquation::hasKnownVariables()
{
    return hasKnownVariables(mVariables) || hasKnownVariables(mStateVariables);
}

bool AnalyserInternalEquation::isNonConstantVariable(const AnalyserInternalVariablePtr &variable)
{
    // Note: we don't check for AnalyserInternalVariable::Type::CONSTANT because
    //       a variable's type becomes constant at the very end, i.e. once we
    //       know for sure that it's neither a state variable nor a variable
    //       that is computed using an NLA system.

    return variable->mIsExternalVariable
           || ((variable->mType != AnalyserInternalVariable::Type::UNKNOWN)
               && (variable->mType != AnalyserInternalVariable::Type::INITIALISED)
               && (variable->mType != AnalyserInternalVariable::Type::COMPUTED_TRUE_CONSTANT)
               && (variable->mType != AnalyserInternalVariable::Type::COMPUTED_VARIABLE_BASED_CONSTANT));
}

bool AnalyserInternalEquation::hasNonConstantVariables(const AnalyserInternalVariablePtrs &variables)
{
    return std::any_of(variables.begin(), variables.end(), [](const auto &v) {
        return isNonConstantVariable(v);
    });
}

bool AnalyserInternalEquation::hasNonConstantVariables()
{
    return hasNonConstantVariables(mVariables) || hasNonConstantVariables(mStateVariables);
}

bool AnalyserInternalEquation::isVariable(const AnalyserInternalVariablePtr &variable,
                                          const AnalyserEquationAstPtr &astChild)
{
    VariablePtr astVariable;

    switch (astChild->type()) {
    case AnalyserEquationAst::Type::CI:
        astVariable = astChild->variable();

        break;
    case AnalyserEquationAst::Type::DIFF:
        astVariable = astChild->rightChild()->variable();

        break;
    default:
        return false;
    }

    return (astVariable == variable->mVariable) || astVariable->hasEquivalentVariable(variable->mVariable);
}

bool AnalyserInternalEquation::containsVariable(const AnalyserInternalVariablePtr &variable,
                                                const AnalyserEquationAstPtr &astChild)
{
    if (astChild == nullptr) {
        return false;
    }

    if ((astChild->type() == AnalyserEquationAst::Type::CI) && ((variable->mVariable == astChild->variable()) || variable->mVariable->hasEquivalentVariable(astChild->variable()))) {
        if (variable->mType == AnalyserInternalVariable::Type::STATE) {
            // State variables should be considered known and thus return false if they are used as a typical variable
            // (rather than in an ODE).

            return astChild->parent()->type() == AnalyserEquationAst::Type::DIFF;
        }

        return true;
    }

    return containsVariable(variable, astChild->leftChild()) || containsVariable(variable, astChild->rightChild());
}

bool AnalyserInternalEquation::isVariableIsolated(const AnalyserInternalVariablePtr &variable)
{
    if (isVariable(variable, mAst->leftChild())) {
        return !containsVariable(variable, mAst->rightChild());
    }

    if (isVariable(variable, mAst->rightChild())) {
        return !containsVariable(variable, mAst->leftChild());
    }

    // The variable is not isolated on either side.

    return false;
}

SymEngineEquation AnalyserInternalEquation::rearrangeForSymEngineSymbol(const SymEngineVariable &symEngineVariable)
{
    auto isSymEngineEquationComplex = [&](const auto &self, const SymEngineEquation &symEngineEquation) -> bool {
        if (SymEngine::is_a_Complex(*symEngineEquation)) {
            return true;
        }

        for (const auto &arg : symEngineEquation->get_args()) {
            if (self(self, arg)) {
                return true;
            }
        }

        return false;
    };

    auto containsSymEngineByPredicate = [&](const auto &self,
                                            const SymEngineEquation &symEngineEquation,
                                            const auto &predicate) -> bool {
        if (symEngineEquation.is_null()) {
            return false;
        }

        if (predicate(symEngineEquation)) {
            return true;
        }

        for (const auto &arg : symEngineEquation->get_args()) {
            if (self(self, arg, predicate)) {
                return true;
            }
        }

        return false;
    };

    auto containsSymEngineVariable = [&](const SymEngineEquation &symEngineEquation) -> bool {
        auto predicate = [&](const SymEngineEquation &candidate) {
            return (candidate->get_type_code() == SymEngine::SYMENGINE_SYMBOL)
                   && (SymEngine::rcp_static_cast<const SymEngine::Symbol>(candidate)->get_name() == symEngineVariable->get_name());
        };

        return containsSymEngineByPredicate(containsSymEngineByPredicate, symEngineEquation, predicate);
    };

    auto containsAnySymEngineSymbol = [&](const SymEngineEquation &symEngineEquation) -> bool {
        auto predicate = [&](const SymEngineEquation &candidate) {
            return candidate->get_type_code() == SymEngine::SYMENGINE_SYMBOL;
        };

        return containsSymEngineByPredicate(containsSymEngineByPredicate, symEngineEquation, predicate);
    };

    auto filterSolutions = [&](SymEngine::vec_basic solutions) {
        solutions.erase(std::remove_if(solutions.begin(), solutions.end(),
                                       [&](const SymEngineEquation &solution) {
                                           return isSymEngineEquationComplex(isSymEngineEquationComplex, solution)
                                                  || containsSymEngineVariable(solution);
                                       }),
                        solutions.end());

        return solutions;
    };

    auto containsSymEngineExp = [&](const SymEngineEquation &symEngineEquation) -> bool {
        auto predicate = [&](const SymEngineEquation &candidate) {
            if (candidate->get_type_code() != SymEngine::SYMENGINE_POW) {
                return false;
            }

            auto args = candidate->get_args();

            return SymEngine::eq(*args[0], *SymEngine::E);
        };

        return containsSymEngineByPredicate(containsSymEngineByPredicate, symEngineEquation, predicate);
    };

    auto containsSymEngineLog = [&](const SymEngineEquation &symEngineEquation) -> bool {
        auto predicate = [&](const SymEngineEquation &candidate) {
            return candidate->get_type_code() == SymEngine::SYMENGINE_LOG;
        };

        return containsSymEngineByPredicate(containsSymEngineByPredicate, symEngineEquation, predicate);
    };

    auto simplifySymEngineExpression = [&](const auto &self, const SymEngineEquation &symEngineEquation) -> SymEngineEquation {
        if (symEngineEquation.is_null()) {
            return symEngineEquation;
        }

        auto simplifiedArgs = [&](const SymEngineEquation &symEngineEquation) {
            SymEngine::vec_basic args;

            args.reserve(symEngineEquation->get_args().size());

            for (const auto &arg : symEngineEquation->get_args()) {
                args.push_back(self(self, arg));
            }

            return args;
        };

        if (symEngineEquation->get_type_code() == SymEngine::SYMENGINE_POW) {
            auto args = simplifiedArgs(symEngineEquation);
            auto simplifiedBase = args[0];
            auto simplifiedExponent = args[1];

            if (SymEngine::eq(*simplifiedBase, *SymEngine::E)) {
                auto simplifiedExponentExpanded = SymEngine::simplify(SymEngine::expand(simplifiedExponent));

                if (SymEngine::eq(*simplifiedExponentExpanded, *SymEngine::integer(0))
                    || SymEngine::eq(*simplifiedExponentExpanded, *SymEngine::number(0.0))) {
                    return SymEngine::integer(1);
                }

                simplifiedExponent = simplifiedExponentExpanded;
            }

            return SymEngine::simplify(SymEngine::pow(simplifiedBase, simplifiedExponent));
        }

        if (symEngineEquation->get_type_code() == SymEngine::SYMENGINE_ADD) {
            return SymEngine::simplify(SymEngine::expand(SymEngine::add(simplifiedArgs(symEngineEquation))));
        }

        if (symEngineEquation->get_type_code() == SymEngine::SYMENGINE_MUL) {
            return SymEngine::simplify(SymEngine::expand(SymEngine::mul(simplifiedArgs(symEngineEquation))));
        }

        if (symEngineEquation->get_type_code() == SymEngine::SYMENGINE_CONSTANT) {
            return symEngineEquation;
        }

        if (!containsAnySymEngineSymbol(symEngineEquation)) {
            try {
                return SymEngine::number(SymEngine::eval_double(*symEngineEquation));
            } catch (const std::exception &) {
                // Not evaluable as a number, so keep the symbolic form.
            }
        }

        return SymEngine::simplify(symEngineEquation);
    };

    auto invertSimpleOddPower = [&](const SymEngineEquation &symEngineEquation) -> SymEngineEquation {
        if (symEngineEquation.is_null() || (symEngineEquation->get_type_code() != SymEngine::SYMENGINE_EQUALITY)) {
            return SymEngine::null;
        }

        auto equalityArgs = symEngineEquation->get_args();

        for (size_t i = 0; i < 2; ++i) {
            auto powerSide = equalityArgs[i];
            auto otherSide = equalityArgs[1 - i];

            if (powerSide->get_type_code() != SymEngine::SYMENGINE_POW) {
                continue;
            }

            auto powerArgs = powerSide->get_args();

            if ((powerArgs.size() != 2)
                || (powerArgs[0]->get_type_code() != SymEngine::SYMENGINE_SYMBOL)
                || (powerArgs[1]->get_type_code() != SymEngine::SYMENGINE_INTEGER)) {
                continue;
            }

            auto baseSymbol = SymEngine::rcp_static_cast<const SymEngine::Symbol>(powerArgs[0]);
            auto exponent = SymEngine::rcp_static_cast<const SymEngine::Integer>(powerArgs[1])->as_int();

            if ((baseSymbol->get_name() == symEngineVariable->get_name())
                && (exponent > 0) && ((exponent % 2) == 1)) {
                return SymEngine::pow(otherSide, SymEngine::div(SymEngine::integer(1), SymEngine::integer(exponent)));
            }
        }

        return SymEngine::null;
    };

    // Solve the equation for the given variable.

    SymEngine::RCP<const SymEngine::Set> solutionSet;

    try {
        solutionSet = solve(mSymEngineEquation, symEngineVariable);
    } catch (const std::exception &) {
        // SymEngine failed to solve the equation. This is likely because the variable we are trying to rearrange for is
        // nested within a function that SymEngine cannot invert (e.g., sin(), log()), or because the equation is not a
        // polynomial that SymEngine can handle.

        return SymEngine::null;
    }

    // Filter the solutions to remove any that are complex or that contain the variable we are trying to rearrange for.

    auto solutions = filterSolutions(solutionSet->get_args());

    // If there isn't exactly one solution left, we cannot be sure which one to return, so return null.

    if (solutions.size() != 1) {
        return SymEngine::null;
    }

    auto exactSolution = solutions[0];

    // If the exact solution is not complex and does not contain the variable we are trying to rearrange for, return it.

    try {
        return SymEngine::number(SymEngine::eval_double(*exactSolution));
    } catch (const std::exception &) {
        // The solution is not a number, so return the exact solution, but first try to simplify it, if possible.

        // Try to simplify exponentials and logarithms in the solution, if there are any.

        auto simplifiedSolution = exactSolution;

        if (containsSymEngineExp(exactSolution) || containsSymEngineLog(exactSolution)) {
            simplifiedSolution = simplifySymEngineExpression(simplifySymEngineExpression, exactSolution);
        }

        if (simplifiedSolution.is_null()) {
            simplifiedSolution = exactSolution;
        }

        // Try to do a simple odd-power inversion, if possible.

        auto oddPowerInversion = invertSimpleOddPower(mSymEngineEquation);

        return oddPowerInversion.is_null() ? simplifiedSolution : oddPowerInversion;
    }
}

Analyser::AnalyserImpl::AnalyserImpl()
{
    // Customise our generator's profile.

    mGeneratorProfile->setAbsoluteValueString("abs");
    mGeneratorProfile->setNaturalLogarithmString("ln");
    mGeneratorProfile->setCommonLogarithmString("log");
    mGeneratorProfile->setRemString("rem");
    mGeneratorProfile->setAsinString("arcsin");
    mGeneratorProfile->setAcosString("arccos");
    mGeneratorProfile->setAtanString("arctan");
    mGeneratorProfile->setAsecString("arcsec");
    mGeneratorProfile->setAcscString("arccsc");
    mGeneratorProfile->setAcotString("arccot");
    mGeneratorProfile->setAsinhString("arcsinh");
    mGeneratorProfile->setAcoshString("arccosh");
    mGeneratorProfile->setAtanhString("arctanh");
    mGeneratorProfile->setAsechString("arcsech");
    mGeneratorProfile->setAcschString("arccsch");
    mGeneratorProfile->setAcothString("arccoth");
    mGeneratorProfile->setTrueString("true");
    mGeneratorProfile->setFalseString("false");
    mGeneratorProfile->setEString("exponentiale");
    mGeneratorProfile->setPiString("pi");
    mGeneratorProfile->setInfString("infinity");
    mGeneratorProfile->setNanString("notanumber");
}

AnalyserInternalVariablePtr Analyser::AnalyserImpl::internalVariable(const VariablePtr &variable)
{
    // Check the direct pointer cache first.

    auto cacheIt = mInternalVariableCache.find(variable.get());

    if (cacheIt != mInternalVariableCache.end()) {
        return cacheIt->second;
    }

    // Not in the cache, so do the equivalence-based search.

    for (const auto &internalVariable : mInternalVariables) {
        if (mAnalyserModel->areEquivalentVariables(variable, internalVariable->mVariable)) {
            // Cache this internal variable pointer for future lookups.

            mInternalVariableCache.emplace(variable.get(), internalVariable);

            return internalVariable;
        }
    }

    // No internal variable exists for the given variable, so create one, track
    // it and return it.

    auto res = AnalyserInternalVariable::create(variable);

    mInternalVariables.push_back(res);
    mInternalVariableCache.emplace(variable.get(), res);

    return res;
}

VariablePtr Analyser::AnalyserImpl::voiFirstOccurrence(const VariablePtr &variable,
                                                       const ComponentPtr &component)
{
    // Recursively look for the first occurrence of the given variable in the
    // given component.

    for (size_t i = 0; i < component->variableCount(); ++i) {
        auto componentVariable = component->variable(i);

        if (mAnalyserModel->areEquivalentVariables(variable, componentVariable)) {
            return componentVariable;
        }
    }

    VariablePtr res;

    for (size_t i = 0; (res == nullptr) && (i < component->componentCount()); ++i) {
        res = voiFirstOccurrence(variable, component->component(i));
    }

    return res;
}

void Analyser::AnalyserImpl::analyseNode(const XmlNodePtr &node,
                                         AnalyserEquationAstPtr &ast,
                                         const AnalyserEquationAstPtr &astParent,
                                         const ComponentPtr &component,
                                         const AnalyserInternalEquationPtr &equation)
{
    // Create the AST, if needed.

    if (ast == nullptr) {
        ast.reset(new AnalyserEquationAst {});
    }

    // Basic content elements.
    // Note: we don't need to check for the MathML namespace here since we can only analyse if a model is valid.

    const char *elemName = node->rawName();

    if (strcmp(elemName, "apply") == 0) {
        // We may have 1, 2, 3 or more child nodes, e.g.
        //
        //                 +--------+
        //                 |   +    |
        //        "+a" ==> |  / \   |
        //                 | a  nil |
        //                 +--------+
        //
        //                 +-------+
        //                 |   +   |
        //       "a+b" ==> |  / \  |
        //                 | a   b |
        //                 +-------+
        //
        //                 +-------------+
        //                 |   +         |
        //                 |  / \        |
        //                 | a   +       |
        //                 |    / \      |
        // "a+b+c+d+e" ==> |   b   +     |
        //                 |      / \    |
        //                 |     c   +   |
        //                 |        / \  |
        //                 |       d   e |
        //                 +-------------+

        auto childCount = mathmlChildCount(node);
        AnalyserEquationAstPtr astRightChild;
        AnalyserEquationAstPtr tempAst;

        for (size_t i = childCount - 1; i > 0; --i) {
            astRightChild = tempAst;
            tempAst = AnalyserEquationAst::create();

            if (astRightChild != nullptr) {
                if (i == childCount - 2) {
                    astRightChild->swapLeftAndRightChildren();

                    tempAst = astRightChild;
                } else {
                    astRightChild->mPimpl->mParent = tempAst;
                    tempAst->mPimpl->mOwnedRightChild = astRightChild;
                }
            }

            if (i != childCount - 2) {
                analyseNode(mathmlChildNode(node, 0), tempAst, nullptr, component, equation);
            }

            analyseNode(mathmlChildNode(node, i), tempAst->mPimpl->mOwnedLeftChild, tempAst, component, equation);
        }

        analyseNode(mathmlChildNode(node, 0), tempAst, astParent, component, equation);

        ast = tempAst;

        // Relational and logical operators.

    } else if (strcmp(elemName, "eq") == 0) {
        // This element is used both to describe "a = b" and "a == b". We can
        // distinguish between the two by checking its grandparent. If it's a
        // "math" element then it means that it is used to describe "a = b"
        // otherwise it is used to describe "a == b". In the former case, there
        // is nothing more we need to do since `ast` is already of
        // AnalyserEquationAst::Type::EQUALITY type.

        if (!node->parent()->parent()->isMathmlElement("math")) {
            ast->mPimpl->populate(AnalyserEquationAst::Type::EQ, astParent);

            mAnalyserModel->mPimpl->mNeedEqFunction = true;
        }
    } else if (strcmp(elemName, "neq") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::NEQ, astParent);

        mAnalyserModel->mPimpl->mNeedNeqFunction = true;
    } else if (strcmp(elemName, "lt") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::LT, astParent);

        mAnalyserModel->mPimpl->mNeedLtFunction = true;
    } else if (strcmp(elemName, "leq") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::LEQ, astParent);

        mAnalyserModel->mPimpl->mNeedLeqFunction = true;
    } else if (strcmp(elemName, "gt") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::GT, astParent);

        mAnalyserModel->mPimpl->mNeedGtFunction = true;
    } else if (strcmp(elemName, "geq") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::GEQ, astParent);

        mAnalyserModel->mPimpl->mNeedGeqFunction = true;
    } else if (strcmp(elemName, "and") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::AND, astParent);

        mAnalyserModel->mPimpl->mNeedAndFunction = true;
    } else if (strcmp(elemName, "or") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::OR, astParent);

        mAnalyserModel->mPimpl->mNeedOrFunction = true;
    } else if (strcmp(elemName, "xor") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::XOR, astParent);

        mAnalyserModel->mPimpl->mNeedXorFunction = true;
    } else if (strcmp(elemName, "not") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::NOT, astParent);

        mAnalyserModel->mPimpl->mNeedNotFunction = true;

        // Arithmetic operators.

    } else if (strcmp(elemName, "plus") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::PLUS, astParent);
    } else if (strcmp(elemName, "minus") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::MINUS, astParent);
    } else if (strcmp(elemName, "times") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::TIMES, astParent);
    } else if (strcmp(elemName, "divide") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::DIVIDE, astParent);
    } else if (strcmp(elemName, "power") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::POWER, astParent);
    } else if (strcmp(elemName, "root") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::ROOT, astParent);
    } else if (strcmp(elemName, "abs") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::ABS, astParent);
    } else if (strcmp(elemName, "exp") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::EXP, astParent);
    } else if (strcmp(elemName, "ln") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::LN, astParent);
    } else if (strcmp(elemName, "log") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::LOG, astParent);
    } else if (strcmp(elemName, "ceiling") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::CEILING, astParent);
    } else if (strcmp(elemName, "floor") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::FLOOR, astParent);
    } else if (strcmp(elemName, "min") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::MIN, astParent);

        mAnalyserModel->mPimpl->mNeedMinFunction = true;
    } else if (strcmp(elemName, "max") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::MAX, astParent);

        mAnalyserModel->mPimpl->mNeedMaxFunction = true;
    } else if (strcmp(elemName, "rem") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::REM, astParent);

        // Calculus elements.

    } else if (strcmp(elemName, "diff") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::DIFF, astParent);

        // Trigonometric operators.

    } else if (strcmp(elemName, "sin") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::SIN, astParent);
    } else if (strcmp(elemName, "cos") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::COS, astParent);
    } else if (strcmp(elemName, "tan") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::TAN, astParent);
    } else if (strcmp(elemName, "sec") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::SEC, astParent);

        mAnalyserModel->mPimpl->mNeedSecFunction = true;
    } else if (strcmp(elemName, "csc") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::CSC, astParent);

        mAnalyserModel->mPimpl->mNeedCscFunction = true;
    } else if (strcmp(elemName, "cot") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::COT, astParent);

        mAnalyserModel->mPimpl->mNeedCotFunction = true;
    } else if (strcmp(elemName, "sinh") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::SINH, astParent);
    } else if (strcmp(elemName, "cosh") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::COSH, astParent);
    } else if (strcmp(elemName, "tanh") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::TANH, astParent);
    } else if (strcmp(elemName, "sech") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::SECH, astParent);

        mAnalyserModel->mPimpl->mNeedSechFunction = true;
    } else if (strcmp(elemName, "csch") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::CSCH, astParent);

        mAnalyserModel->mPimpl->mNeedCschFunction = true;
    } else if (strcmp(elemName, "coth") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::COTH, astParent);

        mAnalyserModel->mPimpl->mNeedCothFunction = true;
    } else if (strcmp(elemName, "arcsin") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::ASIN, astParent);
    } else if (strcmp(elemName, "arccos") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::ACOS, astParent);
    } else if (strcmp(elemName, "arctan") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::ATAN, astParent);
    } else if (strcmp(elemName, "arcsec") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::ASEC, astParent);

        mAnalyserModel->mPimpl->mNeedAsecFunction = true;
    } else if (strcmp(elemName, "arccsc") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::ACSC, astParent);

        mAnalyserModel->mPimpl->mNeedAcscFunction = true;
    } else if (strcmp(elemName, "arccot") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::ACOT, astParent);

        mAnalyserModel->mPimpl->mNeedAcotFunction = true;
    } else if (strcmp(elemName, "arcsinh") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::ASINH, astParent);
    } else if (strcmp(elemName, "arccosh") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::ACOSH, astParent);
    } else if (strcmp(elemName, "arctanh") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::ATANH, astParent);
    } else if (strcmp(elemName, "arcsech") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::ASECH, astParent);

        mAnalyserModel->mPimpl->mNeedAsechFunction = true;
    } else if (strcmp(elemName, "arccsch") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::ACSCH, astParent);

        mAnalyserModel->mPimpl->mNeedAcschFunction = true;
    } else if (strcmp(elemName, "arccoth") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::ACOTH, astParent);

        mAnalyserModel->mPimpl->mNeedAcothFunction = true;

        // Piecewise statement.

    } else if (strcmp(elemName, "piecewise") == 0) {
        auto childCount = mathmlChildCount(node);

        ast->mPimpl->populate(AnalyserEquationAst::Type::PIECEWISE, astParent);

        analyseNode(mathmlChildNode(node, 0), ast->mPimpl->mOwnedLeftChild, ast, component, equation);

        if (childCount >= 2) {
            AnalyserEquationAstPtr astRight;
            AnalyserEquationAstPtr tempAst;

            analyseNode(mathmlChildNode(node, childCount - 1), astRight, nullptr, component, equation);

            for (auto i = childCount - 2; i > 0; --i) {
                tempAst = AnalyserEquationAst::create();

                tempAst->mPimpl->populate(AnalyserEquationAst::Type::PIECEWISE, astParent);

                analyseNode(mathmlChildNode(node, i), tempAst->mPimpl->mOwnedLeftChild, tempAst, component, equation);

                astRight->mPimpl->mParent = tempAst;

                tempAst->mPimpl->mOwnedRightChild = astRight;
                astRight = tempAst;
            }

            astRight->mPimpl->mParent = ast;

            ast->mPimpl->mOwnedRightChild = astRight;
        }
    } else if (strcmp(elemName, "piece") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::PIECE, astParent);

        analyseNode(mathmlChildNode(node, 0), ast->mPimpl->mOwnedLeftChild, ast, component, equation);
        analyseNode(mathmlChildNode(node, 1), ast->mPimpl->mOwnedRightChild, ast, component, equation);
    } else if (strcmp(elemName, "otherwise") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::OTHERWISE, astParent);

        analyseNode(mathmlChildNode(node, 0), ast->mPimpl->mOwnedLeftChild, ast, component, equation);

        // Token elements.

    } else if (strcmp(elemName, "ci") == 0) {
        auto variableName = node->firstChild()->convertToStrippedString();
        auto variable = component->variable(variableName);
        // Note: we always have a variable. Indeed, if we were not to have one,
        //       it would mean that `variableName` is the name of a variable
        //       that is referenced in an equation, but not defined anywhere,
        //       something that is not allowed in CellML and will therefore be
        //       reported when we validate the model.

        // Have our equation track the (state) variable (by state variable, we mean
        // a variable that is used in a "diff" element).

        if (node->parent()->firstChild()->isMathmlElement("diff")) {
            equation->addStateVariable(internalVariable(variable));
        } else if (!node->parent()->isMathmlElement("bvar")) {
            equation->addVariable(internalVariable(variable));
        }

        // Add the variable to our AST and keep track of its unit.

        ast->mPimpl->populate(AnalyserEquationAst::Type::CI, variable, astParent);

        mCiCnUnits.emplace(ast, variable->units());
    } else if (strcmp(elemName, "cn") == 0) {
        // Add the number to our AST and keep track of its unit. Note that in
        // the case of a standard unit, we need to create a units since it's
        // not declared in the model.

        if (mathmlChildCount(node) == 1) {
            // We are dealing with an e-notation based CN value.

            ast->mPimpl->populate(AnalyserEquationAst::Type::CN, node->firstChild()->convertToStrippedString() + "e" + node->firstChild()->next()->next()->convertToStrippedString(), astParent);
        } else {
            ast->mPimpl->populate(AnalyserEquationAst::Type::CN, node->firstChild()->convertToStrippedString(), astParent);
        }

        std::string unitsName = node->attribute("units");

        if (isStandardUnitName(unitsName)) {
            auto iter = mStandardUnits.find(unitsName);

            if (iter == mStandardUnits.end()) {
                auto units = Units::create(unitsName);

                mCiCnUnits.emplace(ast, units);
                mStandardUnits.emplace(unitsName, units);
            } else {
                mCiCnUnits.emplace(ast, iter->second);
            }
        } else {
            mCiCnUnits.emplace(ast, owningModel(component)->units(unitsName));
        }

        // Qualifier elements.

    } else if (strcmp(elemName, "degree") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::DEGREE, astParent);

        analyseNode(mathmlChildNode(node, 0), ast->mPimpl->mOwnedLeftChild, ast, component, equation);
    } else if (strcmp(elemName, "logbase") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::LOGBASE, astParent);

        analyseNode(mathmlChildNode(node, 0), ast->mPimpl->mOwnedLeftChild, ast, component, equation);
    } else if (strcmp(elemName, "bvar") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::BVAR, astParent);

        analyseNode(mathmlChildNode(node, 0), ast->mPimpl->mOwnedLeftChild, ast, component, equation);

        auto rightNode = mathmlChildNode(node, 1);

        if (rightNode != nullptr) {
            analyseNode(rightNode, ast->mPimpl->mOwnedRightChild, ast, component, equation);
        }

        // Constants.

    } else if (strcmp(elemName, "true") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::TRUE, astParent);
    } else if (strcmp(elemName, "false") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::FALSE, astParent);
    } else if (strcmp(elemName, "exponentiale") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::E, astParent);
    } else if (strcmp(elemName, "pi") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::PI, astParent);
    } else if (strcmp(elemName, "infinity") == 0) {
        ast->mPimpl->populate(AnalyserEquationAst::Type::INF, astParent);
    } else {
        // We have checked for everything, so if we reach this point it means
        // that we have a NaN.

        ast->mPimpl->populate(AnalyserEquationAst::Type::NAN, astParent);
    }
}

void Analyser::AnalyserImpl::analyseComponent(const ComponentPtr &component)
{
    // Retrieve the math string associated with the given component and analyse
    // it, one equation at a time, keeping in mind that it may consist of
    // several <math> elements, hence our use of multiRootXml().

    if (!component->math().empty()) {
        for (const auto &doc : multiRootXml(component->math())) {
            for (auto node = doc->rootNode()->firstChild(); node != nullptr; node = node->next()) {
                if (node->isMathmlElement()) {
                    // Create and keep track of the equation associated with the
                    // given node.

                    auto internalEquation = AnalyserInternalEquation::create(component);

                    mInternalEquations.push_back(internalEquation);

                    // Actually analyse the node.
                    // Note: we must not test internalEquation->mAst->parent()
                    //       since if it is equal to nullptr then a parent will
                    //       be created by analyseNode().

                    analyseNode(node, internalEquation->mAst, internalEquation->mAst->parent(), component, internalEquation);

                    // Make sure that our internal equation is an equality
                    // statement.

                    if (internalEquation->mAst->mPimpl->mType != AnalyserEquationAst::Type::EQUALITY) {
                        auto issue = Issue::IssueImpl::create();

                        issue->mPimpl->setDescription("Equation " + expression(internalEquation->mAst)
                                                      + " is not an equality statement (i.e. LHS = RHS).");
                        issue->mPimpl->setReferenceRule(Issue::ReferenceRule::ANALYSER_EQUATION_NOT_EQUALITY_STATEMENT);
                        issue->mPimpl->mItem->mPimpl->setComponent(component);

                        addIssue(issue);
                    }
                }
            }
        }
    }

    // Go through the given component's variables and internally keep track of
    // the ones that have an initial value.

    for (size_t i = 0; i < component->variableCount(); ++i) {
        // If `variable` has an initial value and the variable held by
        // `internalVariable` doesn't, then replace the variable held by
        // `internalVariable`.

        auto variable = component->variable(i);
        auto internalVariable = Analyser::AnalyserImpl::internalVariable(variable);

        if (!variable->initialValue().empty()
            && internalVariable->mVariable->initialValue().empty()) {
            internalVariable->setVariable(variable);
        }
    }

    // Do the same for the components encapsulated by the given component.

    for (size_t i = 0; i < component->componentCount(); ++i) {
        analyseComponent(component->component(i));
    }
}

void Analyser::AnalyserImpl::analyseComponentVariables(const ComponentPtr &component)
{
    // Go through the given component's variables and make sure that everything
    // makes sense.

    for (size_t i = 0; i < component->variableCount(); ++i) {
        // If `variable` and the variable held by `internalVariable` are
        // different then make sure that they don't both have an initial value.
        // Alternatively, if the variable held by `internalVariable` has an
        // initial value which is the name of another variable then make sure
        // that it has the same units.
        // Note: we always have an initialising variable in the second case.
        //       Indeed, if we were not to have one, it would mean that the
        //       variable is initialised using a reference to a variable that is
        //       not defined anywhere, something that is not allowed in CellML
        //       and will therefore be reported when we validate the model.

        auto variable = component->variable(i);
        auto internalVariable = Analyser::AnalyserImpl::internalVariable(variable);

        if ((variable != internalVariable->mVariable)
            && !variable->initialValue().empty()) {
            auto issue = Issue::IssueImpl::create();

            issue->mPimpl->setDescription("Variable '" + variable->name()
                                          + "' in component '" + component->name()
                                          + "' and variable '" + internalVariable->mVariable->name()
                                          + "' in component '" + owningComponent(internalVariable->mVariable)->name()
                                          + "' are equivalent and cannot therefore both be initialised.");
            issue->mPimpl->setReferenceRule(Issue::ReferenceRule::ANALYSER_VARIABLE_INITIALISED_MORE_THAN_ONCE);
            issue->mPimpl->mItem->mPimpl->setVariable(variable);

            addIssue(issue);
        } else if (!variable->initialValue().empty()
                   && !isCellMLReal(variable->initialValue())) {
            auto initialisingVariable = owningComponent(variable)->variable(variable->initialValue());
            auto initialisingInternalVariable = Analyser::AnalyserImpl::internalVariable(initialisingVariable);

            if (initialisingInternalVariable->mType == AnalyserInternalVariable::Type::INITIALISED) {
                auto scalingFactor = Units::scalingFactor(variable->units(), initialisingVariable->units());

                if (!areNearlyEqual(scalingFactor, 1.0)) {
                    auto issue = Issue::IssueImpl::create();

                    issue->mPimpl->setDescription("Variable '" + variable->name()
                                                  + "' in component '" + component->name()
                                                  + "' is initialised using variable '" + variable->initialValue()
                                                  + "' which has different units.");
                    issue->mPimpl->setLevel(Issue::Level::WARNING);
                    issue->mPimpl->setReferenceRule(Issue::ReferenceRule::ANALYSER_VARIABLE_INITIALISED_USING_VARIABLE_WITH_DIFFERENT_UNITS);
                    issue->mPimpl->mItem->mPimpl->setVariable(variable);

                    addIssue(issue);
                }
            }
        }
    }

    // Do the same for the components encapsulated by the given component.

    for (size_t i = 0; i < component->componentCount(); ++i) {
        analyseComponentVariables(component->component(i));
    }
}

void Analyser::AnalyserImpl::analyseEquationAst(const AnalyserEquationAstPtr &ast)
{
    // Make sure that we have an AST to analyse.

    if (ast == nullptr) {
        return;
    }

    // Look for the definition of a variable of integration and make sure that
    // we don't have more than one of it and that it's not initialised.

    auto astParent = ast->parent();
    auto astGrandparent = (astParent != nullptr) ? astParent->parent() : nullptr;
    auto astGreatGrandparent = (astGrandparent != nullptr) ? astGrandparent->parent() : nullptr;

    if ((ast->mPimpl->mType == AnalyserEquationAst::Type::CI)
        && (astParent->mPimpl->mType == AnalyserEquationAst::Type::BVAR)) {
        auto astVariable = ast->variable();

        internalVariable(astVariable)->makeVoi();
        // Note: we must make the variable a variable of integration in all
        //       cases (i.e. even if there is, for example, already another
        //       variable of integration) otherwise unnecessary issue messages
        //       may be reported (since the type of the variable would be
        //       unknown).

        if (mAnalyserModel->mPimpl->mVoi == nullptr) {
            // We have found our variable of integration, but this may not be
            // the one defined in our first component (i.e. the component under
            // which we are likely to expect to see the variable of integration
            // to be defined), so go through our components and look for the
            // first occurrence of our variable of integration.

            auto model = owningModel(astVariable);
            auto i = MAX_SIZE_T;
            VariablePtr voi;

            do {
                voi = voiFirstOccurrence(astVariable, model->component(++i));

                if (voi != nullptr) {
                    // We have found the first occurrence of our variable of
                    // integration, but now we must ensure that it (or any of
                    // its equivalent variables) is not initialised.

                    auto isVoiInitialised = false;

                    for (const auto &voiEquivalentVariable : equivalentVariables(voi)) {
                        if (!voiEquivalentVariable->initialValue().empty()) {
                            auto issue = Issue::IssueImpl::create();

                            issue->mPimpl->setDescription("Variable '" + voiEquivalentVariable->name()
                                                          + "' in component '" + owningComponent(voiEquivalentVariable)->name()
                                                          + "' cannot be both a variable of integration and initialised.");
                            issue->mPimpl->setReferenceRule(Issue::ReferenceRule::ANALYSER_VOI_INITIALISED);
                            issue->mPimpl->mItem->mPimpl->setVariable(voiEquivalentVariable);

                            addIssue(issue);

                            isVoiInitialised = true;
                        }
                    }

                    if (!isVoiInitialised) {
                        mAnalyserModel->mPimpl->mVoi = AnalyserVariable::AnalyserVariableImpl::create();

                        mAnalyserModel->mPimpl->mVoi->mPimpl->populate(AnalyserVariable::Type::VARIABLE_OF_INTEGRATION,
                                                                       0, nullptr, voi, mAnalyserModel, {});
                    }
                }
            } while (voi == nullptr);
        } else {
            auto voiVariable = mAnalyserModel->mPimpl->mVoi->variable();

            if (!mAnalyserModel->areEquivalentVariables(astVariable, voiVariable)) {
                auto issue = Issue::IssueImpl::create();

                issue->mPimpl->setDescription("Variable '" + voiVariable->name()
                                              + "' in component '" + owningComponent(voiVariable)->name()
                                              + "' and variable '" + astVariable->name()
                                              + "' in component '" + owningComponent(astVariable)->name()
                                              + "' cannot both be the variable of integration.");
                issue->mPimpl->setReferenceRule(Issue::ReferenceRule::ANALYSER_VOI_SEVERAL);
                issue->mPimpl->mItem->mPimpl->setVariable(astVariable);

                addIssue(issue);
            }
        }
    }

    // Make sure that we only use first-order ODEs.

    if ((ast->mPimpl->mType == AnalyserEquationAst::Type::CN)
        && (astParent->mPimpl->mType == AnalyserEquationAst::Type::DEGREE)
        && (astGrandparent->mPimpl->mType == AnalyserEquationAst::Type::BVAR)) {
        double value;

        convertToDouble(ast->mPimpl->mValue, value);

        if (!areEqual(value, 1.0)) {
            auto variable = astGreatGrandparent->mPimpl->mOwnedRightChild->variable();
            auto issue = Issue::IssueImpl::create();

            issue->mPimpl->setDescription("The differential equation for variable '" + variable->name()
                                          + "' in component '" + owningComponent(variable)->name()
                                          + "' must be of the first order.");
            issue->mPimpl->mItem->mPimpl->setMath(owningComponent(variable));
            issue->mPimpl->setReferenceRule(Issue::ReferenceRule::ANALYSER_ODE_NOT_FIRST_ORDER);

            addIssue(issue);
        }
    }

    // Make a variable a state if it is used in an ODE.

    if ((ast->mPimpl->mType == AnalyserEquationAst::Type::CI)
        && (astParent->mPimpl->mType == AnalyserEquationAst::Type::DIFF)) {
        internalVariable(ast->variable())->makeState();
    }

    // Recursively check the given AST's children.

    analyseEquationAst(ast->mPimpl->mOwnedLeftChild);
    analyseEquationAst(ast->mPimpl->mOwnedRightChild);
}

void Analyser::AnalyserImpl::updateUnitsMapWithStandardUnit(const std::string &unitsName,
                                                            UnitsMap &unitsMap,
                                                            double unitsExponent)
{
    // Update the given units map using the given standard unit.

    for (const auto &iter : standardUnitsList.at(unitsName)) {
        if (unitsMap.find(iter.first) == unitsMap.end()) {
            unitsMap.emplace(iter.first, 0.0);
        }

        unitsMap[iter.first] += iter.second * unitsExponent;
    }
}

void Analyser::AnalyserImpl::updateUnitsMap(const ModelPtr &model,
                                            const std::string &unitsName,
                                            UnitsMap &unitsMap,
                                            bool userUnitsMap,
                                            double unitsExponent,
                                            double unitsMultiplier)
{
    // Update the given units map using the given information.

    if (userUnitsMap) {
        if (unitsName != "dimensionless") {
            unitsMap.emplace(unitsName, unitsExponent);
        }
    } else {
        if (isStandardUnitName(unitsName)) {
            updateUnitsMapWithStandardUnit(unitsName, unitsMap, unitsExponent);
        } else {
            UnitsPtr units = model->units(unitsName);

            if (units->isBaseUnit()) {
                auto iter = unitsMap.find(unitsName);

                if (iter == unitsMap.end()) {
                    unitsMap.emplace(unitsName, unitsExponent);
                } else {
                    unitsMap[iter->first] += unitsExponent;
                }
            } else {
                std::string reference;
                std::string prefix;
                double exponent;
                double multiplier;
                std::string id;

                for (size_t i = 0; i < units->unitCount(); ++i) {
                    units->unitAttributes(i, reference, prefix, exponent, multiplier, id);

                    if (isStandardUnitName(reference)) {
                        updateUnitsMapWithStandardUnit(reference, unitsMap, exponent * unitsExponent);
                    } else {
                        updateUnitsMap(model, reference, unitsMap, userUnitsMap,
                                       exponent * unitsExponent,
                                       unitsMultiplier + (std::log10(multiplier) + convertPrefixToInt(prefix)) * unitsExponent);
                    }
                }
            }
        }
    }
}

UnitsMap Analyser::AnalyserImpl::multiplyDivideUnitsMaps(const UnitsMap &firstUnitsMap,
                                                         const UnitsMap &secondUnitsMap,
                                                         bool multiply)
{
    // Multiply/divide the given units maps together, following a multiplication
    // (multiply = true) or a division (multiply = false).

    auto res = firstUnitsMap;
    auto sign = multiply ? 1.0 : -1.0;

    for (const auto &units : secondUnitsMap) {
        auto it = res.find(units.first);

        if (it == res.end()) {
            res.emplace(units.first, sign * units.second);
        } else {
            it->second += sign * units.second;

            if (areNearlyEqual(it->second, 0.0)) {
                // The units has now an exponent value of zero, so no need to
                // track it anymore.

                res.erase(it);
            }
        }
    }

    return res;
}

UnitsMaps Analyser::AnalyserImpl::multiplyDivideUnitsMaps(const UnitsMaps &firstUnitsMaps,
                                                          const UnitsMaps &secondUnitsMaps,
                                                          bool multiply)
{
    // Multiply/divide the given units maps together, following a multiplication
    // (multiply = true) or a division (multiply = false).

    UnitsMaps res;

    res.reserve(firstUnitsMaps.size() * secondUnitsMaps.size());

    for (const auto &firstUnitsMap : firstUnitsMaps) {
        for (const auto &secondUnitsMap : secondUnitsMaps) {
            res.push_back(multiplyDivideUnitsMaps(firstUnitsMap, secondUnitsMap, multiply));
        }
    }

    return res;
}

UnitsMaps Analyser::AnalyserImpl::multiplyDivideUnitsMaps(const UnitsMaps &unitsMaps,
                                                          double factor,
                                                          bool multiply)
{
    // Multiply/divide the given units maps by the given factor, following a
    // multiplication (multiply = true) or a division (multiply = false).

    auto res = unitsMaps;
    auto realFactor = multiply ? factor : 1.0 / factor;

    for (auto &unitsMap : res) {
        for (auto &unitsItem : unitsMap) {
            unitsItem.second *= realFactor;
        }
    }

    return res;
}

double Analyser::AnalyserImpl::multiplyDivideUnitsMultipliers(double firstUnitsMultiplier,
                                                              double secondUnitsMultiplier,
                                                              bool multiply)
{
    // Multiply/divide the given units multipliers together, following a
    // multiplication (multiply = true) or a division (multiply = false).

    return firstUnitsMultiplier + (multiply ? 1.0 : -1.0) * secondUnitsMultiplier;
}

UnitsMultipliers Analyser::AnalyserImpl::multiplyDivideUnitsMultipliers(const UnitsMultipliers &firstUnitsMultipliers,
                                                                        const UnitsMultipliers &secondUnitsMultipliers,
                                                                        bool multiply)
{
    // Multiply/divide the given units multipliers together, following a
    // multiplication (multiply = true) or a division (multiply = false).

    UnitsMultipliers res;

    res.reserve(firstUnitsMultipliers.size() * secondUnitsMultipliers.size());

    for (const auto &firstUnitsMultiplier : firstUnitsMultipliers) {
        for (const auto &secondUnitsMultiplier : secondUnitsMultipliers) {
            res.push_back(multiplyDivideUnitsMultipliers(firstUnitsMultiplier,
                                                         secondUnitsMultiplier,
                                                         multiply));
        }
    }

    return res;
}

UnitsMultipliers Analyser::AnalyserImpl::multiplyDivideUnitsMultipliers(double firstUnitsMultiplier,
                                                                        const UnitsMultipliers &secondUnitsMultipliers,
                                                                        bool multiply)
{
    // Multiply/divide the given units multipliers together, following a
    // multiplication (multiply = true) or a division (multiply = false).

    UnitsMultipliers res;

    res.reserve(secondUnitsMultipliers.size());

    for (const auto &secondUnitsMultiplier : secondUnitsMultipliers) {
        res.push_back(multiplyDivideUnitsMultipliers(firstUnitsMultiplier,
                                                     secondUnitsMultiplier,
                                                     multiply));
    }

    return res;
}

UnitsMultipliers Analyser::AnalyserImpl::powerRootUnitsMultipliers(const UnitsMultipliers &unitsMultipliers,
                                                                   double factor,
                                                                   bool power)
{
    // Power/root the given units multipliers to the given factor, following a
    // power (power = true) or a root (power = false) operation.

    UnitsMultipliers res;
    auto realFactor = power ? factor : 1.0 / factor;

    res.reserve(unitsMultipliers.size());

    for (const auto &unitsMultiplier : unitsMultipliers) {
        res.push_back(realFactor * unitsMultiplier);
    }

    return res;
}

bool Analyser::AnalyserImpl::areSameUnitsMaps(const UnitsMaps &firstUnitsMaps,
                                              const UnitsMaps &secondUnitsMaps)
{
    // Check whether the given units maps are the same by checking their
    // exponents.

    for (const auto &firstUnitsMap : firstUnitsMaps) {
        for (const auto &secondUnitsMap : secondUnitsMaps) {
            UnitsMap unitsMap;

            for (const auto &units : firstUnitsMap) {
                if (units.first != "dimensionless") {
                    unitsMap[units.first] += units.second;
                }
            }

            for (const auto &units : secondUnitsMap) {
                if (units.first != "dimensionless") {
                    unitsMap[units.first] -= units.second;
                }
            }

            for (const auto &unitsItem : unitsMap) {
                if (!areNearlyEqual(unitsItem.second, 0.0)) {
                    return false;
                }
            }
        }
    }

    return true;
}

bool Analyser::AnalyserImpl::isDimensionlessUnitsMaps(const UnitsMaps &unitsMaps)
{
    // Check whether the given units maps is dimensionless.

    for (const auto &unitsMap : unitsMaps) {
        for (const auto &unitsItem : unitsMap) {
            if (unitsItem.first != "dimensionless") {
                return false;
            }
        }
    }

    return true;
}

bool Analyser::AnalyserImpl::areSameUnitsMultipliers(const UnitsMultipliers &firstUnitsMultipliers,
                                                     const UnitsMultipliers &secondUnitsMultipliers)
{
    // Return whether the units multipliers are equals.

    for (const auto &firstUnitsMultiplier : firstUnitsMultipliers) {
        for (const auto &secondUnitsMultiplier : secondUnitsMultipliers) {
            if (!areNearlyEqual(firstUnitsMultiplier, secondUnitsMultiplier)) {
                return false;
            }
        }
    }

    return true;
}

void Analyser::AnalyserImpl::updateUnitsMultiplier(const ModelPtr &model,
                                                   const std::string &unitsName,
                                                   double &newUnitsMultiplier,
                                                   double unitsExponent,
                                                   double unitsMultiplier)
{
    // Update the given units multiplier using the given information.

    if (isStandardUnitName(unitsName)) {
        newUnitsMultiplier += unitsMultiplier + standardMultiplierList.at(unitsName);
    } else {
        auto units = model->units(unitsName);

        if (units->isBaseUnit()) {
            newUnitsMultiplier += unitsMultiplier;
        } else {
            std::string reference;
            std::string prefix;
            double exponent;
            double multiplier;
            std::string id;

            for (size_t i = 0; i < units->unitCount(); ++i) {
                units->unitAttributes(i, reference, prefix, exponent, multiplier, id);

                if (isStandardUnitName(reference)) {
                    newUnitsMultiplier += unitsMultiplier + (standardMultiplierList.at(reference) + std::log10(multiplier) + convertPrefixToInt(prefix)) * exponent * unitsExponent;
                } else {
                    updateUnitsMultiplier(model, reference, newUnitsMultiplier,
                                          exponent * unitsExponent,
                                          unitsMultiplier + (std::log10(multiplier) + convertPrefixToInt(prefix)) * unitsExponent);
                }
            }
        }
    }
}

std::string Analyser::AnalyserImpl::componentName(const AnalyserEquationAstPtr &ast)
{
    // Return the name of the component in which the given AST is, by going
    // through the AST, if needed, and returning the component of the first
    // variable we find on the LHS/RHS.

    auto astVariable = ast->variable();

    if (astVariable != nullptr) {
        return std::dynamic_pointer_cast<Component>(astVariable->parent())->name();
    }

    auto res = (ast->mPimpl->mOwnedLeftChild != nullptr) ?
                   componentName(ast->mPimpl->mOwnedLeftChild) :
                   "";

    if (res.empty()) {
        res = (ast->mPimpl->mOwnedRightChild != nullptr) ?
                  componentName(ast->mPimpl->mOwnedRightChild) :
                  "";
    }

    return res;
}

double Analyser::AnalyserImpl::powerValue(const AnalyserEquationAstPtr &ast,
                                          PowerData &powerData)
{
    // Make sure that we have an AST to process.

    static const double NAN = std::numeric_limits<double>::quiet_NaN();

    if (ast == nullptr) {
        return NAN;
    }

    // Retrieve the power value of the LHS and RHS of the given AST.

    auto lhs = powerValue(ast->mPimpl->mOwnedLeftChild, powerData);

    if (!powerData.mExponentValueAvailable) {
        return lhs;
    }

    auto rhs = powerValue(ast->mPimpl->mOwnedRightChild, powerData);

    if (!powerData.mExponentValueAvailable) {
        return rhs;
    }

    // Return the power value for the given AST.

    switch (ast->mPimpl->mType) {
        // Relational and logical operators.

    case AnalyserEquationAst::Type::EQ:
        return lhs == rhs;
    case AnalyserEquationAst::Type::NEQ:
        return lhs != rhs;
    case AnalyserEquationAst::Type::LT:
        return lhs < rhs;
    case AnalyserEquationAst::Type::LEQ:
        return lhs <= rhs;
    case AnalyserEquationAst::Type::GT:
        return lhs > rhs;
    case AnalyserEquationAst::Type::GEQ:
        return lhs >= rhs;
    case AnalyserEquationAst::Type::AND:
        return lhs && rhs;
    case AnalyserEquationAst::Type::OR:
        return lhs || rhs;
    case AnalyserEquationAst::Type::XOR:
        return (lhs != 0.0) ^ (rhs != 0.0);
    case AnalyserEquationAst::Type::NOT:
        return !lhs;

        // Arithmetic operators.

    case AnalyserEquationAst::Type::PLUS:
        if (ast->mPimpl->mOwnedRightChild != nullptr) {
            return lhs + rhs;
        }

        return lhs;
    case AnalyserEquationAst::Type::MINUS:
        if (ast->mPimpl->mOwnedRightChild != nullptr) {
            return lhs - rhs;
        }

        return -lhs;
    case AnalyserEquationAst::Type::TIMES:
        return lhs * rhs;
    case AnalyserEquationAst::Type::DIVIDE:
        return lhs / rhs;
    case AnalyserEquationAst::Type::POWER:
        return std::pow(lhs, rhs);
    case AnalyserEquationAst::Type::ROOT:
        if (ast->mPimpl->mOwnedLeftChild->type() == AnalyserEquationAst::Type::DEGREE) {
            return std::pow(rhs, 1.0 / lhs);
        }

        return std::pow(lhs, 1.0 / 2.0);
    case AnalyserEquationAst::Type::ABS:
        return std::abs(lhs);
    case AnalyserEquationAst::Type::EXP:
        return std::exp(lhs);
    case AnalyserEquationAst::Type::LN:
        return std::log(lhs);
    case AnalyserEquationAst::Type::LOG:
        if (ast->mPimpl->mOwnedLeftChild->type() == AnalyserEquationAst::Type::LOGBASE) {
            if (areNearlyEqual(lhs, 10.0)) {
                return std::log10(rhs);
            }

            return std::log(rhs) / std::log(lhs);
        }

        return std::log10(lhs);
    case AnalyserEquationAst::Type::CEILING:
        return std::ceil(lhs);
    case AnalyserEquationAst::Type::FLOOR:
        return std::floor(lhs);
    case AnalyserEquationAst::Type::MIN:
        return (lhs < rhs) ? lhs : rhs;
    case AnalyserEquationAst::Type::MAX:
        return (lhs > rhs) ? lhs : rhs;
    case AnalyserEquationAst::Type::REM:
        return std::fmod(lhs, rhs);

        // Trigonometric operators.

    case AnalyserEquationAst::Type::SIN:
        return std::sin(lhs);
    case AnalyserEquationAst::Type::COS:
        return std::cos(lhs);
    case AnalyserEquationAst::Type::TAN:
        return std::tan(lhs);
    case AnalyserEquationAst::Type::SEC:
        return 1.0 / std::cos(lhs);
    case AnalyserEquationAst::Type::CSC:
        return 1.0 / std::sin(lhs);
    case AnalyserEquationAst::Type::COT:
        return 1.0 / std::tan(lhs);
    case AnalyserEquationAst::Type::SINH:
        return std::sinh(lhs);
    case AnalyserEquationAst::Type::COSH:
        return std::cosh(lhs);
    case AnalyserEquationAst::Type::TANH:
        return std::tanh(lhs);
    case AnalyserEquationAst::Type::SECH:
        return 1.0 / std::cosh(lhs);
    case AnalyserEquationAst::Type::CSCH:
        return 1.0 / std::sinh(lhs);
    case AnalyserEquationAst::Type::COTH:
        return 1.0 / std::tanh(lhs);
    case AnalyserEquationAst::Type::ASIN:
        return std::asin(lhs);
    case AnalyserEquationAst::Type::ACOS:
        return std::acos(lhs);
    case AnalyserEquationAst::Type::ATAN:
        return std::atan(lhs);
    case AnalyserEquationAst::Type::ASEC:
        return std::acos(1.0 / lhs);
    case AnalyserEquationAst::Type::ACSC:
        return std::asin(1.0 / lhs);
    case AnalyserEquationAst::Type::ACOT:
        return std::atan(1.0 / lhs);
    case AnalyserEquationAst::Type::ASINH:
        return std::asinh(lhs);
    case AnalyserEquationAst::Type::ACOSH:
        return std::acosh(lhs);
    case AnalyserEquationAst::Type::ATANH:
        return std::atanh(lhs);
    case AnalyserEquationAst::Type::ASECH: {
        auto xInv = 1.0 / lhs;

        return std::log(xInv + std::sqrt(xInv * xInv - 1.0));
    }
    case AnalyserEquationAst::Type::ACSCH: {
        auto xInv = 1.0 / lhs;

        return std::log(xInv + std::sqrt(xInv * xInv + 1.0));
    }
    case AnalyserEquationAst::Type::ACOTH: {
        auto xInv = 1.0 / lhs;

        return 0.5 * std::log((1.0 + xInv) / (1.0 - xInv));
    }

        // Token elements.

    case AnalyserEquationAst::Type::CI: {
        auto initialValue = ast->variable()->initialValue();

        if (initialValue.empty()) {
            powerData.mExponentValueAvailable = false;

            return NAN;
        }

        powerData.mExponentValueChangeable = true;

        return std::stod(initialValue);
    }
    case AnalyserEquationAst::Type::CN:
        return std::stod(ast->value());

        // Qualifier elements.

    case AnalyserEquationAst::Type::DEGREE:
    case AnalyserEquationAst::Type::LOGBASE:
        return lhs;

        // Constants.

    case AnalyserEquationAst::Type::TRUE:
        return 1.0;
    case AnalyserEquationAst::Type::FALSE:
        return 0.0;
    case AnalyserEquationAst::Type::E: {
        static const double E = exp(1.0);

        return E;
    }
    case AnalyserEquationAst::Type::PI:
        return M_PI;
    case AnalyserEquationAst::Type::INF: {
        static const double INF = std::numeric_limits<double>::infinity();

        return INF;
    }
    case AnalyserEquationAst::Type::NAN:
        return NAN;
    default:
        // This corresponds to one of the following cases:
        //  - AnalyserEquationAst::Type::EQUALITY (we should never come across this case);
        //  - AnalyserEquationAst::Type::DIFF,
        //    AnalyserEquationAst::Type::BVAR; and
        //  - AnalyserEquationAst::Type::PIECEWISE,
        //    AnalyserEquationAst::Type::PIECE,
        //    AnalyserEquationAst::Type::OTHERWISE.
        // In all these cases, we may not have a constant (power) value.

        powerData.mExponentValueAvailable = false;

        return NAN;
    }
}

std::string Analyser::AnalyserImpl::expression(const AnalyserEquationAstPtr &ast,
                                               bool includeHierarchy)
{
    // Return the generated code for the given AST, specifying the equation and
    // component in which it is, if needed and requested.

    std::string res = "'" + Generator::equationCode(ast, mGeneratorProfile) + "'";

    if (includeHierarchy) {
        auto equationAst = ast;
        auto equationAstParent = ast->parent();
        auto equationAstGrandparent = (equationAstParent != nullptr) ? equationAstParent->parent() : nullptr;

        while (equationAstParent != nullptr) {
            equationAst = equationAstParent;
            equationAstParent = equationAstGrandparent;
            equationAstGrandparent = (equationAstParent != nullptr) ? equationAstParent->parent() : nullptr;

            res += std::string(" in")
                   + ((equationAstParent == nullptr) ? " equation" : "")
                   + " '" + Generator::equationCode(equationAst, mGeneratorProfile) + "'";
        }

        res += " in component '" + componentName(equationAst) + "'";
    }

    return res;
}

std::string Analyser::AnalyserImpl::expressionUnits(const UnitsMaps &unitsMaps,
                                                    const UnitsMultipliers &unitsMultipliers)
{
    // Return a string version of the given units maps and units multipliers.

    Strings units;

    for (size_t i = 0; i < unitsMaps.size(); ++i) {
        auto unitsMap = unitsMaps[i];
        std::string unit;

        if (!unitsMultipliers.empty()) {
            auto intExponent = int(unitsMultipliers[i]);
            auto exponent = areNearlyEqual(unitsMultipliers[i], intExponent) ?
                                convertToString(intExponent) :
                                convertToString(unitsMultipliers[i], false);

            if (exponent != "0") {
                unit += "10^" + exponent;
            }
        }

        for (const auto &unitsItem : unitsMap) {
            if ((unitsItem.first != "dimensionless")
                && !areNearlyEqual(unitsItem.second, 0.0)) {
                auto intExponent = int(unitsItem.second);
                auto exponent = areNearlyEqual(unitsItem.second, intExponent) ?
                                    convertToString(intExponent) :
                                    convertToString(unitsItem.second, false);

                if (!unit.empty()) {
                    unit += " x ";
                }

                unit += unitsItem.first;

                if (exponent != "1") {
                    unit += "^" + exponent;
                }
            }
        }

        if (!unit.empty()) {
            units.push_back(unit);
        }
    }

    std::string unitsString;

    for (size_t i = 0; i < units.size(); ++i) {
        if (i > 0) {
            unitsString += (i == units.size() - 1) ? " and " : ", ";
        }

        unitsString += "'" + units[i] + "'";
    }

    return unitsString;
}

std::string Analyser::AnalyserImpl::expressionUnits(const AnalyserEquationAstPtr &ast,
                                                    const UnitsMaps &unitsMaps,
                                                    const UnitsMaps &userUnitsMaps,
                                                    const UnitsMultipliers &unitsMultipliers)
{
    // Return a string version of the given AST and (user) units maps and units
    // multipliers.

    auto res = expression(ast, false) + " is ";
    auto unitsString = expressionUnits(unitsMaps, unitsMultipliers);
    auto userUnitsString = expressionUnits(userUnitsMaps);

    if (userUnitsString.empty()) {
        res += "'dimensionless'";
    } else {
        res += "in " + userUnitsString;

        if (!unitsString.empty() && (unitsString != userUnitsString)) {
            res += " (i.e. " + unitsString + ")";
        }
    }

    return res;
}

void Analyser::AnalyserImpl::defaultUnitsMapsAndMultipliers(UnitsMaps &unitsMaps,
                                                            UnitsMaps &userUnitsMaps,
                                                            UnitsMultipliers &unitsMultipliers)
{
    // Default units maps and multipliers.

    unitsMaps = {UnitsMap()};
    userUnitsMaps = {UnitsMap()};
    unitsMultipliers = {0.0};
}

void Analyser::AnalyserImpl::analyseEquationUnits(const AnalyserEquationAstPtr &ast,
                                                  UnitsMaps &unitsMaps,
                                                  UnitsMaps &userUnitsMaps,
                                                  UnitsMultipliers &unitsMultipliers,
                                                  std::string &issueDescription,
                                                  PowerData &powerData)
{
    // Analyse the units used with different MathML elements (table 2.1 of the
    // CellML 2.0 normative specification; see https://bit.ly/3vBbyO5):
    //  - Simple operands ('ci' and 'cn'; note: 'sep' is not relevant here): the
    //    operand can have any unit.
    //  - Basic structural (note: 'apply' is not relevant here):
    //     - 'piecewise': the returned value of the different 'piece' and
    //       'otherwise' statements should have equivalent units.
    //     - 'piece': the returned value can have any unit while the condition
    //       should be dimensionless.
    //     - 'otherwise': the returned value can have any unit.
    //  - Relational operators ('eq', 'neq', 'gt', 'lt', 'geq' and 'leq'): the
    //    two operands should have equivalent units. (The result of the
    //    comparison is dimensionless.)
    //  - Logical operators:
    //     - 'and', 'or', 'xor': the two operands should be dimensionless.
    //     - 'not': the operand should be dimensionless.
    //  - Arithmetic operators:
    //     - 'plus': the two operands should have equivalent units.
    //     - 'minus': if there is one operand, then it can have any unit. If
    //       there are two operands, then they should have equivalent units.
    //     - 'times' and 'divide': the two operands can have any units.
    //     - 'power': the base can have any unit while the exponent should be
    //       dimensionless.
    //     - 'root': the base can have any unit while the exponent, if present,
    //       should be dimensionless.
    //     - 'abs': the argument can have any unit.
    //     - 'exp' and 'ln': the argument should be dimensionless.
    //     - 'log': the argument and the base, if present, should be
    //       dimensionless.
    //     - 'floor' and 'ceiling': the argument can have any unit.
    //     - 'min' and 'max': all the arguments should have equivalent units.
    //     - 'rem': the two arguments should have equivalent units.
    //  - Calculus elements ('diff'): the differentiated variable can have any
    //    unit. (See 'bvar' below for the bounding variable.)
    //  - Qualifier elements:
    //     - 'bvar': a bounding variable can have any unit.
    //     - 'degree': a degree should be dimensionless.
    //     - 'logbase': a base should be dimensionless.
    //  - Trigonometric operators ('sin', 'cos', 'tan', etc.): the argument
    //    should be dimensionless.
    //  - Mathematical and logical constants ('pi', 'exponentiale',
    //    'notanumber','infinity', 'true' and 'false'): those constants are
    //    dimensionless.

    // Make sure that we have an AST to analyse.

    if (ast == nullptr) {
        unitsMaps = {};
        userUnitsMaps = {};
        unitsMultipliers = {};

        return;
    }

    // Check whether we are dealing with a CI/CN element and, if so, retrieve
    // both its units maps and multipliers.

    switch (ast->mPimpl->mType) {
    case AnalyserEquationAst::Type::CI:
    case AnalyserEquationAst::Type::CN: {
        auto units = mCiCnUnits[ast];
        auto model = owningModel(units);

        defaultUnitsMapsAndMultipliers(unitsMaps, userUnitsMaps, unitsMultipliers);

        for (auto &unitsMap : unitsMaps) {
            updateUnitsMap(model, units->name(), unitsMap);
        }

        for (auto &userUnitsMap : userUnitsMaps) {
            updateUnitsMap(model, units->name(), userUnitsMap, true);
        }

        for (auto &unitsMultiplier : unitsMultipliers) {
            updateUnitsMultiplier(model, units->name(), unitsMultiplier);
        }

        return;
    }
    default:
        break;
    }

    // Check the left and right children.

    analyseEquationUnits(ast->mPimpl->mOwnedLeftChild, unitsMaps, userUnitsMaps, unitsMultipliers, issueDescription, powerData);

    if (!issueDescription.empty()) {
        return;
    }

    UnitsMaps rightUnitsMaps;
    UnitsMaps rightUserUnitsMaps;
    UnitsMultipliers rightUnitsMultipliers;

    analyseEquationUnits(ast->mPimpl->mOwnedRightChild, rightUnitsMaps, rightUserUnitsMaps, rightUnitsMultipliers, issueDescription, powerData);

    if (!issueDescription.empty()) {
        return;
    }

    switch (ast->mPimpl->mType) {
    case AnalyserEquationAst::Type::EQUALITY:
    case AnalyserEquationAst::Type::EQ:
    case AnalyserEquationAst::Type::NEQ:
    case AnalyserEquationAst::Type::LT:
    case AnalyserEquationAst::Type::LEQ:
    case AnalyserEquationAst::Type::GT:
    case AnalyserEquationAst::Type::GEQ:
    case AnalyserEquationAst::Type::PLUS:
    case AnalyserEquationAst::Type::MINUS:
    case AnalyserEquationAst::Type::MIN:
    case AnalyserEquationAst::Type::MAX:
    case AnalyserEquationAst::Type::REM: {
        auto sameUnitsMaps = rightUnitsMaps.empty()
                             || areSameUnitsMaps(unitsMaps, rightUnitsMaps);
        auto sameUnitsMultipliers = rightUnitsMultipliers.empty()
                                    || areSameUnitsMultipliers(unitsMultipliers, rightUnitsMultipliers);
        auto sameUnits = sameUnitsMaps && sameUnitsMultipliers;

        if (sameUnits && powerData.mExponentValueAvailable && !powerData.mExponentValueChangeable) {
            // Relational operators result in a dimensionless unit.

            switch (ast->mPimpl->mType) {
            case AnalyserEquationAst::Type::EQ:
            case AnalyserEquationAst::Type::NEQ:
            case AnalyserEquationAst::Type::LT:
            case AnalyserEquationAst::Type::LEQ:
            case AnalyserEquationAst::Type::GT:
            case AnalyserEquationAst::Type::GEQ:
                defaultUnitsMapsAndMultipliers(unitsMaps, userUnitsMaps, unitsMultipliers);

                break;
            default:
                break;
            }
        } else if (powerData.mExponentValueAvailable) {
            if (sameUnits) {
                if (!powerData.mDimensionlessBase) {
                    issueDescription = "The units in " + expression(ast) + " are equivalent as long as the value of "
                                       + expression(powerData.mExponentAst, false) + " is equal to '" + generateDoubleCode(convertToString(powerData.mExponentValue)) + "'.";
                }
            } else {
                issueDescription = "The units in " + expression(ast) + " are not equivalent. "
                                   + expressionUnits(ast->mPimpl->mOwnedLeftChild, unitsMaps, userUnitsMaps, unitsMultipliers) + " while "
                                   + expressionUnits(ast->mPimpl->mOwnedRightChild, rightUnitsMaps, rightUserUnitsMaps, rightUnitsMultipliers) + ".";
            }
        } else if (!isDimensionlessUnitsMaps(unitsMaps)) {
            auto leftChild = ast->mPimpl->mOwnedLeftChild;
            auto rightChild = ast->mPimpl->mOwnedRightChild;

            if (leftChild->type() == AnalyserEquationAst::Type::POWER) {
                if (rightChild != nullptr) {
                    if (rightChild->type() == AnalyserEquationAst::Type::POWER) {
                        issueDescription = "The units in " + expression(ast) + " may not be equivalent. "
                                           + expression(leftChild->mPimpl->mOwnedRightChild, false) + " may result in " + expression(leftChild, false) + " having different units while "
                                           + expression(rightChild->mPimpl->mOwnedRightChild, false) + " may result in " + expression(rightChild, false) + " having different units.";
                    } else {
                        issueDescription = "The units in " + expression(ast) + " may not be equivalent. "
                                           + expression(leftChild->mPimpl->mOwnedRightChild, false) + " may result in " + expression(leftChild, false) + " having different units while "
                                           + expressionUnits(rightChild, unitsMaps, userUnitsMaps, unitsMultipliers) + ".";
                    }
                }
            } else if (rightChild->type() == AnalyserEquationAst::Type::POWER) {
                issueDescription = "The units in " + expression(ast) + " may not be equivalent. "
                                   + expressionUnits(leftChild, unitsMaps, userUnitsMaps, unitsMultipliers) + " while "
                                   + expression(rightChild->mPimpl->mOwnedRightChild, false) + " may result in " + expression(rightChild, false) + " having different units.";
            }
        }
    } break;
    case AnalyserEquationAst::Type::PIECEWISE:
        unitsMaps.insert(std::end(unitsMaps),
                         std::begin(rightUnitsMaps),
                         std::end(rightUnitsMaps));
        userUnitsMaps.insert(std::end(userUnitsMaps),
                             std::begin(rightUserUnitsMaps),
                             std::end(rightUserUnitsMaps));
        unitsMultipliers.insert(std::end(unitsMultipliers),
                                std::begin(rightUnitsMultipliers),
                                std::end(rightUnitsMultipliers));

        break;
    case AnalyserEquationAst::Type::PIECE:
        if (!Analyser::AnalyserImpl::isDimensionlessUnitsMaps(rightUnitsMaps)) {
            issueDescription = "The unit of " + expression(ast->mPimpl->mOwnedRightChild) + " is not dimensionless. "
                               + expressionUnits(ast->mPimpl->mOwnedRightChild, rightUnitsMaps, rightUserUnitsMaps, rightUnitsMultipliers) + ".";
        }

        break;
    case AnalyserEquationAst::Type::AND:
    case AnalyserEquationAst::Type::OR:
    case AnalyserEquationAst::Type::XOR:
    case AnalyserEquationAst::Type::NOT:
    case AnalyserEquationAst::Type::EXP:
    case AnalyserEquationAst::Type::LN:
    case AnalyserEquationAst::Type::LOG: {
        auto isDimensionlessUnitsMaps = Analyser::AnalyserImpl::isDimensionlessUnitsMaps(unitsMaps);

        if (!isDimensionlessUnitsMaps) {
            auto isDimensionlessRightUnitsMaps = Analyser::AnalyserImpl::isDimensionlessUnitsMaps(rightUnitsMaps);
            issueDescription.reserve(512);

            issueDescription = "The unit";

            if (!isDimensionlessRightUnitsMaps) {
                issueDescription += "s";
            }

            issueDescription += " of " + expression(ast->mPimpl->mOwnedLeftChild, false);

            if (!isDimensionlessRightUnitsMaps) {
                issueDescription += " and " + expression(ast->mPimpl->mOwnedRightChild, false);
            }

            issueDescription += " in " + expression(ast);

            if (!isDimensionlessRightUnitsMaps) {
                issueDescription += " are ";
            } else {
                issueDescription += " is ";
            }

            issueDescription += "not dimensionless. " + expressionUnits(ast->mPimpl->mOwnedLeftChild, unitsMaps, userUnitsMaps, unitsMultipliers);

            if (!isDimensionlessRightUnitsMaps) {
                issueDescription += " while " + expressionUnits(ast->mPimpl->mOwnedRightChild, rightUnitsMaps, rightUserUnitsMaps, rightUnitsMultipliers);
            }

            issueDescription += ".";
        }
    } break;
    case AnalyserEquationAst::Type::TIMES:
    case AnalyserEquationAst::Type::DIVIDE: {
        auto isTimes = ast->mPimpl->mType == AnalyserEquationAst::Type::TIMES;

        unitsMaps = multiplyDivideUnitsMaps(unitsMaps, rightUnitsMaps, isTimes);
        userUnitsMaps = multiplyDivideUnitsMaps(userUnitsMaps, rightUserUnitsMaps, isTimes);
        unitsMultipliers = multiplyDivideUnitsMultipliers(unitsMultipliers, rightUnitsMultipliers, isTimes);
    } break;
    case AnalyserEquationAst::Type::POWER:
    case AnalyserEquationAst::Type::ROOT: {
        auto isPower = ast->mPimpl->mType == AnalyserEquationAst::Type::POWER;

        // Determine whether we are dealing with a dimensionless base.

        if (isPower) {
            powerData.mDimensionlessBase = Analyser::AnalyserImpl::isDimensionlessUnitsMaps(unitsMaps);
        } else {
            if (ast->mPimpl->mOwnedLeftChild->type() == AnalyserEquationAst::Type::DEGREE) {
                powerData.mDimensionlessBase = Analyser::AnalyserImpl::isDimensionlessUnitsMaps(rightUnitsMaps);
            } else {
                powerData.mDimensionlessBase = Analyser::AnalyserImpl::isDimensionlessUnitsMaps(unitsMaps);
            }
        }

        // Determine whether we are dealing with a dimensionless exponent and report it if not.

        auto isDimensionlessExponent = true;

        if (isPower
            || (ast->mPimpl->mOwnedLeftChild->type() == AnalyserEquationAst::Type::DEGREE)) {
            powerData.mExponentAst = isPower ?
                                         ast->mPimpl->mOwnedRightChild :
                                         ast->mPimpl->mOwnedLeftChild;
            isDimensionlessExponent = Analyser::AnalyserImpl::isDimensionlessUnitsMaps(isPower ?
                                                                                           rightUnitsMaps :
                                                                                           unitsMaps);

            if (!isDimensionlessExponent) {
                auto exponentUnitsMaps = isPower ? rightUnitsMaps : unitsMaps;
                auto exponentUserUnitsMaps = isPower ? rightUserUnitsMaps : userUnitsMaps;
                auto exponentUnitsMultipliers = isPower ? rightUnitsMultipliers : unitsMultipliers;

                issueDescription = "The unit of " + expression(powerData.mExponentAst) + " is not dimensionless. "
                                   + expressionUnits(powerData.mExponentAst, exponentUnitsMaps, exponentUserUnitsMaps, exponentUnitsMultipliers) + ".";
            }
        }

        // Retrieve the exponent and apply it to our units maps and multipliers.

        if (isDimensionlessExponent) {
            if (isPower) {
                powerData.mExponentValue = powerValue(ast->mPimpl->mOwnedRightChild, powerData);
            } else { // AnalyserEquationAst::Type::ROOT.
                if (ast->mPimpl->mOwnedLeftChild->type() == AnalyserEquationAst::Type::DEGREE) {
                    unitsMaps = rightUnitsMaps;
                    userUnitsMaps = rightUserUnitsMaps;
                    unitsMultipliers = rightUnitsMultipliers;

                    powerData.mExponentValue = powerValue(ast->mPimpl->mOwnedLeftChild, powerData);
                } else {
                    // No DEGREE element, which means that we are dealing with a
                    // square root.

                    powerData.mExponentValue = 2.0;
                }
            }

            if (powerData.mExponentValueAvailable) {
                unitsMaps = multiplyDivideUnitsMaps(unitsMaps, powerData.mExponentValue, isPower);
                userUnitsMaps = multiplyDivideUnitsMaps(userUnitsMaps, powerData.mExponentValue, isPower);
                unitsMultipliers = powerRootUnitsMultipliers(unitsMultipliers, powerData.mExponentValue, isPower);
            }
        }
    } break;
    case AnalyserEquationAst::Type::SIN:
    case AnalyserEquationAst::Type::COS:
    case AnalyserEquationAst::Type::TAN:
    case AnalyserEquationAst::Type::SEC:
    case AnalyserEquationAst::Type::CSC:
    case AnalyserEquationAst::Type::COT:
    case AnalyserEquationAst::Type::SINH:
    case AnalyserEquationAst::Type::COSH:
    case AnalyserEquationAst::Type::TANH:
    case AnalyserEquationAst::Type::SECH:
    case AnalyserEquationAst::Type::CSCH:
    case AnalyserEquationAst::Type::COTH:
    case AnalyserEquationAst::Type::ASIN:
    case AnalyserEquationAst::Type::ACOS:
    case AnalyserEquationAst::Type::ATAN:
    case AnalyserEquationAst::Type::ASEC:
    case AnalyserEquationAst::Type::ACSC:
    case AnalyserEquationAst::Type::ACOT:
    case AnalyserEquationAst::Type::ASINH:
    case AnalyserEquationAst::Type::ACOSH:
    case AnalyserEquationAst::Type::ATANH:
    case AnalyserEquationAst::Type::ASECH:
    case AnalyserEquationAst::Type::ACSCH:
    case AnalyserEquationAst::Type::ACOTH:
        if (!Analyser::AnalyserImpl::isDimensionlessUnitsMaps(unitsMaps)) {
            issueDescription = "The unit of " + expression(ast->mPimpl->mOwnedLeftChild) + " is not dimensionless. "
                               + expressionUnits(ast->mPimpl->mOwnedLeftChild, unitsMaps, userUnitsMaps, unitsMultipliers) + ".";
        }

        break;
    case AnalyserEquationAst::Type::DIFF:
        unitsMaps = multiplyDivideUnitsMaps(unitsMaps, rightUnitsMaps);
        userUnitsMaps = multiplyDivideUnitsMaps(userUnitsMaps, rightUserUnitsMaps);
        unitsMultipliers = multiplyDivideUnitsMultipliers(unitsMultipliers, rightUnitsMultipliers);

        break;
    case AnalyserEquationAst::Type::BVAR:
        for (auto &unitsMap : unitsMaps) {
            for (auto &unitsItem : unitsMap) {
                unitsItem.second *= -1.0;
            }
        }

        for (auto &userUnitsMap : userUnitsMaps) {
            for (auto &userUnits : userUnitsMap) {
                userUnits.second *= -1.0;
            }
        }

        unitsMultipliers = multiplyDivideUnitsMultipliers(0.0, unitsMultipliers, false);

        break;
    case AnalyserEquationAst::Type::TRUE:
    case AnalyserEquationAst::Type::FALSE:
    case AnalyserEquationAst::Type::E:
    case AnalyserEquationAst::Type::PI:
    case AnalyserEquationAst::Type::INF:
    case AnalyserEquationAst::Type::NAN:
        defaultUnitsMapsAndMultipliers(unitsMaps, userUnitsMaps, unitsMultipliers);

        break;
    default:
        // Other types we don't care about.

        break;
    }
}

double Analyser::AnalyserImpl::scalingFactor(const VariablePtr &variable)
{
    return Units::scalingFactor(variable->units(), internalVariable(variable)->mVariable->units());
}

void Analyser::AnalyserImpl::scaleAst(const AnalyserEquationAstPtr &ast,
                                      const AnalyserEquationAstPtr &astParent,
                                      double scalingFactor)
{
    // Scale the given AST using the given scaling factor.

    auto scaledAst = AnalyserEquationAst::create();

    scaledAst->mPimpl->populate(AnalyserEquationAst::Type::TIMES, astParent);

    scaledAst->mPimpl->mOwnedLeftChild = AnalyserEquationAst::create();
    scaledAst->mPimpl->mOwnedRightChild = ast;

    scaledAst->mPimpl->mOwnedLeftChild->mPimpl->populate(AnalyserEquationAst::Type::CN, convertToString(scalingFactor), scaledAst);

    ast->mPimpl->mParent = scaledAst;

    if (astParent->mPimpl->mOwnedLeftChild == ast) {
        astParent->mPimpl->mOwnedLeftChild = scaledAst;
    } else {
        astParent->mPimpl->mOwnedRightChild = scaledAst;
    }
}

void Analyser::AnalyserImpl::scaleEquationAst(const AnalyserEquationAstPtr &ast)
{
    // Make sure that we have an AST to scale.

    if (ast == nullptr) {
        return;
    }

    // Recursively scale the given AST's children.

    scaleEquationAst(ast->mPimpl->mOwnedLeftChild);
    scaleEquationAst(ast->mPimpl->mOwnedRightChild);

    // If the given AST node is a variable (i.e. a CI node) then we may need to
    // do some scaling.

    if (ast->mPimpl->mType == AnalyserEquationAst::Type::CI) {
        // The kind of scaling we may end up doing depends on whether we are
        // dealing with a rate or some other variable, i.e. whether or not it
        // has a DIFF node as a parent.

        auto astParent = ast->parent();

        if (astParent->mPimpl->mType == AnalyserEquationAst::Type::DIFF) {
            // We are dealing with a rate, so retrieve the scaling factor for
            // its corresponding variable of integration and apply it, if
            // needed.

            auto scalingFactor = Analyser::AnalyserImpl::scalingFactor(astParent->mPimpl->mOwnedLeftChild->mPimpl->mOwnedLeftChild->variable());

            if (!areNearlyEqual(scalingFactor, 1.0)) {
                // We need to scale using the inverse of the scaling factor, but
                // how we do it depends on whether the rate is to be computed or
                // used.

                auto astGrandparent = astParent->parent();

                if (astGrandparent->mPimpl->mType == AnalyserEquationAst::Type::EQUALITY) {
                    scaleAst(astGrandparent->mPimpl->mOwnedRightChild, astGrandparent, scalingFactor);
                } else {
                    scaleAst(astParent, astGrandparent, 1.0 / scalingFactor);
                }
            }
        }

        if (((astParent->mPimpl->mType != AnalyserEquationAst::Type::EQUALITY)
             || (astParent->mPimpl->mOwnedLeftChild != ast))
            && (astParent->mPimpl->mType != AnalyserEquationAst::Type::BVAR)) {
            // We are dealing with a variable which is neither a computed
            // variable nor our variable of integration, so retrieve its scaling
            // factor and apply it, if needed, distinguishing between a rate
            // variable and an algebraic variable.

            auto scalingFactor = Analyser::AnalyserImpl::scalingFactor(ast->variable());

            if (!areNearlyEqual(scalingFactor, 1.0)) {
                if (astParent->mPimpl->mType == AnalyserEquationAst::Type::DIFF) {
                    scaleAst(astParent, astParent->parent(), scalingFactor);
                } else {
                    scaleAst(ast, astParent, scalingFactor);
                }
            }
        }
    }
}

bool Analyser::AnalyserImpl::isExternalVariable(const AnalyserInternalVariablePtr &variable)
{
    return variable->mIsExternalVariable;
}

bool Analyser::AnalyserImpl::isStateRateBased(const AnalyserEquationPtr &analyserEquation,
                                              std::unordered_set<AnalyserEquation *> &checkedEquations)
{
    if (!checkedEquations.insert(analyserEquation.get()).second) {
        return false;
    }

    for (const auto &dependency : analyserEquation->dependencies()) {
        // A rate is computed either through an ODE equation or through an NLA
        // equation in case the rate is not on its own on either the LHS or RHS
        // of the equation.

        if ((dependency->type() == AnalyserEquation::Type::ODE)
            // TODO: ---GRY--- Check whether we need to test for dependency->stateCount() == 1 (it's not covered by any tests at the moment).
            /*
            || ((dependency->type() == AnalyserEquation::Type::NLA)
                && (dependency->stateCount() == 1))
            */
            || (dependency->type() == AnalyserEquation::Type::NLA)
            || isStateRateBased(dependency, checkedEquations)) {
            return true;
        }
    }

    return false;
}

void Analyser::AnalyserImpl::resolveUnknownVariablesUsingGlobalNlaSystems()
{
    struct UnresolvedInternalEquationData
    {
        AnalyserInternalEquationPtr internalEquation;
        AnalyserInternalVariablePtrs unresolvedInternalVariables;
    };

    std::vector<UnresolvedInternalEquationData> unresolvedInternalEquations;
    std::unordered_map<AnalyserInternalVariable *, std::vector<size_t>> internalVariableToInternalEquationIndices;

    // Build the unresolved equation/variable graph.
    // Note: global NLA systems are only for remaining unknown algebraic variables. If an unresolved equation still
    //       references a state, then that equation should be revisited once its algebraic dependencies have been
    //       resolved rather than have the state rate folded into the global NLA system.

    for (const auto &internalEquation : mInternalEquations) {
        if ((internalEquation->mType != AnalyserInternalEquation::Type::UNKNOWN) || (!internalEquation->mStateVariables.empty())) {
            continue;
        }

        AnalyserInternalVariablePtrs unresolvedInternalVariables;

        for (const auto &variable : internalEquation->mVariables) {
            if ((variable->mType == AnalyserInternalVariable::Type::UNKNOWN)
                || (variable->mType == AnalyserInternalVariable::Type::ALGEBRAIC_VARIABLE)) {
                unresolvedInternalVariables.push_back(variable);
            }
        }

        if (unresolvedInternalVariables.empty()) {
            continue;
        }

        auto internalEquationIndex = unresolvedInternalEquations.size();

        unresolvedInternalEquations.push_back({internalEquation, unresolvedInternalVariables});

        for (const auto &unresolvedVariable : unresolvedInternalVariables) {
            internalVariableToInternalEquationIndices[unresolvedVariable.get()].push_back(internalEquationIndex);
        }
    }

    // If there are no unresolved equations, then we are done.

    if (unresolvedInternalEquations.empty()) {
        return;
    }

    // Find connected systems in the unresolved internal equation/variable graph, which correspond to structurally
    // square coupled systems, and resolve the unknown internal variables in those systems using global NLA systems.

    std::vector<bool> visitedUnresolvedInternalEquations(unresolvedInternalEquations.size(), false);

    for (size_t unresolvedInternalEquationIndex = 0; unresolvedInternalEquationIndex < unresolvedInternalEquations.size(); ++unresolvedInternalEquationIndex) {
        if (visitedUnresolvedInternalEquations[unresolvedInternalEquationIndex]) {
            continue;
        }

        // We have an unresolved internal equation which is part of a coupled system that we haven't yet visited, so we
        // need to find all the internal equations and internal variables in that coupled system. We do this by
        // traversing the unresolved internal equation/variable graph starting from this unresolved internal equation.

        std::vector<size_t> unresolvedInternalEquationStack = {unresolvedInternalEquationIndex};
        std::vector<size_t> unresolvedInternalEquationIndices;
        std::unordered_set<AnalyserInternalVariable *> unresolvedInternalVariablesSet;
        AnalyserInternalVariablePtrs unresolvedInternalVariables;

        while (!unresolvedInternalEquationStack.empty()) {
            // Get the next unresolved internal equation to visit.

            auto currentUnresolvedEquationIndex = unresolvedInternalEquationStack.back();

            unresolvedInternalEquationStack.pop_back();

            if (visitedUnresolvedInternalEquations[currentUnresolvedEquationIndex]) {
                continue;
            }

            visitedUnresolvedInternalEquations[currentUnresolvedEquationIndex] = true;

            // We are visiting an unresolved internal equation which is part of the coupled system, so we add it to the
            // list of unresolved internal equations and we add its unresolved internal variables to the list of
            // unresolved internal variables, making sure to avoid duplicates, and we add the linked unresolved internal
            // equations to the stack of unresolved internal equations to visit.

            unresolvedInternalEquationIndices.push_back(currentUnresolvedEquationIndex);

            for (const auto &unresolvedInternalVariable : unresolvedInternalEquations[currentUnresolvedEquationIndex].unresolvedInternalVariables) {
                if (unresolvedInternalVariablesSet.insert(unresolvedInternalVariable.get()).second) {
                    unresolvedInternalVariables.push_back(unresolvedInternalVariable);
                }

                for (const auto &internalVariableToInternalEquationIndex : internalVariableToInternalEquationIndices[unresolvedInternalVariable.get()]) {
                    if (!visitedUnresolvedInternalEquations[internalVariableToInternalEquationIndex]) {
                        unresolvedInternalEquationStack.push_back(internalVariableToInternalEquationIndex);
                    }
                }
            }
        }

        // Only a structurally square coupled system can be solved as one global NLA system.

        if (unresolvedInternalEquationIndices.size() != unresolvedInternalVariables.size()) {
            continue;
        }

        // Mark the unknown internal variables as algebraic variables.

        for (const auto &unresolvedInternalVariable : unresolvedInternalVariables) {
            unresolvedInternalVariable->mType = AnalyserInternalVariable::Type::ALGEBRAIC_VARIABLE;
        }

        // Mark the unresolved internal equations in this coupled system as NLA equations and set their unknown internal
        // variables to be the unresolved internal variables in this coupled system.

        std::unordered_set<AnalyserInternalVariable *> unknownInternalVariablesSet;

        for (const auto &unresolvedInternalVariable : unresolvedInternalVariables) {
            unknownInternalVariablesSet.insert(unresolvedInternalVariable.get());
        }

        for (const auto &systemEquationIndex : unresolvedInternalEquationIndices) {
            const auto &unresolvedInternalEquation = unresolvedInternalEquations[systemEquationIndex].internalEquation;

            unresolvedInternalEquation->mType = AnalyserInternalEquation::Type::NLA;
            unresolvedInternalEquation->mUnknownVariables = unresolvedInternalVariables;
            unresolvedInternalEquation->mUnknownVariablesSet = unknownInternalVariablesSet;
        }
    }
}

void Analyser::AnalyserImpl::addInvalidVariableIssue(const AnalyserInternalVariablePtr &variable,
                                                     Issue::ReferenceRule referenceRule)
{
    std::string descriptionStart = "Variable";
    std::string descriptionEnd;

    switch (variable->mType) {
    case AnalyserInternalVariable::Type::UNKNOWN:
        descriptionStart = "The type of variable";
        descriptionEnd = "is unknown";

        break;
    case AnalyserInternalVariable::Type::SHOULD_BE_STATE:
        descriptionEnd = "is used in an ODE, but it is not initialised";

        break;
    case AnalyserInternalVariable::Type::UNDERCONSTRAINED:
        descriptionEnd = "is underconstrained";

        break;
    default: // AnalyserInternalVariable::Type::OVERCONSTRAINED.
        descriptionEnd = "is overconstrained";

        break;
    }

    auto issue = Issue::IssueImpl::create();
    auto realVariable = variable->mVariable;

    issue->mPimpl->setDescription(descriptionStart + " '" + realVariable->name()
                                  + "' in component '" + owningComponent(realVariable)->name()
                                  + "' " + descriptionEnd + ".");
    issue->mPimpl->setReferenceRule(referenceRule);
    issue->mPimpl->mItem->mPimpl->setVariable(realVariable);

    addIssue(issue);
}

SymEngineEquationResult Analyser::AnalyserImpl::astToSymEngine(const AnalyserEquationAstPtr &ast)
{
    // Make sure that we have an AST to convert.

    if (ast == nullptr) {
        return {true, SymEngine::null};
    }

    // Recursively convert the left and right children.

    auto leftAst = ast->leftChild();
    auto rightAst = ast->rightChild();

    auto [leftSuccess, left] = astToSymEngine(leftAst);
    auto [rightSuccess, right] = astToSymEngine(rightAst);

    if (!leftSuccess || !rightSuccess) {
        return {false, SymEngine::null};
    }

    // Check the AST's type and return its SymEngine equivalent.

    auto astType = ast->type();

    switch (astType) {
        // Equality.

    case AnalyserEquationAst::Type::EQUALITY:
        return {true, SymEngine::Eq(left, right)};

        // Relational and logical operators.

    case AnalyserEquationAst::Type::EQ:
        return {true, SymEngine::Eq(left, right)};
    case AnalyserEquationAst::Type::NEQ:
        return {true, SymEngine::Ne(left, right)};
    case AnalyserEquationAst::Type::LT:
        return {true, SymEngine::Lt(left, right)};
    case AnalyserEquationAst::Type::LEQ:
        return {true, SymEngine::Le(left, right)};
    case AnalyserEquationAst::Type::GT:
        return {true, SymEngine::Gt(left, right)};
    case AnalyserEquationAst::Type::GEQ:
        return {true, SymEngine::Ge(left, right)};
    case AnalyserEquationAst::Type::AND:
    case AnalyserEquationAst::Type::OR:
    case AnalyserEquationAst::Type::XOR: {
        if (!SymEngine::is_a_Boolean(*left) || !SymEngine::is_a_Boolean(*right)) {
            return {false, SymEngine::null};
        }

        auto leftBoolean = SymEngine::rcp_dynamic_cast<const SymEngine::Boolean>(left);
        auto rightBoolean = SymEngine::rcp_dynamic_cast<const SymEngine::Boolean>(right);

        if (astType == AnalyserEquationAst::Type::AND) {
            return {true, SymEngine::logical_and(SymEngine::set_boolean({leftBoolean, rightBoolean}))};
        }

        if (astType == AnalyserEquationAst::Type::OR) {
            return {true, SymEngine::logical_or(SymEngine::set_boolean({leftBoolean, rightBoolean}))};
        }

        return {true, SymEngine::logical_xor(SymEngine::vec_boolean({leftBoolean, rightBoolean}))};
    }
    case AnalyserEquationAst::Type::NOT: {
        if (!SymEngine::is_a_Boolean(*left)) {
            return {false, SymEngine::null};
        }

        return {true, SymEngine::logical_not(SymEngine::rcp_dynamic_cast<const SymEngine::Boolean>(left))};
    }

        // Arithmetic operators.

    case AnalyserEquationAst::Type::PLUS:
        if (!right.is_null()) {
            return {true, SymEngine::add(left, right)};
        }

        return {true, left};
    case AnalyserEquationAst::Type::MINUS:
        if (!right.is_null()) {
            return {true, SymEngine::sub(left, right)};
        }

        return {true, SymEngine::mul(SymEngine::number(-1.0), left)};
    case AnalyserEquationAst::Type::TIMES:
        return {true, SymEngine::mul(left, right)};
    case AnalyserEquationAst::Type::DIVIDE:
        return {true, SymEngine::div(left, right)};
    case AnalyserEquationAst::Type::POWER:
        return {true, SymEngine::pow(left, right)};
    case AnalyserEquationAst::Type::ROOT:
        if (!right.is_null()) {
            return {true, SymEngine::pow(right, SymEngine::div(SymEngine::number(1.0), left))};
        }

        return {true, SymEngine::pow(left, SymEngine::div(SymEngine::number(1.0), SymEngine::number(2.0)))};
    case AnalyserEquationAst::Type::ABS:
        return {true, SymEngine::abs(left)};
    case AnalyserEquationAst::Type::EXP:
        return {true, SymEngine::exp(left)};
    case AnalyserEquationAst::Type::LN:
        return {true, SymEngine::log(left)};
    case AnalyserEquationAst::Type::LOG:
        if (!right.is_null()) {
            return {true, SymEngine::div(SymEngine::log(right), SymEngine::log(left))};
        }

        return {true, SymEngine::div(SymEngine::log(left), SymEngine::log(SymEngine::number(10.0)))};
    case AnalyserEquationAst::Type::CEILING:
        return {true, SymEngine::ceiling(left)};
    case AnalyserEquationAst::Type::FLOOR:
        return {true, SymEngine::floor(left)};
    case AnalyserEquationAst::Type::MIN:
        return {true, SymEngine::min({left, right})};
    case AnalyserEquationAst::Type::MAX:
        return {true, SymEngine::max({left, right})};
    case AnalyserEquationAst::Type::REM:
        return {true, SymEngine::function_symbol("mod", {left, right})};

        // Calculus elements.

    case AnalyserEquationAst::Type::DIFF:
        return {true, SymEngine::function_symbol("diff", {left, right})};

        // Trigonometric operators.

    case AnalyserEquationAst::Type::SIN:
        return {true, SymEngine::sin(left)};
    case AnalyserEquationAst::Type::COS:
        return {true, SymEngine::cos(left)};
    case AnalyserEquationAst::Type::TAN:
        return {true, SymEngine::tan(left)};
    case AnalyserEquationAst::Type::SEC:
        return {true, SymEngine::sec(left)};
    case AnalyserEquationAst::Type::CSC:
        return {true, SymEngine::csc(left)};
    case AnalyserEquationAst::Type::COT:
        return {true, SymEngine::cot(left)};
    case AnalyserEquationAst::Type::SINH:
        return {true, SymEngine::sinh(left)};
    case AnalyserEquationAst::Type::COSH:
        return {true, SymEngine::cosh(left)};
    case AnalyserEquationAst::Type::TANH:
        return {true, SymEngine::tanh(left)};
    case AnalyserEquationAst::Type::SECH:
        return {true, SymEngine::sech(left)};
    case AnalyserEquationAst::Type::CSCH:
        return {true, SymEngine::csch(left)};
    case AnalyserEquationAst::Type::COTH:
        return {true, SymEngine::coth(left)};
    case AnalyserEquationAst::Type::ASIN:
        return {true, SymEngine::asin(left)};
    case AnalyserEquationAst::Type::ACOS:
        return {true, SymEngine::acos(left)};
    case AnalyserEquationAst::Type::ATAN:
        return {true, SymEngine::atan(left)};
    case AnalyserEquationAst::Type::ASEC:
        return {true, SymEngine::asec(left)};
    case AnalyserEquationAst::Type::ACSC:
        return {true, SymEngine::acsc(left)};
    case AnalyserEquationAst::Type::ACOT:
        return {true, SymEngine::acot(left)};
    case AnalyserEquationAst::Type::ASINH:
        return {true, SymEngine::asinh(left)};
    case AnalyserEquationAst::Type::ACOSH:
        return {true, SymEngine::acosh(left)};
    case AnalyserEquationAst::Type::ATANH:
        return {true, SymEngine::atanh(left)};
    case AnalyserEquationAst::Type::ASECH:
        return {true, SymEngine::asech(left)};
    case AnalyserEquationAst::Type::ACSCH:
        return {true, SymEngine::acsch(left)};
    case AnalyserEquationAst::Type::ACOTH:
        return {true, SymEngine::acoth(left)};

        // Token elements.

    case AnalyserEquationAst::Type::CI: {
        auto aiVariable = internalVariable(ast->variable());

        if (mAIVariableSEVariableMap.find(aiVariable) == mAIVariableSEVariableMap.end()) {
            // We haven't yet created a SymEngine variable for this internal variable, so create one (making sure that
            // its name is unique) and add it to mAIVariableSEVariableMap and mSEVariableAIVariableMap.

            auto variableName = aiVariable->mVariable->name();
            auto uniqueSeVariable = SymEngine::symbol(variableName);
            auto uniqueVariableNameIndex = MAX_SIZE_T;

            while (mSEVariableAIVariableMap.find(uniqueSeVariable) != mSEVariableAIVariableMap.end()) {
                uniqueSeVariable = SymEngine::symbol(variableName + "_" + convertToString(++uniqueVariableNameIndex));
            }

            auto seVariable = std::move(uniqueSeVariable);

            mAIVariableSEVariableMap[aiVariable] = seVariable;
            mSEVariableAIVariableMap[seVariable] = aiVariable;
        }

        return {true, mAIVariableSEVariableMap.at(aiVariable)};
    }
    case AnalyserEquationAst::Type::CN: {
        // Whole CN values should be converted to integers, so that SymEngine can simplify them properly.

        auto cnValue = std::stod(ast->value());
        auto roundedCnValue = std::llround(cnValue);

        if (areNearlyEqual(cnValue, static_cast<double>(roundedCnValue))) {
            return {true, SymEngine::integer(roundedCnValue)};
        }

        return {true, SymEngine::number(cnValue)};
    }

        // Qualifier elements.

    case AnalyserEquationAst::Type::DEGREE:
    case AnalyserEquationAst::Type::LOGBASE:
    case AnalyserEquationAst::Type::BVAR:
        return {true, left};

        // Constants.

    case AnalyserEquationAst::Type::TRUE:
        return {true, SymEngine::number(1.0)};
    case AnalyserEquationAst::Type::FALSE:
        return {true, SymEngine::number(0.0)};
    case AnalyserEquationAst::Type::E:
        return {true, SymEngine::E};
    case AnalyserEquationAst::Type::PI:
        return {true, SymEngine::pi};
    case AnalyserEquationAst::Type::INF:
        return {true, SymEngine::Inf};
    default:
        // Conversion is not possible with this type.

        return {false, SymEngine::null};
    }
}

AnalyserEquationAstPtr Analyser::AnalyserImpl::simplifyAst(const AnalyserEquationAstPtr &ast)
{
    auto isCnValue = [](const AnalyserEquationAstPtr &ast, double value) {
        if ((ast == nullptr) || (ast->type() != AnalyserEquationAst::Type::CN)) {
            return false;
        }

        return areNearlyEqual(std::stod(ast->value()), value);
    };

    auto makeUnaryMinus = [](const AnalyserEquationAstPtr &child, const AnalyserEquationAstPtr &parent) {
        auto unaryMinus = AnalyserEquationAst::create();

        unaryMinus->setType(AnalyserEquationAst::Type::MINUS);
        unaryMinus->setParent(parent);
        unaryMinus->setLeftChild(child);

        if (child != nullptr) {
            child->setParent(unaryMinus);
        }

        return unaryMinus;
    };

    auto simplify = [&](const auto &self, const AnalyserEquationAstPtr &ast) -> AnalyserEquationAstPtr {
        if (ast == nullptr) {
            return ast;
        }

        auto leftChild = self(self, ast->leftChild());
        auto rightChild = self(self, ast->rightChild());

        if (leftChild != ast->leftChild()) {
            ast->setLeftChild(leftChild);
        }

        if (rightChild != ast->rightChild()) {
            ast->setRightChild(rightChild);
        }

        if (leftChild != nullptr) {
            leftChild->setParent(ast);
        }

        if (rightChild != nullptr) {
            rightChild->setParent(ast);
        }

        auto rewriteLeadingNegatedTerm = [&](const AnalyserEquationAstPtr &lhs,
                                             const AnalyserEquationAstPtr &rhs,
                                             AnalyserEquationAst::Type rootType) {
            if ((lhs == nullptr) || (rhs == nullptr)
                || ((rhs->type() != AnalyserEquationAst::Type::PLUS)
                    && (rhs->type() != AnalyserEquationAst::Type::MINUS))) {
                return false;
            }

            auto rhsLeftChild = rhs->leftChild();
            auto rhsRightChild = rhs->rightChild();

            if ((rhsLeftChild == nullptr) || (rhsRightChild == nullptr)
                || (rhsLeftChild->type() != AnalyserEquationAst::Type::MINUS)
                || (rhsLeftChild->leftChild() == nullptr) || (rhsLeftChild->rightChild() != nullptr)) {
                return false;
            }

            auto b = rhsLeftChild->leftChild();
            auto c = rhsRightChild;
            auto firstTerm = AnalyserEquationAst::create();

            firstTerm->setType(rootType == AnalyserEquationAst::Type::PLUS ?
                                   AnalyserEquationAst::Type::MINUS :
                                   AnalyserEquationAst::Type::PLUS);
            firstTerm->setParent(ast);
            firstTerm->setLeftChild(lhs);
            firstTerm->setRightChild(b);

            lhs->setParent(firstTerm);
            b->setParent(firstTerm);
            c->setParent(ast);

            if (rootType == AnalyserEquationAst::Type::PLUS) {
                ast->setType(rhs->type());
            } else {
                ast->setType(rhs->type() == AnalyserEquationAst::Type::PLUS ?
                                 AnalyserEquationAst::Type::MINUS :
                                 AnalyserEquationAst::Type::PLUS);
            }

            ast->setLeftChild(firstTerm);
            ast->setRightChild(c);

            return true;
        };

        auto makePositiveTimes = [&](const AnalyserEquationAstPtr &term, double negativeValue) {
            auto positiveConstant = AnalyserEquationAst::create();

            positiveConstant->setType(AnalyserEquationAst::Type::CN);
            positiveConstant->setValue(convertToString(std::abs(negativeValue)));

            auto positiveTimes = AnalyserEquationAst::create();

            positiveTimes->setType(AnalyserEquationAst::Type::TIMES);
            positiveTimes->setParent(nullptr);
            positiveTimes->setLeftChild(positiveConstant);
            positiveTimes->setRightChild(term);

            positiveConstant->setParent(positiveTimes);
            term->setParent(positiveTimes);

            return positiveTimes;
        };

        auto positiveTermFromNegatedExpression = [&](const AnalyserEquationAstPtr &term) -> AnalyserEquationAstPtr {
            if (term == nullptr) {
                return nullptr;
            }

            auto lhs = term->leftChild();
            auto rhs = term->rightChild();

            if ((term->type() == AnalyserEquationAst::Type::MINUS)
                && (lhs != nullptr) && (rhs == nullptr)) {
                return lhs;
            }

            if (term->type() == AnalyserEquationAst::Type::TIMES) {
                if ((lhs != nullptr) && (lhs->type() == AnalyserEquationAst::Type::CN)
                    && (rhs != nullptr)) {
                    auto lhsValue = std::stod(lhs->value());

                    if (lhsValue < 0.0) {
                        return makePositiveTimes(rhs, lhsValue);
                    }
                }

                if ((lhs != nullptr)
                    && (rhs != nullptr) && (rhs->type() == AnalyserEquationAst::Type::CN)) {
                    auto rhsValue = std::stod(rhs->value());

                    if (rhsValue < 0.0) {
                        return makePositiveTimes(lhs, rhsValue);
                    }
                }
            }

            return nullptr;
        };

        switch (ast->type()) {
        case AnalyserEquationAst::Type::PLUS: {
            // 0.0 + a -> a

            if (isCnValue(leftChild, 0.0) && (rightChild != nullptr)) {
                rightChild->setParent(ast->parent());

                return rightChild;
            }

            // a + 0.0 -> a

            if ((leftChild != nullptr) && isCnValue(rightChild, 0.0)) {
                leftChild->setParent(ast->parent());

                return leftChild;
            }

            // a + (-b) -> a - b

            if ((rightChild != nullptr)
                && (rightChild->type() == AnalyserEquationAst::Type::MINUS)
                && (rightChild->leftChild() != nullptr)
                && (rightChild->rightChild() == nullptr)) {
                auto minusArgument = rightChild->leftChild();

                minusArgument->setParent(ast);

                ast->setType(AnalyserEquationAst::Type::MINUS);
                ast->setRightChild(minusArgument);

                return self(self, ast);
            }

            if (auto positiveRight = positiveTermFromNegatedExpression(rightChild)) {
                positiveRight->setParent(ast);

                ast->setType(AnalyserEquationAst::Type::MINUS);
                ast->setRightChild(positiveRight);

                return self(self, ast);
            }

            if (rewriteLeadingNegatedTerm(leftChild, rightChild, AnalyserEquationAst::Type::PLUS)) {
                return self(self, ast);
            }

            break;
        }
        case AnalyserEquationAst::Type::MINUS:
            if (rightChild == nullptr) {
                // Unary minus cases.

                auto negateAdditiveSubtree = [&](const auto &selfNegate, const AnalyserEquationAstPtr &subtree,
                                                 const AnalyserEquationAstPtr &parent) -> AnalyserEquationAstPtr {
                    if (subtree == nullptr) {
                        return subtree;
                    }

                    // -(a + b) -> -a - b

                    if (subtree->type() == AnalyserEquationAst::Type::PLUS) {
                        auto negated = AnalyserEquationAst::create();

                        negated->setType(AnalyserEquationAst::Type::PLUS);
                        negated->setParent(parent);

                        auto negatedLeft = selfNegate(selfNegate, subtree->leftChild(), negated);
                        auto negatedRight = selfNegate(selfNegate, subtree->rightChild(), negated);

                        negated->setLeftChild(negatedLeft);
                        negated->setRightChild(negatedRight);

                        if (negatedLeft != nullptr) {
                            negatedLeft->setParent(negated);
                        }

                        if (negatedRight != nullptr) {
                            negatedRight->setParent(negated);
                        }

                        return negated;
                    }

                    // -(a - b) -> -a + b

                    if ((subtree->type() == AnalyserEquationAst::Type::MINUS)
                        && (subtree->rightChild() != nullptr)) {
                        auto negated = AnalyserEquationAst::create();

                        negated->setType(AnalyserEquationAst::Type::PLUS);
                        negated->setParent(parent);

                        auto negatedLeft = selfNegate(selfNegate, subtree->leftChild(), negated);
                        auto right = subtree->rightChild();

                        negated->setLeftChild(negatedLeft);
                        negated->setRightChild(right);

                        if (negatedLeft != nullptr) {
                            negatedLeft->setParent(negated);
                        }

                        if (right != nullptr) {
                            right->setParent(negated);
                        }

                        return negated;
                    }

                    // -(-a) -> a

                    if ((subtree->type() == AnalyserEquationAst::Type::MINUS)
                        && (subtree->rightChild() == nullptr)
                        && (subtree->leftChild() != nullptr)) {
                        auto inner = subtree->leftChild();

                        inner->setParent(parent);

                        return inner;
                    }

                    return makeUnaryMinus(subtree, parent);
                };

                // Push the negation down to the leaves of additive/subtractive trees (i.e. -(a + b - c) -> -a - b + c).

                if ((leftChild != nullptr)
                    && ((leftChild->type() == AnalyserEquationAst::Type::PLUS)
                        || ((leftChild->type() == AnalyserEquationAst::Type::MINUS)
                            && (leftChild->rightChild() != nullptr)))) {
                    return self(self, negateAdditiveSubtree(negateAdditiveSubtree, leftChild, ast->parent()));
                }
            } else {
                // Binary minus cases.

                // 0.0 - a -> -a

                if (isCnValue(leftChild, 0.0) && (rightChild != nullptr)) {
                    return makeUnaryMinus(rightChild, ast->parent());
                }

                // a - 0.0 -> a

                if ((leftChild != nullptr) && isCnValue(rightChild, 0.0)) {
                    leftChild->setParent(ast->parent());

                    return leftChild;
                }

                // a - (-b) -> a + b

                if ((rightChild->type() == AnalyserEquationAst::Type::MINUS)
                    && (rightChild->leftChild() != nullptr)
                    && (rightChild->rightChild() == nullptr)) {
                    auto minusArgument = rightChild->leftChild();

                    minusArgument->setParent(ast);

                    ast->setType(AnalyserEquationAst::Type::PLUS);
                    ast->setRightChild(minusArgument);

                    return self(self, ast);
                }

                if (auto positiveRight = positiveTermFromNegatedExpression(rightChild)) {
                    positiveRight->setParent(ast);

                    ast->setType(AnalyserEquationAst::Type::PLUS);
                    ast->setRightChild(positiveRight);

                    return self(self, ast);
                }

                if (rewriteLeadingNegatedTerm(leftChild, rightChild, AnalyserEquationAst::Type::MINUS)) {
                    return self(self, ast);
                }
            }

            break;
        case AnalyserEquationAst::Type::TIMES:
            // 1.0 * a -> a

            if (isCnValue(leftChild, 1.0) && (rightChild != nullptr)) {
                rightChild->setParent(ast->parent());

                return rightChild;
            }

            // a * 1.0 -> a

            if ((leftChild != nullptr) && isCnValue(rightChild, 1.0)) {
                leftChild->setParent(ast->parent());

                return leftChild;
            }

            // -1.0 * a -> -a

            if (isCnValue(leftChild, -1.0) && (rightChild != nullptr)) {
                return makeUnaryMinus(rightChild, ast->parent());
            }

            // a * -1.0 -> -a

            if ((leftChild != nullptr) && isCnValue(rightChild, -1.0)) {
                return makeUnaryMinus(leftChild, ast->parent());
            }

            // a * 1.0 / b -> a / b

            if ((leftChild != nullptr)
                && (rightChild != nullptr) && (rightChild->type() == AnalyserEquationAst::Type::DIVIDE)
                && isCnValue(rightChild->leftChild(), 1.0)) {
                auto newDivide = AnalyserEquationAst::create();

                newDivide->setType(AnalyserEquationAst::Type::DIVIDE);
                newDivide->setParent(ast->parent());
                newDivide->setLeftChild(leftChild);
                newDivide->setRightChild(rightChild->rightChild());

                leftChild->setParent(newDivide);

                if (rightChild->rightChild() != nullptr) {
                    rightChild->rightChild()->setParent(newDivide);
                }

                return newDivide;
            }

            break;
        case AnalyserEquationAst::Type::DIVIDE:
            // a / 1.0 -> a

            if (isCnValue(rightChild, 1.0) && (leftChild != nullptr)) {
                leftChild->setParent(ast->parent());

                return leftChild;
            }

            // a / -1.0 -> -a

            if (isCnValue(rightChild, -1.0) && (leftChild != nullptr)) {
                return makeUnaryMinus(leftChild, ast->parent());
            }

            break;
        case AnalyserEquationAst::Type::POWER:
            // a^-1.0 -> 1.0 / a

            if (isCnValue(rightChild, -1.0) && (leftChild != nullptr)) {
                auto one = AnalyserEquationAst::create();

                one->setType(AnalyserEquationAst::Type::CN);
                one->setValue("1.0");
                one->setParent(ast);

                auto inverse = AnalyserEquationAst::create();

                inverse->setType(AnalyserEquationAst::Type::DIVIDE);
                inverse->setParent(ast->parent());
                inverse->setLeftChild(one);
                inverse->setRightChild(leftChild);

                if (leftChild != nullptr) {
                    leftChild->setParent(inverse);
                }

                return inverse;
            }

            // e^a -> exp(a)

            if ((leftChild != nullptr) && (leftChild->type() == AnalyserEquationAst::Type::E)
                && (rightChild != nullptr)) {
                auto exp = AnalyserEquationAst::create();

                exp->setType(AnalyserEquationAst::Type::EXP);
                exp->setParent(ast->parent());
                exp->setLeftChild(rightChild);

                if (rightChild != nullptr) {
                    rightChild->setParent(exp);
                }

                return exp;
            }

            break;
        default:
            break;
        }

        return ast;
    };

    return simplify(simplify, ast);
}

AnalyserEquationAstPtr Analyser::AnalyserImpl::symEngineToAst(const SymEngineEquation &symEngineEquation,
                                                              const AnalyserEquationAstPtr &parentAst)
{
    // Our top AST node.

    auto res = AnalyserEquationAst::create();

    res->setParent(parentAst);

    // Check the type of the SymEngine equation and return its AST equivalent, recursively converting its arguments as
    // needed.

    auto currentAst = res;
    auto symEngineEquationArgs = symEngineEquation->get_args();

    switch (symEngineEquation->get_type_code()) {
        // Relational and logical operators.

    case SymEngine::SYMENGINE_EQUALITY:
        // Convert to either AnalyserEquationAst::Type::EQUALITY or AnalyserEquationAst::Type::EQ depending on whether
        // we are at the top level or not.

        currentAst->setType((parentAst == nullptr) ?
                                AnalyserEquationAst::Type::EQUALITY :
                                AnalyserEquationAst::Type::EQ);

        break;
    case SymEngine::SYMENGINE_UNEQUALITY:
        currentAst->setType(AnalyserEquationAst::Type::NEQ);

        break;
    case SymEngine::SYMENGINE_STRICTLESSTHAN:
        currentAst->setType(AnalyserEquationAst::Type::LT);

        break;
    case SymEngine::SYMENGINE_LESSTHAN:
        currentAst->setType(AnalyserEquationAst::Type::LEQ);

        break;

        // Note: AnalyserEquationAst::Type::GT and AnalyserEquationAst::Type::GEQ are not accounted for since SymEngine
        //       doesn't have strict greater than and greater than or equal to operators.

    case SymEngine::SYMENGINE_AND:
        currentAst->setType(AnalyserEquationAst::Type::AND);

        break;
    case SymEngine::SYMENGINE_OR:
        currentAst->setType(AnalyserEquationAst::Type::OR);

        break;
    case SymEngine::SYMENGINE_XOR:
        currentAst->setType(AnalyserEquationAst::Type::XOR);

        break;
    case SymEngine::SYMENGINE_NOT:
        currentAst->setType(AnalyserEquationAst::Type::NOT);

        break;

        // Arithmetic operators.

    case SymEngine::SYMENGINE_ADD:
        currentAst->setType(AnalyserEquationAst::Type::PLUS);

        break;

        // Note: AnalyserEquationAst::Type::MINUS is not accounted for since SymEngine doesn't have a minus operator.

    case SymEngine::SYMENGINE_MUL: {
        currentAst->setType(AnalyserEquationAst::Type::TIMES);

        break;
    }

        // Note: AnalyserEquationAst::Type::DIVIDE is not accounted for since SymEngine doesn't have a divide operator.

    case SymEngine::SYMENGINE_POW:
        currentAst->setType(AnalyserEquationAst::Type::POWER);

        break;

        // Note: AnalyserEquationAst::Type::ROOT is not accounted for since SymEngine doesn't have a root operator.

    case SymEngine::SYMENGINE_ABS:
        currentAst->setType(AnalyserEquationAst::Type::ABS);

        break;

        // Note: AnalyserEquationAst::Type::EXP is not accounted for since SymEngine doesn't have an exponential
        //       operator.

    case SymEngine::SYMENGINE_LOG:
        currentAst->setType(AnalyserEquationAst::Type::LN);

        break;

        // Note: SymEngine doesn't have a log function with a base argument, so although we can easily convert
        //       AnalyserEquationAst::Type::LOG to SymEngine::SYMENGINE_LOG, to do the reverse is a bit more involved
        //       and not worth the effort at the moment, so we just convert SymEngine::SYMENGINE_LOG to
        //       AnalyserEquationAst::Type::LN.

    case SymEngine::SYMENGINE_CEILING:
        currentAst->setType(AnalyserEquationAst::Type::CEILING);

        break;
    case SymEngine::SYMENGINE_FLOOR:
        currentAst->setType(AnalyserEquationAst::Type::FLOOR);

        break;
    case SymEngine::SYMENGINE_MIN:
        currentAst->setType(AnalyserEquationAst::Type::MIN);

        break;
    case SymEngine::SYMENGINE_MAX:
        currentAst->setType(AnalyserEquationAst::Type::MAX);

        break;

        // Note: AnalyserEquationAst::Type::REM is accounted for in the default case since SymEngine doesn't have a
        //       built-in remainder function, so we converted it to a function symbol with the name "mod".

        // Calculus elements.

    case SymEngine::SYMENGINE_DERIVATIVE: {
        currentAst->setType(AnalyserEquationAst::Type::DIFF);

        auto bVarAst = AnalyserEquationAst::create();

        bVarAst->setType(AnalyserEquationAst::Type::BVAR);
        bVarAst->setParent(currentAst);
        bVarAst->setLeftChild(symEngineToAst(symEngineEquationArgs[1], bVarAst));

        currentAst->setLeftChild(bVarAst);
        currentAst->setRightChild(symEngineToAst(symEngineEquationArgs[0], currentAst));

        return res;
    }

        // Trigonometric operators.

    case SymEngine::SYMENGINE_SIN:
        currentAst->setType(AnalyserEquationAst::Type::SIN);

        break;
    case SymEngine::SYMENGINE_COS:
        currentAst->setType(AnalyserEquationAst::Type::COS);

        break;
    case SymEngine::SYMENGINE_TAN:
        currentAst->setType(AnalyserEquationAst::Type::TAN);

        break;
    case SymEngine::SYMENGINE_SEC:
        currentAst->setType(AnalyserEquationAst::Type::SEC);

        break;
    case SymEngine::SYMENGINE_CSC:
        currentAst->setType(AnalyserEquationAst::Type::CSC);

        break;
    case SymEngine::SYMENGINE_COT:
        currentAst->setType(AnalyserEquationAst::Type::COT);

        break;
    case SymEngine::SYMENGINE_SINH:
        currentAst->setType(AnalyserEquationAst::Type::SINH);

        break;
    case SymEngine::SYMENGINE_COSH:
        currentAst->setType(AnalyserEquationAst::Type::COSH);

        break;
    case SymEngine::SYMENGINE_TANH:
        currentAst->setType(AnalyserEquationAst::Type::TANH);

        break;
    case SymEngine::SYMENGINE_SECH:
        currentAst->setType(AnalyserEquationAst::Type::SECH);

        break;
    case SymEngine::SYMENGINE_CSCH:
        currentAst->setType(AnalyserEquationAst::Type::CSCH);

        break;
    case SymEngine::SYMENGINE_COTH:
        currentAst->setType(AnalyserEquationAst::Type::COTH);

        break;
    case SymEngine::SYMENGINE_ASIN:
        currentAst->setType(AnalyserEquationAst::Type::ASIN);

        break;
    case SymEngine::SYMENGINE_ACOS:
        currentAst->setType(AnalyserEquationAst::Type::ACOS);

        break;
    case SymEngine::SYMENGINE_ATAN:
        currentAst->setType(AnalyserEquationAst::Type::ATAN);

        break;
    case SymEngine::SYMENGINE_ASEC:
        currentAst->setType(AnalyserEquationAst::Type::ASEC);

        break;
    case SymEngine::SYMENGINE_ACSC:
        currentAst->setType(AnalyserEquationAst::Type::ACSC);

        break;
    case SymEngine::SYMENGINE_ACOT:
        currentAst->setType(AnalyserEquationAst::Type::ACOT);

        break;
    case SymEngine::SYMENGINE_ASINH:
        currentAst->setType(AnalyserEquationAst::Type::ASINH);

        break;
    case SymEngine::SYMENGINE_ACOSH:
        currentAst->setType(AnalyserEquationAst::Type::ACOSH);

        break;
    case SymEngine::SYMENGINE_ATANH:
        currentAst->setType(AnalyserEquationAst::Type::ATANH);

        break;
    case SymEngine::SYMENGINE_ASECH:
        currentAst->setType(AnalyserEquationAst::Type::ASECH);

        break;
    case SymEngine::SYMENGINE_ACSCH:
        currentAst->setType(AnalyserEquationAst::Type::ACSCH);

        break;
    case SymEngine::SYMENGINE_ACOTH:
        currentAst->setType(AnalyserEquationAst::Type::ACOTH);

        break;

        // Token elements.

    case SymEngine::SYMENGINE_SYMBOL: {
        auto symEngineVariable = SymEngine::rcp_dynamic_cast<const SymEngine::Symbol>(symEngineEquation);

        currentAst->setType(AnalyserEquationAst::Type::CI);
        currentAst->setVariable(mSEVariableAIVariableMap.at(symEngineVariable)->mVariable);

        break;
    }
    case SymEngine::SYMENGINE_INTEGER:
        currentAst->setType(AnalyserEquationAst::Type::CN);
        currentAst->setValue(symEngineEquation->__str__() + ".0");

        break;
    case SymEngine::SYMENGINE_REAL_DOUBLE:
        currentAst->setType(AnalyserEquationAst::Type::CN);
        currentAst->setValue(symEngineEquation->__str__());

        break;
    case SymEngine::SYMENGINE_RATIONAL: {
        auto value = convertToString(SymEngine::eval_double(*symEngineEquation));

        if (value.find('.') == std::string::npos) {
            value += ".0";
        }

        currentAst->setType(AnalyserEquationAst::Type::CN);
        currentAst->setValue(value);

        break;
    }

        // Qualifier elements.

        // Note: we don't need to account for AnalyserEquationAst::Type::DEGREE, AnalyserEquationAst::Type::LOGBASE, and
        //       AnalyserEquationAst::Type::BVAR since we already account for them elsewhere.

        // Constants.

        // Note: we don't need to account for AnalyserEquationAst::Type::TRUE and AnalyserEquationAst::Type::FALSE. We
        //       use SymEngine::number(1.0) and SymEngine::number(0.0) to represent true and false, respectively, so
        //       they are converted to CN nodes rather than TRUE and FALSE nodes.

    case SymEngine::SYMENGINE_CONSTANT:
        // It must be either e or π.

        if (SymEngine::eq(*SymEngine::rcp_dynamic_cast<const SymEngine::Constant>(symEngineEquation), *SymEngine::E)) {
            currentAst->setType(AnalyserEquationAst::Type::E);
        } else {
            currentAst->setType(AnalyserEquationAst::Type::PI);
        }

        break;
    case SymEngine::SYMENGINE_INFTY:
        currentAst->setType(AnalyserEquationAst::Type::INF);

        break;
    default: { // SymEngine::SYMENGINE_FUNCTIONSYMBOL.
        auto functionName = SymEngine::rcp_dynamic_cast<const SymEngine::FunctionSymbol>(symEngineEquation)->get_name();

        if (functionName == "diff") {
            currentAst->setType(AnalyserEquationAst::Type::DIFF);

            auto bVarAst = AnalyserEquationAst::create();

            bVarAst->setType(AnalyserEquationAst::Type::BVAR);
            bVarAst->setParent(currentAst);
            bVarAst->setLeftChild(symEngineToAst(symEngineEquationArgs[0], bVarAst));

            currentAst->setLeftChild(bVarAst);
            currentAst->setRightChild(symEngineToAst(symEngineEquationArgs[1], currentAst));

            return res;
        } else { // "mod".
            currentAst->setType(AnalyserEquationAst::Type::REM);
        }

        break;
    }
    }

    if ((parentAst == nullptr)
        && (currentAst->type() == AnalyserEquationAst::Type::EQUALITY)
        && (symEngineEquationArgs.size() == 2)) {
        auto isVariableLike = [](const SymEngineEquation &expression) {
            return (expression->get_type_code() == SymEngine::SYMENGINE_SYMBOL)
                   || ((expression->get_type_code() == SymEngine::SYMENGINE_FUNCTIONSYMBOL)
                       && (SymEngine::rcp_static_cast<const SymEngine::FunctionSymbol>(expression)->get_name() == "diff"));
        };

        if (!isVariableLike(symEngineEquationArgs[0]) && isVariableLike(symEngineEquationArgs[1])) {
            std::swap(symEngineEquationArgs[0], symEngineEquationArgs[1]);
        }
    }

    // All arguments (except the last one) are guaranteed to be left arguments in the AST tree.

    for (size_t i = 0; i + 1 < symEngineEquationArgs.size(); ++i) {
        currentAst->setLeftChild(symEngineToAst(symEngineEquationArgs[i], currentAst));

        if (i < symEngineEquationArgs.size() - 2) {
            // There are more than two arguments left, so we need to create a copy of our original AST node.

            auto ast = AnalyserEquationAst::create();

            ast->setType(currentAst->type());
            ast->setParent(currentAst);
            ast->setValue(currentAst->value());
            ast->setVariable(currentAst->variable());

            currentAst->setRightChild(ast);

            currentAst = ast;
        }
    }

    // The final argument is created and placed where appropriate.

    if (!symEngineEquationArgs.empty()) {
        AnalyserEquationAstPtr childAst;

        if ((currentAst->type() == AnalyserEquationAst::Type::POWER)
            && (symEngineEquationArgs.size() == 2)
            && (symEngineEquationArgs.back()->get_type_code() == SymEngine::SYMENGINE_RATIONAL)) {
            auto rationalString = symEngineEquationArgs.back()->__str__();
            auto slashPosition = rationalString.find('/');

            if (slashPosition != std::string::npos) {
                auto numeratorAst = AnalyserEquationAst::create();
                auto denominatorAst = AnalyserEquationAst::create();

                numeratorAst->setType(AnalyserEquationAst::Type::CN);
                numeratorAst->setParent(nullptr);
                numeratorAst->setValue(rationalString.substr(0, slashPosition) + ".0");

                denominatorAst->setType(AnalyserEquationAst::Type::CN);
                denominatorAst->setParent(nullptr);
                denominatorAst->setValue(rationalString.substr(slashPosition + 1) + ".0");

                childAst = AnalyserEquationAst::create();
                childAst->setType(AnalyserEquationAst::Type::DIVIDE);
                childAst->setParent(currentAst);
                childAst->setLeftChild(numeratorAst);
                childAst->setRightChild(denominatorAst);

                numeratorAst->setParent(childAst);
                denominatorAst->setParent(childAst);
            }
        }

        if (childAst == nullptr) {
            childAst = symEngineToAst(symEngineEquationArgs.back(), currentAst);
        }

        if (symEngineEquationArgs.size() == 1) {
            currentAst->setLeftChild(childAst);
        } else {
            currentAst->setRightChild(childAst);
        }
    }

    return simplifyAst(res);
}

void Analyser::AnalyserImpl::replaceAstTree(const AnalyserInternalEquationPtr &equation, const AnalyserEquationAstPtr &ast)
{
    equation->mAst = ast;

    equation->mDependencies.clear();

    equation->mVariables.clear();
    equation->mVariablesSet.clear();
    equation->mStateVariables.clear();
    equation->mStateVariablesSet.clear();
    equation->mAllVariables.clear();

    AnalyserEquationAstPtrs astStack;

    astStack.push_back(ast);

    do {
        auto ast = astStack.back();

        astStack.pop_back();

        if (ast->type() == AnalyserEquationAst::Type::CI) {
            auto astVariable = ast->variable();

            if (ast->parent()->type() == AnalyserEquationAst::Type::DIFF) {
                equation->addStateVariable(internalVariable(astVariable));
            } else if (ast->parent()->type() != AnalyserEquationAst::Type::BVAR) {
                equation->addVariable(internalVariable(astVariable));
            }
        }

        if (ast->leftChild() != nullptr) {
            astStack.push_back(ast->leftChild());
        }

        if (ast->rightChild() != nullptr) {
            astStack.push_back(ast->rightChild());
        }
    } while (!astStack.empty());
}

void Analyser::AnalyserImpl::makeVariableKnown(const AnalyserInternalVariablePtr &variable,
                                               const AnalyserInternalEquationPtr &equation)
{
    // Update all the other equations to consider this variable known.

    for (const auto &otherEquation : variable->mUnmatchedEquations) {
        // Add the variable as a dependency for the equation, but only if it isn't the equation we are matching the
        // variable with.

        if (otherEquation == equation) {
            continue;
        }

        otherEquation->mDependencies.push_back(variable);

        // Stop tracking the variable since it is now known.

        otherEquation->removeVariable(variable);
        otherEquation->removeStateVariable(variable);
        otherEquation->removeUnknownVariable(variable);
    }

    // Stop tracking the variable since it is now known.

    if (equation != nullptr) {
        variable->mUnmatchedEquations.clear();
    }
}

bool Analyser::AnalyserImpl::matchVariableAndEquation(const AnalyserInternalVariablePtr &variable,
                                                      const AnalyserInternalEquationPtr &equation)
{
    // If the variable is not isolated in the equation, then we need to rearrange the equation to isolate it before we
    // can match it. If we can't rearrange the equation to isolate the variable, then we can't match them.

    if (!equation->isVariableIsolated(variable)) {
        // We can't rearrange the equation if it doesn't have a SymEngine equation.

        if (equation->mSymEngineEquation.is_null()) {
            return false;
        }

        // Try to rearrange the equation to isolate the variable.

        auto symEngineVariable = mAIVariableSEVariableMap[variable];
        auto rearrangedEquation = equation->rearrangeForSymEngineSymbol(symEngineVariable);

        if (rearrangedEquation.is_null()) {
            return false;
        }

        // Update the equation with the rearranged SymEngine equation and its corresponding AST.

        auto symEngineEquation = SymEngine::Eq(symEngineVariable, rearrangedEquation);

        equation->mSymEngineEquation = symEngineEquation;

        replaceAstTree(equation, symEngineToAst(symEngineEquation));
    }

    // We can now match the variable and the equation.

    equation->addUnknownVariable(variable);

    variable->mMatchedEquation = equation;

    // Update all other variables in the equation to consider this variable known.

    for (const auto &otherVariables : {equation->mStateVariables, equation->mVariables}) {
        for (const auto &otherVariable : otherVariables) {
            // Don't update the variable we are matching.

            if (otherVariable == variable) {
                continue;
            }

            // Stop tracking the variable since it is now known and add it as a dependency for the equation.

            otherVariable->mUnmatchedEquations.erase(std::remove(otherVariable->mUnmatchedEquations.begin(),
                                                                 otherVariable->mUnmatchedEquations.end(),
                                                                 equation),
                                                     otherVariable->mUnmatchedEquations.end());

            equation->mDependencies.push_back(otherVariable);
        }
    }

    // Stop tracking the variable and all other variables in the equation since they are now known.

    equation->mStateVariables.clear();
    equation->mStateVariablesSet.clear();
    equation->mVariables.clear();
    equation->mVariablesSet.clear();

    makeVariableKnown(variable, equation);

    // Stop tracking the variable as a dependency for the equation since it is now known.

    auto dependenccyIt = std::find(equation->mDependencies.begin(), equation->mDependencies.end(), variable);

    if (dependenccyIt != equation->mDependencies.end()) {
        equation->mDependencies.erase(dependenccyIt);
    }

    // Update the variable to use the variable from the equation's component that it is equivalent to, if any.

    for (size_t i = 0; i < equation->mComponent->variableCount(); ++i) {
        auto localVariable = equation->mComponent->variable(i);

        if (mAnalyserModel->areEquivalentVariables(variable->mVariable, localVariable)) {
            variable->setVariable(localVariable, false);

            break;
        }
    }

    return true;
}

void Analyser::AnalyserImpl::matchVariablesAndEquations(AnalyserInternalVariablePtrs &variables,
                                                        AnalyserInternalEquationPtrs &equations,
                                                        bool externalsInitialised)
{
    // Initialise our matching by identifying which variables and equations are initially unknown, and the relationships
    // between them.

    for (const auto &equation : equations) {
        auto iter = equation->mVariables.begin();

        while (iter != equation->mVariables.end()) {
            auto variable = *iter;

            // Ignore the variables that do not require matching and, instead, add them as dependencies.

            if (std::find(variables.begin(), variables.end(), variable) == variables.end()
                || (variable->mType == AnalyserInternalVariable::Type::STATE)
                || (variable->mType == AnalyserInternalVariable::Type::SHOULD_BE_STATE)
                || (externalsInitialised && variable->mIsExternalVariable)) {
                equation->mDependencies.push_back(variable);

                iter = equation->removeVariable(*iter);
            } else {
                // The variable is unknown, so track it as a candidate for matching.

                variable->mUnmatchedEquations.push_back(equation);

                ++iter;
            }
        }

        // State variables are initially unknown, so track them as candidates for matching too.

        for (const auto &stateVariable : equation->mStateVariables) {
            stateVariable->mUnmatchedEquations.push_back(equation);
        }
    }

    // Begin our matching process (see https://doi.org/10.1145/2666202.2666204 for more details).

    auto markEquationsAsNla = [&](const AnalyserInternalEquationPtrs &equations) {
        for (const auto &equation : equations) {
            equation->mType = AnalyserInternalEquation::Type::NLA;

            for (const auto &variable : equation->mAllVariables) {
                if ((variable->mMatchedEquation == nullptr)
                    && (variable->mType != AnalyserInternalVariable::Type::VARIABLE_OF_INTEGRATION)
                    && (variable->mType != AnalyserInternalVariable::Type::INITIALISED)) {
                    variable->mType = AnalyserInternalVariable::Type::ALGEBRAIC_VARIABLE;

                    equation->addUnknownVariable(variable);
                }
            }
        }
    };

    AnalyserInternalVariablePtrs preTearingVariables;
    AnalyserInternalVariablePtrs tearingVariables;
    AnalyserInternalEquationPtrs allEquations = equations;
    auto progressMade = false;

    while (variables.size() > 0) {
        // Match all our unmatched equations with a single unmatched variable it can rearrange for.

        bool localEquationProgress;

        do {
            localEquationProgress = false;

            // Identify the equations that we can currently match.

            auto iter = equations.begin();

            while (iter != equations.end()) {
                auto equation = *iter;

                // We can't match equations that have don't have one unmatched variable.

                if (equation->mVariables.size() + equation->mStateVariables.size() != 1) {
                    ++iter;

                    continue;
                }

                // We can only match equations where the variable is not the variable of integration, not an external
                // variable that we haven't already initialised, and can be rearranged for.

                auto variable = (equation->mVariables.size() == 1) ? equation->mVariables[0] : equation->mStateVariables[0];

                if ((variable->mType == AnalyserInternalVariable::Type::VARIABLE_OF_INTEGRATION)
                    || (variable->mIsExternalVariable && !externalsInitialised && equation->mStateVariables.empty())
                    || !matchVariableAndEquation(variable, equation)) {
                    ++iter;

                    continue;
                }

                // Keep track of the variable as one of our first variables by placing it before any variables that
                // depend on it.

                auto insertIter = std::find_if(mFirstVariables.begin(), mFirstVariables.end(), [&](const auto &otherVariable) {
                    const auto &dependencies = otherVariable->mMatchedEquation->mDependencies;

                    return std::find(dependencies.begin(), dependencies.end(), variable) != dependencies.end();
                });

                mFirstVariables.insert(insertIter, variable);

                // Add the variable to our pre-tearing variables if we haven't identified any tearing variables yet.

                if (tearingVariables.size() == 0) {
                    preTearingVariables.push_back(variable);
                }

                // Stop tracking the variable and equation since they are now matched.

                variables.erase(std::remove(variables.begin(), variables.end(), variable), variables.end());

                iter = equations.erase(iter);

                localEquationProgress = true;
                progressMade = true;
            }
        } while (localEquationProgress);

        // Match all unmatched variables with a single unmatched equation it can be rearranged for.

        bool localVariableProgress;

        do {
            localVariableProgress = false;

            // Identify the variables that we can currently match.

            auto iter = variables.begin();

            while (iter != variables.end()) {
                auto variable = *iter;

                // We can only match variables that are not the variable of integration.

                if (variable->mType == AnalyserInternalVariable::Type::VARIABLE_OF_INTEGRATION) {
                    iter = variables.erase(iter);

                    continue;
                }

                // We can't match variables that have more than one unmatched equation.

                if (variable->mUnmatchedEquations.size() > 1) {
                    ++iter;

                    continue;
                }

                // We can only match variables that are not a variable with no equations left that include it and not an
                // external variable.

                if ((variable->mUnmatchedEquations.size() == 0) || variable->mIsExternalVariable) {
                    iter = variables.erase(iter);

                    continue;
                }

                // Check whether we can match the variable and the equation.

                auto equation = variable->mUnmatchedEquations[0];

                if (matchVariableAndEquation(variable, equation)) {
                    // The variable and the equation can be matched, so keep track of the variable as one of our last
                    // variables and stop tracking the equation.

                    mLastVariables.insert(mLastVariables.begin(), variable);
                    equations.erase(std::remove(equations.begin(), equations.end(), equation), equations.end());

                    progressMade = true;

                } else {
                    // The variable and the equation can't be matched, so it's an "impossible assignment" which means
                    // that the variable should be considered as one of our tearing variables.

                    tearingVariables.push_back(variable);

                    makeVariableKnown(variable, nullptr);
                }

                // Stop tracking the variable since it is now matched or a tearing variable.

                iter = variables.erase(iter);

                localVariableProgress = true;
            }
        } while (localVariableProgress);

        // Pick a tearing variable by choosing the variable that would make the greatest progress towards matching if it
        // were known. We measure this by considering the following statistics:
        //  - the number of equations that would be made matched if this variable was known; and
        //  - the number of unmatched relationships involving the variable.
        // The chosen tearing variable has the greatest sum of these two statistics and has the greatest quantity of the
        // first statistic among the variables with the same sum. If several variables are still tied, we can pick any
        // of them, so we just pick the first one.

        size_t maxSum = 0;
        size_t maxMatchMaking = 0;
        AnalyserInternalVariablePtr tearingVariable;

        for (const auto &variable : variables) {
            // We can't pick an external variable as a tearing variable.

            if (variable->mIsExternalVariable) {
                continue;
            }

            // Calculate the statistics for this variable.

            size_t matchMaking = 0;

            for (const auto &equation : variable->mUnmatchedEquations) {
                if (equation->mStateVariables.size() + equation->mVariables.size() == 2) {
                    ++matchMaking;
                }
            }

            size_t sum = matchMaking + variable->mUnmatchedEquations.size();

            // Check if this variable is a better tearing variable than our current best.

            if (sum > maxSum || ((sum == maxSum) && (matchMaking > maxMatchMaking))) {
                maxSum = sum;
                maxMatchMaking = matchMaking;

                tearingVariable = variable;
            }
        }

        // If we have identified a tearing variable but we have more equations than variables, then we can't actually
        // pick a tearing variable since we won't be able to make progress towards matching by making any variable
        // known.

        if ((tearingVariable != nullptr) && (variables.size() > equations.size())) {
            tearingVariable = nullptr;
        }

        // If we have identified a tearing variable, then track it and consider it as known for the rest of the matching
        // process.

        if (tearingVariable != nullptr) {
            tearingVariables.push_back(tearingVariable);

            variables.erase(std::remove(variables.begin(), variables.end(), tearingVariable), variables.end());

            makeVariableKnown(tearingVariable, nullptr);
        } else {
            // No tearing variable has been identified, so we have stalled and must mark all remaining equations as NLA.
            // But, before doing that, we need to add the tearing variables back to the equations they are still
            // unmatched with since they will be considered unknown for those equations.

            for (const auto &tearingVariable : tearingVariables) {
                for (const auto &equation : tearingVariable->mUnmatchedEquations) {
                    equation->addVariable(tearingVariable);
                    equation->addUnknownVariable(tearingVariable);
                }
            }

            markEquationsAsNla(equations);

            variables.clear();
            equations.clear();
        }
    }

    // No more tearing variables means that we are done with matching.

    if (tearingVariables.size() == 0) {
        return;
    }

    // Reset the unmatched equations of tearing variables as they will be repopulated after equation substitution.

    for (const auto &tearingVariable : tearingVariables) {
        tearingVariable->mUnmatchedEquations.clear();
    }

    // Create a substitution map for SymEngine equations based on the equations we have matched so far.

    SymEngine::map_basic_basic substitutionMap;

    for (const auto &equation : allEquations) {
        // Ignore the equations that we haven't managed to match or that we don't have a SymEngine equivalent for.

        if (std::find(equations.begin(), equations.end(), equation) != equations.end()
            || equation->mSymEngineEquation.is_null()) {
            continue;
        }

        // SymEngine may have swapped the LHS and RHS of our equation, so we need to check both sides of the equation to
        // identify on which side our unknown variable is and thus determine the correct substitution.

        auto symEngineVariableFromSymEngineEquation = [](const SymEngineEquation &symEngineEquation) {
            SymEngineVariable res;

            if (symEngineEquation->get_type_code() == SymEngine::SYMENGINE_SYMBOL) {
                res = SymEngine::rcp_static_cast<const SymEngine::Symbol>(symEngineEquation);
            } else if ((symEngineEquation->get_type_code() == SymEngine::SYMENGINE_FUNCTIONSYMBOL)
                       && (SymEngine::rcp_static_cast<const SymEngine::FunctionSymbol>(symEngineEquation)->get_name() == "diff")) {
                res = SymEngine::rcp_static_cast<const SymEngine::Symbol>(symEngineEquation->get_args()[1]);
            }

            return res;
        };

        auto symEngineEquationArgs = equation->mSymEngineEquation->get_args();
        auto symEngineLhsVariable = symEngineVariableFromSymEngineEquation(symEngineEquationArgs[0]);
        auto symEngineRhsVariable = symEngineVariableFromSymEngineEquation(symEngineEquationArgs[1]);

        auto analyserInternalVariableFromSymEngineVariable = [&](const SymEngineVariable &seVariable) {
            if (seVariable.is_null()) {
                return AnalyserInternalVariablePtr {};
            }

            auto it = mSEVariableAIVariableMap.find(seVariable);

            return (it == mSEVariableAIVariableMap.end()) ? AnalyserInternalVariablePtr {} : it->second;
        };

        // The unknown variable of the equation must be on either the LHS or RHS. We also need to check that the unknown
        // variable is not a pre-tearing variable since those are still considered unknown at this stage.

        auto unknownVariable = equation->mUnknownVariables.empty() ?
                                   analyserInternalVariableFromSymEngineVariable(symEngineLhsVariable) :
                                   equation->mUnknownVariables[0];

        if (unknownVariable == nullptr) {
            unknownVariable = analyserInternalVariableFromSymEngineVariable(symEngineRhsVariable);
        }

        if ((unknownVariable == nullptr)
            || (std::find(preTearingVariables.begin(), preTearingVariables.end(), unknownVariable) != preTearingVariables.end())) {
            continue;
        }

        // We have identified the unknown variable and the correct substitution, so add it to our substitution map.

        if (!symEngineLhsVariable.is_null()
            && (analyserInternalVariableFromSymEngineVariable(symEngineLhsVariable) == unknownVariable)) {
            substitutionMap[symEngineEquationArgs[0]] = symEngineEquationArgs[1];
        } else {
            substitutionMap[symEngineEquationArgs[1]] = symEngineEquationArgs[0];
        }
    }

    // Substitute the SymEngine equations of all our equations and regenerate the AST tree for all our equations.

    for (const auto &equation : equations) {
        // If the equation doesn't have a SymEngine equivalent, we still need to "replace" its AST tree so that its
        // internals get updated.

        if (equation->mSymEngineEquation.is_null()) {
            replaceAstTree(equation, equation->mAst);

            continue;
        }

        // Do the substitution for this equation and then replace its AST tree with the new one generated from the
        // substituted SymEngine equation.

        if (!substitutionMap.empty()) {
            equation->mSymEngineEquation = SymEngine::msubs(equation->mSymEngineEquation, substitutionMap);
        }

        replaceAstTree(equation, symEngineToAst(equation->mSymEngineEquation));
    }

    // We have made some progress towards matching variables and equations, so try again using our tearing variables as
    // unknown variables.

    if (progressMade) {
        auto unknownVariables = tearingVariables;

        matchVariablesAndEquations(unknownVariables, equations, externalsInitialised);

        return;
    }

    // We haven't made any progress towards matching variables and equations, so try again by initialising our external
    // variables if we haven't already.

    if (!externalsInitialised && mExternalVariables.size() > 0) {
        AnalyserInternalVariablePtrs unknownVariables;

        for (const auto &variable : tearingVariables) {
            if (!variable->mIsExternalVariable) {
                unknownVariables.push_back(variable);
            }
        }

        matchVariablesAndEquations(unknownVariables, equations, true);

        return;
    }

    // Our matching algorithm has stalled which means that the rest of the system must be classified as an NLA system.

    markEquationsAsNla(equations);
}

void Analyser::AnalyserImpl::classifyVariablesAndEquations()
{
    // Classify our analyser internal equations and analyser internal variables.

    for (const auto *orderedVariables : {&mFirstVariables, &mLastVariables}) {
        for (const auto &orderedVariable : *orderedVariables) {
            auto matchedEquation = orderedVariable->mMatchedEquation;

            // Ignore variables without a matching equation since they will have been classified as part of an NLA system.

            if (matchedEquation == nullptr) {
                continue;
            }

            // The matched equation of state variables and variables that should be state variables are always
            // classified as ODEs.

            if ((orderedVariable->mType == AnalyserInternalVariable::Type::STATE)
                || (orderedVariable->mType == AnalyserInternalVariable::Type::SHOULD_BE_STATE)) {
                matchedEquation->mType = AnalyserInternalEquation::Type::ODE;

                continue;
            }

            // For other variables, we need to inspect the variables that their matched equation depends on in order to
            // classify them.

            std::unordered_set<AnalyserInternalVariable *> dependentVariables;
            auto onlyConstants = true;
            auto onlyComputedConstants = true;

            auto classifyDependentVariable = [&](const AnalyserInternalVariablePtr &dependentVariable) {
                if (dependentVariable == orderedVariable) {
                    return;
                }

                if (!dependentVariables.insert(dependentVariable.get()).second) {
                    return;
                }

                if (dependentVariable->mIsExternalVariable) {
                    onlyConstants = false;
                    onlyComputedConstants = false;

                    return;
                }

                switch (dependentVariable->mType) {
                case AnalyserInternalVariable::Type::CONSTANT:
                    onlyComputedConstants = false;

                    break;
                case AnalyserInternalVariable::Type::INITIALISED:
                case AnalyserInternalVariable::Type::COMPUTED_TRUE_CONSTANT:
                case AnalyserInternalVariable::Type::COMPUTED_VARIABLE_BASED_CONSTANT:
                    onlyConstants = false;

                    break;
                default:
                    onlyConstants = false;
                    onlyComputedConstants = false;

                    break;
                }
            };

            for (const auto &dependentVariable : matchedEquation->mDependencies) {
                classifyDependentVariable(dependentVariable);
            }

            for (const auto &dependentVariable : matchedEquation->mAllVariables) {
                classifyDependentVariable(dependentVariable);
            }

            // Classify equations and variables.

            if (onlyConstants) {
                orderedVariable->mType = AnalyserInternalVariable::Type::COMPUTED_TRUE_CONSTANT;
                matchedEquation->mType = AnalyserInternalEquation::Type::CONSTANT;
            } else if (onlyComputedConstants) {
                orderedVariable->mType = AnalyserInternalVariable::Type::COMPUTED_VARIABLE_BASED_CONSTANT;
                matchedEquation->mType = AnalyserInternalEquation::Type::COMPUTED_CONSTANT;
            } else {
                orderedVariable->mType = AnalyserInternalVariable::Type::ALGEBRAIC_VARIABLE;
                matchedEquation->mType = AnalyserInternalEquation::Type::ALGEBRAIC;
            }
        }
    }

    // Mark any remaining unknown variables as initialised variables, should they be external variables.

    for (const auto &variable : mInternalVariables) {
        if (variable->mIsExternalVariable && (variable->mType == AnalyserInternalVariable::Type::UNKNOWN)) {
            variable->mType = AnalyserInternalVariable::Type::INITIALISED;
        }
    }

    // Mark any remaining equations with no unknown variables as NLAs.

    for (const auto &equation : mInternalEquations) {
        if (equation->mUnknownVariables.empty() && (equation->mType == AnalyserInternalEquation::Type::UNKNOWN)) {
            equation->mType = AnalyserInternalEquation::Type::NLA;
        }
    }
}

void Analyser::AnalyserImpl::analyseModel(const ModelPtr &model)
{
    // Reset a few things in case this analyser was to be used to analyse more
    // than one model.

    mAnalyserModel = AnalyserModel::AnalyserModelImpl::create(model);

    mInternalVariables.clear();
    mInternalVariableCache.clear();
    mInternalEquations.clear();

    mCiCnUnits.clear();

    // Recursively analyse the model's components, so that we end up with an AST
    // for each of the model's equations.

    for (size_t i = 0; i < model->componentCount(); ++i) {
        analyseComponent(model->component(i));
    }

    // Recursively analyse the model's components' variables.
    // Note #1: we can't do this as part of analyseComponent() since we don't
    //          necessarily know the state of all the variables.
    // Note #2: when it comes to variables initialised using another variable,
    //          we can only do this after all the equations have been analysed,
    //          i.e. once we know the type of all the variables since a variable
    //          can be initialised using another variable, but only if it is not
    //          an algebraic variable.

    for (size_t i = 0; i < model->componentCount(); ++i) {
        analyseComponentVariables(model->component(i));
    }

    if (mAnalyser->errorCount() != 0) {
        mAnalyserModel->mPimpl->mType = AnalyserModel::Type::INVALID;

        return;
    }

    // Mark some variables as external variables, should there be some and
    // should they belong to the model being analysed.

    std::unordered_map<VariablePtr, VariablePtrs> primaryExternalVariables;

    if (!mExternalVariables.empty()) {
        for (const auto &externalVariable : mExternalVariables) {
            auto variable = externalVariable->variable();

            if (owningModel(variable) != model) {
                auto issue = Issue::IssueImpl::create();

                issue->mPimpl->setDescription("Variable '" + variable->name()
                                              + "' in component '" + owningComponent(variable)->name()
                                              + "' is marked as an external variable, but it belongs to a different model and will therefore be ignored.");
                issue->mPimpl->setLevel(Issue::Level::MESSAGE);
                issue->mPimpl->setReferenceRule(Issue::ReferenceRule::ANALYSER_EXTERNAL_VARIABLE_DIFFERENT_MODEL);
                issue->mPimpl->mItem->mPimpl->setVariable(variable);

                addIssue(issue);
            } else {
                auto internalVariable = Analyser::AnalyserImpl::internalVariable(variable);
                auto isVoi = (mAnalyserModel->mPimpl->mVoi != nullptr)
                             && (internalVariable->mVariable == mAnalyserModel->mPimpl->mVoi->variable());

                primaryExternalVariables[internalVariable->mVariable].push_back(variable);

                if (!isVoi && !internalVariable->mIsExternalVariable) {
                    internalVariable->mIsExternalVariable = true;

                    for (const auto &dependency : externalVariable->dependencies()) {
                        internalVariable->mDependencies.push_back(Analyser::AnalyserImpl::internalVariable(dependency));
                    }
                }
            }
        }
    }

    // Analyse our different equations' AST to determine the type of our
    // variables.

    for (const auto &internalEquation : mInternalEquations) {
        analyseEquationAst(internalEquation->mAst);
    }

    if (mAnalyser->errorCount() != 0) {
        mAnalyserModel->mPimpl->mType = AnalyserModel::Type::INVALID;

        return;
    }

    // Check that the variables that were marked as external were rightly so.

    bool hasInvalidVoiExternalVariable = false;

    for (const auto &primaryExternalVariable : primaryExternalVariables) {
        std::string description;
        auto isVoi = (mAnalyserModel->mPimpl->mVoi != nullptr)
                     && (primaryExternalVariable.first == mAnalyserModel->mPimpl->mVoi->variable());
        auto equivalentVariableCount = primaryExternalVariable.second.size();
        auto hasPrimaryVariable = std::find(primaryExternalVariable.second.begin(),
                                            primaryExternalVariable.second.end(),
                                            primaryExternalVariable.first)
                                  != primaryExternalVariable.second.end();

        if (isVoi || (equivalentVariableCount > 1) || !hasPrimaryVariable) {
            description.reserve(256 + 250 * equivalentVariableCount);

            description += (equivalentVariableCount == 2) ? "Both " : "";

            for (size_t i = 0; i < equivalentVariableCount; ++i) {
                if (i != 0) {
                    description += (i != equivalentVariableCount - 1) ? ", " : " and ";
                }

                auto variableString = ((i == 0) && (equivalentVariableCount != 2)) ?
                                          std::string("Variable") :
                                          std::string("variable");

                description += variableString + " '" + primaryExternalVariable.second[i]->name()
                               + "' in component '" + owningComponent(primaryExternalVariable.second[i])->name()
                               + "'";
            }

            Issue::ReferenceRule referenceRule;

            if (isVoi) {
                description += (equivalentVariableCount == 1) ?
                                   " is marked as an external variable, but it is" :
                                   " are marked as external variables, but they are";

                if ((equivalentVariableCount == 1) && hasPrimaryVariable) {
                    description += " the";
                } else {
                    description += " equivalent to variable '" + primaryExternalVariable.first->name()
                                   + "' in component '" + owningComponent(primaryExternalVariable.first)->name()
                                   + "', the primary";
                }

                description += " variable of integration which cannot be used as an external variable.";

                referenceRule = Issue::ReferenceRule::ANALYSER_EXTERNAL_VARIABLE_VOI;
                hasInvalidVoiExternalVariable = true;
            } else {
                description += (equivalentVariableCount == 1) ?
                                   " is marked as an external variable, but it is not a primary variable." :
                                   " are marked as external variables, but they are";
                description += (equivalentVariableCount > 2) ? " all" : "";
                description += (equivalentVariableCount == 1) ? "" : " equivalent.";
                description += " Variable '" + primaryExternalVariable.first->name()
                               + "' in component '" + owningComponent(primaryExternalVariable.first)->name()
                               + "' is";
                description += hasPrimaryVariable ?
                                   " the" :
                               (equivalentVariableCount == 1) ?
                                   " its corresponding" :
                                   " their corresponding";
                description += " primary variable and will therefore be the one used as an external variable.";

                referenceRule = Issue::ReferenceRule::ANALYSER_EXTERNAL_VARIABLE_USE_PRIMARY_VARIABLE;
            }

            auto issue = Issue::IssueImpl::create();

            issue->mPimpl->setDescription(description);
            issue->mPimpl->setLevel(Issue::Level::MESSAGE);
            issue->mPimpl->setReferenceRule(referenceRule);
            issue->mPimpl->mItem->mPimpl->setVariable(primaryExternalVariable.first);

            addIssue(issue);
        }
    }

    if (hasInvalidVoiExternalVariable) {
        mAnalyserModel->mPimpl->mType = AnalyserModel::Type::INVALID;

        return;
    }

    // Analyse our different equations' units to make sure that everything is
    // consistent.

    for (const auto &internalEquation : mInternalEquations) {
        UnitsMaps unitsMaps;
        UnitsMaps userUnitsMaps;
        UnitsMultipliers unitsMultipliers;
        std::string issueDescription;
        PowerData powerData;

        analyseEquationUnits(internalEquation->mAst, unitsMaps, userUnitsMaps, unitsMultipliers,
                             issueDescription, powerData);

        if (!issueDescription.empty()) {
            auto issue = Issue::IssueImpl::create();

            issue->mPimpl->setDescription(issueDescription);
            issue->mPimpl->setLevel(Issue::Level::WARNING);
            issue->mPimpl->setReferenceRule(Issue::ReferenceRule::ANALYSER_UNITS);

            addIssue(issue);
        }
    }

    if (mAnalyser->issueCount() != 0) {
        mAnalyserModel->mPimpl->mType = AnalyserModel::Type::INVALID;

        return;
    }

    // Determine whether some variables have been marked as external.

    auto hasExternalVariables = std::any_of(mInternalVariables.begin(), mInternalVariables.end(), [](const auto &iv) {
        return iv->mIsExternalVariable;
    });

    // Remove any equations that define an external state variable. Indeed, they are not used to compute internal
    // variables since the state variable is supplied externally.

    mInternalEquations.erase(
        std::remove_if(mInternalEquations.begin(), mInternalEquations.end(), [](const auto &equation) {
            return std::any_of(equation->mStateVariables.begin(), equation->mStateVariables.end(), [](const auto &stateVariable) {
                return stateVariable->mIsExternalVariable;
            });
        }),
        mInternalEquations.end());

    // Initialise the variables and the equations that we have to match.
    // Note: the VOI and initialised variables (i.e.  constants and state variables, normally) are not included since
    //       they are expected to be known and therefore don't need to be matched.

    AnalyserInternalVariablePtrs unknownVariables;
    AnalyserInternalEquationPtrs unknownEquations = mInternalEquations;

    std::copy_if(mInternalVariables.begin(), mInternalVariables.end(),
                 std::back_inserter(unknownVariables),
                 [](const auto &variable) {
                     return (variable->mType != AnalyserInternalVariable::Type::VARIABLE_OF_INTEGRATION)
                            && (variable->mType != AnalyserInternalVariable::Type::INITIALISED);
                 });

    // Generate the SymEngine equivalent of our unknown equations.

    for (const auto &unknownEquation : unknownEquations) {
        auto [result, symEngineEquation] = astToSymEngine(unknownEquation->mAst);

        if (result) {
            unknownEquation->mSymEngineEquation = symEngineEquation;
        }
    }

    // Start matching our unknown variables and equations, and classify them.

    matchVariablesAndEquations(unknownVariables, unknownEquations, false);

    classifyVariablesAndEquations();

    // At this stage, there may be some unknown variables that part of a coupled system of equations that we couldn't
    // resolve in isolation, so we need to try to resolve them using some global NLA systems.

    resolveUnknownVariablesUsingGlobalNlaSystems();

    // TODO: ---GRY--- If we still need to call resolveUnknownVariablesUsingGlobalNlaSystems() above then we should revisit any still-unknown equations now that the global NLA systems have made some algebraic variables known.

    // Make sure that our variables are valid.

    for (const auto &internalVariable : mInternalVariables) {
        if (internalVariable->mType == AnalyserInternalVariable::Type::INITIALISED) {
            // The variable is (still) initialised so it has to be a constant.

            internalVariable->makeConstant();
        }
    }

    // Make sure that variables that are initialised using another variable are
    // not initialised using an algebraic variable.

    const auto origErrorCount = mAnalyser->errorCount();

    for (const auto &internalVariable : mInternalVariables) {
        if ((internalVariable->mInitialisingVariable != nullptr)
            && !isCellMLReal(internalVariable->mInitialisingVariable->initialValue())) {
            auto initialisingInternalVariable = Analyser::AnalyserImpl::internalVariable(owningComponent(internalVariable->mInitialisingVariable)->variable(internalVariable->mInitialisingVariable->initialValue()));

            if (initialisingInternalVariable->mType == AnalyserInternalVariable::Type::ALGEBRAIC_VARIABLE) {
                auto issue = Issue::IssueImpl::create();

                issue->mPimpl->setDescription("Variable '" + internalVariable->mVariable->name()
                                              + "' in component '" + owningComponent(internalVariable->mVariable)->name()
                                              + "' is initialised using variable '" + initialisingInternalVariable->mVariable->name()
                                              + "', which is an algebraic variable. Only a reference to a constant, a computed constant, a state variable, or a computable non-linear algebraic variable is allowed.");
                issue->mPimpl->setReferenceRule(Issue::ReferenceRule::ANALYSER_VARIABLE_INITIALISED_USING_ALGEBRAIC_VARIABLE);
                issue->mPimpl->mItem->mPimpl->setVariable(internalVariable->mVariable);

                addIssue(issue);
            }
        }
    }

    if (mAnalyser->errorCount() != origErrorCount) {
        mAnalyserModel->mPimpl->mType = AnalyserModel::Type::INVALID;

        return;
    }

    // Make sure that our equations are valid.

    AnalyserInternalVariablePtrs addedExternalVariables;
    std::unordered_set<AnalyserInternalVariable *> addedExternalVariablesSet;
    AnalyserInternalEquationPtrs addedInternalEquations;
    AnalyserInternalEquationPtrs removedInternalEquations;
    auto nlaSystemIndex = MAX_SIZE_T;

    for (const auto &internalEquation : mInternalEquations) {
        // Account for the unknown variables, in an NLA equation, that have been
        // marked as an external variable. This means removing them from
        // mUnknownVariables and adding a new equation for them so that the NLA
        // equation can have a dependency on them.

        if (internalEquation->mType == AnalyserInternalEquation::Type::NLA) {
            AnalyserInternalVariablePtrs externalUnknownVariables;

            for (const auto &unknownVariable : internalEquation->mUnknownVariables) {
                if (unknownVariable->mIsExternalVariable
                    && addedExternalVariablesSet.insert(unknownVariable.get()).second) {
                    addedExternalVariables.push_back(unknownVariable);
                    addedInternalEquations.push_back(AnalyserInternalEquation::create(unknownVariable));
                }

                if (unknownVariable->mIsExternalVariable) {
                    externalUnknownVariables.push_back(unknownVariable);
                }
            }

            for (const auto &unknownVariable : externalUnknownVariables) {
                internalEquation->removeUnknownVariable(unknownVariable);
            }
        }

        // Discard the equation if we have no unknown variables left and it does not contain any remaining dependencies.
        // Otherwise, keep it as a constraint on known variables.

        if (internalEquation->mUnknownVariables.empty()
            && internalEquation->mDependencies.empty()) {
            removedInternalEquations.push_back(internalEquation);
        }
    }

    for (const auto &internalEquation : mInternalEquations) {
        if (internalEquation->mType != AnalyserInternalEquation::Type::NLA) {
            continue;
        }

        if (internalEquation->mNlaSystemIndex == MAX_SIZE_T) {
            internalEquation->mNlaSystemIndex = ++nlaSystemIndex;
        }

        std::unordered_set<AnalyserInternalVariable *> internalEquationVariablesSet;

        internalEquationVariablesSet.reserve(internalEquation->mUnknownVariables.size() + internalEquation->mDependencies.size());

        for (const auto &unknownVariable : internalEquation->mUnknownVariables) {
            if (!unknownVariable->mIsExternalVariable
                && (unknownVariable->mType != AnalyserInternalVariable::Type::CONSTANT)
                && (unknownVariable->mType != AnalyserInternalVariable::Type::COMPUTED_TRUE_CONSTANT)
                && (unknownVariable->mType != AnalyserInternalVariable::Type::COMPUTED_VARIABLE_BASED_CONSTANT)) {
                internalEquationVariablesSet.insert(unknownVariable.get());
            }
        }
        for (const auto &dependency : internalEquation->mDependencies) {
            if (!dependency->mIsExternalVariable
                && (dependency->mType != AnalyserInternalVariable::Type::CONSTANT)
                && (dependency->mType != AnalyserInternalVariable::Type::COMPUTED_TRUE_CONSTANT)
                && (dependency->mType != AnalyserInternalVariable::Type::COMPUTED_VARIABLE_BASED_CONSTANT)) {
                internalEquationVariablesSet.insert(dependency.get());
            }
        }

        for (const auto &otherInternalEquation : mInternalEquations) {
            if ((otherInternalEquation != internalEquation)
                && (otherInternalEquation->mType == AnalyserInternalEquation::Type::NLA)) {
                // Check what common variables there are between internalEquation and otherInternalEquation, if any.
                // Note: we would normally use std::set_intersection() for this, but this would require both sets to be
                //       sorted. Since they are not, we use a hash set instead.

                bool hasCommonVariables = false;

                for (const auto &unknownVariable : otherInternalEquation->mUnknownVariables) {
                    if (!unknownVariable->mIsExternalVariable
                        && (unknownVariable->mType != AnalyserInternalVariable::Type::CONSTANT)
                        && (unknownVariable->mType != AnalyserInternalVariable::Type::COMPUTED_TRUE_CONSTANT)
                        && (unknownVariable->mType != AnalyserInternalVariable::Type::COMPUTED_VARIABLE_BASED_CONSTANT)
                        && (internalEquationVariablesSet.count(unknownVariable.get()) != 0)) {
                        hasCommonVariables = true;

                        break;
                    }
                }

                if (!hasCommonVariables) {
                    for (const auto &dependency : otherInternalEquation->mDependencies) {
                        if (!dependency->mIsExternalVariable
                            && (dependency->mType != AnalyserInternalVariable::Type::CONSTANT)
                            && (dependency->mType != AnalyserInternalVariable::Type::COMPUTED_TRUE_CONSTANT)
                            && (dependency->mType != AnalyserInternalVariable::Type::COMPUTED_VARIABLE_BASED_CONSTANT)
                            && (internalEquationVariablesSet.count(dependency.get()) != 0)) {
                            hasCommonVariables = true;

                            break;
                        }
                    }
                }

                // Consider otherInternalEquation as an NLA sibling of internalEquation if there are some common
                // variables, and make sure that it has the same NLA system index as internalEquation.

                if (hasCommonVariables) {
                    internalEquation->mNlaSiblings.push_back(otherInternalEquation);

                    otherInternalEquation->mNlaSystemIndex = internalEquation->mNlaSystemIndex;
                }
            }
        }
    }

    // Remove any internal equations that are no longer needed.

    if (!removedInternalEquations.empty()) {
        std::unordered_set<AnalyserInternalEquation *> removedInternalEquationsSet;

        removedInternalEquationsSet.reserve(removedInternalEquations.size());

        for (const auto &removedInternalEquation : removedInternalEquations) {
            removedInternalEquationsSet.insert(removedInternalEquation.get());
        }

        mInternalEquations.erase(
            std::remove_if(mInternalEquations.begin(), mInternalEquations.end(),
                           [&removedInternalEquationsSet](const auto &removedInternalEquation) {
                               return removedInternalEquationsSet.count(removedInternalEquation.get()) != 0;
                           }),
            mInternalEquations.end());
    }

    if (!addedInternalEquations.empty()) {
        mInternalEquations.insert(mInternalEquations.end(), addedInternalEquations.begin(), addedInternalEquations.end());
    }

    // Build the companion sets of unknown variables for all the NLA equations before linking the equations together.

    for (const auto &internalEquation : mInternalEquations) {
        if (internalEquation->mType != AnalyserInternalEquation::Type::NLA) {
            continue;
        }

        internalEquation->mUnknownVariablesSet.clear();

        for (const auto &unknownVariable : internalEquation->mUnknownVariables) {
            internalEquation->mUnknownVariablesSet.insert(unknownVariable.get());
        }
    }

    // Confirm that the variables in an NLA system are not overconstrained.
    // Note: this may happen if an NLA system contains too many NLA equations to compute its unknown variables and/or if
    //       some internal equations were removed (as a result of some variables in an NLA equation having been marked
    //       as external).

    std::unordered_set<AnalyserInternalVariable *> underconstrainedVariablesSet;
    std::unordered_set<AnalyserInternalVariable *> overconstrainedVariablesSet;

    for (const auto &internalEquation : mInternalEquations) {
        switch (internalEquation->mType) {
        case AnalyserInternalEquation::Type::COMPUTED_CONSTANT: {
            auto unknownVariable = internalEquation->mUnknownVariables[0];

            for (const auto &variable : internalEquation->mAllVariables) {
                if ((variable != unknownVariable)
                    && (variable->mType != AnalyserInternalVariable::Type::CONSTANT)
                    && (variable->mType != AnalyserInternalVariable::Type::COMPUTED_TRUE_CONSTANT)
                    && (variable->mType != AnalyserInternalVariable::Type::COMPUTED_VARIABLE_BASED_CONSTANT)) {
                    // We are supposed to compute a variable-based constant, yet we have come across a variable which is
                    // not some kind of a constant. In fact, it was an algebraic variable (with an initial guess) that
                    // needs to be computed using an NLA system. So, requalify the unknown variable and equation.

                    unknownVariable->mType = AnalyserInternalVariable::Type::ALGEBRAIC_VARIABLE;
                    internalEquation->mType = AnalyserInternalEquation::Type::ALGEBRAIC;

                    break;
                }
            }
        } break;
        case AnalyserInternalEquation::Type::NLA: {
            auto markVariableAsUnderconstrained = [&](const AnalyserInternalVariablePtr &variable) {
                if ((variable->mType == AnalyserInternalVariable::Type::INITIALISED)
                    || (variable->mType == AnalyserInternalVariable::Type::STATE)
                    || (variable->mType == AnalyserInternalVariable::Type::CONSTANT)
                    || variable->mIsExternalVariable) {
                    return;
                }

                if (underconstrainedVariablesSet.insert(variable.get()).second) {
                    variable->mType = AnalyserInternalVariable::Type::UNDERCONSTRAINED;
                }
            };

            auto nlaSiblingCount = internalEquation->mNlaSiblings.size() + 1; // +1 to account for internalEquation itself.

            if (nlaSiblingCount < internalEquation->mUnknownVariables.size()) {
                // There are fewer NLA equations than unknown variables, so all the variables involved in the NLA system
                // should be considered as underconstrained.

                for (const auto &variable : internalEquation->mAllVariables) {
                    markVariableAsUnderconstrained(variable);
                }
            } else if (nlaSiblingCount > internalEquation->mUnknownVariables.size()) {
                // There are more NLA equations than unknown variables, so the NLA system should be considered as
                // overconstrained.
                // Note: this can still happen for equations that have no unknown variables left because the variables
                //       they depend on are already known.

                auto markVariableAsOverconstrained = [&](const AnalyserInternalVariablePtr &variable) {
                    if ((variable->mType == AnalyserInternalVariable::Type::INITIALISED)
                        || (variable->mType == AnalyserInternalVariable::Type::STATE)
                        || (variable->mType == AnalyserInternalVariable::Type::CONSTANT)
                        || variable->mIsExternalVariable) {
                        return;
                    }

                    if (overconstrainedVariablesSet.insert(variable.get()).second) {
                        variable->mType = AnalyserInternalVariable::Type::OVERCONSTRAINED;
                    }
                };

                if (internalEquation->mUnknownVariables.empty()) {
                    size_t variableCount = 0;

                    for (const auto &dependency : internalEquation->mDependencies) {
                        if (dependency->mIsExternalVariable
                            || (dependency->mType == AnalyserInternalVariable::Type::CONSTANT)
                            || (dependency->mType == AnalyserInternalVariable::Type::COMPUTED_TRUE_CONSTANT)
                            || (dependency->mType == AnalyserInternalVariable::Type::COMPUTED_VARIABLE_BASED_CONSTANT)) {
                            continue;
                        }

                        ++variableCount;
                    }

                    if (nlaSiblingCount < variableCount) {
                        for (const auto &dependency : internalEquation->mDependencies) {
                            markVariableAsUnderconstrained(dependency);
                        }
                    } else if (nlaSiblingCount > variableCount) {
                        for (const auto &dependency : internalEquation->mDependencies) {
                            markVariableAsOverconstrained(dependency);
                        }
                    }
                } else {
                    for (const auto &unknownVariable : internalEquation->mUnknownVariables) {
                        markVariableAsOverconstrained(unknownVariable);
                    }

                    for (const auto &dependency : internalEquation->mDependencies) {
                        if (dependency->mIsExternalVariable
                            || (dependency->mType == AnalyserInternalVariable::Type::CONSTANT)
                            || (dependency->mType == AnalyserInternalVariable::Type::COMPUTED_TRUE_CONSTANT)
                            || (dependency->mType == AnalyserInternalVariable::Type::COMPUTED_VARIABLE_BASED_CONSTANT)) {
                            continue;
                        }

                        markVariableAsOverconstrained(dependency);
                    }
                }
            }
        }

        break;
        default:
            // Other types we don't care about.

            break;
        }
    }

    AnalyserInternalVariablePtrs pendingInvalidVariables;

    for (const auto &internalVariable : mInternalVariables) {
        switch (internalVariable->mType) {
        case AnalyserInternalVariable::Type::UNKNOWN:
        case AnalyserInternalVariable::Type::SHOULD_BE_STATE:
        case AnalyserInternalVariable::Type::UNDERCONSTRAINED:
        case AnalyserInternalVariable::Type::OVERCONSTRAINED:
            pendingInvalidVariables.push_back(internalVariable);

            break;
        default:
            // Other types we don't care about.

            break;
        }
    }

    for (const auto &pendingInvalidVariable : pendingInvalidVariables) {
        switch (pendingInvalidVariable->mType) {
        case AnalyserInternalVariable::Type::UNKNOWN:
            addInvalidVariableIssue(pendingInvalidVariable,
                                    Issue::ReferenceRule::ANALYSER_VARIABLE_UNUSED);

            break;
        case AnalyserInternalVariable::Type::SHOULD_BE_STATE:
            addInvalidVariableIssue(pendingInvalidVariable,
                                    Issue::ReferenceRule::ANALYSER_STATE_NOT_INITIALISED);

            break;
        case AnalyserInternalVariable::Type::UNDERCONSTRAINED:
            addInvalidVariableIssue(pendingInvalidVariable,
                                    Issue::ReferenceRule::ANALYSER_VARIABLE_UNDERCONSTRAINED);

            break;
        case AnalyserInternalVariable::Type::OVERCONSTRAINED:
            addInvalidVariableIssue(pendingInvalidVariable,
                                    Issue::ReferenceRule::ANALYSER_VARIABLE_OVERCONSTRAINED);

            break;
        default:
            // Any other type has already been handled elsewhere.

            break;
        }
    }

    if (mAnalyser->errorCount() != 0) {
        auto hasUnderconstrainedVariables = std::any_of(mInternalVariables.begin(), mInternalVariables.end(), [](const auto &iv) {
            switch (iv->mType) {
            case AnalyserInternalVariable::Type::UNKNOWN:
            case AnalyserInternalVariable::Type::SHOULD_BE_STATE:
            case AnalyserInternalVariable::Type::UNDERCONSTRAINED:
                return true;
            default:
                return false;
            }
        });
        auto hasOverconstrainedVariables = std::any_of(mInternalVariables.begin(), mInternalVariables.end(), [](const auto &iv) {
            return iv->mType == AnalyserInternalVariable::Type::OVERCONSTRAINED;
        });

        if (hasUnderconstrainedVariables) {
            if (hasOverconstrainedVariables) {
                mAnalyserModel->mPimpl->mType = AnalyserModel::Type::UNSUITABLY_CONSTRAINED;
            } else {
                mAnalyserModel->mPimpl->mType = AnalyserModel::Type::UNDERCONSTRAINED;
            }
        } else {
            mAnalyserModel->mPimpl->mType = AnalyserModel::Type::OVERCONSTRAINED;
        }

        return;
    }

    // Determine the type of our model.

    auto hasNlaEquations = std::any_of(mInternalEquations.begin(), mInternalEquations.end(), [&](const auto &ie) {
        if (ie->mType == AnalyserInternalEquation::Type::NLA) {
            // Make sure that not all the variables involved in the NLA system have been marked as external.

            return std::any_of(ie->mUnknownVariables.begin(), ie->mUnknownVariables.end(), [](const auto &uv) {
                return !uv->mIsExternalVariable;
            });
        }

        return false;
    });

    if (mAnalyserModel->mPimpl->mVoi != nullptr) {
        mAnalyserModel->mPimpl->mType = hasNlaEquations ?
                                            AnalyserModel::Type::DAE :
                                            AnalyserModel::Type::ODE;
    } else if (!mInternalVariables.empty()) {
        mAnalyserModel->mPimpl->mType = hasNlaEquations ?
                                            AnalyserModel::Type::NLA :
                                            AnalyserModel::Type::ALGEBRAIC;
    }

    if (!mAnalyserModel->isValid()) {
        return;
    }

    // Add a dummy equation for each of our true constants.
    // Note: this is so that a constant can be marked as an external variable.

    for (const auto &internalVariable : mInternalVariables) {
        if (internalVariable->mType == AnalyserInternalVariable::Type::CONSTANT) {
            mInternalEquations.push_back(AnalyserInternalEquation::create(internalVariable));
        }
    }

    // Make it known through our API whether the model has some external variables.

    mAnalyserModel->mPimpl->mHasExternalVariables = hasExternalVariables;

    // Create a mapping between our internal equations and our future equations in the API.

    std::unordered_map<AnalyserInternalEquationPtr, AnalyserEquationPtr> aie2aeMappings;

    for (const auto &internalEquation : mInternalEquations) {
        aie2aeMappings.emplace(internalEquation, AnalyserEquation::AnalyserEquationImpl::create());
    }

    // Start making our internal equations available through our API.
    // Note: start because we need to determine the type of our equations before we can make our internal variables
    //       available through our API.

    std::unordered_map<AnalyserInternalEquationPtr, AnalyserEquation::Type> aie2aetMappings;

    for (const auto &internalEquation : mInternalEquations) {
        // Determine whether the equation is an external one.

        auto externalEquation = true;

        for (const auto &unknownVariable : internalEquation->mUnknownVariables) {
            if (!unknownVariable->mIsExternalVariable) {
                externalEquation = false;

                break;
            }
        }

        // Determine and keep track of the type of the equation.

        AnalyserEquation::Type type;

        if (externalEquation) {
            type = AnalyserEquation::Type::EXTERNAL;
        } else {
            switch (internalEquation->mType) {
            case AnalyserInternalEquation::Type::CONSTANT:
                type = AnalyserEquation::Type::CONSTANT;

                break;
            case AnalyserInternalEquation::Type::COMPUTED_CONSTANT:
                type = AnalyserEquation::Type::COMPUTED_CONSTANT;

                break;
            case AnalyserInternalEquation::Type::ODE:
                type = AnalyserEquation::Type::ODE;

                break;
            case AnalyserInternalEquation::Type::NLA:
                type = AnalyserEquation::Type::NLA;

                break;
            case AnalyserInternalEquation::Type::ALGEBRAIC:
                type = AnalyserEquation::Type::ALGEBRAIC;

                break;
            default: // AnalyserEquation::Type::UNKNOWN.
                continue;
            }
        }

        aie2aetMappings.emplace(internalEquation, type);
    }

    // Make our internal variables available through our API.

    std::unordered_map<AnalyserInternalVariablePtr, AnalyserVariablePtr> aiv2avMappings;
    std::unordered_map<VariablePtr, AnalyserVariablePtr> v2avMappings;
    auto stateIndex = MAX_SIZE_T;
    auto constantIndex = MAX_SIZE_T;
    auto computedConstantIndex = MAX_SIZE_T;
    auto algebraicVariableIndex = MAX_SIZE_T;
    auto externalVariableIndex = MAX_SIZE_T;

    for (const auto &internalVariable : mInternalVariables) {
        // Determine the type of the variable.

        AnalyserVariable::Type variableType;

        if (internalVariable->mIsExternalVariable) {
            variableType = AnalyserVariable::Type::EXTERNAL_VARIABLE;
        } else {
            switch (internalVariable->mType) {
            case AnalyserInternalVariable::Type::STATE:
                variableType = AnalyserVariable::Type::STATE;

                break;
            case AnalyserInternalVariable::Type::CONSTANT:
                variableType = AnalyserVariable::Type::CONSTANT;

                break;
            case AnalyserInternalVariable::Type::COMPUTED_TRUE_CONSTANT:
            case AnalyserInternalVariable::Type::COMPUTED_VARIABLE_BASED_CONSTANT:
                variableType = AnalyserVariable::Type::COMPUTED_CONSTANT;

                break;
            case AnalyserInternalVariable::Type::ALGEBRAIC_VARIABLE:
                variableType = AnalyserVariable::Type::ALGEBRAIC_VARIABLE;

                break;
            default: // AnalyserInternalVariable::Type::VARIABLE_OF_INTEGRATION.
                // This is the variable of integration, so skip it.

                continue;
            }
        }

        // Retrieve the equations used to compute the variable.

        AnalyserEquationPtrs equations;
        // TODO: ---GRY--- To be uncommented if we need to correct the type of a computed constant that is computed using an NLA equation.
        /*
        auto isNlaEquation = false;
        */

        for (const auto &internalEquation : mInternalEquations) {
            if (internalEquation->mUnknownVariablesSet.count(internalVariable.get()) != 0) {
                equations.push_back(aie2aeMappings[internalEquation]);

                // TODO: ---GRY--- To be uncommented if we need to correct the type of a computed constant that is computed using an NLA equation.
                /*
                auto aetIt = aie2aetMappings.find(internalEquation);

                if ((aetIt != aie2aetMappings.end()) && (aetIt->second == AnalyserEquation::Type::NLA)) {
                    isNlaEquation = true;
                }
                */
            }
        }

        // Correct the type of the variable if it is a computed constant that is computed using an NLA equation.

        // TODO: ---GRY--- Confirm whether this is still needed (it's not covered by any test case... anymore?).
        /*
        if ((variableType == AnalyserVariable::Type::COMPUTED_CONSTANT) && isNlaEquation) {
            variableType = AnalyserVariable::Type::ALGEBRAIC_VARIABLE;
        }
        */

        // Populate and keep track of the state/variable.

        auto variable = AnalyserVariable::AnalyserVariableImpl::create();

        variable->mPimpl->populate(variableType,
                                   (variableType == AnalyserVariable::Type::STATE) ?
                                       ++stateIndex :
                                   (variableType == AnalyserVariable::Type::CONSTANT) ?
                                       ++constantIndex :
                                   (variableType == AnalyserVariable::Type::COMPUTED_CONSTANT) ?
                                       ++computedConstantIndex :
                                   (variableType == AnalyserVariable::Type::ALGEBRAIC_VARIABLE) ?
                                       ++algebraicVariableIndex :
                                       ++externalVariableIndex,
                                   (variableType == AnalyserVariable::Type::EXTERNAL_VARIABLE) ?
                                       nullptr :
                                       internalVariable->mInitialisingVariable,
                                   internalVariable->mVariable, mAnalyserModel, equations);

        aiv2avMappings.emplace(internalVariable, variable);
        v2avMappings.emplace(internalVariable->mVariable, variable);

        if (variableType == AnalyserVariable::Type::STATE) {
            mAnalyserModel->mPimpl->mStates.push_back(variable);
        } else if (variableType == AnalyserVariable::Type::CONSTANT) {
            mAnalyserModel->mPimpl->mConstants.push_back(variable);
        } else if (variableType == AnalyserVariable::Type::COMPUTED_CONSTANT) {
            mAnalyserModel->mPimpl->mComputedConstants.push_back(variable);
        } else if (variableType == AnalyserVariable::Type::ALGEBRAIC_VARIABLE) {
            mAnalyserModel->mPimpl->mAlgebraicVariables.push_back(variable);
        } else { // AnalyserVariable::Type::EXTERNAL_VARIABLE.
            mAnalyserModel->mPimpl->mExternalVariables.push_back(variable);
        }
    }

    // Finish making our internal equations available through our API.

    for (const auto &internalEquation : mInternalEquations) {
        // Make sure that the type of the equation is known.

        auto aetIt = aie2aetMappings.find(internalEquation);

        if (aetIt == aie2aetMappings.end()) {
            continue;
        }

        // Scale our internal equation's AST to take into account the fact that
        // we may have mapped variables that use compatible units rather than
        // equivalent ones.

        scaleEquationAst(internalEquation->mAst);

        // Manipulate the equation, if needed.

        auto equationType = aetIt->second;

        if (equationType == AnalyserEquation::Type::NLA) {
            // The equation is currently of the form LHS = RHS, but we want it in the form LHS-RHS, so replace the
            // equality element with a minus one.

            internalEquation->mAst->setType(AnalyserEquationAst::Type::MINUS);
        }

        // Determine the equation's dependencies, i.e. the equations for the
        // variables on which this equation depends.

        VariablePtrs variableDependencies;

        if (equationType == AnalyserEquation::Type::EXTERNAL) {
            for (const auto &unknownVariable : internalEquation->mUnknownVariables) {
                for (const auto &dependency : unknownVariable->mDependencies) {
                    variableDependencies.push_back(dependency->mVariable);
                }
            }
        } else {
            variableDependencies.reserve(internalEquation->mDependencies.size());

            for (const auto &dependency : internalEquation->mDependencies) {
                variableDependencies.push_back(dependency->mVariable);
            }

            if (equationType == AnalyserEquation::Type::NLA) {
                for (const auto &nlaSibling : internalEquation->mNlaSiblings) {
                    for (const auto &dependency : nlaSibling.lock()->mDependencies) {
                        variableDependencies.push_back(dependency->mVariable);
                    }
                }
            }
        }

        AnalyserEquationPtrs equationDependencies;
        std::unordered_set<AnalyserEquation *> seenAnalyserEquations;
        std::unordered_set<size_t> seenNlaSystemIndices;

        for (const auto &variableDependency : variableDependencies) {
            auto analyserVariable = v2avMappings[variableDependency];

            if (analyserVariable != nullptr) {
                for (const auto &analyserEquation : analyserVariable->analyserEquations()) {
                    if ((analyserEquation->type() == AnalyserEquation::Type::NLA)
                        && !seenNlaSystemIndices.insert(analyserEquation->nlaSystemIndex()).second) {
                        continue;
                    }

                    if (seenAnalyserEquations.insert(analyserEquation.get()).second) {
                        if (analyserVariable->type() == AnalyserVariable::Type::CONSTANT) {
                            // This is a constant, so keep track of it in case it is untracked and in case we need to
                            // generate some code for it.

                            analyserEquation->mPimpl->mConstant = analyserVariable;
                        }

                        equationDependencies.push_back(analyserEquation);
                    }
                }
            }
        }

        // Determine the equation's NLA siblings, i.e. the equations that should
        // be computed as part of an NLA system, should this equation be an NLA
        // one.

        AnalyserEquationPtrs equationNlaSiblings;

        for (const auto &nlaSibling : internalEquation->mNlaSiblings) {
            equationNlaSiblings.push_back(aie2aeMappings[nlaSibling.lock()]);
        }

        // Populate and keep track of the equation.

        auto equation = aie2aeMappings[internalEquation];

        equation->mPimpl->mType = equationType;
        equation->mPimpl->mAst = (equationType == AnalyserEquation::Type::EXTERNAL) ?
                                     nullptr :
                                     internalEquation->mAst;
        equation->mPimpl->mNlaSystemIndex = internalEquation->mNlaSystemIndex;

        for (const auto &unknownVariable : internalEquation->mUnknownVariables) {
            // Keep track of the variable that the equation computes.
            // Note: an unknown variable cannot be the variable of integration (since it cannot be computed) or a
            //       constant (snce it is not computed but set), so no need to check for those.

            auto variable = aiv2avMappings[unknownVariable];
            auto variableType = variable->type();

            if (variableType == AnalyserVariable::Type::STATE) {
                equation->mPimpl->mStates.push_back(variable);
            } else if (variableType == AnalyserVariable::Type::COMPUTED_CONSTANT) {
                equation->mPimpl->mComputedConstants.push_back(variable);
            } else if (variableType == AnalyserVariable::Type::ALGEBRAIC_VARIABLE) {
                equation->mPimpl->mAlgebraicVariables.push_back(variable);
            } else { // AnalyserVariable::Type::EXTERNAL_VARIABLE.
                equation->mPimpl->mExternalVariables.push_back(variable);
            }
        }

        std::copy(equationDependencies.begin(), equationDependencies.end(), back_inserter(equation->mPimpl->mDependencies));
        std::copy(equationNlaSiblings.begin(), equationNlaSiblings.end(), back_inserter(equation->mPimpl->mNlaSiblings));

        mAnalyserModel->mPimpl->mAnalyserEquations.push_back(equation);
    }

    // Remove the staging equations for our constants.
    // Note: indeed, some equations may have a dependency on one or several constants (for which there are no proper
    //       equations). So, we need to remove those dependencies, and obviously this can only be done once all our
    //       equations are ready.

    for (const auto &analyserEquation : mAnalyserModel->mPimpl->mAnalyserEquations) {
        analyserEquation->mPimpl->removeStagingDependencies();
    }

    // Determine whether our equations are state/rate based.
    // Note: obviously, this can only be done once all our equations are ready.

    for (const auto &analyserEquation : mAnalyserModel->mPimpl->mAnalyserEquations) {
        std::unordered_set<AnalyserEquation *> checkedEquations;

        analyserEquation->mPimpl->mIsStateRateBased = isStateRateBased(analyserEquation, checkedEquations);
    }
}

AnalyserExternalVariablePtrs::const_iterator Analyser::AnalyserImpl::findExternalVariable(const VariablePtr &variable) const
{
    return std::find_if(mExternalVariables.begin(), mExternalVariables.end(), [&](const auto &ev) {
        return ev->variable() == variable;
    });
}

AnalyserExternalVariablePtrs::const_iterator Analyser::AnalyserImpl::findExternalVariable(const AnalyserExternalVariablePtr &externalVariable) const
{
    return std::find_if(mExternalVariables.begin(), mExternalVariables.end(), [&](const auto &ev) {
        return ev == externalVariable;
    });
}

Analyser::AnalyserImpl *Analyser::pFunc()
{
    return reinterpret_cast<Analyser::AnalyserImpl *>(Logger::pFunc());
}

const Analyser::AnalyserImpl *Analyser::pFunc() const
{
    return reinterpret_cast<Analyser::AnalyserImpl const *>(Logger::pFunc());
}

Analyser::Analyser()
    : Logger(new AnalyserImpl())
{
    pFunc()->mAnalyser = this;
}

Analyser::~Analyser()
{
    delete pFunc();
}

AnalyserPtr Analyser::create() noexcept
{
    return std::shared_ptr<Analyser> {new Analyser {}};
}

void Analyser::analyseModel(const ModelPtr &model)
{
    // Make sure that we have a model and that it is valid before analysing it.

    pFunc()->removeAllIssues();

    if (model == nullptr) {
        auto issue = Issue::IssueImpl::create();

        issue->mPimpl->setDescription("The model is null.");
        issue->mPimpl->setReferenceRule(Issue::ReferenceRule::INVALID_ARGUMENT);

        pFunc()->addIssue(issue);

        return;
    }

    auto validator = Validator::create();

    validator->validateModel(model);

    if (validator->issueCount() > 0) {
        // The model is not valid, so retrieve the validation issues and make
        // them our own.

        for (size_t i = 0; i < validator->issueCount(); ++i) {
            pFunc()->addIssue(validator->issue(i));
        }

        pFunc()->mAnalyserModel->mPimpl->mType = AnalyserModel::Type::INVALID;
    }

    // Check for non-validation errors that will render the given model invalid
    // for analysis.

    if (model->hasUnlinkedUnits()) {
        auto issue = Issue::IssueImpl::create();

        issue->mPimpl->setDescription("The model has units which are not linked together.");
        issue->mPimpl->setReferenceRule(Issue::ReferenceRule::ANALYSER_UNLINKED_UNITS);

        pFunc()->addIssue(issue);
    }

    // Analyse the model, but only if we didn't come across any issues.

    if (issueCount() == 0) {
        pFunc()->analyseModel(model);
    }
}

bool Analyser::addExternalVariable(const VariablePtr &variable)
{
    if (containsExternalVariable(variable)) {
        return false;
    }

    pFunc()->mExternalVariables.push_back(AnalyserExternalVariable::create(variable));

    return true;
}

bool Analyser::addExternalVariable(const AnalyserExternalVariablePtr &externalVariable)
{
    if (containsExternalVariable(externalVariable)) {
        return false;
    }

    pFunc()->mExternalVariables.push_back(externalVariable);

    return true;
}

bool Analyser::removeExternalVariable(size_t index)
{
    if (index < pFunc()->mExternalVariables.size()) {
        pFunc()->mExternalVariables.erase(pFunc()->mExternalVariables.begin() + ptrdiff_t(index));

        return true;
    }

    return false;
}

bool Analyser::removeExternalVariable(const VariablePtr &variable)
{
    auto result = pFunc()->findExternalVariable(variable);

    if (result != pFunc()->mExternalVariables.end()) {
        pFunc()->mExternalVariables.erase(result);

        return true;
    }

    return false;
}

bool Analyser::removeExternalVariable(const AnalyserExternalVariablePtr &externalVariable)
{
    auto result = pFunc()->findExternalVariable(externalVariable);

    if (result != pFunc()->mExternalVariables.end()) {
        pFunc()->mExternalVariables.erase(result);

        return true;
    }

    return false;
}

void Analyser::removeAllExternalVariables()
{
    pFunc()->mExternalVariables.clear();
}

bool Analyser::containsExternalVariable(const VariablePtr &variable) const
{
    return pFunc()->findExternalVariable(variable) != pFunc()->mExternalVariables.end();
}

bool Analyser::containsExternalVariable(const AnalyserExternalVariablePtr &externalVariable) const
{
    return pFunc()->findExternalVariable(externalVariable) != pFunc()->mExternalVariables.end();
}

AnalyserExternalVariablePtr Analyser::externalVariable(size_t index) const
{
    if (index < pFunc()->mExternalVariables.size()) {
        return pFunc()->mExternalVariables[index];
    }

    return nullptr;
}

AnalyserExternalVariablePtr Analyser::externalVariable(const VariablePtr &variable) const
{
    auto result = pFunc()->findExternalVariable(variable);

    if (result != pFunc()->mExternalVariables.end()) {
        return *result;
    }

    return nullptr;
}

size_t Analyser::externalVariableCount() const
{
    return pFunc()->mExternalVariables.size();
}

AnalyserModelPtr Analyser::analyserModel() const
{
    return pFunc()->mAnalyserModel;
}

} // namespace libcellml
