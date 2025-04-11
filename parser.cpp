#include "parser.h"
#include <stack>
#include <map>
using namespace std;
int precedence(const string& op) {
    if (op == "^") return 4;  // Highest precedence
    if (op == "*" || op == "/") return 3;
    if (op == "+" || op == "-") return 2;
    return 0;  // Lowest (for parentheses, assignments)
}

bool isRightAssociative(const string& op) {
    return op == "^";  // Exponentiation is right-associative
}

Node* parse(const vector<Token>& tokens) {
    stack<Node*> operands;
    stack<string> operators;
    auto applyOperator = [&]() {
        string op = operators.top();
        operators.pop();
        Node* right = operands.top();
        operands.pop();
        Node* left = operands.top();
        operands.pop();
        operands.push(new Node{op, left, right});
    };

    for (size_t i = 0; i < tokens.size(); ++i) {
        const Token& tok = tokens[i];
        if (tok.type == NUMBER || tok.type == VARIABLE) {
            operands.push(new Node{tok.value, nullptr, nullptr});
        }
        else if (tok.type == LPAREN) {
            operators.push("(");
        }
        else if (tok.type == RPAREN) {
            while (!operators.empty() && operators.top() != "(") {
                applyOperator();
            }
            if (!operators.empty()) operators.pop();  // Pop '('
        }
        else if (tok.type == OPERATOR || tok.type == ASSIGN) {
            while (!operators.empty() &&
                   operators.top() != "(" &&
                   ((isRightAssociative(tok.value) ?
                       (precedence(tok.value) < precedence(operators.top())) :
                       (precedence(tok.value) <= precedence(operators.top()))))) {
                applyOperator();
            }
            operators.push(tok.value);
        }
    }

    while (!operators.empty()) {
        applyOperator();
    }

    return operands.empty() ? nullptr : operands.top();
}
    /*
    
    KHUS
      /
    /   \
   *     +
 / \     / \
2   *   x   y
   / \
  x   y

    */

