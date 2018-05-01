;/*
; * macroses.inc
; *
; *  Created: 01.05.2018 6:28:38
; *   Author: zoosman
; */ 


;/******************************** Long Time Counter ******************************/
.MACRO set_tcnt
  ldi tcntH, 0x00 // 0x01 0x00
  ldi tcntL, 0xff // 0xff 0x05
.ENDMACRO

;/*********************************** DELAY MACRO *********************************/
.MACRO DELAY_MACRO
  ldi tmp, @0
  mov dClock, tmp
  ldi tmp, @1
  mov dRate, tmp
  rcall DELAY
.ENDMACRO
