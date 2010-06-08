#ifndef MYSERIAL_H
# define MYSERIAL_H

#include <linux/module.h>	// for init_module()
#include <linux/proc_fs.h>	// for create_proc_read_entry()
#include <linux/interrupt.h>	// for request_irq()
#include <linux/poll.h>		// for poll_wait()
#include <asm/uaccess.h>	// for copy_to/from_user()
#include <asm/io.h>		// for inb(), outb()
#include <linux/sched.h>	//for TASK_INTERUPTIBLE macro

ssize_t		my_read(struct file *file, char *buf, size_t len, loff_t *pos);
ssize_t		my_write(struct file *file, const char *buf, size_t len, loff_t *pos);
unsigned int	my_poll(struct file *file, struct poll_table_struct *wait);


irqreturn_t	my_isr(int irq, void *devinfo);
int		my_proc_read(char *buf, char **start, off_t off, int count, int *eof, void *data);

#define UART_IRQ	4	// UART's Interrupt Request pin
#define UART_BASE	0x03F8	// UART's base I/O port-address
#define INTR_MASK	0x0F	// UART Interrupt Mask

enum {
  UART_RX_DATA = UART_BASE + 0,		//
  UART_TX_DATA = UART_BASE + 0,		//
  UART_DLATCH_LO = UART_BASE + 0,	// DLL divisor latch lsb
  UART_DLATCH_HI = UART_BASE + 1,	// DLM divisor latch msb
  UART_IER = UART_BASE + 1,		// Interrupt enable register
  UART_IIR = UART_BASE + 2,		// Interrupt identification register
  UART_FCR = UART_BASE + 2,		// Fifo Control Register
  UART_LCR = UART_BASE + 3,		// Line Control Register
  UART_MCR = UART_BASE + 4,		// Modem Control  Register
  UART_LSR = UART_BASE + 5,		// line status Register
  UART_MSR = UART_BASE + 6,		// Modem Status Register
  UART_SCR = UART_BASE + 7,		// Scratch
};

char modname[] = "myserial";
int  my_major = 84;
wait_queue_head_t  waitq_recv;
wait_queue_head_t  waitq_xmit;

struct file_operations my_fops = {
 owner:THIS_MODULE,
 write:my_write,
 read:my_read,
 poll:my_poll,
};

#endif // !! MYSERIAL_H
