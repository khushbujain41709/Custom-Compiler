#include "icg.h"
#include <sstream>
#include <string>
#include <vector>
#include <functional>
using namespace std;
static int tempCount = 1;

vector<string> generate3AC(Node* root) {
    vector<string> code;
    tempCount = 1;
    
    function<string(Node*)> visit = [&](Node* node) -> string {
        if (!node) return "";
        
        // Handle leaf nodes (values/variables)
        if (!node->left && !node->right) {
            return node->value;
        }
        
        // Handle KHUS pattern first
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
        if (node->value == "/") {
            // Check for KHUS pattern: (2*x*y)/(x+y)
            if (node->left && node->left->value == "*") {
                Node* left_mult = node->left;
                // Case 1: (2*(x*y))
                if (left_mult->left && left_mult->left->value == "2" && 
                    left_mult->right && left_mult->right->value == "*") {
                    Node* inner_mult = left_mult->right;
                    if (inner_mult->left && inner_mult->right) {
                        string x = inner_mult->left->value;
                        string y = inner_mult->right->value;
                        
                        if (node->right && node->right->value == "+" &&
                            node->right->left && node->right->right &&
                            ((node->right->left->value == x && node->right->right->value == y) ||
                             (node->right->left->value == y && node->right->right->value == x))) {
                            string temp = "t" + to_string(tempCount++);
                            code.push_back(temp + " = TAYLOR " + x + " " + y);
                            return temp;
                        }
                    }
                }
                // Case 2: ((2*x)*y)
                else if (left_mult->left && left_mult->left->value == "*" &&
                         left_mult->left->left && left_mult->left->left->value == "2" &&
                         left_mult->left->right && left_mult->right) {
                    string x = left_mult->left->right->value;
                    string y = left_mult->right->value;
                    
                    if (node->right && node->right->value == "+" &&
                        node->right->left && node->right->right &&
                        ((node->right->left->value == x && node->right->right->value == y) ||
                         (node->right->left->value == y && node->right->right->value == x))) {
                        string temp = "t" + to_string(tempCount++);
                        code.push_back(temp + " = TAYLOR " + x + " " + y);
                        return temp;
                    }
                }
            }
            
            // REGULAR DIVISION - FIXED OPERAND ORDER
            string numerator = visit(node->left);
            string denominator = visit(node->right);
            string temp = "t" + to_string(tempCount++);
            code.push_back(temp + " = " + numerator + " / " + denominator);
            return temp;
        }
        
        // Handle unary operators
        if (!node->right){
            string operand = visit(node->left);
            string temp = "t" + to_string(tempCount++);
            code.push_back(temp + " = " + node->value + " " + operand);
            return temp;
        }
        
        // Handle binary operators (except division which we handled above)
        string left = visit(node->left);
        string right = visit(node->right);
        string temp = "t" + to_string(tempCount++);
        
        // Special handling for assignment
        if (node->value == "=") {
            code.push_back("y = " + right);
            return left;
        }
        
        // Default binary operation
        code.push_back(temp + " = " + left + " " + node->value + " " + right);
        return temp;
    };
    
    if (root) visit(root);
    return code;
}