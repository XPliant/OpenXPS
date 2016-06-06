/************************************************************************
* Copyright (C) 2016, Cavium, Inc.
* All Rights Reserved.
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; version 2
* of the License.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* A copy of the GNU General Public License, version 2 is available in the file 
* named LICENSE-GPLv2.md either in this directory or its root. 
* Alernatively to obtain a copy, write to the Free Software Foundation, Inc., 
* 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*
* File: xp_pcie_slave.c
* 
* Abstract: This file contains the pcie core/slave implementation required for 
* xpliant.
************************************************************************/
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/interrupt.h>
#include <linux/version.h>
#include <linux/pid_namespace.h>
#include <linux/netdevice.h>

#include "xp_common.h"
#include "xp_reg_info.h"
#include "xp_pcie_slave.h"
#include "xp_header.h"
#include "xp_netdev.h"

#define VENDOR_ID             0x177D
#define CNX88091_A0           0xF000
#define CNX88091_B0           0xF008
#define CNX88072_B0           0xF009
#define CNX88071_B0           0xF00A
#define CNX88061_B0           0xF00B
#define DRIVER_NAME           "xpPCIe"

#define SUPPORTED_DEVICE_IDS  5

#define DRIVER_VERSION        "3.2.3"

#define XP_PCI_MAX_ADAPTERS   255

/* XP80 IO memory size which should be mmaped. */
#define XP_MAX_IO_MEM         (256 * 1024 * 1024)
#define XP_DMA_NODE_MINOR     1048575

#define BAR0                  0  /* Base address register - 0 */

#define GET_REG               1  /* Read Flag for direction   */
#define SET_REG               0  /* write flage for direction */

/* Intrupt Enable */
#define INTR_COUNT            2  /* MSI vector's count */

#define PCIE_IOCTL_ID         40 /* PCIe device ID */
#define PCIE_REG_ID           0
#define PCIE_REGISTER_ID      1
#define PHY_MEM_ALLOC_ID      2
#define PHY_MEM_FREE_ID       3
#define DMA_CLEAN_UP_ID       4
#define NETDEV_INIT_ID        5
#define NETDEV_DE_INIT_ID     6
#define DRIVER_VERSION_ID     7
#define PCIE_CONFIG_READ_ID   8

#define XP_RT_SIGNAL          (SIGRTMIN + 3)

#define HIGH                  1
#define LOW                   0

#define TOTAL_RX_QUEUE        64
#define TOTAL_TX_QUEUE        64

#define PCI_CONFIG_SPACE_SIZE 256

#define RX_QUEUE              1
#define TX_QUEUE              0

#define XPREG_PROC_FILE_NAME "xpregaccess"
#define XPREG_PROC_FILE_PATH "/proc/" XPREG_PROC_FILE_NAME

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,16,0)
#define xp_pci_enable_msi_block(a,b) \
    pci_enable_msi_range(a,b,b)
#else
#define xp_pci_enable_msi_block(a,b) \
    pci_enable_msi_block(a,b)
#endif

#define INIT_NETDEV _IO(PCIE_IOCTL_ID, NETDEV_INIT_ID)
#define DMA_CLEAN_UP _IO(PCIE_IOCTL_ID, DMA_CLEAN_UP_ID)
#define REGISTER_PID _IO(PCIE_IOCTL_ID, PCIE_REGISTER_ID)
#define DEINIT_NETDEV _IO(PCIE_IOCTL_ID, NETDEV_DE_INIT_ID)

#define PCIE_REG_CMD _IOWR(PCIE_IOCTL_ID, PCIE_REG_ID, uint64_t)
#define PHY_MEM_FREE _IOWR(PCIE_IOCTL_ID, PHY_MEM_FREE_ID, uint64_t)
#define PHY_MEM_ALLOC _IOWR(PCIE_IOCTL_ID, PHY_MEM_ALLOC_ID, uint64_t)
#define GET_DRIVER_VERSION _IOWR(PCIE_IOCTL_ID, DRIVER_VERSION_ID, uint64_t)
#define PCIE_CONFIG_READ_CMD _IOWR(PCIE_IOCTL_ID, PCIE_CONFIG_READ_ID,\
                                   uint64_t)

typedef struct __attribute__((__packed__)) xp_mem_user {
    u64 addr;
    u64 paddr;
    u32 len;
    u32 offset;
} xp_mem_user_t;

typedef struct __attribute__((__packed__)) xp_mem_info {
    u64 addr;
    u64 paddr;
    u32 len;
    u32 offset;
    struct list_head list;
} xp_mem_list_t;

typedef struct xp_work {
    pid_t app_pid;
    struct siginfo sig_info;
    struct work_struct work;
} xp_work_t;

typedef struct xp_dma_private {
    struct cdev cdev;
    u32 mem_index;
    struct device *dev;
} xp_dma_private_t;

typedef struct xp_intr_info {
    u32 reg_type       : 1;
    u32 block_id       : 8;
    u32 dev_id         : 6;
    u32 has_queue_info : 1;
    u32 q_type         : 1;
    u32 q_num          : 6;
    u32 intr_status    : 3;
    u32 r0             : 6; /* Reserved */
} xp_intr_info_t;

/* Read PCIe config space */
typedef struct __attribute__((__packed__)) xp_pcie_config {
    u16 offset; /* Pcie config space offset to read */

    /* Size to be read in bytes this
       should not be greater than 256 bytes */
    u8  size;
    u64 value;  /* Value(s) to be read */
} xp_pcie_config_t;

extern int xp_dev_reg_read(u32 *rw_value, u32 reg_addr, 
                           u8 reg_size, xp_private_t *priv);
extern int xp_dev_reg_write(u32 rw_value, u32 reg_addr, 
                            u8 reg_size, xp_private_t *priv);
extern netdev_tx_t xpnet_start_xmit(struct sk_buff *skb,
                               xpnet_private_t *net_priv);
extern int xp_netdev_init(xp_private_t *priv);
extern void xp_netdev_deinit(xp_private_t *priv);

/* User can override it via command line argument */
static bool isr_enable = 1;

/* Default 0, user can override it via command line argument */
static unsigned int dyn_pcie_dev_id;

static dev_t xp_dev;
static xp_dma_private_t *xp_dma_priv;
static unsigned int xp_pcidev_major;
static LIST_HEAD(xp_mem_list_head);
static struct class *xp_pci_class;

static u8 xp_hp_reg_bit_width[NUM_HP_INT_REG_BLOCKS] = {
    1, 1, 1,  1,  1, 1,  1, 
    1, 1, 2, 12,  3, 4,  1, 
    1, 1, 1,  1, 12, 3,  4, 
    1, 1, 1,  1,  1, 1, 16, 
    2, 2, 2, 32,  1, 1,  1
};

static u8 xp_hp_reg_start_bit[NUM_HP_INT_REG_BLOCKS] = {
     0,  1,  2,  3,   4,   5,   6, 
     7,  8,  9, 11,  23,  26,  30, 
    31, 32, 33, 34,  35,  47,  50, 
    54, 55, 56, 57,  58,  59,  60, 
    76, 78, 80, 82, 114, 115, 116
};

static u8 xp_hp_reg_end_bit[NUM_HP_INT_REG_BLOCKS] = {
     0,  1,  2,   3,   4,  5,    6, 
     7,  8, 10,  22,  25, 29,   30, 
    31, 32, 33,  34,  46, 49,   53, 
    54, 55, 56,  57,  58, 59,   75, 
    77, 79, 81, 113, 114, 115, 116
};

static u8 xp_lp_reg_bit_width[NUM_LP_INT_REG_BLOCKS] = {
    1, 1, 1, 1, 1,  1, 1,  1, 1, 2, 12, 
    3, 4, 1, 1, 1,  1, 1, 12, 3, 4,  1, 
    1, 1, 1, 1, 1, 16, 2,  2, 2, 1,  1
};

static u8 xp_lp_reg_start_bit[NUM_LP_INT_REG_BLOCKS] = {
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 11, 
    23, 26, 30, 31, 32, 33, 34, 35, 47, 50, 54, 
    55, 56, 57, 58, 59, 60, 76, 78, 80, 82, 83
};

static u8 xp_lp_reg_end_bit[NUM_LP_INT_REG_BLOCKS] = {
     0,  1,  2,  3,  4,  5,  6,  7,  8, 10, 22, 
    25, 29, 30, 31, 32, 33, 34, 46, 49, 53, 54, 
    55, 56, 57, 58, 59, 75, 77, 79, 81, 82, 83
};

static u32 xp_reg_mask[33] = {
    0x00000000, 0x00000001, 0x00000003, 
    0x00000007, 0x0000000f, 0x0000001f, 
    0x0000003f, 0x0000007f, 0x000000ff, 
    0x000001ff, 0x000003ff, 0x000007ff, 
    0x00000fff, 0x00001fff, 0x00003fff, 
    0x00007fff, 0x0000ffff, 0x0001ffff, 
    0x0003ffff, 0x0007ffff, 0x000fffff, 
    0x001fffff, 0x003fffff, 0x007fffff, 
    0x00ffffff, 0x01ffffff, 0x03ffffff, 
    0x07ffffff, 0x0fffffff, 0x1fffffff, 
    0x3fffffff, 0x7fffffff, 0xffffffff
};

u32 xp_regs_list[XP_MAX_REG_ACCESS_LIST][XP80_SUPPORTED_DEVICE_MODES] =
{   /* A0-Uncompressed, B0-Uncompressed, B0-Compressed  */
    /* HIGH_PRIO_REG                                    */
    { 0x08a80080, 0x08a8019c, 0x025e819c },
    /* LOW_PRIO_REG                                     */
    { 0x08a80090, 0x08a801ac, 0x025e81ac },
    /* HIGH_INTR_SOURCE_REG                             */
    { 0x08a8018c, 0x08a802c4, 0x025e82c4 },
    /* TXDONE_STATUS_REG                                */
    { 0x08b50000, 0x08b50000, 0x02744000 },
    /* RXDONE_STATUS_REG                                */
    { 0x08b50800, 0x08b50800, 0x02748000 },
    /* HIGE_PRIO_INT_ENABLE_REG                         */
    {          0, 0x08a80180, 0x025e8180 },
    /* LOW_PRIO_INT_ENABLE_REG                          */
    {          0, 0x08a80190, 0x025e8190 },
    /* DMA0_RX_CDP_REG_E                                */
    { 0x08b51400, 0x08b51400, 0x02750000 },
    /* DMA0_TX_CDP_REG_E                                */
    { 0x08b51000, 0x08b51000, 0x0274c000 },
    /* DMA0_RX_CMD_REG_E                                */
    { 0x08b51600, 0x08b51600, 0x02750200 },
    /* DMA0_TX_CMD_REG_E                                */
    { 0x08b51200, 0x08b51200, 0x0274c200 },
    /* CORE_CTRL_REG__1_E                               */
    { 0x08a80168, 0x08a8028c, 0x025e828c },
    /* TX_DMA0_RETRY_CNT_REG_E                          */
    { 0x08a802dc, 0x08a80444, 0x025e8444 },
    /* MGMT_CTRL_REG_E                                  */
    { 0x08a801a8, 0x08a802f4, 0x025e82f4 },
    /* DMA0_CLR_ERR_CNT_REG_E                           */
    { 0x08b51300, 0x08b51300, 0x0274c300 },
    /* DMA0_TX_CHAIN_LEN_ERR_REG_E                      */
    { 0x08b51304, 0x08b51304, 0x0274c304 },
    /* DMA0_TX_CPU_OWN_DESC_ERR_REG_E                   */
    { 0x08b51308, 0x08b51308, 0x0274c308 },
    /* DMA0_TX_ZERO_BUF_LEN_ERR_REG_E                   */
    { 0x08b5130c, 0x08b5130c, 0x0274c30c },
    /* DMA0_TX_PCIE_ERR_REG_E                           */
    { 0x08b51310, 0x08b51310, 0x0274c310 },
    /* DMA0_TX_DMA_INTF_ERR_REG_E                       */
    { 0x08b51314, 0x08b51314, 0x0274c314 },
    /* DMA0_TX_PKT_DROP_E                               */
    { 0x08b51318, 0x08b51318, 0x0274c318 },
    /* TX_DMA0_CFG_REGLOCKREG_E                         */
    { 0x08b5131c, 0x08b51328, 0x0274c328 },
    /* TX_DMA0_SCRATCHPAD_E                             */
    { 0x08b51320, 0x08b5132c, 0x0274c32c },
    /* DMA0_RX_CHAIN_LEN_ERR_REG_E                      */
    { 0x08b51700, 0x08b51700, 0x02750300 },
    /* DMA0_RX_CPU_OWN_DESC_ERR_REG_E                   */
    { 0x08b51704, 0x08b51704, 0x02750304 },
    /* DMA0_RX_ZERO_BUF_LEN_ERR_REG_E                   */
    { 0x08b51708, 0x08b51708, 0x02750308 },
    /* DMA0_RX_PCIE_ERR_REG_E                           */
    { 0x08b5170c, 0x08b5170c, 0x0275030c },
    /* RX_DMA0_CFG_REGLOCKREG_E                         */
    { 0x08b51710, 0x08b5171c, 0x0275031c },
    /* RX_DMA0_SCRATCHPAD_E                             */
    { 0x08b51714, 0x08b51720, 0x02750320 },
};

#define MAX_DEV_SUPPORTED 8
static xp_private_t *xp_pcie_dev_ptr[MAX_DEV_SUPPORTED];

static int xp_driver_version_get(void __user *argp)
{
    return copy_to_user(argp, DRIVER_VERSION, sizeof(DRIVER_VERSION));
}

static void reg_procfs_help(struct seq_file *sf, int minor)
{
   if(NULL == sf)
      return;
  
   seq_printf(sf, "\nUsage: echo [OPTIONS] >%s%d; cat %s%d\n\n",
                   XPREG_PROC_FILE_PATH, minor, XPREG_PROC_FILE_PATH, minor);
   seq_printf(sf,"1) To read register value.\n"
                 "\techo r <regoffset> >%s%d; cat %s%d"
                 "\n\tex1: echo r 0x25e82f4 >%s%d; cat %s%d",
                 XPREG_PROC_FILE_PATH, minor, XPREG_PROC_FILE_PATH, minor,
                 XPREG_PROC_FILE_PATH, minor, XPREG_PROC_FILE_PATH, minor);
   seq_printf(sf,"\n\n2) To write register value.\n"
                 "\techo w <regoffset> <value> >%s%d; cat %s%d"
                 "\n\tex1: echo w 0x25e82f4 0x12 >%s%d; cat %s%d",
                 XPREG_PROC_FILE_PATH, minor, XPREG_PROC_FILE_PATH, minor,
                 XPREG_PROC_FILE_PATH, minor, XPREG_PROC_FILE_PATH, minor);
   seq_printf(sf,"\n\n3) To print help.\n"
                 "\techo help >%s%d; cat %s%d\n",
                 XPREG_PROC_FILE_PATH, minor, XPREG_PROC_FILE_PATH, minor);


}

static int xpreg_seq_show(struct seq_file *sf, void *v)
{
    xp_private_t *xp_reg_priv = NULL;
    xp_reg_priv = sf->private;
  
    if (!strnicmp(xp_reg_priv->reg_rw_status, NAME_STR_HELP, sizeof(NAME_STR_HELP) - 1)) {
       reg_procfs_help(sf, MINOR(xp_reg_priv->cdev.dev));
    } else if(strlen(xp_reg_priv->reg_rw_status) < 1) {
       seq_printf(sf, "Invalid input. Please find help as below..");
       reg_procfs_help(sf, MINOR(xp_reg_priv->cdev.dev));
    } else {
       seq_printf(sf, "%s\n", xp_reg_priv->reg_rw_status);
    }

    return 0;
}

static int xpreg_seq_open(struct inode *inode, struct file *file)
{
    xp_private_t *xp_reg_priv = NULL;
    struct seq_file *s;
    int result;

    result = single_open(file, xpreg_seq_show, NULL);

    s = (struct seq_file *)file->private_data;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
    xp_reg_priv = PDE_DATA(file_inode(file));
#else
    xp_reg_priv = PROC_I(inode)->pde->data;
#endif
    s->private = xp_reg_priv;

    return result;

}

static ssize_t xpreg_proc_write(struct file *filp, const char *buf,
                                size_t bufsize, loff_t * off)
{
    u32 reg_index=0x0, reg_value=0x0;
    xp_private_t *xp_reg_priv = NULL;
    int rc;

    if (bufsize != 0){
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
          xp_reg_priv = PDE_DATA(file_inode(filp));
#else
          struct dentry *dentry = filp->f_path.dentry;
          struct inode *inode = dentry->d_inode;
          xp_reg_priv = PROC_I(inode)->pde->data;
#endif
         memset(xp_reg_priv->reg_rw_status, 0, 
                sizeof(xp_reg_priv->reg_rw_status));
   
         if (2 == sscanf(buf, "w 0x%x 0x%x", &reg_index, &reg_value)) {
             rc = xp_dev_reg_write(reg_value,reg_index, DWORD_SIZE,
                                  xp_reg_priv);
             if (rc == -EINVAL) {
                 snprintf(xp_reg_priv->reg_rw_status,
                          sizeof(xp_reg_priv->reg_rw_status) - 1,
                         "Register write failed\n");
             } else {
                 snprintf(xp_reg_priv->reg_rw_status, 
                          sizeof(xp_reg_priv->reg_rw_status) - 1, 
                         "Write register = 0x%x value = 0x%x\n",
                          reg_index, reg_value);
             }
         } else if(1 == sscanf(buf, "r 0x%x", &reg_index)) {
             rc = xp_dev_reg_read(&reg_value,reg_index, DWORD_SIZE,
                                 xp_reg_priv);
             if (rc == -EINVAL) {
                 snprintf(xp_reg_priv->reg_rw_status,
                          sizeof(xp_reg_priv->reg_rw_status) - 1,
                         "Register read failed\n");
             } else {
                 snprintf(xp_reg_priv->reg_rw_status,
                          sizeof(xp_reg_priv->reg_rw_status) - 1,
                          "Read register = 0x%x value = 0x%x\n",reg_index,
                          reg_value);
             }
        } else if(!strnicmp(buf, NAME_STR_HELP, sizeof(NAME_STR_HELP) - 1)) {
                snprintf(xp_reg_priv->reg_rw_status,
                         sizeof(xp_reg_priv->reg_rw_status) - 1,
                         "%s", NAME_STR_HELP);
        }
    }
    return bufsize;
}

static const struct file_operations xpreg_proc_fops = {
	.owner = THIS_MODULE,
	.open = xpreg_seq_open,
	.read = seq_read,
	.write = xpreg_proc_write,
	.llseek = seq_lseek,
	.release = single_release,
};

struct proc_dir_entry *xpreg_proc_create(const char *root,
                   struct proc_dir_entry *parent, xp_private_t *data)
{
    return  proc_create_data(root, 0644, parent, &xpreg_proc_fops, data);
}


static u32 xp_block_number_get(u32 *reg_value, u32 start_bit, 
                               u32 end_bit, u8 bit_width)
{
    u8 index = 0;
    u32 mask = 0x1, reg_word_0 = 0x0, 
        reg_word_1 = 0x0, reg_word_2 = 0x0;

    /* Get the block data. */
    if ((start_bit / 32) == (end_bit / 32)) {
        /* If the start bit and end bit are in the same dword then
         * the bits are in a single dword.
         */
        index  = start_bit / 32;
        reg_word_0 = reg_value[index];
        reg_word_0 >>= (start_bit % 32);

        if (start_bit != end_bit)
            mask = xp_reg_mask[bit_width];

        /* else the bit width is 1 so use the default value of mask = 0x1 */
    } else {
        /* If the start bit and end bit are in the diffrent dwords then
         * the bits are in different dwords. They need to be combined to get 
         * a single word.
         */
        reg_word_1 = reg_value[start_bit / 32];
        reg_word_1 = reg_word_1 >> (start_bit % 32);
        reg_word_2 = reg_value[end_bit / 32];
        reg_word_2 = reg_word_2 << (32- (start_bit % 32));
        reg_word_0 = reg_word_1 | reg_word_2;
        mask = xp_reg_mask[bit_width];
    }

    pr_debug("xppci: reg_word_0: 0x%x\n", reg_word_0);
    return reg_word_0 & mask;
}

static void xp_task_work_handler(struct work_struct *w)
{
    struct task_struct *task = NULL;
    xp_work_t *work = container_of(w, xp_work_t, work);

    if (!work->app_pid) {
        pr_err("No pid is Registered for SDK.\n");
        return;
    }

    pr_debug("Sending event with Interrupt Offset = %d\n", 
            work->sig_info.si_int);
    pr_debug("Application Pid  = %d\n", work->app_pid);

    rcu_read_lock();
    task = pid_task(find_pid_ns(work->app_pid, &init_pid_ns), PIDTYPE_PID);
    rcu_read_unlock();

    if (!task) {
        pr_debug("Get Task for registered PID Fail!\n");
    } else {
        if (send_sig_info(XP_RT_SIGNAL, &work->sig_info, task) < 0)
            pr_debug("Error in sending signal to user.\n");
    }

    kfree((void *)work);
}

static int xp_irq_handler(xp_private_t *priv, u8 reg_blocks, 
                          u8 *reg_start_bits, u8 *reg_end_bits, 
                          u8 *reg_bit_widths, u32 *intr_reg_val)
{
    u32 block_num = 0, start_bit = 0, end_bit = 0, block_value = 0;
    xp_work_t *task = NULL;
    static xp_intr_info_t intr_info;

    for (block_num = 0; block_num < reg_blocks; block_num++) {
        start_bit = reg_start_bits[block_num];
        end_bit = reg_end_bits[block_num];

        block_value = xp_block_number_get(intr_reg_val, start_bit, 
                                          end_bit, reg_bit_widths[block_num]);
        if (block_value) {
            memset(&intr_info, 0, sizeof(intr_info));
            intr_info.block_id = block_num;

            if (reg_blocks == NUM_HP_INT_REG_BLOCKS) {
                intr_info.reg_type = HIGH;
            } else {
                intr_info.reg_type = LOW;
            }

            intr_info.dev_id = MINOR(priv->cdev.dev);

            task = kmalloc(sizeof(xp_work_t), GFP_ATOMIC);
            if (!task) {
                pr_err("Failed to allocate memory.\n");
                return -ENOMEM;
            }

            /* Copy intr info to pass it to task handler. */
            memcpy(&task->sig_info, &priv->sig_info, sizeof(task->sig_info));
            memcpy(&task->sig_info.si_int, 
                   &intr_info, sizeof(task->sig_info.si_int));
            memcpy(&task->app_pid, &priv->app_pid, sizeof(task->app_pid));

            if (priv->w_queue) {
                INIT_WORK(&task->work, xp_task_work_handler);
            }

            pr_debug("block_num : %d\n", block_num);
            pr_debug("Interrupt generated for block: %x\n", 
                     task->sig_info.si_int);
            queue_work(priv->w_queue, &task->work);
        }
    }

    return 0;
}

static int xp_irq_mgmt_handler(xp_private_t *priv)
{
    u8 intr_status = 0, i = 0, j = 0, bit_pos = 0;
    u32 high_intr_src_reg[4], status_reg_addr = 0, queue_bit_map = 0;
    xp_work_t *task = NULL;
    unsigned long flags = 0;

    static xp_intr_info_t intr_info;

    memset(&intr_info, 0, sizeof(intr_info));

    for (i = 0; i < 5; i++) {
        high_intr_src_reg[i] = 0; /* Clear the values for older interrupts. */
        spin_lock_irqsave(&priv->tx_dma_read_lock, flags);
        high_intr_src_reg[i] = 
            *((u32*)((uint8_t*)(priv->vma) + 
            XP_GET_PCI_BASE_OFFSET_FROM_REG_NAME(HIGH_INTR_SOURCE_REG_ADDR, 
                                                 priv->mode)) + i); 
        spin_unlock_irqrestore(&priv->tx_dma_read_lock, flags);

        if (high_intr_src_reg[i]) {
            /* Get the queue number only if the bit in this register is set. */
            if (i == 0) {
                j = DMA0_INTR_START_BIT_POS(priv->mode);
            } else {
                j = 0;
            }

            for(; j < (sizeof(u32) * 8); j++) {
                /* Check for any  dma0 RX/TX interrupt bit enable or not. */
                if ((high_intr_src_reg[i] >> j) & 0x1) {
                    intr_info.has_queue_info = 1; 
                    break;
                }
            }
        }

        if (intr_info.has_queue_info) {
            /* Calculate bit position. */
            bit_pos = (i * 32) + j; 

            if (bit_pos >= DMA0_INTR_START_BIT_POS(priv->mode) &&
                bit_pos < (TOTAL_RX_QUEUE + DMA0_INTR_START_BIT_POS(priv->mode))) {
                intr_info.q_num = bit_pos - DMA0_INTR_START_BIT_POS(priv->mode);
                intr_info.q_type = RX_QUEUE;
            } else if (bit_pos >= (TOTAL_TX_QUEUE + DMA0_INTR_START_BIT_POS(priv->mode)) &&
                       bit_pos < (DMA0_INTR_START_BIT_POS(priv->mode) +
                                  TOTAL_RX_QUEUE + TOTAL_TX_QUEUE)) {
                intr_info.q_num = bit_pos -
                    (TOTAL_TX_QUEUE + DMA0_INTR_START_BIT_POS(priv->mode));
                intr_info.q_type = TX_QUEUE;
            }

            intr_info.block_id = 0;
            intr_info.dev_id = MINOR(priv->cdev.dev);

            if (intr_info.q_type == RX_QUEUE) {
                status_reg_addr = 
                    DMA0_RXDONE(XP_GET_PCI_BASE_OFFSET_FROM_REG_NAME(
                       DMA0_RXDONE_STATUS_REG, priv->mode), 
                                intr_info.q_num);
            } else {
                status_reg_addr = 
                    DMA0_TXDONE(XP_GET_PCI_BASE_OFFSET_FROM_REG_NAME(
                       DMA0_TXDONE_STATUS_REG, priv->mode), 
                                intr_info.q_num);
            }
                        
            spin_lock_irqsave(&priv->tx_dma_read_lock, flags);
            intr_status = *(u32*)((uint8_t*)(priv->vma) + status_reg_addr);
            spin_unlock_irqrestore(&priv->tx_dma_read_lock, flags);
            intr_info.intr_status = intr_status;

            task = kmalloc(sizeof(xp_work_t), GFP_ATOMIC);
            if (!task) {
                pr_err("Failed to allocate memory.\n");
                return -ENOMEM;
            }

            /* Copy intr info to pass it to task handler. */
            memcpy(&task->sig_info, &priv->sig_info, sizeof(task->sig_info));
            memcpy(&task->sig_info.si_int, 
                   &intr_info, sizeof(task->sig_info.si_int));
            memcpy(&task->app_pid, &priv->app_pid, sizeof(task->app_pid));

            if (priv->w_queue) {
                INIT_WORK(&task->work, xp_task_work_handler);
            }

            queue_work(priv->w_queue, &task->work);
            intr_status = 0x7; /* Clear all interrupts. */
            xp_dev_reg_write(intr_status, status_reg_addr, 1, priv);
            queue_bit_map = 1 << j;
            xp_dev_reg_write(queue_bit_map, 
                             XP_GET_PCI_BASE_OFFSET_FROM_REG_NAME(
                             HIGH_INTR_SOURCE_REG_ADDR, priv->mode) + 
                             (i * 4), 4, priv);
            break;
        }
    }

    return 0;
}

static irqreturn_t xp_msi_irq_handler_high(int irq, void *data)
{
    int rc = 0;
    int i = 0;
    u32 reg_value[4];
    unsigned long flags = 0;
    xp_private_t *priv = (xp_private_t *)data;

    pr_debug("XP80:%s:Interrupt handler at %d, "
            "irq: %d\n", __func__, __LINE__, irq);

    for (i = 0; i < XP_HP_INT_REG_SIZE_IN_DWRD; i++) {
        reg_value[i] = 0; /* Clear the values for older interrupts. */
        spin_lock_irqsave(&priv->tx_dma_read_lock, flags);

        reg_value[i] = *((u32*)((uint8_t*)(priv->vma) + 
            XP_GET_PCI_BASE_OFFSET_FROM_REG_NAME(HIGH_PRIO_REG_ADDR, 
                                                 priv->mode)) + i);

        spin_unlock_irqrestore(&priv->tx_dma_read_lock, flags);
    }

    /* Bit 116 in high priority register indicates mgmt block. */
    if (reg_value[3] & 0x100000) {
        rc = xp_irq_mgmt_handler(priv);
        if (rc < 0) {
            return rc;
        } else {
            return IRQ_HANDLED;
        }
    }

    if ((reg_value[0] == 0) & (reg_value[1] == 0) & 
        (reg_value[2] == 0) & (reg_value[3] == 0)) {
        return IRQ_HANDLED;
    } else {
        /* Handle interrupts only if the high priority 
           interrupt summary register has non-zero value. */
        rc = xp_irq_handler(priv, NUM_HP_INT_REG_BLOCKS, xp_hp_reg_start_bit, 
                            xp_hp_reg_end_bit, xp_hp_reg_bit_width, reg_value);
        if (rc < 0) {
            pr_err("ERR: Could not handle high prioirty interrupts.\n");
            return -1;
        }
    }

    return IRQ_HANDLED;
}

static irqreturn_t xp_msi_irq_handler_low(int irq, void *data)
{
    int rc = 0;
    int i = 0;
    u32 reg_value[4];
    unsigned long flags = 0;
    xp_private_t *priv = (xp_private_t *)data;

    pr_debug("XP80:%s:Interrupt handler at %d, "
            "irq: %d\n", __func__, __LINE__, irq);

    for (i = 0; i < XP_LP_INT_REG_SIZE_IN_DWRD; i++) {
        reg_value[i] = 0; /* Clear the values for older interrupts. */
        spin_lock_irqsave(&priv->tx_dma_read_lock, flags);

        reg_value[i] = *((u32*)((uint8_t*)(priv->vma) + 
            XP_GET_PCI_BASE_OFFSET_FROM_REG_NAME(LOW_PRIO_REG_ADDR, 
                                                 priv->mode)) + i);

        spin_unlock_irqrestore(&priv->tx_dma_read_lock, flags);
    }

    /* Bit 83 in low priority register indicates mgmt block. */
    if(reg_value[2] & 0x80000) {
        rc = xp_irq_mgmt_handler(priv);
        if (rc < 0) {
            return -1;
        } else {
            return IRQ_HANDLED;
        }
    }

    if ((reg_value[0] == 0) & (reg_value[1] == 0) & 
        (reg_value[2] == 0) & (reg_value[3] == 0)) {
        return IRQ_HANDLED;
    } else {
        rc = xp_irq_handler(priv, NUM_LP_INT_REG_BLOCKS, xp_lp_reg_start_bit, 
                            xp_lp_reg_end_bit, xp_lp_reg_bit_width, reg_value);
        if (rc < 0) {
            pr_err("ERR: Could not handle low prioirty interrupts.\n");
        }
    }

    return IRQ_HANDLED;
}

static int xp_dma_mem_access(void __user *argp, xp_dma_private_t *priv)
{
    int i = 0;
    int rc = 0;
    size_t area = 0;
    dma_addr_t mem_dma;
    char *mem = NULL;
    xp_mem_user_t *user_mem = NULL;
    xp_mem_list_t *entry = NULL;
    xp_mem_list_t *mem_info_list = NULL;

    pr_debug("%s:Enter\n", __func__);

    /* User level value. */
    user_mem = (xp_mem_user_t *)kmalloc(sizeof(xp_mem_user_t), GFP_KERNEL);
    if (user_mem == NULL) {
        pr_err("Failed to allocate memory.\n");
        return -ENOMEM;
    }

    rc = copy_from_user(user_mem, argp, sizeof(xp_mem_user_t));
    if (rc) {
        pr_err("Failed to copy from user space.\n");
        goto err_nomem;
    }

    pr_info("user_mem->len = %d\n", user_mem->len);

    /* Round it up to the page bondary. */
    area = (size_t)((((u64)user_mem->len) + PAGE_SIZE - 1) & PAGE_MASK);

    pr_info("area = %zx\n", (size_t) area);

    /* Allocate a memory area with dma_alloc_coherent.
       Will be rounded up to a page boundary. */
    mem = dma_alloc_coherent(priv->dev, area, &mem_dma, GFP_KERNEL);
    if (!mem) {
        pr_err("Memory allocation fail.\n");
        rc = -ENOMEM;
        goto err_nomem;
    }

    mem_info_list = (xp_mem_list_t *)kmalloc(sizeof(xp_mem_list_t), 
                                             GFP_KERNEL);
    if (mem_info_list == NULL) {
        pr_err("Linked list kmalloc fail.\n");
        rc = -ENOMEM;
        goto err_nomem;
    }

    /* Linklist internal structure. */
    mem_info_list->offset = priv->mem_index;
    mem_info_list->addr = (u64)(size_t)mem;
    mem_info_list->len = area;
    mem_info_list->paddr = (u64)(size_t)mem_dma;

    /* User level structure. */
    user_mem->offset = priv->mem_index * PAGE_SIZE;
    user_mem->addr = (u64)(size_t)mem;
    user_mem->len = area;
    user_mem->paddr = (u64)(size_t)mem_dma;

    pr_info("mem_info_list->len : %d, mem_info_list->offset : %d,", 
            mem_info_list->len, mem_info_list->offset);
    pr_info("mem_info_list->addr : %llu\n", mem_info_list->addr);
    pr_info("  Virtual Addr : %llu,  Physical Addr : %llu\n", 
            mem_info_list->addr, user_mem->paddr);

    INIT_LIST_HEAD(&mem_info_list->list);
    list_add_tail(&(mem_info_list->list), &xp_mem_list_head);

    list_for_each_entry(entry, &xp_mem_list_head, list) {
        pr_info("index : %d entry->len : %d,", i, entry->len);
        pr_info("entry->offset : %d entry->addr : %llu\n", 
                entry->offset, entry->addr);
        i++;
    }

    rc = copy_to_user((void *)argp, (void *)user_mem, sizeof(xp_mem_user_t));
    if (rc)
        goto err_nomem;

    priv->mem_index++;

err_nomem:
    kfree(user_mem);
    return rc;
}

static int xp_dma_mem_free(void __user *argp, xp_dma_private_t *priv)
{
    int rc = 0;
    xp_mem_user_t *user_mem = NULL;
    xp_mem_list_t *entry = NULL, *tmp_entry = NULL;

    user_mem = (xp_mem_user_t *)kmalloc(sizeof(xp_mem_user_t), GFP_KERNEL);
    if (user_mem == NULL) {
        pr_err("Failed to allocate memory.\n");
        return -ENOMEM;
    }

    rc = copy_from_user(user_mem, argp, sizeof(xp_mem_user_t));
    if (rc) {
        pr_err("Failed to copy from user space.");
        goto err_copy_from_user;
    }

    list_for_each_entry_safe(entry, tmp_entry, &xp_mem_list_head, list) {
        if (entry->paddr == user_mem->paddr) {
            dma_free_coherent(priv->dev, entry->len, 
                              (void*)entry->addr, (dma_addr_t)user_mem->paddr);
            list_del(&entry->list);
            kfree(entry);
            pr_debug("Freeing node.\n");
        }
    }

err_copy_from_user:
    kfree(user_mem);
    return rc;
}

static int xp_dma_mem_cleanup(xp_dma_private_t *priv)
{
    xp_mem_list_t *entry = NULL, *tmp_entry = NULL;

    list_for_each_entry_safe(entry, tmp_entry, &xp_mem_list_head, list) {
        pr_info("Freeing node : %llu\n", entry->paddr);
        dma_free_coherent(priv->dev, entry->len, 
                          (void*)entry->addr, (dma_addr_t)entry->paddr);
        list_del(&entry->list);
        kfree(entry);
    }

    pr_info("DMA cleanup done.\n");
    priv->mem_index = 0;

    return 0;
}

static int xp_dma_open(struct inode *inode, struct file *filp)
{
    pr_info("PCI DMA node opened.");
    filp->private_data = container_of(inode->i_cdev, xp_dma_private_t, cdev);

    return 0;
}

static int xp_dma_close(struct inode *inode, struct file *filp)
{
    pr_info("PCI node closed with minor number : %d\n", iminor(inode));
    filp->private_data = NULL;

    return 0;
}

static int xp_dma_mmap(struct file *filp, struct vm_area_struct *vma)
{
    int mem_len = 0;
    char *usr_addr = NULL;
    xp_mem_list_t *entry = NULL;

    long length = vma->vm_end - vma->vm_start;
    xp_dma_private_t *priv = filp->private_data;

    pr_debug("%s:Enter\n", __func__);

    /* Page offset is bigger than allocated memory then return error. */
    if (vma->vm_pgoff >= priv->mem_index) {
        pr_err("Error xp_dma_mmap, invalid page offset.\n");
        /* At any other offset we return an error. */
        return -EIO;
    }

    /* Traversing loop for user specific offset. */
    list_for_each_entry(entry, &xp_mem_list_head, list) {
        pr_debug(" entry->offset= %d\t vma->vm_pgoff =%ld\n",
                 entry->offset, vma->vm_pgoff);

        if (entry->offset == vma->vm_pgoff) {
            pr_info("%s: entry->len : %d, entry->offset : %d, "
                    "entry->addr =%llu\n", __func__, 
                    entry->len, entry->offset, entry->addr);

            mem_len = entry->len;
            usr_addr = (char*)entry->addr;
            break;
        }
    }

    pr_info("\n:mem_len=%d usr_addr = %p length = %ld\n", 
            mem_len, usr_addr, length);

    /* Check length - do not allow larger mappings
       than the number of pages allocated. */
    if (length > mem_len) {
        pr_err("Requested length of memory not available.\n");
        return -ENOMEM;
    }

    pr_debug("\nMAP Memory.\n");

    /* Map the whole physically contiguous area in one piece. */
    return remap_pfn_range(vma, vma->vm_start, 
                           virt_to_phys((void *)usr_addr) >> PAGE_SHIFT,
                           length, vma->vm_page_prot);
}

static long xp_dma_ioctl(struct file *filp, 
                         unsigned int cmd, unsigned long arg)
{
    long rc = 0;
    void __user *argp = (void __user *)arg;
    xp_dma_private_t *priv = filp->private_data;
    
    /* This command is not involved in any DMA operation so don't check 
     * coherent_dma_mask flag. */
    if (cmd == GET_DRIVER_VERSION) {
        return xp_driver_version_get(argp);
    } /* For all other commands check coherent_dma_mask flag. */ 
    else if (!(priv->dev->coherent_dma_mask)) {
        pr_err("Coherent_dma_mask is not set.\n");
        return -ENODEV;
    }

    switch (cmd) {
        case PHY_MEM_ALLOC:
            pr_info("Physical memory allocation request.\n");
            rc = xp_dma_mem_access(argp, priv);
            break;

        case PHY_MEM_FREE:
            pr_info("Physical memory free request.\n");
            rc = xp_dma_mem_free(argp, priv);
            break;

        case DMA_CLEAN_UP:
            pr_info("DMA memory clean up request.\n");
            rc = xp_dma_mem_cleanup(priv);
            break;

        default:
            pr_err("XP DMA Invalid IOCTL command.\n");
            return -EINVAL;
    }

    return rc; 
}

static struct file_operations xp_dma_fops = {
    .owner      = THIS_MODULE,
    .open       = xp_dma_open,
    .release    = xp_dma_close,
    .mmap       = xp_dma_mmap,
    .unlocked_ioctl = xp_dma_ioctl,
    .compat_ioctl = xp_dma_ioctl,
};

static int xp_dev_reg_access(void __user *argp, xp_private_t *priv)
{
    int rc = 0;
    int i = 0;
    u8 reg_size = 0;
    u32 reg_addr = 0;
    u16 count = 0;
    xp_reg_rw_t *pci_rw = NULL;
    u32 *tmp_value = NULL, *tmp_alias = NULL;

    pci_rw = (xp_reg_rw_t *) kmalloc(sizeof(xp_reg_rw_t), GFP_KERNEL);
    if (pci_rw == NULL) {
        pr_err("Failed to allocate memory.\n");
        return -ENOMEM;
    }

    rc = copy_from_user(pci_rw, argp, sizeof(xp_reg_rw_t));
    if (rc) {
        pr_err("Failed to copy from user space.\n");
        goto err_mem_pci_rw;
    }

    count = pci_rw->count;
    if (!count) {
        rc = -EINVAL;
        goto err_mem_pci_rw;
    }

    if (!((pci_rw->size == BYTE_SIZE) || 
          (pci_rw->size == WORD_SIZE) || 
          (pci_rw->size == DWORD_SIZE))) {
        rc = -EINVAL;
        goto err_mem_pci_rw;
    }

    reg_size = pci_rw->size;
    reg_addr = pci_rw->reg_address;

    tmp_value = kmalloc(count * sizeof(u32), GFP_KERNEL);
    if (!tmp_value) {
        rc = -ENOMEM;
        goto err_mem_pci_rw;
    }

    if (pci_rw->direction == SET_REG) {
        if (copy_from_user(tmp_value, (void*)pci_rw->value, 
                           pci_rw->count * reg_size)) {
            rc = -EFAULT;
            goto err_mem_tmp;
        }

        for (i = 0; i < count; i++) {
            rc = xp_dev_reg_write(tmp_value[i], reg_addr, reg_size, priv);
            if (rc) {
                rc = -EFAULT;
                goto err_mem_tmp;
            }

            reg_addr += reg_size;
        }
    } else if (pci_rw->direction == GET_REG) {
        tmp_alias = tmp_value;

        for (i = 0; i < count; i++) {
            rc = xp_dev_reg_read(&tmp_value[i], reg_addr, reg_size, priv);
            if (rc) {
                rc = -EFAULT;
                goto err_mem_tmp;
            }

            reg_addr += reg_size;
        }

        if (copy_to_user((void*)pci_rw->value,
                         tmp_alias, count * sizeof(u32)))
            rc = -EFAULT;
    } else {
        pr_err("%s: Invalid choice of reg direction=%x\n", 
               __func__, pci_rw->direction);
        rc = -EINVAL;
    }

err_mem_tmp:
    kfree(tmp_value);

err_mem_pci_rw:
    kfree(pci_rw);
    return rc;
}

static int xp_pci_conf_get(void __user *argp, xp_private_t *priv)
{
    int i = 0;
    int rc = 0;
    u16 offset = 0;
    u8 count = 0;
    xp_pcie_config_t *conf = NULL;
    u8 *tmp_value = NULL, *tmp_alias = NULL;

    conf = (xp_pcie_config_t *) kmalloc(sizeof(xp_pcie_config_t), GFP_KERNEL);
    if (conf == NULL) {
        pr_err("Failed to allocate memory.\n");
        return -ENOMEM;
    }

    rc = copy_from_user(conf, argp, sizeof(xp_pcie_config_t));
    if (rc) {
        pr_err("Failed to copy from user space.\n");
        goto err_copy_config;
    }

    /* Copy value in local variables. */
    count = conf->size;
    if (!count) {
        rc = -EINVAL;
        goto err_copy_config;
    }

    offset = conf->offset;

    if ((offset + count) > PCI_CONFIG_SPACE_SIZE) {
        rc = -EINVAL;
        goto err_copy_config;
    }

    tmp_value = kmalloc(count * sizeof(u8), GFP_KERNEL);
    if (!tmp_value) {
        rc = -ENOMEM;
        goto err_copy_config;
    }

    tmp_alias = tmp_value;

    for (i = 0; i < count; i++) {
        pci_read_config_byte(priv->pdev, offset, &tmp_value[i]);

        if (rc) {
            rc = -EFAULT;
            goto err_config_read;
        }

        offset++;
    }

    if (copy_to_user((void *)conf->value,
                     tmp_alias, count * sizeof(u8)))
        rc = -EFAULT;

err_config_read:
    kfree(tmp_value);

err_copy_config:
    kfree(conf);

    return rc;
}

static int xp_dev_open(struct inode *inode, struct file *filp)
{
    pr_info("PCI node opened with minor number : %d\n", iminor(inode));
    filp->private_data = container_of(inode->i_cdev, xp_private_t, cdev);
    
    return 0;
}

static int xp_dev_close(struct inode *inode, struct file *filp)
{
    pr_info("PCI node closed with minor number : %d\n", iminor(inode));
    filp->private_data = NULL;

    return 0;
}

static int xp_dev_mmap(struct file *filp, struct vm_area_struct *vma)
{
    xp_private_t *priv = filp->private_data;
    unsigned int vsize = vma->vm_end - vma->vm_start;

    pr_info("MMAP request from minor number : %d\n", MINOR(priv->cdev.dev));

    if (vsize > XP_MAX_IO_MEM) {
        pr_err("Invalid size : Requested size %d is greater "
               "than available size %d\n", vsize, XP_MAX_IO_MEM);
        return -EINVAL;
     }

    vma->vm_flags |= VM_IO;
    vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
    pr_info("Base Addr: 0x%lx\n", 
            (unsigned long int)pci_resource_start(priv->pdev, BAR0));

    return io_remap_pfn_range(vma, vma->vm_start, 
                              pci_resource_start(priv->pdev, BAR0) 
                              >> PAGE_SHIFT, vsize, vma->vm_page_prot);
}

static long xp_dev_ioctl(struct file *filp, 
                         unsigned int cmd, unsigned long arg)
{
    long rc = 0;
    void __user *argp = (void __user *)arg;
    xp_private_t *priv = filp->private_data;
    
    switch (cmd) {
        case PCIE_REG_CMD:
            rc = xp_dev_reg_access(argp, priv);
            break;

        case REGISTER_PID:
            priv->sig_info.si_pid = current->tgid;
            priv->app_pid = current->pid;
            pr_debug("Registered Pid = %d\n", priv->app_pid);
            break;

        case INIT_NETDEV:
            pr_info("Netdev init request.\n");
            rc = xp_netdev_init(priv);
            break;

        case DEINIT_NETDEV:
            pr_info("Netdev deinit request.\n");
            xp_netdev_deinit(priv);
            break;

        case PCIE_CONFIG_READ_CMD:
            pr_info(KERN_INFO "Pcie config space read request.\n");
            rc = xp_pci_conf_get(argp, priv);
            break;

        default:
            pr_err("XP80 PCI Invalid IOCTL command.\n");
            return -EINVAL;
    }

    return rc;
}

static struct file_operations xp_dev_fops = {
    .owner      = THIS_MODULE,
    .open       = xp_dev_open,
    .release    = xp_dev_close,
    .mmap       = xp_dev_mmap,
    .unlocked_ioctl = xp_dev_ioctl,
    .compat_ioctl = xp_dev_ioctl,
};

static int xp_pci_probe(struct pci_dev *pdev, const struct pci_device_id *id)
{
    char queue_name[25];
    u16 dev_id = 0;
    int rc = 0, msi_count = 0;
    u32 mmio_start = 0, mmio_len = 0;
    xp_private_t *priv = NULL;
    struct device *pci_device;
    
    static int minor;
    static int instance;

    /* Minor number is 20 bit field so max minor number can be 1048575,
     * but we have reserved 1048575 for DMA node. */
    if (WARN_ON(minor > 1048574)) {
        pr_warn("Warning : Please restart system, no more minor available.\n");
        return -EINVAL;
    }

    /* Allocate memory for private structure. */
    priv = kzalloc(sizeof(xp_private_t), GFP_KERNEL);
    if (!priv)
        return -ENOMEM;

    priv->pdev = pdev;

    rc = pci_enable_device(priv->pdev);
    if (rc) {
        pr_err("%s: pci_enable_device failed.\n", __func__);
        rc = -EFAULT;
        goto err_free_memory;
    }

    rc = pci_request_regions(priv->pdev, pci_name(priv->pdev));
    if (rc) {
        pr_err("Unable to request regions. (%s)\n", pci_name(priv->pdev));
        rc = -ENOMEM;
        goto err_req_region;
    }

    rc = pci_set_dma_mask(priv->pdev, 0xFFFFFFFF);
    if (!rc) {
        pr_debug("setConsitentDma Mask to 64\n");
        /* The query for consistent allocations is performed via a call. */
        rc = pci_set_consistent_dma_mask(priv->pdev, 0xFFFFFFFF);
        if (rc) {
            dev_err(&priv->pdev->dev, 
                    "No usable DMA configuration, aborting\n");
            goto err_dma;
        }
    } else {
        pr_debug("setConsitentDma Mask to 32\n");
        rc = pci_set_dma_mask(priv->pdev, DMA_BIT_MASK(32));
        if (!rc) {
            rc = pci_set_consistent_dma_mask(priv->pdev, DMA_BIT_MASK(32));
            if (rc) {
                dev_err(&priv->pdev->dev, 
                        "No usable DMA configuration, aborting.\n");
                goto err_dma;
            }
        }
    }
  
    /* Copy DMA mask and other details to dma private strcture. */
    if (!(xp_dma_priv->dev->coherent_dma_mask)) {
        if (priv->pdev->dev.dma_mask) {
            *(xp_dma_priv->dev->dma_mask) = *(priv->pdev->dev.dma_mask);
        }

        xp_dma_priv->dev->coherent_dma_mask = 
            priv->pdev->dev.coherent_dma_mask;

        if (priv->pdev->dev.dma_parms) {
            memcpy(xp_dma_priv->dev->dma_parms, priv->pdev->dev.dma_parms, 
                   sizeof(struct device_dma_parameters));
        }
    }

    /* Enable the Dma by setting Bus master bit in the PCI_COMMAND register. */
    pci_set_master(priv->pdev);

    if (!pci_is_pcie(priv->pdev)) {
        pr_err("Can't find PCI Express capability!\n");
        rc = -EFAULT;
        goto err_free_memory;
    }

    /* Save the PCI configuration space of a device before suspending. */
    rc = pci_save_state(priv->pdev);
    if (rc) {
        pr_err("xp80: Failed to save state.\n");
        rc = -EFAULT;
        goto err_free_memory;
    }

    pci_set_drvdata(priv->pdev, priv);

    if (isr_enable) {
        msi_count = xp_pci_enable_msi_block(priv->pdev, INTR_COUNT);
        if (msi_count < 0) {
            pr_debug("xp80: Failed to enable MSI.\n");
            priv->msi_fail = 1;
            pr_warn("Warning : BOTH MSI registration failed.\n");
        }

        if (msi_count != 0 && msi_count < INTR_COUNT) {
            rc = xp_pci_enable_msi_block(priv->pdev, msi_count);
            if (rc < 0) {
                priv->msi_fail = 1;
                pr_warn("Warning : BOTH MSI registration failed.\n");
            }
        }

        if (!(priv->msi_fail)) {
            priv->msi_count = msi_count;
            priv->irq = priv->pdev->irq;
            sprintf(queue_name, "xp_queue_%d", instance);

            /* Create Workqueue. */
            priv->w_queue = create_workqueue(queue_name);
            rc = request_irq(priv->pdev->irq, 
                             xp_msi_irq_handler_high, 0, DRIVER_NAME, priv);
            if (rc) {
                pr_err("Failed to register MSI.\n");
                rc = -ENODEV;
                goto err_free_memory;
            }

            if (!msi_count) {
                rc = request_irq(priv->pdev->irq + 1, 
                                 xp_msi_irq_handler_low, 0, DRIVER_NAME, priv);
                if (rc) {
                    pr_err("Failed to register MSI.\n");
                    rc = -ENODEV;
                }
            } else {
                pr_warn("Warning : Only High Priority MSI INTR is enabled.\n");
            }
        }
    }

    mmio_start = pci_resource_start(priv->pdev, BAR0);
    mmio_len = pci_resource_len(priv->pdev, BAR0);

    /* For A0 revision we need to map only
       256M even though BAR advertises for 512M. */
#if 0
    xpMmioLen = xpMmioLen / 2; 
#endif

    if (mmio_len) {
        pr_debug("BAR_0 Base is %x size is %d successfull.\n", 
                 mmio_start, mmio_len);
    } else {
        pr_err("BAR_0 Base is %x size is %d failed.\n", mmio_start, mmio_len);
        rc = -ENOMEM;
        goto cleanup_ports;
    }

    pr_debug("mmio_start: 0x%x  mmio_len: 0x%x\n", mmio_start, mmio_len);

    priv->vma = pci_iomap(priv->pdev, BAR0, mmio_len);
    if (!priv->vma) {
        rc = -ENOMEM;
        goto err_pci_iomap;
    }

    pr_debug("%s::%d va:%pK pa:%llx\n", 
             __FILE__, __LINE__, priv->vma, (u64)__pa(priv->vma));

    priv->mmio_start = mmio_start;
    priv->mmio_end = priv->vma + mmio_len;

    pr_info("Mapped BAR0 Address is %zx :: Mapped memLen is %d MBytes.\n", 
            (size_t)priv->vma, mmio_len / (1024 * 1024));

    /* Reading PCI register space to identify the device. */
    pci_read_config_word(priv->pdev, PCI_DEVICE_ID, &dev_id);

    if (dev_id == CNX88091_A0) {
        priv->mode = XP_A0_UNCOMPRESSED;
    } else if ((dev_id == CNX88091_B0) || (dev_id == CNX88072_B0)) {
        /* Verifying between compress and decompress mode. */
        if (64 == mmio_len / (1024 * 1024)) {
            priv->mode = XP_B0_COMPRESSED;
        } else {
            priv->mode = XP_B0_UNCOMPRESSED;
        }
    } else {
        pr_debug(KERN_ERR "xp80: Failed to read configuration space.\n");
        rc = -EFAULT;
        goto err_free_memory;
    }

    cdev_init(&priv->cdev, &xp_dev_fops);
    priv->cdev.owner = THIS_MODULE;

    rc = cdev_add(&priv->cdev, MKDEV(xp_pcidev_major, minor), 1);
    if (rc)
        goto err_cdev_add;

    pci_device = device_create(xp_pci_class, NULL, 
                               MKDEV(xp_pcidev_major, minor), 
                                     NULL, "%s%u", "xppcidev", minor);
    if (!pci_device)
        goto err_device_create;

    pr_debug("Device : %s%u created with Major : %d and Minor: %u", 
             "xppcidev", minor, xp_pcidev_major, minor);

    priv->sig_info.si_signo = XP_RT_SIGNAL;
    priv->sig_info.si_errno = 0;
    priv->sig_info.si_code  = SI_QUEUE;
    priv->sig_info.si_pid   = current->tgid;
    priv->sig_info.si_int   = 0;

    priv->app_pid = current->pid;
    priv->xpnet = 0;

    spin_lock_init(&priv->tx_dma_read_lock);

    memset(priv->proc_fs_name, 0, sizeof(priv->proc_fs_name));
    snprintf(priv->proc_fs_name, sizeof(priv->proc_fs_name) - 1, "%s%u",
             XPREG_PROC_FILE_NAME, minor);
    priv->reg_proc = xpreg_proc_create(priv->proc_fs_name, NULL, priv);
    if(NULL == priv->reg_proc){
        pr_err("XP: xpreg_proc_create failed.\n");
    }

    if (minor < MAX_DEV_SUPPORTED) {
        xp_pcie_dev_ptr[minor] = priv;
    }

    /* For each device there should be new workqueue instance. */
    instance++;
    minor++;

    pr_info("XP: Pcie Slave Probe successful.\n");
    return 0;


err_device_create:
    cdev_del(&priv->cdev);

err_cdev_add:
    pci_iounmap(priv->pdev, priv->vma);

err_pci_iomap:
    pci_release_region(priv->pdev, BAR0);

cleanup_ports:
    release_region(mmio_start, mmio_len);

err_dma:
err_req_region:
    pci_disable_device(priv->pdev);

err_free_memory:
    if (priv && priv->w_queue) {
        flush_workqueue(priv->w_queue);
        destroy_workqueue(priv->w_queue);
        kfree(priv);
    }

    pr_err("Failed to probe.\n");
    return rc;
}

static void xp_pci_remove(struct pci_dev *pdev)
{
    xp_private_t *priv = pci_get_drvdata(pdev);
    int minor =  MINOR(priv->cdev.dev);

    xp_netdev_deinit(priv);
    device_destroy(xp_pci_class, 
                   MKDEV(xp_pcidev_major, MINOR(priv->cdev.dev)));
    cdev_del(&priv->cdev);

    /* Release region. */
    pci_iounmap(pdev, priv->vma);
    pci_release_region(priv->pdev, BAR0);

    /* No need to check isr_enable value. */
    if (isr_enable && !(priv->msi_fail)) {
        /* Free both irq. */
        free_irq(pdev->irq, priv);

        if (!priv->msi_count)
            free_irq(pdev->irq + 1, priv);

        /* Msi disable. */
        pci_disable_msi(pdev);

        /* Flush and destroy workqueue. */
        flush_workqueue(priv->w_queue);
        destroy_workqueue(priv->w_queue);
    }

    if(NULL != priv->reg_proc){
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
	    proc_remove(priv->reg_proc);
#else
	    remove_proc_entry(priv->proc_fs_name, NULL);
#endif
    }

    kfree(priv);

    /* PCIe disable device. */
    pci_disable_device(pdev);
    unregister_chrdev(xp_pcidev_major, DRIVER_NAME);

    if (minor < MAX_DEV_SUPPORTED) {
        xp_pcie_dev_ptr[minor] = NULL;
    }

    pr_info("XP: Pci module remove successful.\n");
}

static struct pci_device_id xp_pci_ids[SUPPORTED_DEVICE_IDS + 2] = {
    { PCI_DEVICE(VENDOR_ID, CNX88091_A0), },
    { PCI_DEVICE(VENDOR_ID, CNX88091_B0), },
    { PCI_DEVICE(VENDOR_ID, CNX88072_B0), },
    { PCI_DEVICE(VENDOR_ID, CNX88061_B0), },
    { PCI_DEVICE(VENDOR_ID, CNX88071_B0), },

    /* Reserved for command line argument */
    { PCI_DEVICE(VENDOR_ID, 0),           },
    { 0,                                  }
};

static struct pci_driver xp_pci_driver = {
    .name     = DRIVER_NAME,
    .probe    = xp_pci_probe,
    .id_table = xp_pci_ids,
    .remove   = xp_pci_remove,
};

static int __init xp_module_init(void)
{
    int rc = 0, i = 0, update_pci_table = 0;

    pr_info("XP80 PCIe End Point Driver (version: %s).\n", DRIVER_VERSION),
    pr_info("Copyright(c) 2013 Xpliant INC.\n");

    /* Create sys class */
    xp_pci_class = class_create(THIS_MODULE, "xppci");
    if (!xp_pci_class) {
        pr_debug("xpPcie driver : Error in creating class.\n\n");
        return -ENOMEM;
    }

    /* Register char device, allocate major number. */
    if (alloc_chrdev_region(&xp_dev, 0, XP_PCI_MAX_ADAPTERS, "xpPCI")) {
        pr_debug("xpPcie driver : Error in allocating dev_t.\n");
        rc = -EBUSY;
        goto err_alloc_chrdev_region;
    }

    xp_pcidev_major = MAJOR(xp_dev);
    pr_info("XP: Pcie Slave Driver: MAJOR NUMBER:= %d\n", xp_pcidev_major);

    /* Allocate memory for private structure of DMA allocator. */
    xp_dma_priv = kzalloc(sizeof(xp_dma_private_t), GFP_KERNEL);
    if (!xp_dma_priv)
        goto err_dma_prv_alloc;

    cdev_init(&xp_dma_priv->cdev, &xp_dma_fops);
    xp_dma_priv->cdev.owner = THIS_MODULE;

    if (cdev_add(&xp_dma_priv->cdev, 
                 MKDEV(xp_pcidev_major, XP_DMA_NODE_MINOR), 1))
        goto err_cdev_dma_add;

    xp_dma_priv->dev = device_create(xp_pci_class, NULL, 
        MKDEV(xp_pcidev_major, XP_DMA_NODE_MINOR), NULL, "%s", "xpdmamem");

    if (!xp_dma_priv->dev)
        goto err_device_create_dma;

    pr_debug("Device : %s created with Major : %d and Minor: %u", 
             "xpdmamem", xp_pcidev_major, XP_DMA_NODE_MINOR);

    /* It will be updated by probe of first device. */
    xp_dma_priv->dev->coherent_dma_mask = 0;

    xp_dma_priv->dev->dma_mask = kzalloc(sizeof(u64), GFP_KERNEL);
    if (!(xp_dma_priv->dev->dma_mask))
        goto err_dma_mask_alloc;

    xp_dma_priv->dev->dma_parms = 
        kzalloc(sizeof(struct device_dma_parameters), GFP_KERNEL);
    if (!(xp_dma_priv->dev->dma_parms))
        goto err_dma_parms_alloc;


    if (dyn_pcie_dev_id) {
        /* Add device Id supplied by user in PCIe table. */
        update_pci_table = 1;

        /* Check existing device id list. */
        for (i = 0; i < SUPPORTED_DEVICE_IDS; i++) {
            if (xp_pci_ids[i].device == dyn_pcie_dev_id) {
                pr_debug("Device ID : 0x%x is already part of PCIe table.\n", 
                         dyn_pcie_dev_id);
                update_pci_table = 0; /* Already present in PCIe table. */
            }
        }
    }

    if (update_pci_table) {
        xp_pci_ids[SUPPORTED_DEVICE_IDS].device = dyn_pcie_dev_id;
        pr_debug("Added 0x%x as part of PCIe table.\n", dyn_pcie_dev_id);
    } else { /* Make invalid entry at SUPPORTED_DEVICE_IDS index. */
        memset(&(xp_pci_ids[SUPPORTED_DEVICE_IDS]), 
               0x00, sizeof(struct pci_device_id));
        pr_debug("Reset %d index of PCIe table.\n", SUPPORTED_DEVICE_IDS);
    }

    rc = pci_register_driver(&xp_pci_driver);
    if (rc < 0) {
        pr_err("PCI registration failed.\n");
        goto err_pci_driver_register;
    }

    return 0;


err_pci_driver_register:
    kfree(xp_dma_priv->dev->dma_parms);

err_dma_parms_alloc:
    kfree(xp_dma_priv->dev->dma_mask);

err_dma_mask_alloc:
    device_destroy(xp_pci_class, 
                   MKDEV(xp_pcidev_major, MINOR(xp_dma_priv->cdev.dev)));

err_device_create_dma:
    cdev_del(&xp_dma_priv->cdev);

err_cdev_dma_add:
    kfree(xp_dma_priv);
    xp_dma_priv = NULL;

err_dma_prv_alloc:
    unregister_chrdev_region(xp_dev, XP_PCI_MAX_ADAPTERS);

err_alloc_chrdev_region:
    class_destroy(xp_pci_class);
    return rc;
}

static void __exit xp_module_exit(void)
{
    if (xp_dma_priv) {
        /* Cleanup allocated DMA memory, give it back to kernel. */
        if (xp_dma_priv->dev->coherent_dma_mask)
            xp_dma_mem_cleanup(xp_dma_priv);

        kfree(xp_dma_priv->dev->dma_mask);
        kfree(xp_dma_priv->dev->dma_parms);

        device_destroy(xp_pci_class, MKDEV(xp_pcidev_major, 
                                           MINOR(xp_dma_priv->cdev.dev)));
        cdev_del(&xp_dma_priv->cdev);

        kfree(xp_dma_priv);
        xp_dma_priv = NULL;

        pci_unregister_driver(&xp_pci_driver);
        unregister_chrdev_region(xp_dev, XP_PCI_MAX_ADAPTERS);
        class_destroy(xp_pci_class);
    }

    pr_info("XP80 PCIe Unregistered successful.\n");
}

s32 xp_pci_drv_write_reg(int dev, u32 rw_value,
               u32 reg_addr, u8 reg_size)
{
    if (dev >= MAX_DEV_SUPPORTED) {
        return -1;
    }
    if (NULL == xp_pcie_dev_ptr[dev]) {
        return -1;
    }
    return xp_dev_reg_write(rw_value, reg_addr,
                reg_size, xp_pcie_dev_ptr[dev]);
}

s32 xp_pci_drv_read_reg(int dev, u32 *rw_value,
               u32 reg_addr, u8 reg_size)
{
    if (dev >= MAX_DEV_SUPPORTED) {
        return -1;
    }
    if (NULL == xp_pcie_dev_ptr[dev]) {
        return -1;
    }

    return xp_dev_reg_read(rw_value, reg_addr,
            reg_size, xp_pcie_dev_ptr[dev]);
}

u32 xp_get_reg_addr_by_id(int dev, s32 reg_id) {
    if (dev >= MAX_DEV_SUPPORTED) {
        return -1;
    }
    if (NULL == xp_pcie_dev_ptr[dev]) {
        return -1;
    }

    return XP_GET_PCI_BASE_OFFSET_FROM_REG_NAME(reg_id,
            xp_pcie_dev_ptr[dev]->mode);
}

int xp_packet_send(int dev, struct sk_buff *skb) {
    if (dev >= MAX_DEV_SUPPORTED) {
        return -1;
    }
    if (NULL == xp_pcie_dev_ptr[dev]) {
        return -1;
    }
    if (! xp_pcie_dev_ptr[dev]->xpnet) {
        xp_netdev_init(xp_pcie_dev_ptr[dev]);
    }
    if (! xp_pcie_dev_ptr[dev]->xpnet) {
        printk("Net probe error...\n");
        return -1;
    }
    if (0 != xpnet_start_xmit(skb, xp_pcie_dev_ptr[dev]->xpnet)) {
        printk("xmit failed DMA is busy...\n");
        return -1;
    }
    return 0;
}

EXPORT_SYMBOL(xp_pci_drv_write_reg);
EXPORT_SYMBOL(xp_pci_drv_read_reg);
EXPORT_SYMBOL(xp_get_reg_addr_by_id);
EXPORT_SYMBOL(xp_packet_send);

module_init(xp_module_init);
module_exit(xp_module_exit);
module_param(dyn_pcie_dev_id, int, 0);
module_param(isr_enable, bool, 0);

MODULE_AUTHOR("Xpliant INC Confidential.");
MODULE_DESCRIPTION("User mode PCIe Endpoint device interface.");
MODULE_LICENSE("GPL");
MODULE_VERSION(DRIVER_VERSION);
MODULE_ALIAS("pcie:pciedev");
MODULE_DEVICE_TABLE(pci, xp_pci_ids);

