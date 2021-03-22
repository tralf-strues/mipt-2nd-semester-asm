; Calling writeFormatted from C and printf from asm
; Further read:
;   - ?mangling?
;   - compiler driver (gcc = GNU compiler collection) 
;   - Richard ?Stawman?
;   - Write in C song
;   - eax = 0 not-fixed # of parameters due to floating point numbers
;   - 

; ====C-file====
extern "C" int CallAsm      (int a, int b);
extern "C" int CallCFromAsm (int res);
; OR 
extern "C"
{
    int CallAsm      (int a, int b);
    int CallCFromAsm (int res);
}

int main()
{
    printf("Start\n");

    int res = CallAsm(22, 33);
    printf("main(): res = %d\n", res);

    printf("End\n");
}

int CallCFromAsm(int res)
{
    fprintf(stderr, "CallCFromAsm(): res = %d\n", res);
}

; ====ASM-file====
.global _CallAsm
.extern _CallCFromAsm
.extern _printf

_CallAsm:
                push ebp
                mov ebp, esp

                mov eax, [ebp+4 + 4*1] ; a
                add eax, [ebp+4 + 4*2] ; b

                push eax 
                push offset Msg
                call _printf 
                pop eax 
                pop eax 

                push eax 
                call _CallCFromAsm
                pop eax 

                pop ebp 
                ret

section .data
Msg:            .string "CallAsm(): res = %d\n"

; ====Building====
gcc file1.cpp file2.s ; will call two different compilers

; ====NASM====
gcc file1.cpp file2.o ; due to fact that nasm isn't supported by gcc

; ====64-bit====
only stdcall

Трамплин :)
pushes 6 registers to stack for printf
