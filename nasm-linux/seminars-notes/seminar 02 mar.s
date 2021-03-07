// Further reading:
// - stdlog
// - курс Хорошилова
// - know the order in which registers are used in one's compiler
// - windows sdt
// - 

// Ho to assemble (-s to remove debugger info from .o):
// gcc -c seminar.s              |;| ld -s seminar.o
// as -a seminar.s > seminar.lst |;| ld -s seminar.o

// Debugging
// - gdb
// - edb = extended debugger
// - ddd = data driven debugger
// - ida (eda) ? = disasm
// - radar 2     = disasm
// - 

// Can't
// - read ports
// - use  segment register (rax e.g. is already 64bit --> no need)
//   |
//   --> cpu uses segment registers for managing access rights
// -  



.intel_syntax noprefix // noprefix = no % and no $
.text
                .globl _start

_start:
                mov eax, 0x04 // write(ebx, ecx, edx)
                mov ebx, 1    // stdout, 0 - stdin, 2 - stderr
                mov ecx, Msg  
                mov edx, 10   // strlen (Msg)
                int 0x80

                mov eax, 0x01 // exit(ebx)
                xor ebx, ebx 
                int 0x80

// if you put this as an assembly listing to your 
// C or C++ code and use intel syntax then you're
// obligated to write this for the compiler to 
// switch
.att_syntax prefix // to use att syntax

// can't put data in code segment - segfault is 
.data

// .string = db
Msg: .string "__Hllwrld\n"



//===================NASM===================
// -m elf_i386 = to specify 32-bit
// nasm -f elf 1-nasm.s |;| ld -s -m elf_i386 -o seminar seminar.o 
section .text 
global _start 

_start: 

                mov eax, 4
                mov ebx, 1
                mov ecx, Msg 
                mov edx, MsgLen 
                int 0x80

                mov eax, 1
                xor ebx, ebx 
                int 0x80

section .data 
Msg: db "__Hllwrld", 0x0a
MsgLen equ $ - Msg



//===================NASM-64===================
// nasm -f elf64 seminar.s |;| ld -s seminar.o 
section .text 
global _start 

; predefined enty point name for ld
_start: 
                mov rax, 0x01   ; write64(rdi, rsi, rdx) ... r10, r8, r9
                mov rdi, 1      ; stdout
                mov rsi, Msg 
                mov rdx, MsgLen ; strlen(Msg)
                syscall

                mov rax, 0x3c   ; exit64(rdi)
                xor rdi, rdi 
                syscall

section .data 
Msg: db "__Hllwrld", 0x0a // "\n" still can be used 
MsgLen equ $ - Msg



//===================Additional Features===================
mov rax, [Array + rsi + rdi*4] // *2, *4, *8

lea rax, [Array + rsi + rdi*4] // only one cpu ?tick?
add rax, rdi 
mov rax, [rax] // costs one tick more than "mov rbx, [rax]"

