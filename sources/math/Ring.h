#ifndef MATH_RING_H
#define MATH_RING_H

#include "debug.h"
#include "math/R.h"
#include <string>

enum RingType{
    //Special type is for the zero element and the one element. Make sure to add comparison in the operator==
    SPECIAL,
    DOUBLE, 
};

/**
 * DO NOT use this class or it's subclasses directly for computation! Wrap it using R{}. See R.cpp and R.h
*/
class Ring{
protected:
    /**
     * Type of the ring.
    */
    const RingType type;

    Ring(RingType);

    /**
     * Operations can only be done with compatible types. Used for debugging.
    */
    virtual bool is_type_compatible(const Ring& other) const final;

    /**
     * ONLY call this function in R.cpp/R.h. The pointer should be immediately assigned to R so it gets properly deallocated with the destructor/copy assignment... of R
    */
    virtual const Ring* copy() const=0;

    /**
    * Console output
    */
    virtual std::string to_string() const;
    /**
    * Console output, but signed
    */
    virtual std::string to_signed_string() const;

    /**
    * Latex output
    */
    virtual std::string to_latex() const;
    /**
    * Latex output, signed
    */
    virtual std::string to_signed_latex() const;

    /**
    * Usual +
    */
    virtual const Ring* addImpl (const Ring* r) const=0;
    /**
    * Usual -
    */
    virtual const Ring* minusImpl (const Ring* r) const=0;
    /**
    * Usual multiplication
    */
    virtual const Ring* multImpl (const Ring* r) const=0;
    /**
    * The 'integral' part of division, express this=div*k+r. Returns k.
    */
    virtual const Ring* divImpl (const Ring* div) const=0;
    /**
     * The 'remainder' part of division, see operator*. In fields this should always be 0
    */
    virtual const Ring* remainderImpl (const Ring* div) const=0;

    virtual bool equalsImpl(const Ring* compare) const=0;

    /**
     * The 'degree' of the element, for example degree of a polynomial, absolute value etc...
    */
    virtual int euclideanFunc() const=0;

    friend class R;
};

/**
 * Special Element Zero class.
*/
class ZeroElmt final : public Ring{
private:
    ZeroElmt();

    const Ring* copy() const override;

    const Ring* addImpl(const Ring* r) const override;

    const Ring* minusImpl(const Ring* r) const override;

    const Ring* multImpl(const Ring* r) const override;

    const Ring* divImpl(const Ring* r) const override;

    const Ring* remainderImpl(const Ring* r) const override;

    bool equalsImpl(const Ring* compare) const override;

    int euclideanFunc() const override;

    friend R;
};

/**
 * Special Element One class.
*/
class OneElmt final : public Ring{
private:
    OneElmt();

    const Ring* copy() const override;

    const Ring* addImpl(const Ring* r) const override;

    const Ring* minusImpl(const Ring* r) const override;

    const Ring* multImpl(const Ring* r) const override;

    const Ring* divImpl(const Ring* r) const override;

    const Ring* remainderImpl(const Ring* r) const override;

    bool equalsImpl(const Ring* compare) const override;

    int euclideanFunc() const override;

    friend R;
};

#endif