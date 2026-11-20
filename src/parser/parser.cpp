#include<iostream>
#include<cstdlib>
#include<vector>
#include "lexer/lexer.h"
#include "parser/parser.h"
using namespace std;


void expect(const vector<Token>& tokens,size_t& pos, TokenKind kind){
    if(pos>=tokens.size()){
        cerr<<"---------WARNING----------\n"
            <<"error: unexpected end of file. expected input "
            <<kindName(kind)
            <<"\n------------END------------\n";
        exit(1);
    }
    if(tokens[pos].kind != kind){
                    cerr<<"---------WARNING----------\n"
            <<"error: expected "
            <<kindName(kind)
            <<"but found "
            <<tokens[pos].text
            <<"at position "
            <<tokens[pos].pos
            <<"\n------------END------------\n";
            exit(1);

        }
    pos++;
}
