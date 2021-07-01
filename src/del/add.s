	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 10, 16	sdk_version 11, 0
	.globl	_arm64_add              ## -- Begin function arm64_add
	.p2align	4, 0x90
_arm64_add:                             ## @arm64_add
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	-4(%rbp), %eax
	addl	-8(%rbp), %eax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
.subsections_via_symbols
