# moth_comp (mcc)

A C compiler I'm building by hand. This project is my excuse to practice and learn how low-level code actually works: what really happens between `int main` and the CPU running it.

**No AI-written code.** Every line in `src/` and `include/` is mine, bugs included. I use books, docs, and Compiler Explorer to learn, then I write it myself. The point is to understand it, not just to have it.

For context: I started programming in May 2026. This is the biggest thing I've tried so far.

## What it does right now

mcc takes a `.c` file and runs it through a real compiler pipeline:

```
source.c → preprocess (gcc -E) → lexer → parser → AST → codegen → assembly → link (gcc) → executable
```

Current status:

- [x] Driver: preprocesses with gcc, stops at any stage with a flag, links the final binary
- [x] Lexer: keywords, identifiers, constants, punctuation; rejects junk like `1foo` and `@`
- [x] Parser: hand-written recursive descent that builds an AST
- [x] Error messages for bad syntax (missing `;`, missing `)`, early end of file, extra tokens)
- [ ] Codegen: **still a stub**. Right now every program returns 42, no matter what you write. Fixing that is next.

It handles exactly one program shape so far:

```c
int main(void) {
    return 42;
}
```

That sounds small, but it's the whole pipeline end to end. Every feature from here makes each stage a little wider.

## Building

Needs Linux x86-64, CMake 3.20+, a C++ compiler, and `gcc` (for preprocessing and linking).

```sh
cmake -S . -B build
cmake --build build
```

The compiler itself builds with `-Wall -Wextra -Werror` and AddressSanitizer + UBSan, so memory bugs fail loudly instead of hiding.

## Usage

```sh
./build/mcc program.c            # full compile → ./program
./build/mcc --lex program.c      # stop after lexing, print tokens
./build/mcc --parse program.c    # stop after parsing, print the AST
./build/mcc --codegen program.c  # stop before writing assembly
./build/mcc -S program.c         # stop after writing program.s
```

Example AST dump:

```
$ ./build/mcc --parse tests/return_42.c
Program
  Function main
    Return
      Constant 42
```

## Tests

`tests/` has one valid program and a bunch that should fail:

| File | What's wrong with it |
|---|---|
| `return_42.c` | nothing, it should compile |
| `invalid_at.c` | `@` isn't valid C |
| `invalid_num.c` | `1foo` isn't a number or a name |
| `merged.c` | `return42` is one identifier, not a keyword + number |
| `invalid_no_semi.c` | missing `;` |
| `invalid_no_paren.c` | missing `)` |
| `invalid_eof.c` / `num_at_end.c` | file ends in the middle of the function |
| `invalid_extra.c` | junk after the function |

## Where it's going

Roughly following the chapter order of Nora Sandler's *Writing a C Compiler*. Each step widens every stage of the pipeline at once:

1. Real codegen for return values
2. Unary operators (`-`, `~`, `!`) and an intermediate representation (IR)
3. Binary math, comparisons, `&&` / `||`
4. Local variables, `if`/`else`, loops
5. Functions with the System V calling convention, so mcc programs can call `printf`
6. Pointers, arrays, strings, `long`, `unsigned`, `double`
7. Structs, `typedef`, `switch`
8. Compile a real program with it

Long-term goal: build my own OS someday and compile its programs with mcc.

## Resources I'm learning from

- *Writing a C Compiler* by Nora Sandler, plus her test suite
- *Crafting Interpreters* by Robert Nystrom
- chibicc by Rui Ueyama, a reference for how a real small C compiler is structured
- Compiler Explorer (godbolt.org), for checking my assembly against gcc's
