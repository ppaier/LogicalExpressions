// -----------------------------------------------------------
// LogicalAtomBehavior classes
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


template <typename T> class CLogicalAtom;
template <typename T> class CConstAtomBehavior;
template <typename T> class CVarAtomBehavior;
template <typename T> class CModifiedAtomBehavior;
template <typename T> class CCombAtomBehavior;

template <typename T>
class CLogicalAtomBehavior
{
    friend class CModifiedAtomBehavior<T>;
    friend class CCombAtomBehavior<T>;
    friend class CLogicalAtom<T>;

public:
    virtual ~CLogicalAtomBehavior() {}

protected:
    CLogicalAtomBehavior() {}

private:
    virtual T substitute(const std::vector<T> &values) const = 0;
};


template <typename T>
class CConstAtomBehavior :
    public CLogicalAtomBehavior < T >
{
    friend class CModifiedAtomBehavior<T>;
    friend class CCombAtomBehavior<T>;
    friend class CLogicalAtom<T>;

private:
    T m_dConst;

public:
    virtual ~CConstAtomBehavior(void) {}

private:
    CConstAtomBehavior() {}
    CConstAtomBehavior(T dConstVal) : m_dConst(dConstVal) {}
    virtual T substitute(const std::vector<T> &values) const { return m_dConst; }
};


template <typename T>
class CVarAtomBehavior :
    public CLogicalAtomBehavior<T>
{
    friend class CModifiedAtomBehavior<T>;
    friend class CCombAtomBehavior<T>;
    friend class CLogicalAtom<T>;

private:
    unsigned int m_nIdx;

public:
    virtual ~CVarAtomBehavior(void){}

private:
    CVarAtomBehavior() {}
    CVarAtomBehavior(unsigned int idx) : m_nIdx(idx) {}
    virtual T substitute(const std::vector<T> &values) const
    {
        if (m_nIdx < values.size())
            return values[m_nIdx];
        else
            throw(std::out_of_range("Index out of bounds for subst in CVarAtom."));
    }
};

template <typename T>
class CModifiedAtomBehavior :
    public CLogicalAtomBehavior<T>
{
    friend class CCombAtomBehavior<T>;
    friend class CLogicalAtom<T>;

private:
    std::shared_ptr<CLogicalAtomBehavior<T>> m_atom;
    std::function<T(T)> m_modifier;

public:
    virtual ~CModifiedAtomBehavior(void){}

private:
    CModifiedAtomBehavior() {}
    CModifiedAtomBehavior(std::shared_ptr<CLogicalAtomBehavior<T>> a, std::function<T(T)> f) :
        m_atom(a), m_modifier(f) {}
    virtual T substitute(const std::vector<T> &values) const
    {
        return m_modifier(m_atom->substitute(values));
    }
};


template <typename T>
class CCombAtomBehavior :
    public CLogicalAtomBehavior<T>
{
    friend class CModifiedAtomBehavior<T>;
    friend class CLogicalAtom<T>;

private:
    std::shared_ptr<CLogicalAtomBehavior<T>> m_atom1;
    std::shared_ptr<CLogicalAtomBehavior<T>> m_atom2;
    std::function<T(T, T)> m_combiner;

public:
    virtual ~CCombAtomBehavior(void){}

private:
    CCombAtomBehavior() {}
    CCombAtomBehavior(std::shared_ptr<CLogicalAtomBehavior<T>> a1, std::shared_ptr<CLogicalAtomBehavior<T>> a2, std::function<T(T, T)> f) :
        m_atom1(a1), m_atom2(a2), m_combiner(f) {}
    virtual T substitute(const std::vector<T> &values) const
    {
        return m_combiner(m_atom1->substitute(values), m_atom2->substitute(values));
    }
};
