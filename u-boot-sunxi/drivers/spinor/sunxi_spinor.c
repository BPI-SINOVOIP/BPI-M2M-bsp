/*
 * (C) Copyright 2007-2013
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * Jerry Wang <wangflord@allwinnertech.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <common.h>
#include <malloc.h>
#include <spi.h>
#include <asm/arch/spi.h>
#include <sunxi_mbr.h>
#include <private_boot0.h>
#include <sys_config.h>

static void spinor_enter_4bytes_addr(int);
#define ENABLE_4BYTES	1
#define DISABLE_4BYTES  0
static int 	 spinor_4bytes_addr_mode = 0;
static int   spinor_flash_inited = 0;
uint  total_write_bytes;
#ifndef CONFIG_SMALL_MEMSIZE
static char *spinor_store_buffer;
#else
static int first_write = 1;
#endif
//static char  spinor_mbr[SUNXI_MBR_SIZE];
static char *spinor_write_cache;
static int   spinor_cache_block = -1;
struct private_info
{
	uint part_sectors;
	char *part_buf;
	char part_name[32];
};

struct private_info  pri_info[1];
unsigned int protect_num;

#ifndef CONFIG_SUNXI_SPINOR_PLATFORM
int update_boot0_dram_para(char *buffer);
#endif

#define  SYSTEM_PAGE_SIZE        (512)
#define  SPINOR_PAGE_SIZE        (256)
#define  NPAGE_IN_1SYSPAGE       (SYSTEM_PAGE_SIZE/SPINOR_PAGE_SIZE)
#define  SPINOR_BLOCK_BYTES      (64 * 1024)
#define  SPINOR_BLOCK_SECTORS    (SPINOR_BLOCK_BYTES/512)
#define  NOR_ERASE_SECTOR   (64*1024)


/* instruction definition */
#ifndef CONFIG_FPGA
#define SPINOR_READ        0x03
#define SPINOR_FREAD       0x0b
#define SPINOR_WREN        0x06
#define SPINOR_WRDI        0x04
#define SPINOR_RDSR        0x05
#define SPINOR_WRSR        0x01
#define SPINOR_PP          0x02
#define SPINOR_SE          0xd8
#define SPINOR_BE          0xc7
#define SPINOR_RDID        0x9f
#else
#define SPINOR_READ		0x03 // sclk <= 30MHz
#define SPINOR_FREAD_DUAL_IO 	0xbb // sclk <= 50MHz
#define SPINOR_FREAD_DUAL_OUT 	0x3b // sclk <= 75MHz
#define SPINOR_FREAD		0x0b // sclk <= 80MHz
#define SPINOR_RAPIDS_READ 	0x1b
#define SPINOR_WREN  		0x06
#define SPINOR_WRDI  		0x04
#define SPINOR_RDSR  		0x05
#define SPINOR_WRSR  		0x01
#define SPINOR_WRSR2 		0x31
#define SPINOR_PP    		0x02
#define SPINOR_DUAL_IN_PP    	0xa2
//#define SPINOR_SE_4K 		0x20
//#define SPINOR_SE_32K 		0x52
#define SPINOR_SE	 		0xd8
#define SPINOR_BE    		0xc7

#define SPINOR_RDID  		0x9f
#define SSTAAI_PRG   		0xad
#define SPINOR_RESET 		0xF0
#define SPINOR_RESET_CFM   	0xd0
          
#endif


#define writel(value,addr)  (*((volatile __u32 *)(addr))  = (value))   /* word output */
#if defined CONFIG_ARCH_SUN8IW8P1 || defined CONFIG_ARCH_SUN8IW5P1
static void spic_config_dual_mode(u32 spi_no, u32 rxdual, u32 dbc, u32 stc)
{
	writel((rxdual<<28)|(dbc<<24)|(stc), SPI_BCC);
}
#endif
#ifndef CONFIG_SUNXI_SPINOR_PLATFORM
extern uint sunxi_sprite_generate_checksum(void *buffer, uint length, uint src_sum);

extern int sunxi_sprite_verify_checksum(void *buffer, uint length, uint src_sum);
#endif
/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :
*
*    return        :
*
*    note          :
*
*
************************************************************************************************************
*/
static int __spinor_wren(void)
{
    uint sdata = SPINOR_WREN;
    int ret = -1;
#if defined CONFIG_ARCH_SUN8IW8P1 || defined CONFIG_ARCH_SUN8IW5P1
	int  txnum ;
	txnum = 1;
	spic_config_dual_mode(0, 0, 0, txnum);
#endif
    ret = spic_rw(1, (void *)&sdata, 0, NULL);

	return ret;
}
/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :
*
*    return        :
*
*    note          :
*
*
************************************************************************************************************
*/
static int __spinor_rdsr(u8 *reg)
{
	u8 sdata = SPINOR_RDSR;
    int ret = -1;
#if defined CONFIG_ARCH_SUN8IW8P1 || defined CONFIG_ARCH_SUN8IW5P1
	int  txnum = 1;
	spic_config_dual_mode(0, 0, 0, txnum);
#endif
	ret = spic_rw(1, (void *)&sdata, 1, (void *)reg);

	return ret;
}

static int wait_till_ready(void)
{
    u8 status;
	do {
		if (__spinor_rdsr(&status) < 0)
            return -1;
		__msdelay(1);
	} while (status&0x01);
	return 0;
}
static uint __spinor_wrsr(u8 reg)
{
	u8 sdata[2] = {0};
	s32 ret = -1;
	u8  status = 0;
	uint txnum = 0;
	ret = __spinor_wren();
	if (ret==-1)
		goto __err_out_;
		
	txnum = 2;
	
	sdata[0] = SPINOR_WRSR;
	sdata[1] = reg;
	
#if defined CONFIG_ARCH_SUN8IW8P1 || defined CONFIG_ARCH_SUN8IW5P1
	spic_config_dual_mode(0, 0, 0, txnum);
#endif
	ret = spic_rw(txnum, (void*)sdata, 0, (void*)0);
	if (ret==-1)
		goto __err_out_;
	
	do {
		ret = __spinor_rdsr( &status);
		if (ret==-1)
			goto __err_out_;
			
		__msdelay(1);
	} while(status&0x01);
	ret = 0;
	
__err_out_:
	
	return ret;
}

/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :
*
*    return        :
*
*    note          :
*
*
************************************************************************************************************
*/
static int __spinor_erase_block(uint block_index)
{
    uint  blk_addr = block_index * SPINOR_BLOCK_BYTES;
    u8    sdata[5] = {0};
    int   ret = -1;
    uint  txnum = 0;

	if (wait_till_ready() < 0) {
		printf("nor flash wart sr failed\n");
		return -1;
	}
	if (__spinor_wren() < 0) {
		printf("nor flash write enable failed\n");
		return -1;
	}

	sdata[0] = SPINOR_SE;
	if (spinor_4bytes_addr_mode == 0) {
		txnum = 4;
		sdata[1] = (blk_addr >> 16) & 0xff;
		sdata[2] = (blk_addr >> 8 ) & 0xff;
		sdata[3] =  blk_addr        & 0xff;
	} else {
		txnum = 5;
		sdata[1] = (blk_addr >> 24) & 0xff;
		sdata[2] = (blk_addr >> 16) & 0xff;
		sdata[3] = (blk_addr >> 8 ) & 0xff;
		sdata[4] =  blk_addr        & 0xff;
	}
#if defined CONFIG_ARCH_SUN8IW8P1 || defined CONFIG_ARCH_SUN8IW5P1
	spic_config_dual_mode(0, 0, 0, txnum);
#endif
	ret = spic_rw(txnum, (void *)sdata, 0, (void *)0);
	if (-1 == ret)
    {
        return -1;
    }

	return 0;
}
/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :
*
*    return        :
*
*    note          :
*
*
************************************************************************************************************
*/
static int __spinor_erase_all(void)
{
    uint  sdata = 0;
    int   ret = -1;
    u8    status = 0;
    uint  txnum, rxnum;
    printf("begin to erase all :\n");
	int count = 0;
	ret = __spinor_wren();
	if (-1 == ret)
	{
	    return -1;
	}

	txnum = 1;
	rxnum = 0;

	sdata = SPINOR_BE;
#if defined CONFIG_ARCH_SUN8IW8P1 || defined CONFIG_ARCH_SUN8IW5P1
	spic_config_dual_mode(0, 0, 0, txnum);
#endif
    ret = spic_rw(txnum, (void*)&sdata, rxnum, 0);
	if (ret==-1)
    {
        return -1;
    }

	do
	{
	    ret = __spinor_rdsr(&status);
	    if (-1 == ret)
	    {
	        return -1;
	    }
		count ++;
                __msdelay(500);
                printf(".");
		if (count % 16 == 0)
			printf("\n");
	} while (status & 0x01);
	printf("\nstatus = %d\n",status);
	printf("count = %d \n",count);
	return 0;
}

/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :
*
*    return        :
*
*    note          :
*
*
************************************************************************************************************
*/
static int __spinor_pp(uint page_addr, void *buf, uint len)
{
    u8   sdata[300];
    u8   status = 0;
    uint i = 0;
    int  ret = -1;
    uint txnum, rxnum;

    if (wait_till_ready() < 0) {
	    printf("nor flash wart sr failed\n");
	    return -1;
    }

    if (len > 256)
    {
        return -1;
    }

	ret = __spinor_wren();
	if (ret < 0)
	{
	    return -1;
	}

	txnum = len+4;
	rxnum = 0;

	if(spinor_4bytes_addr_mode == 0)
	{
		txnum = len+4;
		sdata[0]  =  SPINOR_PP;
		sdata[1]  = (page_addr >> 16) & 0xff;
		sdata[2]  = (page_addr >> 8 ) & 0xff;
		sdata[3]  =  page_addr        & 0xff;
		memcpy((void *)(sdata+4), buf, len);
	}
	else if(spinor_4bytes_addr_mode == 1)
	{
		txnum = len+5;
		sdata[0]  =  SPINOR_PP;
		sdata[1]  = (page_addr >> 24) & 0xff;
		sdata[2]  = (page_addr >> 16) & 0xff;
		sdata[3]  = (page_addr >> 8 ) & 0xff;
		sdata[4]  =  page_addr        & 0xff;
		memcpy((void *)(sdata+5), buf, len);
	}
#if defined CONFIG_ARCH_SUN8IW8P1 || defined CONFIG_ARCH_SUN8IW5P1
	spic_config_dual_mode(0, 0, 0, txnum);
#endif
    ret = spic_rw(txnum, (void *)sdata, rxnum, 0);
	if (-1 == ret)
	{
        return -1;
    }

	do
	{
	    ret = __spinor_rdsr(&status);
	    if (ret < 0)
	    {
	        return -1;
	    }

	    for (i=0; i < 100; i++);
	} while (status & 0x01);

    return 0;
}
/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :
*
*    return        :
*
*    note          :
*
*
************************************************************************************************************
*/
static int __spinor_read_id(uint *id)
{
    uint sdata = SPINOR_RDID;
#if defined CONFIG_ARCH_SUN8IW8P1 || defined CONFIG_ARCH_SUN8IW5P1
	int  txnum ;
	txnum = 1;
	spic_config_dual_mode(0, 0, 0, txnum);
#endif
	return spic_rw(1, (void *)&sdata, 3, (void *)id);
}
/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :
*
*    return        :
*
*    note          :
*
*
************************************************************************************************************
*/
static int __spinor_sector_read(uint start, uint sector_cnt, void *buf)
{
    uint page_addr;
    uint rbyte_cnt;
    u8   sdata[6] = {0};
    int ret = 0;
    uint tmp_cnt, tmp_offset = 0;
    void  *tmp_buf;
    uint txnum, rxnum;

    if (wait_till_ready() < 0) {
	    printf("nor flash wart sr failed\n");
	    return -1;
    }

    txnum = 4;

    while (sector_cnt)
    {
        if (sector_cnt > 127)
        {
            tmp_cnt = 127;
        }
        else
        {
            tmp_cnt = sector_cnt;
        }

        page_addr = (start + tmp_offset) * SYSTEM_PAGE_SIZE;
        rbyte_cnt = tmp_cnt * SYSTEM_PAGE_SIZE;
        if(spinor_4bytes_addr_mode == 0)
        {
          txnum = 4;
          sdata[0]  =  SPINOR_READ;
          sdata[1]  = (page_addr >> 16) & 0xff;
          sdata[2]  = (page_addr >> 8 ) & 0xff;
          sdata[3]  =  page_addr        & 0xff;
        }
        else if(spinor_4bytes_addr_mode == 1)
        {
          txnum = 5;
          sdata[0]  =  SPINOR_READ;
          sdata[1]  = (page_addr >> 24) & 0xff;
          sdata[2]  = (page_addr >> 16) & 0xff;
          sdata[3]  = (page_addr >> 8 ) & 0xff;
          sdata[4]  =  page_addr        & 0xff;
        }

        rxnum   = rbyte_cnt;
        tmp_buf = (u8 *)buf + (tmp_offset << 9);
#if defined CONFIG_ARCH_SUN8IW8P1 || defined CONFIG_ARCH_SUN8IW5P1
		spic_config_dual_mode(0, 0, 0, txnum);
#endif
        if (spic_rw(txnum, (void *)sdata, rxnum, tmp_buf))
        {
            ret = -1;
            break;
        }

        sector_cnt -= tmp_cnt;
        tmp_offset += tmp_cnt;
    }

    return ret;
}
/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :
*
*    return        :
*
*    note          :
*
*
************************************************************************************************************
*/
static int __spinor_sector_write(uint sector_start, uint sector_cnt, void *buf)
{
    uint page_addr = sector_start * SYSTEM_PAGE_SIZE;
    uint nor_page_cnt = sector_cnt * (NPAGE_IN_1SYSPAGE);
    uint i = 0;
    int ret = -1;

    for (i = 0; i < nor_page_cnt; i++)
    {
    	//printf("spinor program page : 0x%x\n", page_addr + SPINOR_PAGE_SIZE * i);
		if((i & 0x1ff) == 0x1ff)
		{
			printf("nor_page_cnt=%d\n", i);
		}
        ret = __spinor_pp(page_addr + SPINOR_PAGE_SIZE * i, (void *)((uint)buf + SPINOR_PAGE_SIZE * i), SPINOR_PAGE_SIZE);
        if (-1 == ret)
        {
            return -1;
        }
    }

    return 0;
}
/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :
*
*    return        :
*
*    note          :
*
*
************************************************************************************************************
*/
int spinor_size(void);
int spinor_init(int stage)
{
	int spi_size = spinor_size();
	if(spinor_flash_inited)
	{
		puts("sunxi spinor is already inited\n");
                return 0;
	}
	else
	{
		puts("sunxi spinor is initing...\n");
		if(spic_init(0))
		{
			puts("sunxi spi init Fail\n");

			return -1;
		}
		else
		{
			puts("sunxi spi init OK\n");
		}

	}
	spinor_flash_inited ++;
	if(spi_size > 16*1024*1024/512)
	{
		spinor_4bytes_addr_mode = 1;
		spinor_enter_4bytes_addr(ENABLE_4BYTES);
	}
#ifndef CONFIG_SMALL_MEMSIZE
	if(uboot_spare_head.boot_data.work_mode != WORK_MODE_BOOT)
	{

		spinor_store_buffer  = (char *)malloc_noncache(16 * 1024 * 1024);
		if(!spinor_store_buffer)
		{
			puts("memory malloced fail for store buffer\n");

			return -1;
		}
	}
#endif
	spinor_write_cache  = (char *)malloc_noncache(64 * 1024);
	if(!spinor_write_cache)
	{
		puts("memory malloced fail for spinor data buffer\n");

#ifndef CONFIG_SMALL_MEMSIZE
		if(spinor_store_buffer)
		{
			free_noncache(spinor_store_buffer);

			return -1;
		}
#endif
	}

	return 0;
}
/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :
*
*    return        :
*
*    note          :
*
*
************************************************************************************************************
*/
int spinor_exit(int force)
{
	int need_exit = 0;
	int ret = 0;

	if(!spinor_flash_inited)
	{
		printf("sunxi spinor has not inited\n");

		return -1;
	}
	if(force == 1)
	{
		if(spinor_flash_inited)
		{
			printf("force sunxi spinor exit\n");

			spinor_flash_inited = 0;
			need_exit = 1;
		}
	}
	else
	{
		if(spinor_flash_inited)
		{
			spinor_flash_inited --;
		}
		if(!spinor_flash_inited)
		{
			printf("sunxi spinor is exiting\n");
			need_exit = 1;
		}
	}
	if(need_exit)
	{
		//printf("spi cache block == %d \n",spinor_cache_block);
		//printf("spi cache addr == %x \n",(uint)spinor_write_cache);
		//if((spinor_cache_block >= 0) && (spinor_write_cache))
		//{
		//	if(__spinor_sector_write(spinor_cache_block * SPINOR_BLOCK_SECTORS, SPINOR_BLOCK_SECTORS, spinor_write_cache))
		//	{
		//		printf("spinor write cache block fail\n");

		//		ret = -1;
		//	}
		//}
		//这里关闭spinor控制器
		//spic_exit(0);
	}

	return ret;
}
/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :
*
*    return        :
*
*    note          :
*
*
************************************************************************************************************
*/
int spinor_read(uint start, uint nblock, void *buffer)
{
	int tmp_block_index;

    debug("spinor read: start 0x%x, sector 0x%x\n", start, nblock);

	if(spinor_cache_block < 0)
	{
        debug("%s %d\n", __FILE__, __LINE__);
		if(__spinor_sector_read(start, nblock, buffer))
		{
			printf("spinor read fail no buffer\n");

			return 0;
		}
	}
	else
	{
		tmp_block_index = start/SPINOR_BLOCK_SECTORS;
		if(spinor_cache_block == tmp_block_index)
		{
			memcpy(buffer, spinor_write_cache + (start % SPINOR_BLOCK_SECTORS) * 512, nblock * 512);
		}
		else
		{
			if(__spinor_sector_read(start, nblock, buffer))
			{
				printf("spinor read fail with buffer\n");

				return 0;
			}
		}
	}

	return nblock;
}
/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :
*
*    return        :
*
*    note          :
*
*
************************************************************************************************************
*/
int spinor_write(uint start, uint nblock, void *buffer)
{
	int tmp_block_index;

	if(spinor_cache_block < 0)
	{
		spinor_cache_block = start/SPINOR_BLOCK_SECTORS;
		if(__spinor_sector_read(spinor_cache_block * SPINOR_BLOCK_SECTORS, SPINOR_BLOCK_SECTORS, spinor_write_cache))
		{
			printf("spinor read cache block fail\n");

			return 0;
		}
		if (__spinor_erase_block(spinor_cache_block) < 0)
			printf("spinor erase block %d fail\n", spinor_cache_block);

		memcpy(spinor_write_cache + (start % SPINOR_BLOCK_SECTORS) * 512, buffer, nblock * 512);
	}
	else
	{
		tmp_block_index = start/SPINOR_BLOCK_SECTORS;
		if(spinor_cache_block == tmp_block_index)
		{
			memcpy(spinor_write_cache + (start % SPINOR_BLOCK_SECTORS) * 512, buffer, nblock * 512);
		}
		else
		{
			if(__spinor_sector_write(spinor_cache_block * SPINOR_BLOCK_SECTORS, SPINOR_BLOCK_SECTORS, spinor_write_cache))
			{
				printf("spinor write cache block fail\n");

				return 0;
			}
			spinor_cache_block = tmp_block_index;
			if(__spinor_sector_read(spinor_cache_block * SPINOR_BLOCK_SECTORS, SPINOR_BLOCK_SECTORS, spinor_write_cache))
			{
				printf("spinor read cache block fail\n");

				return 0;
			}
			__spinor_erase_block(spinor_cache_block);
			memcpy(spinor_write_cache + (start % SPINOR_BLOCK_SECTORS) * 512, buffer, nblock * 512);
		}
	}

	return nblock;
}
/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :
*
*    return        :
*
*    note          :
*
*
************************************************************************************************************
*/
int spinor_flush_cache(void)
{
	if((spinor_cache_block >= 0) && (spinor_write_cache))
	{
		if(__spinor_sector_write(spinor_cache_block * SPINOR_BLOCK_SECTORS, SPINOR_BLOCK_SECTORS, spinor_write_cache))
		{
			printf("spinor write cache block fail\n");

			return -1;
		}
	}

	return 0;
}
/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :  erase:  0:
*
*							  1: erase the all flash
*    return        :
*
*    note          :
*
*
************************************************************************************************************
*/
int spinor_erase_all_blocks(int erase)
{
	if(erase)		//当参数为0，表示根据情况，自动判断是否需要进入擦除
	{
		__spinor_erase_all();
	}

	return 0;
}






/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :
*
*    return        :
*
*    note          :
*
*
************************************************************************************************************
*/
int spinor_size(void)
{
	int size = 0;
	int ret = 0;
	ret = script_parser_fetch("spi_board0","sflash_size", &size, sizeof(int));
	printf("flash size =%d M\n",size);
	if (ret)
	{
		size = 8*1024*1024/512;
		printf("get flash_size warning\n");
	}
	else
	{
		size =  size*1024*1024/512;
	}
	return size;
}
/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :
*
*    return        :
*
*    note          : per-buffer 64K must
*
*
************************************************************************************************************
*/
int spinor_sprite_write(uint start, uint nblock, void *buffer)
{
#ifdef CONFIG_SMALL_MEMSIZE
	/*int page_offset;*/
	/* Wait until finished previous write command. */
	if (first_write == 1) {
		first_write = 0;
#ifndef CONFIG_SUNXI_SPINOR_PLATFORM
		update_boot0_dram_para(buffer);
#endif
	}
	spinor_write(start, nblock, buffer);
	total_write_bytes += nblock * 512;
#else
	memcpy(spinor_store_buffer + start*512, buffer, nblock*512);
	total_write_bytes += nblock * 512;
#endif
	return nblock;
}
/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :
*
*    return        :
*
*    note          :
*
*
************************************************************************************************************
*/
//static void spinor_dump(void *buffer, int len)
//{
//	char *addr;
//	int  i;
//
//	addr = (char *)buffer;
//	for(i=0;i<len;i++)
//	{
//		printf("%02x  ", addr[i]);
//		if((i & 0x07) == 0x07)
//		{
//			puts("\n");
//		}
//	}
//}
#ifndef CONFIG_SUNXI_SPINOR_PLATFORM
int update_boot0_dram_para(char *buffer)
{
    boot0_file_head_t    *boot0  = (boot0_file_head_t *)buffer;
	int i;
	uint *addr = (uint *)DRAM_PARA_STORE_ADDR;

	//校验特征字符是否正确
	printf("%s\n", boot0->boot_head.magic);
	if(strncmp((const char *)boot0->boot_head.magic, BOOT0_MAGIC, MAGIC_SIZE))
	{
		printf("sunxi sprite: boot0 magic is error\n");
		return -1;
	}
	if(sunxi_sprite_verify_checksum((void *)buffer, boot0->boot_head.length, boot0->boot_head.check_sum))
	{
		printf("sunxi sprite: boot0 checksum is error\n");

		return -1;
	}

	for(i=0;i<32;i++)
	{
		printf("dram para[%d] = %x\n", i, addr[i]);
	}
	memcpy((void *)&boot0->prvt_head.dram_para, (void *)DRAM_PARA_STORE_ADDR, 32 * 4);
	/* regenerate check sum */
	boot0->boot_head.check_sum = sunxi_sprite_generate_checksum(buffer, boot0->boot_head.length, boot0->boot_head.check_sum);
	//校验数据是否正确
	if(sunxi_sprite_verify_checksum((void *)buffer, boot0->boot_head.length, boot0->boot_head.check_sum))
	{
		printf("sunxi sprite: boot0 checksum is error\n");
		return -1;
	}
	printf("update dram para success \n");
	return 0;

}
#endif

int sunxi_sprite_store_private_data(void)
{
	int i;
	sunxi_mbr_t* mbr; //mbr size 16K
	mbr = (sunxi_mbr_t *) malloc (16*1024);
	memset(mbr, 0 , 16 *1024);
	spinor_read((512-16)*1024/512, 16*1024/512, mbr);
		
	for(i=0; i < mbr->PartCount; i++) {
		printf("part name %s\n", mbr->array[i].name);
		printf("keydata = 0x%x\n", mbr->array[i].keydata);
		if(!strcmp((const char *)mbr->array[i].name, "private"))
		{
			printf("find keypart %s\n", mbr->array[i].name);
			printf("keypart read start: 0x%x, sectors 0x%x\n", mbr->array[i].addrlo, mbr->array[i].lenlo);

			pri_info[0].part_buf = (char *)malloc(mbr->array[i].lenlo * 512);
			if(!pri_info[0].part_buf)
			{
				printf("sprite protect private data fail: cant malloc memory for part %s, sectors 0x%x\n", mbr->array[i].name, mbr->array[i].lenlo);

				goto __sunxi_sprite_store_part_data_fail;
			}
			if(__spinor_sector_read(mbr->array[i].addrlo + (512-16)*1024/512, mbr->array[i].lenlo, (void *)pri_info[0].part_buf))
			{
				printf("sunxi sprite error : read private data error\n");

				goto __sunxi_sprite_store_part_data_fail;
			}
			printf("keypart part %s read end: 0x%x, sectors 0x%x\n", mbr->array[i].name, mbr->array[i].addrlo, mbr->array[i].lenlo);

			pri_info[0].part_sectors = mbr->array[i].lenlo;
			strcpy(pri_info[0].part_name, (const char *)mbr->array[i].name);
			
			protect_num++;
			break;
		}

	}
	free(mbr);
	
	return 0;
	
__sunxi_sprite_store_part_data_fail:
	free(pri_info[0].part_buf);
	free(mbr);
	
	return -1;
}

#ifndef CONFIG_SMALL_MEMSIZE
int sunxi_sprite_restore_private_data(void)
{
	int i;
	int ret = -1;
	uint      down_sectors;
	sunxi_mbr_t* img_mbr; //mbr size 16K

	img_mbr = (sunxi_mbr_t *)malloc(16 * 1024);//16K 
	memset(img_mbr, 0 , 16*1024);
	memcpy(img_mbr,spinor_store_buffer + 496 * 1024, 16*1024);
	
	if(protect_num) {
		for(i=0;i < img_mbr->PartCount; i++)
		{
			if(!strcmp(pri_info[0].part_name, (const char*)img_mbr->array[i].name))
			{
				//确保在分区表有变化的情况下，也能正常恢复private数据
				if(pri_info[0].part_sectors > img_mbr->array[i].lenlo)
				{
					printf("origin sectors 0x%x, new part sectors 0x%x\n", pri_info[0].part_sectors, img_mbr->array[i].lenlo);
					printf("fix it, only store less sectors\n");

					down_sectors = img_mbr->array[i].lenlo;
				}
				else
				{
					down_sectors = pri_info[0].part_sectors;
				}

				printf("keypart write start: 0x%x, sectors 0x%x\n", img_mbr->array[i].addrlo, down_sectors);
				if(__spinor_sector_write(img_mbr->array[i].addrlo + (512-16)*1024/512, down_sectors, (void *)pri_info[0].part_buf))
				{
					printf("sunxi sprite error : write private data error\n");

					goto __sunxi_sprite_restore_part_data_fail;
				}

				printf("keypart write end: 0x%x, sectors 0x%x\n", img_mbr->array[i].addrlo, down_sectors);
				protect_num--;
				break;
			}
		}
	}

	ret = 0;

__sunxi_sprite_restore_part_data_fail:
	free(pri_info[0].part_buf);
	free(img_mbr);

	return ret;
}
#endif

int spinor_datafinish(void)
{
#ifndef CONFIG_SMALL_MEMSIZE
	uint  id = 0xff;
	int   ret;

	printf("spinor_datafinish\n");
	__spinor_read_id(&id);

	printf("spinor id = 0x%x\n", id);
	__spinor_wrsr(0);
	//sunxi_sprite_store_private_data();
	__spinor_erase_all();

	printf("spinor has erasered\n");

#ifndef CONFIG_SUNXI_SPINOR_PLATFORM
	if(update_boot0_dram_para(spinor_store_buffer))
	{
		return -1;
	}
#endif

	printf("total write bytes = %d\n", total_write_bytes);
	ret = __spinor_sector_write(0, total_write_bytes/512, spinor_store_buffer);
	if(ret)
	{
		printf("spinor write img fail\n");

		return -1;
	}
	
	//sunxi_sprite_restore_private_data();
//	buffer = (char *)malloc(8 * 1024 * 1024);
//	memset(buffer, 0xff, 8 * 1024 * 1024);
//	ret = __spinor_sector_read(0, total_write_bytes/512, buffer);
//	if(ret)
//	{
//		printf("spinor read img fail\n");
//
//		return -1;
//	}
//	printf("spinor read data ok\n");
//	for(i=0;i<total_write_bytes;i++)
//	{
//		if(buffer[i] != spinor_store_buffer[i])
//		{
//			printf("compare spinor read and write error\n");
//			printf("offset %d\n", i);
//
//			return -1;
//		}
//	}

	printf("spinor download data ok\n");
#endif
	spinor_enter_4bytes_addr(DISABLE_4BYTES);
	return 0;
}
/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :
*
*    return        :
*
*    note          :
*
*
************************************************************************************************************
*/
//int nand_get_mbr(char *buffer, int mbr_size)
//{
//	memcpy(spinor_mbr, buffer, SUNXI_MBR_SIZE);

//	return 0;
//}


s32 spi_nor_rw_test(u32 spi_no)
{

	const u32 test_len = 256*64;
    char* wbuf = (char*)malloc(test_len);
	char* rbuf = (char*)malloc(test_len);
	u32 i;
	s32 ret;
	uint id =0xffff;

	if(spinor_init(0))
	{
		printf("spinor init failed \n");
		return -1;
	}
	if(__spinor_read_id(&id))
	{
		printf("spinor read id failed \n");
		return -1;
	}
	__spinor_wrsr(0);
	printf("the id is %x\n",id);
	printf("bfeore spi_nor_rw_test\n");
	
	__spinor_sector_read(0, 1, rbuf);

	for(i = 0; i < 256; i+=4)
	{
		printf("verify rbuf=0x%x\n",*(u32 *)(rbuf+i));
	}
	
	printf("before erase all \n");
	ret = __spinor_erase_all();
	printf("after erase all \n");
	printf("bfeore spi_nor_rw_test,ret=%d\n",ret);

	if (!wbuf || !rbuf) {
		printf("spi %d malloc buffer failed\n", spi_no);
		return -1;
	}

	for (i=0; i<test_len; i++)
		wbuf[i] = i;
	
	
	i = 0;
	ret = __spinor_sector_read( i, 1, rbuf);
 	if (ret) {
 		printf("spi %d read page %d failed\n", spi_no,i);
 			return -1;
 	}

	for (i = 0; i < (test_len >> 8)/4; i++) 
	printf("verify rbuf=[0x%x]\n",*(u32 *)(rbuf+i*4));
	
//	ret = __spinor_erase_block(0);	//erase 64K
//	if (ret) {
//		printf("spi erase ss1 nor failed\n");
//		return -1;
//	}
	
    for (i=0; i<10; i++)
	printf("b rbuf=[0x%x]\n",*(u32 *)(rbuf));
    for (i = 0; i < (test_len >> 8); i++) {
		printf("begin to write \n");
		ret = __spinor_sector_write(i, 1, wbuf);
		if (ret) {
			printf("spi %d write page %d failed\n", spi_no,i);
			return -1;
		}
		printf("end to write \n");
		memset(rbuf, 0, 256);
		printf("begin to read\n");
		ret = __spinor_sector_read(i, 1, rbuf);
		if (ret) {
			printf("spi %d read page %d failed\n", spi_no,i);
			return -1;
		}
		printf("end to read \n");
		//compare data
		if (memcmp(wbuf, rbuf, 256)) {
			printf("spi %d page %d read/write failed\n", spi_no, i);
			while(1);
			return -1;
		} else
			printf("spi %d page %d read/write ok\n", spi_no, i);
		
	}
	free(wbuf);
	free(rbuf);
	return 0;
}



u32 try_spi_nor(u32 spino)
{
	uint id =0xffff;
	if(spinor_init(spino))
	{
		printf("spinor init failed \n");
		return -1;
	}
	if(__spinor_read_id(&id))
	{
		printf("spinor read id failed \n");
		return -1;
	}
	if((id & 0xFFFF) == 0xffff || id == 0x0)
	{
	    printf("read spinor id failed:%x \n",id);
		return -1;
	}
	printf("spinor id is %x \n",id);
	return 0;
}

static void spinor_enter_4bytes_addr(int enable)
{
	int command = 0;
	if(spinor_4bytes_addr_mode == 1 && enable == 1)
		command = 0xB7;
	else if(spinor_4bytes_addr_mode == 1 && enable == 0)
		command = 0xE9;
	else
		return ;
	spic_config_dual_mode(0, 0, 0, 1);
	spic_rw(1, (void*)&command, 0, 0);
	return;
}
