section .data
global x, t1
x: dd 0
t1: dd 0
t10: dd 0
t11: dd 0
t2: dd 0
t3: dd 0
t4: dd 0
t5: dd 0
t6: dd 0
t7: dd 0
t8: dd 0
t9: dd 0

section .text
global _start

_start:

_pow:
    ; Input: eax = base, ecx = exponent
    ; Output: eax = result
    push ebx
    mov ebx, eax
    mov eax, 1
    cmp ecx, 0
    je .pow_end
.pow_loop:
    imul eax, ebx
    dec ecx
    jnz .pow_loop
.pow_end:
    pop ebx
    ret

    ; t1 =  e x
    mov [t1], eax

    ; t2 = 1 + x
    mov eax, 1
    add eax, [x]
    mov [t2], eax

    ; t3 = x ^ 2
    mov eax, [x]
    mov ecx, 2
    call _pow
    mov [t3], eax

    ; t4 = t3 / 2
    mov eax, [t3]
    cdq
    mov ebx, 2
    idiv ebx
    mov [t4], eax

    ; t5 = t2 + t4
    mov eax, [t2]
    add eax, [t4]
    mov [t5], eax

    ; t6 = x ^ 3
    mov eax, [x]
    mov ecx, 3
    call _pow
    mov [t6], eax

    ; t7 = t6 / 6
    mov eax, [t6]
    cdq
    mov ebx, 6
    idiv ebx
    mov [t7], eax

    ; t8 = t5 + t7
    mov eax, [t5]
    add eax, [t7]
    mov [t8], eax

    ; t9 = x ^ 4
    mov eax, [x]
    mov ecx, 4
    call _pow
    mov [t9], eax

    ; t10 = t9 / 24
    mov eax, [t9]
    cdq
    mov ebx, 24
    idiv ebx
    mov [t10], eax

    ; t11 = t8 + t10
    mov eax, [t8]
    add eax, [t10]
    mov [t11], eax

    ; t1 = t11
    mov [t1], eax

    ; Exit program
    mov eax, 1
    xor ebx, ebx
    int 0x80
