#include<iostream>
#include<string>
#include<cctype>
#include "lexer/lexer.h"
using namespace std;

int lex(const string& source){
    size_t i = 0;
        while(i<source.size()){
            char c = source[i];
            if (!isspace(c)){
                switch(c){
                    case'(':
                        cout<<i<<" : "<<"OpenParen\n";
                        break;
                    case')':
                        cout<<i<<" : "<<"ClosedParen\n";
                        break;
                    case'{':
                        cout<<i<<" : "<<"OpenBrace\n";
                        break;
                    case'}':
                        cout<<i<<" : "<<"ClosedBrace\n";
                        break;
                    case';':
                        cout<<i<<" : "<<"Semicolon\n";
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
                            cout<<start<<" : Constant "<<num<<"\n";
                            continue;//end of digit
                        }
                        if(isalpha(static_cast<unsigned char>(c))||c=='_'){
                            size_t start = i;
                            string word = "";
                            string token;
                            while(i<source.size()&&(isalnum(static_cast<unsigned char>(source[i]))|| source[i]=='_')){
                                word = word+ source[i];
                                i++;
                            }
                            if(word=="int"||word=="void"||word=="return"){
                                    token = "Keyword ";
                            }
                            else{
                                    token = "Identifier ";
                            }
                            cout<<start<<" : "<<token<<word<<"\n";
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
