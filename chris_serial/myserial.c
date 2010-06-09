#include "myserial.h"

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");

static	int	__init myserial_init(void)
{
  int ret;
  printk(KERN_INFO "Loading Driver %s version %s", DRIVER_DESC, DRIVER_VERSION);


  ret = uart_register_driver(&myserial_uart_driver);
  if (ret)
    return ret;

  ret = driver_register(&myserial_device_driver);
  /* ret = platform_driver_register(/\*&atmel_serial_driver*\/); */
  if (ret)
    uart_unregister_driver(&myserial_uart_driver);
  return (ret);  
}


static void	__exit myserial_exit(void)
{
  printk(KERN_INFO "Unloading Driver %s version %s", DRIVER_DESC, DRIVER_VERSION);
}

//device_driver callbacks
int myserial_probe(struct device * dev)
{
  return (0);
}

int myserial_remove(struct device * dev)
{
  return (0);
}

int myserial_suspend(struct device *dev, pm_message_t state)
{
  return (0);
}

int myserial_resume(struct device *dev)
{
  return (0);
}

// Mandatories

/*
 * Return string describing the specified port
 */
static const char *myserial_type(struct uart_port *port)
{
  return (port->type == MYSERIAL_PORT ? "myserial_port" : NULL);
}



/*
 * Start transmitting.
 */
static void myserial_start_tx(struct uart_port *port)
{

}

/*                                                                                                                                           * Stop transmitting.                                                                                                                        */
static void myserial_stop_tx(struct uart_port *port)
{
}

/*
** Stop transmitting
*/
static void myserial_stop_rx(struct uart_port *port)
{

}

static int myserial_startup(struct uart_port *port)
{
  return 0;
}

/*
 * Disable the port
 */
static void myserial_shutdown(struct uart_port *port)
{
}

//Optionals
static unsigned int myserial_tx_empty(struct uart_port *port) 
{
  return 0;
}

static void myserial_set_mctrl(struct uart_port *port, unsigned int mctrl) 
{ 
}

static unsigned int myserial_get_mctrl(struct uart_port *port) 
{ 
  return 0;
}

static void myserial_break_ctl(struct uart_port *port, int break_state) 
{ 

}

static void myserial_enable_ms(struct uart_port *port) 
{ 
}

static void myserial_release_port(struct uart_port *port) 
{ 
}

static int myserial_request_port(struct uart_port *port) 
{ 
  return 0;
}
static void myserial_config_port(struct uart_port *port, int flags) 
{
}

static int myserial_verify_port(struct uart_port *port, struct serial_struct *ser) 
{ 
  return 0; 
}

static void myserial_set_termios(struct uart_port *port, struct ktermios *new, struct ktermios *old) 
{
}

module_init(myserial_init);
module_exit(myserial_exit);


