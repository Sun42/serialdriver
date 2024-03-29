//A character-mode device-driver for the 16550 Serial UART.
//for cables without DTR<-->DSR
//-------------------------------------------------------------------
#include "myserial.h"

/**
 **
 **
 **/
irqreturn_t my_isr(int irq, void *devinfo)
{
  // static intrep = 0;
  int	interrupt_id = inb(UART_IIR) & 0x0F;
  if (interrupt_id & 1)
    return IRQ_NONE;

  // ++rep;
  // printk( "UART %d: interrupt_id=%02X \n", rep, interrupt_id );

  switch (interrupt_id)
    {
    case 6:	// Receiver Line Status (errors)
      inb(UART_LSR);
      break;
    case 4:	// Received Data Ready
    case 12:	// Character timeout
      wake_up_interruptible(&waitq_recv);
      break;
    case 2:	// Transmitter Empty
      wake_up_interruptible(&waitq_xmit);
      break;
    case 0:	// Modem Status Changed
      inb(UART_MSR);
      break;
    }
  return IRQ_HANDLED;
}

/**
 ** BTW: Un ‘interruptible’ sleep > un sleep car il peut etre reveille par un signal,
 ** au cas ou l'on voudrait `unload` le driver par exemple
 **/

int	my_proc_read(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
  int interrupt_id = inb(UART_IIR);
  int line_status = inb(UART_LSR);
  int modem_status = inb(UART_MSR);
  int len = 0;

  len += sprintf(buf + len, "\n %02X=modem_status  ", modem_status);
  len += sprintf(buf + len, "\n %02X=line_status   ", line_status);
  len += sprintf(buf + len, "\n %02X=interrupt_id  ", interrupt_id);
  len += sprintf(buf + len, "\n\n");

  return len;
}

/*
**
**
*/
static int __init	uart_init(void)
{
  printk(KERN_INFO "\nInstalling \'%s\' module\n", modname);

  //initialisation de files d'attentes emission/reception
  init_waitqueue_head(&waitq_xmit);
  init_waitqueue_head(&waitq_recv);

  /**
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    ~~ configure l' UART
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  **/
  // on ne veut pas d'interruptions pour le moment
  outb(0x00, UART_IER);

  /*
  ** Chris:  faut-il prendre en compte l'endianess?, le 14 bytes me parait louche, le reste me parait logique
  ** 0xC7 <=> 1100 0111
  ** Bit 0	[1]	=> Enable FIFO
  ** Bit 1	[1]	=> Clear receive FIFO
  ** Bit 2	[1]	=> Clear transmit FIFO
  ** Bit 3	[0]	=> DMA mode 0
  ** Bit 4	[0]	=> Reserved
  ** Bit 5	[0]	=> No, no, no i said no 64 bit
  ** Bit 6/7	[1][1]	=> interupt every 14 bytes
   */
  outb(0xC7, UART_FCR);

  /*
  ** 0x83 <=> 1000 0011
  ** Bit 0/1	[1][1]		=> data word len  8 bits (oups ca coincide pas avec le 14 bytes j'ai du fail dans l'endianess)
  ** Bit 2	[0]		=> 1 seul stop bit
  ** Bit 3/4/5	[0][0][0]	=> no parity
  ** Bit 6	[0]		=> Break signal disabled
  ** Bit 7	[1]		=> DLAB : DLM et DLL accessible- > a completer
   */
  outb(0x83, UART_LCR);

  //9600 bauds max
  outb(0x0C, UART_DLL);
  outb(0x00, UART_DLM);
  // data word len = 8 bits
  outb(0x03, UART_LCR);
  // DTR = true  RTS = true (pas certaincar ca conincide pas avec le "no dtr" du commentaire)
  outb(0x03, UART_MCR);

  // reseting 4 lsb registers qui vont servir pour la reception et transmission de data
  inb(UART_MSR);
  //
  inb(UART_LSR);
  //
  inb(UART_RX_DATA);
  //
  inb(UART_IIR);

  // demande d'irq
  if (request_irq(UART_IRQ, my_isr, IRQF_SHARED, modname, &modname) < 0)
    {
      printk(KERN_WARNING "%s couldn't get IRQ %i", modname, UART_IRQ);
      return -EBUSY;
    }
  outb(INTR_MASK, UART_IER);
  outb(0x0B, UART_MCR);
  printk(KERN_INFO " Interrupt-ID: %02X \n", inb(UART_IIR));

  create_proc_read_entry(modname, 0, NULL, my_proc_read, NULL);
  return register_chrdev(my_major, modname, &my_fops);
}


static void __exit	uart_exit(void)
{
  unregister_chrdev(my_major, modname);
  remove_proc_entry(modname, NULL);

  //
  outb(0x00, UART_IER);
  //Set to `Data terminal ready`
  outb(0x00, UART_MCR);
  free_irq(UART_IRQ, modname);

  printk(KERN_INFO "<1>Removing \'%s\' module\n", modname);
}

module_init(uart_init);
module_exit(uart_exit);
MODULE_LICENSE("GPL");

ssize_t my_read(struct file *file, char *buf, size_t len, loff_t *pos)
{
  int count, i, line_status = inb(UART_LSR);

  if ((line_status & 1) == 0)
    {
      if (file->f_flags & O_NONBLOCK)
	return 0;
      if (wait_event_interruptible(waitq_recv,(inb( UART_LSR) & 1)))
	return -EINTR;
    }

  count = 0;
  for (i = 0; i < len; i++)
    {
      unsigned char datum = inb(UART_RX_DATA);
      if (copy_to_user(buf + i, &datum, 1))
	return -EFAULT;
      ++count;
      if ((inb(UART_LSR) & 1) == 0)
	break;
    }
  return count;
}

ssize_t my_write(struct file *file, const char *buf, size_t len, loff_t *pos)
{
  int	i, count = 0;
  int   modem_status = inb(UART_MSR);

  if ((modem_status & 0x10) != 0x10)
    {
      if (file->f_flags & O_NONBLOCK)
	return 0;
      if (wait_event_interruptible(waitq_xmit, (inb(UART_MSR) & 0x10) == 0x10))
	return -EINTR;
    }

  for (i = 0; i < len; i++)
    {
      unsigned char datum;
      if (copy_from_user(&datum, buf + i, 1))
	return -EFAULT;
      while ((inb(UART_LSR) & 0x20) == 0);
      outb(datum, UART_TX_DATA);
      ++count;
      if ((inb(UART_MSR) & 0x10) != 0x10)
	break;
    }
  return count;
}

/**
 ** poll sert a multiplexer notre driver
 **
 **/
unsigned int my_poll(struct file *file, struct poll_table_struct *wait)
{
  unsigned int mask = 0;

  /**
   ** Met en file d'attente le processus courant dans toutes les files d'attentes
   ** susceptibles de le reveiller par la suite (poll_wait)
   **/
  poll_wait(file, &waitq_recv, wait);
  poll_wait(file, &waitq_xmit, wait);

  /*
  ** si read() est ready pour retourner de la data
  ** on retourne le mask correspondant
  */
  if (inb(UART_LSR) & 0x01)
    mask |= (POLLIN | POLLRDNORM);
  // si write() est ready pour accepter de la data
  if (inb(UART_LSR) & 0x20)
    mask |= (POLLOUT | POLLWRNORM);
  return mask;
}
