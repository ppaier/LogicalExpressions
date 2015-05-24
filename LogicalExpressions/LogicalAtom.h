// -----------------------------------------------------------
// Logical Atom interface class
// Responsibility: Philipp Paier
//
// DESCRIPTION:
// A logical atom must implement a subst method, that gets a
// vector of values and returns one value (not necessarily
// out of the vector)
// -----------------------------------------------------------
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

    T subst(const std::vector<T> &values) const
    {
        return m_laBehavior->subst(values);
    }

private:
    CLogicalAtom() {}
    CLogicalAtom(std::shared_ptr<CLogicalAtomBehavior<T>> lab) : m_laBehavior(lab) {}

    std::shared_ptr<CLogicalAtomBehavior<T>> getBehavior() const { return m_laBehavior; }
};



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
