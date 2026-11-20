#include<iostream>
#include<string>
#include<cstdlib>
#include<fstream>
#include<cstdio>
#include<cctype>
#include<vector>
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "codegen/codegen.h"
using namespace std;

int main(int argc, char* argv[]){
    string stop= "";
    string input = "";
    for (int i = 1; i<argc;i++){
        string arg = argv[i];
        if(arg[0]=='-'){
            stop = argv[i];
            if(stop != "--lex"&&stop!="--parse"&&stop!="--codegen"&&stop!="-S"){
                cerr<<"\n----------WARNING----------\nunknown flag: "<<stop<<"\n------------END------------\n";
                return 1;
            }
        }
        else{
            input = arg;
        }
            
    }
    
    if(input.empty()){
        cerr << "no file entered.\n";
        return 1;
    }
    size_t dot = input.rfind('.');
    if (dot == string::npos){
        cerr<<"\n----------WARNING----------\nfile given is not a c file: "<<input<< "\n------------END------------\n";
        return 1; 

    }
    if (".c"!=input.substr(dot)){
        cerr<<"\n----------WARNING----------\nfile given is not a c file: "<<input<< "\n------------END------------\n";
        return 1; 
    }

    string assem = input.substr(0, input.rfind('.')) + ".s";
    string ex = input.substr(0,input.rfind('.'));
    string pre = ex + ".i";

    string start = "gcc -E -P ";
    string middle = " -o ";
    string command = start + input + middle + pre; 
    int result = system(command.c_str());
    if (result !=0){
        cerr<<"\n----------WARNING----------\npreproccessing failed for file : "<<input<<"\n------------END------------\n";
        return 1;
    }
    //actual lexer
    string source = "";
    ifstream in(pre);
    string line;
    while(getline(in, line)){
        source = source + line + "\n";
    }
    in.close();
    remove(pre.c_str());
    vector<Token> tokens;
    int lexResult = lex(source, tokens);
    if (lexResult != 0){
        return 1;
    }
    if(stop == "--lex"){
        for (size_t k =0; k< tokens.size();k++){
            cout<<tokens[k].pos <<" : "
            <<kindName(tokens[k].kind)<< " "
            << tokens[k].text<<"\n";
        }
        return 0;
    }
    Program program = parseProgram(tokens);

    if(stop=="--parse"){
        printProgram(program); 
        
        return 0;
    }
    
    string asmText = generate(program);
    if("--codegen"==stop){
        cout<<asmText;
        return 0;
    }


    ofstream out(assem);
    out<< asmText;
    out.close();

    if (stop == "-S"){
        cout<<"compiling: "<<input<< "\n";
        cout<<"preprocessed: "<< pre<<"\n";
        cout<<"assembly file: "<< assem<< "\n";

        return 0;    
    }

    string linker = "gcc "+assem+" -o "+ex;
    int link = system(linker.c_str());
    if (link!=0){
        cerr<<"\n----------WARNING----------\nlinking failed for file : "<<input<<"\n------------END------------\n";
        return 1;
    }
    remove(assem.c_str());

    
    cout<<"compiling: "<<input<< "\n";
    cout<<"preprocessed: "<< pre<<"\n";
    cout<<"assembly file: "<< assem<< "\n";
    cout<<"executable: "<<ex<<"\n";


    return 0;
}
