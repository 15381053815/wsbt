.CODE

lxexec	PROC
	mov		qword ptr[rsp+08h],rcx
	push	rdi
	push	rbx
	mov		rdi,rsp
	sub		rsp,20h

	mov		rax,qword ptr[rcx]
	cmp		rax,0
	je		lxe_exit
	mov		rbx,qword ptr[rcx+08h]
	cmp		rbx,0
	jne		lxe_callthis
	mov		rdx,qword ptr[rcx+20h]
	mov		r8, qword ptr[rcx+28h]
	mov		rcx,qword ptr[rcx+18h]
	jmp		lxe_call
lxe_callthis:
	mov		rdx,qword ptr[rcx+18h]
	mov		r8, qword ptr[rcx+20h]
	mov		r9, qword ptr[rcx+28h]
	mov		rcx,rbx
lxe_call:
	call	rax	
lxe_exit:
	add		rsp,20h
	pop		rbx
	pop		rdi
	ret
lxexec	ENDP

lxcas	PROC
	mov		rax,rdx
	lock cmpxchg [rcx],r8
	setz	al
	and		rax,0FFh
	ret		
lxcas	ENDP
END