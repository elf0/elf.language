#ifndef LEXER_H
#define LEXER_H

//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

#include "Type.h"
#include "String.h"
#include "File.h"
#include "Token.h"

#define MAX_SOURCE_SIZE 65535
#define MAX_LINE_SIZE 255
#define MAX_IDENTIFIER_SIZE 63

class Lexer{
public:
    Lexer(){
        _pBegin = (Char*)malloc(MAX_SOURCE_SIZE + 1);
        _pEnd = _pBegin + MAX_SOURCE_SIZE;
    }

    ~Lexer(){
        free(_pBegin);
    }

    inline Bool Load(const Char *pPathName){
        File file;
        if(!File_OpenForRead(&file, pPathName))
            return false;

        if(!File_ReadMeta(&file)){
            File_Close(&file);
            return false;
        }

        U32 nFileSize = file.meta.st_size;
        if(nFileSize > MAX_SOURCE_SIZE){
            File_Close(&file);
            return false;
        }

        if((U32)File_Read(&file, _pBegin, nFileSize) != nFileSize){
            File_Close(&file);
            return false;
        }
        File_Close(&file);
        _pBegin[nFileSize] = 0;
        _pLine = _pBegin;
        _pChar = _pBegin;
        return true;
    }

    inline TokenType Read();

    inline const Token &GetToken()const{
        return _token;
    }

    inline I64 GetI64Value()const{
        return _i64Value;
    }

    inline F64 GetF64Value()const{
        return _f64Value;
    }

    inline U32 GetLine()const{
        return _nLine;
    }

    inline U32 GetColumn()const{
        return _pChar - _pLine + 1;
    }
private:
    Char *_pBegin = null;
    Char *_pEnd = null;
    Char *_pChar = null;
    Char *_pLine = null;
    U32 _nLine = 1;
    union{
        Token _token;
        I64 _i64Value;
        F64 _f64Value;
    };
};

TokenType Lexer::Read(){
    static void *Labels[256]={
        //0x00
        &&L_C_EOS, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID,
        //0x08 0x09(Tabulator) 0x0A(LineFeed), 0x0B(VerticalTabulator) 0x0C 0x0D(CarriageReturn) 0x0E 0x0F
        &&L_C_INVALID, &&L_C_INVALID, &&L_C_LINE_FEED, &&L_C_INVALID, &&L_C_INVALID, &&L_C_CARRIAGE_RETURN, &&L_C_INVALID, &&L_C_INVALID,
        //0x10
        &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID,
        //0x18
        &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID,
        //space ! " # $ % & '
        &&L_NEXT, &&L_C_INVALID, &&L_C_QUOTATION, &&L_C_SHARP, &&L_C_DOLLAR, &&L_C_INVALID, &&L_C_INVALID, &&L_C_SINGLE_QUOTATION,
        //( ) * + , - . /
        &&L_C_PARENTHESIS_OPEN, &&L_C_PARENTHESIS_CLOSE, &&L_C_STAR, &&L_C_PLUS, &&L_C_COMMA, &&L_C_MINUS, &&L_C_DOT, &&L_C_SLASH,
        //0 1 2 3 4 5 6 7
        &&L_C_DIGIT, &&L_C_DIGIT, &&L_C_DIGIT, &&L_C_DIGIT, &&L_C_DIGIT, &&L_C_DIGIT, &&L_C_DIGIT, &&L_C_DIGIT,
        //8 9 : ; < = > ?
        &&L_C_DIGIT, &&L_C_DIGIT, &&L_C_INVALID, &&L_C_INVALID, &&L_C_LESS, &&L_C_EQUAL, &&L_C_GREATER, &&L_C_INVALID,
        //@ A B C D E F G
        &&L_C_INVALID, &&L_C_UPPER_A, &&L_C_UPPER_B, &&L_C_UPPER_C, &&L_C_UPPER_D, &&L_C_UPPER_E, &&L_C_UPPER_F, &&L_C_UPPER_G,
        //H I J K L M N O
        &&L_C_UPPER_H, &&L_C_UPPER_I, &&L_C_UPPER_J, &&L_C_UPPER_K, &&L_C_UPPER_L, &&L_C_UPPER_M, &&L_C_UPPER_N, &&L_C_UPPER_O,
        //P Q R S T U V W
        &&L_C_UPPER_P, &&L_C_UPPER_Q, &&L_C_UPPER_R, &&L_C_UPPER_S, &&L_C_UPPER_T, &&L_C_UPPER_U, &&L_C_UPPER_V, &&L_C_UPPER_W,
        //X Y Z [ \ ] ^ _
        &&L_C_UPPER_X, &&L_C_UPPER_Y, &&L_C_UPPER_Z, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID,
        //` a b c d e f g
        &&L_C_INVALID, &&L_C_LOWER_A, &&L_C_LOWER_B, &&L_C_LOWER_C, &&L_C_LOWER_D, &&L_C_LOWER_E, &&L_C_LOWER_F, &&L_C_LOWER_G,
        //h i j k l m n o
        &&L_C_LOWER_H, &&L_C_LOWER_I, &&L_C_LOWER_J, &&L_C_LOWER_K, &&L_C_LOWER_L, &&L_C_LOWER_M, &&L_C_LOWER_N, &&L_C_LOWER_O,
        //p q r s t u v w
        &&L_C_LOWER_P, &&L_C_LOWER_Q, &&L_C_LOWER_R, &&L_C_LOWER_S, &&L_C_LOWER_T, &&L_C_LOWER_U, &&L_C_LOWER_V, &&L_C_LOWER_W,
        //x y z { | } ~ 0x7F
        &&L_C_LOWER_X, &&L_C_LOWER_Y, &&L_C_LOWER_Z, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID,
        //0x80
        &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID,
        &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID,
        &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID,
        &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID,
        &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID,
        &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID,
        &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID,
        &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID,
        &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID,
        &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID,
        &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID,
        &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID,
        &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID,
        &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID,
        &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID,
        &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID, &&L_C_INVALID,
    };

    void *label_continue;
    Char c;
    Char *p = _pChar;
L_NEXT:
    c = *p++;
    goto *Labels[c];
L_C_EOS:
    _pChar = p - 1;
    return tkEOS;
L_C_LINE_FEED://0x0A
    _pLine = p;
    ++_nLine;
    goto L_NEXT;
L_C_CARRIAGE_RETURN://0x0D
    if(*p == 0x0A)
        ++p;
    goto L_C_LINE_FEED;
L_C_QUOTATION:{
        Char *pBeginBegin = p - 1;
        p = (Char*)String_Skip(p, '"');
        _token.pBegin = p;
        U32 nBeginCount = p - pBeginBegin;
L_C_QUOTATION_RETRY:
        *_pEnd = '"';
        while(true){
            c = *p++;
            //FIXME: Only 0x0D in source?
            if(c == 0x0A){
                _pLine = p;
                ++_nLine;
            }
            else if(c == '"'){
                *_pEnd = 0;
                if(p > _pEnd)
                    goto L_C_EOS;//FIXME: Error, Expect end

                Char *pEndBegin = p - 1;
                p = (Char*)String_Skip(p, '"');
                U32 nEndCount = p - pEndBegin;
                if(nEndCount != nBeginCount)
                    goto L_C_QUOTATION_RETRY;
                _token.pEnd = pEndBegin;
                _pChar = p;
                return tkQuotation;
            }
        }
    }
L_C_SHARP:{
        Char *pBeginBegin = p - 1;
        p = (Char*)String_Skip(p, '#');
        U32 nBeginCount = p - pBeginBegin;
        if(nBeginCount > 1){
L_C_SHARP_RETRY:
            *_pEnd = '#';
            while(true){
                c = *p++;
                //FIXME: Only 0x0D in source?
                if(c == 0x0A){
                    _pLine = p;
                    ++_nLine;
                }
                else if(c == '#'){
                    *_pEnd = 0;
                    if(p > _pEnd)
                        goto L_C_EOS;

                    Char *pEndBegin = p - 1;
                    p = (Char*)String_Skip(p, '#');
                    U32 nEndCount = p - pEndBegin;
                    if(nEndCount != nBeginCount)
                        goto L_C_SHARP_RETRY;
                    goto L_NEXT;
                }
            }
        }
        else{
            //FIXME: Only 0x0D in source?
            *_pEnd = 0x0A;
            p = (Char*)String_SkipUntil(p, 0x0A);
            *_pEnd = 0;
            if(p++ != _pEnd)
                goto L_C_LINE_FEED;
            else
                goto L_C_EOS;
        }
    }
L_C_DOLLAR:
    _pChar = p;
    return tkFunctionDefine;
L_C_SINGLE_QUOTATION:{
        Char *pBeginBegin = p - 1;
        p = (Char*)String_Skip(p, '\'');
        _token.pBegin = p;
        U32 nBeginCount = p - pBeginBegin;
L_C_SINGLE_QUOTATION_RETRY:
        *_pEnd = '\'';
        while(true){
            c = *p++;
            //FIXME: Only 0x0D in source?
            if(c == 0x0A){
                _pLine = p;
                ++_nLine;
            }
            else if(c == '\''){
                *_pEnd = 0;
                if(p > _pEnd)
                    goto L_C_EOS;//FIXME: Error, Expect end

                Char *pEndBegin = p - 1;
                p = (Char*)String_Skip(p, '\'');
                U32 nEndCount = p - pEndBegin;
                if(nEndCount != nBeginCount)
                    goto L_C_SINGLE_QUOTATION_RETRY;
                _token.pEnd = pEndBegin;
                _pChar = p;
                return tkSingleQuotation;
            }
        }
    }
L_C_PARENTHESIS_OPEN:
    _pChar = p;
    return tkParenthesisOpen;
L_C_PARENTHESIS_CLOSE:
    _pChar = p;
    return tkParenthesisClose;
L_C_STAR:
    _pChar = p;
    return tkStar;
L_C_PLUS:
    _pChar = p;
    return tkPlus;
L_C_COMMA:
    _pChar = p;
    return tkComma;
L_C_MINUS:
    _pChar = p;
    return tkMinus;
L_C_DOT:
    _pChar = p;
    return tkDot;
L_C_SLASH:
    _pChar = p;
    return tkSlash;
L_C_DIGIT:{
        I64 i64Value = c - '0';
        p = (Char*)String_ParseI64(p, &i64Value);
        if(*p != '.'){
            _i64Value = i64Value;
            return tkIntegerLiteral;
        }

        Char *pFloat = p++;
        p = (Char*)String_SkipDigit(p);
        _f64Value = (F64)i64Value + strtod((const char *)pFloat, 0);
        return tkFloatLiteral;
    }
L_C_LESS:
    _pChar = p;
    return tkLess;
L_C_EQUAL:
    _pChar = p;
    return tkEqual;
L_C_GREATER:
    _pChar = p;
    return tkGreater;
L_C_UPPER_A:
L_C_UPPER_B:
L_C_UPPER_C:
L_C_UPPER_D:
L_C_UPPER_E:
L_C_UPPER_F:
L_C_UPPER_G:
L_C_UPPER_H:
L_C_UPPER_I:
L_C_UPPER_J:
L_C_UPPER_K:
L_C_UPPER_L:
L_C_UPPER_M:
L_C_UPPER_N:
L_C_UPPER_O:
L_C_UPPER_P:
L_C_UPPER_Q:
L_C_UPPER_R:
L_C_UPPER_S:
L_C_UPPER_T:
L_C_UPPER_U:
L_C_UPPER_V:
L_C_UPPER_W:
L_C_UPPER_X:
L_C_UPPER_Y:
L_C_UPPER_Z:
    label_continue = &&L_C_UPPER_IDENTIFIER_CONTINUE;
    goto L_C_IDENTIFIER;
L_C_UPPER_IDENTIFIER_CONTINUE:
    _pChar = p;
    return tkIdentifierUpper;

L_C_LOWER_A:
L_C_LOWER_B:
L_C_LOWER_C:
L_C_LOWER_D:
L_C_LOWER_E:
L_C_LOWER_F:
L_C_LOWER_G:
L_C_LOWER_H:
    //L_C_LOWER_I:
L_C_LOWER_J:
L_C_LOWER_K:
L_C_LOWER_L:
L_C_LOWER_M:
L_C_LOWER_N:
L_C_LOWER_O:
L_C_LOWER_P:
L_C_LOWER_Q:
L_C_LOWER_R:
L_C_LOWER_S:
L_C_LOWER_T:
L_C_LOWER_U:
L_C_LOWER_V:
L_C_LOWER_W:
L_C_LOWER_X:
L_C_LOWER_Y:
L_C_LOWER_Z:
    label_continue = &&L_C_LOWER_IDENTIFIER_CONTINUE;
    goto L_C_IDENTIFIER;
L_C_LOWER_IDENTIFIER_CONTINUE:
    _pChar = p;
    return tkIdentifierLower;


L_C_LOWER_I:
    label_continue = &&L_C_LOWER_I_CONTINUE;
    goto L_C_IDENTIFIER;
L_C_LOWER_I_CONTINUE:
    _pChar = p;
    if((_token.pEnd - _token.pBegin) == 2){
        Char c1 = _token.pBegin[1];
        if(c1 == 'f')
            return tkIf;
        else if(c1 == 's')
            return tkIs;
        else if(c1 == 'n')
            return tkIn;
    }
    return tkIdentifierLower;


L_C_IDENTIFIER:
    _token.pBegin = p - 1;
    while(isalnum(*p))
        ++p;
    _token.pEnd = p;
    goto *label_continue;

L_C_INVALID:
    _pChar = p - 1;
    return tkInvalid;
}

#endif // LEXER_H


