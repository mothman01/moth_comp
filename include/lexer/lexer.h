#pragma once
#include <string>
#include<vector>
enum class TokenKind{
    OpenParen, CloseParen, OpenBrace, CloseBrace, Semicolon, Constant, Identifier, KwInt,
    KwVoid,KwReturn,Tilde,Hyphen,Decrement
};



struct Token{
    TokenKind kind;
    std::string text;
    size_t pos;

};



int lex(const std::string& source, std::vector<Token>& tokens);
std::string kindName(TokenKind k);

