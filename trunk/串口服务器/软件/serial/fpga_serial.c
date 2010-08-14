/*======================================================================
    led_driver.c
======================================================================*/
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <asm/arch-at91rm9200/at91sam9260.h>
#include <linux/interrupt.h>
//#include <asm/arch/at91_pio.h>
//#include <asm-alpha/termbits.h>
#include <linux/ioport.h>
//#include <asm-alpha/ioctls.h>
#include <linux/tty.h>
#include <asm/arch/at91sam926x_mc.h>
#include <asm/arch/at91sam9260.h>
#include <asm/arch/gpio.h>

//typedef	unsigned char u8;

MODULE_AUTHOR("xul Gxuliang@gmail.com");
MODULE_LICENSE("GPL");

#define __DEBUG__

#ifdef __DEBUG__
#define DBG(fmt,args...) printk("serial: "fmt,##args)
#else
#define DBG(fmt,args...)
#endif

#define	DEVICE_NAME	"serial_ext_regs"
#define	SERIAL_MAJOR 100
#define	MAX_PORT	16
#define	FPGA_BASE_ADDR	0x30000000
#define	FPGA_MAP_SIZE	0x10000000
#define	F_READ	0x01
#define	F_WRITE	0x02
/*read and write reg*/
#define	UART_GET_CHAR(p)	(u8)readb(p + fpga_mapped_addr)
#define	UART_PUT_CHAR(p,c)	writeb(c, p+ fpga_mapped_addr)
/*read and write state reg*/
#define	UART_GET_RSR(p)		(u8)readb(p + (p < 8 ? 0x66 : 0x6D) + fpga_mapped_addr)
#define	UART_PUT_RSR(p)		(u8)readb(p + (p < 8 ? 0x67 : 0x6E) + fpga_mapped_addr)
/*read and write irg reg*/
#define	UART_GET_RST(p)		(u8)readb((p < 8 ? 0x30 : 0x68) + fpga_mapped_addr)
#define	UART_GET_WST(p)		(u8)readb((p < 8 ? 0x33 : 0x6B) + fpga_mapped_addr)


#define	UART_GET_RIRQ(p)	(u8)(readb((p < 8 ? 0x31 : 0x69) + fpga_mapped_addr))
#define	UART_ENB_RIRQ(p)	writeb((1 << (p%8)) | UART_GET_RIRQ(p),  (p < 8 ? 0x31 : 0x69) + fpga_mapped_addr)
#define	UART_DIB_RIRQ(p)	writeb(~(1 << (p%8)) & UART_GET_RIRQ(p), (p < 8 ? 0x31 : 0x69) + fpga_mapped_addr)
#define	UART_GET_WIRQ(p)	(u8)readb((p < 8 ? 0x32 : 0x69) + fpga_mapped_addr)
#define	UART_ENB_WIRQ(p)	writeb((1 << (p%8)) | UART_GET_WIRQ(p), (p < 8 ? 0x32 : 0x69) + fpga_mapped_addr)
#define	UART_DIB_WIRQ(p)	writeb(~(1 << (p%8)) & UART_GET_WIRQ(p), (p < 8 ? 0x32 : 0x6A) + fpga_mapped_addr)
/*FIFO reset reg*/

/*baud rates*/
#define	UART_GET_BAUD(p)	(u8)readb(p + 0x20 + fpga_mapped_addr)
#define	UART_SET_BAUD(p,c)	writeb(c, p + 0x20 + fpga_mapped_addr)
/*data bit */
#define	UART_GET_DBIT(p)	(u8)(readb(p + 0x10 + fpga_mapped_addr) & 0x03)
#define	UART_SET_DBIT(p,c)	writeb((c & 0x03) | (UART_GET_DBIT(p) & 0xfc), p + 0x10 + fpga_mapped_addr)
/*check bit*/
#define	UART_GET_CBIT(p)	(u8)((readb(p + 0x10 + fpga_mapped_addr) >> 4) & 0x03)
#define	UART_SET_CBIT(p,c)	writeb(((c & 0x03) << 4) | (UART_GET_CBIT(p) & 0x3f), p + 0x10 + fpga_mapped_addr)
/*stop bit*/
#define	UART_GET_SBIT(p)	(u8)((readb(p + 0x10 + fpga_mapped_addr) >> 6) & 0x01)
#define	UART_SET_SBIT(p,c)	writeb(((c & 0x01) << 6) | (UART_GET_SBIT(p) & 0x4f), p + 0x10 + fpga_mapped_addr)

struct baud_rates {
	unsigned int rate;
	unsigned int cflag;
};
static const struct baud_rates baud_rates[] = {
	{ 115200, B115200 },
	{  57600, B57600  },
	{  38400, B38400  },
	{  19200, B19200  },
	{   9600, B9600   },
	{   4800, B4800   },
	{   2400, B2400   },
	{   1200, B1200   },
	{      0, B38400  }
};

static const u8 fpga_baud[] = {
	119,
	339,
	559,
	11,
	23,
	47,
	95,
	191,
	0
};


static void * fpga_mapped_addr;
#define	MAX_BUF_LEN		1024
struct serial_dev
{
	struct cdev cdev;
	int index;
//	void __iomem *p;
//	unsigned char value;
	struct ktermios termios;
	char send_buf[MAX_BUF_LEN];
	char recv_buf[MAX_BUF_LEN];
	unsigned int send_rd_point;
	unsigned int send_wr_point;
	u8 send_full_flag;
	unsigned int recv_rd_point;
	unsigned int recv_wr_point;
	u8 recv_full_flag;
	
	struct semaphore sem; /*并发控制用的信号量*/           
	wait_queue_head_t r_wait; /*阻塞读用的等待队列头*/     
	wait_queue_head_t w_wait; /*阻塞写用的等待队列头*/     
};

struct serial_dev *serial_devp;
int serial_major = SERIAL_MAJOR;




int get_char(char *buf, int i, int len)
{
	int j = 0;
	UART_DIB_RIRQ(i);
	while(UART_GET_RSR(i))
	{
		buf[j++] = UART_GET_CHAR(i);
		if(j == len)
			break;
	}
	UART_ENB_RIRQ(i);
	return j;
}

static void get_buf(struct serial_dev *devp)
{
	UART_DIB_RIRQ(devp->index);//disable recvive irq
	while(devp->recv_full_flag == 0 && UART_GET_RSR(devp->index))
	{
		devp->recv_buf[devp->recv_wr_point++] = UART_GET_CHAR(devp->index);
		if(devp->recv_wr_point >= MAX_BUF_LEN)
			devp->recv_wr_point = 0;
		if(devp->recv_wr_point == devp->recv_rd_point)
			devp->recv_full_flag = 1;			
	}
	UART_ENB_RIRQ(devp->index);
	return ;
}
static void get_recv_irg(void);
DECLARE_TASKLET(recv_tasklet, get_recv_irg, 0);
/*当接收中断来时调用此函数*/
static void get_recv_irg(void)
{
	int i;
	for(i = 0; i < MAX_PORT; i++)
	{
		if(UART_GET_RST(i) & (i%8))
		{
			get_buf(&serial_devp[i]);
		}
	}
}
irqreturn_t recv_interrupt(void)
{
	tasklet_schedule(&recv_tasklet);
}
static int recvbuf_to_userbuf(struct serial_dev *devp, char* buf, int len)
{
	int i = 0;
	char *p = kmalloc(len, GFP_KERNEL);
	while(devp->recv_rd_point != devp->recv_wr_point)
	{
		p[i++] = devp->recv_buf[devp->recv_rd_point++];
		devp->recv_full_flag = 0;
		if(devp->recv_rd_point >= MAX_BUF_LEN)
			devp->recv_rd_point = 0;
		if(i == len)
			break;
	}
	copy_to_user(buf, p, i);
	kfree(p);
	return i;
}

static void send_buf(struct serial_dev *devp)
{
	UART_DIB_WIRQ(devp->index);//disable recvive irq
	while(devp->send_rd_point != devp->send_wr_point)
	{
		if(!UART_PUT_RSR(devp->index))	
		{
			UART_PUT_CHAR(devp->index, devp->send_buf[devp->send_rd_point++]);
			if(devp->send_rd_point >= MAX_BUF_LEN)
			{
				devp->send_rd_point = 0;
			}
			devp->send_full_flag = 0;
		}
		else
		{
			break;
		}
	}

	if(devp->send_rd_point == devp->send_wr_point)
	{
	//	UART_DIB_WIRQ(devp->index);
		return;
	}
	UART_ENB_WIRQ(devp->index);
}
static void get_send_irg(void);
DECLARE_TASKLET(send_tasklet, get_send_irg, 0);
static void get_send_irg(void)
{
	int i;
	for(i = 0; i < MAX_PORT; i++)
	{
		if(UART_GET_WST(i) & (i%8))
		{
			send_buf(&serial_devp[i]);
		}
	}
}
irqreturn_t send_interrupt(void)
{
	tasklet_schedule(&send_tasklet);
}

static int userbuf_to_sendbuf(struct serial_dev *devp, char *buf, int len)
{
	int i = 0;
	char *p = NULL;
	if(devp->send_full_flag == 1)
		return 0;//it should be bolck

	p = kmalloc(len , GFP_KERNEL);
	copy_from_user(p, buf, len);
	while(devp->send_full_flag == 0 && i < len)
	{	
		devp->send_buf[devp->send_wr_point++] = p[i++];
		if(devp->send_wr_point >= MAX_BUF_LEN)
			devp->send_wr_point = 0;
		if(devp->send_wr_point == devp->send_rd_point)
			devp->send_full_flag = 1;
	}
	kfree(p);
	UART_ENB_WIRQ(devp->index);
	return i;
}
#if 0
static int recvbuf_to_userbuf(struct serial_dev *devp, char* buf, int len)
{
	int ret = 0, left = len;
	while(left && devp->recv_rd_point != devp->recv_wr_point)
	{
		if(devp->recv_rd_point < devp->recv_wr_point)
		{
			if(devp->recv_wr_point - devp->recv_rd_point >= left)
			{
				copy_to_user(buf + ret, devp->recv_buf[devp->recv_rd_point], left);
				recv_rd_point += left;
				ret += left;
				left = 0;
				break;
			}
			else
			{
				copy_to_user(buf + ret, devp->recv_buf[devp->recv_rd_point], devp->recv_wr_point - devp->recv_rd_point);
				recv_rd_point = recv_wr_point;
				ret += (devp->recv_wr_point - devp->recv_rd_point);
				break;
			}
		}
			else
			{
				if((MAX_BUF_LEN - recv_rd_point) > left)
				{
					copy_to_user(buf + ret, devp->recv_buf[devp->recv_rd_point], left);
					recv_rd_point += left;
					ret += left;
					left = 0;
					break;
				}
				else
				{
					copy_to_user(buf + ret, devp->recv_buf[devp->recv_rd_point], MAX_BUF_LEN - recv_rd_point - 1);
					recv_rd_point = 0;
					ret += MAX_BUF_LEN - recv_rd_point - 1;
					left -= (MAX_BUF_LEN - recv_rd_point - 1);
				}

			}
		}
	}

	return ret;

}
#endif
unsigned int get_baud_rates(int i)
{
	u8 baud;
	int j;
	baud = UART_GET_BAUD(i);
	for(j = 0; fpga_baud[j]; j++)
	{
		if(baud == fpga_baud[j])
			break;
	}

	return j;
	/*
	if(fpga_baud[j] == 0)
		return 0;
	return baud_rates[j].rate;
	*/
}

int set_baud_rates(int i, u16 x)
{
	int j;
	for(j = 0; baud_rates[j].rate; j++)
	{
		if(baud_rates[j].cflag == x)
			break;
	}
	if(baud_rates[j].rate == 0)
		return -1;
	UART_SET_BAUD(i, fpga_baud[j]);
	return 0;
}


/*文件打开函数*/
int serial_open(struct inode *inode, struct file *filp)
{
  /*将设备结构体指针赋值给文件私有数据指针*/
  struct serial_dev *dev;
  
  dev = container_of(inode->i_cdev,struct serial_dev,cdev); 
  printk(KERN_ALERT "1open.dev is %p\n",dev);
  //printk(KERN_ALERT "open.dev->p is %p\n",dev->p);
  filp->private_data = dev;  
  return 0;
}

/*文件释放函数*/
int serial_release(struct inode *inode, struct file *filp)
{
	struct serial_dev *dev = filp->private_data;/*获得设备结构体指针*/

  return 0;
}

/*写函数*/
static ssize_t serial_write(struct file *filp, const char __user *buf,
  size_t size, loff_t *ppos)
{
  unsigned long p =  *ppos;
  unsigned int count = size;
  int ret = 0;
  struct serial_dev *dev = filp->private_data; /*获得设备结构体指针*/
  printk(KERN_ALERT "write: lsbuf is [%s], size = %d\n", buf, size);
//  userbuf_to_sendbuf(dev, buf, size);
//   printk(KERN_ALERT "write.dev is %p,i=%d\n",dev,dev->index);  
  /*用户空间->内核空间*/
//  if (copy_from_user(&(dev->value) , buf, 1))
  //  ret =  - EFAULT;

  return ret;
}

/*写函数*/
static ssize_t serial_read(struct file *filp, const char __user *buf,
  size_t size, loff_t *ppos)
{
  unsigned long p =  *ppos;
  unsigned int count = size;
  int ret = 0;
  struct serial_dev *dev = filp->private_data; /*获得设备结构体指针*/
  
  DBG("read\n");
  DECLARE_WAITQUEUE(wait, current); //定义等待队列

  down(&dev->sem); //获得信号量
  add_wait_queue(&dev->r_wait, &wait); //进入读等待队列头
  __set_current_state(TASK_INTERRUPTIBLE); //改变进程状态为睡眠
  up(&dev->sem);

  schedule(); //调度其他进程执行
  down(&dev->sem);

  recvbuf_to_userbuf(dev, buf, size);

  up(&dev->sem); //释放信号量
  remove_wait_queue(&dev->w_wait, &wait); //从附属的等待队列头移除
  set_current_state(TASK_RUNNING);
  /*用户空间->内核空间*/
//  if (copy_to_user(buf, (void*)&(dev->value), 1))
  //  ret =  - EFAULT;
//  else
  {
    
  //  printk(KERN_INFO "read %d bytes(s) from %p\n", count, p);
  }

  return 1;
}

int get_attr(struct termios __user * u_termio, struct ktermios *a_termios, int index)
{
	struct ktermios *k_termios = (a_termios);
	int ret = -1, i;
//	k_termio.c_cflag = B50;
//	printk("k_termio.c_cflag = 0x%x\n", k_termios->c_cflag);
	i = get_baud_rates(index);
	k_termios->c_cflag = baud_rates[i].cflag;
	ret = copy_to_user(u_termio, k_termios, sizeof(struct ktermios));
//	printk("u_termio->c_cflag = 0x%x\n", (struct termios __user *)u_termio->c_cflag);
//	printk("ret1 = %d\n", ret);
	return 0;
}

int set_attr(struct termios __user * u_termio, struct ktermios *a_termios, int index)
{
	struct ktermios *k_termios = (a_termios);
	int ret = -1;
//	k_termio.c_cflag = B50;
//	printk("k_termio.c_cflag = 0x%x\n", k_termios->c_cflag);
	ret = copy_from_user(k_termios, u_termio, sizeof(struct ktermios));
	set_baud_rates(index, k_termios->c_cflag & 0x3f);
//	printk("u_termio->c_cflag = 0x%x\n", (struct termios __user *)u_termio->c_cflag);
//	printk("ret1 = %d\n", ret);
	return 0;
}
static int serial_ioctl(struct inode *tty, struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret = -ENOIOCTLCMD;
	void __user *p = (void __user *)arg;	
	printk("cmd = 0x%04x\n", cmd);
	char *pp = (char *)arg;
	struct serial_dev *dev = filp->private_data; /*获得设备结构体指针*/
//	dev->termios.c_cflag = 0x03;
//	printk("cflag = 0x%x\n", dev->termios.c_cflag);
	switch(cmd)
	{
	case TCGETS:
		get_attr((struct termios __user *)arg, &dev->termios, dev->index);
		return 0;
	case TCFLSH:
		break;
	case TCSETS:
		set_attr((struct termios __user *)arg, &dev->termios, dev->index);
		break;
	case F_READ:
		pp[1] = (unsigned char) readb(fpga_mapped_addr + pp[0]);
		DBG("read: %d, %d\n", pp[0], pp[1]);
		break;
	case F_WRITE:
		writeb(pp[1], fpga_mapped_addr + pp[0]);
		DBG("write: %d, %d\n", pp[0], pp[1]);
		break;
	default:
		break;
	}
	return ret;
}

/*文件操作结构体*/
static const struct file_operations serial_fops =
{
  .owner = THIS_MODULE,
  .read = serial_read,
  .write = serial_write,
  .open = serial_open,
  .release = serial_release,
  .ioctl = serial_ioctl,
};


/*初始化并注册cdev*/
static void serial_setup_cdev(struct serial_dev *dev, int index)
{
  int err, devno = MKDEV(serial_major, index);
  printk(KERN_ALERT "setup.dev is %p\n",dev);
  cdev_init(&dev->cdev, &serial_fops);
  dev->cdev.owner = THIS_MODULE;
  dev->cdev.ops = &serial_fops;
  dev->index = index;
  err = cdev_add(&dev->cdev, devno, 1);
  if (err)
    printk(KERN_NOTICE "Error %d adding ttyFPGA%d", err, index);
}

int serial_init(void)
{
	int i, result;
	printk(KERN_ALERT "serial init\n");

	at91_set_A_periph(AT91_PIN_PC11, 1);//CS2
	at91_sys_write(AT91_SMC_CYCLE(2), AT91_SMC_NWECYCLE_(24) |  AT91_SMC_NRDCYCLE_(24));
	at91_sys_write(AT91_SMC_SETUP(2), AT91_SMC_NWESETUP_(4) |  AT91_SMC_NCS_WRSETUP_(3) |  AT91_SMC_NRDSETUP_(4) |  AT91_SMC_NCS_RDSETUP_(3));
	at91_sys_write(AT91_SMC_PULSE(2), AT91_SMC_NWEPULSE_(16) |  AT91_SMC_NRDPULSE_(16) | AT91_SMC_NCS_WRPULSE_(18) |  AT91_SMC_NCS_RDPULSE_(18));
	at91_sys_write(AT91_SMC_MODE(2), AT91_SMC_READMODE | AT91_SMC_WRITEMODE | AT91_SMC_EXNWMODE_DISABLE | AT91_SMC_DBW_8 |/* AT91_SMC_TDFMODE |*/ AT91_SMC_TDF_(1));
#if 1 
	DBG("SMC_MODE(2): 0x%08x   AT91_SMC_PULSE(2): 0x%08x\n",
			at91_sys_read(AT91_SMC_MODE(2)),  at91_sys_read(AT91_SMC_PULSE(2)));
	DBG("SMC_SETUP(2): 0x%08x	SMC_CYCLE[2]: 0x%08x\n",  at91_sys_read(AT91_SMC_SETUP(2)),  at91_sys_read(AT91_SMC_CYCLE(2)));
#endif 

	request_mem_region(FPGA_BASE_ADDR, FPGA_MAP_SIZE, "serial_fpga_mem");
	fpga_mapped_addr = (void *)ioremap(FPGA_BASE_ADDR, FPGA_MAP_SIZE);
	if(fpga_mapped_addr == NULL)
	{
		printk("fpga_serial: fpga mem ioremap failed!\n");
		return 0;
	}
	dev_t devno = MKDEV(serial_major, 0);
	
	/* 申请设备号*/
  if (serial_major)
    result = register_chrdev_region(devno, MAX_PORT, "ttyFPGA");
  else  /* 动态申请设备号 */
  {
    result = alloc_chrdev_region(&devno, 0, MAX_PORT, "ttyFPGA");
    serial_major = MAJOR(devno);
  }  
  if (result < 0)
  {
	  printk("fpga_serial:register_chrdev failed!\n");
	  iounmap(fpga_mapped_addr);
	  return result;
  }

  result = request_irq(AT91SAM9260_ID_IRQ0, recv_interrupt, SA_SHIRQ, "FPGA_uart", NULL);
  result = request_irq(AT91SAM9260_ID_IRQ1, send_interrupt, SA_SHIRQ, "FPGA_uart", NULL);

	/* 动态申请2个设备结构体的内存*/
  serial_devp = kmalloc(MAX_PORT*sizeof(struct serial_dev), GFP_KERNEL);
  if (!serial_devp)    /*申请失败*/
  {
    result =  - ENOMEM;
    goto fail_malloc;
  }
  memset(serial_devp, 0, MAX_PORT*sizeof(struct serial_dev));
  
  for(i = 0; i < MAX_PORT; i ++)
  	serial_setup_cdev(&serial_devp[i], i);
	
  init_MUTEX(&serial_devp->sem);   /*初始化信号量*/
  init_waitqueue_head(&serial_devp->r_wait); /*初始化读等待队列头*/
  init_waitqueue_head(&serial_devp->w_wait); /*初始化写等待队列头*/

  return 0;

  fail_malloc: 
	iounmap(fpga_mapped_addr);
  	unregister_chrdev_region(devno, 1);
  	return result;
}


/*模块卸载函数*/
void serial_exit(void)
{
  int i;
  printk(KERN_ALERT "serial exit\n");
  for(i = 0; i < MAX_PORT; i++)
  	cdev_del(&(serial_devp[i].cdev));   /*注销cdev*/
  kfree(serial_devp);     /*释放设备结构体内存*/
  iounmap(fpga_mapped_addr);
  unregister_chrdev_region(MKDEV(serial_major, 0), MAX_PORT); /*释放设备号*/
}

module_init(serial_init);
module_exit(serial_exit);


