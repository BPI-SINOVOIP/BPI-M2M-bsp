obj-y += nand.o

nand-objs += nand_interface.o \
	nfd/nand_ota_burn.o \
	nfd/boot_head/boot0_op.o \
	nfd/nand_base.o \
	nfd/nand_blk.o \
	nfd/nand_dev.o \
	nfd/nand_class.o \
	nfd/nand_osal_for_linux.o \
	libnand_sun8iw8p1

ifneq ($(wildcard $(obj)/lib),)
$(obj)/libnand_sun8iw8p1 : FORCE
	$(MAKE) $(build)=$(obj)/lib
	@cp -f $(obj)/lib/libnand.o $@
endif
