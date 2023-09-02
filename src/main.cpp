#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "Tokenizer.hpp"

std::string tokens_to_asm(std::vector<Token> tokens)
{
    std::stringstream output;
    output << "global _start:" << std::endl
           << "_start:" << std::endl;
    for (auto t = tokens.begin(); t != tokens.end(); t++) {
        if (t->type == TokenType::exit) {
            t++;
            if (t->type == TokenType::int_lit) {
                std::string lit = t->value.value();
                t++;
                if (t->type == TokenType::semi) {
                    output << "    mov rax, 60" << std::endl
                           << "    mov rdi, " << lit << std::endl
                           << "    syscall";
                } else {
                    std::cerr << "Need a ';' after a return statement ! " << std::endl;
                    exit(EXIT_FAILURE);
                }
            } else {
                std::cerr << " Return statment needs a number !" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
    }
    return output.str();
}
int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Wrong usage. Correct usage is..." << std::endl;
        std::cerr << "hydro <input.hy>" << std::endl;
        return EXIT_FAILURE;
    }
    std::string contents;
    {
        std::fstream input(argv[1], std::ios::in);
        std::stringstream contents_stream;
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }
    // std::cout << contents << std::endl;
    auto tokens = Tokenizer::tokenize(contents);
    // std::cout << tokens.size() << std::endl;
    // std::cout << tokens_to_asm(tokens) << std::endl;
    {
        std::fstream file("out.asm", std::ios::out);
        file << tokens_to_asm(tokens);
        file << std::endl;
    }

    system("nasm -felf64 out.asm");
    system("ld -o out out.o");
    return EXIT_SUCCESS;
}
