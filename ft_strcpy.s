;char *strcpy(char *restrict dst, const char *restrict src)

global ft_strcpy

section .text
ft_strcpy:
	mov		rax, rdi
.while:
	mov		dl, [rsi]
	mov		[rdi], dl
	test	dl, dl
	jz		.done
	inc		rsi
	inc		rdi
	jmp		.while
.done:
	ret