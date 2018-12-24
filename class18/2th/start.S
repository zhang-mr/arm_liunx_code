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
		/* 恢复现场 */
	ldmia sp!, {r0-r12, pc}^  /* ^会把spsr的值恢复到cpsr里 */


do_swi:
	ldr sp, =0x33c00000	
	stmdb sp!, {r0-r12, lr}  
	sub r4, lr, #4
	MRS r0, SPSR
	ldr r1, =swi_string
	bl printException
	mov r0, r4
	bl printfSWIVal	
		/* 恢复现场 */
	ldmia sp!, {r0-r12, pc}^  /* ^会把spsr的值恢复到cpsr里 */
		
swi_string:
		.string "swi exception"

.align 4

do_und:
	/* 执行到这里之前:
	 * 1. lr_und保存有被中断模式中的下一条即将执行的指令的地址
	 * 2. SPSR_und保存有被中断模式的CPSR
	 * 3. CPSR中的M4-M0被设置为11011, 进入到und模式
	 * 4. 跳到0x4的地方执行程序 
	 */

/* sp_und未设置, 先设置它 */
	ldr sp, =0x33b00000
	/* 在und异常处理函数中有可能会修改r0-r12, 所以先保存 */
	/*lr是异常处理完后的返回地址, 也要保存 */
	
	stmdb sp!, {r0-r12, lr}  
		
	/* 保存现场 */
	/* 处理und异常 */
	mrs r0, cpsr
	ldr r1, =und_string
	bl printException
		
	/* 恢复现场 */
	ldmia sp!, {r0-r12, pc}^ /* ^会把spsr的值恢复到cpsr里 */
		
und_string:
	.string "undefined instruction exception"
	
.align 4

	
reset:
	/* 关闭看门狗	*/
	ldr r0, =0x53000000
	ldr r1, =0
	str r1, [R0]
	
	/* 设置MPLL，FCLK，HCLK，PCLK */
	/* 设置CLOCKTIME=0xffffffff */
	ldr r0, =0x4c000000
	ldr r1, =0x0FFFFFFF
	str r1, [r0]

	/* 设置CLKDIVN   */
	ldr r0, =0x4c000014
	ldr r1, =0x3    //1:2:4
	str r1, [r0]

	/* 设置CPU工作于异步模式 */
	mrc p15,0,r0,c1,c0,0
	orr r0,r0, #0xc0000000      //#R1_nF:OR:R1_iA = 0xc0000000
	mcr p15,0,r0,c1,c0,0

	/* 设置MCLK */
	ldr r0, =0x4c000004
	ldr r1, =(125<<12)|(1<<4)|(1<<0)   // 266M
	str r1, [r0]
	
	/* 一旦设置PLL，就会锁定lock_time知道PLL输出稳定，然后CPU工作在新的频率 */
	
	/*
	 * 分辨是nor还是nand启动  
	 * 写0到0地址，在读出来，如果是0则是NANDflash，
	 * 否则是nor启动。
	 */
	mov r1, #0
	/* 先读出原来的值  */
	ldr r0, [R1]
	str r1, [r1]
	ldr r2, [R1]
	cmp r1, r2
	
	/* 假设是nor启动  */
	ldr sp, =0x40000000+4096
	/* 	如果相等   则是nor启动 	   */  
	moveq sp, #4096 
	streq r0, [r1]

	/* 初始化SDRAM */
	bl sdram_init

	/* copy  data段 */
	bl copy_data_sdram

	/* 清除bss段 */
	bl clean_bss_sdram

	/*
 	 * 在复位之后，CPU处于svc模式
 	 * 我们需要切换到usr模式，
 	 */
 	 MRS r0, CPSR    
 	 bic r0, r0, #0xf
 	 bic r0, r0, #(1<<7)    // 使能总中断
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




