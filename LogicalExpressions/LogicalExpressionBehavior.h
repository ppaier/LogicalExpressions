// -----------------------------------------------------------
// Logical Expression interface class
// Responsibility: Philipp Paier
//
// DESCRIPTION: 
// A logical expression must implement an evaluate method, 
// that return a bool given a vector of values
// -----------------------------------------------------------

#pragma once

#include "LogicalAtom.h"

#include <memory>
#include <vector>

template <typename T>
class CLogicalExpressionBehavior
{
public:
    CLogicalExpressionBehavior(void){}
    virtual ~CLogicalExpressionBehavior(void){}

	virtual bool evaluate(const std::vector<T> &values) const = 0;

};


template <typename T>
class CAtomExpressionBehavior :
    public CLogicalExpressionBehavior<T>
{
public:
    CAtomExpressionBehavior(const CLogicalAtom<T> &a1, const CLogicalAtom<T> &a2, std::function<bool(T, T)> f) :
        m_atom1(a1), m_atom2(a2), m_comparer(f) {}

    virtual ~CAtomExpressionBehavior(void){}

    virtual bool evaluate(const std::vector<T> &values) const
    {
        return m_comparer(m_atom1.subst(values), m_atom2.subst(values));
    }

private:
    CLogicalAtom<T> m_atom1;
    CLogicalAtom<T> m_atom2;
    std::function<bool(T, T)> m_comparer;

    CAtomExpressionBehavior(void) {}
};

template <typename T>
class CModifiedExpressionBehavior :
    public CLogicalExpressionBehavior<T>
{
public:
    CModifiedExpressionBehavior(std::shared_ptr<CLogicalExpressionBehavior<T>> e, std::function<bool(bool)> f) :
        m_expr(e), m_modifier(f) {}

    virtual ~CModifiedExpressionBehavior(void){}

    virtual bool evaluate(const std::vector<T> &values) const
    {
        return m_modifier(m_expr->evaluate(values));
    }

private:
    std::shared_ptr<CLogicalExpressionBehavior<T>> m_expr;
    std::function<bool(bool)> m_modifier;

    CModifiedExpressionBehavior(void) {}
};


template <typename T>
class CCombExpressionBehavior :
    public CLogicalExpressionBehavior<T>
{
public:
    CCombExpressionBehavior(std::shared_ptr<CLogicalExpressionBehavior<T>> e1, 
                            std::shared_ptr<CLogicalExpressionBehavior<T>> e2, 
                            std::function<bool(bool, bool)> f) :
                            m_expr1(e1), m_expr2(e2), m_combiner(f) {}

    virtual ~CCombExpressionBehavior(void){}

    virtual bool evaluate(const std::vector<T> &values) const
    {
        return m_combiner(m_expr1->evaluate(values), m_expr2->evaluate(values));
    }

private:
    std::shared_ptr<CLogicalExpressionBehavior<T>> m_expr1;
    std::shared_ptr<CLogicalExpressionBehavior<T>> m_expr2;
    std::function<bool(bool, bool)> m_combiner;

    CCombExpressionBehavior(void) {}
};

