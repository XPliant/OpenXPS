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
* File: xp_common.h
* 
* Abstract: This file contains the macros and definitions required.
************************************************************************/

#ifndef _XP_COMMON_H
#define _XP_COMMON_H

#include <linux/types.h>
#include "xp_netdev.h"

#define BYTE_SIZE   1
#define WORD_SIZE   2
#define DWORD_SIZE  4
#define QWORD_SIZE  8

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

