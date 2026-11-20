#include<iostream>
#include<cstdlib>
#include<vector>
#include "lexer/lexer.h"
#include "parser/parser.h"
using namespace std;


Token expect(const vector<Token>& tokens,size_t& pos, TokenKind kind){
    if(pos>=tokens.size()){
        cerr<<"---------WARNING----------\n"
            <<" error: unexpected end of file. expected input "
            <<kindName(kind)
            <<"\n------------END------------\n";
        exit(1);
    }
    if(tokens[pos].kind != kind){
                    cerr<<"---------WARNING----------\n"
            <<" error: expected "
            <<kindName(kind)
            <<" but found "
            <<tokens[pos].text
            <<" at position "
            <<tokens[pos].pos
            <<"\n------------END------------\n";
            exit(1);

        }
    Token t = tokens[pos];
    pos++;
    return t;
}

Constant parseExp(const vector<Token>& tokens, size_t& pos){
    Token t =expect(tokens, pos, TokenKind::Constant);
    return Constant{stoi(t.text)};

}

Return parseStatement(const vector<Token>& tokens, size_t& pos){
    expect(tokens, pos, TokenKind::KwReturn);
    Constant exp = parseExp(tokens, pos);
    expect(tokens, pos, TokenKind::Semicolon);
    return Return{exp};



}

Function parseFunction(const vector<Token>& tokens, size_t& pos){
    expect(tokens, pos, TokenKind::KwInt);
    Token name = expect(tokens, pos, TokenKind::Identifier);
    expect(tokens, pos, TokenKind::OpenParen);
    expect(tokens, pos, TokenKind::KwVoid);
    expect(tokens, pos, TokenKind::CloseParen);
    expect(tokens, pos, TokenKind::OpenBrace);
    Return body = parseStatement(tokens, pos);
    expect(tokens, pos, TokenKind::CloseBrace);
    return Function{name.text, body};

}

Program parseProgram(const vector<Token>& tokens){
    size_t pos = 0;
    Function func = parseFunction(tokens, pos);
    if (pos != tokens.size()){
        cerr<<"---------WARNING----------\n"
        <<" unexpected "
        << tokens[pos].text
        <<" at postion "
        <<tokens[pos].pos
        <<" after end of function. "
         <<"\n------------END------------\n";
        exit(1);

    
    }
    return Program{func};

}

void printProgram(const Program& p){
    cout<<"Program\n"
    <<"  Function "
    <<p.function.name
    <<"\n    "
    <<"Return "
    <<"\n      "
    <<"Constant"
    <<" "
    <<p.function.body.exp.value
    <<"\n";

}

