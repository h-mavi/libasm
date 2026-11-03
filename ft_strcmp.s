; int strcmp(const char *s1, const char *s2)

global ft_strcmp

section .text
ft_strcmp:
	xor		rax, rax
.while:
	mov		dl, [rsi]
	mov		al, [rdi]
	cmp		al, dl
	jne		.diff
	test	al, al
	jz		.return
	inc		rsi
	inc		rdi
	jmp		.while
.diff:
	movzx	eax, al
	movzx	edx, dl
	sub		eax, edx
	ret
.return:
	xor		eax, eax
	ret