global _start 

section .text 

_start: 
                push '!'
                push test_2nd_str
                push test_1st_str
                push test_fstring

                call writeFormatted
                add rsp, 32

                mov rax, EXIT_CMD 
                xor rdi, rdi      ; return value
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
; Returns: RAX = number of successfully written arguments
; Changes: 
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
;          |  %o  |  4  |  octal       integer (unsigned)  |
;          +------+-----+----------------------------------+
;          |  %x  |  4  |  hexadecimal integer (unsigned)  |
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

                lea r10, [rbp + 24]     ; r10 = rbp + 24        
 
                mov rdi, buffer         ; rdi = current char in output buffer   

                xor r8, r8              ; r8 = 0 for later use of r8b
; Loop that parses format string
.loop_parse:
                mov rbx, rsi            ; saving current rsi value 
                call nextSpecSymbol

                xor r8b, r8b 
                xchg byte [rsi], r8b    ; put 0 to [rsi] for writeString
                xchg rbx, rsi           ; set rsi to the beginning
                call writeString

                mov rsi, rbx            ; setting back rsi
                mov byte [rsi], r8b     ; restoring symbol in [rsi] 

                test r8b, r8b 
                jz .loop_parse_end      ; exit if reached the end

                inc rsi 
                mov r8b, [rsi]          ; symbol after specification symbol '%'
                sub r8b, 'b'            ; r8b -= 'b' because it's the first 
                                        ;            symbol in the jmp table
                inc rsi 
                
                ; TODO: jump to switch table
                mov r9, rsi             ; saving rsi
                mov rsi, qword [r10]    ; rsi = current argument
                call qword [writef_jmp_table + 8*r8]
                
                mov rsi, r9             ; restoring rsi
                add r10, 8              ; r10 = pos of the next argument

                jmp .loop_parse
.loop_parse_end:

                cmp rdi, buffer
                je .skip_flush   ; if there're symbols left in the buffer then
                call flushBuffer ; flush them
.skip_flush:

                pop rbp
                ret

;------------------------------------------------------------------------------
; Finds the next specification symbol '%'.
;
; Expects: RSI = the memory position from which to start searching  
;
; Returns: RSI = position of the first '%' or 0.
; Changes: AX, RSI
; Details: (none)
;------------------------------------------------------------------------------
nextSpecSymbol:              
                mov ah, '%'
.loop_find:     
                mov al, byte [rsi]

                test al, al
                jz .loop_end ; if reached 0 exit

                cmp al, ah 
                je .loop_end ; if found '%' exit

                inc rsi
                jmp .loop_find
.loop_end:

                ret

;------------------------------------------------------------------------------
; Writes a null-terminated string to the buffer and flushes the buffer if 
; needed.
;
; Expects: RSI = string to be written
;          RDI = address in the buffer where to write the char
;
; Returns: (none)
; Changes: RAX, RDX, RSI=position of the first 0, RDI=next position in buffer 
; Details: (none)
;------------------------------------------------------------------------------
writeString:
.loop_write:
                mov al, byte [rsi]
                test al, al    
                jz .loop_end  ; if reached 0 exit the loop  

                mov byte [rdi], al
                inc rsi 
                inc rdi

                cmp rdi, buffer_end
                jb  .skip_flush      ; if rdi >= buffer_end
                                     ; then flush
                call flushBuffer
.skip_flush:

                jmp .loop_write
.loop_end:
                ret

;------------------------------------------------------------------------------
; Writes a char to the buffer and flushes the buffer if needed.
;
; Expects: SIL = char to be written
;          RDI = address in the buffer where to write the char
;
; Returns: (none)
; Changes: RAX, RDI, RSI, RDX
; Details: (none)
;------------------------------------------------------------------------------
writeChar:
                mov byte [rdi], sil       

                inc rdi
                cmp rdi, buffer_end
                jb  .skip_flush             ; if rdi >= buffer_end
                                            ; then flush
                call flushBuffer
.skip_flush:

                ret

;------------------------------------------------------------------------------
; Flushes the buffer up to RDI (the latter not including). After that sets RDI 
; to the beginning of the buffer.
;
; Expects: RDI = the address up to which flush the buffer
;
; Returns: (none)
; Changes: RAX, RDI=buffer, RSI, RDX
; Details: (none)
;------------------------------------------------------------------------------
flushBuffer:
                mov rax, WRITE_CMD ; write(rdi=fd, rsi=buf, rdx=cnt) 
                mov rsi, buffer

                mov rdx, rdi
                sub rdx, buffer    ; rdx = rdi - buffer = cur buffer size

                mov rdi, STDOUT      
                syscall

                mov rdi, buffer

                ret 

;------------------------------------------------------------------------------
; Writes a decimal number to the buffer and flushes the buffer if needed.
;
; Expects: ESI = number to be written
;          RDI = address in the buffer where to write the number
;
; Returns: (none)
; Changes: RAX, RDI, RSI, RDX
; Details: (none)
;------------------------------------------------------------------------------
writeDecimal:
                

                ret

section .data 

WRITE_CMD       equ 0x01
STDOUT          equ 0x01
EXIT_CMD        equ 0x3c
BUFFER_SIZE     equ 512

buffer          resb BUFFER_SIZE
buffer_end:

writef_jmp_table:
                ; times '%' - 2       dq writeFormatted.DEFAULT_CASE         ; symbols before %, not including 0 

                                    ; dq writeFormatted.CNVRT_INDICATOR_CASE ; %

                ; times 'b' - '%' - 1 dq writeFormatted.DEFAULT_CASE         ; symbols after % and before b

                                    dq 0         ; b
                                    dq writeChar           ; c
                                    dq 0         ; d
                times 'o' - 'd' - 1 dq 0        ; symbols after d and before o
                                    dq 0          ; o
                times 's' - 'o' - 1 dq 0        ; symbols after o and before s
                                    dq writeString            ; s
                times 'u' - 's' - 1 dq 0        ; symbols after s and before u
                                    dq 0       ; u
                times 'x' - 'u' - 1 dq 0        ; symbols after u and before x
                                    dq 0            ; x
                                    
test_fstring:   db "Hello, %s, I'm glad to see %s again %c", 10, 0
test_1st_str:   db "Tralf", 0
test_2nd_str:   db "not you", 0
