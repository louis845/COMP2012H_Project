## Token List

Check [asciimath](http://asciimath.org/) for the meaning of the tokens. Note that the priority of the list matters.

Please update the list if new tokens are needed.

```
literal     regex           token name          token type
-----------------------------------------------------------------
,           [,]             COMMA               DELIM
|           [\|]            TEXTBAR             DELIM
+           [\+]            PLUS                OPERATOR
-           [-]             MINUS               // emitted for below
**          [\*\*]          AST             
*           [\*]            CDOT
//          [\/\/]          DIV             
xx          [xx]            CROSS                
/           [\/]            FRAC                
^           [\^]            SUP
sqrt        [sqrt]          SQRT
root        [root]          ROOT            
(           [\(]            LP
)           [\)]            RP
=           [=]             EQUAL
[           [\[]            LSB
]           []]             RSB
{           [\{]            LCB
}           [\}]            RCB
_           [_]             SUB
%           [%]             PERCENT
e           [e]             E                   NUM
I           [I]             IDENTITY_MATRIX     NUM
pi          [pi]            PI                  NUM
abs         [abs]           ABS
norm        [norm]          NORM
alpha       [alpha]         ALPHA
beta        [beta]          // emitted for below
theta       [theta]
lambda      [lambda]
mu          [mu]
phi         [phi]
varphi      [varphi]
omega       [omega]
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
max         [min]
trace       [(trace|tr)]
RREF        [(RREF|rref)]
IDENTIFIER  [a-zA-Z]            IDENTIFIER          IDENTIFIER
NUMERICAL   [(\.[0-9]+|[0-9]+\.?[0-9]*)]            NUM    
```