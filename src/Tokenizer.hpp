#pragma once

#include <optional>
#include <string>
#include <vector>

enum class TokenType {
    exit,
    int_lit,
    semi
};
struct Token {
    TokenType type;
    std::optional<std::string> value;
};
class Tokenizer {
public:
    static std::vector<Token> tokenize(const std::string);
};
