
#ifndef __NAND_DEV_H__
#define __NAND_DEV_H__

#include "nand_blk.h"

struct nand_kobject{
    struct kobject           kobj;
    struct _nftl_blk*        nftl_blk;
    char name[32];
};

extern unsigned int do_static_wear_leveling(void* zone);
extern unsigned short nftl_get_zone_write_cache_nums(void * _zone);
extern unsigned int  garbage_collect(void* zone);
extern unsigned int  do_prio_gc(void* zone);
extern int lock_all_blk(void);
extern int unlock_all_blk(void);
extern int critical_dev_nand_write(uchar *dev_name, __u32 start_sector,
		__u32 sector_num, unsigned char *buf);
extern int critical_dev_nand_read(uchar *dev_name, __u32 start_sector,
		__u32 sector_num, uchar *buf);

#endif
