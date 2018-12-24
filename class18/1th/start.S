/*
 * 2018/12/9   
 */

.text
.global _start

_start:
	b reset
	ldr pc,und_addr	 /* 4 */
	ldr pc,swi_addr	 /* 8 */
	b reset				 /* c */
	b reset	 			 /* 10 */
	b reset	 			 /* 14 */
	ldr pc,irq_addr	  	/* 18 */  
	ldr pc,irq_addr	 	/* 1c */


und_addr:
	.word do_und

swi_addr:
	.word do_swi

irq_addr:
	.word do_irq

do_irq:
	ldr sp, =0x33d00000	
	sub lr, lr, #4
	stmdb sp!, {r0-r12, lr}  
	
	bl handle_irq_c	
		/* �ָ��ֳ� */
	ldmia sp!, {r0-r12, pc}^  /* ^���spsr��ֵ�ָ���cpsr�� */


do_swi:
	ldr sp, =0x33c00000	
	stmdb sp!, {r0-r12, lr}  
	sub r4, lr, #4
	MRS r0, SPSR
	ldr r1, =swi_string
	bl printException
	mov r0, r4
	bl printfSWIVal	
		/* �ָ��ֳ� */
	ldmia sp!, {r0-r12, pc}^  /* ^���spsr��ֵ�ָ���cpsr�� */
		
swi_string:
		.string "swi exception"

.align 4

do_und:
	/* ִ�е�����֮ǰ:
	 * 1. lr_und�����б��ж�ģʽ�е���һ������ִ�е�ָ��ĵ�ַ
	 * 2. SPSR_und�����б��ж�ģʽ��CPSR
	 * 3. CPSR�е�M4-M0������Ϊ11011, ���뵽undģʽ
	 * 4. ����0x4�ĵط�ִ�г��� 
	 */

/* sp_undδ����, �������� */
	ldr sp, =0x33b00000
	/* ��und�쳣�����������п��ܻ��޸�r0-r12, �����ȱ��� */
	/*lr���쳣�������ķ��ص�ַ, ҲҪ���� */
	
	stmdb sp!, {r0-r12, lr}  
		
	/* �����ֳ� */
	/* ����und�쳣 */
	mrs r0, cpsr
	ldr r1, =und_string
	bl printException
		
	/* �ָ��ֳ� */
	ldmia sp!, {r0-r12, pc}^ /* ^���spsr��ֵ�ָ���cpsr�� */
		
und_string:
	.string "undefined instruction exception"
	
.align 4

	
reset:
	/* �رտ��Ź�	*/
	ldr r0, =0x53000000
	ldr r1, =0
	str r1, [R0]
	
	/* ����MPLL��FCLK��HCLK��PCLK */
	/* ����CLOCKTIME=0xffffffff */
	ldr r0, =0x4c000000
	ldr r1, =0x0FFFFFFF
	str r1, [r0]

	/* ����CLKDIVN   */
	ldr r0, =0x4c000014
	ldr r1, =0x3    //1:2:4
	str r1, [r0]

	/* ����CPU�������첽ģʽ */
	mrc p15,0,r0,c1,c0,0
	orr r0,r0, #0xc0000000      //#R1_nF:OR:R1_iA = 0xc0000000
	mcr p15,0,r0,c1,c0,0

	/* ����MCLK */
	ldr r0, =0x4c000004
	ldr r1, =(125<<12)|(1<<4)|(1<<0)   // 266M
	str r1, [r0]
	
	/* һ������PLL���ͻ�����lock_time֪��PLL����ȶ���Ȼ��CPU�������µ�Ƶ�� */
	
	/*
	 * �ֱ���nor����nand����  
	 * д0��0��ַ���ڶ������������0����NANDflash��
	 * ������nor������
	 */
	mov r1, #0
	/* �ȶ���ԭ����ֵ  */
	ldr r0, [R1]
	str r1, [r1]
	ldr r2, [R1]
	cmp r1, r2
	
	/* ������nor����  */
	ldr sp, =0x40000000+4096
	/* 	������   ����nor���� 	   */  
	moveq sp, #4096 
	streq r0, [r1]

	/* ��ʼ��SDRAM */
	bl sdram_init

	/* copy  data�� */
	bl copy_data_sdram

	/* ���bss�� */
	bl clean_bss_sdram

	/*
 	 * �ڸ�λ֮��CPU����svcģʽ
 	 * ������Ҫ�л���usrģʽ��
 	 */
 	 MRS r0, CPSR    
 	 bic r0, r0, #0xf
 	 bic r0, r0, #(1<<7)    // ʹ�����ж�
 	 MSR CPSR, r0

	ldr sp, =0x33f00000

	bl uart0_init

	MRS r0, CPSR  
    bl printfSWIVal 
	
	.word 0xdeadc0de
	
	swi 144

	//bl main
	 
	ldr pc,=main

halt: 
	b halt 



