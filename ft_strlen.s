;size_t strlen(const char *s)

global ft_strlen

section .text
ft_strlen:
	xor		rax, rax
.while:
	mov		dl, [rdi + rax]
	test	dl, dl
	jz		.done
	inc		rax
	jmp		.while
.done:
	ret