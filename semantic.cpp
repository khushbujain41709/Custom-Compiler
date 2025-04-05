// ============================
// File: semantic.cpp
// ============================
#include "semantic.h"
#include <iostream>
#include <unordered_set>

void collectDeclaredVariables(Node* node, std::unordered_set<std::string>& declared) {
    if (!node) return;

    // If it's a function assignment like e(x) = ..., declare x
    if (node->value == "=" && node->left && node->left->right) {
        declared.insert(node->left->right->value);  // x in e(x)
    }

    collectDeclaredVariables(node->left, declared);
    collectDeclaredVariables(node->right, declared);
}

void checkSemanticErrors(Node* node, const std::unordered_set<std::string>& declared) {
    if (!node) return;

    // Check for division by zero
    if (node->value == "/" && node->right && node->right->value == "0") {
        std::cerr << "Semantic Error: Division by zero." << std::endl;
        exit(1);
    }

    // Check for variable usage without declaration
    if (!node->left && !node->right && isalpha(node->value[0])) {
        if (declared.find(node->value) == declared.end()) {
            std::cerr << "Semantic Error: Undeclared variable used -> " << node->value << std::endl;
            exit(1);
        }
    }

    checkSemanticErrors(node->left, declared);
    checkSemanticErrors(node->right, declared);
}

void semanticCheck(Node* root) {
    std::unordered_set<std::string> declared;
    collectDeclaredVariables(root, declared);
    checkSemanticErrors(root, declared);
}
