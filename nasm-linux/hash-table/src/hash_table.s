	.file	"hash_table.cpp"
	.intel_syntax noprefix
	.text
	.p2align 4
	.globl	_Z9constructP9HashTablemPFiPKcS2_EPFjS2_E
	.type	_Z9constructP9HashTablemPFiPKcS2_EPFjS2_E, @function
_Z9constructP9HashTablemPFiPKcS2_EPFjS2_E:
.LFB5382:
	.cfi_startproc
	push	r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	mov	r14, rdx
	push	r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	mov	r13, rcx
	push	r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	mov	r12, rsi
	mov	esi, 24
	push	rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	mov	rbp, rdi
	mov	rdi, r12
	push	rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	call	calloc@PLT
	mov	QWORD PTR 8[rbp], rax
	test	r12, r12
	je	.L2
	mov	rdx, rax
	xor	ebx, ebx
	xor	eax, eax
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L9:
	mov	rdx, QWORD PTR 8[rbp]
.L3:
	lea	rax, [rax+rax*2]
	lea	rdi, [rdx+rax*8]
	call	_Z9constructP6Bucket@PLT
	lea	eax, 1[rbx]
	mov	rbx, rax
	cmp	rax, r12
	jb	.L9
.L2:
	pop	rbx
	.cfi_def_cfa_offset 40
	mov	QWORD PTR 0[rbp], r12
	mov	QWORD PTR 16[rbp], r14
	mov	QWORD PTR 24[rbp], r13
	pop	rbp
	.cfi_def_cfa_offset 32
	pop	r12
	.cfi_def_cfa_offset 24
	pop	r13
	.cfi_def_cfa_offset 16
	pop	r14
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE5382:
	.size	_Z9constructP9HashTablemPFiPKcS2_EPFjS2_E, .-_Z9constructP9HashTablemPFiPKcS2_EPFjS2_E
	.p2align 4
	.globl	_Z7destroyP9HashTable
	.type	_Z7destroyP9HashTable, @function
_Z7destroyP9HashTable:
.LFB5383:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	push	rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	mov	rbx, rdi
	sub	rsp, 8
	.cfi_def_cfa_offset 32
	mov	rdi, QWORD PTR 8[rdi]
	test	rdi, rdi
	je	.L11
	cmp	QWORD PTR [rbx], 0
	je	.L12
	xor	ebp, ebp
	xor	eax, eax
	.p2align 4,,10
	.p2align 3
.L14:
	lea	rax, [rax+rax*2]
	lea	rdi, [rdi+rax*8]
	call	_Z7destroyP6Bucket@PLT
	lea	eax, 1[rbp]
	cmp	rax, QWORD PTR [rbx]
	mov	rdi, QWORD PTR 8[rbx]
	mov	rbp, rax
	jb	.L14
.L12:
	call	free@PLT
.L11:
	mov	QWORD PTR [rbx], 0
	mov	QWORD PTR 8[rbx], 0
	mov	QWORD PTR 16[rbx], 0
	mov	QWORD PTR 24[rbx], 0
	add	rsp, 8
	.cfi_def_cfa_offset 24
	pop	rbx
	.cfi_def_cfa_offset 16
	pop	rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE5383:
	.size	_Z7destroyP9HashTable, .-_Z7destroyP9HashTable
	.p2align 4
	.globl	_Z8strToYMMPKc
	.type	_Z8strToYMMPKc, @function
_Z8strToYMMPKc:
.LFB5384:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rsi, rdi
	lea	rdi, _ZZ8strToYMMPKcE6buffer[rip]
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	and	rsp, -32
	call	strcpy@PLT
	vmovdqa	ymm0, YMMWORD PTR _ZZ8strToYMMPKcE6buffer[rip]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5384:
	.size	_Z8strToYMMPKc, .-_Z8strToYMMPKc
	.p2align 4
	.globl	_Z6insertP9HashTablePKc9DictEntry
	.type	_Z6insertP9HashTablePKc9DictEntry, @function
_Z6insertP9HashTablePKc9DictEntry:
.LFB5385:
	.cfi_startproc
	lea	r10, 8[rsp]
	.cfi_def_cfa 10, 0
	and	rsp, -32
	push	QWORD PTR -8[r10]
	push	rbp
	.cfi_escape 0x10,0x6,0x2,0x76,0
	mov	rbp, rsp
	push	r14
	push	r13
	.cfi_escape 0x10,0xe,0x2,0x76,0x78
	.cfi_escape 0x10,0xd,0x2,0x76,0x70
	mov	r13, rsi
	push	r12
	.cfi_escape 0x10,0xc,0x2,0x76,0x68
	mov	r12, r10
	push	r10
	.cfi_escape 0xf,0x3,0x76,0x60,0x6
	push	rbx
	.cfi_escape 0x10,0x3,0x2,0x76,0x58
	mov	rbx, rdi
	mov	rdi, rsi
	sub	rsp, 72
	call	[QWORD PTR 24[rbx]]
	mov	eax, eax
	xor	edx, edx
	div	QWORD PTR [rbx]
	mov	rax, QWORD PTR 8[rbx]
	mov	rsi, r13
	lea	rdi, _ZZ8strToYMMPKcE6buffer[rip]
	lea	rdx, [rdx+rdx*2]
	lea	r14, [rax+rdx*8]
	call	strcpy@PLT
	mov	rcx, QWORD PTR 8[r14]
	vmovdqa	ymm2, YMMWORD PTR _ZZ8strToYMMPKcE6buffer[rip]
	test	rcx, rcx
	je	.L22
	mov	rsi, QWORD PTR [r14]
	vmovdqa	ymm1, ymm2
	xor	edx, edx
	xor	eax, eax
	jmp	.L24
	.p2align 4,,10
	.p2align 3
.L35:
	lea	eax, 1[rdx]
	mov	rdx, rax
	cmp	rax, rcx
	jnb	.L22
.L24:
	sal	rax, 6
	vpcmpeqb	ymm0, ymm1, YMMWORD PTR [rsi+rax]
	vpmovmskb	eax, ymm0
	cmp	eax, -1
	jne	.L35
	vzeroupper
	jmp	.L33
	.p2align 4,,10
	.p2align 3
.L22:
	mov	rcx, QWORD PTR 16[r12]
	mov	rax, QWORD PTR [r12]
	mov	rdx, QWORD PTR 8[r12]
	mov	QWORD PTR -64[rbp], rcx
	push	QWORD PTR -56[rbp]
	vmovdqa	YMMWORD PTR -112[rbp], ymm2
	mov	rdi, r14
	push	QWORD PTR -64[rbp]
	mov	QWORD PTR -80[rbp], rax
	mov	QWORD PTR -72[rbp], rdx
	push	rdx
	push	rax
	push	QWORD PTR -88[rbp]
	push	QWORD PTR -96[rbp]
	push	QWORD PTR -104[rbp]
	push	QWORD PTR -112[rbp]
	vzeroupper
	call	_Z8pushBackP6Bucket4Pair@PLT
	add	rsp, 64
.L33:
	lea	rsp, -40[rbp]
	pop	rbx
	pop	r10
	.cfi_def_cfa 10, 0
	pop	r12
	pop	r13
	pop	r14
	pop	rbp
	lea	rsp, -8[r10]
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5385:
	.size	_Z6insertP9HashTablePKc9DictEntry, .-_Z6insertP9HashTablePKc9DictEntry
	.p2align 4
	.globl	_Z16reallocHashTableP9HashTable
	.type	_Z16reallocHashTableP9HashTable, @function
_Z16reallocHashTableP9HashTable:
.LFB5386:
	.cfi_startproc
	ret
	.cfi_endproc
.LFE5386:
	.size	_Z16reallocHashTableP9HashTable, .-_Z16reallocHashTableP9HashTable
	.p2align 4
	.globl	_Z6removeP9HashTablePKc
	.type	_Z6removeP9HashTablePKc, @function
_Z6removeP9HashTablePKc:
.LFB5387:
	.cfi_startproc
	ret
	.cfi_endproc
.LFE5387:
	.size	_Z6removeP9HashTablePKc, .-_Z6removeP9HashTablePKc
	.p2align 4
	.globl	_Z4findPK9HashTablePKc
	.type	_Z4findPK9HashTablePKc, @function
_Z4findPK9HashTablePKc:
.LFB5388:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	push	r12
	.cfi_offset 12, -24
	mov	r12, rsi
	push	rbx
	.cfi_offset 3, -32
	mov	rbx, rdi
	mov	rdi, rsi
	and	rsp, -32
	call	[QWORD PTR 24[rbx]]
	mov	eax, eax
	xor	edx, edx
	div	QWORD PTR [rbx]
	mov	rax, QWORD PTR 8[rbx]
	mov	rsi, r12
	lea	rdi, _ZZ8strToYMMPKcE6buffer[rip]
	lea	rdx, [rdx+rdx*2]
	lea	rbx, [rax+rdx*8]
	call	strcpy@PLT
	mov	rsi, QWORD PTR 8[rbx]
	vmovdqa	ymm1, YMMWORD PTR _ZZ8strToYMMPKcE6buffer[rip]
	test	rsi, rsi
	je	.L42
	mov	rdi, QWORD PTR [rbx]
	xor	edx, edx
	xor	eax, eax
	jmp	.L41
	.p2align 4,,10
	.p2align 3
.L40:
	lea	eax, 1[rdx]
	mov	rdx, rax
	cmp	rax, rsi
	jnb	.L42
.L41:
	sal	rax, 6
	add	rax, rdi
	vpcmpeqb	ymm0, ymm1, YMMWORD PTR [rax]
	vpmovmskb	ecx, ymm0
	cmp	ecx, -1
	jne	.L40
	add	rax, 32
	vzeroupper
	lea	rsp, -16[rbp]
	pop	rbx
	pop	r12
	pop	rbp
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
	.p2align 4,,10
	.p2align 3
.L42:
	.cfi_restore_state
	xor	eax, eax
	vzeroupper
	lea	rsp, -16[rbp]
	pop	rbx
	pop	r12
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5388:
	.size	_Z4findPK9HashTablePKc, .-_Z4findPK9HashTablePKc
	.local	_ZZ8strToYMMPKcE6buffer
	.comm	_ZZ8strToYMMPKcE6buffer,32,32
	.ident	"GCC: (GNU) 10.2.0"
	.section	.note.GNU-stack,"",@progbits
