// -----------------------------------------------------------
// LogicalAtom class
// Responsibility: Philipp Paier
//
// DESCRIPTION:
//
//
// -----------------------------------------------------------

#pragma once

#include "TermBehavior.h"

template <typename T> class CLogicalExpression;

template <typename T>
class CTerm final
{
private:
    std::shared_ptr<CTermBehavior<T>> m_laBehavior;

public:
    CTerm(CTerm<T> a, CTerm<T> b, std::function<T(T, T)> f) :
        m_laBehavior(std::shared_ptr<CCombinedTermBehavior<T>>(new CCombinedTermBehavior<T>(a.getBehavior(), b.getBehavior(), f))) {}
    CTerm(CTerm<T> a, std::function<T(T)> f) :
        m_laBehavior(std::shared_ptr<CModifiedTermBehavior<T>>(new CModifiedTermBehavior<T>(a.getBehavior(), f))) {}
    ~CTerm() {}

    static CTerm<T> CreateConstTerm(T val)
    {
        return CTerm<T>(std::shared_ptr<CConstTermBehavior<T>>(new CConstTermBehavior<T>(val)));
    }

    static CTerm<T> CreateVariableTerm(unsigned int idx)
    {
        return CTerm<T>(std::shared_ptr<CVariableTermBehavior<T>>(new CVariableTermBehavior<T>(idx)));
    }

    T substitute(const std::vector<T> &values) const
    {
        return m_laBehavior->substitute(values);
    }

    std::vector<T> substitute(const std::vector<std::vector<T>> &valuesVec) const
    {
        std::vector<T> substVec;
        for(auto & values : valuesVec)
            substVec.push_back(m_laBehavior->substitute(values));

        return std::move(substVec);
    }

private:
    CTerm() {}
    CTerm(std::shared_ptr<CTermBehavior<T>> lab) : m_laBehavior(lab) {}

    std::shared_ptr<CTermBehavior<T>> getBehavior() const { return m_laBehavior; }
};

template <typename T>
std::vector<T> substitute(std::vector<T> values, std::vector<CTerm<T>> atoms)
{
    std::vector<T> substVec;
    for(auto & a : atoms)
        substVec.push_back(a.substitute(values));

    return std::move(substVec);
}

template <typename T>
std::vector<std::vector<T>> substitute(const std::vector<std::vector<T>> &valuesVec,
                           const std::vector<CTerm<T>> &atoms)
{
    std::vector<std::vector<T>> substVec;
    for(auto & values : valuesVec)
        substVec.push_back(substitute(values, atoms));

    return std::move(substVec);
}

template <typename T>
CTerm<T> operator+(CTerm<T> a, CTerm<T> b)
{
    return CTerm<T>(a, b, std::plus<T>());
}


template <typename T>
CTerm<T> operator-(CTerm<T> a, CTerm<T> b)
{
    return CTerm<T>(a, b, std::minus<T>());
}


template <typename T>
CTerm<T> operator*(CTerm<T> a, CTerm<T> b)
{
    return CTerm<T>(a, b, std::multiplies<T>());
}


template <typename T>
CTerm<T> operator/(CTerm<T> a, CTerm<T> b)
{
    return CTerm<T>(a, b, std::divides<T>());
}


template <typename T>
CTerm<T> operator+(CTerm<T> a, T val)
{
    return CTerm<T>(a, [val](double a) { return a + val; });
}

template <typename T>
CTerm<T> operator+(T val, CTerm<T> b)
{
    return CTerm<T>(b, [val](double b) { return val + b; });
}


template <typename T>
CTerm<T> operator-(CTerm<T> a, T val)
{
    return CTerm<T>(a, [val](double a) { return a - val; });
}

template <typename T>
CTerm<T> operator-(T val, CTerm<T> b)
{

    return CTerm<T>(b, [val](double b) { return val - b; });
}

template <typename T>
CTerm<T> operator*(CTerm<T> a, T val)
{
    return CTerm<T>(a, [val](double a) { return a*val; });
}

template <typename T>
CTerm<T> operator*(T val, CTerm<T> b)
{
    return CTerm<T>(b, [val](double b) { return val * b; });
}

template <typename T>
CTerm<T> operator/(CTerm<T> a, T val)
{
    return CTerm<T>(a, [val](double a) { return a / val; });
}

template <typename T>
CTerm<T> operator/(T val, CTerm<T> b)
{
    return CTerm<T>(b, [val](double b) { return val / b; });
}
