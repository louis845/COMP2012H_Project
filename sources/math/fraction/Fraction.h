#include "math/Field.h"

using namespace std;

class Fraction : public Field {
public:
/**
 * Fraction(over, under). This represents a fraction over/under. 
*/
    Fraction(R,R);
protected:

    const R over;

    const R under;

    const Ring* addImpl (const Ring* r) const override;
    
    const Ring* minusImpl (const Ring* r) const override;
    
    const Ring* multImpl (const Ring* r) const override;
    
    const Field* divImpl (const Ring* div) const override;

    const Field* invert() const override;
    
    const Fraction* copy() const override;

    const Fraction* negate() const override;

    bool equalsImpl(const Ring* other) const override;

    virtual string to_string() const override;

    virtual string to_signed_string() const override;

    virtual string to_latex() const override;

    virtual string to_signed_latex() const override;
};