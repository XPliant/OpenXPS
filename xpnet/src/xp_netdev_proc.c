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
* File: xp_netdev_proc.c
* 
* Abstract: This file contains the netdev message processing implementation for 
* xpliant.
************************************************************************/
#include "xp_common.h"
#include "xp_netdev.h"

#define XP_DBG_GLOBAL        (1 << 0)
#define XP_DBG_FP            (1 << 1)
#define XP_DBG_NETDEV_PRINT  (1 << 2)
#define XP_DBG_TX_HDR_PRINT  (1 << 3)

#define DEBUG_BUF_SIZE 256

char debug_buffer[DEBUG_BUF_SIZE];
int g_debug = 0;
xpnet_que_info_t g_net_que_info[XPNET_NUM_QUEUES];

extern int xp_trap_table_print(struct seq_file *sf);
extern void xp_debug_set(int mode);
extern void xp_netdev_print(struct seq_file *sf);
extern void xp_netdev_tx_header_print(unsigned int xpnet_intf_id,
                                      struct seq_file *sf);

static int sprint_descriptor(char *buf, int bufsize, void *p, char *msg)
{
    int i = 0;
    int chars = 0;
    char *fmt = "[%01d]=%#016llx ";
    xpnet_descriptor_t *desc = (xpnet_descriptor_t *)p;

    if (bufsize < 18) {
        return 0;
    }

    chars += sprintf(buf + chars, "(%p):%s", desc, msg);
    for (i = 0; i < 4; i++) {
        if (chars < (bufsize - (23 + 1))) {
            chars += sprintf(buf + chars, fmt, i, desc->qword[i]);
        } else {
            chars += sprintf(buf + chars, "No space...");
        }
    }

    return chars;
}

static void dma_stats_dump(xpnet_private_t *net_priv, struct seq_file *sf)
{
    seq_printf(sf, "\nRX-TX DMA statstastic\n");
    seq_printf(sf, "----------------------------------------------------\n");
    seq_printf(sf, "RX packets:%ld errors:%ld dropped:%ld\n",
               net_priv->stats.rx_packets, net_priv->stats.rx_length_errors, 
               net_priv->stats.rx_dropped);
    seq_printf(sf, "TX packets:%ld errors:%ld dropped:%ld\n", 
               net_priv->stats.tx_packets, net_priv->stats.tx_aborted_errors +
               net_priv->stats.tx_errors, net_priv->stats.tx_dropped);
    seq_printf(sf, "RX bytes:%ld   TX bytes:%ld\n", 
               net_priv->stats.rx_bytes, net_priv->stats.tx_bytes);
}

static void common_registers_dump(xpnet_private_t *net_priv, 
                                  struct seq_file *sf)
{
    u32 regval;
    u32 reg;
    int qno = 0;

    seq_printf(sf, "DMA 0 registers\n");
    seq_printf(sf, "----------------------------------------------------\n");
    reg = MGMT_CTRL_REG_E;
    xp_dev_reg_read_q(net_priv, reg, 1, (u32 *)&regval, qno);
    seq_printf(sf, "MGMT_CTRL_REG_E "
                   "= %#010x\n", regval);
    reg = CORE_CTRL_REG__1_E;
    xp_dev_reg_read_q(net_priv, reg, 1, (u32 *)&regval, qno);
    seq_printf(sf, "CORE_CTRL_REG__1_E "
                   "= %#010x\n", regval);
    reg = TX_DMA0_RETRY_CNT_REG_E;
    xp_dev_reg_read_q(net_priv, reg, 1, (u32 *)&regval, qno);
    seq_printf(sf, "XP_MGMT_LOCAL_REG_TX_DMA0_RETRY_CNT_REG "
                   "= %#010x\n", regval);
    reg = DMA0_CLR_ERR_CNT_REG_E;
    xp_dev_reg_read_q(net_priv, reg, 1, (u32 *)&regval, qno);
    seq_printf(sf, "DMA0_CLR_ERR_CNT_REG "
                   "= %#010x\n", regval);
    reg = DMA0_TX_CHAIN_LEN_ERR_REG_E;
    xp_dev_reg_read_q(net_priv, reg, 1, (u32 *)&regval, qno);
    seq_printf(sf, "DMA0_TX_CHAIN_LEN_ERR_REG "
                   "= %#010x\n", regval);
    reg = DMA0_TX_CPU_OWN_DESC_ERR_REG_E;
    xp_dev_reg_read_q(net_priv, reg, 1, (u32 *)&regval, qno);
    seq_printf(sf, "DMA0_TX_CPU_OWN_DESC_ERR_REG "
                   "= %#010x\n", regval);
    reg = DMA0_TX_ZERO_BUF_LEN_ERR_REG_E;
    xp_dev_reg_read_q(net_priv, reg, 1, (u32 *)&regval, qno);
    seq_printf(sf, "DMA0_TX_ZERO_BUF_LEN_ERR_REG "
                   "= %#010x\n", regval);
    reg = DMA0_TX_PCIE_ERR_REG_E;
    xp_dev_reg_read_q(net_priv, reg, 1, (u32 *)&regval, qno);
    seq_printf(sf, "DMA0_TX_PCIE_ERR_REG "
                   "= %#010x\n", regval);
    reg = DMA0_TX_DMA_INTF_ERR_REG_E;
    xp_dev_reg_read_q(net_priv, reg, 1, (u32 *)&regval, qno);
    seq_printf(sf, "DMA0_TX_DMA_INTF_ERR_REG "
                   "= %#010x\n", regval);
    reg = DMA0_TX_PKT_DROP_E;
    xp_dev_reg_read_q(net_priv, reg, 1, (u32 *)&regval, qno);
    seq_printf(sf, "DMA0_TX_PKT_DROP "
                   "= %#010x\n", regval);
    reg = TX_DMA0_CFG_REGLOCKREG_E;
    xp_dev_reg_read_q(net_priv, reg, 1, (u32 *)&regval, qno);
    seq_printf(sf, "XP_MGMT_TX_DMA0_CFG_REGLOCKREG "
                   "= %#010x\n", regval);
    reg = TX_DMA0_SCRATCHPAD_E;
    xp_dev_reg_read_q(net_priv, reg, 1, (u32 *)&regval, qno);
    seq_printf(sf, "TX_DMA0_SCRATCHPAD "
                   "= %#010x\n", regval);
    reg = DMA0_RX_CHAIN_LEN_ERR_REG_E;
    xp_dev_reg_read_q(net_priv, reg, 1, (u32 *)&regval, qno);
    seq_printf(sf, "DMA0_RX_CHAIN_LEN_ERR_REG "
                   "= %#010x\n", regval);
    reg = DMA0_RX_CPU_OWN_DESC_ERR_REG_E;
    xp_dev_reg_read_q(net_priv, reg, 1, (u32 *)&regval, qno);
    seq_printf(sf, "DMA0_RX_CPU_OWN_DESC_ERR_REG "
                   "= %#010x\n", regval);
    reg = DMA0_RX_ZERO_BUF_LEN_ERR_REG_E;
    xp_dev_reg_read_q(net_priv, reg, 1, (u32 *)&regval, qno);
    seq_printf(sf, "DMA0_RX_ZERO_BUF_LEN_ERR_REG "
                   "= %#010x\n", regval);
    reg = DMA0_RX_PCIE_ERR_REG_E;
    xp_dev_reg_read_q(net_priv, reg, 1, (u32 *)&regval, qno);
    seq_printf(sf, "DMA0_RX_PCIE_ERR_REG "
                   "= %#010x\n", regval);
    reg = RX_DMA0_CFG_REGLOCKREG_E;
    xp_dev_reg_read_q(net_priv, reg,1,  (u32 *)&regval, qno);
    seq_printf(sf, "XP_MGMT_RX_DMA0_CFG_REGLOCKREG "
                   "= %#010x\n", regval);
    reg = RX_DMA0_SCRATCHPAD_E;
    xp_dev_reg_read_q(net_priv, reg, 1, (u32 *)&regval, qno);
    seq_printf(sf, "RX_DMA0_SCRATCHPAD "
                   "= %#010x\n", regval);
}

static void internal_qstate_dump(xpnet_private_t *net_priv, 
                                 int qno, struct seq_file *sf)
{
    xpnet_queue_struct_t *rq = NULL, *tq = NULL;
    char *type = NULL;

    if (qno < XPNET_RX_NUM_QUEUES) {
        rq = &net_priv->rx_queue[qno];
    }

    if (qno < XPNET_TX_NUM_QUEUES) {
        tq = &net_priv->tx_queue[qno];
    }

    type = "\nUPSTREAM-RX(host)";
    seq_printf(sf, "%s\n", type);
    seq_printf(sf, "----------------------------------------------------\n");
    if (rq) {
        seq_printf(sf, "xpq_id = %d\n", rq->xpq_id);
        seq_printf(sf, "xpq_num_desc = %d\n", rq->xpq_num_desc);
        seq_printf(sf, "status = %d\n", rq->status);
        seq_printf(sf, "va = %p, dma = %pad\n", rq->va, &rq->dma);
    }

    type = "\nDSTREAM-TX(host)";
    seq_printf(sf, "%s\n", type);
    seq_printf(sf, "----------------------------------------------------\n");
    if (tq) {
        seq_printf(sf, "xpq_id = %d\n", tq->xpq_id);
        seq_printf(sf, "xpq_num_desc = %d\n", tq->xpq_num_desc);
        seq_printf(sf, "status = %d\n", tq->status);
        seq_printf(sf, "head %d, tail %d\n", tq->head, tq->tail);
        seq_printf(sf, "free desc %d, busy %d\n",
                   xpnet_get_free_desc(tq), xpnet_get_busy_desc(tq));
        seq_printf(sf, "va = %p, dma = %pad\n", tq->va, &tq->dma);
    }
}

static void queue_registers_dump(xpnet_private_t *net_priv, int qno,
                                 struct seq_file *sf)
{
    char *type;
    u64 regval = 0;
    u32 reg;

    type = "UPSTREAM-RX(host) registers";
    seq_printf(sf, "\n%s\n", type);
    seq_printf(sf, "----------------------------------------------------\n");
    if(qno <= XPNET_TX_NUM_QUEUES){
        reg = DMA0_TX_CDP_REG_E;
        xp_dev_reg_read_q(net_priv, reg, 2, (u32 *)&regval, qno);
        seq_printf(sf, "DMA0_TX_CDP_REG[%d] "
                       "= %#016llx\n", qno, regval);
    
        reg = DMA0_TX_CMD_REG_E;
        regval = 0;
        xp_dev_reg_read_q(net_priv, reg, 1, (u32 *)&regval, qno);
        seq_printf(sf, "DMA0_TX_CMD_REG[%d] "
        "= %#016llx\n", qno, regval);
    }
   
    type = "DSTREAM-TX(host) registers";
    seq_printf(sf, "\n%s\n", type);
    seq_printf(sf, "----------------------------------------------------\n");
    if(qno <= XPNET_RX_NUM_QUEUES){
        reg = DMA0_RX_CDP_REG_E;
        regval = 0;
        xp_dev_reg_read_q(net_priv, reg, 2, (u32 *)&regval, qno);
        seq_printf(sf, "DMA0_RX_CDP_REG[%d] "
                       "= %#016llx\n", qno, regval);
    
        reg = DMA0_RX_CMD_REG_E;
        regval = 0;
        xp_dev_reg_read_q(net_priv, reg, 1, (u32 *)&regval, qno);
        seq_printf(sf, "DMA0_RX_CMD_REG[%d] "
        "= %#016llx\n", qno, regval);
    }
}

static void descriptors_dump(xpnet_private_t *net_priv, char *dest_buf,
                             int dblen, int qno, xpnet_enum_t rxtx,
                             struct seq_file *sf)
{
    xpnet_descriptor_t *d;
    xpnet_queue_struct_t *q;
    char *type;
    char local[64];
    int i;

    if (rxtx == XPNET_QUEUE_TYPE_TX) {
        q = (qno >= XPNET_TX_NUM_QUEUES) ? NULL : &net_priv->tx_queue[qno];
        type = "DSTREAM-TX(host)";
    } else {
        q = (qno >= XPNET_RX_NUM_QUEUES) ? NULL : &net_priv->rx_queue[qno];
        type = "UPSTREAM-RX(host)";
    }

    if (q == NULL) {
        sprintf(dest_buf, "Invalid queue %d", qno);
        return;
    }

    seq_printf(sf, "\n%s queue[%d] : #of desc = %d, status = %d\n",
               type, qno, q->xpq_num_desc, q->status);

    if (q->xpq_type == XPNET_QUEUE_TYPE_TX) {
        seq_printf(sf, "head = %d, tail = %d\n", q->head, q->tail);
    }
    seq_printf(sf, "----------------------------------------------------\n");

    for (i = 0; i < q->xpq_num_desc; i++) {
        d = q->xpq_desc_meta[i].va;
        sprintf(local, "%sDESC[%d] : ", type, i);
        sprint_descriptor(dest_buf, dblen, d, local);
        seq_printf(sf, "%s\n", dest_buf);
    }
}

static int xpnet_seq_show_header(xpnet_private_t *priv, struct seq_file *sf)
{
    seq_printf(sf, "DMA queue information\n");
    seq_printf(sf, "----------------------------------------------------\n");
    seq_printf(sf, "hw_flags = %#x\n", priv->hw_flags);
    seq_printf(sf, "Base txqno = %d \nBase rxqno = %d\n",
               priv->txqno, priv->rxqno);
    seq_printf(sf, "Number of txqueues = %d\nNumber of rxqueues = %d\n",
               priv->num_txqueues, priv->num_rxqueues);

    return 0;
}

static int xpnet_que_seq_show(struct seq_file *sf, void *v)
{
    xpnet_que_info_t *que_info = NULL;
    xpnet_private_t *priv = NULL;
    int que_no;

    que_info = sf->private;
    priv = que_info->priv;
    que_no = que_info->que_no;

    xpnet_seq_show_header(priv, sf);
    common_registers_dump(priv, sf);
    queue_registers_dump(priv, que_no, sf);
    internal_qstate_dump(priv, que_no, sf);
    descriptors_dump(priv, debug_buffer,sizeof(debug_buffer),
                     que_no, XPNET_QUEUE_TYPE_RX, sf);
    descriptors_dump(priv, debug_buffer,sizeof(debug_buffer),
                     que_no, XPNET_QUEUE_TYPE_TX, sf);

    return 0;
}

static int xpnet_que_seq_open(struct inode *inode, struct file *file)
{
    struct seq_file *s;
    xpnet_que_info_t *que_info = NULL;
    int result;

    result = single_open(file, xpnet_que_seq_show, NULL);

    s = (struct seq_file *)file->private_data;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
    que_info = PDE_DATA(file_inode(file));
#else
    que_info = PROC_I(inode)->pde->data;
#endif
    s->private = que_info;

    return result;

}

static ssize_t xpnet_que_proc_write(struct file *filp, const char *buf,
                                size_t bufsize, loff_t * off)
{
    return bufsize;
}

static const struct file_operations xpnet_proc_fops = {
    .owner = THIS_MODULE,
    .open = xpnet_que_seq_open,
    .read = seq_read,
    .write = xpnet_que_proc_write,
    .llseek = seq_lseek,
    .release = single_release,
};

static int xpnet_stats_seq_show(struct seq_file *sf, void *v)
{
    xpnet_private_t *priv = NULL;

    priv = sf->private;

    xpnet_seq_show_header(priv, sf);
    dma_stats_dump(priv, sf);
    
    return 0;
}

static int xpnet_stats_seq_open(struct inode *inode, struct file *file)
{
    struct seq_file *s;
    xpnet_private_t  *xp_net_priv = NULL;
    int result;

    result = single_open(file, xpnet_stats_seq_show, NULL);

    s = (struct seq_file *)file->private_data;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
    xp_net_priv = PDE_DATA(file_inode(file));
#else
    xp_net_priv = PROC_I(inode)->pde->data;
#endif
    s->private = xp_net_priv;

    return result;
}

static ssize_t xpnet_stats_proc_write(struct file *filp, const char *buf,
                                size_t bufsize, loff_t * off)
{
    return bufsize;
}


static const struct file_operations xpnet_proc_stats_fops = {
    .owner = THIS_MODULE,
    .open = xpnet_stats_seq_open,
    .read = seq_read,
    .write = xpnet_stats_proc_write,
    .llseek = seq_lseek,
    .release = single_release,
};

static int xpnet_ttable_seq_show(struct seq_file *sf, void *v)
{
    xp_trap_table_print(sf);
    
    return 0;
}

static int xpnet_ttable_seq_open(struct inode *inode, struct file *file)
{
    return  single_open(file, xpnet_ttable_seq_show, NULL);
}

static ssize_t xpnet_ttable_proc_write(struct file *filp, const char *buf,
                                size_t bufsize, loff_t * off)
{
    return bufsize;
}

static const struct file_operations xpnet_proc_ttable_fops = {
    .owner = THIS_MODULE,
    .open = xpnet_ttable_seq_open,
    .read = seq_read,
    .write = xpnet_ttable_proc_write,
    .llseek = seq_lseek,
    .release = single_release,
};

static int xpnet_debug_seq_show(struct seq_file *sf, void *v)
{
    seq_printf(sf, "Debug level = (%d)\n", g_debug);

    return 0;
}

static int xpnet_debug_seq_open(struct inode *inode, struct file *file)
{
    return single_open(file, xpnet_debug_seq_show, NULL);
}

static ssize_t xpnet_debug_proc_write(struct file *filp, const char *buf,
                                size_t bufsize, loff_t * off)
{
    u32 val;

    fdebug("Entering %s()\n", __func__);
    fdebug("Called xpnet_write(), %zu\n", bufsize);

    if (bufsize != 0) {
        if (1 == sscanf(buf, "%u", &val)) {
            g_debug = val & XP_DBG_GLOBAL;
            xp_debug_set(val & XP_DBG_FP);
        }
    }
    return bufsize;
}


static const struct file_operations xpnet_proc_debug_fops = {
    .owner = THIS_MODULE,
    .open = xpnet_debug_seq_open,
    .read = seq_read,
    .write = xpnet_debug_proc_write,
    .llseek = seq_lseek,
    .release = single_release,
};

static int xpnet_netdev_seq_show(struct seq_file *sf, void *v)
{
    xp_netdev_print(sf);

    return 0;
}

static int xpnet_netdev_seq_open(struct inode *inode, struct file *file)
{
    return single_open(file, xpnet_netdev_seq_show, NULL);
}

static ssize_t xpnet_netdev_proc_write(struct file *filp, const char *buf,
                                size_t bufsize, loff_t * off)
{
    return bufsize;
}

static const struct file_operations xpnet_proc_netdev_fops = {
    .owner = THIS_MODULE,
    .open = xpnet_netdev_seq_open,
    .read = seq_read,
    .write = xpnet_netdev_proc_write,
    .llseek = seq_lseek,
    .release = single_release,
};

static int xpnet_txhdr_seq_show(struct seq_file *sf, void *v)
{
    xp_netdev_tx_header_print(0, sf);

    return 0;
}

static int xpnet_txhdr_seq_open(struct inode *inode, struct file *file)
{
    return single_open(file, xpnet_txhdr_seq_show, NULL);
}

static ssize_t xpnet_txhdr_proc_write(struct file *filp, const char *buf,
                                size_t bufsize, loff_t * off)
{
    return bufsize;
}

static const struct file_operations xpnet_proc_txhdr_fops = {
    .owner = THIS_MODULE,
    .open = xpnet_txhdr_seq_open,
    .read = seq_read,
    .write = xpnet_txhdr_proc_write,
    .llseek = seq_lseek,
    .release = single_release,
};

int xpnet_proc_create(xpnet_private_t *net_priv)
{
    int count = 0;
    char queue_name[25];

    fdebug("Entering %s()\n", __func__);
    
    memset(queue_name, 0, sizeof(queue_name));
    snprintf(queue_name, sizeof(queue_name) - 1, "xpnet%d", net_priv->instance);
    net_priv->proc_root = proc_mkdir(queue_name, NULL);;

    for (count = 0; count < XPNET_NUM_QUEUES; count++) {
        g_net_que_info[count].que_no = count;
        g_net_que_info[count].priv = g_net_priv;

        memset(queue_name, 0, sizeof(queue_name));
        snprintf(queue_name, sizeof(queue_name) - 1, "queue%d", count);
        net_priv->proc_que[count] = proc_create_data(queue_name, 0644, 
                                    net_priv->proc_root, &xpnet_proc_fops,
                                    &g_net_que_info[count]);
        if (NULL == net_priv->proc_que[count]) {
            return -1;
        }
    }
 
    net_priv->proc_stats = proc_create_data(XPNET_PROC_STATS, 0644,
                           net_priv->proc_root, &xpnet_proc_stats_fops,
                           g_net_priv);
    if (NULL == net_priv->proc_stats) {
        return -1;
    }
    
    net_priv->proc_ttable = proc_create(XPNET_PROC_TRAP_TABLE, 0644,
                            net_priv->proc_root, &xpnet_proc_ttable_fops);
    if (NULL == net_priv->proc_ttable) {
       return -1;
    }
 
    net_priv->proc_debug = proc_create(XPNET_PROC_DEBUG, 0644,
                            net_priv->proc_root, &xpnet_proc_debug_fops);
    if (NULL == net_priv->proc_debug) {
       return -1;
    }
 
    net_priv->proc_netdev = proc_create(XPNET_PROC_NETDEV, 0644,
                            net_priv->proc_root, &xpnet_proc_netdev_fops);
    if (NULL == net_priv->proc_netdev) {
       return -1;
    }
    net_priv->proc_txhdr = proc_create(XPNET_PROC_TXHDR, 0644,
                            net_priv->proc_root, &xpnet_proc_txhdr_fops);
    if (NULL == net_priv->proc_txhdr) {
       return -1;
    }
    
    return 0;
}

