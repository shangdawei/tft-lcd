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
#include <asm-alpha/delay.h>
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
#define	UART_GET_RSR(p)		(u8)readb((p < 8 ? 0x66 : 0x6D) + fpga_mapped_addr)
#define	UART_PUT_RSR(p)		(u8)readb((p < 8 ? 0x67 : 0x6E) + fpga_mapped_addr)
/*read and write irg reg*/
#define	UART_GET_RST(p)		(u8)readb((p < 8 ? 0x30 : 0x68) + fpga_mapped_addr)
#define	UART_GET_WST(p)		(u8)readb((p < 8 ? 0x33 : 0x6B) + fpga_mapped_addr)


#define	UART_GET_RIRQ(p)	(u8)(readb((p < 8 ? 0x31 : 0x69) + fpga_mapped_addr))
#define	UART_ENB_RIRQ(p)	writeb((1 << (p%8)) | UART_GET_RIRQ(p),  (p < 8 ? 0x31 : 0x69) + fpga_mapped_addr)
#define	UART_DIB_RIRQ(p)	writeb(~(1 << (p%8)) & UART_GET_RIRQ(p), (p < 8 ? 0x31 : 0x69) + fpga_mapped_addr)
#define	UART_GET_WIRQ(p)	(u8)readb((p < 8 ? 0x32 : 0x6a) + fpga_mapped_addr)
#define	UART_ENB_WIRQ(p)	writeb((1 << (p%8)) | UART_GET_WIRQ(p), (p < 8 ? 0x32 : 0x6a) + fpga_mapped_addr)
#define	UART_DIB_WIRQ(p)	writeb(~(1 << (p%8)) & UART_GET_WIRQ(p), (p < 8 ? 0x32 : 0x6A) + fpga_mapped_addr)
/*FIFO reset reg*/

/*baud rates*/
#define	UART_GET_BAUD(p)	(u8)readb(p + 0x20 + fpga_mapped_addr)
#define	UART_SET_BAUD(p,c)	writeb(c, p + 0x20 + fpga_mapped_addr)
/*data bit */
#define	UART_GET_ALL(p)		(u8)(readb(p + 0x10 + fpga_mapped_addr))
#define	UART_GET_DBIT(p)	(u8)(readb(p + 0x10 + fpga_mapped_addr) & 0x03)
#define	UART_SET_DBIT(p,c)	writeb((c & 0x03) | (UART_GET_ALL(p) & 0xfc), p + 0x10 + fpga_mapped_addr)
/*check bit*/
#define	UART_GET_CBIT(p)	(u8)((readb(p + 0x10 + fpga_mapped_addr) >> 4) & 0x03)
#define	UART_SET_CBIT(p,c)	writeb(((c & 0x03) << 4) | (UART_GET_ALL(p) & 0xcf), p + 0x10 + fpga_mapped_addr)
/*stop bit*/
#define	UART_GET_SBIT(p)	(u8)((readb(p + 0x10 + fpga_mapped_addr) >> 6) & 0x01)
#define	UART_SET_SBIT(p,c)	writeb(((c & 0x01) << 6) | (UART_GET_ALL(p) & 0x3f), p + 0x10 + fpga_mapped_addr)
/*FIFO reset*/
#define	UART_FIFO_GETSET(p)	(u8)(readb((p < 8 ? 0x45 : 0x6c) + fpga_mapped_addr))
#define	UART_FIFO_RESET(p)	writeb((1 << (p%8)) | UART_FIFO_GETSET(p),  (p < 8 ? 0x45 : 0x6c) + fpga_mapped_addr)
#define	UART_FIFO_NORMAL(p)	writeb(~(1 << (p%8)) & UART_FIFO_GETSET(p), (p < 8 ? 0x45 : 0x6c) + fpga_mapped_addr)


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
	{      0, B115200  }
};

static const u8 fpga_baud[] = {
	1,
	3,
	5,
	11,
	23,
	47,
	95,
	191,
	1
};


static void * fpga_mapped_addr;
#define	MAX_BUF_LEN		8//1024
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
//	int time = 10000;
	UART_DIB_RIRQ(devp->index);//disable recvive irq
//	DBG("index = %d, RSR = 0x%08x\n", devp->index, UART_GET_RSR(devp->index));
	while(devp->recv_full_flag == 0 && (~UART_GET_RSR(devp->index) & (1 << (devp->index%8))))
	{
		devp->recv_buf[devp->recv_wr_point++] = UART_GET_CHAR(devp->index);
		if(devp->recv_wr_point >= MAX_BUF_LEN)
			devp->recv_wr_point = 0;
		if(devp->recv_wr_point == devp->recv_rd_point)
			devp->recv_full_flag = 1;			
	}
//	while(time --);
//	DBG("wp = %d, rp = %d\n", devp->recv_wr_point, devp->recv_rd_point);
//	UART_ENB_RIRQ(devp->index);
	return ;
}
static void get_recv_irg(void);
DECLARE_TASKLET(recv_tasklet, get_recv_irg, 0);
/*当接收中断来时调用此函数*/
static void get_recv_irg(void)
{
	int i;
//	printk("ssssssssssss\n");
	for(i = 0; i < MAX_PORT; i++)
	{
//		DBG("iiii = %d\n", i);
		if(UART_GET_RST(i) & (1 << (i%8)))
		{
//			DBG("i = %d\n", i);
			get_buf(&serial_devp[i]);
			wake_up_interruptible(&serial_devp[i].r_wait);//wake up read
		}
	}
}
irqreturn_t my_recv_interrupt(void)
{
	unsigned int state;
//	disable_irq(AT91SAM9260_ID_IRQ0);
//	DBG("my_recv_interrupt\n");
//	state = at91_sys_read(AT91_AIC_ISR);
//	DBG("state = 0x%08x\n", state);
	get_recv_irg();
//	tasklet_schedule(&recv_tasklet);
//	enable_irq(AT91SAM9260_ID_IRQ0);
	return IRQ_HANDLED;
}
static int recvbuf_to_userbuf(struct serial_dev *devp, char __user *buf, int len)
{
	int i = 0;
//	int time = 30000;
//	while(time --);
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
	if(copy_to_user(buf, p, i))
	{
		i = -EFAULT;
	}
	kfree(p);
//	DBG("112wp = %d, rp = %d\n", devp->recv_wr_point, devp->recv_rd_point);
	while(1)
	return i;
}

static void send_buf(struct serial_dev *devp)
{
	int cnt = 0;
	UART_DIB_WIRQ(devp->index);//disable recvive irq
	while(devp->send_rd_point != devp->send_wr_point || devp->send_full_flag == 1)
	{
//		DBG("w!=r\n");
		if(1)//(!UART_PUT_RSR(devp->index))	
		{
			UART_PUT_CHAR(devp->index, devp->send_buf[devp->send_rd_point++]);
			cnt ++;
			if(devp->send_rd_point >= MAX_BUF_LEN)
			{
				devp->send_rd_point = 0;
			}
			devp->send_full_flag = 0;
			/*write less than 128 bytes every time*/
			if(cnt >= 128) break;
		}
		else
		{
			break;
		}
	}

	if(devp->send_rd_point == devp->send_wr_point)
	{
	//	UART_DIB_WIRQ(devp->index);
//		DBG("w==r\n");
		return;
	}
	UART_ENB_WIRQ(devp->index);
	enable_irq(AT91SAM9260_ID_IRQ1);
}
static void get_send_irg(void);
DECLARE_TASKLET(send_tasklet, get_send_irg, 0);
static void get_send_irg(void)
{
	int i;
	for(i = 0; i < MAX_PORT; i++)
	{
		if(UART_GET_WST(i) & (1 << (i%8)))
		{
//			DBG("i = %d\n", i);
			send_buf(&serial_devp[i]);
		}
	}
}
irqreturn_t my_send_interrupt(void)
{
	unsigned int state;
	disable_irq(AT91SAM9260_ID_IRQ1);
//	DBG("my_send_interrupt\n");
	state = at91_sys_read(AT91_AIC_ISR);
//	DBG("state = 0x%08x\n", state);
	get_send_irg();
//	enable_irq(AT91SAM9260_ID_IRQ1);

//	tasklet_schedule(&send_tasklet);
	return IRQ_HANDLED;
}

static int userbuf_to_sendbuf(struct serial_dev *devp, const char *buf, int len)
{
	int i = 0;
//	char *p = NULL;
//	DBG("userbuf_to_sendbuf: flag = %d\n", devp->send_full_flag);
	if(devp->send_full_flag == 1)
		return 0;//it should be bolck

//	p = kmalloc(len , GFP_KERNEL);
//	copy_from_user(p, buf, len);
	while(devp->send_full_flag == 0 && i < len)
	{	
		devp->send_buf[devp->send_wr_point++] = buf[i++];
		if(devp->send_wr_point >= MAX_BUF_LEN)
			devp->send_wr_point = 0;
		if(devp->send_wr_point == devp->send_rd_point)
			devp->send_full_flag = 1;
	}
//	kfree(p);
	UART_ENB_WIRQ(devp->index);
	enable_irq(AT91SAM9260_ID_IRQ1);
//	DBG("userbuf_to_sendbuf: i = %d\n", i);
	return i;
}
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
	DBG("i = %d, x = %x\n", i , x);
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
int serial_my_open(struct inode *inode, struct file *filp)
{
  /*将设备结构体指针赋值给文件私有数据指针*/
  struct serial_dev *dev;
  
  dev = container_of(inode->i_cdev,struct serial_dev,cdev); 
  printk(KERN_ALERT "open.dev is %d\n",dev->index);
  //printk(KERN_ALERT "open.dev->p is %p\n",dev->p);
  filp->private_data = dev; 
  UART_FIFO_RESET(dev->index);
  UART_FIFO_NORMAL(dev->index);
  dev->send_rd_point = 0;
  dev->send_wr_point = 0;
  dev->recv_rd_point = 0;
  dev->recv_wr_point = 0;
  dev->send_full_flag = 0;
  dev->recv_full_flag = 0;
//  UART_ENB_RIRQ(dev->index);
  return 0;
}

/*文件释放函数*/
int serial_my_release(struct inode *inode, struct file *filp)
{
//	printk(KERN_ALERT "close.dev is \n");
	struct serial_dev *dev = filp->private_data;/*获得设备结构体指针*/
	printk(KERN_ALERT "close.dev is %d\n",dev->index);
//	UART_FIFO_RESET(dev->index);
	return 0;
}

/*写函数*/
static ssize_t serial_my_write(struct file *filp, const char __user *buf,
  size_t size, loff_t *ppos)
{
  //unsigned long p =  *ppos;
  //unsigned int count = size;
  int ret = 0;
  struct serial_dev *dev = filp->private_data; /*获得设备结构体指针*/
//  printk(KERN_ALERT "write: lsbuf is [%s], size = %d\n", buf, size);
  ret = userbuf_to_sendbuf(dev, buf, size);
//  printk(KERN_ALERT "write.dev is %p,i=%d\n",dev,dev->index);  
  /*用户空间->内核空间*/
//  if (copy_from_user(&(dev->value) , buf, 1))
  //  ret =  - EFAULT;
//  DBG("write: ret = %d\n", ret);
  return ret;
}

/*写函数*/
static ssize_t serial_my_read(struct file *filp, char __user *buf,
  size_t size, loff_t *ppos)
{
//  unsigned long p =  *ppos;
//  unsigned int count = size;
  int ret = 0;
  struct serial_dev *dev = filp->private_data; /*获得设备结构体指针*/
  
//  DBG("read\n");
	if(dev->recv_rd_point != dev->recv_wr_point)
	{
//  		DBG("read=====\n");
 		 ret = recvbuf_to_userbuf(dev, buf, size);
		 return ret;
	}

 // 		DBG("read111=====\n");
  DECLARE_WAITQUEUE(wait, current); //定义等待队列

  down(&dev->sem); //获得信号量
//  DBG("recv1\n");
  add_wait_queue(&dev->r_wait, &wait); //进入读等待队列头
//  DBG("recv2\n");
  __set_current_state(TASK_INTERRUPTIBLE); //改变进程状态为睡眠
  UART_ENB_RIRQ(dev->index);
//	DBG("index = %d, val = 0x%x\n", dev->index, UART_GET_RIRQ(dev->index));

//  DBG("recv3\n");
  up(&dev->sem);
//  DBG("recv4\n");
//  DBG("index = %d\n", dev->index);
  schedule(); //调度其他进程执行
  down(&dev->sem);
//  DBG("recv5\n");
  ret = recvbuf_to_userbuf(dev, buf, size);
/*  DBG("index = %d\n", dev->index);
  UART_ENB_RIRQ(dev->index);
	enable_irq(AT91SAM9260_ID_IRQ0);
  DBG("index = %d\n", dev->index);
*/
  up(&dev->sem); //释放信号量
  remove_wait_queue(&dev->w_wait, &wait); //从附属的等待队列头移除
  set_current_state(TASK_RUNNING);
//	enable_irq(AT91SAM9260_ID_IRQ0);
  /*用户空间->内核空间*/
//  if (copy_to_user(buf, (void*)&(dev->value), 1))
  //  ret =  - EFAULT;
//  else
  {
    
  //  printk(KERN_INFO "read %d bytes(s) from %p\n", count, p);
  }

  return ret;
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

static void set_databits(int i, u16 x)
{
	DBG("databits: [%d]---%x\n", i, x);
	switch(x)
	{
		case 5:
			UART_SET_DBIT(i, 0x03);
			break;
		case 6:
			UART_SET_DBIT(i, 0x02);
			break;
		case 7:
			UART_SET_DBIT(i, 0x01);
			break;
		case 8:
		default:
			UART_SET_DBIT(i, 0x00);
			break;
	}
}

static void set_checkbits(int i, u16 x)
{
	DBG("checkbits: [%d]---%x\n", i, x);
	if(x == 0)
	{//no checkbits
		UART_SET_CBIT(i, 0);
	}
	else
	{
		if(x == 1)
		{//ji
			UART_SET_CBIT(i, 3);
		}
		else
		{
			UART_SET_CBIT(i, 2);
		}
	}
}

static void set_stopbits(int i, u16 x)
{
	DBG("stopbits: [%d]---%x\n", i, x);
	if(x == 2)
	{//2
		UART_SET_SBIT(i, 1);
	}
	else
	{
		UART_SET_SBIT(i, 0);
	}
}

int set_attr(struct termios __user * u_termio, struct ktermios *a_termios, int index)
{
	struct ktermios *k_termios = (a_termios);
	int ret = -1;
//	k_termio.c_cflag = B50;
//	printk("k_termio.c_cflag = 0x%x\n", k_termios->c_cflag);
	ret = copy_from_user(k_termios, u_termio, sizeof(struct ktermios));
	set_baud_rates(index, k_termios->c_cflag);
	set_databits(index, k_termios->c_iflag);
	set_checkbits(index, k_termios->c_oflag);
	set_stopbits(index, k_termios->c_lflag);
//	printk("u_termio->c_cflag = 0x%x\n", (struct termios __user *)u_termio->c_cflag);
//	printk("ret1 = %d\n", ret);
	return 0;
}
static int serial_ioctl(struct inode *tty, struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret = -ENOIOCTLCMD;
//	void __user *p = (void __user *)arg;	
	printk("cmd = 0x%04x\n", cmd);
	char *pp = (char *)arg;
	struct serial_dev *dev = filp->private_data; /*获得设备结构体指针*/
//	DBG("serial_ioctl: wp = %d, rp = %d\n", serial_devp[7].recv_wr_point, serial_devp[7].recv_rd_point);
//	dev->termios.c_cflag = 0x03;
//	printk("cflag = 0x%x\n", dev->termios.c_cflag);
//	ret = at91_sys_read(AT91_AIC_IPR);
//	printk("IPR = 0x%08x\n", ret);
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
		DBG("read: 0x%x, 0x%x\n", pp[0], pp[1]);
		break;
	case F_WRITE:
		writeb(pp[1], fpga_mapped_addr + pp[0]);
		DBG("write: 0x%x, 0x%x\n", pp[0], pp[1]);
		break;
	default:
		break;
	}
	return 0;
}

/*文件操作结构体*/
static const struct file_operations serial_fops =
{
  .owner = THIS_MODULE,
  .read = serial_my_read,
  .write = serial_my_write,
  .open = serial_my_open,
  .release = serial_my_release,
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

//  at91_set_A_periph(AT91_PIN_PC12, 1);//irq0
//  at91_sys_write(AT91_AIC_ISCR, 1 << AT91SAM9260_ID_IRQ0);
//  at91_sys_write(AT91_AIC_IECR, 1 << AT91SAM9260_ID_IRQ0);
//	disable_irq(AT91SAM9260_ID_IRQ1);

	/* 动态申请2个设备结构体的内存*/
  serial_devp = kmalloc(MAX_PORT*sizeof(struct serial_dev), GFP_KERNEL);
  if (!serial_devp)    /*申请失败*/
  {
    result =  - ENOMEM;
    goto fail_malloc;
  }
  memset(serial_devp, 0, MAX_PORT*sizeof(struct serial_dev));
  
  for(i = 0; i < MAX_PORT; i ++)
  {
  	serial_setup_cdev(&serial_devp[i], i);
	
  init_MUTEX(&serial_devp[i].sem);   /*初始化信号量*/
  init_waitqueue_head(&serial_devp[i].r_wait); /*初始化读等待队列头*/
  init_waitqueue_head(&serial_devp[i].w_wait); /*初始化写等待队列头*/
//  UART_FIFO_RESET(i);
  }

	result = request_irq(AT91SAM9260_ID_IRQ0, my_recv_interrupt, SA_SAMPLE_RANDOM, "FPGA_uart", serial_devp);
	printk("irq0 ret = %d\n", result);
	result = request_irq(AT91SAM9260_ID_IRQ1, my_send_interrupt, SA_SAMPLE_RANDOM, "FPGA_uart", serial_devp);
	printk("irq1 ret = %d\n", result);
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
  free_irq(AT91SAM9260_ID_IRQ0, serial_devp);
  free_irq(AT91SAM9260_ID_IRQ1, serial_devp);
  for(i = 0; i < MAX_PORT; i++)
  	cdev_del(&(serial_devp[i].cdev));   /*注销cdev*/
  kfree(serial_devp);     /*释放设备结构体内存*/
  iounmap(fpga_mapped_addr);
  unregister_chrdev_region(MKDEV(serial_major, 0), MAX_PORT); /*释放设备号*/
}

module_init(serial_init);
module_exit(serial_exit);


