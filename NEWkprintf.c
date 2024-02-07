/**
 *
 *kprintf is responsible for establishing serial communications between the
 *Operating System and the Hardware. Characters can be read from and input
 *to the hardware via the different helper functions, described in each function below
 *
 *@author Maximillian Marquez & Katie Knizner
 *Instructo Brylow
 *TA-BOT:MAILTO maximillian.marquez@marquette.edu
 */



#include <xinu.h>

#define UNGETMAX 10             /* Can un-get at most 10 characters. */
static unsigned char ungetArray[UNGETMAX];
unsigned int bufp = 0;

syscall kgetc()
{
    volatile struct ns16550_uart_csreg *regptr;
    regptr = (struct ns16550_uart_csreg *)UART_BASE;

    //  First, check the unget buffer for a character.
    //  Otherwise, check UART line status register, and
    //  once there is data ready, get character c.

    char c = NULL;
    if(bufp>0){  //if there is a char in the unget array
            bufp = bufp - 1; //take that character
            c = ungetArray[bufp];
    }else{
            do{
                //wait for data ready flag to be set
            }while(((regptr->lsr)&UART_LSR_DR)==0); //when the data ready flag is set
            c = regptr->rbr;} //take the char from the buffer
    return c;
    return SYSERR;
}

/**
 * kcheckc - check to see if a character is available.
 * @return true if a character is available, false otherwise.
 */
syscall kcheckc(void)
{
    volatile struct ns16550_uart_csreg *regptr;
    regptr = (struct ns16550_uart_csreg *)UART_BASE;

    // Check the unget buffer and the UART for characters.

    if(bufp>0 || regptr->lsr&UART_LSR_DR) //if there is something in the unget array or the data ready bit is set
    {
            return 1; //return true
    }else{
    return 0; //return false otherwise
    }
    return SYSERR;
}

/**
 * kungetc - put a serial character "back" into a local buffer.
 * @param c character to unget.
 * @return c on success, SYSERR on failure.
 */
syscall kungetc(unsigned char c)
{
    //Check for room in unget buffer, put the character in or discard.

    if(bufp>=UNGETMAX){ //if unget is called and the buffer is full
            return SYSERR; //return syserr
    }else{
    ungetArray[bufp] = c;//otherwise put the char in the buffer
    bufp++;
    return c;}
}

syscall kputc(uchar c)
{
    volatile struct ns16550_uart_csreg *regptr;
    regptr = (struct ns16550_uart_csreg *)UART_BASE;

    // Check UART line status register.
    // Once the Transmitter FIFO is empty, send character c.
    int val = 0;
    do{
        val = (regptr->lsr); //check if the LSR is ready for new data
    }while((val & UART_LSR_THRE)==0); //while the LSR says that it is NOT ready to take a new char
    regptr->thr = c; //put the inputted character into the buffer for the hardware
    return c;
}

syscall kprintf(const char *format, ...)
{
    int retval;
    va_list ap;

    va_start(ap, format);
    retval = _doprnt(format, ap, (int (*)(long, long))kputc, 0);
    va_end(ap);
    return retval;
}
