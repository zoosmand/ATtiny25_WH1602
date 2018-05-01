;/*
; * var.inc
; *
; *  Created: 01.05.2018 6:39:51
; *   Author: zoosman
; */ 

;/******************************** VARIABLES & DATAS *****************************/
.cseg
  WH1602A_init_bytes: .db 0x27, 0x03, 0x04, 0x82, 0x03, 0x04, 0x82, 0x03, 0x04, 0x05, 0x02, 0x04, 0x30, 0x28, 0x03, 0x05, 0x0c, 0x03, 0x05, 0x01, 0x04, 0x30

;  0x27 - LCD WH1602 I2C address
;  0x03 - init command, 0x04, 0x82 - TCCR0B (1<<CS02) - 265, 130, 4.1ms
;  0x03 - init command, 0x04, 0x82 - TCCR0B (1<<CS02) - 265, 130, 4.1ms
;  0x03 - init command, 0x04, 0x05 - TCCR0B (1<<CS02) - 265, 5, 150us
;  0x02 - init command, 0x04, 0x30 - TCCR0B (1<<CS02) - 265, 48, 1.53ms
;  0x28 - init command, 0x03, 0x05 - TCCR0B (1<<CS01)|(1<<CS00) - 64, 5, 39us
;  0x0c - init command, 0x03, 0x05 - TCCR0B (1<<CS01)|(1<<CS00) - 64, 5, 39us
;  0x01 - init command, 0x04, 0x30 - TCCR0B (1<<CS02) - 265, 48, 1.53ms

;  0x03 - 4-bit initialization
;  0x02 - Cursor is in upper left position
;  0x28 - Bus width is 4-bit, LCD is 2-lines
;  0x0c - Allow output without cursor blinking
;  0x01 - Clear display

WH1602A_symb: .db "Hallo!!!"
;  0x80 - Clear display
;  0x48 - H
;  0x54 - T
;  0x87 - R

.dseg
.org 0x0060
  lcdAddr:    .byte 1
 
;.eseg
;  StartParam:	.db "Ready"
