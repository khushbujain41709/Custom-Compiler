#include "lexer.h"
#include <cctype>

std::vector<Token> tokenize(const std::string& code) {
    std::vector<Token> tokens;
    size_t i = 0;
    while (i < code.size()) {
        if (isspace(code[i])) { i++; continue; }
        if (isdigit(code[i])) {
            std::string num;
            while (isdigit(code[i])) num += code[i++];
            tokens.push_back({NUMBER, num});
        } else if (isalpha(code[i])) {
            std::string var;
            while (isalpha(code[i])) var += code[i++];
            if (var == "e") tokens.push_back({FUNC, var});
            else tokens.push_back({VARIABLE, var});
        } else {
            switch (code[i]) {
                case '+': case '-': case '*': case '/': case '^':
                    tokens.push_back({OPERATOR, std::string(1, code[i++])}); break;
                case '=': tokens.push_back({ASSIGN, "="}); i++; break;
                case '(': tokens.push_back({LPAREN, "("}); i++; break;
                case ')': tokens.push_back({RPAREN, ")"}); i++; break;
                default: i++; break;
            }
        }
    }
    tokens.push_back({END, ""});
    return tokens;
}