# Docs for Parser

## Parser class API

1. Simply initialize it with the AsciiMath string or user input string you want to parse. You can reset the input anytime to avoid instantiate `Parser` multiple times.

```c++
    // note that this input should be in AsciiMath format
    // either manually input by the user or got via OCR
    string input;               
    Parser parser(input);
    parser.reset_input(new_input);
```

1. Invoke `parse(int engine_type=0)` to parse the input. `engine_type=0` (default) for auto dectection, `engine_type=1` forces the parser to use `R` class to evaluate the expression, `engine_type=2` forces using Armadillo.

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
        std::exception* err{nullptr};
        int engine_chosen{0};           // user chosen engine, 0 = auto, 1 = R, 2 = Armadillo
        int engine_used{1};             // 1 for using R class, 2 for using Armadillo
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

        std::string warning{""};

        void clear();
        void addMat(ROperand operand, Token::TokName op);
    };
```
* `bool success` indicates the overall parsing & evaluation status, `true` if the both process encounter no expections.
* `std::exception* err` will be set if `success` is false, you can show the `err.what()` to the user for their reference. You can also do RTTI to classify the exception more precisely.

```c++
    typeid(*err) == typeid(std::invalid_argument)   // parsing error
    typeid(*err) == typeid(std::runtime_error)      // unexpected exception, fatal in other words
```

* `int engine_used` is the engine used, 1 for R and 2 for Armadillo, you may also need to show it on the screen.
* `parsed_mat` contains all the matrices in user's input and their operators and `mat_size` stores the corresponding sizes. Check the full list of `enum class Token::TokName` in [tokens.h](token.h). However, the `parsed_mat` will only contain part of them:

```c++
    TokName::RREF       // self explanatory
    TokName::DET
    TokName::INV
    TokName::SOLVE
    TokName::CHAR_POLY
    TokName::ORTH       // orthogonalization
    TokName::NA         // user do not specify a linear operation
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

## Token List

Check [asciimath](http://asciimath.org/) for the meaning of the tokens. Note that the order of the tokens in the list matters.

Please update the list if new tokens are needed.

```text
literal     regex           token name          token type
-----------------------------------------------------------------
,           [,]             COMMA               DELIM

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
dim         [dim]
ran         [(R|r)an]
col         [(C|c)ol]
ker         [(K|k)er]
mod         [mod]
gcd         [gcd]
lcm         [lcm]
min         [min]
max         [max]
trace       [(trace|tr)]
RREF        [(RREF|rref)]

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
                            NUMERICAL           NUM
```

## Parser Grammar

Reference to [asciimath renderer](https://github.com/asciimath/asciimathml/blob/master/ASCIIMathML.js).

```text
number      ::= [(\+|-)?(\.[0-9]+|[0-9]+\.?[0-9]*)((e|E)(\+|-)?[0-9]+)?] | constants
variable    ::= [A-Za-z] | greek letters
function    ::= functions
binop       ::= + | - | * | ** | xx | // | / | ^ | %

numberexpr      ::= number

parenexpr       ::= '(' expression ( ',' expression )* ')'
                ::= '[' expression ( ',' expression )* ']'
                ::= '{' expression ( ',' expression )* '}'

absexpr         ::= '|' expression '|'

identifierexpr  ::= variable
                ::= function ( number | variable )
                ::= function '(' expression ( ',' expression )* ')'
                ::= function matrixexpr

matrixexpr      ::= '[' parenexpr ( ',' parenexpr )* ']'
                ::= '(' parenexpr ( ',' parenexpr )* ')'
                ::= '{' parenexpr ( ',' parenexpr )* '}'

primaryexpr ::= numberexpr
            ::= parenexpr
            ::= matrixexpr
            ::= identifierexpr

binoprhs    ::= ( binop ( '+' | '-' )? primaryexpr | ( '+' | '-' )? primaryexpr )*

expression  ::= ( '+' | '-' )? primaryexpr binoprhs


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
