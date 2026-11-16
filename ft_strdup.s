; char *strdup(const char *s)

global ft_strdup
extern __errno_location
extern ft_strlen
extern ft_strcpy
extern malloc

section .text
ft_strdup:
	push	rbx
	mov		rbx, rdi
	call	ft_strlen wrt ..plt
	mov		rcx, rax
	inc		rcx					; per il null-terminator
	mov		rdi, rcx
	call	malloc wrt ..plt
	test	rax, rax
	jz		.end
	mov		rdi, rax
	mov		rsi, rbx
	call	ft_strcpy wrt ..plt
.end:
	pop		rbx
	ret