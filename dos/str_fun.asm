;------------------------------------------------------------------------------
; Implementations of memchr, memcpy, memcmp, strlen, strchr, strcpy and strcmp
; using cdecl calling convention and assembly's string functions. 
;                     Copyright (c) 2021 Mochalov Nikita
;          *                  *
;              __                *
;           ,db'    *     *
;          ,d8/       *        *    *
;          888
;          `db\       *     *
;            `o`_                    **
;       *               *   *    _      *
;             *                 / )
;          *    (\__/) *       ( (  *
;        ,-.,-.,)    (.,-.,-.,-.) ).,-.,-.
;       | @|  ={      }= | @|  / / | @|o |
;      _j__j__j_)     `-------/ /__j__j__j_
;      ________(               /___________
;       |  | @| \              || o|O | @|
;       |o |  |,'\       ,   ,'"|  |  |  |
;      vV\|/vV|`-'\  ,---\   | \Vv\hjwVv\//v
;                 _) )    `. \ /
;                (__/       ) )
;                          (_/
;------------------------------------------------------------------------------
.model tiny
.code
org 100h

start:
                ; Testing memchr
                push 4   ; count
                push 'l' ; ch
                push offset exampleString
                call memchr
                add sp, 5 * 2

                ; Testing strlen
                push offset exampleString
                mov bx, cs
                mov es, bx
                call strlen
                add sp, 3 * 2

                mov ax, 4c00h
                int 21h

;------------------------------------------------------------------------------
; Finds the first occurence of ch in first count characters of the memory 
; pointed to by src.
;
; Expects: [bp + 4] = src
;          [bp + 6] = ch
;          [bp + 8] = count
;          ES = segment of src
;
; Returns: AX = pointer to the location of the character found, or 0 if not
;               found
; Changes: 
; Details: (none)
;------------------------------------------------------------------------------
memchr          proc 

                push bp
                mov bp, sp

                cld

                mov di, [bp + 4]
                mov al, [bp + 6]
                mov cx, [bp + 8]

                repne scasb 
                or cx, cx  
                jz MEMCHR_NOT_FOUND
                mov ax, di 
                dec ax
MEMCHR_NOT_FOUND:

                pop bp

                ret
                endp

;------------------------------------------------------------------------------
; Copies count bytes from src to dest.
;
; Expects: [bp + 4] = dest
;          [bp + 6] = src
;          [bp + 8] = count
;          ES = dest segment
;          DS = src segment
;
; Returns: (none)
; Changes: 
; Details: (none)
;------------------------------------------------------------------------------
memcpy          proc 

                push bp
                mov bp, sp

                mov di, [bp + 4]
                mov si, [bp + 6]
                mov cx, [bp + 8]

                cld
                rep movsb

                pop bp

                ret
                endp

;------------------------------------------------------------------------------
; Compares the first count characters of str1 and str2. The comparison is done 
; lexicographically. The sign of the result is the sign of the difference 
; between the values of the first pair of bytes that differ in the strings 
; being compared.
;
; Expects: [bp + 4] = str1
;          [bp + 6] = str2
;          [bp + 8] = count
;          DS = str1 segment
;          ES = str2 segment
;
; Returns: AX = 0 if str1 == str2, < 0 if str1 < str2 and > 0 if str1 > str2
; Changes: 
; Details: (none)
;------------------------------------------------------------------------------
memcmp          proc 

                push bp
                mov bp, sp

                mov si, [bp + 4]
                mov di, [bp + 6]
                mov cx, [bp + 8]

                xor ax, ax

                cld
                repe cmpsb
                or cx, cx
                jz MEMCMP_EQUAL
                mov ax, ds:[si]
                sub ax, es:[di]

MEMCMP_EQUAL:   
                pop bp

                ret
                endp

;------------------------------------------------------------------------------
; Returns the length of the given null-terminated byte string, that is, the 
; number of characters in a character array whose first element is pointed to 
; by str up to and not including the first null character.
;
; Expects: [bp + 4] = str
;          ES       = str segment
;          
;
; Returns: AX = str length
; Changes: 
; Details: (none)
;------------------------------------------------------------------------------
strlen          proc 

                push bp
                mov bp, sp

                mov di, [bp + 4]
                mov al, 0 

                xor cx, cx
LOOP_STRLEN:
                scasb
                je LOOP_STRLEN_END
                inc cx
                jmp LOOP_STRLEN
LOOP_STRLEN_END:

                mov ax, cx
                pop bp
                ret
                endp

exampleString   db 'Hello, World 12345', 0

end start