;------------------------------------------------------------------------------
; Numeral system changing program with *cool*(strikethrough) visuals.
;                     Copyright (c) 2021 Mochalov Nikita
;
; Huge thanks to
; - elisfromkirov
; - vssense
; - locllos for suggesting to use 'changes' instead of 'deletes' 
; - Negin X for explaining arithmetic to me
;
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

VIDEO_SEGMENT 	= 0b800h
MAX_LEN_BASE    = 5
MAX_LEN_NUMB    = 16
SCREEN_WIDTH	= 80
SCREEN_HEIGHT	= 24

RSHADOW_POS     = 2 * (80 *  2 + 56)
RSHADOW_PARAMS  = 14b1h         ; 14h = length, b1 = char
BSHADOW_POS     = 2 * (80 * 21 + 26)
BSHADOW_PARAMS  = 1eb1h         ; 1eh = length, b1 = char

SLEEP_TIME      = 0afffh
SLEEP_TIME_LONG = 0ffffh

COLOR_ATTRIBUTE = 1eh           ; bright yellow on blue

CAT_Y           = 3
SLEEP_CAT_Y     = 15

GOODBYE_MSSG_Y  = 13

; These are used by drawFrame6x4 and are equal to the x and y coordinates
; of the upper-left corner of the centered square with size = 2.
SCREEN_X_CENTER = 39
SCREEN_Y_CENTER = 10

; Frame specification
MAX_FRAME_SCALE = 5
FRAME_TEXT_X    = 27

CONVERTED_X     = 32
CONVERTED_Y     = 7

CHAR_HLINE      = 0cdh
CHAR_VLINE      = 0bah
CHAR_ULEFT      = 0c9h
CHAR_URIGHT     = 0bbh
CHAR_BLEFT      = 0c8h
CHAR_BRIGHT     = 0bch

.code
org 100h

start:
                mov bx, video_segment
                mov es, bx

                ; FRAME DRAWING
                mov bh, MAX_FRAME_SCALE
                call drawFrame6x4
                call printCat

                mov bx, RSHADOW_POS
                mov dx, RSHADOW_PARAMS
                mov ah, COLOR_ATTRIBUTE
                mov si, 2 * SCREEN_WIDTH
                call drawLine

                mov bx, BSHADOW_POS
                mov dx, BSHADOW_PARAMS
                mov ah, COLOR_ATTRIBUTE
                mov si, 2
                call drawLine

                ; INPUT
                call getInput

                ; after this bx contains base
                mov cl, 10              ; the input is in decimal
                mov si, offset input_base
                call stringToInt

                ; after this bx contains number
                mov cl, bl
                mov si, offset input_number
                call stringToInt

                ; OUTPUT
                mov si, bx              ; saving the number to si

                ; converting to dec, hex, oct, bs3, bin
                call convertAllBasic

                ; printing dec, hex, oct, bs3, bin
                call printConverted

                ; printing goodbye output
                mov bh, FRAME_TEXT_X
                mov bl, GOODBYE_MSSG_Y
                mov si, offset goodbye_message
                call printString

                call printSleepCat

                xor ah, ah
                int 16h

                mov ax, 4c00h
                int 21h

;------------------------------------------------------------------------------
; Draws a centered zooming out frame with aspect ratio 6x4.
; 
; Expects: BH = resulting scale (final width = 6 * BH, height = 4 * BH)
;          ES = 0b800h
; 
; Returns: (none)
;
; Changes: AX, BX, DX, SI, DI 
;
; Details: (none)
;------------------------------------------------------------------------------	
drawFrame6x4    proc

                mov ah, CHAR_HLINE
                mov al, CHAR_VLINE

                mov dh, 6
                mov dl, 4

                ; sets si to the upper-left   corner of the smallest rectangle
                ; sets di to the bottom-right corner of the smallest rectangle
                mov si, 162 + 2 * SCREEN_X_CENTER + 2 * SCREEN_WIDTH * SCREEN_Y_CENTER - 326
                mov di,       2 * SCREEN_X_CENTER + 2 * SCREEN_WIDTH * SCREEN_Y_CENTER + 326

LOOP_DRAW_FRAME:
                cmp bh, 0
                je LOOP_DRAW_FRAME_END

                call clearVBuffer
                call drawRectangle
                call sleepLong

                xchg si, di

                sub si, 326         ; moves U-L corner to the next scale 
                add di, 326         ; moves B-R corner to the next scale
                add dh, 6
                add dl, 4
                dec bh

                jmp LOOP_DRAW_FRAME
LOOP_DRAW_FRAME_END:

                ret
                endp

;------------------------------------------------------------------------------
; Draws a rectangle with animation.
; 
; Expects: AH = character for the horizontal line
;          AL = character for the vertical line
;          DH = width
;          DL = height
;          SI = the upper-left   corner position (in video segment)
;          DI = the bottom-right corner position (in video segment)
;          ES = 0b800h
;        
; Returns: (none)
;
; Changes: CX, SI, DI
;
; Details: SI and DI could be calculated as 2 * (80 * y + x).
;
;          After the execution SI and DI will swap values (SI will be 
;          bottom-right and DI will be upper-left).
;------------------------------------------------------------------------------
drawRectangle   proc

                ; HORIZONTAL LINES
                mov byte ptr es:[si],     CHAR_ULEFT
                mov byte ptr es:[si + 1], COLOR_ATTRIBUTE
                mov byte ptr es:[di],     CHAR_BRIGHT
                mov byte ptr es:[di + 1], COLOR_ATTRIBUTE
                inc si
                inc si
                dec di
                dec di

                xor ch, ch
                mov cl, dh
                dec cl
                dec cl  

LOOP_DRAW_HLINES:

                mov byte ptr es:[si], ah
                mov byte ptr es:[si + 1], COLOR_ATTRIBUTE
                mov byte ptr es:[di], ah
                mov byte ptr es:[di + 1], COLOR_ATTRIBUTE
                call sleep
                
                inc si
                inc si
                dec di
                dec di

                loop LOOP_DRAW_HLINES

                ; VERTICAL LINES
                mov byte ptr es:[si],     CHAR_URIGHT
                mov byte ptr es:[si + 1], COLOR_ATTRIBUTE
                mov byte ptr es:[di],     CHAR_BLEFT
                mov byte ptr es:[di + 1], COLOR_ATTRIBUTE
                add si, 2 * SCREEN_WIDTH
                sub di, 2 * SCREEN_WIDTH

                xor ch, ch
                mov cl, dl
                dec cl
                dec cl  

LOOP_DRAW_VLINES:

                mov byte ptr es:[si], al
                mov byte ptr es:[si + 1], COLOR_ATTRIBUTE
                mov byte ptr es:[di], al
                mov byte ptr es:[di + 1], COLOR_ATTRIBUTE
                call sleep

                add si, 2 * SCREEN_WIDTH
                sub di, 2 * SCREEN_WIDTH

                loop LOOP_DRAW_VLINES

                ret
                endp

;------------------------------------------------------------------------------
; Draws a line starting at the specified memory position.
;
; Expects: BX = memory position of the starting point
;          DH = length
;          DL = char used for the line (ASCII)
;          AH = color attribute
;          SI = step
;	       ES = 0b800h
;             
; Returns: (none)
;
; Changes: AL, BX, CX, DX 
;
; Details: BX after execution will pointing to the char after the last drawn
;          char.
;------------------------------------------------------------------------------
drawLine	    proc

                xor cx, cx
                mov cl, dh              ; cx = dh

                jcxz LOOP_LINE_END  
LOOP_LINE:
                mov byte ptr es:[bx],     dl
                mov byte ptr es:[bx + 1], ah

                call sleep

                add bx, SI
                loop LOOP_LINE
LOOP_LINE_END:

                ret
                endp

;------------------------------------------------------------------------------
; Clears video buffer with COLOR_ATTRIBUTE.
; 
; Expects: ES = 0b800h
;        
; Returns: (none)
; Changes: (none)
; Details: (none)   
;------------------------------------------------------------------------------
clearVBuffer    proc

                push bx

                mov bx, 2 * SCREEN_WIDTH * (SCREEN_HEIGHT + 1)

LOOP_CLEAR_VBUFFER:
                test bx, bx
                jz LOOP_CLEAR_VBUFFER_END

                dec bx
                dec bx
                mov byte ptr es:[bx], 0
                mov byte ptr es:[bx + 1], COLOR_ATTRIBUTE
        
                jmp LOOP_CLEAR_VBUFFER

LOOP_CLEAR_VBUFFER_END:		

                pop bx

                ret
                endp

;------------------------------------------------------------------------------
; Prints a cat inside the frame.
;
; Expects: ES = 0b800h               
;
; Returns: (none)
;
; Changes: 
;
; Details: Here's the picture
;                /\_/\
;               ( o.o )
;                > ^ <
;------------------------------------------------------------------------------
printCat        proc 

PRINT_CAT_LINE  macro line

                mov bh, FRAME_TEXT_X
                mov bl, CAT_Y + line - 1
                mov si, offset cat_line&line
                call printString

                endm

                PRINT_CAT_LINE 1
                PRINT_CAT_LINE 2
                PRINT_CAT_LINE 3

                ret
                endp

;------------------------------------------------------------------------------
; Prints a sleeping cat inside the frame.
;
; Expects: ES = 0b800h               
;
; Returns: (none)
;
; Changes: 
;
; Details: Here's the picture
;    _______           ZZZZ$
;   / __    \  /\/\ ZZZZ$   
;  /    \    \/ uu \$       
; / _____|_  > ____/<$      
; \________)______))$       
;------------------------------------------------------------------------------
printSleepCat   proc 

PRINT_SLEEP_CAT_LINE macro line

                mov bh, 28
                mov bl, SLEEP_CAT_Y + line - 1
                mov si, offset sleep_cat_line&line
                call printString

                endm

                PRINT_SLEEP_CAT_LINE 1
                PRINT_SLEEP_CAT_LINE 2
                PRINT_SLEEP_CAT_LINE 3
                PRINT_SLEEP_CAT_LINE 4
                PRINT_SLEEP_CAT_LINE 5

                ret
                endp

;------------------------------------------------------------------------------
; Set cursor to the specified line.
;
; Expects: DH = row
;          DL = column               
;
; Returns: (none)
; Changes: AH, BX
; Details: (none)
;------------------------------------------------------------------------------
setCursor       proc
                
                mov ah, 02h
                mov bx, 0
                int 10h
                
                ret
                endp

;------------------------------------------------------------------------------
; Reads a string terminated with '$'.
;
; Expects: DI = string address
;
; Returns: AL = string's length
;
; Changes: AX, DI
;
; Details: Sets DS[DI] to the string's length.
;------------------------------------------------------------------------------
readString      proc

                mov si, di              ; si saves di
                inc di
                mov ah, 01h
LOOP_READ_STR:
                int 21h

                cmp al, 0ah
                je LOOP_READ_STR_END

                mov [di], al

                inc di
                jmp LOOP_READ_STR
LOOP_READ_STR_END:

                ; saving length
                mov ax, di
                sub ax, si
                dec ax
                mov [si], al

                ret
                endp

;------------------------------------------------------------------------------
; Converts string to integer in the specified numeral system.
;
; Expects: CL      = base of a numeral system
;          SI      = string address
;          DS:[SI] = length of the string
;
; Returns: BX      = converted number
;
; Changes: AX, BX, CX, DX, SI
;
; Details: (none)
;------------------------------------------------------------------------------
stringToInt     proc

                mov ch, [si]            ; ch is counter
                inc si                  ; si points to the current symbol
                xor bx, bx              ; bx contains the result number
LOOP_STR_TO_INT:
                cmp ch, 0
                je LOOP_STR_TO_INT_END

                ; bx *= base
                mov ax, bx
                mul cl
                mov bx, ax

                mov al, [si]
                cmp al, '9'
                ja IF_LETTER 
                
                ; if digit
                sub al, '0'
                jmp IF_LETTER_END
IF_LETTER:
                ; if letter
                sub al, 'A' - 10
IF_LETTER_END:
                xor ah, ah
                add bx, ax

                inc si
                dec ch
                jmp LOOP_STR_TO_INT
LOOP_STR_TO_INT_END:

                ret 
                endp

;------------------------------------------------------------------------------
; Gets base and number from user and puts them into input_base and 
; input_number, respectively.
;
; Expects: (none)
; Returns: (none)
; Changes: AX, BX, DX, DI
; Details: (none)
;------------------------------------------------------------------------------
getInput        proc

                ; asking base from user
                mov bx, 0016h       ; row = 22, column = 0
                mov si, offset ask_base_message
                call printString

                ; setting the cursor
                mov dx, 161dh       ; row = 29, column = 22
                call setCursor

                ; getting the base
                mov di, offset input_base
                call readString

                ; setting the cursor
                mov dx, 1700h       ; row = 0, column = 23
                call setCursor

                ; asking number from user
                mov bx, 0017h       ; row = 23, column = 23 
                mov si, offset ask_num_message
                call printString

                ; setting the cursor
                mov dx, 1717h       ; row = 23, column = 23
                call setCursor

                ; getting the number
                mov di, offset input_number
                call readString

                ; setting the cursor
                mov dx, 1800h       ; row = 24, column = 0
                call setCursor
                
                ret
                endp

;------------------------------------------------------------------------------
; Converts number in SI to the following:
;     - decimal     (puts into converted_dec)  
;     - hexadecimal (puts into converted_hex)  
;     - octal       (puts into converted_oct)  
;     - base 3      (puts into converted_bs3)  
;     - binary      (puts into converted_bin)  
;
; Expects: SI = number to convert 
; Returns: (none)
; Changes: AX, BX, CX, DX, DI
; Details: (none)
;------------------------------------------------------------------------------
convertAllBasic proc

CONVERT_BASIC   macro base_num, base_str
                mov ax, si
                mov cl, base_num
                mov di, offset converted_&base_str
                call intToString
                endm    

CONVERT_BASIC2  macro base_num, base_str, power
                mov ax, si
                mov ch, base_num - 1
                mov cl, power
                mov di, offset converted_&base_str
                call intToString2
                endm    

                CONVERT_BASIC 10, dec

                ; mov ax, si
                ; mov ch, 16 - 1
                ; mov cl, 4
                ; mov di, offset converted_hex
                ; call intToString2

                CONVERT_BASIC2 16, hex, 4
                CONVERT_BASIC2 8,  oct, 3
                CONVERT_BASIC  3,  bs3
                CONVERT_BASIC2 2,  bin, 1

                ret 
                endp

;------------------------------------------------------------------------------
; Waits SLEEP_TIME microseconds.
; 
; Expects: (none)        
; Returns: (none)
; Changes: (none)
; Details: (none)   
;------------------------------------------------------------------------------
sleep           proc

                push cx
                
                mov cx, SLEEP_TIME
LOOP_SLEEP:     loop LOOP_SLEEP

                pop cx

                ret 
                endp

;------------------------------------------------------------------------------
; Waits SLEEP_TIME_LONG microseconds.
; 
; Expects: (none)        
; Returns: (none)
; Changes: (none)
; Details: (none)   
;------------------------------------------------------------------------------
sleepLong       proc

                push cx
                
                mov cx, SLEEP_TIME_LONG
LOOP_SLEEP_LONG: 
                loop LOOP_SLEEP_LONG

                pop cx

                ret 
                endp

;------------------------------------------------------------------------------
; Print all converted versions:
;     - decimal     (from converted_dec)  
;     - hexadecimal (from converted_hex)  
;     - octal       (from converted_oct)  
;     - base 3      (from converted_bs3)  
;     - binary      (from converted_bin)  
;
; Expects: SI = number to convert 
; Returns: (none)
; Changes: AX, BX, CX, DX, DI
; Details: (none)
;------------------------------------------------------------------------------
printConverted  proc

PRINT_ONE_CONVERTED macro line, base

                mov bh, FRAME_TEXT_X
                mov bl, CONVERTED_Y + line - 1
                mov si, offset base&_string
                call printString
                
                mov bh, CONVERTED_X
                mov bl, CONVERTED_Y  + line - 1
                mov si, offset converted_&base
                call printString

                endm

                PRINT_ONE_CONVERTED 1, dec
                PRINT_ONE_CONVERTED 2, hex
                PRINT_ONE_CONVERTED 3, oct
                PRINT_ONE_CONVERTED 4, bs3
                PRINT_ONE_CONVERTED 5, bin

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
; Changes: AX, BX, SI 
; Details: (none)
;------------------------------------------------------------------------------
printString     proc

                call getOffsetFromXY    ; puts to bx address of the beginning

                mov dh, bl              ; dh contains y for cursor
                mov dl, bh              ; dl contains x for cursor

LOOP_PRINT_STRING:
                mov al, [si]
                cmp al, '$'
                je LOOP_PRINT_STRING_END

                mov byte ptr es:[bx], al

                call sleep

                inc dl
                inc si
                inc bx 
                inc bx
                jmp LOOP_PRINT_STRING
LOOP_PRINT_STRING_END:

                ret
                endp  

;------------------------------------------------------------------------------
; Converts integer to string in the numeral system that is the power of 2 and 
; puts the result to DS:[DI]
; 
; Expects: AX = number to convert 
;          CH = mask  (shift number of 1's)
;          CL = shift (for bin - 1, oct - 3, hex - 4, etc.)
;          DI = buffer address
;
; Returns: BX = length of the string
;
; Changes: AX, BX, CX, DX
;
; Details: The string is terminated with '$' (ASCII 24h), and it isn't inluded
;          in length in register BX.
;------------------------------------------------------------------------------
intToString2    proc

                mov bx, di

LOOP_INT_TO_STR2:
                test ax, ax
                jz LOOP_INT_TO_STR2_END

                mov dl, al
                and dl, ch          ; dl = get last 'digit'

                cmp dl, 9
                ja IF_ABOVE_DECIMAL2

                mov dh, '0'         
                add dh, dl
                mov byte ptr [bx], dh 

                jmp END_ABOVE_DECIMAL2
IF_ABOVE_DECIMAL2:
                mov dh, 'A' - 10 
                add dh, dl

                mov byte ptr [bx], dh 
END_ABOVE_DECIMAL2:

                inc bx
                shr ax, cl

                jmp LOOP_INT_TO_STR2
LOOP_INT_TO_STR2_END:

                mov byte ptr [bx], '$'
        
                ; inversing the result string
                mov ax, di              ; saving di

                push bx                 ; saving bx 

                dec bx
LOOP_STR_INVERSE2:
                cmp di, bx
                jae LOOP_STR_INVERSE_END2

                mov ch, [di]
                xchg [bx], ch 
                mov [di], ch

                inc di
                dec bx 

                jmp LOOP_STR_INVERSE2
LOOP_STR_INVERSE_END2:
                
                pop bx

                mov di, ax              ; restoring di
                sub bx, di

                ret
                endp

;------------------------------------------------------------------------------
; Converts integer to string in the specified numeral system and puts the 
; result to DS:[DI].
; 
; Expects: AX = number to convert 
;          CL = base of a numeral system
;          DI = buffer address
;
; Returns: BX = length of the string
;
; Changes: AX, BX, CX, DX
;
; Details: The string is terminated with '$' (ASCII 24h), and it isn't inluded
;          in length in register BX.
;------------------------------------------------------------------------------
intToString     proc
                
                mov bx, di

LOOP_INT_TO_STR:
                test al, al
                jz LOOP_INT_TO_STR_END

                div cl

                cmp ah, 9
                ja IF_ABOVE_DECIMAL 

                mov ch, '0'         
                add ch, ah
                mov byte ptr [bx], ch 

                jmp END_ABOVE_DECIMAL
IF_ABOVE_DECIMAL:
                mov ch, 'A' - 10 
                add ch, ah

                mov byte ptr [bx], ch 
END_ABOVE_DECIMAL:

                xor ah, ah
                inc bx

                jmp LOOP_INT_TO_STR
LOOP_INT_TO_STR_END: 

                mov byte ptr [bx], '$'
        
                ; inversing the result string
                mov ax, di              ; saving di

                push bx                 ; saving bx 

                dec bx
LOOP_STR_INVERSE:
                cmp di, bx
                jae LOOP_STR_INVERSE_END

                mov ch, [di]
                xchg [bx], ch 
                mov [di], ch

                inc di
                dec bx 

                jmp LOOP_STR_INVERSE
LOOP_STR_INVERSE_END:
                
                pop bx

                mov di, ax              ; restoring di
                sub bx, di

                ret 
                endp

;------------------------------------------------------------------------------
; Calculates memory offset of the character at position (x, y).
;
; Expects: BH - x
;          BL - y
;             
; Returns: BX = 2 * (y * SCREEN_WIDTH + x)
;
; Changes: AX, BX, DX, DI 
;
; Details: (none)
;------------------------------------------------------------------------------
getOffsetFromXY	proc
            
                mov ax, 2 * SCREEN_WIDTH
                mul bl

                mov di, ax              ; now di = (y * 2) * SCREEN_WIDTH

                xor ax, ax
                mov al, bh
                sal ax, 1               ; ax = x * 2 

                add ax, di                
                mov bx, ax              ; now bx contains the line's start 

                ret 
                endp

cat_line1           db ' /\_/\$'
cat_line2           db '( o.o ) - ', 4h, 'Any', 4h, ' base! Meow!$'
cat_line3           db ' > ^ <$'

sleep_cat_line1     db '   _______           ZZZZ$'
sleep_cat_line2     db '  / __    \  /\/\ ZZZZ$'
sleep_cat_line3     db ' /    \    \/ uu \$   '
sleep_cat_line4     db '/ _____|_  > ____/<$'
sleep_cat_line5     db '\________)______))$'

ask_base_message    db 03h, 'Meow', 03h, '! Please, enter a base:$'
ask_num_message     db 'Please, enter a number:$'

goodbye_message     db 'Bye! Mrr...$'

dec_string          db 'DEC: $'
hex_string          db 'HEX: $'
oct_string          db 'OCT: $'
bs3_string          db 'BS3: $'
bin_string          db 'BIN: $'

frame_specification db COLOR_ATTRIBUTE, CHAR_ULEFT, CHAR_URIGHT, CHAR_BLEFT, CHAR_BRIGHT

converted_dec       db 8  dup(0) 
converted_hex       db 8  dup(0) 
converted_oct       db 8  dup(0) 
converted_bs3       db 16 dup(0) 
converted_bin       db 16 dup(0) 

input_base          db MAX_LEN_BASE dup(0)
input_number        db MAX_LEN_NUMB dup(0)

end start






; ;------------------------------------------------------------------------------
; ; Draws a rectangle.
; ; 
; ; Expects: BH = x coordinate of the upper-left corner
; ;          BL = y coordinate of the upper-left corner
; ;          DH = width 
; ;          DL = height
; ;          CH = char used for the horizontal lines of the frame (ASCII)
; ;          CL = char used for the vertical   lines of the frame (ASCII)
; ;          SI = address of the drawing specification
; ;          ES = 0b800h
; ; 
; ; Returns: (none)
; ;
; ; Changes: AX, BX, CX, DX, DI  
; ;
; ; Details: Drawing specification has to have the following format
; ;              [0][1][2][3][4] (5 bytes total)
; ;
; ;              [0] - color attribute
; ;              [1] - char used for the upper-left   corner
; ;              [2] - char used for the upper-right  corner
; ;              [3] - char used for the bottom-left  corner
; ;              [4] - char used for the bottom-right corner
; ;------------------------------------------------------------------------------	
; drawRectangle   proc
    
;                 ; calculating BX = 2 * (y * SCREEN_WIDTH + x)
;                 push dx
;                 call getOffsetFromXY
;                 pop dx

;                 push bx                 ; saving upper-left corner for later

;                 dec dh
;                 dec dh                  ; dh -= 2 for the hline (- 2 corners)

;                 dec dl
;                 dec dl                  ; dl -= 2 for the vline (- 2 corners)

;                 ; drawing upper-left corner
;                 mov ah, [si]

;                 mov al, [si + 1]
;                 mov byte ptr es:[bx],     al
;                 mov byte ptr es:[bx + 1], ah

;                 inc bx
;                 inc bx

;                 ; drawing top horizontal line
;                 push dx
;                 push cx

;                 mov dl, ch
;                 call drawHLine

;                 pop cx
;                 pop dx

;                 ; drawing upper-right corner
;                 mov ah, [si]

;                 mov al, [si + 2]
;                 mov byte ptr es:[bx],   al
;                 mov byte ptr es:[bx+1], ah

;                 add bx, 2 * SCREEN_WIDTH

;                 ; drawing right vertical line
;                 push dx
;                 push cx

;                 mov dh, dl
;                 mov dl, cl
;                 call drawVLine

;                 pop cx
;                 pop dx

;                 ; drawing bottom-right corner
;                 mov ah, [si]

;                 mov al, [si + 4]
;                 mov byte ptr es:[bx],   al
;                 mov byte ptr es:[bx+1], ah

;                 pop bx                  ; loading back the upper-left corner
;                 add bx, 2 * SCREEN_WIDTH

;                 ; drawing left vertical line
;                 push dx
;                 push cx

;                 mov dh, dl
;                 mov dl, cl
;                 call drawVLine

;                 pop cx
;                 pop dx

;                 ; drawing bottom-left corner
;                 mov ah, [si]

;                 mov al, [si + 3]
;                 mov byte ptr es:[bx],   al
;                 mov byte ptr es:[bx+1], ah

;                 inc bx
;                 inc bx

;                 ; drawing bottom horizontal line
;                 mov dl, ch
;                 call drawHLine

;                 ret
;                 endp

; ;------------------------------------------------------------------------------
; ; Draws a horizontal line starting at the specified memory position.
; ;
; ; Expects: BX = memory position of the left line's end
; ;          DH = length
; ;          DL = char used for the line (ASCII)
; ;          AH = color attribute
; ;	   ES = 0b800h
; ;             
; ; Returns: (none)
; ;
; ; Changes: AL, BX, CX, DX 
; ;
; ; Details: BX after execution will pointing to the char after the last drawn
; ;          char.
; ;------------------------------------------------------------------------------
; drawHLine	proc

;                 xor cx, cx
;                 mov cl, dh              ; cx = dh

;                 jcxz LOOP_HLINE_END  
; LOOP_HLINE:
;                 mov byte ptr es:[bx],     dl
;                 mov byte ptr es:[bx + 1], ah

;                 call sleep

;                 inc bx
;                 inc bx	
;                 loop LOOP_HLINE
; LOOP_HLINE_END:

;                 ret
;                 endp
        
; ;------------------------------------------------------------------------------
; ; Draws a vertical line starting at the specified memory position.
; ;
; ; Expects: BX = memory position of the top line's end
; ;          DH = height
; ;          DL = char used for the line (ASCII)
; ;          AH = color attribute
; ;          ES = 0b800h             
; ;
; ; Returns: (none)
; ;
; ; Changes: AL, BX, CX, DX 
; ;
; ; Details: (none)
; ;------------------------------------------------------------------------------
; drawVLine	proc

;             xor cx, cx
;             mov cl, dh              ; cx = dh

;             jcxz LOOP_VLINE_END  
; LOOP_VLINE:
;             mov byte ptr es:[bx],     dl
;             mov byte ptr es:[bx + 1], ah

;             call sleep

;             add bx, 2 * SCREEN_WIDTH
;             loop LOOP_VLINE
; LOOP_VLINE_END:

;             ret
;             endp

; ;------------------------------------------------------------------------------
; ; Draws a horizontal line starting at potition (x, y).
; ;
; ; Expects: BH = x coordinate of the left line's end
; ;          BL = y coordinate of the left line's end
; ;          DH = length
; ;          DL = char used for the line (ASCII)
; ;          CH = color attribute
; ;          ES = 0b800h             
; ;
; ; Returns: (none)
; ;
; ; Changes: AX, BX, CX, DX, DI, SI 
; ;
; ; Details: 
; ;------------------------------------------------------------------------------	
; drawHLineXY     proc
        
;                 ; putting the address of the beginning of the line in video 
;                 ; memory to BX 
;                 ;
;                 ; beginning of the line is at (y * 2) * SCREEN_WIDTH + (x * 2)
;                 ; multiplication by 2 is needed, for each character in memory
;                 ; takes up two bytes - one for the character itself and one
;                 ; for its color attribute

;                 mov si, dx              ; saving dx to si (before mul)

;                 call getOffsetFromXY

;                 mov dx, si              ; loading dx back

;                 ; drawing the line
;                 mov al, ch              
;                 call drawHLine

;                 ret
;                 endp

; ;------------------------------------------------------------------------------
; ; Draws a vertical line starting at potition (x, y).
; ;
; ; Expects: BH = x coordinate of the top line's end
; ;          BL = y coordinate of the top line's end
; ;          DH = height
; ;          DL = char used for the line (ASCII)
; ;          CH = color attribute
; ;          ES = 0b800h             
; ;
; ; Returns: (none)
; ; Changes: AX, BX, CX, DX, DI, SI
; ; 
; ; Details: 
; ;------------------------------------------------------------------------------	
; drawVLineXY     proc

;                 ; putting the address of the beginning of the line in video 
;                 ; memory to BX 
;                 ;
;                 ; beginning of the line is at (y * 2) * SCREEN_WIDTH + (x * 2)
;                 ; multiplication by 2 is needed, for each character in memory
;                 ; takes up two bytes - one for the character itself and one
;                 ; for its color attribute

;                 mov si, dx                ; saving dx to si (before mul)

;                 call getOffsetFromXY

;                 mov dx, si                ; loading dx back

;                 ; drawing the line
;                 mov al, ch              
;                 call drawVLine

;                 ret
;                 endp