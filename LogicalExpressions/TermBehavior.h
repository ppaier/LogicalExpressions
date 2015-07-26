// -----------------------------------------------------------
// TermBehavior classes
// Responsibility: Philipp Paier
//
// DESCRIPTION:
//
//
// -----------------------------------------------------------

#pragma once

#include <vector>
#include <memory>
#include <functional>
#include <stdexcept>

namespace tc
{
    // -----------------------------------------------------------
    // necessary forward declarations
    // -----------------------------------------------------------
    template <typename T> class Term;
    template <typename T> class ConstTermBehavior;
    template <typename T> class VariableTermBehavior;
    template <typename T> class ModifiedTermBehavior;
    template <typename T> class CombinedTermBehavior;


    // -----------------------------------------------------------
    // interface class for the behavior of a term
    // -----------------------------------------------------------
    template <typename T>
    class TermBehavior
    {
        friend class ModifiedTermBehavior < T > ;
        friend class CombinedTermBehavior < T > ;
        friend class Term < T > ;

    public:
        virtual ~TermBehavior() {}

    protected:
        TermBehavior() {}

    private:
        virtual T substitute(const std::vector<T> &values) const = 0;
    };


    // -----------------------------------------------------------
    // constant term behavior (e.g. 3)
    // -----------------------------------------------------------
    template <typename T>
    class ConstTermBehavior :
        public TermBehavior < T >
    {
        friend class ModifiedTermBehavior < T > ;
        friend class CombinedTermBehavior < T > ;
        friend class Term < T > ;

    private:
        T m_dConst;

    public:
        virtual ~ConstTermBehavior(void) {}

    private:
        ConstTermBehavior() = delete;
        ConstTermBehavior(T dConstVal) : m_dConst(dConstVal) {}
        virtual T substitute(const std::vector<T> &values) const { return m_dConst; }
    };


    // -----------------------------------------------------------
    // variable term behavior e.g. ( vector[3] representing x3 )
    // -----------------------------------------------------------
    template <typename T>
    class VariableTermBehavior :
        public TermBehavior < T >
    {
        friend class ModifiedTermBehavior < T > ;
        friend class CombinedTermBehavior < T > ;
        friend class Term < T > ;

    private:
        size_t m_nIdx;

    public:
        virtual ~VariableTermBehavior(void){}

    private:
        VariableTermBehavior() = delete;
        VariableTermBehavior(size_t idx) : m_nIdx(idx) {}
        virtual T substitute(const std::vector<T> &values) const
        {
            if (m_nIdx < values.size())
                return values[m_nIdx];
            else
                throw(std::out_of_range("Index out of bounds for substitution in CTerm."));
        }
    };

    // -----------------------------------------------------------
    // modified term behavior (e.g. 3*term)
    // -----------------------------------------------------------
    template <typename T>
    class ModifiedTermBehavior :
        public TermBehavior < T >
    {
        friend class CombinedTermBehavior < T > ;
        friend class Term < T > ;

    private:
        std::shared_ptr<TermBehavior<T>> m_term;
        std::function<T(T)> m_modifier;

    public:
        virtual ~ModifiedTermBehavior(void){}

    private:
        ModifiedTermBehavior() = delete;
        ModifiedTermBehavior(std::shared_ptr<TermBehavior<T>> t, std::function<T(T)> f) :
            m_term(t), m_modifier(f) {}
        virtual T substitute(const std::vector<T> &values) const
        {
            return m_modifier(m_term->substitute(values));
        }
    };


    // -----------------------------------------------------------
    // combined term behavior (e.g. term1+term2)
    // -----------------------------------------------------------
    template <typename T>
    class CombinedTermBehavior :
        public TermBehavior < T >
    {
        friend class ModifiedTermBehavior < T > ;
        friend class Term < T > ;

    private:
        std::shared_ptr<TermBehavior<T>> m_term1;
        std::shared_ptr<TermBehavior<T>> m_term2;
        std::function<T(T, T)> m_combiner;

    public:
        virtual ~CombinedTermBehavior(void){}

    private:
        CombinedTermBehavior() = delete;
        CombinedTermBehavior(std::shared_ptr<TermBehavior<T>> t1, std::shared_ptr<TermBehavior<T>> t2, std::function<T(T, T)> f) :
            m_term1(t1), m_term2(t2), m_combiner(f) {}
        virtual T substitute(const std::vector<T> &values) const
        {
            return m_combiner(m_term1->substitute(values), m_term2->substitute(values));
        }
    };

}
