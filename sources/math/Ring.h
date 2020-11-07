#ifndef MATH_RING_H
#define MATH_RING_H

#include "debug.h"
#include <string>

enum RingType{
    //Special type is for the zero element and the one element. Make sure to add comparison in the operator==
    SPECIAL_ZERO,SPECIAL_ONE,
    DOUBLE, 
};

/**
 * DO NOT use this class or it's subclasses directly for computation! Wrap it using R{}. See R.cpp and R.h
*/
class Ring{
public:
    virtual ~Ring();
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
    virtual std::string to_string() const=0;
    /**
    * Console output, but signed
    */
    virtual std::string to_signed_string() const=0;

    /**
    * Latex output
    */
    virtual std::string to_latex() const=0;
    /**
    * Latex output, signed
    */
    virtual std::string to_signed_latex() const=0;

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

    /**
     * Partial inverse, this is same as computing 
     * a*this+r=1, and returning a (r is the remainder).
    */
    virtual const Ring* invert () const = 0;

    virtual const Ring* negate() const = 0;

    friend class R;

    friend class ZeroElmt;

    friend class OneElmt;
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

    std::string to_string() const;
    
    std::string to_signed_string() const;

    std::string to_latex() const;

    std::string to_signed_latex() const;

    const Ring* invert() const override;

    const Ring* negate() const override;

    friend class R;
    friend class Field;
};

/**
 * Inclusion of this header should guarantee the inclusion of R.h (since Ring impls must be used with R)
 * But R.h requires the definition of Ring, so it must be included after the definitions.
*/
#include "math/R.h"

#endif