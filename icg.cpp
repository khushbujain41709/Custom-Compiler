#include "icg.h"
#include <sstream>
#include <string>
#include <vector>
#include <functional>
using namespace std;

static int tempCount = 1;

vector<string> generate3AC(Node* root) {
    vector<string> code;
    int tempCount = 1;  // Start from t1 (conventional in TAC)

    function<string(Node*)> visit = [&](Node* node) -> string {
        if (!node) return "";

        // Handle leaf nodes (variables/constants)
        if (!node->left && !node->right) {
            return node->value;
        }

        // Handle unary operators
        if (!node->right) {
            string operand = visit(node->left);
            string temp = "t" + to_string(tempCount++);
            code.push_back(temp + " = " + node->value + " " + operand);
            return temp;
        }

        // Handle binary operators and assignments
        string left = visit(node->left);
        string right = visit(node->right);
        string temp = "t" + to_string(tempCount++);

        // Special case for assignment (don't create new temp)
        if (node->value == "=") {
            code.push_back(left + " = " + right);
            return left;
        }

        // Normal binary operation
        code.push_back(temp + " = " + left + " " + node->value + " " + right);
        return temp;
    };

    if (root) visit(root);
    return code;
}