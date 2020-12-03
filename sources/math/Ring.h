#ifndef MATH_RING_H
#define MATH_RING_H

#include "debug.h"
#include <string>

enum RingType{
    //Special type is for the zero element and the one element. Make sure to add comparison in the operator==
    SPECIAL_ZERO,
    DOUBLE, LONG, FRACTION, POLYNOMIAL, COMPLEXIFY, MOD_FIELD
};

/**
 * Linked list of types. For example, a field of fractions over polynomials over fractions of integers will be 
 * FRACTION -> POLYNOMIAL -> FRACTION -> LONG
 * 
 * This can be used to determine compatibility of binary operations, for example it should be possible to multiply
 * Fraction type with Long type.
 * 
 * The construction of a larger type should use COMPLEXIFY only immediately after a primitive type, and
 * it is expected that only FRACTION and POLYNOMIAL should be used for higher types.
 * 
 * Note that for the POLYNOMIAL functions to work, polynomials should only be constructed over fields.
*/
class NestedRingType final{
private:
    NestedRingType* sub_type;

    RingType current_type;
    int extra_info; //The current type is equal if and only if RingTypes are equal and extra_info are equal

    bool is_complex;
    int no_fraction; //Number of nested fractions in the type.
    int no_polynomial; //Number of nested polynomials in the type.
    

    NestedRingType* deep_copy() const;
public:
    static NestedRingType FRACTION_COMPLEX_LONG;

    /**
     * Initializes sub_type to nullptr, this means that this type does not 'contain' any subrings.
    */
    NestedRingType(const RingType& current_type);

    /**
     * Initializes sub_type to the given pointer, without doing a deep copy of subtype. 
     * Use the default constructor and then set_sub_type to set sub_type to a deep copy.
    */
    NestedRingType(const RingType& current_type, NestedRingType* subtype);

    ~NestedRingType();

    NestedRingType(const NestedRingType&);

    NestedRingType(NestedRingType&&);

    NestedRingType& operator= (const NestedRingType&);

    NestedRingType& operator= (NestedRingType&&);

    /**
     * Checks whether sub_type is not nullptr
    */
    bool has_sub_type() const;

    /**
     * Returns the reference to sub_type. Check has_sub_type before calling this.
    */
    const NestedRingType& get_sub_type() const;

    /**
     * Whether the types are equal, disregarding the subtype.
    */
    bool shallow_equals(const NestedRingType& other) const;

    /**
     * Whether the type are exactly equals. Use Ring::is_type_compatible(const NestedRingType&, const NestedRingType&) for
     * compatibility for binary operations
    */
    bool deep_equals(const NestedRingType& other) const;

    /**
     * Deep deallocates the current subtype (if not nullptr) and deep copies the given subtype to sub_type. Returns *this.
    */
    NestedRingType& set_sub_type(const NestedRingType* subtype);

    /**
     * Sets the extra info of the NestedRingType.
    */
    void set_extra_info(const int& i);

    /**
     * Deep deallocates the current subtype (if not nullptr) and assigns the given subtype to sub_type. DOES NOT deep copy. Returns *this.
    */
    NestedRingType& set_sub_type_no_copy(NestedRingType* subtype);

    const RingType& get_current_type() const;

    std::string to_string() const;

    bool complex() const;
};

//Outputs a nested ring type.
std::ostream& operator<< (std::ostream&, const NestedRingType&);

/**
 * DO NOT use this class or it's subclasses directly for computation! Wrap it using R{}. See R.cpp and R.h. All instances should be dynamically allocated and passed into
 * the wrapper class R.
*/
class Ring{
#if DEBUG_MODE
private:
    int* RING_DEBUG_NUM_CREATE; //Keeps track of the number of R (wrapper) this ring resides in.
#endif
public:
    virtual ~Ring();
    /**
     * Type of the ring.
    */
    const RingType type_shallow;

    /**
     * Binary operations below can only be done with compatible types. Used for debugging and internal operations.
    */
    virtual bool is_type_compatible(const Ring& other) const final;

    /**
     * Shallow equals.
    */
    static bool is_type_compatible_shallow(const NestedRingType&, const NestedRingType&);

    /**
     * Whether it is compatible with binary operations. Our implementation support operations where either
     * A includes B or B includes A. Note that SPECIAL_ZERO is always compatible.
    */
    static bool is_type_compatible(const NestedRingType&, const NestedRingType&);

    /**
     * Returns -2 if they are not compatible, -1 if is_type_subset(b, a) (i.e. the ring of b includes the ring of a)
     * 0 if they are deep equals,
     * 1 if is_type_subset(a, b) (i.e. the ring of a includes the ring of b)
    */
    static int compatibility(const NestedRingType& a, const NestedRingType& b);

    /**
     * Whether the type supset contains the type subset. The inclusion need not be strict.
     * Note that subset==SPECIAL_ZERO always satisfies the inclusion. 
    */
    static bool is_type_subset(const NestedRingType& supset, const NestedRingType& subset);

    /**
     * We do not allow copy and assignment
    */
    Ring(const Ring&) = delete;

    Ring(Ring &&) = delete;

    virtual Ring& operator= (const Ring &) = delete;

    virtual Ring& operator= (Ring &&) = delete;

    /**
     * Gets the detailed type.
    */

    const NestedRingType& get_type() const;

    /**
     * The pointer should be immediately assigned to R so it gets properly deallocated with the destructor/copy assignment... of R
    */
    virtual const Ring* copy() const=0;
    NestedRingType* type;

    virtual bool is_field() const;

    bool is_complex() const;
protected:

    Ring(RingType);

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
     * Console output. The default implementation returns to_string() if it is not one, and returns the sign - for negative one.
    */
    virtual std::string to_leading_coeff() const;

    /**
     * Console output. The default implementation returns to_string() if it is not one, and returns the sign +/- for one and negative one.
    */
    virtual std::string to_coeff() const;

    /**
     * Latex output. The default implementation returns to_string() if it is not one, and returns the sign - for negative one.
    */
    virtual std::string to_latex_leading_coeff() const;

    /**
     * Latex output. The default implementation returns to_string() if it is not one, and returns the sign +/- for one and negative one.
    */
    virtual std::string to_latex_coeff() const;

    /**
     * Indicates whether the display of this element needs brackets (if it is a sum of parts?). Default is false.
    */
    virtual bool needs_bracket() const;

    /**
     * Indicates whether the display of this element needs brackets in Latex (if it is a sum of parts?). Default is false.
    */
    virtual bool needs_bracket_latex() const;

    /**
     * Allocates and promotes the given Ring r to the same value, matching exactly the type of 'this'.
     * Make sure that the given r is a subset of this, given by is_type_subset.
    */
    virtual const Ring* promote(const Ring* const& r) const=0;

    /**
     * BINARY OPERATIONS AND COMPARISON OF RING IMPLEMENTATIONS. For functions that return a Ring* pointer, it is expected a new Ring (or subclasses) should be dynamically allocated.
     * In the implementation in subclasses, it is ensured in the wrapper class R that the ring of the argument is always directly compatiable with the ring of this, given by this->get_type().deep_equals( r->get_type()) )
     * Therefore the pointer can be dynamic_cast ed to the appropriate type, and along with its subtypes
     * 
     * Make sure the type returned is exactly the same (in the sense of deep equals) as this and r.
    */

    /**
    * Usual +. No need to check for SPECIAL_ZERO (can cast type to appropriate type)
    */
    virtual const Ring* addImpl (const Ring* r) const=0;
    /**
    * Usual -. No need to check for SPECIAL_ZERO (can cast type to appropriate type)
    */
    virtual const Ring* minusImpl (const Ring* r) const=0;
    /**
    * Usual multiplication. No need to check for SPECIAL_ZERO (can cast type to appropriate type)
    */
    virtual const Ring* multImpl (const Ring* r) const=0;
    /**
    * The 'integral' part of division, express this=div*k+r. Returns k. No need to check for SPECIAL_ZERO (can cast type to appropriate type)
    */
    virtual const Ring* divImpl (const Ring* div) const=0;
    /**
     * The 'remainder' part of division, see operator*. In fields this should always be 0. No need to check for SPECIAL_ZERO (can cast type to appropriate type)
    */
    virtual const Ring* remainderImpl (const Ring* div) const=0;

    /**
     * Finds both the quotient and remainder. This can be overriden for optimization in algorithms that requires both the remainder and quotient, to avoid computing twice. The default
     * implementation is to compute the divImpl and remainderImpl separately and then pass onto the quot and rem. Note that quot and rem are dynamically allocated in this function.
    */
    virtual void quotAndRemainder(const Ring* div, const Ring*& quot, const Ring*& rem) const;

    /**
     * Whether they are equal. No need to check for SPECIAL_ZERO (can cast to appropriate type)
    */

    virtual bool equalsImpl(const Ring* compare) const=0;

    /**
     * Compares according to the Euclidean function. For integers and Gaussian integers this is just comparing the absolute value
     * For polynomials this is comparing the degree of the polynomial.
     * 
     * Returns: 1 if the Euclidean function of this is larger than compare
     * 0 if they are the same
     * -1 if compare is larger than this
     * 
     *  No need to check for SPECIAL_ZERO (can cast type to appropriate type)
    */
    virtual int euclideanFuncCompare(const Ring* compare) const=0;

    /**
     * Partial inverse, this is same as computing 
     * a*this+r=1, and returning a (r is the remainder).
    */
    virtual const Ring* invert() const = 0;

    virtual const Ring* negate() const = 0;

    /**
     * Dynamically allocates a ring element equal to one with type same as the type of this.
    */
    virtual const Ring* promote_one() const = 0;

    /**
     * Dynamically allocates and returns a complex ring element.
    */
    virtual const Ring* complexify() const = 0;

    /**
     * If type is complex, returns a copy of this. Returns a finite field version of this otherwise, and returns this if field is already finite field.
     * Does not check whether mod is prime. Throws an error if divide by zero occurs.
    */
    virtual const Ring* to_finite_field(int mod) const;

    /**
     * Returns the a newly allocated complex conjugate of this. Returns a newly allocated copy() of itself if 
     * it does not belong to a complex field. In default implementation returns itself. For subclasses override
     * this function to handle possible complex scenarios.
    */
    virtual const Ring* conjugate() const;

    /**
     * Splits the element into a product this*unit=morph, where unit is invertible. Dynamically allocates to the references to the pointers.
    */
    virtual void split_canonical(const Ring*& morph, const Ring*& unit) const=0;

    /**
     * Checks whether the element is invertible.
    */
    virtual bool is_unit() const=0;

    /*
    * Checks whether the element is exactly one.
    */
    virtual bool is_one() const=0;

    /*
    * Checks whether the element is exactly zero.
    */
    virtual bool is_zero() const;

    /*
    * Checks whether the element is exactly negative one.
    */
    virtual bool is_neg_one() const;

    friend class R;

    friend class RF;

    friend class ZeroElmt;
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

    int euclideanFuncCompare(const Ring* compare) const override;

    std::string to_string() const override;
    
    std::string to_signed_string() const override;

    std::string to_latex() const override;

    std::string to_signed_latex() const override;

    std::string to_leading_coeff() const override;

    std::string to_coeff() const override;

    std::string to_latex_leading_coeff() const override;

    std::string to_latex_coeff() const override;

    const Ring* invert() const override;

    const Ring* negate() const override;

    const Ring* promote(const Ring* const& r) const override;

    const Ring* promote_one() const override;

    const Ring* complexify() const override;

    const Ring* to_finite_field(int mod) const override;

    bool is_unit() const override;

    bool is_one() const override;

    bool is_zero() const override;

    bool is_neg_one() const override;

    void split_canonical(const Ring*&,const Ring*&) const override;

    friend class R;
    friend class Field;
};

/**
 * Inclusion of this header should guarantee the inclusion of R.h (since Ring impls must be used with R)
 * But R.h requires the definition of Ring, so it must be included after the definitions.
*/
#include "math/R.h"

#endif