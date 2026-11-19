#include<iostream>
#include<string>
#include<cstdlib>
#include<fstream>
#include<cstdio>
#include<cctype>
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
                    if(isdigit(static_cast<unsigned char>(c))){
                        size_t start = i;
                        string num = "";
                        while (i<source.size()&&isdigit(static_cast<unsigned char>(source[i]))){
                            char d = source[i];
                             num =num + d;
                            if(isalpha(static_cast<unsigned char>(source[i+1]))||source[i+1]=='_'){
                                cerr<<"\n----------WARNING----------\n"<< "error letter or undrscore follows digit: "<< "'"<<source[i+1]<<"'"<<"\n------------END------------\n";
                                return 1;

                            }
                            i++;
                        }
                        cout<<start<<" : Constant "<<num<<"\n";
                        continue;
                    }

                    else if (isalnum((unsigned char)c)||c == '_'){
                        cout<<i<<" : "<<"(not handeled yeti) "<<c<<"\n";
                        break;
                    }
                    else{
                        cerr<<"\n----------WARNING----------\n"<< "error unexpected charachter: "<< "'"<<c<<"'"<<"\n------------END------------\n";
                        return 1;
                    }
            }
        }

        i++;
    }

    if(stop == "--lex"||stop=="--parse"||stop =="--codegen"){
        cout<<"compiling: "<<input<< "\n";
        cout<<"preprocessed: "<< pre<<"\n";
        

        return 0;
    }
    ofstream out(assem);
    out<<"  .globl main\n";
    out<<"main:\n";
    out<<"  movl $42, %eax\n";
    out<<"  ret\n";
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
