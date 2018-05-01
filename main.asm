;
; ATtiny25_AS__I2C_Master_1602A_.asm
;
; Created: 09.06.2017 12:24:53
; Author : setup
;
.include "tn25def.inc"
.equ F_CPU = 8000000

.def dClock   = R12
.def dRate    = R13
.def cntHL    = R14
.def cntd     = R15
.def tmp      = R16
.def _EREG_   = R17
.def txByte   = R18
.def rxByte   = R19
.def tcntL    = R20
.def tcntH    = R21
.def tmpL     = R22
.def tmpH     = R23
.def cntLL    = R24
.def cntLH    = R25

.equ LEDDDR   = DDRB
.equ LEDPORT  = PORTB
.equ LEDPIN   = PINB
.equ LED0PIN  = PB3

.equ I2CDDR   = DDRB
.equ I2CPORT  = PORTB
.equ I2CPIN   = PINB
.equ SDA      = PB0
.equ SCL      = PB2

;/******************************* Event REGistry *********************************/
.equ _MIF_     = 0 ; Timer Measure Interval Flag
.equ _I2CRWF_  = 1 ; I2C Read/Write Flag, 0 - write (m->s), 1 - read (m<-s)
.equ _I2CANF_  = 2 ; I2C Ack/Nack Flag, 0 - ACK, 1 - NACK
.equ _I2CERF_  = 3 ; I2C ERror Flag
.equ _1602RSF_ = 4 ; 1602A RS Flag, 0 - command, 1 - data
.equ _1602RWF_ = 5 ; 1602A R/W Flag, 0 - write, 1 - read
.equ _1602EF_  = 6 ; 1602A E Flag, Enable (Strobe) Flag
.equ _BF_      = 7 ; Bit Flag

;/******************************** WH1602A Pins **********************************/
.equ _E   = 2 ; Strobe pin
.equ _Rw  = 1 ; Read/Write pin
.equ _Rs  = 0 ; Command/Data pin
.equ _BL  = 3 ; Backlight pin, 4-th pin is responded for backlight with PC8574 I2C transceiver


.cseg
.org 0

.include "./inc/vectors.inc"
.include "./inc/macroses.inc"
.include "./inc/init.inc"

;/*********************************** MAIN LOOP **********************************/
MAIN:
  sbrs _EREG_, _MIF_
  rjmp _GO_TO_SLEEP
  cbr _EREG_, (1<<_MIF_)
  rcall SEND_HALLO

  _RESTORE_TIMER:
    rcall CLEAR_TIMER
    rcall INIT_TIMER

  _GO_TO_SLEEP:
    in tmp, MCUCR
    sbr tmp, (1<<SE)
    out MCUCR, tmp
    sei
    sleep

  rjmp MAIN
  rjmp THE_END

.include "./inc/i2c.inc"
.include "./inc/wh1602a.inc"
.include "./inc/interrupts.inc"
.include "./inc/utils.inc"


;/*********************************** Hallo!!! ************************************/
SEND_HALLO:
  rcall CLEAR_TIMER

  rcall I2CM_START
  rcall WH1602A_SEND_ADDRESS
  ldi txByte, 0x02
  rcall WH1602A_SEND_COMMAND
  rcall I2CM_STOP
  DELAY_MACRO 0x04, 0x30 ; TCCR0B (1<<CS02) - 265, 48, 1.53ms

  ldi ZH, high(2*WH1602A_symb)
  ldi ZL, low(2*WH1602A_symb)
  ldi tmp, 0x08
  mov R0, tmp

  _SEND_HALLO_BYTE:
    rcall I2CM_START
    rcall WH1602A_SEND_ADDRESS
    lpm txByte, Z+
    rcall WH1602A_SEND_DATA
    rcall I2CM_STOP
    DELAY_MACRO 0x03, 0x05 ; TCCR0B (1<<CS01)|(1<<CS00) - 64, 5, 39us
    dec R0
    brne _SEND_HALLO_BYTE

  ret


;/************************************ THE END *************************************/
THE_END:
  cli
  rjmp 0


.include "./inc/var.inc"
