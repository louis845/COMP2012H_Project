#include "math/double/Double.h"

Double::Double(double val) : Field(RingType::DOUBLE), val(val){}

const Ring* Double::addImpl(const Ring* r) const{

}

const Ring* Double::multImpl(const Ring* r) const{

}

const Ring* Double::minusImpl(const Ring* r) const{

}

const Field* Double::divImpl(const Ring* r) const{

}



const Field* Double::invert() const{

}