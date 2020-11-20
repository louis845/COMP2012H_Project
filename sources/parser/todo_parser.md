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
