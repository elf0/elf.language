#ifndef TOKEN_H
#define TOKEN_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Type.h"

enum TokenType{
    ttInvalid,
    ttEOS,
    ttVersion,
    ttLiteralInteger,
    ttLiteralFloat,
    ttLiteralString,
    ttLiteralFunction,
    ttParenthesisOpen,
    ttParenthesisClose,
    ttBraceOpen,
    ttBraceClose,
    ttStar,
    ttPlus,
    ttComma,
    ttMinus,
    ttDot,
    ttSlash,
    ttEqual,
    ttLess,
    ttLessEqual,
    ttGreater,
    ttGreaterEqual,
    ttDefineConstant,
    ttDefineConstantFunction,
    ttDefineConstantClass,
    ttDefineVariable,
    ttDefineVariableFunction,
    ttDefineVariableClass,
    ttLower,
    ttUpper,
    ttColon,
    ttIf,
    ttIn,
    ttIs,
    ttThen,
    ttElse,
    ttFor,
    MAX_TOKEN_TYPE
};

struct TokenLocation{
    U16 nLine;
    U16 nColumn;
};

typedef struct{
    Char *pBegin;
    Char *pEnd;
}TokenString;

typedef struct{
    TokenType type;
    TokenLocation location;
    union{
        TokenString str;
        U32 u32;
        I64 i64;
        F64 f64;
    };
}Token;

#endif // TOKEN_H


