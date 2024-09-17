/**********************************************************************
* © 2005 Microchip Technology Inc.
*
* FileName:        rtsApi.s
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       dsPIC33Fxxxx/PIC24Hxxxx
* Compiler:        MPLAB® C30 v2.01.00 or higher
*
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Inc. (“Microchip”) licenses this software to you
* solely for use with Microchip dsPIC® digital signal controller
* products. The software is owned by Microchip and is protected under
* applicable copyright laws.  All rights reserved.
*
* SOFTWARE IS PROVIDED “AS IS.”  MICROCHIP EXPRESSLY DISCLAIMS ANY
* WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE, OR NON-INFRINGEMENT. IN NO EVENT SHALL MICROCHIP
* BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL
* DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR EQUIPMENT, COST OF
* PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
* BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
* ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER SIMILAR COSTS.
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            Date      Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Settu D 			03/15/06  First release of source file
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
*
*
**********************************************************************/

; Select the device family
.equ __dsPIC33F__,1
;.equ __PIC24H__,1

.ifdef __dsPIC33F__
.include "p33fxxxx.inc"
.endif

.ifdef __PIC24H__
.include "p24hxxxx.inc"
.endif


.equ    FLASH_PAGE_ERASE_CODE, 	0x4042
.equ    FLASH_ROW_PROG_CODE, 	0x4001

	.global _flashPageRead
	.global _flashPageErase
	.global _flashPageWrite
	.global _flashPageModify

	.section .text

/******************************************************************************
  Flash Page Read 
  Read EIGHT rows (PAGE) of memory, upper PM byte is discarded
*******************************************************************************/
_flashPageRead:
        push    TBLPAG
        mov     w0, TBLPAG
        mov     #512, w3

readNext:     
		tblrdl  [w1],[w2++]
		tblrdh	[w1++],w6		; Discard PM upper byte
        dec     w3, w3
        bra     nz, readNext

		clr 	w0
 		pop     TBLPAG
        return

/******************************************************************************
  Flash Page Modify 
  Read EIGHT rows (PAGE) of memory, upper PM byte is discarded
*******************************************************************************/
_flashPageModify:
		sl 		w0,#7,w0 	
		add		w0,w3,w3	
				
modifyNext:     
		mov	   [w2++],[w3++]	; Discard PM upper byte
        dec     w1, w1
        bra     nz, modifyNext

		return

/******************************************************************************
  Flash Page Erase 
  Erase EIGHT rows (PAGE) of memory
*******************************************************************************/
_flashPageErase:
        push    TBLPAG

        mov     w0, TBLPAG		; Init Pointer to row to be erased
		tblwtl  w1,[w1]			; Dummy write to select the row

								; Setup NVCON for page erase
		mov   	#FLASH_PAGE_ERASE_CODE,w7
        mov     w7, NVMCON
		bset 	w7,#WR
		disi 	#5				; Block all interrupt with priority <7 for next 5 instructions	
		mov     #0x55, W0
        mov     W0, NVMKEY
        mov     #0xAA, W0
        mov     W0, NVMKEY		
		mov     w7,NVMCON		; Start Program Operation
        nop
        nop


erase_wait:     
		btsc    NVMCON, #WR
        bra     erase_wait

		clr 	w0
		pop     TBLPAG
        return


/******************************************************************************
  Flash Page Program
  Program EIGHT rows (PAGE) of memory, 
  Each row contains 192 bytes of data (64 instructions, with upper PM byte == NOP) 
*******************************************************************************/


_flashPageWrite:
        push    TBLPAG
        mov     w0, TBLPAG		; Init Pointer to row to be programed
		
		mov		#0,w6
		mov		#8,w5			; w5=row counter
row_loop:
	    mov 	#64,w3

pinst_loop: 
		tblwtl  [W2++],[W1]
		tblwth  w6,[W1++]		; load 0x00 into 3rd byte (will be decoded as NOP should PC attempt to execute)

        dec     w3, w3
        bra     nz, pinst_loop

								; Setup NVCON for row program
		mov   	#FLASH_ROW_PROG_CODE,w7
        mov     w7, NVMCON
		bset 	w7,#WR
		disi 	#5				; Block all interrupt with priority <7 for next 5 instructions	
		mov     #0x55, W0
        mov     W0, NVMKEY
        mov     #0xAA, W0
        mov     W0, NVMKEY		
		mov     w7,NVMCON		; Start Program Operation
        nop
        nop

prog_wait:     
		btsc    NVMCON, #WR
        bra     prog_wait

		dec     w5, w5
		bra     nz, row_loop

		clr 	w0
        pop     TBLPAG
        return


.end
