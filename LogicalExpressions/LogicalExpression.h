// -----------------------------------------------------------
// LogicalExpression class
// Responsibility: Philipp Paier
//
// DESCRIPTION:
//
//
// -----------------------------------------------------------

#pragma once

#include "LogicalExpressionBehavior.h"

template <typename T>
class CLogicalExpression final
{
private:
    std::shared_ptr<CLogicalExpressionBehavior<T>> m_leBehavior;

public:
    CLogicalExpression(CLogicalAtom<T> la1, CLogicalAtom<T> la2, std::function<bool(T, T)> f) :
        m_leBehavior(std::shared_ptr<CAtomExpressionBehavior<T>>(new CAtomExpressionBehavior<T>(la1, la2, f))) {}
    CLogicalExpression(CLogicalAtom<T> la, std::function<bool(T)> f) :
        m_leBehavior(std::shared_ptr<CSingleAtomExpressionBehavior<T>>(new CSingleAtomExpressionBehavior<T>(la, f))) {}
    CLogicalExpression(CLogicalExpression<T> le, std::function<bool(bool)> f) :
        m_leBehavior(std::shared_ptr<CModifiedExpressionBehavior<T>>(new CModifiedExpressionBehavior<T>(le.getBehavior(), f))) {}
    CLogicalExpression(CLogicalExpression<T> le1, CLogicalExpression<T> le2, std::function<bool(bool, bool)> f) :
        m_leBehavior(std::shared_ptr<CCombExpressionBehavior<T>>(new CCombExpressionBehavior<T>(le1.getBehavior(), le2.getBehavior(), f))) {}
    ~CLogicalExpression() {}

    bool evaluate(const std::vector<T> &values) const
    {
        return m_leBehavior->evaluate(values);
    }

    std::vector<bool> evaluate(const std::vector<std::vector<T>> &valuesVec) const
    {
        std::vector<bool> evalVec;
        for(auto & values : valuesVec)
            evalVec.push_back(m_leBehavior->evaluate(values));

        return std::move(evalVec);
    }

private:
    CLogicalExpression() {}
    CLogicalExpression(std::shared_ptr<CLogicalExpressionBehavior<T>> leb) : m_leBehavior(leb) {}
    std::shared_ptr<CLogicalExpressionBehavior<T>> getBehavior() const { return m_leBehavior; }
};


// -----------------------------------------------------------
// evaluate a bunch of expressions for one vector at once
// -----------------------------------------------------------
template <typename T>
std::vector<bool> evaluate(const std::vector<T> &values,
                           const std::vector<CLogicalExpression<T>> &expressions)
{
    std::vector<bool> evalVec;
    for(auto & e : expressions)
        evalVec.push_back(e.evaluate(values));

    return std::move(evalVec);
}

// -----------------------------------------------------------
// evaluate a bunch of expressions for varios vectors at once
// -----------------------------------------------------------
template <typename T>
std::vector<std::vector<bool>> evaluate(const std::vector<std::vector<T>> &valuesVec,
                           const std::vector<CLogicalExpression<T>> &expressions)
{
    std::vector<std::vector<bool>> evalVec;
    for(auto & values : valuesVec)
        evalVec.push_back(evaluate(values, expressions));

    return std::move(evalVec);
}

// -----------------------------------------------------------
// some predefined comparison operators to make usage of
// logical expressions easier
// -----------------------------------------------------------

// -----------------------------------------------------------
// standard comparisons of two logical atoms
// -----------------------------------------------------------
template <typename T>
CLogicalExpression<T> operator<(CLogicalAtom<T> a, CLogicalAtom<T> b)
{
    return CLogicalExpression<T>(a, b, std::less<T>());
}

template <typename T>
CLogicalExpression<T> operator<=(CLogicalAtom<T> a, CLogicalAtom<T> b)
{
    return CLogicalExpression<T>(a, b, std::less_equal<T>());
}

template <typename T>
CLogicalExpression<T> operator>(CLogicalAtom<T> a, CLogicalAtom<T> b)
{
    return CLogicalExpression<T>(a, b, std::greater<T>());
}


template <typename T>
CLogicalExpression<T> operator>=(CLogicalAtom<T> a, CLogicalAtom<T> b)
{
    return CLogicalExpression<T>(a, b, std::greater_equal<T>());
}


template <typename T>
CLogicalExpression<T> operator==(CLogicalAtom<T> a, CLogicalAtom<T> b)
{
    return CLogicalExpression<T>(a, b, std::equal_to<T>());
}

template <typename T>
CLogicalExpression<T> operator!=(CLogicalAtom<T> a, CLogicalAtom<T> b)
{
    return CLogicalExpression<T>(a, b, std::not_equal_to<T>());
}


// -----------------------------------------------------------
// standard comparisons of a logical atom and a constant
// -----------------------------------------------------------
template <typename T>
CLogicalExpression<T> operator<(CLogicalAtom<T> a, T val)
{
    return CLogicalExpression<T>(a, [val](double a){ return a < val; } );
}

template <typename T>
CLogicalExpression<T> operator<=(CLogicalAtom<T> a, T val)
{
    return CLogicalExpression<T>(a, [val](double a){ return a <= val; } );
}


template <typename T>
CLogicalExpression<T> operator>(CLogicalAtom<T> a, T val)
{
    return CLogicalExpression<T>(a, [val](double a){ return a > val; } );
}

template <typename T>
CLogicalExpression<T> operator>=(CLogicalAtom<T> a, T val)
{
    return CLogicalExpression<T>(a, [val](double a){ return a >= val; } );
}


template <typename T>
CLogicalExpression<T> operator==(CLogicalAtom<T> a, T val)
{
    return CLogicalExpression<T>(a, [val](double a){ return a == val; } );
}

template <typename T>
CLogicalExpression<T> operator!=(CLogicalAtom<T> a, T val)
{
    return CLogicalExpression<T>(a, [val](double a){ return a != val; } );
}

template <typename T>
CLogicalExpression<T> operator<(T val, CLogicalAtom<T> b)
{
    return CLogicalExpression<T>(b, [val](double b){ return val < b; } );
}

template <typename T>
CLogicalExpression<T> operator<=(T val, CLogicalAtom<T> b)
{
    return CLogicalExpression<T>(b, [val](double b){ return val <= b; } );
}


template <typename T>
CLogicalExpression<T> operator>(T val, CLogicalAtom<T> b)
{
    return CLogicalExpression<T>(b, [val](double b){ return val > b; } );
}

template <typename T>
CLogicalExpression<T> operator>=(T val, CLogicalAtom<T> b)
{
    return CLogicalExpression<T>(b, [val](double b){ return val >= b; } );
}


template <typename T>
CLogicalExpression<T> operator==(T val, CLogicalAtom<T> b)
{
    return CLogicalExpression<T>(b, [val](double b){ return val == b; } );
}

template <typename T>
CLogicalExpression<T> operator!=(T val, CLogicalAtom<T> b)
{
    return CLogicalExpression<T>(b, [val](double b){ return val != b; } );
}


// -----------------------------------------------------------
// basic logical operators for two logical expressions
// -----------------------------------------------------------
template <typename T>
CLogicalExpression<T> operator&&(CLogicalExpression<T> a, CLogicalExpression<T> b)
{
    return CLogicalExpression<T>(a, b, std::logical_and<bool>());
}

template <typename T>
CLogicalExpression<T> operator||(CLogicalExpression<T> a, CLogicalExpression<T> b)
{
    return CLogicalExpression<T>(a, b, std::logical_or<bool>());
}

template <typename T>
CLogicalExpression<T> operator==(CLogicalExpression<T> a, CLogicalExpression<T> b)
{
    return CLogicalExpression<T>(a, b, std::equal_to<bool>());
}

template <typename T>
CLogicalExpression<T> operator!=(CLogicalExpression<T> a, CLogicalExpression<T> b)
{
    return CLogicalExpression<T>(a, b, std::not_equal_to<bool>());
}

template <typename T>
CLogicalExpression<T> operator!(CLogicalExpression<T> a)
{
    return CLogicalExpression<T>(a, std::logical_not<bool>());
}
