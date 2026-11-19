cmake_minimum_required(VERSION 3.20)
project(compiler LANGUAGES CXX)
add_executable(mcc
    src/main.cpp
    src/lexer/lexer.cpp
    src/parser/parser.cpp
    src/codegen/codegen.cpp
    src/main.cpp src/lexer/lexer.cpp
)

target_include_directories(mcc PRIVATE include)

target_compile_options( mcc PRIVATE
    -Wall
    -Wextra
    -Werror
    -g
    -fsanitize=address,undefined
)

target_link_options(mcc PRIVATE
    -fsanitize=address,undefined

)

