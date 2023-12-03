.MODEL LARGE,C
.CODE

EXTRN GrBuff:DWORD
EXTRN GrPtr:DWORD
EXTRN CharPtr:DWORD
EXTRN pWall:DWORD
EXTRN GrArea:DWORD
EXTRN pFon6:DWORD
EXTRN tex_ptr:DWORD
EXTRN pWeap:DWORD
EXTRN tex_n:WORD
EXTRN tex_column:WORD
EXTRN tex_top:WORD
EXTRN tex_end:WORD
EXTRN tex_index:WORD
EXTRN tex_x:WORD
EXTRN tex_shad:WORD
EXTRN light_seg:WORD
EXTRN tex_row:DWORD
EXTRN tex_dy:DWORD
EXTRN tex_index2:DWORD

.386p
LOCALS

PUBLIC CLEAR
PUBLIC NEGATIVE
PUBLIC POSITIVE
PUBLIC PUTCH6
PUBLIC PUTCH6COLORED
PUBLIC SWBUFF
PUBLIC WAL_COLUMN
PUBLIC TRN_COLUMN
PUBLIC TRI_COLUMN
PUBLIC DRAW_BACK

CLEAR PROC FAR
  les di,GrBuff
  add di,1280
  add di,DGROUP:tex_x
  mov cx,DGROUP:tex_top
  mov bx,120
  sub bx,cx
  mov eax,0A0A1A0A1h
  cld
@@L1:
	 mov dx,cx
	 mov cx,25
	 rep stosd
	 mov cx,dx
	 add di,220
  loop @@L1
  mov eax,0B0B1B0B1h
  mov cx,bx
@@L2:
	 mov dx,cx
	 mov cx,25
	 rep stosd
	 mov cx,dx
	 add di,220
  loop @@L2
  ret
CLEAR ENDP

NEGATIVE PROC FAR
  les di,DGROUP:GrBuff
  add di,1281
  add di,DGROUP:tex_x
  mov bx,DGROUP:tex_n
  mov bh,15
  mov dx,120
@@L1:
  mov cx,100
@@L2:
  mov al,BYTE PTR es:[di]
  and al,bh
  mov ah,bl
  sub ah,al
  mov al,ah
  stosb
  loop @@L2
  add di,220
  dec dx
  jnz short @@L1
  ret
NEGATIVE ENDP

POSITIVE PROC FAR
  les di,DGROUP:GrBuff
  add di,1281
  add di,DGROUP:tex_x
  mov bx,DGROUP:tex_n
  mov bh,15
  mov dx,120
@@L1:
  mov cx,100
@@L2:
  mov al,BYTE PTR es:[di]
  and al,bh
  mov ah,bl
  add ah,al
  mov al,ah
  stosb
  loop @@L2
  add di,220
  dec dx
  jnz short @@L1
  ret
POSITIVE ENDP

PUTCH6 PROC FAR
push ds
push di
push si

lfs bx,DGROUP:CharPtr
les di,DGROUP:GrPtr
mov ax,DGROUP:tex_x
add di,ax
mov ax,DGROUP:tex_top
shl ax,6
add di,ax
shl ax,2
add di,ax

mov ax,DGROUP:tex_n
lds si,DGROUP:pFon6
shl ax,2
add si,ax
shl ax,3
add si,ax
mov al,5
xor ch,ch

@@TEX_LOOP:
 mov cl,BYTE PTR ds:[si]
 mov ah,BYTE PTR fs:[bx]
 jcxz short @@L1
 mov es:[di],ah
@@L1:
 inc di
 inc si
 inc bx
 mov cl,BYTE PTR ds:[si]
 mov ah,BYTE PTR fs:[bx]
 jcxz short @@L2
 mov es:[di],ah
@@L2:
 inc di
 inc si
 inc bx
 mov cl,BYTE PTR ds:[si]
 mov ah,BYTE PTR fs:[bx]
 jcxz short @@L3
 mov es:[di],ah
@@L3:
 inc di
 inc si
 inc bx
 mov cl,BYTE PTR ds:[si]
 mov ah,BYTE PTR fs:[bx]
 jcxz short @@L4
 mov es:[di],ah
@@L4:
 inc di
 inc si
 inc bx
 mov cl,BYTE PTR ds:[si]
 mov ah,BYTE PTR fs:[bx]
 jcxz short @@L5
 mov es:[di],ah
@@L5:
 inc si
 inc si
 inc bx
 inc bx
 add di,316
 dec al
 jnz short @@TEX_LOOP

pop si
pop di
pop ds
ret
PUTCH6 ENDP

PUTCH6COLORED PROC FAR
push ds
push di
push si

les di,DGROUP:GrPtr
mov ax,DGROUP:tex_x
add di,ax
mov ax,DGROUP:tex_top
shl ax,6
add di,ax
shl ax,2
add di,ax

mov ax,DGROUP:tex_n
lds si,DGROUP:pFon6
shl ax,2
add si,ax
shl ax,3
add si,ax
mov al,5
xor ch,ch

@@TEX_LOOP:
 mov cl,BYTE PTR ds:[si]
 jcxz short @@L1
 mov es:[di],cl
@@L1:
 inc di
 inc si
 mov cl,BYTE PTR ds:[si]
 jcxz short @@L2
 mov es:[di],cl
@@L2:
 inc di
 inc si
 mov cl,BYTE PTR ds:[si]
 jcxz short @@L3
 mov es:[di],cl
@@L3:
 inc di
 inc si
 mov cl,BYTE PTR ds:[si]
 jcxz short @@L4
 mov es:[di],cl
@@L4:
 inc di
 inc si
 mov cl,BYTE PTR ds:[si]
 jcxz short @@L5
 mov es:[di],cl
@@L5:
 inc si
 inc si
 add di,316
 dec al
 jnz short @@TEX_LOOP

pop si
pop di
pop ds
ret
PUTCH6COLORED ENDP

SWBUFF PROC FAR
push ds
push es
push di
push si
push cx
les di,DGROUP:GrArea
lds si,DGROUP:GrBuff
mov cx,16000
cld
rep movsd
pop cx
pop si
pop di
pop es
pop ds
ret
SWBUFF ENDP

WAL_COLUMN PROC FAR
push ds
push si
push di
push bp

les di,DGROUP:GrBuff
mov ax,DGROUP:tex_x
add di,ax
mov ax,DGROUP:tex_top
shl ax,6
add di,ax
shl ax,2
add di,ax

mov cx,DGROUP:tex_end
mov ax,DGROUP:tex_n
mov bx,DGROUP:tex_column
shl ax,10
shl bx,5
add ax,bx
mov ebp,DGROUP:tex_dy
mov edx,DGROUP:tex_index2

mov bx,DGROUP:tex_shad
cmp bl,32
jb short @@L0
mov bl,31
@@L0:
shl bx,4
add bx,DGROUP:light_seg
mov fs,bx
xor bh,bh

lds si,DGROUP:pWall
add si,ax
shl esi,16
add edx,esi
mov esi,edx
shr esi,16
@@L1:
	mov bl,ds:[si]
	mov al,fs:[bx]
	mov es:[di],al
	add di,320
	add edx,ebp
	mov esi,edx
	shr esi,16
	loop @@L1
pop bp
pop di
pop si
pop ds
ret
WAL_COLUMN ENDP

TRN_COLUMN PROC FAR
push ds
push si
push di
les di,DGROUP:GrBuff
mov ax,DGROUP:tex_x
add di,ax
mov ax,DGROUP:tex_top
shl ax,6
add di,ax
shl ax,2
add di,ax

mov bx,DGROUP:tex_end
mov cx,DGROUP:tex_column
mov eax,DGROUP:tex_dy
mov edx,DGROUP:tex_index2

lds si,DGROUP:tex_ptr
add si,cx
shl esi,16
add edx,esi
mov esi,edx
shr esi,16
xor ch,ch
@@L1:
	mov cl,ds:[si]
	jcxz short @@L2
	mov es:[di],cl
@@L2:
	add di,320
	add edx,eax
	mov esi,edx
	shr esi,16
	dec bx
	jnz @@L1
pop di
pop si
pop ds
ret
TRN_COLUMN ENDP

TRI_COLUMN PROC FAR
push ds
push si
push di
les di,DGROUP:GrBuff
mov ax,DGROUP:tex_x
add di,ax
mov ax,DGROUP:tex_top
shl ax,6
add di,ax
shl ax,2
add di,ax

mov cx,DGROUP:tex_end
mov bx,DGROUP:tex_column
mov eax,DGROUP:tex_dy

lds si,DGROUP:tex_ptr
add si,bx
shl esi,16
mov edx,esi
shr esi,16
@@L1:
	mov bl,ds:[si]
	mov es:[di],bl
	add di,320
	add edx,eax
	mov esi,edx
	shr esi,16
	loop @@L1
pop di
pop si
pop ds
ret
TRI_COLUMN ENDP

DRAW_BACK PROC FAR
push ds
push di
push si

les di,DGROUP:GrBuff
lds si,DGROUP:tex_ptr

mov bx,20
mov ax,10    ;// 20*10 = 200
jmp short @@L1
@@L3:
	mov ax,10
	sub si,(10*16)
@@L2:
	add si,16
@@L1:
	mov cx,20  ;// 20*(4+4+4+4) = 320
@@L0:
	movsd
	movsd
	movsd
	movsd
	sub si,16
loop @@L0
dec ax
jnz short @@L2
dec bx
jnz short @@L3

pop si
pop di
pop ds
ret
DRAW_BACK ENDP

END
