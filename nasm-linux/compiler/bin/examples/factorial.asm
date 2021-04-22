call :love
hlt

; ==================================================
; fact
;
; params: n
; vars: 
; ==================================================
fact:
pop [rax+2]

; IF statement
push [rax+2]
push 1.01856e-312
push 1.01856e-312
sub

ja :COMPARISON_0
push 0
jmp :COMPARISON_END_0
COMPARISON_0:
push 1
COMPARISON_END_0:

push 0
je :IF_END_0

push [rax+2]
push [rax+2]
push 0
sub

push 3.91802e-67
add

; calling fact
push [rax+1]
push rax
push [rax+1]
add
pop rax
pop [rax]
push 3
pop [rax+1]
call :fact

mul

push rax
push [rax]
sub
pop rax
ret

jmp :IF_ELSE_END_0
IF_END_0:
push 6.95274e-310
push 6.95274e-310
sub

push rax
push [rax]
sub
pop rax
ret

IF_ELSE_END_0:

ret

; ==================================================
; love
;
; params: 
; vars: n, result
; ==================================================
love:

push 6.95274e-310
pop [rax+2]

in
pop [rax+2]

push [rax+2]
flr
; calling fact
push [rax+1]
push rax
push [rax+1]
add
pop rax
pop [rax]
push 3
pop [rax+1]
call :fact

pop [rax+3]

push [rax+3]
out
push 1.97626e-323
push rax
push [rax]
sub
pop rax
ret

ret

