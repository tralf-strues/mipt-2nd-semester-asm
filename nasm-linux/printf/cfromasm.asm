global main
extern printf

section .text 

main:
                mov rdi, test_fstring
                mov rsi, test_1st_str
                mov rdx, 3802
                mov rcx, test_2nd_str
                mov r8,  test_3rd_str
                mov r9,  100
                push 255
                push 33

                xor rax, rax
                call printf 

                mov rax, 0x3c 
                xor rdi, rdi            ; return value = 0
                syscall

section .data 

test_fstring:   db "I %s %x %s %s %d%%%c%x", 10, 0
test_1st_str:   db "love", 0
test_2nd_str:   db "and", 0
test_3rd_str:   db "you", 0
