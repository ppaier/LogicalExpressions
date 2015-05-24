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


template <typename T> class CTerm;
template <typename T> class CConstTermBehavior;
template <typename T> class CVariableTermBehavior;
template <typename T> class CModifiedTermBehavior;
template <typename T> class CCombinedTermBehavior;

template <typename T>
class CTermBehavior
{
    friend class CModifiedTermBehavior<T>;
    friend class CCombinedTermBehavior<T>;
    friend class CTerm<T>;

public:
    virtual ~CTermBehavior() {}

protected:
    CTermBehavior() {}

private:
    virtual T substitute(const std::vector<T> &values) const = 0;
};


template <typename T>
class CConstTermBehavior :
    public CTermBehavior < T >
{
    friend class CModifiedTermBehavior<T>;
    friend class CCombinedTermBehavior<T>;
    friend class CTerm<T>;

private:
    T m_dConst;

public:
    virtual ~CConstTermBehavior(void) {}

private:
    CConstTermBehavior() {}
    CConstTermBehavior(T dConstVal) : m_dConst(dConstVal) {}
    virtual T substitute(const std::vector<T> &values) const { return m_dConst; }
};


template <typename T>
class CVariableTermBehavior :
    public CTermBehavior<T>
{
    friend class CModifiedTermBehavior<T>;
    friend class CCombinedTermBehavior<T>;
    friend class CTerm<T>;

private:
    unsigned int m_nIdx;

public:
    virtual ~CVariableTermBehavior(void){}

private:
    CVariableTermBehavior() {}
    CVariableTermBehavior(unsigned int idx) : m_nIdx(idx) {}
    virtual T substitute(const std::vector<T> &values) const
    {
        if (m_nIdx < values.size())
            return values[m_nIdx];
        else
            throw(std::out_of_range("Index out of bounds for subst in CVarAtom."));
    }
};

template <typename T>
class CModifiedTermBehavior :
    public CTermBehavior<T>
{
    friend class CCombinedTermBehavior<T>;
    friend class CTerm<T>;

private:
    std::shared_ptr<CTermBehavior<T>> m_term;
    std::function<T(T)> m_modifier;

public:
    virtual ~CModifiedTermBehavior(void){}

private:
    CModifiedTermBehavior() {}
    CModifiedTermBehavior(std::shared_ptr<CTermBehavior<T>> t, std::function<T(T)> f) :
        m_term(t), m_modifier(f) {}
    virtual T substitute(const std::vector<T> &values) const
    {
        return m_modifier(m_term->substitute(values));
    }
};


template <typename T>
class CCombinedTermBehavior :
    public CTermBehavior<T>
{
    friend class CModifiedTermBehavior<T>;
    friend class CTerm<T>;

private:
    std::shared_ptr<CTermBehavior<T>> m_term1;
    std::shared_ptr<CTermBehavior<T>> m_term2;
    std::function<T(T, T)> m_combiner;

public:
    virtual ~CCombinedTermBehavior(void){}

private:
    CCombinedTermBehavior() {}
    CCombinedTermBehavior(std::shared_ptr<CTermBehavior<T>> t1, std::shared_ptr<CTermBehavior<T>> t2, std::function<T(T, T)> f) :
        m_term1(t1), m_term2(t2), m_combiner(f) {}
    virtual T substitute(const std::vector<T> &values) const
    {
        return m_combiner(m_term1->substitute(values), m_term2->substitute(values));
    }
};
