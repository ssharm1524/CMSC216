;;; Shiva Sharma
;;; 117912678
;;; ssharm08

;;; Implementation that determines the integer sqare root of a passed positive number
    .global isqrt ; exposes isqrt as a label for the linker
isqrt:            ; the label: the next instruction starts isqrt function.
    cpi r24, 2    ; compare our passed int (we will refer to this as n in futher commetns) and 2
    brlo lesstwo  ; branch to lesstwo if n<2

    push r24      ; stash this calls version of n in its stack frame
    lsr r24       ; n >> 1
    lsr r24       ; n >> 1. r24 now holds n>>2
    call isqrt    ; isqrt(n >> 2)
    lsl r24       ; isqrt(n >> 2) << 1
    mov r20, r24  ; store isqrt(n >> 2) << 1 (small) in r20
    pop r24       ; pop this version of n back into r24
    mov r22, r24  ; move n into r22

    mov r30, r20  ; copy small into r30
    adiw r30,1    ; small+1. r30 now holds large

    mov r24, r30  ; move large into register r24
    ldi r26, 2    ; set our increment variable (we will refer to it as i from now on) to 2
    clr r27       ; set our increment variable (we will refer to it as i from now on) to 2

    ;; loop that squares large by repeated addition
startloop:
    cp r30, r26   ; compare large to i 
    brlo endloop  ; if large < i, branch to endloop

    add r24, r30  ; add large to the originial large. essenentially large*i
    adc r25, r31  ; add large to the originial large. essenentially large*i

    adiw r26, 1   ; i++
    jmp startloop ; restart loop
endloop:
    
    cp r22, r24   ; compare n to large*large
    brlo retsmall ; if n < large*large branch to retsmall
    mov r24, r30  ; set out register to large
    clr r25       ; set out register to large
    ret           ; return large

retsmall:
    mov r24, r20  ; set out register to small
    clr r25       ; set out register to small
    ret           ; return small
    
lesstwo:
    ret           ; return with n still in r24 (retun n)

