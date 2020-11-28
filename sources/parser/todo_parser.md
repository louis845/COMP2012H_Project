# Parser
1. If expression is of type double - use other packages
2. If expression is integer type (or fraction or polynomial etc..), convert expression into RF**

## Create integer (long)
<pre>
R val=R{ new Long{value} };
</pre>

## Create complex integer
<pre>
R val=R{ new LongComplex{real, imaginary} );
</pre>

## Create Fraction
Aim: Create fraction representing over/under
<pre>
R over, under;
R result;
if(R::complexify_if_needed(over, under)){ //Note that this may change the values of over, under.
    if(under.is_zero()){
        //divide by zero here
    }else{
        result=R{ new Fraction{ over, under }};
    }
}else{
    //type not compatible here
}
</pre>

Note that the two R's stored inside the Fraction will be automatically converted to the appropriate type, and the the fraction will be reduced by GCD automatically.

## Create Monic Polynomial
Aim: Create polynomial given the degree and the coefficient of monic polynomial.
1. Example: Create monic polynomial, degree=5, coeff=(4+3i). This means (4+3i)t^5.
<pre>
R coeff;
int degree;

R result; //store monic polynomial here

if(!coeff.is_field()){
    R one=coeff.promote_one(); //Change to field if it is not
    coeff=R{new Fraction{ coeff, one}};
}

R* arr=new R[degree+1];
arr[degree]=coeff;

result=R{ new Polynomial{arr, degree+1} };

delete[] arr;
</pre>

## Binary operations
### Add subtract multiply:
<pre>
R front, back;
R result;
if(R::complexify_if_needed(front, back)){ //Note that this may change the values of over, under.
    result = front+back; //Use + - * 
}else{
    //Type not compatible here
}
</pre>
### Division
<pre>
R front, back;
R result;

if(back.is_zero()){
    //Divide by zero
}else{
    if(!back.is_field()){
        R one=back.promote_one();
        back=R{new Fraction{back,one}};
    }
    if(R::complexify_if_needed(front, back)){ //Note that this may change the values of over, under.
        result = front/back;
    }else{
        //Type not compatible here
    }
}
</pre>

## Create matrix
Create a two-dimensional dynamically allocated array. Use the RF class (assumes same types, faster computation).
### Direct conversion operator from R to RF
<pre>
R value;
RF value2=value; //Direct conversion with same underlying type, and same value
</pre>
### Matrix creation
<pre>
RF **matrix; //Two dimensional array
int rows, cols;
/**
* Your code to assign values to the matrix, and rows cols
*/

if(RF::ensure_types_equal(matrix, rows, cols)){ //This function directly changes the values of the entries of the matrix, so no need to delete
    // OK
}else{
    // Matrix has incompatible types, tell user
}
</pre>

# Parser
## How to display a single mathematical expression (result)
<pre>
StepsHistory *steps=new StepsHistory();
steps->addStep(new StepText{html_string}); //done, pass to return, no need to dealloc since addStep does not copy the given object
</pre>
For example, to display quadratic equation
<pre>
StepsHistory *steps=new StepsHistory();
steps->addStep(new StepText{"$$x=\\frac{-b \\pm \\sqrt{b^2-4ac}}{2a}$$"}); //done, pass to return
</pre>
## My suggestions on what to return
Returns a struct containing an enum indicating the operation, and some other pointers. I think this is possible
<pre>
struct ParserReturn{
    RF** matrix;
    int rows,cols;
    StepsHistory *steps;
    enum class ParserResultType{
        //...
    }
    ParserResultType type;
}
</pre>
## Enum types
1. Linear operation types with steps (see below). In here StepsHistory* steps=nullptr
2. Linear operation without steps (direct result), NOT_R_OPER. In here matrix=nullptr
3. Cannot use both! (two interpretations above does not work), ERROR_OPER
## Possible return values, possibly return a struct, with the data saved accordingly to the return type
1. R** with row/col, if can be interpreted as R**
2. Direct result. Return a newly allocated StepsHistory*, which can display the HTML
3. Return nothing, the returned pointers can be uninitialized (preferrably nullptr)

## Linear operations types (may add more later)
Create an enum/int or something to represent the operation to be done. 
* RREF
* RCEF (reduced col echelon form)
* Solve linear system eq
* Find left/right/inverse of matrix (these three are the same operation)
* Characteristic polynomial
* Orthogonalization (Gram-Schmidt process)
