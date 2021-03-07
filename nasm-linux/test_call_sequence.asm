	.file	"test_call_sequence.cpp"
	.intel_syntax noprefix
	.text
	.globl	_Z4funciccPKcS0_ccix
	.type	_Z4funciccPKcS0_ccix, @function
_Z4funciccPKcS0_ccix:
.LFB0:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	mov	DWORD PTR -4[rbp], edi
	mov	eax, edx
	mov	QWORD PTR -24[rbp], rcx
	mov	QWORD PTR -32[rbp], r8
	mov	edi, r9d
	mov	edx, DWORD PTR 16[rbp]
	mov	ecx, esi
	mov	BYTE PTR -8[rbp], cl
	mov	BYTE PTR -12[rbp], al
	mov	eax, edi
	mov	BYTE PTR -16[rbp], al
	mov	eax, edx
	mov	BYTE PTR -36[rbp], al
	mov	BYTE PTR -12[rbp], 5
	movzx	eax, BYTE PTR -16[rbp]
	add	eax, 1
	mov	BYTE PTR -16[rbp], al
	sal	DWORD PTR 24[rbp]
	movsx	eax, BYTE PTR -8[rbp]
	movsx	edx, BYTE PTR -12[rbp]
	sub	eax, edx
	mov	DWORD PTR -4[rbp], eax
	movzx	eax, BYTE PTR -36[rbp]
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	_Z4funciccPKcS0_ccix, .-_Z4funciccPKcS0_ccix
	.section	.rodata
.LC0:
	.string	"Birthday"
.LC1:
	.string	"Happy"
	.text
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	push	41
	push	-273
	push	97
	mov	r9d, 117
	lea	r8, .LC0[rip]
	lea	rcx, .LC1[rip]
	mov	edx, 2
	mov	esi, 2
	mov	edi, 22
	call	_Z4funciccPKcS0_ccix
	add	rsp, 24
	mov	eax, 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (GNU) 10.2.0"
	.section	.note.GNU-stack,"",@progbits
