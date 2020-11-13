# Math basic types
* Polynomial, Fraction, Long, LongComplex, Double, DoubleComplex (implementations, inherits from Ring)
* R, RF (wrappers)
## Wrappers
* R - General ring type, checks for type compatibility when doing operations/comparison with another R
** Promotes type (see below) when they are compatible, but not exactly equal.
* RF - General ring type (fast), does not check for type compatibility, assumes they are EXACTLY EQUAL
** Does not promote type, hence operations/comparison have to be elements of EXACTLY EQUAL types.
* Both has overloaded operators + - * / etc..
* Comparison => <= > < == DOES NOT compare their value (how do you compare complex? not ordered field!)
* Comparison compares Euclidean funcion (polynomial degree, absolute value).
## Implementations
* IMPORTANT Do not directly use these classes, wrap them with R or RF!
* Polynomial over a field, created by passing an array of R.
** Removes leading zeros and promotes type if compatible.
** Copies the array and stores the copy internally
** Deletes the internal array in destructor
* Fraction Field over Euclidean domain, pass two values over/under.
** Promotes type if compatible, automatically divides by GCD.
** Pass by reference, but copies the content and stores internally
** Attributes are not pointers anyway, internal values will be deleted.
* Long, LongComplex represents Integers and Gaussian integers
* Double, DoubleComplex (discouraged from use)
## R,RF creation and memory management
* R,RF both stores one Ring pointer
* Has copy/move assign/constr, destructor to free the internal Ring pointer
* On creation does not copy the Ring*. For example,
<pre>
RF a{new Fraction{RF{new Long{4}}, RF{new Long{3}}}};
</pre>
is valid since the memory locations of newly allocated Ring objects are directly passed into R, without
copying, so we do not have to delete them outside
## Type promotion and compatibility
* Long<LongComplex, Double<DoubleComplex (obvious)
* Fraction and polynomial wraps a type into a larger type
* e.g. FRACTION LONG > LONG, FRACTION POLYNOMIAL FRACTION LONG > FRACTION LONG (compatible)
* e.g. FRACTION LONG < FRACTION COMPLEXIFY LONG (compatible)
* e.g. FRACTION COMPLEXIFY LONG ??? POLYNOMIAL FRACTION LONG (can't compare, not compatible!)
* e.g. POLYNOMIAL DOUBLE ??? FRACTION LONG (can't compare, not compatible!)
* Compatbility check: use is_type_compatible
* A,B are compatible if and only if A.get_type() contains B.get_type() or B.get_type() contains A.get_type()
* Promotion: make sure that type of A>type of B (check by is_type_subset)
* Promotion steps:
<pre>
R a,b;
a.get_type().is_type_subset(b.get_type()) //if true then a can be promoted to b
R c=b.promote(a) // promote a to match the type of b. a would not be changed, return value to c
</pre>
Actually R class automatically promotes if needed, so a*b is possible even if a.get_type().is_type_subset(b.get_type())
## Complex promotion
* We expect FRACTION COMPLEXIFY LONG and POLYNOMIAL FRACTION LONG to be able to do operations in some cases
* Why not take POLYNOMIAL FRACTION COMPLEXIFY LONG in both cases?
* Use bool R::complexify_if_needed(a,b), pass by reference, changes value a,b if needed and possible to complexify so that type is compatible.
<pre>
R a; //assume type FRACTION COMPLEXIFY LONG
R b; //assume type POLYNOMIAL FRACTION LONG
complexify_if_needed(a,b); 
//now a is FRACTION COMPLEXIFY LONG, b is POLYNOMIAL FRACTION COMPLEXIFY LONG
</pre>
Notice that the types are not to be promoted to be equal.
