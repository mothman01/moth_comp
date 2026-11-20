#include<iostream>
#include<cstdlib>
#include<vector>
#include "lexer/lexer.h"
#include "parser/parser.h"
using namespace std;


void expect(const vector<Token>& tokens,size_t& pos, TokenKind kind){
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
    pos++;
}

void parseExp(const vector<Token>& tokens, size_t& pos){
    expect(tokens, pos, TokenKind::Constant);


}

void parseStatement(const vector<Token>& tokens, size_t& pos){
    expect(tokens, pos, TokenKind::KwReturn);
    parseExp(tokens, pos);
    expect(tokens, pos, TokenKind::Semicolon);



}

void parseFunction(const vector<Token>& tokens, size_t& pos){
    expect(tokens, pos, TokenKind::KwInt);
    expect(tokens, pos, TokenKind::Identifier);
    expect(tokens, pos, TokenKind::OpenParen);
    expect(tokens, pos, TokenKind::KwVoid);
    expect(tokens, pos, TokenKind::CloseParen);
    expect(tokens, pos, TokenKind::OpenBrace);
    parseStatement(tokens, pos);
    expect(tokens, pos, TokenKind::CloseBrace);


}

void parseProgram(const vector<Token>& tokens){
    size_t pos = 0;
    parseFunction(tokens, pos);
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


}

