;------------------------------------------------------------------------------
; A simple interrupt intercepter that prints contents of the registers. 
;                     Copyright (c) 2021 Mochalov Nikita
;
; Huge thanks to
; - elisfromkirov
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

TIMER_INTERRUPT    = 8
KBD_INTERRUPT      = 9

KBD_PORT           = 60h
INTERRUPT_SHORTCUT = 4fh ; End scan code

SCR_WIDTH          = 80
SCR_HEIGHT         = 24
BYTES_PER_SYMB     = 2

JMP_OPCODE         = 0eah

FRAME_WIDTH        = 9
FRAME_HEIGHT       = 6
FRAME_X            = 69
FRAME_Y            = 1
FRAME_START_POS    = BYTES_PER_SYMB * (SCR_WIDTH * FRAME_Y + FRAME_X)
CHAR_HLINE         = 0cdh
CHAR_VLINE         = 0bah
CHAR_ULEFT         = 0c9h
CHAR_URIGHT        = 0bbh
CHAR_BLEFT         = 0c8h
CHAR_BRIGHT        = 0bch
FRAME_COLOR        = 0eh

FRAME_TXT_MARGIN_X = 2 
FRAME_TXT_MARGIN_Y = 1 

.code
org 100h

start:
                xor bx, bx
                mov es, bx 

                mov bx, KBD_INTERRUPT
                mov di, offset oldKbdIntAddress
                mov si, offset kbdIntercept
                call loadIntHandler 

                mov bx, TIMER_INTERRUPT
                mov di, offset oldTimerIntAddress
                mov si, offset timerIntercept
                call loadIntHandler          

                ; Terminating the program and leaving code and data in memory
                mov ax, 3100h              ; al = exit code
                mov dx, offset PROGRAM_END ; dx = memory size to keep in 
                shr dx, 4                  ;      16-byte paragraphs
                inc dx                     ; save one extra paragraph program
                                           ; mod 16 != 0
                int 21h

;------------------------------------------------------------------------------
; Loads new interrupt handler and saves the previous one.
;
; Expects: BX = interrupt number
;          DI = where to save old interrupt handler (4 bytes!)
;          SI = new interrupt handler offset
;          ES = 0
; Returns: (none)
; Changes: AX, BX
; Details: (none)
;------------------------------------------------------------------------------
loadIntHandler  proc 

                shl bx, 2 ; *4, because each address is 4 bytes

                ; Interrupt Flag (IF) determines whether or not CPU will 
                ; respond to *maskable* hardware interrupts.
                cli ; clear interrupts, sets IF to 0
                
                ; Saves old int handler. 
                mov ax, es:[bx]
                mov [di], ax     ; int offset

                mov ax, es:[bx + 2]
                mov [di + 2], ax ; int segment

                ; Sets new int handler.
                mov es:[bx], si
                mov ax, cs
                mov es:[bx + 2], ax

                sti ; set interrupts, sets IF to 1

                ret 
                endp

;------------------------------------------------------------------------------
; Draws a frame at position x = FRAME_X, y = FRAME_Y with width and
; height equal to FRAME_WIDTH and FRAME_HEIGHT, respectively.
;
; Expects: ES = 0b800h
;             
; Returns: (none)
; Changes: AX, BX, CX, DX, DI, SI
; Details: (none)
;------------------------------------------------------------------------------
drawFrame	    proc

                mov ah, FRAME_COLOR
                mov bx, FRAME_START_POS
                mov dh, FRAME_WIDTH
                mov si, BYTES_PER_SYMB
                
                ; TOP LINE
                mov ch, CHAR_ULEFT
                mov cl, CHAR_URIGHT
                mov dl, CHAR_HLINE

                mov di, dx ; save dx (dh changes)
                call drawLine
                mov dx, di

                ; move bx to the beginning of the next line 
                add bx, BYTES_PER_SYMB * (SCR_WIDTH - FRAME_WIDTH - 1)

                ; INNER LINES
                mov ch, CHAR_VLINE
                mov cl, CHAR_VLINE
                mov dl, 0 ; to fill the frame with blank space

                mov al, FRAME_HEIGHT - 2
LOOP_FRAME:
                cmp al, 0
                je LOOP_FRAME_END 

                mov di, dx ; save dx (dh changes)
                call drawLine
                mov dx, di

                ; move bx to the beginning of the next line 
                add bx, BYTES_PER_SYMB * (SCR_WIDTH - FRAME_WIDTH - 1)

                dec al 
                jmp LOOP_FRAME
LOOP_FRAME_END: 

                ; BOTTOM LINE
                mov ch, CHAR_BLEFT
                mov cl, CHAR_BRIGHT
                mov dl, CHAR_HLINE

                call drawLine

                ret
                endp

;------------------------------------------------------------------------------
; Draws a line starting at the specified memory position.
;
; Expects: AH = color attribute
;          BX = memory position of the starting point
;          CH = char used for the beginning (ASCII)
;          CL = char used for the end       (ASCII)
;          DH = length
;          DL = char used for the line (ASCII)
;          SI = step
;	       ES = 0b800h
;             
; Returns: (none)
; Changes: BX, DH
; Details: BX after execution will be pointing to the char after the last drawn
;          char.
;------------------------------------------------------------------------------
drawLine	    proc

                mov byte ptr es:[bx],     ch
                mov byte ptr es:[bx + 1], ah
                inc bx 
                inc bx 

LOOP_LINE:
                cmp dh, 0
                je LOOP_LINE_END

                mov byte ptr es:[bx],     dl
                mov byte ptr es:[bx + 1], ah

                add bx, si
                dec dh
                jmp LOOP_LINE
LOOP_LINE_END:

                mov byte ptr es:[bx],     cl
                mov byte ptr es:[bx + 1], ah

                ret
                endp

;------------------------------------------------------------------------------
; Prints all registers.
;
; Expects: ES = 0b800h               
;
; Returns: (none)
; Changes: (none)
; Details: (none)
;------------------------------------------------------------------------------
printRegisters  proc

                push ax bx cx dx 

                push dx cx bx ax
                mov bx, BYTES_PER_SYMB * (SCR_WIDTH * (FRAME_Y + FRAME_TXT_MARGIN_Y) + ((FRAME_X + FRAME_TXT_MARGIN_X)))

PRINT_REGISTER  macro firstLetter
                
                mov byte ptr es:[bx], firstLetter
                inc bx 
                inc bx 

                mov byte ptr es:[bx], 'X'
                inc bx 
                inc bx 
                inc bx 
                inc bx 

                pop ax

                push ax cx dx si
                call printHex ; AX, CX, DX, SI
                pop si dx cx ax 

                ; Move bx to the next line. -3 is here because after printHex, 
                ; bx is at the first digit of a register's value, and we have
                ; to move it to the left by 3 ('AX 1234')
                ;                               ^  ^
                ;                               \__/
                ;                       3 symbols difference    
                add bx, BYTES_PER_SYMB * (SCR_WIDTH - 3) 

                endm

                PRINT_REGISTER 'A'
                PRINT_REGISTER 'B'
                PRINT_REGISTER 'C'
                PRINT_REGISTER 'D'

                pop dx cx bx ax

                ret 
                endp 

;------------------------------------------------------------------------------
; Prints a 2-byte number in hex.
;
; Expects: AX = number 
;          BX = start position in video buffer
;          ES = 0b800h               
;
; Returns: (none)
; Changes: AX, CX, DX, SI
; Details: (none)
;------------------------------------------------------------------------------
printHex        proc  

                mov ch, 0fh ; mask to get the last hex digit
                mov cl, 4   ; shift 

                ; Set bx to the last digit, *3 because there will be 4 digits. 
                add bx, BYTES_PER_SYMB * 3 ; 

                xor dh, dh
PRINT_DIGIT_HEX macro digit

                mov dl, al 
                and dl, ch
                shr ax, cl

                mov si, dx

                mov dl, cs:[si + offset digitsLetters]
                mov byte ptr es:[bx], dl

                dec bx 
                dec bx

                endm 

                PRINT_DIGIT_HEX
                PRINT_DIGIT_HEX
                PRINT_DIGIT_HEX
                PRINT_DIGIT_HEX

                inc bx 
                inc bx ; to set bx to its prev value

                ret 
                endp 

;------------------------------------------------------------------------------
; Prints a string terminated with 13h.
;
; Expects: BH = x
;          BL = y
;          SI = string address 
;          ES = 0b800h               
;
; Returns: (none)
; Changes: AX, BX, SI, DX
; Details: (none)
;------------------------------------------------------------------------------
printString     proc

                call getOffsetFromXY ; puts to bx address of the beginning

                mov dh, bl ; dh contains y for cursor
                mov dl, bh ; dl contains x for cursor

LOOP_PRINT_STRING:
                mov al, [si]
                cmp al, '$'
                je LOOP_PRINT_STRING_END

                mov byte ptr es:[bx], al

                inc dl
                inc si
                inc bx 
                inc bx
                jmp LOOP_PRINT_STRING
LOOP_PRINT_STRING_END:

                ret
                endp  

;------------------------------------------------------------------------------
; Calculates memory offset of the character at position (x, y).
;
; Expects: BH - x
;          BL - y
;             
; Returns: BX = 2 * (y * SCR_WIDTH + x)
; Changes: AX, BX, DX, DI 
; Details: (none)
;------------------------------------------------------------------------------
getOffsetFromXY	proc
            
                mov ax, 2 * SCR_WIDTH
                mul bl

                mov di, ax ; now di = (y * 2) * SCR_WIDTH

                xor ax, ax
                mov al, bh
                sal ax, 1  ; ax = x * 2 

                add ax, di                
                mov bx, ax ; now bx contains the line's start 

                ret 
                endp

;------------------------------------------------------------------------------
; Custom keyboard interrupt handler. If INTERRUPT_SHORTCUT was pressed, then
; changes visibility of the overlay with registers, otherwise, just calls the 
; default handler.
;
; Expects: (none)
; Returns: (none)
; Changes: (none) 
; Details: (none)
;------------------------------------------------------------------------------	
kbdIntercept    proc
                push ax ; saving registers

                in al, KBD_PORT ; fetches most recent scan code 

                cmp al, INTERRUPT_SHORTCUT
                jne KBD_INTERCEPT_END 

                ; SWITCH OVERLAY VISIBILITY
                mov al, cs:isRunning
                mov ah, 1
                and al, ah 
                xor al, ah
                mov cs:isRunning, al

                ; This way we show the keyboard controller that the keypress has 
                ; been processed.
                in al, 61h  ; another kbd port
                or al, 80h  ; 100..0 = msb mask
                out 61h, al ; setting 8th bit in port 61h to 1 = disable kbd

                and al, 7fh ; 011..1
                out 61h, al ; setting 8th bit in port 61h back to 0 = enable kbd
                
                mov al, 20h ; sixth bit, 0010 0000, telling interrupt controller 
                out 20h, al ; to shut the fuck up

                pop ax ; loading registers back
                iret 

KBD_INTERCEPT_END:
                pop ax ; loading registers back

                  db JMP_OPCODE
oldKbdIntAddress  dd 0
; oldKbdIntOffset   dw 0
; oldKbdIntSeg      dw 0

                endp

;------------------------------------------------------------------------------
; Custom timer interrupt handler. If isRunning, then draws a frame with 
; registers AX, BX, CX, DX' values, otherwise just calls the default handler.
; 
; Expects: (none)
; Returns: (none)
; Changes: (none)
; Details: (none)
;------------------------------------------------------------------------------	
timerIntercept  proc
                push ax bx cx dx di si es ds ; saving registers
                push ax bx cx dx

                mov bx, cs 
                mov ds, bx 

                mov bx, 0b800h
                mov es, bx

                mov bl, isRunning   

                test bl, bl           
                jz TIMER_INTERCEPT_END

                ; DRAWING THE FRAME
                call drawFrame ; changes AX, BX, CX, DX, DI, SI

                pop dx cx bx ax
                call printRegisters 
                push ax bx cx dx

                ; pop ds es si di dx cx bx ax ; loading registers back
                ; iret 

TIMER_INTERCEPT_END:
                pop dx cx bx ax
                pop ds es si di dx cx bx ax ; loading registers back

                   db JMP_OPCODE
oldTimerIntAddress dd 0
; oldTimerIntOffset dw 0
; oldTimerIntSeg    dw 0

                endp

isRunning         db 1
digitsLetters     db '0123456789ABCDEF'

PROGRAM_END:
end start