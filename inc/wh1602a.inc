;/*
; * wh1602a.inc
; *
; *  Created: 01.05.2018 6:36:48
; *   Author: zoosman
; */

;/* /////////////// LCD WH1602A initialization 50ms delay at start ///////////// */
INIT_WH1602A:
  DELAY_MACRO 0x05, 0xc4 ; TCCR0B (1<<CS02)|(1<<CS00) - 1024, 196, 25ms
  DELAY_MACRO 0x05, 0xc4 ; TCCR0B (1<<CS02)|(1<<CS00) - 1024, 196, 25ms

  ldi ZH, high(2*WH1602A_init_bytes)
  ldi ZL, low(2*WH1602A_init_bytes)
  lpm tmp, Z+
	sts lcdAddr, tmp
  ldi tmp, 0x07
  mov R0, tmp
  rcall CLEAR_TIMER
  
  _INIT_WH1602A_BYTE:
    rcall I2CM_START
    rcall WH1602A_SEND_ADDRESS
    lpm txByte, Z+
    rcall WH1602A_SEND_COMMAND
    rcall I2CM_STOP
	lpm dClock, Z+
	lpm dRate, Z+
	rcall DELAY
    dec R0
    brne _INIT_WH1602A_BYTE
  
  rcall INIT_TIMER
	ret

;/* /////////////////////////// 1602A Send Address ///////////////////////////// */
WH1602A_SEND_ADDRESS:
  lds txByte, lcdAddr
  rcall I2CM_SEND_ADDRESS
  ret

;/* ///////////////////////// 1602A Send Half Byte ///////////////////////////// */
WH1602A_SEND_NIBBLE:
  andi txByte, 0xf0
  sbrc _EREG_, _1602RSF_
  sbr txByte, (1<<_Rs)
  sbr txByte, (1<<_BL)|(1<<_E)
  rjmp _WH1602A_SEND_NIBBLE_DATA
  _WH1602A_SEND_NIBBLE_ACK:
    cbr txByte, (1<<_E)
  _WH1602A_SEND_NIBBLE_DATA:
    rcall I2CM_SEND_PACKET
  sbrc txByte, _E
  rjmp _WH1602A_SEND_NIBBLE_ACK
  ret
  
;/* //////////////////////////// 1602A Send Packet ///////////////////////////// */
WH1602A_SEND_PACKET:
  push R0
  mov R0, txByte
  rcall WH1602A_SEND_NIBBLE
  swap R0
  mov txByte, R0
  rcall WH1602A_SEND_NIBBLE
  pop R0
  ret

;/* /////////////////////////// 1602A Send Command ///////////////////////////// */
WH1602A_SEND_COMMAND:
  cbr _EREG_, (1<<_1602RSF_)
  rcall WH1602A_SEND_PACKET
  ret

;/* ///////////////////////////// 1602A Send Data ////////////////////////////// */
WH1602A_SEND_DATA:
  sbr _EREG_, (1<<_1602RSF_)
  rcall WH1602A_SEND_PACKET
  ret
