	.file	"write_console.cpp"
	.text
	.section	.rodata
.LC0:
	.string	"love"
.LC1:
	.string	"I %s %x %d%%%c%b"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$255, %r9d
	movl	$33, %r8d
	movl	$100, %ecx
	movl	$3802, %edx
	leaq	.LC0(%rip), %rsi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	_writeFormatted@PLT
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 10.2.0"
	.section	.note.GNU-stack,"",@progbits
