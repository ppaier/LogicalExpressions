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

namespace tc
{

    template <typename T>
    class LogicalExpression final
    {
    private:
        std::shared_ptr<LogicalExpressionBehavior<T>> m_leBehavior;

    public:
        LogicalExpression(Term<T> la1, Term<T> la2, std::function<bool(T, T)> f) :
            m_leBehavior(std::shared_ptr<CombinedTermExpressionBehavior<T>>(new CombinedTermExpressionBehavior<T>(la1, la2, f))) {}
        LogicalExpression(Term<T> la, std::function<bool(T)> f) :
            m_leBehavior(std::shared_ptr<SingleTermExpressionBehavior<T>>(new SingleTermExpressionBehavior<T>(la, f))) {}
        LogicalExpression(LogicalExpression<T> le, std::function<bool(bool)> f) :
            m_leBehavior(std::shared_ptr<ModifiedExpressionBehavior<T>>(new ModifiedExpressionBehavior<T>(le.getBehavior(), f))) {}
        LogicalExpression(LogicalExpression<T> le1, LogicalExpression<T> le2, std::function<bool(bool, bool)> f) :
            m_leBehavior(std::shared_ptr<CombinedExpressionBehavior<T>>(new CombinedExpressionBehavior<T>(le1.getBehavior(), le2.getBehavior(), f))) {}
        ~LogicalExpression() {}

        bool evaluate(const std::vector<T> &values) const
        {
            return m_leBehavior->evaluate(values);
        }

        std::vector<bool> evaluate(const std::vector<std::vector<T>> &valuesVec) const
        {
            std::vector<bool> evalVec;
            for (auto & values : valuesVec)
                evalVec.push_back(m_leBehavior->evaluate(values));

            return evalVec;
        }

    private:
        LogicalExpression() = delete;
        LogicalExpression(std::shared_ptr<LogicalExpressionBehavior<T>> leb) : m_leBehavior(leb) {}
        std::shared_ptr<LogicalExpressionBehavior<T>> getBehavior() const { return m_leBehavior; }
    };


    // -----------------------------------------------------------
    // evaluate a bunch of expressions for one vector at once
    // -----------------------------------------------------------
    template <typename T>
    std::vector<bool> evaluate(const std::vector<T> &values,
        const std::vector<LogicalExpression<T>> &expressions)
    {
        std::vector<bool> evalVec;
        for (auto & e : expressions)
            evalVec.push_back(e.evaluate(values));

        return evalVec;
    }

    // -----------------------------------------------------------
    // evaluate a bunch of expressions for varios vectors at once
    // -----------------------------------------------------------
    template <typename T>
    std::vector<std::vector<bool>> evaluate(const std::vector<std::vector<T>> &valuesVec,
        const std::vector<LogicalExpression<T>> &expressions)
    {
        std::vector<std::vector<bool>> evalVec;
        for (auto & values : valuesVec)
            evalVec.push_back(evaluate(values, expressions));

        return evalVec;
    }



    // -----------------------------------------------------------
    // some predefined comparison operators to make usage of
    // logical expressions easier
    // -----------------------------------------------------------

    // -----------------------------------------------------------
    // standard comparisons of two terms
    // -----------------------------------------------------------
    template <typename T>
    LogicalExpression<T> operator<(Term<T> a, Term<T> b)
    {
        return LogicalExpression<T>(a, b, std::less<T>());
    }

    template <typename T>
    LogicalExpression<T> operator<=(Term<T> a, Term<T> b)
    {
        return LogicalExpression<T>(a, b, std::less_equal<T>());
    }

    template <typename T>
    LogicalExpression<T> operator>(Term<T> a, Term<T> b)
    {
        return LogicalExpression<T>(a, b, std::greater<T>());
    }


    template <typename T>
    LogicalExpression<T> operator>=(Term<T> a, Term<T> b)
    {
        return LogicalExpression<T>(a, b, std::greater_equal<T>());
    }


    template <typename T>
    LogicalExpression<T> operator==(Term<T> a, Term<T> b)
    {
        return LogicalExpression<T>(a, b, std::equal_to<T>());
    }

    template <typename T>
    LogicalExpression<T> operator!=(Term<T> a, Term<T> b)
    {
        return LogicalExpression<T>(a, b, std::not_equal_to<T>());
    }


    // -----------------------------------------------------------
    // standard comparisons of a term and a constant
    // -----------------------------------------------------------
    template <typename T>
    LogicalExpression<T> operator<(Term<T> a, T val)
    {
        return LogicalExpression<T>(a, [val](T a){ return a < val; });
    }

    template <typename T>
    LogicalExpression<T> operator<=(Term<T> a, T val)
    {
        return LogicalExpression<T>(a, [val](T a){ return a <= val; });
    }


    template <typename T>
    LogicalExpression<T> operator>(Term<T> a, T val)
    {
        return LogicalExpression<T>(a, [val](T a){ return a > val; });
    }

    template <typename T>
    LogicalExpression<T> operator>=(Term<T> a, T val)
    {
        return LogicalExpression<T>(a, [val](T a){ return a >= val; });
    }


    template <typename T>
    LogicalExpression<T> operator==(Term<T> a, T val)
    {
        return LogicalExpression<T>(a, [val](T a){ return a == val; });
    }

    template <typename T>
    LogicalExpression<T> operator!=(Term<T> a, T val)
    {
        return LogicalExpression<T>(a, [val](T a){ return a != val; });
    }

    template <typename T>
    LogicalExpression<T> operator<(T val, Term<T> b)
    {
        return LogicalExpression<T>(b, [val](T b){ return val < b; });
    }

    template <typename T>
    LogicalExpression<T> operator<=(T val, Term<T> b)
    {
        return LogicalExpression<T>(b, [val](T b){ return val <= b; });
    }


    template <typename T>
    LogicalExpression<T> operator>(T val, Term<T> b)
    {
        return LogicalExpression<T>(b, [val](T b){ return val > b; });
    }

    template <typename T>
    LogicalExpression<T> operator>=(T val, Term<T> b)
    {
        return LogicalExpression<T>(b, [val](T b){ return val >= b; });
    }


    template <typename T>
    LogicalExpression<T> operator==(T val, Term<T> b)
    {
        return LogicalExpression<T>(b, [val](T b){ return val == b; });
    }

    template <typename T>
    LogicalExpression<T> operator!=(T val, Term<T> b)
    {
        return LogicalExpression<T>(b, [val](T b){ return val != b; });
    }


    // -----------------------------------------------------------
    // basic logical operators for two logical expressions
    // -----------------------------------------------------------
    template <typename T>
    LogicalExpression<T> operator&&(LogicalExpression<T> a, LogicalExpression<T> b)
    {
        return LogicalExpression<T>(a, b, std::logical_and<bool>());
    }

    template <typename T>
    LogicalExpression<T> operator||(LogicalExpression<T> a, LogicalExpression<T> b)
    {
        return LogicalExpression<T>(a, b, std::logical_or<bool>());
    }

    template <typename T>
    LogicalExpression<T> operator==(LogicalExpression<T> a, LogicalExpression<T> b)
    {
        return LogicalExpression<T>(a, b, std::equal_to<bool>());
    }

    template <typename T>
    LogicalExpression<T> operator!=(LogicalExpression<T> a, LogicalExpression<T> b)
    {
        return LogicalExpression<T>(a, b, std::not_equal_to<bool>());
    }

    template <typename T>
    LogicalExpression<T> operator!(LogicalExpression<T> a)
    {
        return LogicalExpression<T>(a, std::logical_not<bool>());
    }

}
