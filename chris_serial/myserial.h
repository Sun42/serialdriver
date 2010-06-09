#ifndef MYSERIAL_H
# define MYSERIAL_H

#include <linux/kernel.h>		// for printk and co
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/module.h>		


/* #include <linux/console.h> */
/* #include <linux/tty.h> */
/* #include <linux/tty_flip.h> */
/* #include <linux/serial.h> */
#include <linux/serial_core.h>		// high level serial API

struct uart_driver	myuart_driver = {
    .owner        = THIS_MODULE,
    .driver_name  = FOO_SERIAL_NAME,
    .dev_name     = "ttyS",
    .major        = TTY_MAJOR,
    .minor        = 64,
    .nr           = FOO_SERIAL_NR,
  };
/*
372 struct uart_driver {
373         struct module           *owner;
374         const char              *driver_name;
375         const char              *dev_name;
376         int                      major;
377         int                      minor;
378         int                      nr;
379         struct console          *cons;
381         
382           these are private; the low level driver should not
383           touch these; they should be initialised to NULL
384          
385         struct uart_state       *state;
386         struct tty_driver       *tty_driver;
387 };
 */
#endif
