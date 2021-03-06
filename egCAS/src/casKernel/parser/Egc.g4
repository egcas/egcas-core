// Define a grammar for egcas
grammar Egc;
formula:          
        | expr 
        | equation;


equation: <assoc=right> expr '=' expr           # Equality
        |<assoc=right> expr ':' expr            # Definition
        ;

expr : <assoc=right> expr EXP expr              # Exponent
     | '-' expr                                 # UMinus
     | RED_PARENTHESIS_L expr                   # RedParenthesisL
     | expr RED_PARENTHESIS_R                   # RedParenthesisR
     | expr op=(MULT | DIV) expr                # MulDiv
     | expr op=(PLUS | MINUS) expr              # PlusMinus
     | expr EMPTYBINOP expr                     # EmptyBinOp
     | '(' expr ')'                             # Parenthesis
     | NUMBER                                   # Number
     | NAMES                                    # Variable
     | NAMES VARSUB                             # Variable
     | LOGARITHM '(' expr ')'                   # Logarithm
     | NATLOGRITHM '(' expr ')'                 # Natlogarithm
     | SQRT '(' expr ')'                        # Sqrt
     | INTEGRAL '(' explist ')'                 # Integral
     | DIFFERENTIAL '(' explist ')'             # Differential
     | ROOT '(' expr ',' expr ')'               # Root
     | EMPTY                                    # Empty
     | NAMES '(' explist ')'                    # Function
     | EMPTY '(' explist ')'                    # Function
     | LBRACKET_OP expr RBRACKET_OP             # BracketOp
     ;
    
explist: expr                                   # createArglist
       | expr ',' explist                       # addArgument
       ;


// LEXER
//operators
PLUS:           '+';
MINUS:          '-';
MULT:           '*';
DIV:            '/';
EXP:            '^' | '**';
SQRT:           '_sqrt';
//parenthesis to define operation borders
LBRACKET_OP:    '_{';
RBRACKET_OP:    '_}';
INTEGRAL:       '_integrate';
DIFFERENTIAL:   '_diff';
ROOT:           '_root';
EMPTY:          '_empty';
EMPTYBINOP:     '_emptybinop';
LOGARITHM:      '_log';
NATLOGRITHM:    '_ln';
RED_PARENTHESIS_R: '_red_parenth_r';
RED_PARENTHESIS_L: '_red_parenth_l';

NUMBER:         [0-9]+ '.' [0-9]* EXPONENT? | '.'? [0-9]+ EXPONENT?;        //numbers
NAMES:          ALNUMNODE;      //names
VARSUB:         VARSEP ALNUMNODESUB;     //variable separator

WS : [ \t\r\n]+ -> skip ; // skip spaces, tabs, newlines 


fragment EXPONENT:     [Eeb] [-+]? [0-9]+;
fragment VARSEP:       '_1';
fragment UNDERSCORE:   '__';
fragment UNICODESIGN:  '_2' [0-9]+ '_3';
fragment ALNUMNODE:    ([a-zA-Z] | UNICODESIGN) ([a-zA-Z0-9] | UNDERSCORE | UNICODESIGN)*;
fragment ALNUMNODESUB: ([a-zA-Z0-9] | UNDERSCORE | UNICODESIGN)* | EMPTY;
