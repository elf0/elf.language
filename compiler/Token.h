#ifndef TOKEN_H
#define TOKEN_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Type.h"

typedef enum{
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
    ttDefineConstantClass,
    ttDefineVariable,
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
}TokenType;

typedef struct{
    U16 nLine;
    U16 nColumn;
}TokenLocation;

typedef struct{
    Char *pBegin;
    Char *pEnd;
}TokenString;

typedef struct{
    TokenType type;
    TokenLocation location;
    union{
        TokenString strValue;
        U32 u32Value;
        I32 i32Value;
        U64 u64Value;
        I64 i64Value;
        F32 f32Value;
        F64 f64Value;
    };
}Token;

#endif // TOKEN_H

