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

#include "utilities.h"

#include "libcellml/generatorprofile.h"

#include <cmath>
#include <string>

namespace libcellml {

struct GeneratorProfile::GeneratorProfileImpl
{
    // Relational operators

    std::string mEqString = " = ";
    std::string mEqEqString = " == ";
    std::string mNeqString = " != ";
    std::string mLtString = " < ";
    std::string mLeqString = " <= ";
    std::string mGtString = " > ";
    std::string mGeqString = " >= ";

    // Arithmetic operators

    std::string mPlusString = "+";
    std::string mMinusString = "-";
    std::string mTimesString = "*";
    std::string mDivideString = "/";
    std::string mPowerString = "pow";
    std::string mSquareRootString = "sqrt";
    std::string mSquareString = "sqr";
    std::string mAbsoluteValueString = "fabs";
    std::string mExponentialString = "exp";
    std::string mNapierianLogarithmString = "log";
    std::string mCommonLogarithmString = "log10";
    std::string mCeilingString = "ceil";
    std::string mFloorString = "floor";
    std::string mFactorialString = "fact";

    bool mHasPowerOperator = false;

    bool mHasXorOperator = true;

    // Logical operators

    std::string mAndString = " && ";
    std::string mOrString = " || ";
    std::string mXorString = "^";
    std::string mNotString = "!";

    // Min/max operators

    std::string mMinString = "min";
    std::string mMaxString = "max";

    // Gcd/lcm operators

    std::string mGcdString = "gcd";
    std::string mLcmString = "lcm";

    // Trigonometric operators

    std::string mSinString = "sin";
    std::string mCosString = "cos";
    std::string mTanString = "tan";
    std::string mSecString = "sec";
    std::string mCscString = "csc";
    std::string mCotString = "cot";
    std::string mSinhString = "sinh";
    std::string mCoshString = "cosh";
    std::string mTanhString = "tanh";
    std::string mSechString = "sech";
    std::string mCschString = "csch";
    std::string mCothString = "coth";
    std::string mAsinString = "asin";
    std::string mAcosString = "acos";
    std::string mAtanString = "atan";
    std::string mAsecString = "asec";
    std::string mAcscString = "acsc";
    std::string mAcotString = "acot";
    std::string mAsinhString = "asinh";
    std::string mAcoshString = "acosh";
    std::string mAtanhString = "atanh";
    std::string mAsechString = "asech";
    std::string mAcschString = "acsch";
    std::string mAcothString = "acoth";

    // Extra operators

    std::string mRemString = "fmod";

    // Piecewise statement
    // Note: the parentheses around #cond is not needed (because of precedence
    //       rules). It's just that it looks better/clearer to have them
    //       (somewhat subjective indeed).

    std::string mConditionalOperatorIfString = "(#cond)?#if";
    std::string mConditionalOperatorElseString = ":#else";
    std::string mPiecewiseIfString = "piecewise(#cond, #if";
    std::string mPiecewiseElseString = ", #else)";

    bool mHasConditionalOperator = true;

    // Constants

    std::string mTrueString = "true";
    std::string mFalseString = "false";
    std::string mEString = convertDoubleToString(exp(1.0));
    std::string mPiString = convertDoubleToString(M_PI);
    std::string mInfString = "1.0/0.0";
    std::string mNanString = "sqrt(-1.0)";

    // Miscellaneous

    std::string mCommandSeparatorString = ";";
};

GeneratorProfile::GeneratorProfile()
    : mPimpl(new GeneratorProfileImpl())
{
}

GeneratorProfile::~GeneratorProfile()
{
    delete mPimpl;
}

GeneratorProfile::GeneratorProfile(const GeneratorProfile &rhs)
    : mPimpl(new GeneratorProfileImpl())
{
    (void)rhs;
}

GeneratorProfile::GeneratorProfile(GeneratorProfile &&rhs) noexcept
    : mPimpl(rhs.mPimpl)
{
    rhs.mPimpl = nullptr;
}

GeneratorProfile &GeneratorProfile::operator=(GeneratorProfile rhs)
{
    rhs.swap(*this);
    return *this;
}

void GeneratorProfile::swap(GeneratorProfile &rhs)
{
    std::swap(mPimpl, rhs.mPimpl);
}

std::string GeneratorProfile::eqString() const
{
    return mPimpl->mEqString;
}

void GeneratorProfile::setEqString(const std::string &eqString)
{
    mPimpl->mEqString = eqString;
}

std::string GeneratorProfile::eqEqString() const
{
    return mPimpl->mEqEqString;
}

void GeneratorProfile::setEqEqString(const std::string &eqEqString)
{
    mPimpl->mEqEqString = eqEqString;
}

std::string GeneratorProfile::neqString() const
{
    return mPimpl->mNeqString;
}

void GeneratorProfile::setNeqString(const std::string &neqString)
{
    mPimpl->mNeqString = neqString;
}

std::string GeneratorProfile::ltString() const
{
    return mPimpl->mLtString;
}

void GeneratorProfile::setLtString(const std::string &ltString)
{
    mPimpl->mLtString = ltString;
}

std::string GeneratorProfile::leqString() const
{
    return mPimpl->mLeqString;
}

void GeneratorProfile::setLeqString(const std::string &leqString)
{
    mPimpl->mLeqString = leqString;
}

std::string GeneratorProfile::gtString() const
{
    return mPimpl->mGtString;
}

void GeneratorProfile::setGtString(const std::string &gtString)
{
    mPimpl->mGtString = gtString;
}

std::string GeneratorProfile::geqString() const
{
    return mPimpl->mGeqString;
}

void GeneratorProfile::setGeqString(const std::string &geqString)
{
    mPimpl->mGeqString = geqString;
}

std::string GeneratorProfile::plusString() const
{
    return mPimpl->mPlusString;
}

void GeneratorProfile::setPlusString(const std::string &plusString)
{
    mPimpl->mPlusString = plusString;
}

std::string GeneratorProfile::minusString() const
{
    return mPimpl->mMinusString;
}

void GeneratorProfile::setMinusString(const std::string &minusString)
{
    mPimpl->mMinusString = minusString;
}

std::string GeneratorProfile::timesString() const
{
    return mPimpl->mTimesString;
}

void GeneratorProfile::setTimesString(const std::string &timesString)
{
    mPimpl->mTimesString = timesString;
}

std::string GeneratorProfile::divideString() const
{
    return mPimpl->mDivideString;
}

void GeneratorProfile::setDivideString(const std::string &divideString)
{
    mPimpl->mDivideString = divideString;
}

std::string GeneratorProfile::powerString() const
{
    return mPimpl->mPowerString;
}

void GeneratorProfile::setPowerString(const std::string &powerString)
{
    mPimpl->mPowerString = powerString;
}

std::string GeneratorProfile::squareRootString() const
{
    return mPimpl->mSquareRootString;
}

void GeneratorProfile::setSquareRootString(const std::string &squareRootString)
{
    mPimpl->mSquareRootString = squareRootString;
}

std::string GeneratorProfile::squareString() const
{
    return mPimpl->mSquareString;
}

void GeneratorProfile::setSquareString(const std::string &squareString)
{
    mPimpl->mSquareString = squareString;
}

std::string GeneratorProfile::absoluteValueString() const
{
    return mPimpl->mAbsoluteValueString;
}

void GeneratorProfile::setAbsoluteValueString(const std::string &absoluteValueString)
{
    mPimpl->mAbsoluteValueString = absoluteValueString;
}

std::string GeneratorProfile::exponentialString() const
{
    return mPimpl->mExponentialString;
}

void GeneratorProfile::setExponentialString(const std::string &exponentialString)
{
    mPimpl->mExponentialString = exponentialString;
}

std::string GeneratorProfile::napierianLogarithmString() const
{
    return mPimpl->mNapierianLogarithmString;
}

void GeneratorProfile::setNapierianLogarithmString(const std::string &napierianLogarithmString)
{
    mPimpl->mNapierianLogarithmString = napierianLogarithmString;
}

std::string GeneratorProfile::commonLogarithmString() const
{
    return mPimpl->mCommonLogarithmString;
}

void GeneratorProfile::setCommonLogarithmString(const std::string &commonLogarithmString)
{
    mPimpl->mCommonLogarithmString = commonLogarithmString;
}

std::string GeneratorProfile::ceilingString() const
{
    return mPimpl->mCeilingString;
}

void GeneratorProfile::setCeilingString(const std::string &ceilingString)
{
    mPimpl->mCeilingString = ceilingString;
}

std::string GeneratorProfile::floorString() const
{
    return mPimpl->mFloorString;
}

void GeneratorProfile::setFloorString(const std::string &floorString)
{
    mPimpl->mFloorString = floorString;
}

std::string GeneratorProfile::factorialString() const
{
    return mPimpl->mFactorialString;
}

void GeneratorProfile::setFactorialString(const std::string &factorialString)
{
    mPimpl->mFactorialString = factorialString;
}

bool GeneratorProfile::hasPowerOperator() const
{
    return mPimpl->mHasPowerOperator;
}

void GeneratorProfile::setHasPowerOperator(bool hasPowerOperator)
{
    mPimpl->mHasPowerOperator = hasPowerOperator;
}

bool GeneratorProfile::hasXorOperator() const
{
    return mPimpl->mHasXorOperator;
}

void GeneratorProfile::setHasXorOperator(bool hasXorOperator)
{
    mPimpl->mHasXorOperator = hasXorOperator;
}

std::string GeneratorProfile::andString() const
{
    return mPimpl->mAndString;
}

void GeneratorProfile::setAndString(const std::string &andString)
{
    mPimpl->mAndString = andString;
}

std::string GeneratorProfile::orString() const
{
    return mPimpl->mOrString;
}

void GeneratorProfile::setOrString(const std::string &orString)
{
    mPimpl->mOrString = orString;
}

std::string GeneratorProfile::xorString() const
{
    return mPimpl->mXorString;
}

void GeneratorProfile::setXorString(const std::string &xorString)
{
    mPimpl->mXorString = xorString;
}

std::string GeneratorProfile::notString() const
{
    return mPimpl->mNotString;
}

void GeneratorProfile::setNotString(const std::string &notString)
{
    mPimpl->mNotString = notString;
}

std::string GeneratorProfile::minString() const
{
    return mPimpl->mMinString;
}

void GeneratorProfile::setMinString(const std::string &minString)
{
    mPimpl->mMinString = minString;
}

std::string GeneratorProfile::maxString() const
{
    return mPimpl->mMaxString;
}

void GeneratorProfile::setMaxString(const std::string &maxString)
{
    mPimpl->mMaxString = maxString;
}

std::string GeneratorProfile::gcdString() const
{
    return mPimpl->mGcdString;
}

void GeneratorProfile::setGcdString(const std::string &gcdString)
{
    mPimpl->mGcdString = gcdString;
}

std::string GeneratorProfile::lcmString() const
{
    return mPimpl->mLcmString;
}

void GeneratorProfile::setLcmString(const std::string &lcmString)
{
    mPimpl->mLcmString = lcmString;
}

std::string GeneratorProfile::sinString() const
{
    return mPimpl->mSinString;
}

void GeneratorProfile::setSinString(const std::string &sinString)
{
    mPimpl->mSinString = sinString;
}

std::string GeneratorProfile::cosString() const
{
    return mPimpl->mCosString;
}

void GeneratorProfile::setCosString(const std::string &cosString)
{
    mPimpl->mCosString = cosString;
}

std::string GeneratorProfile::tanString() const
{
    return mPimpl->mTanString;
}

void GeneratorProfile::setTanString(const std::string &tanString)
{
    mPimpl->mTanString = tanString;
}

std::string GeneratorProfile::secString() const
{
    return mPimpl->mSecString;
}

void GeneratorProfile::setSecString(const std::string &secString)
{
    mPimpl->mSecString = secString;
}

std::string GeneratorProfile::cscString() const
{
    return mPimpl->mCscString;
}

void GeneratorProfile::setCscString(const std::string &cscString)
{
    mPimpl->mCscString = cscString;
}

std::string GeneratorProfile::cotString() const
{
    return mPimpl->mCotString;
}

void GeneratorProfile::setCotString(const std::string &cotString)
{
    mPimpl->mCotString = cotString;
}

std::string GeneratorProfile::sinhString() const
{
    return mPimpl->mSinhString;
}

void GeneratorProfile::setSinhString(const std::string &sinhString)
{
    mPimpl->mSinhString = sinhString;
}

std::string GeneratorProfile::coshString() const
{
    return mPimpl->mCoshString;
}

void GeneratorProfile::setCoshString(const std::string &coshString)
{
    mPimpl->mCoshString = coshString;
}

std::string GeneratorProfile::tanhString() const
{
    return mPimpl->mTanhString;
}

void GeneratorProfile::setTanhString(const std::string &tanhString)
{
    mPimpl->mTanhString = tanhString;
}

std::string GeneratorProfile::sechString() const
{
    return mPimpl->mSechString;
}

void GeneratorProfile::setSechString(const std::string &sechString)
{
    mPimpl->mSechString = sechString;
}

std::string GeneratorProfile::cschString() const
{
    return mPimpl->mCschString;
}

void GeneratorProfile::setCschString(const std::string &cschString)
{
    mPimpl->mCschString = cschString;
}

std::string GeneratorProfile::cothString() const
{
    return mPimpl->mCothString;
}

void GeneratorProfile::setCothString(const std::string &cothString)
{
    mPimpl->mCothString = cothString;
}

std::string GeneratorProfile::asinString() const
{
    return mPimpl->mAsinString;
}

void GeneratorProfile::setAsinString(const std::string &asinString)
{
    mPimpl->mAsinString = asinString;
}

std::string GeneratorProfile::acosString() const
{
    return mPimpl->mAcosString;
}

void GeneratorProfile::setAcosString(const std::string &acosString)
{
    mPimpl->mAcosString = acosString;
}

std::string GeneratorProfile::atanString() const
{
    return mPimpl->mAtanString;
}

void GeneratorProfile::setAtanString(const std::string &atanString)
{
    mPimpl->mAtanString = atanString;
}

std::string GeneratorProfile::asecString() const
{
    return mPimpl->mAsecString;
}

void GeneratorProfile::setAsecString(const std::string &asecString)
{
    mPimpl->mAsecString = asecString;
}

std::string GeneratorProfile::acscString() const
{
    return mPimpl->mAcscString;
}

void GeneratorProfile::setAcscString(const std::string &acscString)
{
    mPimpl->mAcscString = acscString;
}

std::string GeneratorProfile::acotString() const
{
    return mPimpl->mAcotString;
}

void GeneratorProfile::setAcotString(const std::string &acotString)
{
    mPimpl->mAcotString = acotString;
}

std::string GeneratorProfile::asinhString() const
{
    return mPimpl->mAsinhString;
}

void GeneratorProfile::setAsinhString(const std::string &asinhString)
{
    mPimpl->mAsinhString = asinhString;
}

std::string GeneratorProfile::acoshString() const
{
    return mPimpl->mAcoshString;
}

void GeneratorProfile::setAcoshString(const std::string &acoshString)
{
    mPimpl->mAcoshString = acoshString;
}

std::string GeneratorProfile::atanhString() const
{
    return mPimpl->mAtanhString;
}

void GeneratorProfile::setAtanhString(const std::string &atanhString)
{
    mPimpl->mAtanhString = atanhString;
}

std::string GeneratorProfile::asechString() const
{
    return mPimpl->mAsechString;
}

void GeneratorProfile::setAsechString(const std::string &asechString)
{
    mPimpl->mAsechString = asechString;
}

std::string GeneratorProfile::acschString() const
{
    return mPimpl->mAcschString;
}

void GeneratorProfile::setAcschString(const std::string &acschString)
{
    mPimpl->mAcschString = acschString;
}

std::string GeneratorProfile::acothString() const
{
    return mPimpl->mAcothString;
}

void GeneratorProfile::setAcothString(const std::string &acothString)
{
    mPimpl->mAcothString = acothString;
}

std::string GeneratorProfile::remString() const
{
    return mPimpl->mRemString;
}

void GeneratorProfile::setRemString(const std::string &remString)
{
    mPimpl->mRemString = remString;
}

std::string GeneratorProfile::conditionalOperatorIfString() const
{
    return mPimpl->mConditionalOperatorIfString;
}

void GeneratorProfile::setConditionalOperatorIfString(const std::string &conditionalOperatorIfString)
{
    mPimpl->mConditionalOperatorIfString = conditionalOperatorIfString;
}

std::string GeneratorProfile::conditionalOperatorElseString() const
{
    return mPimpl->mConditionalOperatorElseString;
}

void GeneratorProfile::setConditionalOperatorElseString(const std::string &conditionalOperatorElseString)
{
    mPimpl->mConditionalOperatorElseString = conditionalOperatorElseString;
}

std::string GeneratorProfile::piecewiseIfString() const
{
    return mPimpl->mPiecewiseIfString;
}

void GeneratorProfile::setPiecewiseIfString(const std::string &piecewiseIfString)
{
    mPimpl->mPiecewiseIfString = piecewiseIfString;
}

std::string GeneratorProfile::piecewiseElseString() const
{
    return mPimpl->mPiecewiseElseString;
}

void GeneratorProfile::setPiecewiseElseString(const std::string &piecewiseElseString)
{
    mPimpl->mPiecewiseElseString = piecewiseElseString;
}

bool GeneratorProfile::hasConditionalOperator() const
{
    return mPimpl->mHasConditionalOperator;
}

void GeneratorProfile::setHasConditionalOperator(bool hasConditionalOperator)
{
    mPimpl->mHasConditionalOperator = hasConditionalOperator;
}

std::string GeneratorProfile::trueString() const
{
    return mPimpl->mTrueString;
}

void GeneratorProfile::setTrueString(const std::string &trueString)
{
    mPimpl->mTrueString = trueString;
}

std::string GeneratorProfile::falseString() const
{
    return mPimpl->mFalseString;
}

void GeneratorProfile::setFalseString(const std::string &falseString)
{
    mPimpl->mFalseString = falseString;
}

std::string GeneratorProfile::eString() const
{
    return mPimpl->mEString;
}

void GeneratorProfile::setEString(const std::string &eString)
{
    mPimpl->mEString = eString;
}

std::string GeneratorProfile::piString() const
{
    return mPimpl->mPiString;
}

void GeneratorProfile::setPiString(const std::string &piString)
{
    mPimpl->mPiString = piString;
}

std::string GeneratorProfile::infString() const
{
    return mPimpl->mInfString;
}

void GeneratorProfile::setInfString(const std::string &infString)
{
    mPimpl->mInfString = infString;
}

std::string GeneratorProfile::nanString() const
{
    return mPimpl->mNanString;
}

void GeneratorProfile::setNanString(const std::string &nanString)
{
    mPimpl->mNanString = nanString;
}

std::string GeneratorProfile::commandSeparatorString() const
{
    return mPimpl->mCommandSeparatorString;
}

void GeneratorProfile::setCommandSeparatorString(const std::string &commandSeparator)
{
    mPimpl->mCommandSeparatorString = commandSeparator;
}

} // namespace libcellml