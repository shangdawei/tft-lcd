SET RVCT31INC=C:\Keil\ARM\RV31\INC
SET RVCT31LIB=C:\Keil\ARM\RV31\LIB
SET CPU_TYPE=STM32F101C6
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=STM3210E
SET CPU_CLOCK=0x00000000
C:\Keil\ARM\BIN31\ArmCC --Via ".\obj\main.__i"
C:\Keil\ARM\BIN31\ArmAsm --Via ".\obj\cortexm3_macro._ia"
C:\Keil\ARM\BIN31\ArmAsm --Via ".\obj\stm32f10x_vector._ia"
C:\Keil\ARM\BIN31\ArmCC --Via ".\obj\stm32f10x_gpio.__i"
C:\Keil\ARM\BIN31\ArmCC --Via ".\obj\stm32f10x_flash.__i"
C:\Keil\ARM\BIN31\ArmCC --Via ".\obj\stm32f10x_rcc.__i"
C:\Keil\ARM\BIN31\ArmCC --Via ".\obj\stm32f10x_usart.__i"
C:\Keil\ARM\BIN31\ArmCC --Via ".\obj\stm32f10x_it.__i"
C:\Keil\ARM\BIN31\ArmCC --Via ".\obj\uart.__i"
C:\Keil\ARM\BIN31\ArmCC --Via ".\obj\datadeal.__i"
C:\Keil\ARM\BIN31\ArmCC --Via ".\obj\flash.__i"
C:\Keil\ARM\BIN31\ArmLink --Via ".\Obj\STM3210E-EVAL.lnp"
C:\Keil\ARM\BIN31\fromelf.exe ".\Obj\STM3210E-EVAL.axf" --i32combined --output ".\Obj\STM3210E-EVAL.hex"
