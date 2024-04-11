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

#pragma once

#include <memory>

#include "libcellml/analyservariable.h"

namespace libcellml {

class InterpreterStatement; /**< Forward declaration of InterpreterStatement class. */
using InterpreterStatementPtr = std::shared_ptr<InterpreterStatement>; /**< Type definition for shared InterpreterStatement pointer. */
using InterpreterStatementPtrs = std::vector<InterpreterStatementPtr>; /**< Type definition for a list of shared InterpreterStatement pointers. */

/**
 * @brief The InterpreterStatement class.
 *
 * The InterpreterStatement class is for representing a CellML Interpreter Statement.
 */
class InterpreterStatement
{
public:
    /**
     * @brief The type of an instruction.
     *
     * The type of an instruction, i.e. whether it is an equality type, a relational or a logical operator, an
     * arithmetic operator, a calculus element, a trigonometric operator, a piecewise statement, a token element, a
     * qualifier element, or a constant.
     */
    enum class Type
    {
        // Equality.

        EQUALITY, /**< The equality operator. */

        // Relational and logical operators.

        //---GRY---
        // EQ, /**< The equal to operator. */
        // NEQ, /**< The not equal to operator. */
        // LT, /**< The less than operator. */
        // LEQ, /**< The less than or equal to operator. */
        // GT, /**< The greater than operator. */
        // GEQ, /**< The greater than or equal to operator. */
        // AND, /**< The and operator. */
        // OR, /**< The or operator. */
        // XOR, /**< The exclusive or operator. */
        // NOT, /**< The not operator. */

        // Arithmetic operators.

        //---GRY---
        // PLUS, /**< The plus operator. */
        // MINUS, /**< The minus operator. */
        // TIMES, /**< The times operator. */
        // DIVIDE, /**< The divide operator. */
        // POWER, /**< The power operator. */
        // ROOT, /**< The root operator. */
        // ABS, /**< The absolute value function. */
        // EXP, /**< The exponential function. */
        // LN, /**< The natural logarithm function. */
        // LOG, /**< The common logarithm function. */
        // CEILING, /**< The ceiling function. */
        // FLOOR, /**< The floor function. */
        // MIN, /**< The minimum function. */
        // MAX, /**< The maximum function. */
        // REM, /**< The remainder function. */

        // Calculus elements.

        //---GRY---
        // DIFF, /**< The differentiation operator. */

        // Trigonometric operators.

        //---GRY---
        // SIN, /**< The sine function. */
        // COS, /**< The cosine function. */
        // TAN, /**< The tangent function. */
        // SEC, /**< The secant function. */
        // CSC, /**< The cosecant function. */
        // COT, /**< The cotangent function. */
        // SINH, /**< The hyperbolic sine function. */
        // COSH, /**< The hyperbolic cosine function. */
        // TANH, /**< The hyperbolic tangent function. */
        // SECH, /**< The hyperbolic secant function. */
        // CSCH, /**< The hyperbolic cosecant function. */
        // COTH, /**< The hyperbolic cotangent function. */
        // ASIN, /**< The arc sine function. */
        // ACOS, /**< The arc cosine function. */
        // ATAN, /**< The arc tangent function. */
        // ASEC, /**< The arc secant function. */
        // ACSC, /**< The arc cosecant function. */
        // ACOT, /**< The arc cotangent function. */
        // ASINH, /**< The arc hyperbolic sine function. */
        // ACOSH, /**< The arc hyperbolic cosine function. */
        // ATANH, /**< The arc hyperbolic tangent function. */
        // ASECH, /**< The arc hyperbolic secant function. */
        // ACSCH, /**< The arc hyperbolic cosecant function. */
        // ACOTH, /**< The arc hyperbolic cotangent function. */

        // Piecewise statement.

        //---GRY---
        // PIECEWISE, /**< The "piecewise" statement. */
        // PIECE, /**< The "piece" part of a "piecewise" statement. */
        // OTHERWISE, /**< The "otherwise" part of a "piecewise" statement. */

        // Token elements.

        CI, /**< An identifier (i.e. the name of a model variable). */
        CN, /**< A number. */

        // Qualifier elements.

        //---GRY---
        // DEGREE, /**< The degree of a root operator (it is only used when its value is not 2). */
        // LOGBASE, /**< The base with respect to which the logarithm is taken. */
        // BVAR, /**< The bound variable of a differential equation. */

        // Constants.

        //---GRY---
        // TRUE, /**< The "true" boolean. */
        // FALSE, /**< The "false" boolean. */
        // E, /**< Euler's number. */
        // PI, /**< The π constant. */
        // INF, /**< The infinity value. */
        // NAN /**< The not-a-number value. */
    };

    ~InterpreterStatement(); /**< Destructor, @private. */
    InterpreterStatement(const InterpreterStatement &rhs) = delete; /**< Copy constructor, @private. */
    InterpreterStatement(InterpreterStatement &&rhs) noexcept = delete; /**< Move constructor, @private. */
    InterpreterStatement &operator=(InterpreterStatement rhs) = delete; /**< Assignment operator, @private. */

    /**
     * @brief Create an @ref InterpreterStatement object.
     *
     * Factory method to create an @ref InterpreterStatement for an equality statement. Create an interpreter
     * instruction with::
     *
     * @code
     *   auto interpreterInstruction = libcellml::InterpreterStatement::createEquality(variable, value);
     * @endcode
     *
     * @return A smart pointer to an @ref InterpreterStatement object.
     */
    static InterpreterStatementPtr createEquality(const AnalyserVariablePtr &variable, double value) noexcept;

    /**
     * @brief Evaluate the instruction.
     *
     * Evaluate the instruction using the given arrays of states, rates, and variables.
     *
     * @param states The array of states.
     * @param rates The array of rates.
     * @param variables The array of variables.
     */
    void evaluate(double *states, double *rates, double *variables) const;

private:
    InterpreterStatement(Type type, const InterpreterStatementPtr &leftChild, const InterpreterStatementPtr &rightChild); /**< Constructor, @private. */
    InterpreterStatement(const AnalyserVariablePtr &variable); /**< Constructor, @private. */
    InterpreterStatement(double value); /**< Constructor, @private. */

    struct InterpreterStatementImpl;
    InterpreterStatementImpl *mPimpl; /**< Private member to implementation pointer, @private. */
};

} // namespace libcellml
