#include "codegen.h"
#include <bits/stdc++.h>

#include <set>
#include <sstream>
#include <vector>
#include <string>

void generateAssembly(const std::vector<std::string>& tac, std::ostream& out) {
    // Use ordered set to preserve insert order
    std::set<std::string> variables;

    // Collect all variables used in TAC
    for (const auto& line : tac) {
        std::istringstream iss(line);
        std::string lhs, op, rhs1, rhs2;
        iss >> lhs >> op >> rhs1;
        if (iss >> op >> rhs2) {
            if (!isdigit(lhs[0])) variables.insert(lhs);
            if (!isdigit(rhs1[0])) variables.insert(rhs1);
            if (!isdigit(rhs2[0])) variables.insert(rhs2);
        }
    }

    // Ensure x and t1 are declared first and made global
    out << "section .data\n";
    out << "global x, t1\n";
    out << "x: dd 0\n";
    out << "t1: dd 0\n";

    // Now output other variables (except x and t1 again)
    for (const auto& var : variables) {
        if (var != "x" && var != "t1") {
            out << var << ": dd 0\n";
        }
    }

    // Section for code
    out << "\nsection .text\n";
    out << "global _start\n\n";
    out << "_start:\n";

    // _pow function
    out << "\n_pow:\n"
        << "    ; Input: eax = base, ecx = exponent\n"
        << "    ; Output: eax = result\n"
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

    // Main instruction conversion
    for (const auto& line : tac) {
        std::istringstream iss(line);
        std::string dest, assign, src1, op, src2;
        iss >> dest >> assign >> src1 >> op >> src2;

        out << "    ; " << line << "\n";

        std::string src1_ref = isdigit(src1[0]) ? src1 : "[" + src1 + "]";
        std::string src2_ref = isdigit(src2[0]) ? src2 : "[" + src2 + "]";

        if (op == "+") {
            out << "    mov eax, " << src1_ref << "\n"
                << "    add eax, " << src2_ref << "\n";
        } else if (op == "-") {
            out << "    mov eax, " << src1_ref << "\n"
                << "    sub eax, " << src2_ref << "\n";
        } else if (op == "*") {
            out << "    mov eax, " << src1_ref << "\n"
                << "    imul eax, " << src2_ref << "\n";
        } else if (op == "/") {
            out << "    mov eax, " << src1_ref << "\n"
                << "    cdq\n"
                << "    mov ebx, " << src2_ref << "\n"
                << "    idiv ebx\n";
        } else if (op == "^") {
            out << "    mov eax, " << src1_ref << "\n"
                << "    mov ecx, " << src2_ref << "\n"
                << "    call _pow\n";
        }

        out << "    mov [" << dest << "], eax\n\n";
    }

    // Exit syscall
    out << "    ; Exit program\n"
        << "    mov eax, 1\n"
        << "    xor ebx, ebx\n"
        << "    int 0x80\n";
}

