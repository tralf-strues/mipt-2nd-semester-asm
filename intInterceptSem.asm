.model tiny

KEYBOARD_PORT   = 60h
TILDA_SCAN_CODE = 29h

.code
org 100h

;------------------------------------------------------------------------------
; Simple version of reading scan code.
;------------------------------------------------------------------------------
start:
                mov bx, 0b800h
                mov es, bx

                mov bx, 2 * (80 * 5 + 45)
                mov ah, 2ah ; color

NEXT: 
                in al, KEYBOARD_PORT ; fetches most recent scan code 
                mov es:[bx], ax
                cmp al, TILDA_SCAN_CODE
                jne NEXT

                mov ax, 4c00h
                int 21h
end start


;------------------------------------------------------------------------------
; Interruption interception V0. Doesn't pass input further.
;------------------------------------------------------------------------------
start:
                xor bx, bx
                mov es, bx

                mov bx, 9 * 4 ; interruption 9 (and each address is 4 bytes)

                ; Interrupt Flag (IF) determines whether or not CPU will 
                ; respond to *maskable* hardware interrupts.
                cli ; clear interrupts, sets IF to 0
                
                mov es:[bx], offset kbdIntercept
                mov ax, cs
                mov es:[bx + 2], ax

                sti ; set interrupts, sets IF to 1

                ; Terminating the program and leaving code and data in memory
                mov ax, 3100h              ; al = exit code
                mov dx, offset PROGRAM_END ; dx = memory size to keep in 
                shr dx, 4                  ;      16-byte paragraphs
                inc dx                     ; if program mod 16 != 0, then save
                                           ; one extra paragraph 
                int 21h

kbdIntercept    proc
                push ax bx es ; saving registers

                mov bx, 0b800h
                mov es, bx
                mov bx, 2 * (80 * 5 + 45) ; y = 5, x = 45
                mov ah, 2eh ; color

                in al, KEYBOARD_PORT ; fetches most recent scan code 
                mov es:[bx], ax

                ; This way we show the keyboard controller that the keypress has 
                ; been processed.
                in al, 61h
                or al, 80h  ; 100..0 = msb mask
                out 61h, al ; setting 8th bit in port 61h to 1 = disable kbd

                and al, 7fh ; 011..1
                out 61h, al ; setting 8th bit in port 61h back to 0 = enable kbd
                
                mov al, 20h ; sixth bit, 0010 0000, telling interrupt controller 
                out 20h, al ; to shut the FUCK UP

                pop es bx ax ; loading registers back

                iret
                endp


;------------------------------------------------------------------------------
; Interruption interception V1. Passes input further.
;------------------------------------------------------------------------------
start:
                xor bx, bx
                mov es, bx

                mov bx, 9 * 4 ; interruption 9 (and each address is 4 bytes)

                ; Interrupt Flag (IF) determines whether or not CPU will 
                ; respond to *maskable* hardware interrupts.
                cli ; clear interrupts, sets IF to 0
                
                ; <-saves old 09h int handler-> 
                mov ax, es:[bx]
                mov oldKbdIntOffset, ax
                mov ax, es:[bx + 2]
                mov oldKbdIntSeg, ax
                ; <->

                mov es:[bx], offset kbdIntercept
                mov ax, cs
                mov es:[bx + 2], ax

                sti ; set interrupts, sets IF to 1

                ; Terminating the program and leaving code and data in memory
                mov ax, 3100h              ; al = exit code
                mov dx, offset PROGRAM_END ; dx = memory size to keep in 
                shr dx, 4                  ;      16-byte paragraphs
                inc dx                     ; if program mod 16 != 0, then save
                                           ; one extra paragraph 
                int 21h

kbdIntercept    proc
                push ax bx es ; saving registers

                mov bx, 0b800h
                mov es, bx
                mov bx, 2 * (80 * 5 + 45) ; y = 5, x = 45
                mov ah, 2eh ; color

                in al, KEYBOARD_PORT ; fetches most recent scan code 
                mov es:[bx], ax

                ; <->
                ; This way we show the keyboard controller that the keypress has 
                ; been processed.
                ; in al, 61h
                ; or al, 80h  ; 100..0 = msb mask
                ; out 61h, al ; setting 8th bit in port 61h to 1 = disable kbd

                ; and al, 7fh ; 011..1
                ; out 61h, al ; setting 8th bit in port 61h back to 0 = enable kbd
                
                ; mov al, 20h ; sixth bit, 0010 0000, telling interrupt controller 
                ; out 20h, al ; to shut the FUCK UP
                ; <->

                pop es bx ax ; loading registers back

                ; <->
                ; iret
                ; <->

                ; <-->
                db 0eah ; JMP opcode
oldKbdIntOffset dw 0
oldKbdIntSeg    dw 0
                ; <->

                endp


;------------------------------------------------------------------------------
; Interruption interception V2. Passes input further.
;------------------------------------------------------------------------------
start:
                xor bx, bx
                mov es, bx

                mov bx, 9 * 4 ; interruption 9 (and each address is 4 bytes)

                ; Interrupt Flag (IF) determines whether or not CPU will 
                ; respond to *maskable* hardware interrupts.
                cli ; clear interrupts, sets IF to 0
                
                mov ax, es:[bx]
                mov oldKbdIntOffset, ax
                mov ax, es:[bx + 2]
                mov oldKbdIntSeg, ax

                mov es:[bx], offset kbdIntercept
                mov ax, cs
                mov es:[bx + 2], ax

                sti ; set interrupts, sets IF to 1

                ; Terminating the program and leaving code and data in memory
                mov ax, 3100h              ; al = exit code
                mov dx, offset PROGRAM_END ; dx = memory size to keep in 
                shr dx, 4                  ;      16-byte paragraphs
                inc dx                     ; if program mod 16 != 0, then save
                                           ; one extra paragraph 
                int 21h

kbdIntercept    proc
                push ax bx es ; saving registers

                mov bx, 0b800h
                mov es, bx
                mov bx, 2 * (80 * 5 + 45) ; y = 5, x = 45
                mov ah, 2eh ; color

                in al, KEYBOARD_PORT ; fetches most recent scan code 
                mov es:[bx], ax

                ; <->
                cmp al, 2
                jne interruptionEnd 
                mov al, 03h ; heart ASCII char
                mov es:[bx + 4], ax ; putting the heart char to the right of the prev one
                ; <->

                ; <->
                ; This way we show the keyboard controller that the keypress has 
                ; been processed.
                in al, 61h
                or al, 80h  ; 100..0 = msb mask
                out 61h, al ; setting 8th bit in port 61h to 1 = disable kbd

                and al, 7fh ; 011..1
                out 61h, al ; setting 8th bit in port 61h back to 0 = enable kbd
                
                mov al, 20h ; sixth bit, 0010 0000, telling interrupt controller 
                out 20h, al ; to shut the FUCK UP
                ; <->

                ; <->
                pop es bx ax
                iret 
                ; <->

interruptionEnd:
                pop es bx ax ; loading registers back

                ; iret

                db 0eah ; JMP opcode
oldKbdIntOffset dw 0
oldKbdIntSeg    dw 0

                endp

PROGRAM_END:
end start