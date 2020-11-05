#ifndef MATH_RING_H
#define MATH_RING_H

#include "debug.h"
#include "math/R.h"
#include <string>

enum RingType{
    //Error type is when a Ring via Ring(). This should be impossible as there are pure virtual.
    ERROR
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
    virtual Ring* copy() const=0;

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
    virtual Ring* addImpl (const Ring* r) const=0;
    /**
    * Usual -
    */
    virtual Ring* minusImpl (const Ring* r) const=0;
    /**
    * Usual multiplication
    */
    virtual Ring* multImpl (const Ring* r) const=0;
    /**
    * The 'integral' part of division, express this=div*k+r. Returns k.
    */
    virtual Ring* divImpl (const Ring* div) const=0;
    /**
     * The 'remainder' part of division, see operator*. In fields this should always be 0
    */
    virtual Ring* remainderImpl (const Ring* div) const=0;

    friend class R;
};

#endif