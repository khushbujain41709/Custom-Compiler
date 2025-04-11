#include "semantic.h"
#include <iostream>
#include <unordered_set>
using namespace std;
void collectDeclaredVariables(Node* node, unordered_set<string>& declared) {
    if (!node) return;

    // If it's a leaf node and a variable (not a number), declare it
    if (!node->left && !node->right && isalpha(node->value[0])) {
        declared.insert(node->value);
    }

    collectDeclaredVariables(node->left, declared);
    collectDeclaredVariables(node->right, declared);
}

void checkSemanticErrors(Node* node, const unordered_set<string>& declared) {
    if (!node) return;

    // Check for division by zero
    if (node->value == "/" && node->right && node->right->value == "0") {
        cerr << "Semantic Error: Division by zero." << endl;
        exit(1);
    }

    // Check for variable usage without declaration
    if (!node->left && !node->right && isalpha(node->value[0])) {
        if (declared.find(node->value) == declared.end()) {
            cerr << "Semantic Error: Undeclared variable used -> " << node->value << endl;
            exit(1);
        }
    }

    checkSemanticErrors(node->left, declared);
    checkSemanticErrors(node->right, declared);
}

void semanticCheck(Node* root) {
    unordered_set<string> declared;
    collectDeclaredVariables(root, declared);
    checkSemanticErrors(root, declared);
}
