
#pragma once
#include "LogicalExpressionBehavior.h"


template <typename T>
class CLogicalExpression
{
public:
    CLogicalExpression(std::shared_ptr<CLogicalExpressionBehavior<T>> leb) : m_leBehavior(leb) {}

    CLogicalExpression(CLogicalAtom<T> la1, CLogicalAtom<T> la2, std::function<bool(T, T)> f) :
        m_leBehavior(std::make_shared<CAtomExpressionBehavior<T>>(la1, la2, f)) {}

    CLogicalExpression(CLogicalExpression<T> le, std::function<bool(bool)> f) :
        m_leBehavior(std::make_shared<CModifiedExpressionBehavior<T>>(le.getBehavior(), f)) {}

    CLogicalExpression(CLogicalExpression<T> le1, CLogicalExpression<T> le2, std::function<bool(bool, bool)> f) :
        m_leBehavior(std::make_shared<CCombExpressionBehavior<T>>(le1.getBehavior(), le2.getBehavior(), f)) {}
    
    virtual ~CLogicalExpression() {}

    virtual bool evaluate(const std::vector<T> &values) const
    {
        return m_leBehavior->evaluate(values);
    }
    
private:
    CLogicalExpression() {}
    virtual std::shared_ptr<CLogicalExpressionBehavior<T>> getBehavior() const { return m_leBehavior; }

    std::shared_ptr<CLogicalExpressionBehavior<T>> m_leBehavior;
};



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
    CLogicalAtom<T> b = CLogicalAtom<T>::CreateConstAtom(val);
    return a<b;
}

template <typename T>
CLogicalExpression<T> operator<=(CLogicalAtom<T> a, T val)
{
    CLogicalAtom<T> b = CLogicalAtom<T>::CreateConstAtom(val);
    return a <= b;
}


template <typename T>
CLogicalExpression<T> operator>(CLogicalAtom<T> a, T val)
{
    CLogicalAtom<T> b = CLogicalAtom<T>::CreateConstAtom(val);
    return a>b;
}

template <typename T>
CLogicalExpression<T> operator>=(CLogicalAtom<T> a, T val)
{
    CLogicalAtom<T> b = CLogicalAtom<T>::CreateConstAtom(val);
    return a >= b;
}


template <typename T>
CLogicalExpression<T> operator==(CLogicalAtom<T> a, T val)
{
    CLogicalAtom<T> b = CLogicalAtom<T>::CreateConstAtom(val);
    return a == b;
}

template <typename T>
CLogicalExpression<T> operator!=(CLogicalAtom<T> a, T val)
{
    CLogicalAtom<T> b = CLogicalAtom<T>::CreateConstAtom(val);
    return a != b;
}

template <typename T>
CLogicalExpression<T> operator<(T val, CLogicalAtom<T> b)
{
    CLogicalAtom<T> a = CLogicalAtom<T>::CreateConstAtom(val);
    return a<b;
}

template <typename T>
CLogicalExpression<T> operator<=(T val, CLogicalAtom<T> b)
{
    CLogicalAtom<T> a = CLogicalAtom<T>::CreateConstAtom(val);
    return a <= b;
}


template <typename T>
CLogicalExpression<T> operator>(T val, CLogicalAtom<T> b)
{
    CLogicalAtom<T> a = CLogicalAtom<T>::CreateConstAtom(val);
    return a>b;
}

template <typename T>
CLogicalExpression<T> operator>=(T val, CLogicalAtom<T> b)
{
    CLogicalAtom<T> a = CLogicalAtom<T>::CreateConstAtom(val);
    return a >= b;
}


template <typename T>
CLogicalExpression<T> operator==(T val, CLogicalAtom<T> b)
{
    CLogicalAtom<T> a = CLogicalAtom<T>::CreateConstAtom(val);
    return a == b;
}

template <typename T>
CLogicalExpression<T> operator!=(T val, CLogicalAtom<T> b)
{
    CLogicalAtom<T> a = CLogicalAtom<T>::CreateConstAtom(val);
    return a != b;
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