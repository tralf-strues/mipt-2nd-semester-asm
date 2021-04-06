	.file	"hash_functions.cpp"
	.intel_syntax noprefix
	.text
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC0:
	.string	"uint32_t getConstantHash(const char*)"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"hash_functions.cpp"
.LC2:
	.string	"string"
	.text
	.globl	_Z15getConstantHashPKc
	.type	_Z15getConstantHashPKc, @function
_Z15getConstantHashPKc:
.LFB28:
	.cfi_startproc
	test	rdi, rdi
	je	.L6
	mov	eax, 0
	ret
.L6:
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	lea	rcx, .LC0[rip]
	mov	edx, 25
	lea	rsi, .LC1[rip]
	lea	rdi, .LC2[rip]
	call	__assert_fail@PLT
	.cfi_endproc
.LFE28:
	.size	_Z15getConstantHashPKc, .-_Z15getConstantHashPKc
	.section	.rodata.str1.8
	.align 8
.LC3:
	.string	"uint32_t getStrLenHash(const char*)"
	.text
	.globl	_Z13getStrLenHashPKc
	.type	_Z13getStrLenHashPKc, @function
_Z13getStrLenHashPKc:
.LFB29:
	.cfi_startproc
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	test	rdi, rdi
	je	.L10
	call	strlen@PLT
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L10:
	.cfi_restore_state
	lea	rcx, .LC3[rip]
	mov	edx, 31
	lea	rsi, .LC1[rip]
	lea	rdi, .LC2[rip]
	call	__assert_fail@PLT
	.cfi_endproc
.LFE29:
	.size	_Z13getStrLenHashPKc, .-_Z13getStrLenHashPKc
	.section	.rodata.str1.8
	.align 8
.LC4:
	.string	"uint32_t getFirstCharHash(const char*)"
	.text
	.globl	_Z16getFirstCharHashPKc
	.type	_Z16getFirstCharHashPKc, @function
_Z16getFirstCharHashPKc:
.LFB30:
	.cfi_startproc
	push	rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	test	rdi, rdi
	je	.L14
	mov	rbx, rdi
	call	__ctype_tolower_loc@PLT
	movsx	rdx, BYTE PTR [rbx]
	mov	rax, QWORD PTR [rax]
	mov	eax, DWORD PTR [rax+rdx*4]
	pop	rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L14:
	.cfi_restore_state
	lea	rcx, .LC4[rip]
	mov	edx, 37
	lea	rsi, .LC1[rip]
	lea	rdi, .LC2[rip]
	call	__assert_fail@PLT
	.cfi_endproc
.LFE30:
	.size	_Z16getFirstCharHashPKc, .-_Z16getFirstCharHashPKc
	.section	.rodata.str1.8
	.align 8
.LC5:
	.string	"uint32_t getSumCharsHash(const char*)"
	.text
	.globl	_Z15getSumCharsHashPKc
	.type	_Z15getSumCharsHashPKc, @function
_Z15getSumCharsHashPKc:
.LFB31:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	push	rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	sub	rsp, 8
	.cfi_def_cfa_offset 32
	test	rdi, rdi
	je	.L22
	mov	rbp, rdi
	movzx	ebx, BYTE PTR [rdi]
	test	bl, bl
	je	.L19
	call	__ctype_tolower_loc@PLT
	mov	rsi, QWORD PTR [rax]
	mov	eax, 0
	mov	edx, 0
.L18:
	movsx	rbx, bl
	add	edx, DWORD PTR [rsi+rbx*4]
	add	eax, 1
	mov	ecx, eax
	movzx	ebx, BYTE PTR 0[rbp+rcx]
	test	bl, bl
	jne	.L18
.L15:
	mov	eax, edx
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	pop	rbx
	.cfi_def_cfa_offset 16
	pop	rbp
	.cfi_def_cfa_offset 8
	ret
.L22:
	.cfi_restore_state
	lea	rcx, .LC5[rip]
	mov	edx, 43
	lea	rsi, .LC1[rip]
	lea	rdi, .LC2[rip]
	call	__assert_fail@PLT
.L19:
	mov	edx, 0
	jmp	.L15
	.cfi_endproc
.LFE31:
	.size	_Z15getSumCharsHashPKc, .-_Z15getSumCharsHashPKc
	.section	.rodata.str1.8
	.align 8
.LC6:
	.string	"uint32_t getXorRightRotateHash(const char*)"
	.text
	.globl	_Z21getXorRightRotateHashPKc
	.type	_Z21getXorRightRotateHashPKc, @function
_Z21getXorRightRotateHashPKc:
.LFB32:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	push	rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	sub	rsp, 8
	.cfi_def_cfa_offset 32
	test	rdi, rdi
	je	.L30
	mov	rbp, rdi
	movzx	ebx, BYTE PTR [rdi]
	test	bl, bl
	je	.L27
	call	__ctype_tolower_loc@PLT
	mov	rsi, QWORD PTR [rax]
	mov	edx, 0
	mov	eax, 0
.L26:
	ror	eax
	movsx	rbx, bl
	xor	eax, DWORD PTR [rsi+rbx*4]
	add	edx, 1
	mov	ecx, edx
	movzx	ebx, BYTE PTR 0[rbp+rcx]
	test	bl, bl
	jne	.L26
.L23:
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	pop	rbx
	.cfi_def_cfa_offset 16
	pop	rbp
	.cfi_def_cfa_offset 8
	ret
.L30:
	.cfi_restore_state
	lea	rcx, .LC6[rip]
	mov	edx, 57
	lea	rsi, .LC1[rip]
	lea	rdi, .LC2[rip]
	call	__assert_fail@PLT
.L27:
	mov	eax, 0
	jmp	.L23
	.cfi_endproc
.LFE32:
	.size	_Z21getXorRightRotateHashPKc, .-_Z21getXorRightRotateHashPKc
	.section	.rodata.str1.8
	.align 8
.LC7:
	.string	"uint32_t getXorLeftRotateHash(const char*)"
	.text
	.globl	_Z20getXorLeftRotateHashPKc
	.type	_Z20getXorLeftRotateHashPKc, @function
_Z20getXorLeftRotateHashPKc:
.LFB33:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	push	rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	sub	rsp, 8
	.cfi_def_cfa_offset 32
	test	rdi, rdi
	je	.L38
	mov	rbp, rdi
	movzx	ebx, BYTE PTR [rdi]
	test	bl, bl
	je	.L35
	call	__ctype_tolower_loc@PLT
	mov	rsi, QWORD PTR [rax]
	mov	edx, 0
	mov	eax, 0
.L34:
	rol	eax
	movsx	rbx, bl
	xor	eax, DWORD PTR [rsi+rbx*4]
	add	edx, 1
	mov	ecx, edx
	movzx	ebx, BYTE PTR 0[rbp+rcx]
	test	bl, bl
	jne	.L34
.L31:
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	pop	rbx
	.cfi_def_cfa_offset 16
	pop	rbp
	.cfi_def_cfa_offset 8
	ret
.L38:
	.cfi_restore_state
	lea	rcx, .LC7[rip]
	mov	edx, 72
	lea	rsi, .LC1[rip]
	lea	rdi, .LC2[rip]
	call	__assert_fail@PLT
.L35:
	mov	eax, 0
	jmp	.L31
	.cfi_endproc
.LFE33:
	.size	_Z20getXorLeftRotateHashPKc, .-_Z20getXorLeftRotateHashPKc
	.section	.rodata.str1.8
	.align 8
.LC8:
	.string	"uint32_t getMurmur3Hash(const char*)"
	.text
	.globl	_Z14getMurmur3HashPKc
	.type	_Z14getMurmur3HashPKc, @function

	
_Z14getMurmur3HashPKc:
.LFB34:
	.cfi_startproc
	push	rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	test	rdi, rdi
	je	.L50
	mov	rbx, rdi
	call	strlen@PLT
	mov	rdi, rax
	mov	r8, rax
	shr	r8, 2
	cmp	rax, 3
	jbe	.L46
	mov	ecx, 0
	mov	esi, 220202
	mov	edx, 0
.L42:
	imul	eax, DWORD PTR [rbx+rdx*4], -862048943
	rol	eax, 15
	imul	eax, eax, 461845907
	xor	eax, esi
	rol	eax, 13
	lea	esi, -430675100[rax+rax*4]
	add	ecx, 1
	mov	edx, ecx
	cmp	rdx, r8
	jb	.L42
.L41:
	mov	rax, rdi
	and	rax, -4
	add	rbx, rax
	mov	rax, rdi
	and	eax, 3
	cmp	rax, 2
	je	.L43
	cmp	rax, 3
	je	.L44
	mov	edx, 0
	cmp	rax, 1
	je	.L51
.L45:
	imul	edx, edx, -862048943
	rol	edx, 15
	mov	eax, esi
	xor	eax, edi
	imul	esi, edx, 461845907
	xor	eax, esi
	mov	esi, eax
	shr	esi, 16
	xor	eax, esi
	imul	esi, eax, -2048144789
	mov	eax, esi
	shr	eax, 13
	xor	eax, esi
	imul	eax, eax, -1028477387
	mov	edx, eax
	shr	edx, 16
	xor	eax, edx
	pop	rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L50:
	.cfi_restore_state
	lea	rcx, .LC8[rip]
	mov	edx, 87
	lea	rsi, .LC1[rip]
	lea	rdi, .LC2[rip]
	call	__assert_fail@PLT
.L46:
	mov	esi, 220202
	jmp	.L41
.L51:
	movzx	edx, BYTE PTR [rbx]
	jmp	.L45
.L43:
	movzx	edx, BYTE PTR 1[rbx]
	sal	edx, 8
	jmp	.L45
.L44:
	movzx	edx, BYTE PTR 2[rbx]
	sal	edx, 16
	jmp	.L45
	.cfi_endproc
.LFE34:
	.size	_Z14getMurmur3HashPKc, .-_Z14getMurmur3HashPKc
	.ident	"GCC: (GNU) 10.2.0"
	.section	.note.GNU-stack,"",@progbits
