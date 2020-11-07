#include "math/double/Double.h"
#include <sstream>
#include <iomanip>

int Double::PRECISION=3; //3dp precision

Double::Double(double val) : Field(RingType::DOUBLE), val(val){}

const Ring* Double::addImpl(const Ring* r) const{
    return new Double{val+(dynamic_cast<const Double*>(r)->val)};
}

const Ring* Double::multImpl(const Ring* r) const{
    return new Double{val*(dynamic_cast<const Double*>(r)->val)};
}

const Ring* Double::minusImpl(const Ring* r) const{
    return new Double{val-(dynamic_cast<const Double*>(r)->val)};
}

const Field* Double::divImpl(const Ring* r) const{
    return new Double{val/(dynamic_cast<const Double*>(r)->val)};
}

const Field* Double::invert() const{
    return new Double{1/val};
}

const Double* Double::negate() const{
    return new Double{-val};
}

const Double* Double::copy() const{
    return new Double{val};
}

bool Double::equalsImpl(const Ring* other) const{
    return val == (static_cast<const Double*>(other))->val;
}

string Double::to_string() const{
    ostringstream str;
    str << setprecision(PRECISION);
    str << val;
    return str.str();
}

string Double::to_signed_string() const{
    ostringstream str;
    if(val>=0){
        str << "+";
    }
    str << setprecision(PRECISION);
    str << val;
    return str.str();
}

string Double::to_latex() const{
    ostringstream str;
    str << setprecision(PRECISION);
    str << val;
    return str.str();
}

string Double::to_signed_latex() const{
    ostringstream str;
    if(val>=0){
        str << "+";
    }
    str << setprecision(PRECISION);
    str << val;
    return str.str();
}