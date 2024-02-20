/**
 * @file     ctxsw.s
 * @provides ctxsw
 *
 * COSC 3250 / COEN 4820 Assignment 4
 * @author Max Marquez and Katie Knizner
 * Instructor Dr. Brylow
 * TA-BOT:MAILTO kathleen.knizner@marquette.edu
 * TA-BOT:MAILTO maximillian.marquez@marquette.edu
 */
/* Embedded XINU, Copyright (C) 2023.  All rights reserved. */

#include <riscv.h>

.text
        .align 4
        .globl  ctxsw


/**
 * @fn void ctxsw(&oldstack, &newstack)
 *
 * Switch context (values in registers) to another process, saving the
 * current processes information. This function will not return as normally
 * thought as it will load in the program counter for a different process and
 * jump to that location and begin executing code.
 *
 * @param  &oldstack address of outgoing stack save area
 * @param  &newstack address of incoming stack save area
 * @return special case -- see above
 */
        .func ctxsw
ctxsw:

// TODO: Save process state, including at least the 
//      callee-save ("non-volatile") registers.

//allocate storage for the stack
add sp, sp, #-64

// Save callee-save registers (s0-s11) and the return address (lr)
    str s11, [sp, #CTX_S11*4]
    str s10, [sp, #CTX_S10*4]
    str s9, [sp, #CTX_S9*4]
    str s8, [sp, #CTX_S8*4]
    str s7, [sp, #CTX_S7*4]
    str s6, [sp, #CTX_S6*4]
    str s5, [sp, #CTX_S5*4]
    str s4, [sp, #CTX_S4*4]
    str lr, [sp, #CTX_RA*4] // CTX_RA for saving the return address

    // Save the current stack pointer (sp) in s0
    str sp, [s0]

// TODO: Restore process state, including at least the
//      callee-save ("non-volatile") registers.
//      These call and restore segments must match the register data
//      layout you choose in create().

 // load the new stack pointer (sp) from the new stack save area
    ldr sp, [s1]

    // Restore callee-save registers (s0-s11) and the return address (lr)
    ldr s0, [sp, #CTX_S0*4]
    ldr s1, [sp, #CTX_S1*4]
    ldr s2, [sp, #CTX_S2*4]
    ldr s3, [sp, #CTX_S3*4]
    ldr s4, [sp, #CTX_S4*4]
    ldr s5, [sp, #CTX_S5*4]
    ldr s6, [sp, #CTX_S6*4]
    ldr s7, [sp, #CTX_S7*4]
    ldr s8, [sp, #CTX_S8*4]
    ldr s9, [sp, #CTX_S9*4]
    ldr s10, [sp, #CTX_S10*4]
    ldr s11, [sp, #CTX_S11*4]
    ldr lr, [sp, #CTX_RA*4] 

// deallocate space for saved registers on the stack
    add sp, sp, #64

// TODO: Jump to next function.
//      Note: there are two important values at play here --
//      the address of the process's function that we want to end up in
//      once we leave this ctxsw(), and the value we want to have in the
//      return address register.  Those are almost always the same value, except
//      for the very first time we ctxsw() to a new process, when they
//      are different.  See create() for more details.

bx lr
// Note: This ret line immediately returns from whence we came.  This
//      is here as a placeholder so that you don't crash your O/S when
//      you call ctxsw() before it is complete.  But, this line of code
//      is not actually a part of a completed solution.
        ret

// ASSIGNMENT 5 TODO:
// Note: the following code is for the next assignment.  DO NOT uncomment it until you believe your trap handler is working in assignment
// Determine which mode we privilege mode the process should be in
//      ld t0, CTX_PC*8(**REGISTER THAT HOLDS PROCESS CONTEXT**)
//      beq t0, ra, switch
//      li t4, SSTATUS_S_MODE
//      csrc sstatus, t4
//      csrw sepc, t0
//
//      sret
//switch:
//      ret

        .end ctxsw
