;;; Shiva Sharma
;;; 117912678
;;; ssharm08

;;; Implementation that computes the nth fibonacci number.
    .global fib                 ; exposes fib as a label for the linker
fib:                            ; the label: the next instruction starts fib() function.
    tst r24                     ; test if input is 0
    breq zerocase               ; if so, go to the zero case
    cpi r24, 1                  ; test if input is 1
    breq onecase                ; if so, go to the one case

    sbci r24, 1                 ; put n-1 in r25:r24
    push r24                    ; stash n-1 in the stack
    push r25                    ; stash n-1 in the stack
    call fib                    ; call fib(n-1)
    mov r20, r24                ; store fib(n-1) in r21:r20 
    mov r21, r25                ; store fib(n-1) in r21:r20 
    pop r25                     ; pop n-1 back into r25:r24
    pop r24                     ; pop n-1 back into r25:r24
    push r20                    ; stash fib(n-1) in the stack
    push r21                    ; stash fib(n-1) in the stack
    sbci r24, 1                 ; put n-2 in r24 and r25
    call fib                    ; call fib(n-2)
    pop r21                     ; pop fib(n-1) back into r21:r20
    pop r20                     ; pop fib(n-1) back into r21:r20
    add r24, r20                ; store fib(n-1)+fib(n-2) in the out registers r25:r24
    adc r25, r21                ; store fib(n-1)+fib(n-2) in the out registers r25:r24
    ret                         ; return fib(n-1)+fib(n+2)

zerocase:
    clr 24                      ; set return val to 0
    clr 25                      ; set return val to 0
    ret                         ; return 0

onecase:
    ldi 24, 1                   ; set return val to 1
    clr 25                      ; set return val to 1
    ret                         ; return 0
