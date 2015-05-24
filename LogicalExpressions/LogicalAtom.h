// -----------------------------------------------------------
// LogicalAtom class
// Responsibility: Philipp Paier
//
// DESCRIPTION:
//
//
// -----------------------------------------------------------

#pragma once

#include "LogicalAtomBehavior.h"

template <typename T> class CLogicalExpression;

template <typename T>
class CLogicalAtom final
{
private:
    std::shared_ptr<CLogicalAtomBehavior<T>> m_laBehavior;

public:
    CLogicalAtom(CLogicalAtom<T> a, CLogicalAtom<T> b, std::function<T(T, T)> f) :
        m_laBehavior(std::shared_ptr<CCombAtomBehavior<T>>(new CCombAtomBehavior<T>(a.getBehavior(), b.getBehavior(), f))) {}
    CLogicalAtom(CLogicalAtom<T> a, std::function<T(T)> f) :
        m_laBehavior(std::shared_ptr<CModifiedAtomBehavior<T>>(new CModifiedAtomBehavior<T>(a.getBehavior(), f))) {}
    ~CLogicalAtom() {}

    static CLogicalAtom<T> CreateConstAtom(T val)
    {
        return CLogicalAtom<T>(std::shared_ptr<CConstAtomBehavior<T>>(new CConstAtomBehavior<T>(val)));
    }

    static CLogicalAtom<T> CreateVarAtom(unsigned int idx)
    {
        return CLogicalAtom<T>(std::shared_ptr<CVarAtomBehavior<T>>(new CVarAtomBehavior<T>(idx)));
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
    CLogicalAtom() {}
    CLogicalAtom(std::shared_ptr<CLogicalAtomBehavior<T>> lab) : m_laBehavior(lab) {}

    std::shared_ptr<CLogicalAtomBehavior<T>> getBehavior() const { return m_laBehavior; }
};

template <typename T>
std::vector<T> substitute(std::vector<T> values, std::vector<CLogicalAtom<T>> atoms)
{
    std::vector<T> substVec;
    for(auto & a : atoms)
        substVec.push_back(a.substitute(values));

    return std::move(substVec);
}

template <typename T>
std::vector<std::vector<T>> substitute(const std::vector<std::vector<T>> &valuesVec,
                           const std::vector<CLogicalAtom<T>> &atoms)
{
    std::vector<std::vector<T>> substVec;
    for(auto & values : valuesVec)
        substVec.push_back(substitute(values, atoms));

    return std::move(substVec);
}

template <typename T>
CLogicalAtom<T> operator+(CLogicalAtom<T> a, CLogicalAtom<T> b)
{
    return CLogicalAtom<T>(a, b, std::plus<T>());
}


template <typename T>
CLogicalAtom<T> operator-(CLogicalAtom<T> a, CLogicalAtom<T> b)
{
    return CLogicalAtom<T>(a, b, std::minus<T>());
}


template <typename T>
CLogicalAtom<T> operator*(CLogicalAtom<T> a, CLogicalAtom<T> b)
{
    return CLogicalAtom<T>(a, b, std::multiplies<T>());
}


template <typename T>
CLogicalAtom<T> operator/(CLogicalAtom<T> a, CLogicalAtom<T> b)
{
    return CLogicalAtom<T>(a, b, std::divides<T>());
}


template <typename T>
CLogicalAtom<T> operator+(CLogicalAtom<T> a, T val)
{
    return CLogicalAtom<T>(a, [val](double a) { return a + val; });
}

template <typename T>
CLogicalAtom<T> operator+(T val, CLogicalAtom<T> b)
{
    return CLogicalAtom<T>(b, [val](double b) { return val + b; });
}


template <typename T>
CLogicalAtom<T> operator-(CLogicalAtom<T> a, T val)
{
    return CLogicalAtom<T>(a, [val](double a) { return a - val; });
}

template <typename T>
CLogicalAtom<T> operator-(T val, CLogicalAtom<T> b)
{

    return CLogicalAtom<T>(b, [val](double b) { return val - b; });
}

template <typename T>
CLogicalAtom<T> operator*(CLogicalAtom<T> a, T val)
{
    return CLogicalAtom<T>(a, [val](double a) { return a*val; });
}

template <typename T>
CLogicalAtom<T> operator*(T val, CLogicalAtom<T> b)
{
    return CLogicalAtom<T>(b, [val](double b) { return val * b; });
}

template <typename T>
CLogicalAtom<T> operator/(CLogicalAtom<T> a, T val)
{
    return CLogicalAtom<T>(a, [val](double a) { return a / val; });
}

template <typename T>
CLogicalAtom<T> operator/(T val, CLogicalAtom<T> b)
{
    return CLogicalAtom<T>(b, [val](double b) { return val / b; });
}
