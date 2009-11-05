直接打开工程：

请检查是否加入以下.c文件及启动文件
       FWlib文件夹里：
       stm32f10x_rcc.c
	   stm32f10x_gpio.c
	   stm32f10x_flash.c

	   IRQ文件夹里:
	   stm32f10x_it.c

	   RWMDK文件夹里：
	   cortexm3_macro.s
	   stm32f10x_vector.s 
	   
	   User文件夹里：	  
	    main.c



		编译下载到目标板后，按复位，则程序开始运行


