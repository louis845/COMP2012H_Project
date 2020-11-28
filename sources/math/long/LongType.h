#ifndef LONG_LONG_TYPE_H
#define LONG_LONG_TYPE_H
#define PRIMITIVE_LONG_TYPE mpz_wrapper
// #define PRIMITIVE_LONG_TYPE long long
#define CONVERT_TO_STRING(val) ( val.get_str() )
// #define CONVERT_TO_STRING std::to_string(val)
#endif
