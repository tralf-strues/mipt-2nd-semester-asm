global _start 

%macro check_flush 0 
                cmp rdi, buffer_end
                jb %%skip_flush         ; if rdi >= buffer_end
                call flushBuffer        ; then flush
%%skip_flush:
%endmacro

%macro finish_case 0
                add r10, 8              ; r10 = pos of the next argument
                inc rsi                 ; rsi++ to skip current letter
                jmp .loop_parse
%endmacro

section .text 

_start: 
                "I %s %x %d%%%c%b", 0
                push 255
                push 33
                push 100
                push 3802
                push test_1st_str
                push test_fstring

                call writeFormatted
                add rsp, 40

                mov rax, EXIT_CMD 
                xor rdi, rdi            ; return value = 0
                syscall

; FIXME: RDI RSI RDX RCX R8 R9
; FIXME: RCX R11

;------------------------------------------------------------------------------
; Writes formatted output to stdout.
;
; Expects: [RBP + 16] = formatted string
;          [RBP + 24] = arg1
;          [RBP + 32] = arg2
;               ...
;
; Returns: RAX = number of successfully written characters or -1 if an error 
;                occurred.
; Changes: RAX, RBX, RCX, RDX, RDI, RSI, R8, R9, R10, R12
; Details: You can use either of these argument specifications
; 
;            Symb  Bytes            Description 
;          +------+-----+----------------------------------+
;          |  %c  |  1  |  ASCII character                 |
;          +------+-----+----------------------------------+
;          |  %s  |  8  |  null-terminated string          |
;          +------+-----+----------------------------------+
;          |  %d  |  4  |  decimal     integer (signed)    |
;          +------+-----+----------------------------------+
;          |  %u  |  4  |  decimal     integer (unsigned)  |
;          +------+-----+----------------------------------+
;          |  %x  |  4  |  hexadecimal integer (unsigned)  |
;          +------+-----+----------------------------------+
;          |  %o  |  4  |  octal       integer (unsigned)  |
;          +------+-----+----------------------------------+
;          |  %b  |  4  |  binary      integer (unsigned)  |
;          +------+-----+----------------------------------+
;           
;          To write symbol '%' you should pass '%%'.
;------------------------------------------------------------------------------
writeFormatted:
                push rbp
                mov rbp, rsp
                mov rsi, [rbp + 16]     ; formatted string
                lea r10, [rbp + 24]     ; r10 = rbp + 24 = arg1      
                mov rdi, buffer         ; rdi = current char pos in output buffer   

                xor r8, r8              ; r8  = 0 for later use of r8b
                xor r12, r12            ; r12 = 0 = number of characters 
                                        ;           transmitted to stdout
; Loop that parses format string
.loop_parse:
                mov r8b, [rsi]          ; r8b = current symbol

                test r8b, r8b  
                jz .loop_parse_end

                cmp r8b, '%'
                jne .write_char

.parse_arg: 
                inc rsi
                mov r8b, [rsi]

                cmp r8b, '%'
                je .write_char

                cmp r8b, 'b'
                jb .invalid_spec        ; invalid specification

                cmp r8b, 'x'
                ja .invalid_spec        ; invalid specification

                sub r8b, 'b'            ; r8b -= 'b'
                jmp qword [writef_jmp_table + 8 * r8]

.invalid_spec:
                mov byte [conversion_error], 1

.write_char:
                mov byte [rdi], r8b   
                inc rdi    
                inc rsi
                inc r12

                check_flush
                jmp .loop_parse

.loop_parse_end:

                cmp rdi, buffer
                je .skip_flush          ; if there're symbols left in the buffer then
                call flushBuffer        ; flush them
.skip_flush:

                mov rax, r12            ; rax = number of characters transmitted to stdout
                ; Checking whether there was an error
                cmp byte [conversion_error], 0
                je .no_error  
                mov rax, -1             ; rax = error code
.no_error:      

                pop rbp
                ret

;------------------------------------------------------------------------------
; Writes a char to the buffer and flushes the buffer if needed.
;
; Changes: RAX, RDX, RDI, RSI, R9, R12
;------------------------------------------------------------------------------
.char_case:
                mov r9, qword [r10]     ; r9 = current argument
                mov byte [rdi], r9b       
                inc rdi
                inc r12
                check_flush
                finish_case

;------------------------------------------------------------------------------
; Writes null-terminated string to the buffer and flushes the buffer if needed.
;
; Changes: RAX, RDX, RDI, RSI, R9, R12 
;------------------------------------------------------------------------------
.str_case:
                mov r9, qword [r10]     ; r9 = current argument

.loop_str:
                mov al, byte [r9]
                test al, al    
                jz .loop_str_end        ; if reached 0 exit the loop  

                mov byte [rdi], al
                inc r9 
                inc rdi
                inc r12

                check_flush
                jmp .loop_str
.loop_str_end:

                finish_case

;------------------------------------------------------------------------------
; Writes an unsigned decimal integer.
;
; Expects: RAX = number to be written
; Changes: RAX, RBX, ECX, RDX, RDI, R9  
;------------------------------------------------------------------------------
.uint_write:
                mov ecx, 10
                mov rdi, itostr_tmp
                call intToString
                pop rdi

                mov r9, itostr_tmp
                call writeString
                finish_case

;------------------------------------------------------------------------------
; Writes a signed decimal integer.
;
; Changes: RAX, RBX, RCX, RDX, RDI, R9  
;------------------------------------------------------------------------------
.sint_case:
                mov r9, qword [r10]     ; r9 = current argument

                mov rcx, 0x80000000
                test r9, rcx 
                jz .positive            ; check if positive (msb is 0) 

                not r9 
                inc r9

                mov byte [rdi], '-'
                inc rdi  
.positive:
                push rdi
                mov rax, r9
                jmp .uint_write

;------------------------------------------------------------------------------
; Writes an unsigned decimal integer.
;
; Changes: RAX, RBX, RCX, RDX, RDI, R9  
;------------------------------------------------------------------------------
.uint_case:
                push rdi
                mov eax, dword [r10]    ; eax = current argument
                jmp .uint_write

;------------------------------------------------------------------------------
; Writes an unsigned integer in the base that is a power of two.
;
; Expects: CH = mask  (shift number of 1's, i.e. 2^shift - 1) 
;          CL = shift (for bin - 1, oct - 3, hex - 4, etc.)
; Changes: RAX, RBX, RDX, RDI, RSI, R9 
;------------------------------------------------------------------------------
.pow_two_int_case:
                push rdi
                mov eax, dword [r10]    ; eax = current argument
                mov rdi, itostr_tmp
                call intToString2
                pop rdi

                mov r9, itostr_tmp
                call writeString
                finish_case

;------------------------------------------------------------------------------
; Writes a hexadecimal unsigned integer.
;
; Changes: RAX, RBX, CX, RDX, RDI, RSI, R9 
;------------------------------------------------------------------------------
.xint_case:
                mov ch, 16 - 1
                mov cl, 4
                jmp .pow_two_int_case

;------------------------------------------------------------------------------
; Writes an octal unsigned integer.
;
; Changes: RAX, RBX, CX, RDX, RDI, RSI, R9 
;------------------------------------------------------------------------------
.oint_case:
                mov ch, 8 - 1
                mov cl, 3
                jmp .pow_two_int_case

;------------------------------------------------------------------------------
; Writes a binary unsigned integer.
;
; Changes: RAX, RBX, CX, RDX, RDI, RSI, R9 
;------------------------------------------------------------------------------
.bint_case:
                mov ch, 2 - 1
                mov cl, 1
                jmp .pow_two_int_case

;------------------------------------------------------------------------------
; Flushes the buffer up to RDI (the latter not including). After that sets RDI 
; to the beginning of the buffer.
;
; Expects: RDI = the address up to which flush the buffer
;
; Returns: (none)
; Changes: RAX, RDX, RDI = buffer, RSI
; Details: (none)
;------------------------------------------------------------------------------
flushBuffer:
                mov rax, WRITE_CMD      ; write(rdi=fd, rsi=buf, rdx=cnt) 
                mov rsi, buffer

                mov rdx, rdi
                sub rdx, buffer         ; rdx = rdi - buffer = cur buffer size

                mov rdi, STDOUT      
                syscall

                mov rdi, buffer

                ret 

;------------------------------------------------------------------------------
; Converts integer to string in the specified numeral system and puts the 
; result to [RDI].
; 
; Expects: EAX = number to convert 
;          ECX = base of a numeral system
;          RDI = buffer address
;
; Returns: RBX = length of the string
;
; Changes: RAX, RBX, RDX, RDI, R9B 
;
; Details: The string is terminated with 0, and it isn't inluded in length in 
;          register RBX.
;------------------------------------------------------------------------------
intToString:
                
                mov rbx, rdi
.loop_convert:
                xor rdx, rdx            ; rdx = 0 for division to work 
                div ecx

                mov r9b, [digits_letters + rdx]
                mov byte [rbx], r9b

                inc rbx
                test eax, eax
                jnz .loop_convert
.loop_convert_end: 

                mov byte [rbx], 0
        
                ; Inversing the result string
                mov rax, rdi            ; saving rdi
                push rbx                ; saving rbx 

                dec rbx
.loop_str_inverse:
                cmp rdi, rbx
                jae .loop_str_inverse_end

                mov r9b, byte [rdi]
                xchg byte [rbx], r9b 
                mov byte [rdi], r9b

                inc rdi
                dec rbx 

                jmp .loop_str_inverse
.loop_str_inverse_end:
                                        
                pop rbx                 ; restoring rbx
                mov rdi, rax            ; restoring rdi
                sub rbx, rdi

                ret 

;------------------------------------------------------------------------------
; Converts integer to string in the numeral system that is the power of 2 and 
; puts the result to [RDI].
; 
; Expects: EAX = number to convert 
;          CH  = mask  (shift number of 1's)
;          CL  = shift (for bin - 1, oct - 3, hex - 4, etc.)
;          RDI = buffer address
;
; Returns: RBX = length of the string
;
; Changes: RAX, RBX, RDX, RDI, R9B 
;
; Details: The string is terminated with 0, and it isn't inluded in length in 
;          register RBX.
;------------------------------------------------------------------------------
intToString2:
                mov rbx, rdi
.loop_convert:
                test eax, eax
                jz .loop_convert_end

                mov dl, al
                and dl, ch

                mov r9b, [digits_letters + rdx]
                mov byte [rbx], r9b

                inc rbx
                shr eax, cl

                jmp .loop_convert
.loop_convert_end: 

                mov byte [rbx], 0
        
                ; Inversing the result string
                mov rax, rdi            ; saving rdi
                push rbx                ; saving rbx 

                dec rbx
.loop_str_inverse:
                cmp rdi, rbx
                jae .loop_str_inverse_end

                mov r9b, byte [rdi]
                xchg byte [rbx], r9b 
                mov byte [rdi], r9b

                inc rdi
                dec rbx 

                jmp .loop_str_inverse
.loop_str_inverse_end:
                
                pop rbx                 ; restoring rbx
                mov rdi, rax            ; restoring rdi
                sub rbx, rdi

                ret 

;------------------------------------------------------------------------------
; Writes a null-terminated string to the buffer and flushes the buffer if 
; needed.
;
; Expects: R9  = string to be written
;          RDI = address in the buffer where to write the char
;
; Returns: (none)
; Changes: RAX
;          RDX
;          RDI = next position in buffer
;          RSI
;          R9  = position of the first 0
;          R12 
; Details: (none)
;------------------------------------------------------------------------------
writeString:
.loop_write:
                mov al, byte [r9]
                test al, al    
                jz .loop_end            ; if reached 0 exit the loop  

                mov byte [rdi], al
                inc r9 
                inc rdi
                inc r12                 ; symbols transmitted to stdout ++

                check_flush

                jmp .loop_write
.loop_end:
                ret

section .data 

WRITE_CMD        equ 0x01
STDOUT           equ 0x01
EXIT_CMD         equ 0x3c
ITOSTR_TMP_SIZE  equ 32
BUFFER_SIZE      equ 512

conversion_error db 0 
digits_letters   db   "0123456789ABCDEF"

writef_jmp_table:
                                    dq writeFormatted.bint_case    ; b
                                    dq writeFormatted.char_case    ; c
                                    dq writeFormatted.sint_case    ; d
                times 'o' - 'd' - 1 dq writeFormatted.invalid_spec ; symbols after d and before o
                                    dq writeFormatted.oint_case    ; o
                times 's' - 'o' - 1 dq writeFormatted.invalid_spec ; symbols after o and before s
                                    dq writeFormatted.str_case     ; s
                times 'u' - 's' - 1 dq writeFormatted.invalid_spec ; symbols after s and before u
                                    dq writeFormatted.uint_case    ; u
                times 'x' - 'u' - 1 dq writeFormatted.invalid_spec ; symbols after u and before x
                                    dq writeFormatted.xint_case    ; x
                                    
test_fstring:   db "I %s %x %d%%%c%b", 0
test_1st_str:   db "Tralf", 0
test_2nd_str:   db "not you", 0

section .bss
itostr_tmp      resb ITOSTR_TMP_SIZE
buffer          resb BUFFER_SIZE
buffer_end: