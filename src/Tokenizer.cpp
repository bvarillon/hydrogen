#include <iostream>
#include <ostream>

#include "Tokenizer.hpp"

std::vector<Token> Tokenizer::tokenize(const std::string str)
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
            if (buffer == "exit") {
                tokens.push_back({ .type = TokenType::exit });
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
