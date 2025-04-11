#include "codegen.h"
#include <bits/stdc++.h>
#include <set>
#include <sstream>
#include <vector>
#include <string>
using namespace std;
void generateAssembly(const vector<string>& tac, ostream& out) {
    set<string> variables;

    // Collect all variables used in TAC
    for (const auto& line : tac) {
        istringstream iss(line);
        string token;
        vector<string> parts;
        while (iss >> token) parts.push_back(token);

        for (const auto& part : parts) {
            if (!part.empty() && !isdigit(part[0]) && part != "=" && part != "+" && part != "-" &&
                part != "*" && part != "/" && part != "^" && part != "TAYLOR") {
                variables.insert(part);
            }
        }
    }

    // Data section
    out << "section .data\n";
    out << "global x, t1\n";
    out << "x: dd 0\n";
    out << "t1: dd 0\n";

    // Defines other variables found in TAC, initializing them to 0.
    for (const auto& var : variables) {
        if (var != "x" && var != "t1") {
            out << var << ": dd 0\n";
        }
    }

    // Code section
    out << "\nsection .text\n";
    out << "global _start\n\n";
    out << "_start:\n";

    // _pow function
    // A loop that calculates eax = eax^ecx
    out << "\n_pow:\n"
        << "    push ebx\n"
        << "    mov ebx, eax\n"
        << "    mov eax, 1\n"
        << "    cmp ecx, 0\n"
        << "    je .pow_end\n"
        << ".pow_loop:\n"
        << "    imul eax, ebx\n"
        << "    dec ecx\n"
        << "    jnz .pow_loop\n"
        << ".pow_end:\n"
        << "    pop ebx\n"
        << "    ret\n\n";

    // KHUS function
    out << "TAYLOR:\n"
        << "    push ecx\n"
        << "    push edx\n"
        << "    mov ecx, eax\n"
        << "    imul eax, ebx\n"
        << "    add eax, eax\n"
        << "    add ecx, ebx\n"
        << "    cdq\n"
        << "    idiv ecx\n"
        << "    pop edx\n"
        << "    pop ecx\n"
        << "    ret\n\n";

    // Translate each TAC instruction
    for (const auto& line : tac) {
        istringstream iss(line);
        string dest, assign, token;
        iss >> dest >> assign;

        vector<string> parts;
        while (iss >> token) parts.push_back(token);

        out << "    ; " << line << "\n";

        if (parts.size() == 3 && parts[0] == "TAYLOR") {
            // Function-style KHUS instruction
            string src1 = parts[1];
            string src2 = parts[2];

            out << "    mov eax, " << (isdigit(src1[0]) ? src1 : "[" + src1 + "]") << "\n";
            out << "    mov ebx, " << (isdigit(src2[0]) ? src2 : "[" + src2 + "]") << "\n";
            out << "    call TAYLOR\n";
        } 
        else if (parts.size() == 3) {
            string src1 = parts[0], op = parts[1], src2 = parts[2];
            string src1_ref = isdigit(src1[0]) ? src1 : "[" + src1 + "]";
            string src2_ref = isdigit(src2[0]) ? src2 : "[" + src2 + "]";

            if (op == "+") {
                out << "    mov eax, " << src1_ref << "\n";
                out << "    add eax, " << src2_ref << "\n";
            } 
            else if (op == "-") {
                out << "    mov eax, " << src1_ref << "\n";
                out << "    sub eax, " << src2_ref << "\n";
            } 
            else if (op == "*") {
                out << "    mov eax, " << src1_ref << "\n";
                out << "    imul eax, " << src2_ref << "\n";
            } 
            else if (op == "/") {
                out << "    mov eax, " << src1_ref << "\n";
                out << "    cdq\n"; 
                out << "    mov ebx, " << src2_ref << "\n";
                out << "    idiv ebx\n";
            } 
            else if (op == "^") {
                out << "    mov eax, " << src1_ref << "\n";
                out << "    mov ecx, " << src2_ref << "\n";
                out << "    call _pow\n";
            }
        }

        out << "    mov [" << dest << "], eax\n\n";
    }

    // Exit syscall
    out << "    mov eax, 1\n"
        << "    xor ebx, ebx\n"
        << "    int 0x80\n";
}
