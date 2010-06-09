#include "myserial.h"

#define DRIVER_VERSION "0.1"
#define DRIVER_AUTHOR "Chris"
#define DRIVER_DESC "Super Serial Driver"

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");

static	int __init myserial_init(void)
{
  printk(KERN_INFO "Loading Driver %s version %s", DRIVER_DESC, DRIVER_VERSION);
  return (0);
}


static void	__exit myserial_exit(void)
{
  printk(KERN_INFO "Unloading Driver %s version %s", DRIVER_DESC, DRIVER_VERSION);
}

module_init(myserial_init);
module_exit(myserial_exit);


