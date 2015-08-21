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

    template <typename T> class CombinedTermExpressionBehavior;
    template <typename T> class SingleTermExpressionBehavior;
    template <typename T> class ModifiedExpressionBehavior;
    template <typename T> class CombinedExpressionBehavior;
    template <typename T> class LogicalExpression;

    template <typename T>
    class LogicalExpressionBehavior
    {
        friend class ModifiedExpressionBehavior < T > ;
        friend class CombinedExpressionBehavior < T > ;
        friend class LogicalExpression < T > ;

    public:
        virtual ~LogicalExpressionBehavior(void){}

    protected:
        LogicalExpressionBehavior(void){}

    private:
        virtual bool evaluate(const std::vector<T> &values) const = 0;
    };

    template <typename T>
    class CombinedTermExpressionBehavior :
        public LogicalExpressionBehavior < T >
    {
        friend class ModifiedExpressionBehavior < T > ;
        friend class CombinedExpressionBehavior < T > ;
        friend class LogicalExpression < T > ;

    private:
        Term<T> m_atom1;
        Term<T> m_atom2;
        std::function<bool(T, T)> m_comparer;

    public:
        virtual ~CombinedTermExpressionBehavior(void){}

    private:
        CombinedTermExpressionBehavior(void) = delete;
        CombinedTermExpressionBehavior(const Term<T> &a1, const Term<T> &a2, std::function<bool(T, T)> f) :
            m_atom1(a1), m_atom2(a2), m_comparer(f) {}
        virtual bool evaluate(const std::vector<T> &values) const
        {
            return m_comparer(m_atom1.subst(values), m_atom2.subst(values));
        }
    };

    template <typename T>
    class SingleTermExpressionBehavior :
        public LogicalExpressionBehavior < T >
    {
        friend class ModifiedExpressionBehavior < T > ;
        friend class CombinedExpressionBehavior < T > ;
        friend class LogicalExpression < T > ;

    private:
        Term<T> m_atom;
        std::function<bool(T)> m_comparer;

    public:
        virtual ~SingleTermExpressionBehavior(void){}

    private:
        SingleTermExpressionBehavior(void) = delete;
        SingleTermExpressionBehavior(const Term<T> &a, std::function<bool(T)> f) :
            m_atom(a), m_comparer(f) {}
        virtual bool evaluate(const std::vector<T> &values) const
        {
            return m_comparer(m_atom.substitute(values));
        }
    };


    template <typename T>
    class ModifiedExpressionBehavior :
        public LogicalExpressionBehavior < T >
    {
        friend class CombinedExpressionBehavior < T > ;
        friend class LogicalExpression < T > ;

    private:
        std::shared_ptr<LogicalExpressionBehavior<T>> m_expr;
        std::function<bool(bool)> m_modifier;

    public:
        virtual ~ModifiedExpressionBehavior(void){}

    private:
        ModifiedExpressionBehavior(void) = delete;
        ModifiedExpressionBehavior(std::shared_ptr<LogicalExpressionBehavior<T>> e, std::function<bool(bool)> f) :
            m_expr(e), m_modifier(f) {}
        virtual bool evaluate(const std::vector<T> &values) const
        {
            return m_modifier(m_expr->evaluate(values));
        }
    };


    template <typename T>
    class CombinedExpressionBehavior :
        public LogicalExpressionBehavior < T >
    {
        friend class ModifiedExpressionBehavior < T > ;
        friend class LogicalExpression < T > ;

    private:
        std::shared_ptr<LogicalExpressionBehavior<T>> m_expr1;
        std::shared_ptr<LogicalExpressionBehavior<T>> m_expr2;
        std::function<bool(bool, bool)> m_combiner;

    public:
        virtual ~CombinedExpressionBehavior(void){}

    private:
        CombinedExpressionBehavior(void) = delete;
        CombinedExpressionBehavior(std::shared_ptr<LogicalExpressionBehavior<T>> e1,
            std::shared_ptr<LogicalExpressionBehavior<T>> e2,
            std::function<bool(bool, bool)> f) :
            m_expr1(e1), m_expr2(e2), m_combiner(f) {}
        virtual bool evaluate(const std::vector<T> &values) const
        {
            return m_combiner(m_expr1->evaluate(values), m_expr2->evaluate(values));
        }
    };

}

