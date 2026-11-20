#include "codegen/codegen.h"
using namespace std;

string generate(const Program& p){
    string func =  p.function.name;
    string ret = to_string(p.function.body.exp.value);
    string assem ="    .global "+func+"\n"+func+":\n    movl $"+ret+", %eax\n    ret\n    .section .note.GNU-stack,\"\",@progbits\n";
    return assem;
}


