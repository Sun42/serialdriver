#ifndef MYSERIAL_H
# define MYSERIAL_H

#include <linux/kernel.h>		// for printk and co
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/module.h>		
#include <linux/device.h>



#include <linux/console.h> 
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/serial.h> 
#include <linux/serial_core.h>		// high level serial API

# define MYSERIAL_PORT     4
# define DRIVER_VERSION "0.1"
# define DRIVER_AUTHOR "Chris"
# define DRIVER_DESC "Super Serial Driver"
# define MYSERIAL_NAME "myserial"
# define MYSERIAL_NR	1


static void		myserial_stop_tx(struct uart_port *port);
static void		myserial_start_tx(struct uart_port *port);
static void		myserial_stop_rx(struct uart_port *port);
static void		myserial_enable_ms(struct uart_port *port);
static int		myserial_startup(struct uart_port *port);
static void		myserial_shutdown(struct uart_port *port);
static unsigned int	myserial_tx_empty(struct uart_port *port);
static void		myserial_set_mctrl(struct uart_port *port, unsigned int mctrl);
static unsigned int	myserial_get_mctrl(struct uart_port *port);
static void		myserial_break_ctl(struct uart_port *port, int break_state);
static void		myserial_enable_ms(struct uart_port *port) ;
static void		myserial_release_port(struct uart_port *port) ; 
static const char	*myserial_type(struct uart_port *port);
static int		myserial_request_port(struct uart_port *port) ;
static void		myserial_config_port(struct uart_port *port, int flags) ;
static int		myserial_verify_port(struct uart_port *port, struct serial_struct *ser) ;
static void		myserial_set_termios(struct uart_port *port, struct ktermios *new, struct ktermios *old) ;


int			myserial_probe(struct device * dev);
int			myserial_remove(struct device * dev);
int			myserial_suspend(struct device *dev, pm_message_t state);
int			myserial_resume(struct device *dev);


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

static struct uart_driver	myserial_uart_driver = {
    .owner        = THIS_MODULE,
    .driver_name  = MYSERIAL_NAME,
    .dev_name     = "ttyS",
    .major        = TTY_MAJOR,
    .minor        = 64,
    .nr           = MYSERIAL_NR,	// The nr field specifies the maximum number of serial ports this driver supports.
  };


static struct device_driver myserial_device_driver = {
 name: MYSERIAL_NAME,  //fixme ? 
 bus: NULL, //fixme
 owner: THIS_MODULE,
 probe: myserial_probe,
 remove: myserial_remove,
 suspend: myserial_suspend,
 resume:  myserial_resume, 
};

/*
struct device_driver {
        const char              *name;
        struct bus_type         *bus;

        struct module           *owner;
        const char              *mod_name;       used for built-in modules 

bool suppress_bind_attrs;        disables bind/unbind via sysfs 
int (*probe) (struct device *dev);
int (*remove) (struct device *dev);
void (*shutdown) (struct device *dev);
int (*suspend) (struct device *dev, pm_message_t state);
int (*resume) (struct device *dev);
const struct attribute_group **groups;

const struct dev_pm_ops *pm;

struct driver_private *p;
};
 */

//~/Documents/linux-2.6.33.4/Documentation/serial/driver
//~/Documents/linux-2.6.33.4/include/linux/serial_core.h
//~/Documents/linux-2.6.33.4/drivers/serial/atmel_serial.c
//~/Documents/linux-2.6.33.4/drivers/serial/amba-pl011.c
//~/Documents/linux-2.6.33.4/Documentation/driver-model/device.txt
// ~/Documents/linux-2.6.33.4/include/linux/device.h
//~/Downloads/serial-drivers.odp
//http://www.makelinux.net/ldd3
//http://www.linux-mips.org/wiki/Serial_Driver_and_Console
//http://www.kernel.org/pub/linux/kernel/people/mochel/doc/text/driver.txt

#endif
