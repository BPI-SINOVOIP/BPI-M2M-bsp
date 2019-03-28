
void rot_90_u32(
	unsigned int *src_addr, const unsigned int src_widthstep,
	const unsigned int src_width, const unsigned int src_height,
	unsigned int *dst_addr, const unsigned int dst_widthstep)
{
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned char *src_line0_addr = 0;
	unsigned char *src_line1_addr = 0;
	unsigned char *dst_line0_addr = 0;
	unsigned char *dst_line1_addr = 0;
	unsigned char *tmp_line0_addr = 0;
	unsigned char *tmp_line1_addr = 0;

	unsigned int double_row_argb_size = 0;
	unsigned int double_col_argb_size = 0;
	unsigned int row_argb_size = src_width*4;
	unsigned int col_argb_size = src_height*4;

	double_row_argb_size  = row_argb_size*2;
	double_col_argb_size  = col_argb_size*2;

	src_line0_addr = (unsigned char *)src_addr;
	src_line1_addr = (unsigned char *)src_addr + src_widthstep;
	dst_line0_addr = (unsigned char *)dst_addr + col_argb_size - 8;
	dst_line1_addr = (unsigned char *)dst_addr + dst_widthstep
		+ col_argb_size - 8;

	tmp_line0_addr = dst_line0_addr;
	tmp_line1_addr = dst_line1_addr;

	for (i = 0; i < src_height/2; i++) {
		for (j = 0; j < src_width/2; j++) {
			asm volatile
				(
					"vld1.u8	{d0}, [%[src_line0_addr]]!		\n\t"
					"vld1.u8	{d1}, [%[src_line1_addr]]!		\n\t"
					"vtrn.32	d1, d0			\n\t"
					"vst1.u8	{d0}, [%[tmp_line1_addr]]		\n\t"
					"vst1.u8	{d1}, [%[tmp_line0_addr]]		\n\t"
					"vst1.u8	{d0}, [%[tmp_line1_addr]]		\n\t"
					"vst1.u8	{d1}, [%[tmp_line0_addr]]		\n\t"
				:
				[src_line0_addr] "+r" (src_line0_addr),
				[src_line1_addr] "+r" (src_line1_addr),
				[tmp_line0_addr] "+r" (tmp_line0_addr),
				[tmp_line1_addr] "+r" (tmp_line1_addr)
				: /*[srcY] "r" (srcY) */
				: "cc", "memory", "d0", "d1"
				);
				tmp_line0_addr += dst_widthstep*2;
				tmp_line1_addr += dst_widthstep*2;
		}
		src_line0_addr = src_line1_addr + src_widthstep - row_argb_size;
		src_line1_addr = (src_line0_addr + src_widthstep);
		tmp_line0_addr = dst_line0_addr - 8*(i + 1);
		tmp_line1_addr = dst_line1_addr - 8*(i + 1);
	}
}
