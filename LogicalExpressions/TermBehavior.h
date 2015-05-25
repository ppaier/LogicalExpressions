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
    template <typename T> class CTerm;
    template <typename T> class CConstTermBehavior;
    template <typename T> class CVariableTermBehavior;
    template <typename T> class CModifiedTermBehavior;
    template <typename T> class CCombinedTermBehavior;


    // -----------------------------------------------------------
    // interface class for the behavior of a term
    // -----------------------------------------------------------
    template <typename T>
    class CTermBehavior
    {
        friend class CModifiedTermBehavior < T > ;
        friend class CCombinedTermBehavior < T > ;
        friend class CTerm < T > ;

    public:
        virtual ~CTermBehavior() {}

    protected:
        CTermBehavior() {}

    private:
        virtual T substitute(const std::vector<T> &values) const = 0;
    };


    // -----------------------------------------------------------
    // constant term behavior (e.g. 3)
    // -----------------------------------------------------------
    template <typename T>
    class CConstTermBehavior :
        public CTermBehavior < T >
    {
        friend class CModifiedTermBehavior < T > ;
        friend class CCombinedTermBehavior < T > ;
        friend class CTerm < T > ;

    private:
        T m_dConst;

    public:
        virtual ~CConstTermBehavior(void) {}

    private:
        CConstTermBehavior() = delete;
        CConstTermBehavior(T dConstVal) : m_dConst(dConstVal) {}
        virtual T substitute(const std::vector<T> &values) const { return m_dConst; }
    };


    // -----------------------------------------------------------
    // variable term behavior e.g. ( vector[3] representing x3 )
    // -----------------------------------------------------------
    template <typename T>
    class CVariableTermBehavior :
        public CTermBehavior < T >
    {
        friend class CModifiedTermBehavior < T > ;
        friend class CCombinedTermBehavior < T > ;
        friend class CTerm < T > ;

    private:
        size_t m_nIdx;

    public:
        virtual ~CVariableTermBehavior(void){}

    private:
        CVariableTermBehavior() = delete;
        CVariableTermBehavior(size_t idx) : m_nIdx(idx) {}
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
    class CModifiedTermBehavior :
        public CTermBehavior < T >
    {
        friend class CCombinedTermBehavior < T > ;
        friend class CTerm < T > ;

    private:
        std::shared_ptr<CTermBehavior<T>> m_term;
        std::function<T(T)> m_modifier;

    public:
        virtual ~CModifiedTermBehavior(void){}

    private:
        CModifiedTermBehavior() = delete;
        CModifiedTermBehavior(std::shared_ptr<CTermBehavior<T>> t, std::function<T(T)> f) :
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
    class CCombinedTermBehavior :
        public CTermBehavior < T >
    {
        friend class CModifiedTermBehavior < T > ;
        friend class CTerm < T > ;

    private:
        std::shared_ptr<CTermBehavior<T>> m_term1;
        std::shared_ptr<CTermBehavior<T>> m_term2;
        std::function<T(T, T)> m_combiner;

    public:
        virtual ~CCombinedTermBehavior(void){}

    private:
        CCombinedTermBehavior() = delete;
        CCombinedTermBehavior(std::shared_ptr<CTermBehavior<T>> t1, std::shared_ptr<CTermBehavior<T>> t2, std::function<T(T, T)> f) :
            m_term1(t1), m_term2(t2), m_combiner(f) {}
        virtual T substitute(const std::vector<T> &values) const
        {
            return m_combiner(m_term1->substitute(values), m_term2->substitute(values));
        }
    };

}
