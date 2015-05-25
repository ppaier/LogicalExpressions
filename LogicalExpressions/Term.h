// -----------------------------------------------------------
// Term class
// Responsibility: Philipp Paier
//
// DESCRIPTION:
//
//
// -----------------------------------------------------------

#pragma once

#include "TermBehavior.h"

// -----------------------------------------------------------
// necessary forward declarations
// -----------------------------------------------------------
template <typename T> class CLogicalExpression;


// -----------------------------------------------------------
// term class that wraps a terms behavior
// offers the possibility to substitute the values of a vector
// into the term
// -----------------------------------------------------------
template <typename T>
class CTerm final
{
private:
    std::shared_ptr<CTermBehavior<T>> m_termBehavior;

public:
    CTerm(CTerm<T> a, CTerm<T> b, std::function<T(T, T)> f) :
        m_termBehavior(std::shared_ptr<CCombinedTermBehavior<T>>(new CCombinedTermBehavior<T>(a.getBehavior(), b.getBehavior(), f))) {}
    CTerm(CTerm<T> a, std::function<T(T)> f) :
        m_termBehavior(std::shared_ptr<CModifiedTermBehavior<T>>(new CModifiedTermBehavior<T>(a.getBehavior(), f))) {}
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
        return m_termBehavior->substitute(values);
    }

    std::vector<T> substitute(const std::vector<std::vector<T>> &valuesVec) const
    {
        std::vector<T> substVec;
        for(auto & values : valuesVec)
            substVec.push_back(m_termBehavior->substitute(values));

        return std::move(substVec);
    }

    // various assignment operators
    CTerm<T>& operator+=(const CTerm<T> &rhs)
    {
        m_termBehavior = std::shared_ptr<CCombinedTermBehavior<T>>(new CCombinedTermBehavior<T>(m_termBehavior, rhs.getBehavior(), std::plus<T>()));
        return *this;
    }

    CTerm<T>& operator+=(const T &val)
    {
        m_termBehavior = std::shared_ptr<CModifiedTermBehavior<T>>(new CModifiedTermBehavior<T>(m_termBehavior, [val](const T &a) { return a + val; }));
        return *this;
    }

    CTerm<T>& operator-=(const CTerm<T> &rhs)
    {
        m_termBehavior = std::shared_ptr<CCombinedTermBehavior<T>>(new CCombinedTermBehavior<T>(m_termBehavior, rhs.getBehavior(), std::minus<T>()));
        return *this;
    }

    CTerm<T>& operator-=(const T &val)
    {
        m_termBehavior = std::shared_ptr<CModifiedTermBehavior<T>>(new CModifiedTermBehavior<T>(m_termBehavior, [val](const T &a) { return a - val; }));
        return *this;
    }

    CTerm<T>& operator*=(const CTerm<T> &rhs)
    {
        m_termBehavior = std::shared_ptr<CCombinedTermBehavior<T>>(new CCombinedTermBehavior<T>(m_termBehavior, rhs.getBehavior(), std::multiplies<T>()));
        return *this;
    }

    CTerm<T>& operator*=(const T &val)
    {
        m_termBehavior = std::shared_ptr<CModifiedTermBehavior<T>>(new CModifiedTermBehavior<T>(m_termBehavior, [val](const T &a) { return a * val; }));
        return *this;
    }

    CTerm<T>& operator/=(const CTerm<T> &rhs)
    {
        m_termBehavior = std::shared_ptr<CCombinedTermBehavior<T>>(new CCombinedTermBehavior<T>(m_termBehavior, rhs.getBehavior(), std::divides<T>()));
        return *this;
    }

    CTerm<T>& operator/=(const T &val)
    {
        m_termBehavior = std::shared_ptr<CModifiedTermBehavior<T>>(new CModifiedTermBehavior<T>(m_termBehavior, [val](const T &a) { return a / val; }));
        return *this;
    }

private:
    CTerm() {}
    CTerm(std::shared_ptr<CTermBehavior<T>> tb) : m_termBehavior(tb) {}

    std::shared_ptr<CTermBehavior<T>> getBehavior() const { return m_termBehavior; }
};


// -----------------------------------------------------------
// substitute a bunch of terms for one vector at once
// -----------------------------------------------------------
template <typename T>
std::vector<T> substitute(std::vector<T> values, std::vector<CTerm<T>> terms)
{
    std::vector<T> substVec;
    for(auto & t : terms)
        substVec.push_back(t.substitute(values));

    return std::move(substVec);
}

// -----------------------------------------------------------
// substitute a bunch of terms for various vectors at once
// -----------------------------------------------------------
template <typename T>
std::vector<std::vector<T>> substitute(const std::vector<std::vector<T>> &valuesVec,
                           const std::vector<CTerm<T>> &terms)
{
    std::vector<std::vector<T>> substVec;
    for(auto & values : valuesVec)
        substVec.push_back(substitute(values, terms));

    return std::move(substVec);
}


// -----------------------------------------------------------
// some predefined operators to make usage of
// terms easier and intuitive
// -----------------------------------------------------------

// -----------------------------------------------------------
// sign of a term
// -----------------------------------------------------------
template <typename T>
CTerm<T> operator+(const CTerm<T> &a)
{
    return a;
}

template <typename T>
CTerm<T> operator-(const CTerm<T> &a)
{
    return CTerm<T>(a, [](const T &a) { return -a; });
}

// -----------------------------------------------------------
// addition of two terms, or a term and a constant
// -----------------------------------------------------------
template <typename T>
CTerm<T> operator+(const CTerm<T> &a, const CTerm<T> &b)
{
    return CTerm<T>(a, b, std::plus<T>());
}

template <typename T>
CTerm<T> operator+(const CTerm<T> &a, const T &val)
{
    return CTerm<T>(a, [val](const T &a) { return a + val; });
}

template <typename T>
CTerm<T> operator+(const T &val, const CTerm<T> &b)
{
    return CTerm<T>(b, [val](const T &b) { return val + b; });
}

// -----------------------------------------------------------
// subtraction of two terms, or a term and a constant
// -----------------------------------------------------------
template <typename T>
CTerm<T> operator-(const CTerm<T> &a, const CTerm<T> &b)
{
    return CTerm<T>(a, b, std::minus<T>());
}

template <typename T>
CTerm<T> operator-(const CTerm<T> &a, const T &val)
{
    return CTerm<T>(a, [val](const T &a) { return a - val; });
}

template <typename T>
CTerm<T> operator-(const T &val, const CTerm<T> &b)
{

    return CTerm<T>(b, [val](const T &b) { return val - b; });
}

// -----------------------------------------------------------
// multiplication of two terms, or a term and a constant
// -----------------------------------------------------------
template <typename T>
CTerm<T> operator*(const CTerm<T> &a, const CTerm<T> &b)
{
    return CTerm<T>(a, b, std::multiplies<T>());
}

template <typename T>
CTerm<T> operator*(const CTerm<T> &a, const T &val)
{
    return CTerm<T>(a, [val](const T &a) { return a*val; });
}

template <typename T>
CTerm<T> operator*(const T &val, const CTerm<T> &b)
{
    return CTerm<T>(b, [val](const T &b) { return val * b; });
}

// -----------------------------------------------------------
// division of two terms, or a term and a constant
// -----------------------------------------------------------
template <typename T>
CTerm<T> operator/(const CTerm<T> &a, const CTerm<T> &b)
{
    return CTerm<T>(a, b, std::divides<T>());
}

template <typename T>
CTerm<T> operator/(const CTerm<T> &a, const T &val)
{
    return CTerm<T>(a, [val](const T &a) { return a / val; });
}

template <typename T>
CTerm<T> operator/(const T &val, const CTerm<T> &b)
{
    return CTerm<T>(b, [val](const T &b) { return val / b; });
}

