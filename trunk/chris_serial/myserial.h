#ifndef MYSERIAL_H
# define MYSERIAL_H

#include <linux/kernel.h>		// for printk and co
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/module.h>		


#include <linux/console.h> 
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/serial.h> 
#include <linux/serial_core.h>		// high level serial API

# define MYSERIAL_PORT     4

#define DRIVER_VERSION "0.1"
#define DRIVER_AUTHOR "Chris"
#define DRIVER_DESC "Super Serial Driver"

static void myserial_stop_tx(struct uart_port *port);
static void myserial_start_tx(struct uart_port *port);
static void myserial_stop_rx(struct uart_port *port);
static void		myserial_enable_ms(struct uart_port *port);
static int myserial_startup(struct uart_port *port);
static void myserial_shutdown(struct uart_port *port);
static unsigned int myserial_tx_empty(struct uart_port *port);
static void myserial_set_mctrl(struct uart_port *port, unsigned int mctrl);
static unsigned int	myserial_get_mctrl(struct uart_port *port);
static void		myserial_break_ctl(struct uart_port *port, int break_state);
static void		myserial_enable_ms(struct uart_port *port) ;
static void		myserial_release_port(struct uart_port *port) ; 
static const char *myserial_type(struct uart_port *port);
static int		myserial_request_port(struct uart_port *port) ;
static void		myserial_config_port(struct uart_port *port, int flags) ;
static int		myserial_verify_port(struct uart_port *port, struct serial_struct *ser) ;
static void		myserial_set_termios(struct uart_port *port, struct ktermios *new, struct ktermios *old) ;


static struct uart_ops myserial_ops = {
  .tx_empty     = myserial_tx_empty,
  .set_mctrl    = myserial_set_mctrl,
  .get_mctrl    = myserial_get_mctrl,
  .stop_tx      = myserial_stop_tx,
  .start_tx     = myserial_start_tx,
  .stop_rx      = myserial_stop_rx,
  .enable_ms    = myserial_enable_ms,
  .break_ctl    = myserial_break_ctl,
  .startup      = myserial_startup,
  .shutdown     = myserial_shutdown,
  .type         = myserial_type,
  .release_port = myserial_release_port,
  .request_port = myserial_request_port,
  .config_port  = myserial_config_port,
  .verify_port  = myserial_verify_port,
  .set_termios  = myserial_set_termios,
};








/* struct uart_driver	myuart_driver = { */
/*     .owner        = THIS_MODULE, */
/*     .driver_name  = FOO_SERIAL_NAME, */
/*     .dev_name     = "ttyS", */
/*     .major        = TTY_MAJOR, */
/*     .minor        = 64, */
/*     .nr           = FOO_SERIAL_NR, */
/*   }; */
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
