#include "math/Ring.h"

using namespace std;
Ring::Ring(RingType t): type(t){}

bool Ring::is_type_compatible(const Ring& other) const{
    return type==other.type;
}