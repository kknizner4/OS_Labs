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
    
    ppcb -> state = PRSUSP;
    ppcb -> stkbase = saddr;
    ppcb -> stklen = (ulong *)(saddr - ssize*4); //just setting base size 
    ppcb -> stkptr = ppcb -> stkbase + ppcb-> stklen;
    
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
    *--saddr = funcaddr;
    *--saddr = &userret;

      int arg1 = 0;
      int arg2 = 0;
      int arg3 = 0;
      int arg4 = 0;
      if (nargs >= 1)
        arg1 = va_arg(ap,int);
      if (nargs >= 2)
        arg2 = va_arg(ap,int);
      if (nargs >= 3)
        arg3 = va_arg(ap,int);
      if (nargs >= 4)
        arg4 = va_arg(ap,int);

// fill spots for registers with 0s or first 4 arguments appropriately
    for (i = 0; i < 14; i++ ){
        if ( i == 13 ) {
            *--saddr = arg1;
        } else if ( i == 12) {
            *--saddr = arg2;
        } else if ( i == 11) {
            *--saddr = arg3;
        } else if ( i == 10) {
            *--saddr = arg4;
        } else {
            *--saddr = 0;
        }
    }
        // TODO:  Place arguments into activation record.
        //        See K&R 7.3 for example using va_start, va_arg and
        //        va_end macros for variable argument functions.

   for (i = 4; i < nargs; i++)
        argumentpointer[i-4] = va_arg(ap, int);
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
