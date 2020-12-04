# Docs for Parser

## Use the OCR Function

You can use a screen capture as your input if OCR is enabled.

If you want to use the OCR function, prepare youself with an account of Mathpix Snip. You can find more information about how to get the API key on their [official website](https://mathpix.com/ocr).

If you do have one, you can simply input them at the login interface. Note that this application will NOT store any of your credentials.

## Manual Input

If you do not have the access to OCR function, you can always manually input the expressions. 

### Input Conventions

The input format largely follows the [AsciiMath](http://asciimath.org/#syntax) syntax. Here is a quick summary of the major syntax

  * basic binary operators: 
    * +,- remain unchanged
    * single asterisk "*" is dot product: $\cdot$
    * two asterisk " ** " will be asterisk $*$ in AsciiMath
    * "xx" is cross product: $\times$
    * double slash "//" is division: $/$
    * single slash "/" is now fraction i.e. `/frac{}{}` in LaTeX
    * superscript ^ remains unchanged
    * "%" is parsed but will be treated as ERROR during calculations
    * assume multiplication between two adjacent operands: $xy = x*y$
    * division or fraction is only effective on the next operand: $3/xy$ will be interpreted as $3 / x * y$
  * matrices
    * matrices: [[a, b], [c, d]] now yields to $\left[ \begin{smallmatrix} a & b \\ c & d \\ \end{smallmatrix} \right]$
    * round brackets matrices: ([a, b], [c , d]) now yields to $\left( \begin{smallmatrix} a & b \\ c & d \\ \end{smallmatrix} \right)$
    * "{}" are interchangeable with "[]": {{a, b}, {c ,d}} has the same effect as [[a, b], {c, d}]
    * however, note that the inner rows of the matrix MUST be enclosed in square brackets, each row should have the same number of entries, seperated by commas
  * functions
    * please refer to the evaluation sector or the full [token list](#token-list)
  * constants, numbers and special symbols
    * $\pi$, $e$ and $i$ have their usual meaning, and $I$ is indentity matrix as usual
    * you can use newline to separate your linear equations in [linear system mode](), in other modes the newline has the same effect as any other whitespace characters
    * you can use integers and decimals with scientific notation: $-1.34e-5, 23E6$ are valid. As convention, please do not put decimal at the exponential part.
  * variables and greek letters
    * you can use any alphabet characters or words with any length of subscripts as the name of the variables, as long as they are NOT used as a reserved word for functions of constants.
    * example: $x$ $X$ $variable$ $my_{variable_x}$ $x_1$ $a_{1_2}$ $X_\alpha$ are all valid variable names
    * we support some but not all greek letters: $\alpha\beta\theta\lambda\mu\phi\varphi\omega$, you can simply use their name to represent them, or check the [token list](#token-list) if you like. Note that their capital forms are not supported
    * you can add single quotes "'" to your variables, but they must appear before subscripts, if any


## Calculation

### Overview

The evaluation of the input expression is based on two math libraries, one is designed by ourselves and the other is an opensource linear algebra library [Armadillo](http://arma.sourceforge.net/).

The two libraries focus on different scenarios. Our R library aims to provide support for abstract operations, therefore it can handle polynomials and matrices with unknown parameters and perform linear operations on them. It has no precision issues and can give exact solutions. R also provides step-by-step functions for linear algebra operations so that users can have a better grasp of the computation if they are linear algebra learners.

On the other hand, Armadillo is shipped with a whole bunch of linear operations including decompositions, factorisations, inverses and equation solvers with high performance. It support basically all common elementary functions but as a result is limited to the floating point number precision. It also cannot demonstrate the intermediate steps if used out-of-box.

### Mode Selection

There are four general modes of calculation. You can either choose to let the application automatically detect which computational engine to be used, or you can force using one of them.

In addition, there is a special linear system mode designed to facilitate the input of linear systems.

### Auto Dectection

The automatic dectection follows these rules:
* if decimals or scientific constants are detected, Armadillo will be a prior choice
* if variables are found, R will be used
* if functions that R does not support occur, Armadillo will be used

However, 
* if the number of variables is greater than 1, both engines will return error. You may need to assign values to redundant variables
* if rules above collide, you need to modify your input or force the application to use on of the engine
* R class can deal with some floating point numbers by converting them into fractions but with compromised precision
* if scientific notation occurs and is beyond the range of built-in `long` type, neither engine will be able handle it. You will see a related error message printed on the screen
* without scientific notation, R will use `mpz_wrapper` to handle integers but Armadillo is limited to `double` and `long`

It is possible that after explicitly choosing one of the engines, error still occurs. You may want to check the error message and correct the input or check the supported operation list to see whether the operation is supported.

### Dedicated Linear Systems Mode

Since converting linear systems to augmented matrices is not as natural as input the equations directly, there is a linear system mode provided.

You can input or scan a linear system using this mode, but there are a few restrictions:
  * make sure each linear equation starts a newline or separated by "\\"
  * each variable should be separated, i.e. not bounded by a same pair of parentheses, e.g. $3x + (4 - 5i)y + 6z(7 * 8 / 9) = 10a - 42$ is a valid linear equation
  * however, $3(x + y + z) = 0$ is not, although it is linear
  * variable name "t" is reserved for representing the parameter: $tx = 1$ will give a solution $x = 1/t$

The answer will be given with corresponding variable names arranged in ascending order, which may be different from their order in the linear system. A hint for the ordering of the variables in the output will also be given in the interpretation of the AsciiMath input so that you can figure out which variable the column of the augmented matrix in the step-by-step history corresponds to.

### Supported Operations

#### General Operations supported by both

* $+~-~*~\cdot~\times$ ^ , note that currently $*~\cdot~\times$ are not distinguished, and exponential only supports natural numbers in R, complex numbers in Armadillo
* currently we do not support superscript with special meanings e.g. $A^{-1}~B^*~C^{T}~D^+$. It may be added soon, but so far superscript is only for exponentiation
  
#### Linear Operations supported by R with step-by-step

* $rref(A)$: row reduce
* $inv(A)$: inverse
* $det(A)$: determinant
* $orth(A)$: orthogonalization
* $solve(A)$: solve augmented matrix
* $charpoly(A)$: find characteristic polynomial

#### Functions and Linear Operations in Armadillo

* trigonometry: $\sin(x)~\cos(x)~\tan(x)~\arcsin(x)~\arccos(x)~\arctan(x)$
* $\exp(x)~\ln(x)~\sqrt x~\sqrt[a]{x}$
* $rref(A)$: row reduce
* $orth(A)$: give the orthnormal basis of the column space of a given matrix
* $ker(A)$: find orthonomal basis of the kernel of given matrix
* $trans(A)$: matrix transpose
* $norm(A)$: norm of a vector/matrix<sup>[*](#norm-info)</sup>
* $det(A)$: determinant
* $trace(A)~or~tr(A)$: trace
* $inv(A)$: inverse
* $pinv(A)$: pseudo-inverse
* $solve(A, B(optional))$: solve<sup>[^](#solve-info)</sup> an augmented matrix $A$ or a linear system $Ax=B$
* $eigen(A)$: eigen decomposition
* $schur(A$: Schur decomposition
* $qr(A)$: QR decomposition
* $svd(A)$: singular value decomposition

<span id="norm-info"> 
[*]: p = 2 for both vectors and matrices

<span id="solve-info">

[^]: if no exact solution is found, Armadillo will automatically attempt to find approximated solutions


## A Quick Overview of the Parser & Evaluator Part

This part of the application handles the OCR request, interpertation of the input(either form OCR or manual input), and also the evaluation. **It is just a simple summary here, for more detailed information, please go and check the source code of each files.**

It consists of the following classes:

* `\sources\parser\tokens.h`: The base class `Token`, which is the basis of the lexer and the parser. It is a data type to store the properties and raw values of each token. It has several derived classes representing different token types.
  
* `\sources\parser\lexer.h`: The class `Lexer` will handle raw input and tokenize it into a token stream. It uses regex to classify each token
  
* `\sources\utils\math_wrapper.h`: Contains two classes `ROperand` and `ArmaOperand`, the former is a wrapper for our own `R` class so as to unify matrices and scalars as a single data type to facilitate development. It also overload some operators. The `ArmaOperand` is more or less the same. The rest of the header file also includes some handy inline functions for instantiating `R` objects.
  
* `\sources\parser\expr_ast.h`: The classes related to the abstract syntax tree used in the parser. It contains a base class `ExprAST` and several derived classes, each representing different kinds of node on the AST. The member funtions of each derived classes also handles the evaluation of its own type of tree node. As a result, the whole AST can be constructed, destructed, or evaluated with ease by traversing through the tree.
* In addition, the `expr_ast.h` also contains an `Info` struct for the communications between the parser and the GUI.
  
* `\sources\parser\parser.h`: Core part of the parser. It includes a giant class `Parser`. It is basically an LL(1) parser with some features of the LL(*) parser. It is also shipped with some dedicated parser components for special cases, variable assignment and management functions, the driver functions for evaluation, and a whole bunch of exception handling.
  
* `\sources\utils\ocr_api.h`: The `Ocr` class encapsulates the OCR request functions. It manages the credentials, converts the image input, constructs the HTTP requests, does the post and parse the response. Finally it returns the OCR results in LaTeX and AsciiMath format.

For more information, please check the [readme] under `/sources/parser` as well as the comment in source code.





## Parser class API

1. Simply initialize it with the AsciiMath string or user input string you want to parse. You can reset the input anytime to avoid instantiate `Parser` multiple times.

```c++
    // note that this input should be in AsciiMath format
    // either manually input by the user or got via OCR
    string input;               
    Parser parser(input);
    parser.reset_input(new_input);
```

2. Invoke `parse(int engine_type=0)` to parse the input. `engine_type=0` (default) for auto dectection, `engine_type=1` forces the parser to use `R` class to evaluate the expression, `engine_type=2` forces using Armadillo.

    `parse()` will return `const Info&`, of which the copy constructor and `operator=` are deleted, therefore you may need to do deep copy on the `R**` if neccessary.
    
    The structure of class `Info` are as follows

```c++
    struct Info         // for communications between parser and gui / step-by-step functions
    {
        Info() = default;
        Info(const Info&) = delete;
        Info& operator=(const Info&) = delete;
        ~Info();

        bool success{true};
        std::string err_msg;
        int engine_chosen{0};           // user chosen engine, 0 = auto, 1 = R, 2 = Armadillo, 3 = linear system
        int engine_used{1};             // 1 for using R class, 2 for using Armadillo, 3 for parsing linear system
        bool float_exists{false};
        bool var_exists{false};
        bool func_exists{false};        // true only if function that R cannot handle appears

        std::vector<std::pair<Token::TokName, R**>> parsed_mat;    
        // contains all matrices and corresponding operators the input for step-by-step functions
        // N.B. the 2D arrays are deep-copied hence needed to be deallocated
        // after calling the step-by-step functions

        std::vector<std::pair<int, int>>    mat_size;  // corresponding sizes, row first

        std::string interpreted_input{""};  // an AsciiMath format string of the interpreted input
        std::string eval_result{""};        // computation result in LaTeX format
                                            // N.B. both are raw strings and are NOT enclosed by delimiters
                                            // i.e. $$ for LaTeX and ` for AsciiMath

        void clear();
        void addMat(ROperand operand, Token::TokName op);
    };
```
* `bool success` indicates the overall parsing & evaluation status, `true` if the both process encounter no expections.
* `string err_msg` is the error message passed by the parser as a hint for the user.
* `int engine_used` is the engine used, 1 for R and 2 for Armadillo, you may also need to show it on the screen.
* `parsed_mat` contains all the matrices in user's input and their operators and `mat_size` stores the corresponding sizes. Check the full list of `enum class Token::TokName` in [tokens.h](token.h). However, the `parsed_mat` will only contain part of them:

```c++
    TokName::RREF       // self explanatory
    TokName::DET
    TokName::INV
    TokName::SOLVE
    TokName::CHAR_POLY  // characteristic polynomial
    TokName::ORTH       // orthogonalization
    TokName::NA         // user does not specify any linear operation
```

* `string interpreted_input` contains interpreted AsciiMath input in case you want to render for user to do self-checking.
* `string eval_result` contains computation result in LaTeX.
* all the rest are irrelevant unless you want to show more information to user when error occurs.

The automatic dectection follows these rules:
* if floating point numbers or scientific constants are detected, Armadillo is preferable
* if variables are found, R will be used
* if functions that R does not support occur, Armadillo will be used

However, 
* if the number of variables is greater than 1, both engines will return error.
* if rules above collide, will ask the user to force using one of the engine
* R class can deal with some floating point numbers by converting them into fractions but with compromised precision
* if scientific notation occurs and is beyond the range of built-in `long` type, neither engine will be able handle it
* but without scientific notation, R will use `mpz_wrapper` to handle integral and Armadillo is limited to `double` and `long`

3. the variable assignment function
   
   Currently the parser class has the following accessors and modifiers concerning variables.

```c++
    const Info& parse(int engine_type=0, bool save=false, const std::string& var_name="Ans");
    bool assignVar(const std::string& var_name, const std::string& raw, int type);
    bool assignVar(const std::string& var_name, const ROperand& value);
    bool modifyName(const std::string& ori_name, const std::string& new_name);
    bool eraseVar(const std::string& var_name);
    std::map<std::string, std::string> retrieve_var() const;
```

* When invoking `parse()`, if `save=true`, the final result will be saved to a variable with name `var_name`, which can be reused in the future calculations.
* `assignVar()` can be used to assign values to variables. Either raw AsciiMath string or `ROperand` type can be passed as argument, however, when using literals, engine type must be specified (either 1 or 2).
* `modifyName()` allows users to modify the name of an existing variable, but the new name must be different from any existing ones.
* `eraseVar()` is used to erase an existing variable by passing its name, note that a variable appeared in the expression can NOT be erased no matter whether a value has been assigned to it.
* `retrieve_var()` will return a `map` from all the variable names to their values in LaTeX.


## Token List

The tokens that lexer can recognize are listed here with their properties and regex.

Check [asciimath](http://asciimath.org/) for the meaning of the tokens. Note that the order of the tokens in the list matters.

Please update the list if new tokens are needed.

```text
literal     regex           token name          token type
-----------------------------------------------------------------
,           [,]             COMMA               DELIM
\\          [\\\\]          ENDL                DELIM
^           [\^]            SUP                 OPERATOR
_           [_]             SUB                 OPERATOR
xx          [xx]            CROSS               OPERATOR
**          [\*\*]          AST                 OPERATOR
*           [\*]            CDOT                OPERATOR
//          [\/\/]          DIV                 OPERATOR
/           [\/]            FRAC                OPERATOR
%           [%]             PERCENT             OPERATOR
+           [\+]            PLUS                OPERATOR
-           [-]             MINUS               OPERATOR

|           [\|]            TEXTBAR             OPERATOR
(           [\(]            LP                  OPERATOR
)           [\)]            RP                  OPERATOR
[           [\[]            LSB                 OPERATOR
]           []]             RSB                 OPERATOR
{           [\{]            LCB                 OPERATOR
}           [\}]            RCB                 OPERATOR
=           [=]             EQUAL               OPERATOR
abs         [abs]           ABS                 OPERATOR
norm        [norm]          NORM                OPERATOR
sqrt        [sqrt]          SQRT                OPERATOR
root        [root]          ROOT                OPERATOR
sin         [sin]
cos         [cos]
tan         [tan]
sec         [sec]
csc         [csc]
cot         [cot]
arcsin      [arcsin]
arccos      [arccos]
arctan      [arctan]
exp         [exp]
log         [log]
ln          [ln]
det         [det]
rank        [rank]
orth        [orth|col|ran]
ker         [ker]
trans       [trans]
mod         [mod]
gcd         [gcd]
lcm         [lcm]
min         [min]
max         [max]
trace       [(trace|tr)]
RREF        [rref]
inv         [inv]
pinv        [pinv]
eigen       [eigen]
schur       [schur]
qr          [qr]
svd         [svd]
solve       [solve]
roots       [roots]
charpoly    [charpoly]

alpha       [alpha]                             IDENTIFIER
beta        [beta]                              IDENTIFIER
theta       [theta]                             IDENTIFIER
lambda      [lambda]                            IDENTIFIER
mu          [mu]                                IDENTIFIER
phi         [phi]                               IDENTIFIER
varphi      [varphi]                            IDENTIFIER
omega       [omega]                             IDENTIFIER
x y z       [a-zA-Z]        VARIABLE            IDENTIFIER

e           [e]             E                   NUM
i           [i]             I                   NUM
I           [I]             IDENTITY_MATRIX     NUM
pi          [pi]            PI                  NUM
1 2.3 -4    [(\+|-)?(\.[0-9]+|[0-9]+\.?[0-9]*)((e|E)(\+|-)?[0-9]+)?]
                            INTEGRAL|FLOAT      NUM
```

## Parser Grammar

The full grammar rule for parser is listed here.

Reference to [asciimath renderer](https://github.com/asciimath/asciimathml/blob/master/ASCIIMathML.js).

```text
number      ::= [(\+|-)?(\.[0-9]+|[0-9]+\.?[0-9]*)((e|E)(\+|-)?[0-9]+)?] | constants
variable    ::= [A-Za-z] | greek letters
function    ::= functions
binop       ::= + | - | * | ** | xx | // | / | ^ | %

numberexpr      ::= number

parenexpr       ::= '(' expression ')'
                ::= '|' expression '|'

bracketexpr     ::= '[' expression ( ',' expression )* ']'

identifierexpr  ::= variable
                ::= function ( number | variable )
                ::= function '(' expression ( ',' expression )* ')'
                ::= function matrixexpr

matrixexpr      ::= '[' bracketexpr ( ',' bracketexpr )* ']'
                ::= '(' bracketexpr ( ',' bracketexpr )* ')' 

primaryexpr ::= numberexpr
            ::= parenexpr
            ::= matrixexpr
            ::= identifierexpr

binoprhs    ::= ( binop ( '+' | '-' )? primaryexpr | ( '+' | '-' )? primaryexpr )*

expression  ::= ( '+' | '-' )? primaryexpr binoprhs


special case: decomposition
            ::= ( schur | qr | eigen | svd ) matrixexpr

special case: linear system
            ::= ( expression = expression )+

            where the expression must a list of subexpressions seperated by ( '+' | '-' )
            within each subexpression, there must be at most on variable, and it cannot be
            the right child of a division BinaryExprAst node, or within a matrix or any 
            non-linear functions


/*  Original parser grammar from AsciiMath renderer

    Parsing ASCII math expressions with the following grammar
    v ::= [A-Za-z] | greek letters | numbers | other constant symbols
    u ::= sqrt | text | bb | other unary symbols for font commands
    b ::= frac | root | stackrel         binary symbols
    l ::= ( | [ | { | (: | {:            left brackets
    r ::= ) | ] | } | :) | :}            right brackets
    S ::= v | lEr | uS | bSS             Simple expression
    I ::= S_S | S^S | S_S^S | S          Intermediate expression
    E ::= IE | I/I                       Expression
    Each terminal symbol is translated into a corresponding mathml node.
*/
```
