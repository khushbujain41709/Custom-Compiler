#ifndef LEXER_H
#define LEXER_H
#include <string>
#include <vector>

enum TokenType { NUMBER, VARIABLE, OPERATOR, LPAREN, RPAREN, ASSIGN, FUNC, END };

struct Token {
    TokenType type;
    std::string value;
};

std::vector<Token> tokenize(const std::string& code);
#endif