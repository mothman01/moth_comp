#include<iostream>
#include<cstdlib>
#include<vector>
#include <utility>
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

Exp parseExp(const vector<Token>& tokens, size_t& pos){
    if (pos>=tokens.size()){
        cerr<<"---------WARNING----------\n"
            <<"Unexpected end of file."
            <<"-----------END------------\n";
        exit(1);
    }
    Token t = tokens[pos];

    if (t.kind==TokenKind::Constant){
        pos++;
        Exp e;
        e.kind = ExpKind::Constant;
        e.value = stoi(t.text);
        return e;
    }
    if(t.kind==TokenKind::Hyphen||t.kind==TokenKind::Tilde){
        pos++;
        Exp e;
        e.kind = ExpKind::Unary;
        e.op = (t.kind==TokenKind::Hyphen)? UnaryOp::Negate : UnaryOp::Complement;
        e.inner = make_unique<Exp>( parseExp(tokens, pos) );
        return e;
    }
    if (t.kind==TokenKind::OpenParen){
        pos++;
        Exp inner = parseExp(tokens, pos);
        expect(tokens, pos,TokenKind::CloseParen);
        return inner;

    }
    else{
        cerr<<"expected expression but found "<<t.text<<" at position "<<t.pos<<"\n";
        exit(1);
    }
exit(1);

}

Return parseStatement(const vector<Token>& tokens, size_t& pos){
    expect(tokens, pos, TokenKind::KwReturn);
    Exp exp = parseExp(tokens, pos);
    expect(tokens, pos, TokenKind::Semicolon);
    return Return{std::move(exp)};



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
    return Function{name.text, std::move(body)};

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
    return Program{std::move(func)};

}
void printExp(const Exp& e, int indent){
    cout<< string(indent, ' ');
    if(e.kind == ExpKind::Constant){
        cout<<"Constant "<<e.value<<"\n";
    }
    else{
        
        cout<<"Unary "<<((e.op == UnaryOp::Negate) ? "Negate" : "Complement")<<"\n";
        printExp(*e.inner,indent +2);
    }
    
}





void printProgram(const Program& p){
    cout<<"Program\n"
    <<"  Function "
    <<p.function.name
    <<"\n    "
    <<"Return\n";
    printExp(p.function.body.exp, 6);

}

