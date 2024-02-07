/**
 * @file testcases.c
 * @provides testcases
 *
 * Modified by:
 *Max Marquez
 * and
 *Katie Knizner
 */
/* Embedded XINU, Copyright (C) 2023.  All rights reserved. */

#include <xinu.h>
#define UNGETMAX 10
/**
 * testcases - called after initialization completes to test things.
 */

void testcases(void)
{
    int testFlag = 0;
    int c;
    unsigned char testChar;
    kprintf("===TEST BEGIN===\r\n");

    do{     //menu
        kprintf("\n\rernter 'p' to test putc\r\n");
        kprintf("enter 'g' to test getc\r\n");
        kprintf("enter 'c' to test checkc\r\n");
        kprintf("enter 'u' to test ungetc\r\n");
        kprintf("enter 'r' to ungetc past the buffer limit\r\n");
        kprintf("enter 'e' to end testing\r\n");
        kprintf("enter any other character to print 'Hello Xinu World!'\r\n");

        c = kgetc(); //get user input for menu
        switch (c)
                {
                case 'p': //putc
                        kprintf("Enter a character you want to put\r\n");
                        testChar = kgetc(); //get user input
                        testChar = kputc(testChar);//putchar that character, succesful call causes putchar to return the inputted char
                        kprintf("The put character was %c\r\n",testChar);
                        break;

                case 'g': //getc
