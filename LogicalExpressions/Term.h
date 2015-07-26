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

namespace tc
{
    // -----------------------------------------------------------
    // necessary forward declarations
    // -----------------------------------------------------------
    template <typename T> class LogicalExpression;


    // -----------------------------------------------------------
    // term class that wraps a terms behavior
    // offers the possibility to substitute the values of a vector
    // into the term
    // -----------------------------------------------------------
    template <typename T>
    class Term final
    {
    private:
        std::shared_ptr<TermBehavior<T>> m_termBehavior;

    public:
        Term(Term<T> a, Term<T> b, std::function<T(T, T)> f) :
            m_termBehavior(std::shared_ptr<CombinedTermBehavior<T>>(new CombinedTermBehavior<T>(a.getBehavior(), b.getBehavior(), f))) {}
        Term(Term<T> a, std::function<T(T)> f) :
            m_termBehavior(std::shared_ptr<ModifiedTermBehavior<T>>(new ModifiedTermBehavior<T>(a.getBehavior(), f))) {}
        ~Term() {}

        static Term<T> CreateConstTerm(T val)
        {
            return Term<T>(std::shared_ptr<ConstTermBehavior<T>>(new ConstTermBehavior<T>(val)));
        }

        static Term<T> CreateVariableTerm(unsigned int idx)
        {
            return Term<T>(std::shared_ptr<VariableTermBehavior<T>>(new VariableTermBehavior<T>(idx)));
        }

        T substitute(const std::vector<T> &values) const
        {
            return m_termBehavior->substitute(values);
        }

        std::vector<T> substitute(const std::vector<std::vector<T>> &valuesVec) const
        {
            std::vector<T> substVec;
            for (auto & values : valuesVec)
                substVec.push_back(m_termBehavior->substitute(values));

            return substVec;
        }

        // various assignment operators
        Term<T>& operator+=(const Term<T> &rhs)
        {
            m_termBehavior = std::shared_ptr<CombinedTermBehavior<T>>(new CombinedTermBehavior<T>(m_termBehavior, rhs.getBehavior(), std::plus<T>()));
            return *this;
        }

        Term<T>& operator+=(const T &val)
        {
            m_termBehavior = std::shared_ptr<ModifiedTermBehavior<T>>(new ModifiedTermBehavior<T>(m_termBehavior, [val](const T &a) { return a + val; }));
            return *this;
        }

        Term<T>& operator-=(const Term<T> &rhs)
        {
            m_termBehavior = std::shared_ptr<CombinedTermBehavior<T>>(new CombinedTermBehavior<T>(m_termBehavior, rhs.getBehavior(), std::minus<T>()));
            return *this;
        }

        Term<T>& operator-=(const T &val)
        {
            m_termBehavior = std::shared_ptr<ModifiedTermBehavior<T>>(new ModifiedTermBehavior<T>(m_termBehavior, [val](const T &a) { return a - val; }));
            return *this;
        }

        Term<T>& operator*=(const Term<T> &rhs)
        {
            m_termBehavior = std::shared_ptr<CombinedTermBehavior<T>>(new CombinedTermBehavior<T>(m_termBehavior, rhs.getBehavior(), std::multiplies<T>()));
            return *this;
        }

        Term<T>& operator*=(const T &val)
        {
            m_termBehavior = std::shared_ptr<ModifiedTermBehavior<T>>(new ModifiedTermBehavior<T>(m_termBehavior, [val](const T &a) { return a * val; }));
            return *this;
        }

        Term<T>& operator/=(const Term<T> &rhs)
        {
            m_termBehavior = std::shared_ptr<CombinedTermBehavior<T>>(new CombinedTermBehavior<T>(m_termBehavior, rhs.getBehavior(), std::divides<T>()));
            return *this;
        }

        Term<T>& operator/=(const T &val)
        {
            m_termBehavior = std::shared_ptr<ModifiedTermBehavior<T>>(new ModifiedTermBehavior<T>(m_termBehavior, [val](const T &a) { return a / val; }));
            return *this;
        }

    private:
        Term() = delete;
        Term(std::shared_ptr<TermBehavior<T>> tb) : m_termBehavior(tb) {}

        std::shared_ptr<TermBehavior<T>> getBehavior() const { return m_termBehavior; }
    };


    // -----------------------------------------------------------
    // substitute a bunch of terms for one vector at once
    // -----------------------------------------------------------
    template <typename T>
    std::vector<T> substitute(std::vector<T> values, std::vector<Term<T>> terms)
    {
        std::vector<T> substVec;
        for (auto & t : terms)
            substVec.push_back(t.substitute(values));

        return substVec;
    }

    // -----------------------------------------------------------
    // substitute a bunch of terms for various vectors at once
    // -----------------------------------------------------------
    template <typename T>
    std::vector<std::vector<T>> substitute(const std::vector<std::vector<T>> &valuesVec,
        const std::vector<Term<T>> &terms)
    {
        std::vector<std::vector<T>> substVec;
        for (auto & values : valuesVec)
            substVec.push_back(substitute(values, terms));

        return substVec;
    }


    // -----------------------------------------------------------
    // some predefined operators to make usage of
    // terms easier and intuitive
    // -----------------------------------------------------------

    // -----------------------------------------------------------
    // sign of a term
    // -----------------------------------------------------------
    template <typename T>
    Term<T> operator+(const Term<T> &a)
    {
        return a;
    }

    template <typename T>
    Term<T> operator-(const Term<T> &a)
    {
        return Term<T>(a, [](const T &a) { return -a; });
    }

    // -----------------------------------------------------------
    // addition of two terms, or a term and a constant
    // -----------------------------------------------------------
    template <typename T>
    Term<T> operator+(const Term<T> &a, const Term<T> &b)
    {
        return Term<T>(a, b, std::plus<T>());
    }

    template <typename T>
    Term<T> operator+(const Term<T> &a, const T &val)
    {
        return Term<T>(a, [val](const T &a) { return a + val; });
    }

    template <typename T>
    Term<T> operator+(const T &val, const Term<T> &b)
    {
        return Term<T>(b, [val](const T &b) { return val + b; });
    }

    // -----------------------------------------------------------
    // subtraction of two terms, or a term and a constant
    // -----------------------------------------------------------
    template <typename T>
    Term<T> operator-(const Term<T> &a, const Term<T> &b)
    {
        return Term<T>(a, b, std::minus<T>());
    }

    template <typename T>
    Term<T> operator-(const Term<T> &a, const T &val)
    {
        return Term<T>(a, [val](const T &a) { return a - val; });
    }

    template <typename T>
    Term<T> operator-(const T &val, const Term<T> &b)
    {

        return Term<T>(b, [val](const T &b) { return val - b; });
    }

    // -----------------------------------------------------------
    // multiplication of two terms, or a term and a constant
    // -----------------------------------------------------------
    template <typename T>
    Term<T> operator*(const Term<T> &a, const Term<T> &b)
    {
        return Term<T>(a, b, std::multiplies<T>());
    }

    template <typename T>
    Term<T> operator*(const Term<T> &a, const T &val)
    {
        return Term<T>(a, [val](const T &a) { return a*val; });
    }

    template <typename T>
    Term<T> operator*(const T &val, const Term<T> &b)
    {
        return Term<T>(b, [val](const T &b) { return val * b; });
    }

    // -----------------------------------------------------------
    // division of two terms, or a term and a constant
    // -----------------------------------------------------------
    template <typename T>
    Term<T> operator/(const Term<T> &a, const Term<T> &b)
    {
        return Term<T>(a, b, std::divides<T>());
    }

    template <typename T>
    Term<T> operator/(const Term<T> &a, const T &val)
    {
        return Term<T>(a, [val](const T &a) { return a / val; });
    }

    template <typename T>
    Term<T> operator/(const T &val, const Term<T> &b)
    {
        return Term<T>(b, [val](const T &b) { return val / b; });
    }

}
