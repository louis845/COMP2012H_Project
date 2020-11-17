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

parenexpr       ::= '(' expression ')'
                ::= '|' expression '|'

bracketexpr     ::= '[' expression (',' expression)* ']'

identifierexpr  ::= variable
                ::= function ( number | variable )
                ::= function '(' expression (',' expression)* ')'

matrixexpr      ::= '[' bracketexpr (',' bracketexpr)* ']'

primaryexpr ::= numberexpr
            ::= parenexpr
            ::= matrixexpr
            ::= identifierexpr

binoprhs    ::= ( binop primaryexpr )*

expression  ::= primaryexpr binoprhs


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
