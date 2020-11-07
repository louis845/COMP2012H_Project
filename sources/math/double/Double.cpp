#include "math/double/Double.h"

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

const Double* Double::copy() const{
    return new Double{val};
}

bool Double::equalsImpl(const Ring* other) const{
    return val == (static_cast<const Double*>(other))->val;
}