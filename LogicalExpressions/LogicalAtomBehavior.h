#pragma once
#include <vector>
#include <memory>
#include <functional>
#include <stdexcept>

template <typename T>
class CLogicalAtomBehavior
{

public:
    CLogicalAtomBehavior() {}
    virtual ~CLogicalAtomBehavior() {}

    virtual T subst(const std::vector<T> &values) const = 0;

};


template <typename T>
class CConstAtomBehavior :
    public CLogicalAtomBehavior < T >
{
public:
    CConstAtomBehavior(T dConstVal) : m_dConst(dConstVal) {}
    virtual ~CConstAtomBehavior(void) {}

    virtual T subst(const std::vector<T> &values) const { return m_dConst; }

private:
    CConstAtomBehavior() {}
    T m_dConst;
};


template <typename T>
class CVarAtomBehavior :
    public CLogicalAtomBehavior<T>
{
public:
    CVarAtomBehavior(unsigned int idx) : m_nIdx(idx) {}
    virtual ~CVarAtomBehavior(void){}

    virtual T subst(const std::vector<T> &values) const
    {
        if (m_nIdx < values.size())
            return values[m_nIdx];
        else
            throw(std::out_of_range("Index out of bounds for subst in CVarAtom."));
    }

private:
    CVarAtomBehavior() {}
    unsigned int m_nIdx;

};

template <typename T>
class CModifiedAtomBehavior :
    public CLogicalAtomBehavior<T>
{
public:

    CModifiedAtomBehavior(std::shared_ptr<CLogicalAtomBehavior<T>> a, std::function<T(T)> f) :
        m_atom(a), m_modifier(f) {}

    virtual ~CModifiedAtomBehavior(void){}

    virtual T subst(const std::vector<T> &values) const
    {
        return m_modifier(m_atom->subst(values));
    }

private:
    std::shared_ptr<CLogicalAtomBehavior<T>> m_atom;
    std::function<T(T)> m_modifier;

    CModifiedAtomBehavior() {}
};


template <typename T>
class CCombAtomBehavior :
    public CLogicalAtomBehavior<T>
{
public:

    CCombAtomBehavior(std::shared_ptr<CLogicalAtomBehavior<T>> a1, std::shared_ptr<CLogicalAtomBehavior<T>> a2, std::function<T(T, T)> f) :
        m_atom1(a1), m_atom2(a2), m_combiner(f) {}

    virtual ~CCombAtomBehavior(void){}

    virtual T subst(const std::vector<T> &values) const
    {
        return m_combiner(m_atom1->subst(values), m_atom2->subst(values));
    }

private:
    std::shared_ptr<CLogicalAtomBehavior<T>> m_atom1;
    std::shared_ptr<CLogicalAtomBehavior<T>> m_atom2;
    std::function<T(T, T)> m_combiner;

    CCombAtomBehavior() {}
};
