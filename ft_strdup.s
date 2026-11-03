; char *strdup(const char *s)

global ft_strdup
extern __errno_location
extern malloc

section .text
ft_strdup:
	push	rbx
	mov		rbx, rdi
	xor		rcx, rcx
.len_count:
	mov		dl, [rbx + rcx]
	test	dl, dl
	jz		.alloc
	inc		rcx
	jmp		.len_count
.alloc:
	inc		rcx					; per il null-terminator
	mov		rdi, rcx
	call	malloc wrt ..plt
	test	rax, rax
	jz		.end
	xor		rcx, rcx
.while:
	mov		dl, [rbx + rcx]
	mov		[rax + rcx], dl
	test	dl, dl
	jz		.end
	inc		rcx
	jmp		.while
.end:
	pop		rbx
	ret