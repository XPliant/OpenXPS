/************************************************************************/
/*  Copyright (c) [2016] Cavium, Inc. All rights reserved.              */
/*  Unpublished - rights reserved under the Copyright Laws of the       */
/*  United States.  Use, duplication, or disclosure by the              */
/*  Government is subject to restrictions as set forth in               */
/*  subparagraph (c)(1)(ii) of the Rights in Technical Data and         */
/*  Computer Software clause at 252.227-7013.                           */
/************************************************************************/

/*
 * This software is licensed to you under the terms of the GNU General Public
 * License version 2 (the "GPL"). 
 * TBD: need to update the GPL banner from Cavium Legal .
 */

#ifndef _XP_COMMON_H
#define _XP_COMMON_H

#include <linux/types.h>
#include "xp_netdev.h"

#define BYTE_SIZE   1
#define WORD_SIZE   2
#define DWORD_SIZE  4

#define ERR(...) printk(KERN_ERR "! "__VA_ARGS__)
#define LOG(...) printk(KERN_INFO "! "__VA_ARGS__)
#define DBG(...) if (xp_debug != 0) printk(KERN_INFO "! "__VA_ARGS__)

#define fdebug(fmt,args...)\
({\
    if (g_debug != 0) {\
        printk(KERN_CRIT "%s:%d" fmt, __FILE__, __LINE__, ##args);\
    }\
})

#define pdebug(fmt,args...)\
({\
    if (g_debug != 0) {\
        if (printk_ratelimit()) printk(KERN_CRIT fmt, ##args);\
    }\
})

#define pdebug_nr(fmt,args...)\
({\
    if (g_debug != 0) {\
        printk(KERN_CRIT fmt, ##args);\
    }\
})

extern int g_debug;

#endif /* _XP_COMMON_H */

