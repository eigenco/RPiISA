	cpu     8086
	org     0xf000

;;;; BLANK INTERRUPT ;;;;

	iret

;;;; STACK ;;;;

	mov	sp, 0x30
	mov	ss, sp
	mov	sp, 0xfe

;;;; MODIFY INTERRUPT VECTOR TABLE ;;;;

	mov     word [es:0x08*4], INT08 ; TIMER	HW
	mov     word [es:0x09*4], INT09 ; KEYBOARD HW
;	mov     word [es:0x16*4], INT16 ; KEYBOARD SW
	mov     word [es:0x1a*4], INT1A ; TIMER SW

;;;; BOOT ;;;;

;	call	0xc000:3

	jmp     0:0x7c00

;;;; TIMER HW ;;;;

INT08:
	push	ds
	push	ax
	mov     ax, 0x40
	mov     ds, ax
	clc
	adc     word [ds:0x6c], 1
	adc     word [ds:0x6e], 0
	int     0x1c
	pop     ax
	pop     ds
	iret

;;;; KEYBOARD HW ;;;;

INT09:
	in	al, 0x60
	db	0x0f, 0x09
	iret

;;;; KEYBOARD SW ;;;;

INT16:
	xor	ax, ax
	iret

;;;; TIMER SW ;;;;

INT1A:
        push    ax
        push    ds
        mov     ax, 0x40
        mov     ds, ax
        mov     cx, [ds:0x6e]
        mov     dx, [ds:0x6c]
        pop     ds
        pop     ax
        iret

;;;; END ;;;;

	times 4096-($-$$) db 0
