	.file	"instructions.c"
	.text
	.p2align 4,,15
	.globl	load_immediate_byte
	.type	load_immediate_byte, @function
load_immediate_byte:
.LFB41:
	.cfi_startproc
	movb	%sil, (%rdi)
	ret
	.cfi_endproc
.LFE41:
	.size	load_immediate_byte, .-load_immediate_byte
	.p2align 4,,15
	.globl	add_register
	.type	add_register, @function
add_register:
.LFB42:
	.cfi_startproc
	movzbl	(%rdi), %ecx
	movl	%ecx, %r8d
	addb	(%rsi), %r8b
	setc	%r9b
	cmpb	$1, %r8b
	sbbl	%esi, %esi
	andl	$-128, %esi
	addl	$16, %esi
	cmpb	$1, %r8b
	sbbl	%eax, %eax
	andl	$-128, %eax
	testb	%r9b, %r9b
	movl	%r8d, %r9d
	cmovne	%esi, %eax
	andl	$15, %r9d
	andl	$15, %ecx
	movl	%eax, %esi
	orl	$32, %esi
	cmpb	%cl, %r9b
	cmovb	%esi, %eax
	movb	%al, (%rdx)
	movb	%r8b, (%rdi)
	ret
	.cfi_endproc
.LFE42:
	.size	add_register, .-add_register
	.p2align 4,,15
	.globl	add_immediate
	.type	add_immediate, @function
add_immediate:
.LFB43:
	.cfi_startproc
	movzbl	(%rdi), %ecx
	addb	%cl, %sil
	setc	%r9b
	cmpb	$1, %sil
	sbbl	%r8d, %r8d
	andl	$-128, %r8d
	addl	$16, %r8d
	cmpb	$1, %sil
	sbbl	%eax, %eax
	andl	$-128, %eax
	testb	%r9b, %r9b
	movl	%esi, %r9d
	cmovne	%r8d, %eax
	andl	$15, %r9d
	andl	$15, %ecx
	movl	%eax, %r8d
	orl	$32, %r8d
	cmpb	%cl, %r9b
	cmovb	%r8d, %eax
	movb	%al, (%rdx)
	movb	%sil, (%rdi)
	ret
	.cfi_endproc
.LFE43:
	.size	add_immediate, .-add_immediate
	.p2align 4,,15
	.globl	add_indirect
	.type	add_indirect, @function
add_indirect:
.LFB44:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	movq	%rdx, %rbp
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movzwl	(%rsi), %edi
	call	read_byte@PLT
	movzbl	(%rbx), %esi
	addb	%sil, %al
	setc	%dl
	cmpb	$1, %al
	sbbl	%edi, %edi
	andl	$-128, %edi
	addl	$16, %edi
	cmpb	$1, %al
	sbbl	%ecx, %ecx
	andl	$-128, %ecx
	testb	%dl, %dl
	cmovne	%edi, %ecx
	movl	%eax, %edi
	andl	$15, %esi
	movl	%ecx, %edx
	andl	$15, %edi
	orl	$32, %edx
	cmpb	%sil, %dil
	cmovb	%edx, %ecx
	movb	%cl, 0(%rbp)
	movb	%al, (%rbx)
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE44:
	.size	add_indirect, .-add_indirect
	.p2align 4,,15
	.globl	add_register_with_carry
	.type	add_register_with_carry, @function
add_register_with_carry:
.LFB45:
	.cfi_startproc
	movzbl	(%rdx), %eax
	movzbl	(%rdi), %r8d
	movzbl	(%rsi), %ecx
	sarl	$4, %eax
	addl	%r8d, %ecx
	andl	$1, %eax
	addl	%ecx, %eax
	cmpb	$1, %al
	movl	%eax, %r9d
	sbbl	%esi, %esi
	andl	$-128, %esi
	addl	$16, %esi
	cmpb	$1, %al
	sbbl	%ecx, %ecx
	andl	$-128, %ecx
	cmpb	%al, %r8b
	cmova	%esi, %ecx
	andl	$15, %r9d
	andl	$15, %r8d
	movl	%ecx, %esi
	orl	$32, %esi
	cmpb	%r8b, %r9b
	cmovb	%esi, %ecx
	movb	%cl, (%rdx)
	movb	%al, (%rdi)
	ret
	.cfi_endproc
.LFE45:
	.size	add_register_with_carry, .-add_register_with_carry
	.p2align 4,,15
	.globl	add_immediate_with_carry
	.type	add_immediate_with_carry, @function
add_immediate_with_carry:
.LFB46:
	.cfi_startproc
	movzbl	(%rdx), %eax
	movzbl	(%rdi), %ecx
	sarl	$4, %eax
	addl	%ecx, %esi
	andl	$1, %eax
	addl	%eax, %esi
	cmpb	$1, %sil
	movl	%esi, %r9d
	sbbl	%r8d, %r8d
	andl	$-128, %r8d
	addl	$16, %r8d
	cmpb	$1, %sil
	sbbl	%eax, %eax
	andl	$-128, %eax
	cmpb	%sil, %cl
	cmova	%r8d, %eax
	andl	$15, %r9d
	andl	$15, %ecx
	movl	%eax, %r8d
	orl	$32, %r8d
	cmpb	%cl, %r9b
	cmovb	%r8d, %eax
	movb	%al, (%rdx)
	movb	%sil, (%rdi)
	ret
	.cfi_endproc
.LFE46:
	.size	add_immediate_with_carry, .-add_immediate_with_carry
	.p2align 4,,15
	.globl	add_indirect_with_carry
	.type	add_indirect_with_carry, @function
add_indirect_with_carry:
.LFB47:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movq	%rdi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movzwl	(%rsi), %edi
	movq	%rdx, %r12
	movzbl	(%rdx), %ebx
	call	read_byte@PLT
	movzbl	0(%rbp), %esi
	sarl	$4, %ebx
	andl	$1, %ebx
	addl	%esi, %ebx
	addl	%ebx, %eax
	cmpb	$1, %al
	sbbl	%edi, %edi
	andl	$-128, %edi
	addl	$16, %edi
	cmpb	$1, %al
	sbbl	%ecx, %ecx
	andl	$-128, %ecx
	cmpb	%al, %sil
	cmova	%edi, %ecx
	movl	%eax, %edi
	andl	$15, %esi
	movl	%ecx, %edx
	andl	$15, %edi
	orl	$32, %edx
	cmpb	%sil, %dil
	cmovb	%edx, %ecx
	movb	%cl, (%r12)
	movb	%al, 0(%rbp)
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE47:
	.size	add_indirect_with_carry, .-add_indirect_with_carry
	.p2align 4,,15
	.globl	subtract_register
	.type	subtract_register, @function
subtract_register:
.LFB48:
	.cfi_startproc
	movzbl	(%rdi), %ecx
	movl	$-64, %eax
	cmpb	(%rsi), %cl
	movl	%ecx, %r8d
	setb	%r9b
	subb	(%rsi), %r8b
	je	.L50
	cmpb	$1, %r9b
	sbbl	%eax, %eax
	andl	$-16, %eax
	addl	$112, %eax
	cmpb	$1, %r9b
	movl	%r8d, %r9d
	sbbl	%esi, %esi
	andl	$15, %r9d
	andl	$15, %ecx
	andl	$-16, %esi
	addl	$80, %esi
	cmpb	%cl, %r9b
	cmovbe	%esi, %eax
.L50:
	movb	%al, (%rdx)
	movb	%r8b, (%rdi)
	ret
	.cfi_endproc
.LFE48:
	.size	subtract_register, .-subtract_register
	.p2align 4,,15
	.globl	subtract_immediate
	.type	subtract_immediate, @function
subtract_immediate:
.LFB49:
	.cfi_startproc
	movzbl	(%rdi), %ecx
	movl	$-64, %eax
	cmpb	%sil, %cl
	movl	%ecx, %r8d
	setb	%r9b
	subb	%sil, %r8b
	je	.L58
	cmpb	$1, %r9b
	sbbl	%eax, %eax
	andl	$-16, %eax
	addl	$112, %eax
	cmpb	$1, %r9b
	movl	%r8d, %r9d
	sbbl	%esi, %esi
	andl	$15, %r9d
	andl	$15, %ecx
	andl	$-16, %esi
	addl	$80, %esi
	cmpb	%cl, %r9b
	cmovbe	%esi, %eax
.L58:
	movb	%al, (%rdx)
	movb	%r8b, (%rdi)
	ret
	.cfi_endproc
.LFE49:
	.size	subtract_immediate, .-subtract_immediate
	.p2align 4,,15
	.globl	subtract_indirect
	.type	subtract_indirect, @function
subtract_indirect:
.LFB50:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	movq	%rdx, %rbp
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movzwl	(%rsi), %edi
	call	read_byte@PLT
	movzbl	(%rbx), %esi
	movl	$-64, %ecx
	cmpb	%al, %sil
	movl	%esi, %edi
	setb	%dl
	subb	%al, %dil
	je	.L66
	cmpb	$1, %dl
	sbbl	%ecx, %ecx
	andl	$-16, %ecx
	addl	$112, %ecx
	cmpb	$1, %dl
	movl	%edi, %edx
	sbbl	%eax, %eax
	andl	$15, %edx
	andl	$15, %esi
	andl	$-16, %eax
	addl	$80, %eax
	cmpb	%sil, %dl
	cmovbe	%eax, %ecx
.L66:
	movb	%cl, 0(%rbp)
	movb	%dil, (%rbx)
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE50:
	.size	subtract_indirect, .-subtract_indirect
	.p2align 4,,15
	.globl	subtract_register_with_carry
	.type	subtract_register_with_carry, @function
subtract_register_with_carry:
.LFB51:
	.cfi_startproc
	movzbl	(%rdi), %r8d
	movzbl	(%rdx), %eax
	movl	%r8d, %ecx
	subb	(%rsi), %cl
	sarl	$4, %eax
	andl	$1, %eax
	movl	$-64, %esi
	subb	%al, %cl
	movl	%ecx, %eax
	je	.L74
	cmpb	%cl, %r8b
	movl	%eax, %r9d
	sbbl	%esi, %esi
	andl	$16, %esi
	addl	$96, %esi
	cmpb	%cl, %r8b
	sbbl	%ecx, %ecx
	andl	$15, %r9d
	andl	$15, %r8d
	andl	$16, %ecx
	addl	$64, %ecx
	cmpb	%r8b, %r9b
	cmovbe	%ecx, %esi
.L74:
	movb	%sil, (%rdx)
	movb	%al, (%rdi)
	ret
	.cfi_endproc
.LFE51:
	.size	subtract_register_with_carry, .-subtract_register_with_carry
	.p2align 4,,15
	.globl	subtract_immediate_with_carry
	.type	subtract_immediate_with_carry, @function
subtract_immediate_with_carry:
.LFB52:
	.cfi_startproc
	movzbl	(%rdi), %r8d
	movzbl	(%rdx), %eax
	movl	%r8d, %ecx
	sarl	$4, %eax
	subl	%esi, %ecx
	andl	$1, %eax
	movl	$-64, %esi
	subb	%al, %cl
	movl	%ecx, %eax
	je	.L81
	cmpb	%cl, %r8b
	movl	%eax, %r9d
	sbbl	%esi, %esi
	andl	$16, %esi
	addl	$96, %esi
	cmpb	%cl, %r8b
	sbbl	%ecx, %ecx
	andl	$15, %r9d
	andl	$15, %r8d
	andl	$16, %ecx
	addl	$64, %ecx
	cmpb	%r8b, %r9b
	cmovbe	%ecx, %esi
.L81:
	movb	%sil, (%rdx)
	movb	%al, (%rdi)
	ret
	.cfi_endproc
.LFE52:
	.size	subtract_immediate_with_carry, .-subtract_immediate_with_carry
	.p2align 4,,15
	.globl	subtract_indirect_with_carry
	.type	subtract_indirect_with_carry, @function
subtract_indirect_with_carry:
.LFB53:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movq	%rdi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movzwl	(%rsi), %edi
	movq	%rdx, %r12
	movzbl	(%rdx), %ebx
	call	read_byte@PLT
	movzbl	0(%rbp), %esi
	sarl	$4, %ebx
	movl	$-64, %ecx
	andl	$1, %ebx
	movl	%esi, %edx
	subl	%eax, %edx
	movl	%edx, %eax
	subb	%bl, %al
	je	.L88
	cmpb	%al, %sil
	movl	%eax, %edi
	sbbl	%ecx, %ecx
	andl	$16, %ecx
	addl	$96, %ecx
	cmpb	%al, %sil
	sbbl	%edx, %edx
	andl	$15, %edi
	andl	$15, %esi
	andl	$16, %edx
	addl	$64, %edx
	cmpb	%sil, %dil
	cmovbe	%edx, %ecx
.L88:
	movb	%cl, (%r12)
	movb	%al, 0(%rbp)
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE53:
	.size	subtract_indirect_with_carry, .-subtract_indirect_with_carry
	.p2align 4,,15
	.globl	bitwise_and_register
	.type	bitwise_and_register, @function
bitwise_and_register:
.LFB54:
	.cfi_startproc
	movzbl	(%rdi), %eax
	andb	(%rsi), %al
	cmpb	$1, %al
	movb	%al, (%rdi)
	sbbl	%eax, %eax
	andl	$-128, %eax
	addl	$32, %eax
	movb	%al, (%rdx)
	ret
	.cfi_endproc
.LFE54:
	.size	bitwise_and_register, .-bitwise_and_register
	.p2align 4,,15
	.globl	bitwise_and_immediate
	.type	bitwise_and_immediate, @function
bitwise_and_immediate:
.LFB55:
	.cfi_startproc
	andb	(%rdi), %sil
	cmpb	$1, %sil
	movb	%sil, (%rdi)
	sbbl	%eax, %eax
	andl	$-128, %eax
	addl	$32, %eax
	movb	%al, (%rdx)
	ret
	.cfi_endproc
.LFE55:
	.size	bitwise_and_immediate, .-bitwise_and_immediate
	.p2align 4,,15
	.globl	bitwise_and_indirect
	.type	bitwise_and_indirect, @function
bitwise_and_indirect:
.LFB56:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	movq	%rdx, %rbp
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movzwl	(%rsi), %edi
	call	read_byte@PLT
	andb	(%rbx), %al
	cmpb	$1, %al
	movb	%al, (%rbx)
	sbbl	%eax, %eax
	andl	$-128, %eax
	addl	$32, %eax
	movb	%al, 0(%rbp)
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE56:
	.size	bitwise_and_indirect, .-bitwise_and_indirect
	.p2align 4,,15
	.globl	bitwise_or_register
	.type	bitwise_or_register, @function
bitwise_or_register:
.LFB57:
	.cfi_startproc
	movzbl	(%rdi), %eax
	orb	(%rsi), %al
	cmpb	$1, %al
	movb	%al, (%rdi)
	sbbl	%eax, %eax
	andl	$-128, %eax
	movb	%al, (%rdx)
	ret
	.cfi_endproc
.LFE57:
	.size	bitwise_or_register, .-bitwise_or_register
	.p2align 4,,15
	.globl	bitwise_or_immediate
	.type	bitwise_or_immediate, @function
bitwise_or_immediate:
.LFB58:
	.cfi_startproc
	orb	(%rdi), %sil
	cmpb	$1, %sil
	movb	%sil, (%rdi)
	sbbl	%eax, %eax
	andl	$-128, %eax
	movb	%al, (%rdx)
	ret
	.cfi_endproc
.LFE58:
	.size	bitwise_or_immediate, .-bitwise_or_immediate
	.p2align 4,,15
	.globl	bitwise_or_indirect
	.type	bitwise_or_indirect, @function
bitwise_or_indirect:
.LFB59:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	movq	%rdx, %rbp
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movzwl	(%rsi), %edi
	call	read_byte@PLT
	orb	(%rbx), %al
	cmpb	$1, %al
	movb	%al, (%rbx)
	sbbl	%eax, %eax
	andl	$-128, %eax
	movb	%al, 0(%rbp)
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE59:
	.size	bitwise_or_indirect, .-bitwise_or_indirect
	.p2align 4,,15
	.globl	bitwise_xor_register
	.type	bitwise_xor_register, @function
bitwise_xor_register:
.LFB60:
	.cfi_startproc
	movzbl	(%rdi), %ecx
	movzbl	(%rsi), %eax
	movl	%ecx, %esi
	xorl	%eax, %esi
	cmpb	%al, %cl
	movl	$-128, %eax
	movl	$0, %ecx
	movb	%sil, (%rdi)
	cmovne	%ecx, %eax
	movb	%al, (%rdx)
	ret
	.cfi_endproc
.LFE60:
	.size	bitwise_xor_register, .-bitwise_xor_register
	.p2align 4,,15
	.globl	bitwise_xor_immediate
	.type	bitwise_xor_immediate, @function
bitwise_xor_immediate:
.LFB61:
	.cfi_startproc
	movzbl	(%rdi), %eax
	movl	%eax, %ecx
	xorl	%esi, %ecx
	cmpb	%sil, %al
	movl	$-128, %eax
	movb	%cl, (%rdi)
	movl	$0, %ecx
	cmovne	%ecx, %eax
	movb	%al, (%rdx)
	ret
	.cfi_endproc
.LFE61:
	.size	bitwise_xor_immediate, .-bitwise_xor_immediate
	.p2align 4,,15
	.globl	bitwise_xor_indirect
	.type	bitwise_xor_indirect, @function
bitwise_xor_indirect:
.LFB62:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	movq	%rdx, %rbp
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movzwl	(%rsi), %edi
	call	read_byte@PLT
	movzbl	(%rbx), %ecx
	movl	%ecx, %edx
	xorl	%eax, %edx
	cmpb	%al, %cl
	movl	$-128, %eax
	movb	%dl, (%rbx)
	movl	$0, %edx
	cmovne	%edx, %eax
	movb	%al, 0(%rbp)
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE62:
	.size	bitwise_xor_indirect, .-bitwise_xor_indirect
	.p2align 4,,15
	.globl	compare_register
	.type	compare_register, @function
compare_register:
.LFB63:
	.cfi_startproc
	movzbl	(%rdi), %ecx
	movl	$-64, %eax
	cmpb	(%rsi), %cl
	movl	%ecx, %edi
	setb	%r8b
	subb	(%rsi), %dil
	je	.L129
	cmpb	$1, %r8b
	sbbl	%eax, %eax
	andl	$-16, %eax
	addl	$112, %eax
	cmpb	$1, %r8b
	sbbl	%esi, %esi
	andl	$15, %edi
	andl	$15, %ecx
	andl	$-16, %esi
	addl	$80, %esi
	cmpb	%cl, %dil
	cmovbe	%esi, %eax
.L129:
	movb	%al, (%rdx)
	ret
	.cfi_endproc
.LFE63:
	.size	compare_register, .-compare_register
	.p2align 4,,15
	.globl	compare_immediate
	.type	compare_immediate, @function
compare_immediate:
.LFB64:
	.cfi_startproc
	movzbl	(%rdi), %ecx
	movl	$-64, %eax
	cmpb	%sil, %cl
	movl	%ecx, %edi
	setb	%r8b
	subb	%sil, %dil
	je	.L137
	cmpb	$1, %r8b
	sbbl	%eax, %eax
	andl	$-16, %eax
	addl	$112, %eax
	cmpb	$1, %r8b
	sbbl	%esi, %esi
	andl	$15, %edi
	andl	$15, %ecx
	andl	$-16, %esi
	addl	$80, %esi
	cmpb	%cl, %dil
	cmovbe	%esi, %eax
.L137:
	movb	%al, (%rdx)
	ret
	.cfi_endproc
.LFE64:
	.size	compare_immediate, .-compare_immediate
	.p2align 4,,15
	.globl	compare_indirect
	.type	compare_indirect, @function
compare_indirect:
.LFB65:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbp
	movq	%rdx, %rbx
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movzwl	(%rsi), %edi
	call	read_byte@PLT
	movzbl	0(%rbp), %esi
	movl	$-64, %ecx
	cmpb	%al, %sil
	movl	%esi, %edi
	setb	%dl
	subb	%al, %dil
	je	.L145
	cmpb	$1, %dl
	sbbl	%ecx, %ecx
	andl	$-16, %ecx
	addl	$112, %ecx
	cmpb	$1, %dl
	sbbl	%eax, %eax
	andl	$15, %edi
	andl	$15, %esi
	andl	$-16, %eax
	addl	$80, %eax
	cmpb	%sil, %dil
	cmovbe	%eax, %ecx
.L145:
	movb	%cl, (%rbx)
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE65:
	.size	compare_indirect, .-compare_indirect
	.p2align 4,,15
	.globl	increment_register
	.type	increment_register, @function
increment_register:
.LFB66:
	.cfi_startproc
	movzbl	(%rdi), %r8d
	leal	1(%r8), %ecx
	cmpb	$1, %cl
	movl	%ecx, %r9d
	sbbl	%edx, %edx
	andl	$-112, %edx
	addl	$32, %edx
	cmpb	$1, %cl
	sbbl	%eax, %eax
	andl	$15, %r9d
	andl	$15, %r8d
	andl	$-112, %eax
	cmpb	%r8b, %r9b
	cmovb	%edx, %eax
	movb	%al, (%rsi)
	movb	%cl, (%rdi)
	ret
	.cfi_endproc
.LFE66:
	.size	increment_register, .-increment_register
	.p2align 4,,15
	.globl	increment_register_indirect
	.type	increment_register_indirect, @function
increment_register_indirect:
.LFB67:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	movq	%rsi, %rbp
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movzwl	(%rdi), %edi
	call	read_byte@PLT
	leal	1(%rax), %edx
	cmpb	$1, %dl
	movl	%edx, %esi
	sbbl	%edi, %edi
	andl	$-112, %edi
	addl	$32, %edi
	cmpb	$1, %dl
	sbbl	%ecx, %ecx
	andl	$15, %esi
	andl	$15, %eax
	andl	$-112, %ecx
	cmpb	%al, %sil
	movzbl	%dl, %esi
	cmovb	%edi, %ecx
	movb	%cl, 0(%rbp)
	movzwl	(%rbx), %edi
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	write_byte@PLT
	.cfi_endproc
.LFE67:
	.size	increment_register_indirect, .-increment_register_indirect
	.p2align 4,,15
	.globl	decrement_register
	.type	decrement_register, @function
decrement_register:
.LFB68:
	.cfi_startproc
	movzbl	(%rdi), %edx
	movl	$-64, %eax
	movl	%edx, %ecx
	subb	$1, %cl
	je	.L162
	cmpb	%cl, %dl
	movl	%ecx, %r9d
	sbbl	%eax, %eax
	andl	$16, %eax
	addl	$96, %eax
	cmpb	%cl, %dl
	sbbl	%r8d, %r8d
	andl	$15, %r9d
	andl	$15, %edx
	andl	$16, %r8d
	addl	$64, %r8d
	cmpb	%dl, %r9b
	cmovbe	%r8d, %eax
.L162:
	movb	%al, (%rsi)
	movb	%cl, (%rdi)
	ret
	.cfi_endproc
.LFE68:
	.size	decrement_register, .-decrement_register
	.p2align 4,,15
	.globl	decrement_register_indirect
	.type	decrement_register_indirect, @function
decrement_register_indirect:
.LFB69:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	movq	%rsi, %rbp
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movzwl	(%rdi), %edi
	call	read_byte@PLT
	movl	%eax, %edx
	movl	$-64, %ecx
	subb	$1, %dl
	je	.L169
	cmpb	%dl, %al
	movl	%edx, %esi
	sbbl	%ecx, %ecx
	andl	$16, %ecx
	addl	$96, %ecx
	cmpb	%dl, %al
	sbbl	%edi, %edi
	andl	$15, %esi
	andl	$15, %eax
	andl	$16, %edi
	addl	$64, %edi
	cmpb	%al, %sil
	cmovbe	%edi, %ecx
.L169:
	movb	%cl, 0(%rbp)
	movzwl	(%rbx), %edi
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	movzbl	%dl, %esi
	jmp	write_byte@PLT
	.cfi_endproc
.LFE69:
	.size	decrement_register_indirect, .-decrement_register_indirect
	.p2align 4,,15
	.globl	set_flags_add
	.type	set_flags_add, @function
set_flags_add:
.LFB70:
	.cfi_startproc
	cmpb	$1, %dil
	sbbl	%edx, %edx
	andl	$-128, %edx
	addl	$16, %edx
	cmpb	$1, %dil
	sbbl	%eax, %eax
	andl	$-128, %eax
	cmpb	%sil, %dil
	cmovb	%edx, %eax
	andl	$15, %edi
	andl	$15, %esi
	movl	%eax, %edx
	orl	$32, %edx
	cmpb	%sil, %dil
	cmovb	%edx, %eax
	ret
	.cfi_endproc
.LFE70:
	.size	set_flags_add, .-set_flags_add
	.p2align 4,,15
	.globl	set_flags_sub
	.type	set_flags_sub, @function
set_flags_sub:
.LFB71:
	.cfi_startproc
	testb	%dil, %dil
	movl	$-64, %eax
	je	.L182
	cmpb	%dil, %sil
	sbbl	%eax, %eax
	andl	$16, %eax
	addl	$96, %eax
	cmpb	%dil, %sil
	sbbl	%edx, %edx
	andl	$15, %edi
	andl	$15, %esi
	andl	$16, %edx
	addl	$64, %edx
	cmpb	%sil, %dil
	cmovbe	%edx, %eax
.L182:
	rep ret
	.cfi_endproc
.LFE71:
	.size	set_flags_sub, .-set_flags_sub
	.p2align 4,,15
	.globl	indirect_register_add
	.type	indirect_register_add, @function
indirect_register_add:
.LFB72:
	.cfi_startproc
	movzwl	(%rdi), %ecx
	movl	%ecx, %r8d
	addw	(%rsi), %r8w
	movl	%r8d, %eax
	setc	%sil
	andw	$4095, %cx
	andw	$4095, %ax
	sall	$4, %esi
	cmpw	%cx, %ax
	movzbl	(%rdx), %eax
	setb	%cl
	sall	$5, %ecx
	andl	$-128, %eax
	orl	%esi, %eax
	orl	%ecx, %eax
	movb	%al, (%rdx)
	movw	%r8w, (%rdi)
	ret
	.cfi_endproc
.LFE72:
	.size	indirect_register_add, .-indirect_register_add
	.p2align 4,,15
	.globl	stack_pointer_add
	.type	stack_pointer_add, @function
stack_pointer_add:
.LFB73:
	.cfi_startproc
	movzwl	(%rdi), %eax
	movzbl	%sil, %esi
	addw	%ax, %si
	jc	.L197
	movl	$32, %r8d
	xorl	%ecx, %ecx
.L198:
	movl	%esi, %r9d
	andw	$4095, %ax
	andw	$4095, %r9w
	cmpw	%ax, %r9w
	cmovb	%r8d, %ecx
	movb	%cl, (%rdx)
	movw	%si, (%rdi)
	ret
.L197:
	movl	$48, %r8d
	movl	$16, %ecx
	jmp	.L198
	.cfi_endproc
.LFE73:
	.size	stack_pointer_add, .-stack_pointer_add
	.p2align 4,,15
	.globl	increment_register_16
	.type	increment_register_16, @function
increment_register_16:
.LFB74:
	.cfi_startproc
	rep ret
	.cfi_endproc
.LFE74:
	.size	increment_register_16, .-increment_register_16
	.p2align 4,,15
	.globl	decrement_register_16
	.type	decrement_register_16, @function
decrement_register_16:
.LFB91:
	.cfi_startproc
	rep ret
	.cfi_endproc
.LFE91:
	.size	decrement_register_16, .-decrement_register_16
	.p2align 4,,15
	.globl	rotate_register_left_carry_archive
	.type	rotate_register_left_carry_archive, @function
rotate_register_left_carry_archive:
.LFB76:
	.cfi_startproc
	movzbl	(%rdi), %eax
	rolb	%al
	testb	%al, %al
	movb	%al, (%rdi)
	je	.L206
	xorl	%eax, %eax
	xorl	%edx, %edx
	movb	%al, (%rsi)
	movzbl	(%rdi), %eax
	sall	$4, %eax
	andl	$16, %eax
	orl	%edx, %eax
	movb	%al, (%rsi)
	ret
	.p2align 4,,10
	.p2align 3
.L206:
	movl	$-128, %edx
	movl	%edx, %eax
	movb	%al, (%rsi)
	movzbl	(%rdi), %eax
	sall	$4, %eax
	andl	$16, %eax
	orl	%edx, %eax
	movb	%al, (%rsi)
	ret
	.cfi_endproc
.LFE76:
	.size	rotate_register_left_carry_archive, .-rotate_register_left_carry_archive
	.p2align 4,,15
	.globl	rotate_indirect_left_carry_archive
	.type	rotate_indirect_left_carry_archive, @function
rotate_indirect_left_carry_archive:
.LFB77:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	movq	%rsi, %rbp
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movzwl	(%rdi), %edi
	call	read_byte@PLT
	rolb	%al
	movzbl	%al, %esi
	cmpb	$1, %al
	movl	%esi, %edx
	sbbl	%ecx, %ecx
	sall	$4, %edx
	andl	$-128, %ecx
	andl	$16, %edx
	orl	%ecx, %edx
	movb	%dl, 0(%rbp)
	movzwl	(%rbx), %edi
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	write_byte@PLT
	.cfi_endproc
.LFE77:
	.size	rotate_indirect_left_carry_archive, .-rotate_indirect_left_carry_archive
	.p2align 4,,15
	.globl	rotate_register_left_through_carry
	.type	rotate_register_left_through_carry, @function
rotate_register_left_through_carry:
.LFB78:
	.cfi_startproc
	movzbl	(%rdi), %ecx
	movzbl	(%rsi), %eax
	movl	%ecx, %edx
	sarl	$4, %eax
	addl	%ecx, %ecx
	sarl	$3, %edx
	andl	$1, %eax
	andl	$16, %edx
	orl	%ecx, %eax
	movl	%edx, %ecx
	movb	%al, (%rdi)
	orl	$-128, %ecx
	testb	%al, %al
	cmove	%ecx, %edx
	movb	%dl, (%rsi)
	ret
	.cfi_endproc
.LFE78:
	.size	rotate_register_left_through_carry, .-rotate_register_left_through_carry
	.p2align 4,,15
	.globl	rotate_indirect_left_through_carry
	.type	rotate_indirect_left_through_carry, @function
rotate_indirect_left_through_carry:
.LFB79:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	movq	%rsi, %rbp
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movzwl	(%rdi), %edi
	call	read_byte@PLT
	movzbl	0(%rbp), %edx
	movzbl	%al, %eax
	movl	%eax, %ecx
	addl	%eax, %eax
	sarl	$3, %ecx
	andl	$16, %ecx
	sarl	$4, %edx
	movl	%ecx, %esi
	andl	$1, %edx
	orl	$-128, %esi
	orb	%al, %dl
	cmove	%esi, %ecx
	movzbl	%dl, %esi
	movb	%cl, 0(%rbp)
	movzwl	(%rbx), %edi
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	write_byte@PLT
	.cfi_endproc
.LFE79:
	.size	rotate_indirect_left_through_carry, .-rotate_indirect_left_through_carry
	.p2align 4,,15
	.globl	rotate_register_right_carry_archive
	.type	rotate_register_right_carry_archive, @function
rotate_register_right_carry_archive:
.LFB80:
	.cfi_startproc
	movzbl	(%rdi), %eax
	rorb	%al
	cmpb	$1, %al
	movb	%al, (%rdi)
	sbbl	%edx, %edx
	andl	$-128, %edx
	addl	$16, %edx
	cmpb	$1, %al
	sbbl	%eax, %eax
	andl	$-128, %eax
	movb	%al, (%rsi)
	cmpb	$0, (%rdi)
	cmovne	%edx, %eax
	movb	%al, (%rsi)
	ret
	.cfi_endproc
.LFE80:
	.size	rotate_register_right_carry_archive, .-rotate_register_right_carry_archive
	.p2align 4,,15
	.globl	rotate_indirect_right_carry_archive
	.type	rotate_indirect_right_carry_archive, @function
rotate_indirect_right_carry_archive:
.LFB81:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	movq	%rsi, %rbp
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movzwl	(%rdi), %edi
	call	read_byte@PLT
	rorb	%al
	cmpb	$1, %al
	movzbl	%al, %esi
	sbbl	%edx, %edx
	andl	$112, %edx
	addl	$16, %edx
	movb	%dl, 0(%rbp)
	movzwl	(%rbx), %edi
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	write_byte@PLT
	.cfi_endproc
.LFE81:
	.size	rotate_indirect_right_carry_archive, .-rotate_indirect_right_carry_archive
	.p2align 4,,15
	.globl	rotate_register_right_through_carry
	.type	rotate_register_right_through_carry, @function
rotate_register_right_through_carry:
.LFB82:
	.cfi_startproc
	movzbl	(%rdi), %ecx
	movzbl	(%rsi), %eax
	movl	%ecx, %edx
	sall	$3, %eax
	shrb	%cl
	sall	$4, %edx
	andl	$-128, %eax
	andl	$16, %edx
	orl	%ecx, %eax
	movl	%edx, %ecx
	movb	%al, (%rdi)
	orl	$-128, %ecx
	testb	%al, %al
	cmove	%ecx, %edx
	movb	%dl, (%rsi)
	ret
	.cfi_endproc
.LFE82:
	.size	rotate_register_right_through_carry, .-rotate_register_right_through_carry
	.p2align 4,,15
	.globl	rotate_indirect_right_through_carry
	.type	rotate_indirect_right_through_carry, @function
rotate_indirect_right_through_carry:
.LFB83:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	movq	%rsi, %rbp
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movzwl	(%rdi), %edi
	call	read_byte@PLT
	movzbl	0(%rbp), %edx
	movzbl	%al, %eax
	movl	%eax, %ecx
	addl	%eax, %eax
	sall	$4, %ecx
	andl	$16, %ecx
	sall	$3, %edx
	movl	%ecx, %esi
	andl	$-128, %edx
	orl	$-128, %esi
	orb	%al, %dl
	cmove	%esi, %ecx
	movzbl	%dl, %esi
	movb	%cl, 0(%rbp)
	movzwl	(%rbx), %edi
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	write_byte@PLT
	.cfi_endproc
.LFE83:
	.size	rotate_indirect_right_through_carry, .-rotate_indirect_right_through_carry
	.p2align 4,,15
	.globl	swap_nibble_register
	.type	swap_nibble_register, @function
swap_nibble_register:
.LFB84:
	.cfi_startproc
	movzbl	(%rdi), %eax
	rolb	$4, %al
	cmpb	$1, %al
	sbbl	%edx, %edx
	andl	$-128, %edx
	movb	%dl, (%rsi)
	movb	%al, (%rdi)
	ret
	.cfi_endproc
.LFE84:
	.size	swap_nibble_register, .-swap_nibble_register
	.p2align 4,,15
	.globl	swap_nibble_indirect
	.type	swap_nibble_indirect, @function
swap_nibble_indirect:
.LFB85:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	movq	%rsi, %rbp
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movzwl	(%rdi), %edi
	call	read_byte@PLT
	rolb	$4, %al
	cmpb	$1, %al
	movzbl	%al, %esi
	sbbl	%edx, %edx
	andl	$-128, %edx
	movb	%dl, 0(%rbp)
	movzwl	(%rbx), %edi
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	write_byte@PLT
	.cfi_endproc
.LFE85:
	.size	swap_nibble_indirect, .-swap_nibble_indirect
	.p2align 4,,15
	.globl	decimal_adjust_accumulator
	.type	decimal_adjust_accumulator, @function
decimal_adjust_accumulator:
.LFB86:
	.cfi_startproc
	movzbl	(%rsi), %ecx
	movzbl	(%rdi), %eax
	movl	%ecx, %edx
	andl	$64, %edx
	testb	$32, %cl
	jne	.L237
	movl	%eax, %r8d
	andl	$15, %r8d
	cmpb	$9, %r8b
	ja	.L237
.L238:
	andl	$16, %ecx
	jne	.L239
	movl	%eax, %ecx
	andl	$-16, %ecx
	cmpb	$-112, %cl
	jbe	.L240
.L239:
	addl	$96, %eax
	orl	$16, %edx
	movb	%al, (%rdi)
.L240:
	movl	%edx, %ecx
	orl	$-128, %ecx
	testb	%al, %al
	cmove	%ecx, %edx
	movb	%dl, (%rsi)
	ret
	.p2align 4,,10
	.p2align 3
.L237:
	addl	$6, %eax
	movb	%al, (%rdi)
	movzbl	(%rsi), %ecx
	jmp	.L238
	.cfi_endproc
.LFE86:
	.size	decimal_adjust_accumulator, .-decimal_adjust_accumulator
	.p2align 4,,15
	.globl	complement_accumulator
	.type	complement_accumulator, @function
complement_accumulator:
.LFB87:
	.cfi_startproc
	notb	(%rdi)
	ret
	.cfi_endproc
.LFE87:
	.size	complement_accumulator, .-complement_accumulator
	.p2align 4,,15
	.globl	complement_carry_flag
	.type	complement_carry_flag, @function
complement_carry_flag:
.LFB88:
	.cfi_startproc
	movzbl	(%rdi), %eax
	andl	$-97, %eax
	xorl	$16, %eax
	movb	%al, (%rdi)
	ret
	.cfi_endproc
.LFE88:
	.size	complement_carry_flag, .-complement_carry_flag
	.p2align 4,,15
	.globl	set_carry_flag
	.type	set_carry_flag, @function
set_carry_flag:
.LFB89:
	.cfi_startproc
	movzbl	(%rdi), %eax
	andl	$-97, %eax
	orl	$16, %eax
	movb	%al, (%rdi)
	ret
	.cfi_endproc
.LFE89:
	.size	set_carry_flag, .-set_carry_flag
	.ident	"GCC: (Ubuntu 7.2.0-8ubuntu3.2) 7.2.0"
	.section	.note.GNU-stack,"",@progbits
