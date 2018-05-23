.486
.model flat,C
.CODE

lxexec	PROC
	push	ebp
	mov		ebp,esp
	mov		eax,dword ptr[ebp+08h]
	mov		ecx,dword ptr[eax+1Ch]
	cmp		ecx,0
	je		lxe_skipparam
	mov		ebx,ecx
	dec		ebx
	lea		ebx,dword ptr[eax+0Ch+ebx*4]
lxe_loopparam:
	mov		edx,dword ptr[ebx]
	push	edx
	sub		ebx,4
	loop	lxe_loopparam
lxe_skipparam:
	mov		ecx,dword ptr[eax+04h]
	mov		ebx,dword ptr[eax]
	call	ebx
	mov		ebx,dword ptr[ebp+08h]
	cmp		dword ptr[ebx+18h],2
	jne		lxe_noclrstack
	mov		ecx,dword ptr[ebx+1Ch]
	shl		ecx,2
	add		esp,ecx
lxe_noclrstack:
	pop		ebp
	ret
lxexec	ENDP

lxcas	PROC
	push	ebp
	mov		ebp,esp
	mov		eax,dword ptr[ebp+0ch]
	mov		ecx,dword ptr[ebp+10h]
	mov		edx,dword ptr[ebp+08h]

	lock	cmpxchg [edx],ecx
	setz	al
	and		eax,0FFh
	pop		ebp
	ret		
lxcas	ENDP
END