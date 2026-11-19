#include<iostream>
#include<string>
#include<cstdlib>
#include<fstream>
#include<cstdio>
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
    remove(pre.c_str());

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
