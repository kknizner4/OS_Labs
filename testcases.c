/**
 * @file testcases.c
 * @provides testcases
 *
 * Modified by: Max Marquez and Katie Knizner
 */
/* Embedded XINU, Copyright (C) 2023.  All rights reserved. */

#include <xinu.h>

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
    int c;

    kprintf("===TEST BEGIN===\r\n");

    c = kgetc();
    switch (c)
    {
    case -1:
        kprintf("EOF encountered.\r\n");
        break;

    case '\n':
        kprintf("Newline character entered.\r\n");
        break;

    default:
        if (kcheckc())
        {
            kprintf("A character is available in the input.\r\n");
        }
        else
        {
            kprintf("No character is available in the input.\r\n");
        }

        // Test kungetc
        kprintf("Enter a character to unget: ");
        c = kgetc();
        kungetc(c);
        kprintf("Unget buffer now has: %c\r\n", ungetArray[0]);

        kprintf("Hello Xinu World!\r\n");
        break;
    }

    kprintf("\r\n===TEST END===\r\n");
    return;
}
