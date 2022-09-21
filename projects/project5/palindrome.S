;;; Shiva Sharma
;;; 117912678
;;; ssharm08

;;; Implementation that determines if the passed string is a palindrome
    .global is_palindrome ; exposes is_palindrome as a label for the linker
is_palindrome:            ; the label: the next instruction starts is_palindrome() function.
    push r24              ; stash the str onto the stack so it is saved after strlen call
    push r25              ; stash the str onto the stack so it is saved after strlen call
    call strlen           ; call strlen on the passed string in r24. we now have the length of that string in r24
    mov r22, r24          ; put the returned length of the string in r23:r22
    mov r23, r25          ; put the returned length of the string in r23:r22
    pop r25               ; pop the str off the stack back into r25:r24 
    pop r24               ; pop the str off the stack back into r25:r24 
    mov r26, r24          ; copy the str into r27:r26 so that we have X pointing to it
    mov r27, r25          ; copy the str into r27:r26 so that we have X pointing to it
    mov r30, r24          ; stash the str in r31:r30 so that we have Z pointing to it
    mov r31, r25          ; stash the str in r31:r30 so that we have Z pointing to it
    add r30, r22          ; add strlen to Z so Z points to the end of the str+1. We leave the +1 as we will be doing pre-decrement later
    add r31, r23          ; add strlen to Z so Z points to the end of the str+1. We leave the +1 as we will be doing pre-decrement later
    
    ;; loop goes thru half the string and compares corresponding characters in the first half with the second
    clr r24               ; load the increment variable with value 0 (we will refer to this as i) into r24 (adiw compatible register)
    LSR r22               ; shift all bits in r20 1 right (divide it by 2)
startloop:
    cp r24, r22           ; compare strlen/2 with i      
    brsh true             ; if !(i <= strlen/2) then the whole loop has ran and we have a palindrome so we branch to true

    ld r18, X+            ; load str[i] into r18
    ld r20, -Z            ; load str[length-i-1] into r20

    cp r18, r20           ; compare the two characters
    brne false            ; if they are different then this is not a palindrome and we branch to false

    adiw r24, 1           ; increment i by 1
    jmp startloop         ; restart the loop

true:
    ldi r24, 1            ; load true (one) into the out registers r25:r24 
    clr r25               ; load true (one) into the out registers r25:r24 
    ret                   ; end the function and return 1

false: 
    clr r24               ; load false (zero) into the out registers r25:r24 
    clr r25               ; load false (zero) into the out registers r25:r24 
    ret                   ; end the function and return 0

;;; Implementation finds length of passed string then returns it as a uint8_t
strlen:                   ; the label: the next instruction starts the strlen fnctn 
    mov r26,r24           ; copy argument to X (r27:26) pointer
    mov r27,r25           ; copy argument to X (r27:26) pointer
    clr r24               ; initialize return value to zero
    clr r25               ; initialize return value to zero
2:  
    ld r20, X+            ; load X with post-increment
    cpi r20, 0            ; compare loaded value to 0 
    breq 1f               ; if loaded value was zero, branch 1f (label 1, forward)
    adiw r24, 1           ; increment return value
    jmp 2b                ; jump 2b (label 2, backward)
1:
    ret                   ; return