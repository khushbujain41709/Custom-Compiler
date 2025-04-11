# This Python script handles custom assembly processing :

# assemble_custom_code function :
# Reads assembly file
# Replaces standalone "TAYLOR eax, ebx" instructions with custom bytecode (0x0F, 0xAB)
# Writes modified assembly to temp.asm
# Assembles and links using nasm and gcc

# disassemble_and_patch function :
# Runs objdump to disassemble the executable
# Patches disassembly to show "TAYLOR" instead of raw bytes

import subprocess
import re
# subprocess : Used to run external shell commands like nasm, gcc, objdump.
# re : Python’s regular expressions module for pattern matching and substitution.

def assemble_custom_code(source_file):
    with open(source_file, 'r') as f:
        code = f.read()

    pattern = r'^\s*TAYLOR\s+eax,\s*ebx\s*$'
    replacement = '    .byte 0x0F, 0xAB'
    code = re.sub(pattern, replacement, code, flags=re.MULTILINE)

    with open("temp.asm", 'w') as f:
        f.write(code)

    # Assemble and link
    subprocess.run(["nasm", "-f", "elf32", "temp.asm", "-o", "temp.o"], check=True)
    subprocess.run(["gcc", "-m32", "temp.o", "-o", "temp.exe", "-nostartfiles"], check=True)

# Runs objdump -d to disassemble the binary and get human-readable assembly instructions.
def disassemble_and_patch():
    result = subprocess.run(["objdump", "-d", "temp.exe"], capture_output=True, text=True)
    disasm = result.stdout

    # Replace machine code (optional)
    patched_disasm = re.sub(r'0f\s+ab', '   TAYLOR eax, ebx', disasm, flags=re.IGNORECASE)

    # Replace call to KHUS_instr
    patched_disasm = re.sub(r'call\s+.*<TAYLOR>', 'TAYLOR eax, ebx', patched_disasm)

    print("===== Custom Disassembly =====")
    print(patched_disasm)


if __name__ == "__main__":
    assemble_custom_code("output.asm")
    disassemble_and_patch()
