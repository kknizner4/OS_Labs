/**
 * @file create.c
 * @provides create, newpid, userret
 *
 * COSC 3250 Assignment 4
 * @author Max Marquez and Katie Knizner
 * Instructor Dr. Brylow
 * TA-BOT:MAILTO kathleen.knizner@marquette.edu
 * TA-BOT:MAILTO maximillian.marquez@marquette.edu
 */
 */
/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

static pid_typ newpid(void);
void userret(void);
void *getstk(ulong);

/**
 * Create a new process to start running a function.
 * @param funcaddr address of function that will begin in new process
 * @param ssize    stack size in bytes
 * @param name     name of the process, used for debugging
 * @param nargs    number of arguments that follow
 * @return the new process id
 */
syscall create(void *funcaddr, ulong ssize, char *name, ulong nargs, ...)
{
    ulong *saddr;               /* stack address                */
    ulong pid;                  /* stores new process id        */
    pcb *ppcb;                  /* pointer to proc control blk  */
    ulong i;
    va_list ap;                 /* points to list of var args   */
    ulong pads = 0;             /* padding entries in record.   */

    if (ssize < MINSTK)
        ssize = MINSTK;

    ssize = (ulong)((((ulong)(ssize + 3)) >> 2) << 2);
    /* round up to even boundary    */
    saddr = (ulong *)getstk(ssize);     /* allocate new stack and pid   */
    pid = newpid();
    /* a little error checking      */
    if ((((ulong *)SYSERR) == saddr) || (SYSERR == pid))
    {
        return SYSERR;
    }

    numproc++;
    ppcb = &proctab[pid];
       // TODO: Setup PCB entry for new process.
   strncpy(ppcb->name, name, PNMLEN);
    ppcb->state = PRSUSP;
    ppcb->stkbase = saddr;
    ppcb->stklen = ssize;
    ppcb->stkptr = saddr + ssize / sizeof(ulong); 
    
    
    /* Initialize stack with accounting block. */
    *saddr = STACKMAGIC;
    *--saddr = pid;
    *--saddr = ppcb->stklen;
    *--saddr = (ulong)ppcb->stkbase;

    /* Handle variable number of arguments passed to starting function   */
    if (nargs)
    {
        pads = ((nargs - 1) / 8) * 8;
    }
    /* If more than 4 args, pad record size to multiple of native memory */
    /*  transfer size.  Reserve space for extra args                     */
    for (i = 0; i < pads; i++)
    {
        *--saddr = 0;
    }

        // TODO: Initialize process context.
        //
    // set the initial program counter (PC) to the function address
    *--saddr = (ulong)funcaddr;
    // set the return address to the userret function
    *--saddr = (ulong)&userret;
    // place arguments into the activation record and initialize va_list for variable arguments
    va_list ap;
    va_start(ap, nargs);

    // loop through arguments and push them onto the stack
    for (i = 0; i < nargs && i < 4; i++) {
    *--saddr = va_arg(ap, ulong);
    }

    //finish handling arguments
    va_end(ap);

    // fill the remaining slots with zeros if necessary
    for (i = 0; i < pads; i++) {
        *--saddr = 0;
    }

    // fill spots for registers with 0s or first 4 arguments appropriately
    for (i = 0; i < pads; i++)
    {
        *--saddr = 0;
    }

   
        // TODO:  Place arguments into activation record.
        //        See K&R 7.3 for example using va_start, va_arg and
        //        va_end macros for variable argument functions.

  for (i = 0; i < nargs && i < 4; i++) {
        *--saddr = va_arg(ap, ulong);
    }

    va_end(ap);
    ppcb->stkptr = saddr;
    
    return pid;
}

/**
 * Obtain a new (free) process id.
 * @return a free process id, SYSERR if all ids are used
 */
static pid_typ newpid(void)
{
    pid_typ pid;                /* process id to return     */
    static pid_typ nextpid = 0;

    for (pid = 0; pid < NPROC; pid++)
    {                           /* check all NPROC slots    */
        nextpid = (nextpid + 1) % NPROC;
        if (PRFREE == proctab[nextpid].state)
        {
            return nextpid;
        }
return SYSERR;
}

/**
 * Entered when a process exits by return.
 */
void userret(void)
{
    // ASSIGNMENT 5 TODO: Replace the call to kill(); with user_kill();
    // when you believe your trap handler is working in Assignment 5
    // user_kill();
    kill(currpid);
}
