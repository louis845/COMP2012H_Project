#include "math/long/mpz_wrapper.h"

using namespace std;

mpz_wrapper::mpz_wrapper(const long& l){
    mpz_init_set_si(value,l);
}

mpz_wrapper::mpz_wrapper(const string& s){
    mpz_init_set_str(value, s.c_str(), 10);
}

mpz_wrapper::~mpz_wrapper(){
    mpz_clear(value);
}

mpz_wrapper::mpz_wrapper(const mpz_wrapper& w){
    mpz_init_set(value, w.value);
}

mpz_wrapper::mpz_wrapper(mpz_wrapper&& w){
    mpz_init(value);
    mpz_swap(value,w.value);
}

mpz_wrapper& mpz_wrapper::operator=(const mpz_wrapper& w){
    if(&w != this)
        mpz_set(value, w.value);
    return *this;
}

mpz_wrapper& mpz_wrapper::operator=(mpz_wrapper&& w){
    mpz_swap(value, w.value);
    return *this;
}

string mpz_wrapper::get_str() const{
    char* str = new char[mpz_sizeinbase(value, 10) + 2];
	mpz_get_str(str, 10, value);
	string s{ str };
	delete[] str;
    return s;
}

mpz_wrapper mpz_wrapper::operator+ (const mpz_wrapper& o) const{
    mpz_wrapper val(0);
    mpz_add(val.value, value, o.value);
    return val;
}

mpz_wrapper mpz_wrapper::operator- (const mpz_wrapper& o) const{
    mpz_wrapper val(0);
    mpz_sub(val.value, value, o.value);
    return val;
}

mpz_wrapper mpz_wrapper::operator* (const mpz_wrapper& o) const{
    mpz_wrapper val(0);
    mpz_mul(val.value, value, o.value);
    return val;
}

mpz_wrapper mpz_wrapper::operator/ (const mpz_wrapper& o) const{
    mpz_wrapper val(0);
    mpz_tdiv_q(val.value, value, o.value);
    return val;
}

mpz_wrapper mpz_wrapper::operator% (const mpz_wrapper& o) const{
    mpz_wrapper val(0);
    mpz_tdiv_r(val.value, value, o.value);
    return val;
}

bool mpz_wrapper::operator== (const mpz_wrapper& o) const{
    return mpz_cmp(value,o.value)==0;
}

bool mpz_wrapper::operator!= (const mpz_wrapper& o) const{
    return mpz_cmp(value,o.value)!=0;
}

bool mpz_wrapper::operator> (const mpz_wrapper& o) const{
    return mpz_cmp(value,o.value)>0;
}

bool mpz_wrapper::operator< (const mpz_wrapper& o) const{
    return mpz_cmp(value,o.value)<0;
}

bool mpz_wrapper::operator>= (const mpz_wrapper& o) const{
    return mpz_cmp(value,o.value)>=0;
}

bool mpz_wrapper::operator<= (const mpz_wrapper& o) const{
    return mpz_cmp(value,o.value)<=0;
}

bool mpz_wrapper::operator== (const long& l) const{
    return mpz_cmp_si(value,l)==0;
}

bool mpz_wrapper::operator!= (const long& l) const{
    return mpz_cmp_si(value,l)!=0;
}

bool mpz_wrapper::operator> (const long& l) const{
    return mpz_cmp_si(value,l)>0;
}

bool mpz_wrapper::operator< (const long& l) const{
    return mpz_cmp_si(value,l)<0;
}

bool mpz_wrapper::operator>= (const long& l) const{
    return mpz_cmp_si(value,l)>=0;
}

bool mpz_wrapper::operator<= (const long& l) const{
    return mpz_cmp_si(value,l)<=0;
}

void mpz_wrapper::operator++() {
    mpz_add_ui(value, value, 1);
}

void mpz_wrapper::operator--() {
    mpz_sub_ui(value, value, 1);
}

mpz_wrapper mpz_wrapper::operator-() const{
    mpz_wrapper val(0);
    mpz_neg(val.value,value);
    return val;
}

mpz_wrapper abs(const mpz_wrapper& a){
    mpz_wrapper val(0);
    mpz_abs(val.value,a.value);
    return val;
}

mpz_wrapper gcd(const mpz_wrapper& a,const mpz_wrapper& b){
    mpz_wrapper val(0);
    mpz_gcd(val.value,a.value,b.value);
    return val;
}

mpz_wrapper operator/(long l,const mpz_wrapper& b){
    mpz_wrapper val(l);
    mpz_tdiv_q(val.value,val.value,b.value);
    return val;
}
