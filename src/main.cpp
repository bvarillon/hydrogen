#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

enum class TokenType {
    _return,
    int_lit,
    semi
};
struct Token {
    TokenType type;
    std::optional<std::string> value;
};

std::vector<Token> tokenize(const std::string str)
{
    std::string buffer;
    std::vector<Token> tokens;
    auto c = str.begin();
    while (c != str.end()) {
        if (std::isalpha(*c)) {
            buffer.push_back(*c);
            c++;
            while (std::isalnum(*c)) {
                buffer.push_back(*c);
                c++;
            }
            if (buffer == "return") {
                tokens.push_back({ .type = TokenType::_return });
                buffer.clear();
            } else {
                std::cerr << "Unknown token !" << std::endl;
                exit(EXIT_FAILURE);
            }
        } else if (std::isspace(*c)) {
            c++;
        } else if (std::isdigit(*c)) {
            buffer.push_back(*c);
            c++;
            while (std::isdigit(*c)) {
                buffer.push_back(*c);
                c++;
            }
            if (!std::isspace(*c) && (*c != ';')) {
                std::cerr << "Token starting with digit are not valid !" << std::endl;
                exit(EXIT_FAILURE);
            }
            tokens.push_back({ .type = TokenType::int_lit, .value = buffer });
            buffer.clear();
        } else if (*c == ';') {
            tokens.push_back({ .type = TokenType::semi });
            c++;
        }
    }
    return tokens;
}

std::string tokens_to_asm(std::vector<Token> tokens)
{
    std::stringstream output;
    output << "global _start:" << std::endl
           << "_start:" << std::endl;
    for (auto t = tokens.begin(); t != tokens.end();t++) {
        if (t->type == TokenType::_return) {
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
    auto tokens = tokenize(contents);
    //std::cout << tokens.size() << std::endl;
    //std::cout << tokens_to_asm(tokens) << std::endl;
    {
        std::fstream file("out.asm",std::ios::out);
        file << tokens_to_asm(tokens);
    }

    system("nasm -felf64 out.asm");
    system("ld -o out out.o");
    return EXIT_SUCCESS;
}
