// -----------------------------------------------------------
// LogicalExpressionBehavior classes
// Responsibility: Philipp Paier
//
// DESCRIPTION:
//
//
// -----------------------------------------------------------

#pragma once

#include "Term.h"

#include <memory>
#include <vector>

namespace tc
{

template <typename T> class CCombinedTermExpressionBehavior;
template <typename T> class CSingleTermExpressionBehavior;
template <typename T> class CModifiedExpressionBehavior;
template <typename T> class CCombinedExpressionBehavior;
template <typename T> class CLogicalExpression;

template <typename T>
class CLogicalExpressionBehavior
{
    friend class CModifiedExpressionBehavior<T>;
    friend class CCombinedExpressionBehavior<T>;
    friend class CLogicalExpression<T>;

public:
    virtual ~CLogicalExpressionBehavior(void){}

protected:
    CLogicalExpressionBehavior(void){}

private:
	virtual bool evaluate(const std::vector<T> &values) const = 0;
};

template <typename T>
class CCombinedTermExpressionBehavior :
    public CLogicalExpressionBehavior<T>
{
    friend class CModifiedExpressionBehavior<T>;
    friend class CCombinedExpressionBehavior<T>;
    friend class CLogicalExpression<T>;

private:
    CTerm<T> m_atom1;
    CTerm<T> m_atom2;
    std::function<bool(T, T)> m_comparer;

public:
    virtual ~CCombinedTermExpressionBehavior(void){}

private:
    CCombinedTermExpressionBehavior(void) {}
    CCombinedTermExpressionBehavior(const CTerm<T> &a1, const CTerm<T> &a2, std::function<bool(T, T)> f) :
        m_atom1(a1), m_atom2(a2), m_comparer(f) {}
    virtual bool evaluate(const std::vector<T> &values) const
    {
        return m_comparer(m_atom1.subst(values), m_atom2.subst(values));
    }
};

template <typename T>
class CSingleTermExpressionBehavior :
    public CLogicalExpressionBehavior<T>
{
    friend class CModifiedExpressionBehavior<T>;
    friend class CCombinedExpressionBehavior<T>;
    friend class CLogicalExpression<T>;

private:
    CTerm<T> m_atom;
    std::function<bool(T)> m_comparer;

public:
    virtual ~CSingleTermExpressionBehavior(void){}

private:
    CSingleTermExpressionBehavior(void) {}
    CSingleTermExpressionBehavior(const CTerm<T> &a, std::function<bool(T)> f) :
        m_atom(a), m_comparer(f) {}
    virtual bool evaluate(const std::vector<T> &values) const
    {
        return m_comparer(m_atom.substitute(values));
    }
};


template <typename T>
class CModifiedExpressionBehavior :
    public CLogicalExpressionBehavior<T>
{
    friend class CCombinedExpressionBehavior<T>;
    friend class CLogicalExpression<T>;

private:
    std::shared_ptr<CLogicalExpressionBehavior<T>> m_expr;
    std::function<bool(bool)> m_modifier;

public:
    virtual ~CModifiedExpressionBehavior(void){}

private:
    CModifiedExpressionBehavior(void) {}
    CModifiedExpressionBehavior(std::shared_ptr<CLogicalExpressionBehavior<T>> e, std::function<bool(bool)> f) :
        m_expr(e), m_modifier(f) {}
    virtual bool evaluate(const std::vector<T> &values) const
    {
        return m_modifier(m_expr->evaluate(values));
    }
};


template <typename T>
class CCombinedExpressionBehavior :
    public CLogicalExpressionBehavior<T>
{
    friend class CModifiedExpressionBehavior<T>;
    friend class CLogicalExpression<T>;

private:
    std::shared_ptr<CLogicalExpressionBehavior<T>> m_expr1;
    std::shared_ptr<CLogicalExpressionBehavior<T>> m_expr2;
    std::function<bool(bool, bool)> m_combiner;

public:
    virtual ~CCombinedExpressionBehavior(void){}

private:
    CCombinedExpressionBehavior(void) {}
    CCombinedExpressionBehavior(std::shared_ptr<CLogicalExpressionBehavior<T>> e1,
                            std::shared_ptr<CLogicalExpressionBehavior<T>> e2,
                            std::function<bool(bool, bool)> f) :
                            m_expr1(e1), m_expr2(e2), m_combiner(f) {}
    virtual bool evaluate(const std::vector<T> &values) const
    {
        return m_combiner(m_expr1->evaluate(values), m_expr2->evaluate(values));
    }
};

}

