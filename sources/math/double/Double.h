#ifndef MATH_DOUBLE_DOUBLE_H
#define MATH_DOUBLE_DOUBLE_H
/**
 * Implementation of double, actually this just uses the primitive type.
*/
class Double : public Field final{

public:
    double val;

    Double(double);

    virtual Ring* addImpl (const Ring* r) const=0;
    
    virtual Ring* minusImpl (const Ring* r) const=0;
    
    virtual Ring* multImpl (const Ring* r) const=0;
    
    virtual Ring* divImpl (const Ring* div) const=0;
    
    virtual Ring* remainderImpl (const Ring* div) const=0;
};
#endif