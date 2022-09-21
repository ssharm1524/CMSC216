;;; Shiva Sharma
;;; 117912678
;;; ssharm08

    .data
pctd:
    .asciz "%d"           ; Format string for first int
pctdwspace:
    .asciz " %d"          ; Format string for following ints

    .text
;;; Implementation that transforms an array by adding the value at an index to the sum of the remainder of the array. Returns the sum.
    .global reverse_prefix_sum ; exposes reverse_prefix_sum as a label for the linker
reverse_prefix_sum:            ; the label: the next instruction starts reverse_prefix_sum function.
    mov r26, r24               ; moves inputted pointer to r27:r26 so that we can use X
    mov r27, r25               ; moves inputted pointer to r27:r26 so that we can use X
    ld r22, X                  ; load the value at the pointer into r22 and then increment the pointer for the recursive call
    
    cpi r22, -1                ; compare the value to -1
    breq endcase               ; if the value is -1 the array is done and we branch to the endcase to return 0
    
    push r27                   ; stash the pointer to this calls value in the stack
    push r26                   ; stash the pointer to this calls value in the stack
    push r23                   ; stash this calls value in the stack 
    push r22                   ; stash this calls value in the stack 

    adiw r26, 1                ; increase the memory location of our pointer by one (to point to the next element)
    mov r24, r26               ; store the pointer to the next element into r25:r24 (input registers)
    mov r25, r27               ; store the pointer to the next element into r25:r24 (input registers)
    call reverse_prefix_sum    ; recursive call on the pointer to the next element
    
    pop r22                    ; put this calls value back into r23:r22
    pop r23                    ; put this calls value back into r23:r22
    pop r26                    ; set our pointer to point back to the location where this calls value is stored
    pop r27                    ; set our pointer to point back to the location where this calls value is stored

    add r24, r22               ; add the lowbytes of the recursive call and this calls value, and store it in r24
    clr r23                    ; clear the high byte of this calls value, as we only want to add the carry from our previous operation into r25. We dont want to overadd by also adding the highbyte of the value. The way the fucntion works, we look at values as 8 bit numbers INSIDE the funciton, but can return them as 16 bit.
    adc r25, r23               ; add the carry of the lowbit addition into to r25
                      
    st X, r24                  ; set the value at our pointer to the sum of the recursive call + this calls original value
    ret                        ; return the sum of the recursive call + this calls original value
endcase:
    clr r24                    ; set r25:r24 to 0
    clr r25                    ; set r25:r24 to 0
    ret                        ; return 0


;;; Implementation that prints an array of ints passed by an 8bit signed int pointer until it sees -1 
    .global print_array        ; exposes print_array as a label for the linker
print_array:                   ; the label: the next instruction starts the print_array function
    mov r26, r24               ; moves inputted pointer to r27:r26 so that we can use X
    mov r27, r25               ; moves inputted pointer to r27:r26 so that we can use X
    ld r24, X+                 ; load r25:r24 with the first element in the array and then increment the pointer to the next element of the array
    clr r25                    ; load r25:r24 with the first element in the array and then increment the pointer to the next element of the array

    cpi r24, -1                ; compare the first element to -1
    breq newline               ; if first element = -1 we jump to printing the newline

    push r27                   ; stash the pointer in the stack to avoid modification in the prinf func. (pint saves the rest of the registers we are using for us)
    push r26                   ; stash the pointer in the stack to avoid modification in the prinf func. (pint saves the rest of the registers we are using for us)
    call pint                  ; print the first int
    pop r26                    ; pop the pointer back into r27:r26
    pop r27                    ; pop the pointer back into r27:r26

    ;; loop that prints characters in the array until it sees -1
whileloop:
    ld r24, X+                 ; load the current element of the array into r25:r24 and then increment the pointer to the next element of the array
    clr r25                    ; load the current element of the array into r25:r24 and then increment the pointer to the next element of the array
    cpi r24, -1                ; compare the current element to -1
    breq newline               ; if they are equal we end the loop and jump to printing the newline
    push r27                   ; stash the pointer in the stack to avoid modification in the prinf func. (pintwspace saves the rest of the registers we are using for us)
    push r26                   ; stash the pointer in the stack to avoid modification in the prinf func. (pintwspace saves the rest of the registers we are using for us)
    call pintwspace            ; print the current int
    pop r26                    ; pop the pointer back into r27:r26
    pop r27                    ; pop the pointer back into r27:r26
    jmp whileloop              ; restart the loop
    
newline:
    ldi r24, 10                ; load r25:r24 (input registers) with 10 (ASCII value for newline)
    clr r25                    ; load r25:r24 (input registers) with 10 (ASCII value for newline)
    call putchar               ; print a newline
    ret                        ; return 

;;; Implementation that prints an integer to stdout
pint:
        ldi r22, lo8(pctd)      ; Lower byte of the format string address
        ldi r23, hi8(pctd)      ; Higher byte of the format string address
        
        push r25                ; stash the value we are printing in the stack
        push r24                ; stash the value we are printing in the stack
        push r23                ; stash the address of our format string in the stack
        push r22                ; stash the address of our format string in the stack
        call printf             ; pass everything into printf and print our int to stdout
        pop r22                 ; pop the address of our format string back inro r23:r22
        pop r23                 ; pop the address of our format string back inro r23:r22
        pop r24                 ; pop the value we are printing back into r25:r24
        pop r25                 ; pop the value we are printing back into r25:r24

        ret                     ; return 

;;; Implementation that prints a space and then an integer to stdout
pintwspace:
        ldi r22, lo8(pctdwspace)  ; Lower byte of the format string address
        ldi r23, hi8(pctdwspace)  ; Higher byte of the format string address
        
        push r25                ; stash the value we are printing in the stack
        push r24                ; stash the value we are printing in the stack
        push r23                ; stash the address of our format string in the stack
        push r22                ; stash the address of our format string in the stack
        call printf             ; pass everything into printf and print our int to stdout
        pop r22                 ; pop the address of our format string back inro r23:r22
        pop r23                 ; pop the address of our format string back inro r23:r22
        pop r24                 ; pop the value we are printing back into r25:r24
        pop r25                 ; pop the value we are printing back into r25:r24

        ret                     ; return 
