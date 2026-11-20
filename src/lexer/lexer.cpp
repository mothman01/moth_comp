#include<iostream>
#include<string>
#include<cctype>
#include "lexer/lexer.h"
using namespace std;

    int lex(const string& source, vector<Token>& tokens){
    size_t i = 0;
        while(i<source.size()){
            char c = source[i];
            if (!isspace(c)){
                switch(c){
                    case'(':
                        tokens.push_back(Token{TokenKind::OpenParen, "(", i}); 
                        break;
                    case')':
                        tokens.push_back(Token{TokenKind::CloseParen, ")",i});
                        break;
                    case'{':
                        tokens.push_back(Token{TokenKind::OpenBrace, "{", i});
                        break;
                    case'}':
                        tokens.push_back(Token{TokenKind::CloseBrace, "}", i});
                        break;
                    case';':
                        tokens.push_back(Token{TokenKind::Semicolon, ";", i});

                        break;
                    default:
                        //case to help gather full digits
                        if(isdigit(static_cast<unsigned char>(c))){
                            size_t start = i;
                            string num = "";
                            //cycles through digits till end
                            while (i<source.size()&&isdigit(static_cast<unsigned char>(source[i]))){
                                char d = source[i];
                                 num =num + d;

                                //prevents forbidden digit letter combos
                                if(isalpha(static_cast<unsigned char>(source[i+1]))||source[i+1]=='_'){
                                    cerr<<"\n----------WARNING----------\n"<< "error letter or undrscore follows digit: "<< "'"<<source[i+1]<<"'"<<"\n------------END------------\n";
                                    return 1;

                                }
                                i++;
                            }
                            tokens.push_back(Token{TokenKind::Constant, num, start});
                            continue;//end of digit
                        }
                        if(isalpha(static_cast<unsigned char>(c))||c=='_'){
                            size_t start = i;
                            string word = "";
                            TokenKind kind;
                            while(i<source.size()&&(isalnum(static_cast<unsigned char>(source[i]))|| source[i]=='_')){
                                word = word+ source[i];
                                i++;
                            }
                            if(word=="int"){
                                kind = TokenKind::KwInt;

                            }
                            else if(word=="void"){
                                kind = TokenKind::KwVoid;
                            }

                            else if(word=="return"){
                                    kind = TokenKind::KwReturn;
                            }
                            else{
                                    kind = TokenKind::Identifier;
                            }
                            tokens.push_back(Token{kind, word, start});
                            continue;
                            }

                        
                        else{
                            cerr<<"\n----------WARNING----------\n"<< "error unexpected charachter: "<< "'"<<c<<"'"<<"\n------------END------------\n";
                            return 1;
                        }                   
                }           
            } 

            i++;
    }
    return 0;
}

string kindName(TokenKind k) {
    switch (k) {
        case TokenKind::OpenParen: return "OpenParen";
        case TokenKind::CloseParen: return "CloseParen";
        case TokenKind::OpenBrace: return "OpenBrace";
        case TokenKind::CloseBrace: return "CloseBrace";
        case TokenKind::Semicolon: return "Semicolon";
        case TokenKind::Constant: return "Constant";
        case TokenKind::Identifier: return "Identifier";
        case TokenKind::KwInt: return "KwInt";
        case TokenKind::KwVoid: return "KwVoid";
        case TokenKind:: KwReturn: return "KwReturn";
    }
    return "Unknown";
}
