	global main
	extern printf
	section text
	;; FUNC main:
	main:
	push rbp
	mov rbp,rsp
	sub rsp,100
	;; LOADL 4
	mov rax,4
	push rax
	;; LOADL 8
	mov rax,8
	push rax
	;; CALL fact
	call fact
	pop rbx
	add rsp,rbx
	push rax
	;; PRINTF
	 mov rdi,fmt1
	 pop rsi
	 mov rax,0
	 push rbp
	 call printf
	 pop rbp
	;; LOADL 0
	mov rax,0
	push rax
	;; RET
	pop rax
	add rsp,100
	pop rbp
	ret
	;; FUNC fact:
	fact:
	push rbp
	mov rbp,rsp
	sub rsp,100
	;; PARAM 1
	mov rsi, qword[rbp+24]
	mov qword[rbp-8],rsi
	;; LOADV 1
	push qword[rbp-8]
	;; LOADL 0
	mov rax,0
	push rax
	;; ISEQ
	pop rbx
	pop rax
	cmp rax, rbx
	je L4
	mov rax, 0
	jmp L5
	L4:
	mov rax, 1
	L5:
	push rax
	;; JUMPF L2
	pop rax
	cmp rax, 0
	je L2
	;; LOADL 1
	mov rax,1
	push rax
	;; RET
	pop rax
	add rsp,100
	pop rbp
	ret
	;; JUMP L3
	jmp L3
	L2: 
	;; LOADV 1
	push qword[rbp-8]
	;; LOADV 1
	push qword[rbp-8]
	;; LOADL 1
	mov rax,1
	push rax
	;; SUB
	pop rbx
	pop rax
	sub rax,rbx
	push rax
	;; LOADL 8
	mov rax,8
	push rax
	;; CALL fact
	call fact
	pop rbx
	add rsp,rbx
	push rax
	;; MULT
	pop rbx
	pop rax
	imul rbx
	push rax
	;; RET
	pop rax
	add rsp,100
	pop rbp
	ret
	L3: 

 section .data
 fmt1: db `%d\n`, 0
