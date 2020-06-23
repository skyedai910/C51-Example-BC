;4 FUNCTION CALCULATOR PROGRAM
;Assumes 1.2MHz Clock for scan timing.



; TODO : Custom Character for the 'M' sign	
;	 Check instances of multiple decimal point presses ( all covered ?)

;Reset vector
		org 0000h
		jmp start

;Start of the program
		org 0100h

start:		mov A,#030h			;1 line, 8 bits
		call wrcmd
		mov A,#LCD_SETVISIBLE + 4
		call wrcmd
		mov A,#LCD_SETDDADDR+15		; Start at right hand side of the display
		call wrcmd
		mov A,#LCD_SETMODE + 3		; Automatic Increment - Display shift left.  
		call wrcmd

		mov 025h,#00h			; Set output mode (floating point).
		
		call boundsbuffer		; Initialise the bounds buffer - used for error checking.
		mov mode,#4			; Initialise the constant buffer to 100. Primarily used for % ops.
		mov digitcode,#031h
		call storedigit
		mov digitcode,#030h
		call storedigit
		mov digitcode,#030h
		call storedigit

		mov status,#00h			; variable used to determine the first key press after an operation.
		mov bufferctr,#00h
		mov opcounter,#00h
		mov decimalcnt,#00h
		call waitkey		
   
halt:		mov PCON,#1			;Halt


;***********************************************************
;**** Floating Point Package ****
;********************************

$INCLUDE (FP52.ASM)

;Routine to peek arg at DPTR
argout: 	mov R0,#FP_NUMBER_SIZE
aoloop:		movx A,@DPTR
		anl A,#0F0h
		rr a
		rr a
		rr a
		rr a
		add A,#aodata-$-3
		movc A,@A+PC
		call sndchr
		movx A,@DPTR
		anl A,#0Fh
		add A,#aodata-$-3
		movc A,@A+PC
		call sndchr
		inc DPTR
		djnz R0, aoloop
		ret	

aodata:	db '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'

;Routine to output character in A, preserving all but A.
sndchr:		push R0B0
		push R1B0
		call getmode
		mov digitcode,A
		call storedigit
		pop R1B0
		pop R0B0
		ret

;Routine to print error message at DPTR.
prterr: 	jmp wrstr

;Routine to handle input parameter error.
badprm:		mov DPTR,#bpmsg
		jmp wrstr

bpmsg:	db 'Bad Parameter',0


;***********************************************************
;**** LCD Display Routines ****
;******************************

;LCD Registers addresses
LCD_CMD_WR	equ 	00h
LCD_DATA_WR	equ	01h
LCD_BUSY_RD	equ	02h
LCD_DATA_RD	equ	03h

LCD_PAGE	equ	80h

;LCD Commands
LCD_CLS		equ	1
LCD_HOME	equ	2
LCD_SETMODE	equ	4
LCD_SETVISIBLE	equ	8
LCD_SHIFT	equ	16
LCD_SETFUNCTION	equ	32
LCD_SETCGADDR	equ	64
LCD_SETDDADDR	equ	128


;Sub routine to write null terminated string at DPTR in program ram.
wrstr:		mov P2,#LCD_PAGE
		mov R0,#LCD_DATA_WR
wrstr1:		clr A
		movc A,@A+DPTR
		jz wrstr2
		movx @R0,A
		call wtbusy
		inc DPTR
		jmp wrstr1
wrstr2:		ret

;Sub routine to write null terminated string at DPTR in program ram.
wrstrslow:	mov P2,#LCD_PAGE
		mov R0,#LCD_DATA_WR
wrstr1s:	clr A
		movc A,@A+DPTR
		jz wrstr2s
		movx @R0,A
		call wtbusy
		inc DPTR
		push DPL
		push DPH
		mov DPTR,#20
		call wtms
		pop DPH
		pop DPL	
		jmp wrstr1s
wrstr2s:	ret


;Sub routine to write custom character cell A
;with data at DPTR
wrcgc:		mov P2,#LCD_PAGE
		rl A
		rl A
		rl A
		add A,#LCD_SETCGADDR
		call wrcmd

		mov R0,#LCD_DATA_WR
		mov R2,#8
wrcgc1:		clr A
		movc A,@A+DPTR
		movx @R0,A
		call wtbusy
		inc DPTR
		djnz R2, wrcgc1
		ret

;Sub routine to write command:
wrcmd:		mov P2,#LCD_PAGE
		mov R0,#LCD_CMD_WR
		movx @R0,A
		jmp wtbusy
		
	
;Sub routine to write character:
wrdata:		mov P2,#LCD_PAGE
		mov R0,#LCD_DATA_WR
		movx @R0,A
		

;Subroutine to wait for busy clear
wtbusy: 	mov R1,#LCD_BUSY_RD
		movx A,@r1
		jb ACC.7,wtbusy
		ret

;Wait for number of seconds in A
wtsec:		push ACC
		call wtms
		pop ACC
		dec A
		jnz wtsec
		ret

;Wait for number of milliseconds in DPTR
wtms:           xrl DPL,#0FFh			;Can't do DEC DPTR, so do the loop by forming 2's complement
	        xrl DPH,#0FFh			;and incrementing instead.
	        inc DPTR
wtms1:	        mov TL0,#09Ch			;100 ticks before overflow
	        mov TH0,#0FFh	
	        mov TMOD,#1			;Timer 0 mode 1
	        setb TCON.4			;Timer 0 runs
wtms2:	        jnb TCON.5,wtms2	
	        clr TCON.4			;Timer 0 stops
	        clr TCON.5
	        inc DPTR
	        mov A,DPL
	        orl A,DPH
	        jnz wtms1
	        ret

;Subroutine to Center a String on one line of the Display ( 16 character Display )

CentreString:	mov R6,#0
		mov R4,strlength
		mov A,#16		 			; R4 holds the string length.
		subb A,R4     		 			; A holds the total *spare* character spaces.
		jc ExitSub		 			; Exit Routine if string is longer than display width.	                
		rrc A			 			; Shift right (Divide by 2)
		mov R6,A      		 			; R6 now holds the CentreOffset. 
		clr A
CharBuff:	mov A,#CHAR_SPACE				; Write the *padding* to center the string.	
        	call wrdata	
		cjne R6,#0,CharBuff
ExitSub:	ret				


;Subroutine to determine the length of a null terminated string.  

StringLength:	mov R4,#00
loop:		clr A
		movc A,@A+DPTR					
		inc dptr
		inc R4
		jnz loop
		mov strlength ,R4
		ret						; strlength includes the terminating NULL.

;Subroutine to write a null terminated string *wrapped* around an offset.

WriteString:    mov R0,#LCD_DATA_WR
		clr A
		mov A,stroffset					
		mov R5,A					; R5 is the counter.
		mov B,A					        ; B is the counter for the djnz		
                ;mov R7,#5
		mov R6,#16
loop1:		movc A,@A+DPTR                  		; Loop1 goes from the offset to the terminator or to 20 chars -> .
		jz eos_found
		movx @R0,A
		call wtbusy
		inc R5
		mov A,R5
		djnz R6,loop1					; R6 *holds* the string to 20 characters or on 1 line.

eos_found:	mov R5,#0
		mov R2,stroffset
                cjne R2,#0,loop2				; Check for case with no wrap.
		ret
				
loop2:  	mov A,R5					; Loop2 wraps from the first character to the offset.
		movc A,@A+DPTR			
		movx @R0,A
		call wtbusy					
	        inc R5
		djnz B,loop2
             
		push DPL					; Pause after writing the string.
		push DPH
		mov DPTR,#2
		call wtms
		pop DPH
		pop DPL	
		ret

					
;***********************************************************
;**** Keypad Routines ****
;*************************


XSEG 					        ; External Data Memeory - Access through DPTR.

;**** BUFFERS *****
; Buffers are set up with byte 0 as the sign - the relevent number of digits - and the final bit for the terminator.
; The exception to this is the Hundredbuffer which is *hardcoded* at 100. In practice this means that a ten byte
; buffer holds an 8 digit number (sign&number&terminator = 10 ). 

KEYBUFFER:    ds 10				; General I/O buffer. 
OLDNUMBUFFER: ds 10				; Holds the previous number ( used for repeat operations)
MEMORYBUFFER: ds 10				; Holds the number in memory
HUNDREDBUFF : ds 5				; Holds the constant number 100
BOUNDBUFFER:  ds 10				; Holds 99999999 and is signed so both upper and lower bounds can be checked.
TEMPBUFFER:   ds 25				; Holds the operation result until compared with boundsbuffer.

DSEG AT 060h					; Data memory.
	
;***** FLAGS *****
; Flags are used mainly because most of the operators have different functionality when consecutively pressed more than once.
; Status returns 1 after an operator press and 0 after a digit key press.

equalsflag:  	ds 1				; Flag for the equals operator.
memopflag:   	ds 1				; Flag for memory operations.
arithopflag: 	ds 1				; Flag for arithmetic operations.
pctopflag:   	ds 1				; Flag for the percentage operator.
memocc:	     	ds 1				; Flag whether there is a value in the memory buffer

errorflag:   	ds 1				; Flag an error.
signflag:    	ds 1				; Boolean for the sign of the number ( default to +ve ) 	
status:      	ds 1				; Flag the type of key pressed ( operator or digit ).

;***** VARIABLES *****

opcodehex:	ds 1				; Store the operation type.
oldopcode :	ds 1				; As above - must be able to store the last operation as well as the current
						; one for cancel command and consecutive operator presses.
opcodeflag:  	ds 1
bufferctr:   	ds 1				; A counter ( incremented along the buffer on storing a digit ).
opcounter:   	ds 1				; Count the number of operations since a ( total ) Cancel.
digitcode:   	ds 1				; Holds the ascii value of the key pressed.
mode:	     	ds 1				; Determines at which buffer the DTPR addresses.  
memcounter:  	ds 1				; Stores the length of the number currently in the memorybuffer
copyfrom:    	ds 1				; Used to copy the contents of one buffer into another buffer
copyto:		ds 1				; As above.
local1:    	ds 1				; Local1 variable
decimalcnt:	ds 1				; Counter for decimal points - don't allow more than 1 to be inputed per number.
stroffset:	ds 1				; Holds the offset of a string ( for centering purposes ).
strlength:	ds 1				; Holds the length of the string.
CSEG						; Return to code segment.

CHAR_SPACE  equ 0FEh

errorstr: db 'Error!'
	  db 0
string1:  db "YOU'RE JOKING!"
	  db 0

CGC1:	db 010001b				; Memory only.
	db 011011b
	db 010101b
	db 000000b
	db 000000b
	db 000000b
	db 000000b
	db 000000b

CGC2:	db 010001b				; Memory and error.
	db 011011b
	db 010101b
	db 011111b
	db 010000b
	db 011110b
	db 010000b
	db 011111b

CGC3:	db 000000b				; Error only.
	db 000000b
	db 000000b
	db 011111b
	db 010000b
	db 011111b
	db 010000b
	db 011111b
		

	 
;Keycodes returned for function keys:
ON		equ 	1
SGN		equ 	2
PCT		equ	3
SQR		equ 	4
MRC		equ	5
MADD		equ 	6
MSUB		equ	7

KEY_ROW1	equ	0EFh
KEY_ROW2	equ	0DFh
KEY_ROW3	equ	0BFh
KEY_ROW4	equ	07Fh

keyflags	equ	040h

;Data tables for returned row bits

 
keycodes:	db 	ON,  '7','8','9', '*', '/'
		db	SGN, '4','5','6', '-', MRC
		db	PCT, '1','2','3', 0,   MSUB
		db	SQR, '0','.','=', '+', MADD


;-----------------------------------------------------------------------------------------
; WAITKEY - Wait for a keypress, lift the key and display it on screen.
;-----------------------------------------------------------------------------------------

waitkey:	push DPH			; Preserve DPTR
		push DPL			;

		call initialize			; Initialise the keybuffer and the LCD display screen.
wk_keyscan: 	call keyscan			; Wait for key press
	 	jnz wk_wrchar		        ; Handle a pressed key
 		push DPH			; don't allow DPTR to be changed
		push DPL
		mov DPTR,#10			; Time delay to wait
 		call wtms			; Wait set time
		pop DPL
		pop DPH
		jmp wk_keyscan			; Check again


wk_wrchar:	call keytest			; Test the type of key pressed
		mov R5,opcodeflag
		cjne R5,#0,wk_ophandle		; Test whether key pressed is a digit or an operator.
		
;*DIGIT PRESS*:
		call statuscheck		; Determine whether this is the first digit pressed since an op press. 
		call storedigit			; Store the digit and inc bufferctr along the buffer.
		call bufferoutput		; Output the number to the LCD display.
		jmp wk_keyscan			; loop back to scan for next entry.


;*OPERATOR PRESS*:

wk_ophandle:	call getmode			; Determine at which buffer the DPTR addresses.
		call handleop			; Deal with the operator logic. 
		jmp wk_keyscan			; loop back and start again.

wk_done:	pop DPL				; Restore DPTR
		pop DPH	
		ret


;============================================
;********** KEYPRESS FUNCTIONS **************
;============================================


;-----------------------------------------------------------------------------------------
; KEYSCAN - Function to return current keypad state in A.
;-----------------------------------------------------------------------------------------


keyscan:	push DPH
		push DPL
		
		mov R0,#keyflags 		; R0 addresses the key toggle bytes
		mov R1,#KEY_ROW1		; R1 address the keyboard row address
		mov R2,#4			; R2 counts rows
ksrow:		mov P2,R1			; Set row address to port P2
		nop
		mov A,P1			; Read column data from port P1
		mov R3,#6			; R3 counts keys per row
		anl A,#3Fh
ks0:		rrc A				; Move next bit into carry
		mov R4,A			; R4 preserves the row data
		jc ks1				; Jump if key not pressed
		mov A,@R0			; Test if key already pressed
		mov @R0,#1			; Flag pressed anyway
		jz ksnew			; Jump if key newly pressed
		jmp ks2
ks1:		mov @R0,#0			; Flag key as not pressed
ks2:		inc R0				; Loop for next key in this row
		mov A,R4
		djnz R3,ks0

		mov A,R1			; Jiggle R1 to address next row
		rl A 
		mov R1,A
		djnz R2,ksrow

		clr A				; Return zero - no (new) key press.
		jmp ksend

						
ksnew:		mov DPTR,#keycodes		; We've found a new key since last time:
		mov A,R0			; The key flag address (ordinal) is in R0
		clr C
		subb A,#keyflags
		movc A,@A+DPTR
		mov digitcode,A			; digitcode now holds the ascii value of the key in (in hex)		
ksend:		mov P2,#0FFh
		pop DPL
		pop DPH
		ret


;--------------------------------------------------------------------------------------------------------
; KEYTEST - Function to test which type of key is pressed. digitcode holds the key information.
; 	    The digit range holding ascii 0 -> 9 is 030h -> 039h.
;	    Opcodeflag designated as the flag for *key type*.  
;---------------------------------------------------------------------------------------------------------

keytest:	mov R4,digitcode 	

		cjne R4,#030h,kt_testlower	; Test lower boundary of the *digit range*. carry is set if < 030h
		jmp kt_isdigit			; key is 030h so is a digit
kt_testlower:	jc kt_decimalpt			; Test the carry flag - if set then key is not a digit so goto op tests. 
		cjne R4,#039h,kt_testupper	; Test upper boundary of *digit range*. carry is set if < 039h	
		jmp kt_isdigit			; key is 039h so is digit.
kt_testupper:	jc kt_isdigit			; if carry set then within *digit range* so jump to kt_isdigit
kt_decimalpt:	cjne R4,#2Eh,kt_addtest		; allow decimal points.
		jmp kt_isdigit

kt_addtest:	cjne R4,#02Bh,kt_subtest	; Test the key info against ascii '+'
		mov opcodeflag,#1		; Key is the addition operator.					
		jmp kt_done
kt_subtest:     cjne R4,#02Dh,kt_multest	; Test the key against ascii '-'
		mov opcodeflag,#2		; Key is the subtraction operator.
		jmp kt_done
kt_multest:	cjne R4,#02Ah,kt_divtest	; Test the key against ascii '*'
		mov opcodeflag,#3		; Key is the multiply operator.
		jmp kt_done
kt_divtest:    	cjne R4,#02Fh,kt_cancel		; Test the key against ascii '/'
		mov opcodeflag,#4		; Key is the divide operator.
		jmp kt_done
kt_cancel:	cjne R4,#ON,kt_equals		; Test the key against the assigned value for the cancel button.
		mov opcodeflag,#5		; Key is the Cancel operator
		jmp kt_done
kt_equals:      cjne R4,#03Dh,kt_sign		; Test the key against ascii '='
		mov opcodeflag,#6		; Key is the equals operator.
		jmp kt_done
kt_sign:	cjne R4,#SGN,kt_mrc		; Test the key against ascii '=' 
		mov opcodeflag,#7               ; Key is the Sign operator.    
		jmp kt_done
kt_mrc:		cjne R4,#MRC,kt_mplus		; Test the key against ascii '=' 
		mov opcodeflag,#8               ; Key is the MRC operator.    
		jmp kt_done
kt_mplus:	cjne R4,#MADD,kt_msub		; Test the key against ascii '=' 
		mov opcodeflag,#9               ; Key is the M+ operator.    
		jmp kt_done
kt_msub:	cjne R4,#MSUB,kt_pcnt		; Test the key against ascii '=' 
		mov opcodeflag,#10              ; Key is the M- operator.    
		jmp kt_done
kt_pcnt:	cjne R4,#PCT,kt_sqr		; Test the key against ascii '=' 
		mov opcodeflag,#11              ; Key is the Percentage operator.    
		jmp kt_done
kt_sqr:		cjne R4,#SQR,kt_done
		mov opcodeflag,#12
		jmp kt_done
kt_isdigit:	mov opcodeflag,#0		; Key is a digit.
		jmp kt_done
kt_done:	ret


						
;======================================================
;************* OPERATOR FUNCTIONS *********************
;======================================================

;---------------------------------------------------------------------------------
;HANDLEOP - Subroutine to test whether the operator is arithmetic or not
;	    and to call the appropriate function handlers.
; 	    If opcodeflag < =4 then arithmetic op else functional op.
;---------------------------------------------------------------------------------
handleop:       clr c				; Clear the carry flag before a cjne instruction
		mov A,opcodeflag
		cjne A,#4,ho_testcarry		; Test operator against 4	
		jmp ho_arithmcall		; if 4 then arithmetic so jump

ho_testcarry:	jc ho_arithmcall		; if less than 4 then arithmetic 
      		call functionops		; otherwise call function ops.
		mov arithopflag,#00h		; If a functional op then clear arithopflag
		jmp ho_done	

ho_arithmcall:  call arithmeticop
ho_done:	ret


;=========================================
;********* ARITHMETIC OPERATORS **********
;=========================================


;---------------------------------------------------------------------------------
;ARITHMETICOP - Subroutine to handle the operator logic for arithmetic operations
;	        *opcodehex* is stored, *oldopcode* is retrieved.
;---------------------------------------------------------------------------------

arithmeticop:	push DPH			; Preserve the Datapointer.
		push DPL
		mov mode,#1			; DPTR addresses the Keybuffer.
		call getmode			
		
		clr c
		mov R5,arithopflag		; Test for consecutive Arithmetic Operator presses
		cjne R5,#1,ao_equalscheck	; If consecutive just store the op.
		jmp ao_store

ao_equalscheck:	clr c
		mov R5,equalsflag		; If *equ* - *arithmetic op* just store the op
		cjne R5,#1,ao_percentcheck	; The equals operation stores the result.
		jmp ao_store

ao_percentcheck:clr c
		mov R5,pctopflag     		; If *pct* - *arithmetic op* just store the op
                cjne R5,#1,ao_statuscheck         ; The percent operation stores the result.     
		jmp ao_store           

ao_statuscheck:	mov R5,status
		cjne R5,#1,ao_normalinput	; Test for *MRC*  - *aritmetic op* sequence
		mov mode,#1			 	
		mov R5,memcounter		; Memcounter holds the length of the number in the Memorybuffer.
		mov bufferctr,R5
		mov digitcode,#0		; Terminate the number.
		call storedigit
		mov mode,#1
		call inputnum			; Input the number.
		mov copyfrom,#1				 
		mov copyto,#3	
		call buffercopy			; Copy the number into the oldnumbuffer.
		jmp ao_store

ao_normalinput:	mov mode,#1			; Test for *digit* - *arithmetic op* sequence
		mov digitcode,#0		 
		call storedigit			; Terminate the number.
		mov mode,#1
		call inputnum			; Input the number.
		mov copyfrom,#1				 
		mov copyto,#3	
		call buffercopy			; Copy the number.

ao_countcheck:	inc opcounter			; opcounter holds the number of operations.
		mov R5,opcounter
		cjne R5,#1,ao_retrieve
		jmp ao_store			; if this is first op nothing to retrieve/output so goto ao_store

ao_retrieve:	mov mode,#1
		call getmode
		call retrieveop			; retrieve the op, execute the opereration 
						; and output the result to the Keybuffer and to the LCD display.
		mov mode,#1
		call inputnum			; Put the result back on the stack.
ao_store:   	call storeop			; store the op type in *opcodehex*

ao_setflags:	mov memopflag,#00h		; Clear/Set the appropriate flags.
		mov equalsflag,#00h
		mov pctopflag,#00h
		mov arithopflag,#01h		
		
ao_done:	mov status,#01h			; set status to indicate that an operator has been pressed.
		call resetsign
		pop DPL
		pop DPH
		ret

;---------------------------------------------------------------------------------
;STOREOP- Subroutine to store the operator. We store *opcodehex*. 
;---------------------------------------------------------------------------------

storeop:	mov R5,opcodeflag	
so_addition:	cjne R5,#1,so_subtract		; If Addition assign code		
		mov opcodehex,#02Bh
so_subtract:	cjne R5,#2,so_multiply		; If Subtraction assign code
		mov opcodehex,#02Dh
so_multiply:	cjne R5,#3,so_divide		; If Multiplication assign code
		mov opcodehex,#02Ah
so_divide:	cjne R5,#4,so_done		; If Division assign code
		mov opcodehex,#02Fh

so_done:	mov A,opcodehex			; Moves the op type into *oldopcode*.
		mov oldopcode,A			; This means on next op press oldopcode is the
						; old code and opcodehex is the new code.       
		ret	

;---------------------------------------------------------------------------------
;RETRIEVEOP - Subroutine to retrieve the operator. We retrieve *oldopcode*.
;---------------------------------------------------------------------------------

retrieveop:	mov R7,oldopcode		; use R7 locally here for the cjne
		clr A
		mov bufferctr,#00h
ro_addition:	cjne R7,#02Bh,ro_subtract	; Test for addition
		call floating_add		; Perform the operation
		call errorcheck			; Check for errors
		jmp ro_output
ro_subtract:	cjne R7,#02Dh,ro_multiply	; Test for subtraction
		call floating_sub		; Perform the operation
		call errorcheck                 ; Check for errors     
		jmp ro_output
ro_multiply:	cjne R7,#02Ah,ro_divide		; Test for multiplication
		call floating_mul		; Perform the operation
		call errorcheck                 ; Check for errors     
		jmp ro_output
ro_divide:	cjne R7,#02Fh,ro_output		; Test for division
		call floating_div		; Perform the operation
		call errorcheck                 ; Check for errors     
		jmp ro_output

ro_output:	mov R5,errorflag
		cjne R5,#0,ro_clear		; Test for errors.
		mov mode,#1			; No error so output result.
		call getmode
		call bufferclear
		mov bufferctr,#00h
		call floating_point_output	; output result both to LCD and to keybuffer.
		call bufferoutput
		jmp ro_done

ro_clear:	mov status,#1			; If an error occurs we clear everything 
		call cancelop			; ready to start again.
ro_done:	ret	



;===================================
;***** FUNCTION OPERATORS **********
;===================================


;---------------------------------------------------------------------------------
;FUNCTIONOPS - Subroutine to handle the non arithmetic operations.
; 	       Determine which functional op is pressed and the call the appropriate subroutine.	
;---------------------------------------------------------------------------------

functionops: 	mov R5,opcodeflag
fo_cancel:	cjne R5,#5,fo_equal
		call cancelop
		jmp fo_done
fo_equal:	cjne R5,#6,fo_signop
		call equalop
		jmp fo_done	
fo_signop:	cjne R5,#7,fo_mrc
		call signop
		jmp fo_done
fo_mrc:		cjne R5,#8,fo_memplus
		call memrecall
		jmp fo_done
fo_memplus:	cjne R5,#9,fo_memsub
		call memplus
fo_memsub:	cjne R5,#10,fo_pcnt
		call memsub		
fo_pcnt:	cjne R5,#11,fo_sqr
		call percentop
fo_sqr:		cjne R5,#12,fo_done
		call banner
		call cancelop
fo_done:	ret

;---------------------------------------------------------------------------------
;CANCELOP - Subroutine to handle the cancel operation.
;---------------------------------------------------------------------------------

cancelop:	push DPH			; Preserve the Datapointer. 
	 	push DPL
		
		mov R5,status			; Test for full or partial clear.
		cjne R5,#0,co_totalclear	

co_partclear:	mov mode,#1			; Partial Clear - Lose num2 and display num1
		call getmode			; This is Sequence:*num1*-*arithop*-*num2*-*cancel*
		call clearscreen		; Scrap the second number ( isn't on the stack here )  
		mov bufferctr,#00h		; Clear before FPO so we know the *size* of the resulting number.
		mov mode,#1
		call getmode		
		call floating_point_output	; Output the first number to the Keybuffer.	
		call bufferoutput		; Output the first number to the LCD Display.
		call inputnum			; Put the first number back on the stack.
		jmp co_setflags
	

co_totalclear:	mov mode,#1			; Total Clear  - Clear the stack and the Keybuffer.
		call getmode
		call floating_point_output	; Output the contents of the stack.	
		call clearscreen		; Clear the screen.
		call initialize			; initialise the Keybuffer and the LCD dislay.	
		mov oldopcode,#00		
		mov opcounter,#00 		; Start back at zero operations performed.
		call bufferclear		; Clear the Keybuffer.
		mov decimalcnt,#00h
		jmp co_setflags

co_setflags:	mov pctopflag,#00h		; Set/Clear the appropriate flags.
		mov memopflag,#00h
		mov equalsflag,#00h
		mov arithopflag,#00h

co_done:	call resetsign			; Reset the sign to it's default state ( positive )
		mov status,#01h			; Status = 1 'cause it's an operator press.
		pop DPL				; Restore the Datapointer.
		pop DPH
		ret
		
;---------------------------------------------------------------------------------
;EQUALOP - Subroutine to handle the equals operation.
;---------------------------------------------------------------------------------
		
equalop:	push DPH			; Preserve the Datapointer.
		push DPL	       
		mov mode,#1			; DPTR addresses the Keybuffer.
		call getmode

		mov R5,equalsflag		; Check for repeat Equals presses.
		cjne R5,#1,eo_stattest
		jmp eo_multiple			; If repeated goto multiple.
		

eo_multiple:	call clearscreen		; Repeat Operations.
		mov mode,#3			; Use oldnumbuffer (holding last number)
		call inputnum			; Input the last number.
		mov mode,#1			; Address the Keybuffer.
		call getmode		
		call retrieveop			; Perform the operation and output.	
		mov R5,errorflag		; Test for errors.
		cjne R5,#1,eo_input1
		mov errorflag,#0		; Error found - don't input the number. Clear errorflag ( of no further use ).
		jmp eo_setflags		
eo_input1:	call inputnum			; No error so we can input the number safely.	
		jmp eo_setflags	

eo_stattest: 	mov R5,status
		cjne R5,#1,eo_single 		; If status = 1 goto single else test the operator flags.		

eo_pcttest:	mov R5,pctopflag		; If percentage op then treat the same as multiple press.
		cjne R5,#1,eo_memory
		jmp eo_multiple

eo_memory:	mov mode,#1			; If memory press adjust the bufferctr accordingly.
		mov R5,memcounter
		mov bufferctr,R5		; Run through to eo_single

eo_single:	mov digitcode,#0		; Single Equals.
		call storedigit			; Terminate the  ( second )number.
		mov mode,#1
		call inputnum			; Input the second (second )number. 
		mov copyfrom,#1			
		mov copyto,#3
		call buffercopy			; Copy the (second) number.
		call retrieveop			; Execute the operation and output the result.
				
		mov R5,errorflag
		cjne R5,#1,eo_input		; Test for errors.
		mov errorflag,#0		; Error found. Don't put result back on stack.
		jmp eo_setflags			; Clear the errorflag - we don't need it anymore.
eo_input:	call inputnum			; No error - put result back on stack and continue.	
		
eo_setflags:	mov equalsflag,#01h		; Set/Clear the appropriate flags.
		mov memopflag,#00h
		mov arithopflag,#00h
		mov pctopflag,#00h

eo_done:        call resetsign			; Reset the sign to it's default state ( positive ).
		mov status,#01h			; Operator press so Status = 1.
		pop DPL
		pop DPH				; Reatore the Datapointer.
		ret
		
		
;---------------------------------------------------------------------------------
;SIGNOP - Subroutine to change the sign of the number entered. This is a special case -
;         We treat this as a digit press and not as an operator press. This is a bit of a
;	  fudge to ensure that we get the proper visual output on the LCD screen after the next
;	  keypress ( Screen is cleared on a *op* - *digit* sequence which we don't want here ). 
;---------------------------------------------------------------------------------
signop:		mov mode,#1				; DPTR addresses the Keybuffer.
		call getmode
		
		mov R4,memopflag			
		cjne R4,#1,so_initialize		; Test for MRC press immediately prior to this.
		mov R4,memcounter			; MRC pressed so adjust the bufferctr accordingly.
		mov bufferctr,R4
		
so_initialize:	mov R2,signflag				; R2 holds the signflag
		mov R7,bufferctr			; Preserve the bufferctr in R7
		
so_toggle:	cjne R2,#0,so_negative			; If R2 = 0 then positive else negative
		mov R2,#1				; If positve change to negative
		jmp so_continue
so_negative:	mov R2,#0				; If negative change to positive
		
so_continue:	cjne R2,#0,so_enterneg
		mov digitcode,#020h			; If positive output a space.
		mov bufferctr,#00h			; Output in buffer position 0.
		call storedigit
		jmp so_status
		
so_enterneg:	mov digitcode,#02Dh			; If negative output a minus sign.
		mov bufferctr,#00h			; Output in buffer position zero.
		call storedigit
		
so_status:	mov R4,status				; If the status is one we need to change it to zero
		cjne R4,#1,so_complete			; for a correct visual display on the next keypress
		mov status,#0				; If status is zero we do not change anything.
 							
so_complete:	mov bufferctr,R7			; Restore the bufferctr.
		call bufferoutput			; Output the buffer to the LCD display.
		mov signflag,R2				; Set the signflag.
		ret
		
;---------------------------------------------------------------------------------
;PERCENTOP - Subroutine to work out the percentage.Arithmetically (num1*num2/100) is good for
;	     +,- ,* but we use (num1*100)/num2 to set up the divide case. Ordering the inputs on the 
;	     stack is important for the non - commutitive operators ( -,/).
;---------------------------------------------------------------------------------
percentop:	mov R4,memopflag			; Test for *num1* - *op* - *mrc* - *pct*
		cjne R4,#1,po_stattest	
		jmp po_continue

po_stattest:	mov R4,status				; Test for *num1* - *op* - *num2* -*pct*
		cjne R4,#1,po_continue					
		ret					; If *op* - *pct* do nothing.
po_continue:	mov R5,oldopcode
		cjne R5,#02Fh,po_standard		; Test for Divide Case. 		
		mov mode,#4				; Address the Hundred buffer
		call inputnum				; Input 100.		
		call floating_mul			; Gives us (num1 * 100).

		mov R5,memopflag
		cjne R5,#1,po_divok
		mov mode,#1
		mov R5,memcounter			; Adjust the bufferctr for memory buffer contents.
		mov bufferctr,R5
po_divok:	mov digitcode,#0			; Terminate the number.
		call storedigit
		mov mode,#1				; Address the Keybuffer.	
		call inputnum				; Input the number.
		call floating_div			; Divide.
		call floating_point_output		; Output in Keybuffer.
		call bufferoutput			; Output to LCD Display.
		call inputnum				; Put result back on the stack.
		jmp po_setflags				; End up with (num1*100) / num2.	
		
po_standard:	mov R5,memopflag
		cjne R5,#1,po_stok
		mov mode,#1	
		mov R4,memcounter			; Adjust the bufferctr as appropriate.	
		mov bufferctr,R4
		mov digitcode,#0			; Null terminate.
		call storedigit
		
po_stok:	mov mode,#1				; We can do this for the other 3 cases.
		call inputnum
		call floating_mul
		mov mode,#4
		call inputnum
		call floating_div			; We now have (num1*num2/100)
		

;Multiplication: We effectively have what we want from above. i.e (num1*num2/100 ).

		mov R5,oldopcode
		cjne R5,#02Ah,po_sub			; Test for Multiplication.
		mov mode,#1				; Address the Keybuffer.
		call getmode				
		call floating_point_output		; Output to the Keybuffer.
		call bufferoutput			; Output to the LCD display.
		mov digitcode,#0
		call storedigit				; Terminate.
		call inputnum				; Put result back on the stack.
		jmp po_setflags
		
; Subtraction:	We need to rearrange the numbers on the stack into the proper order before executing.
		
po_sub:		cjne R5,#02Dh,po_add			; Test for Subtraction.	
		mov mode,#1				; Address the Keybuffer.
		call getmode				
		call floating_point_output		; Output to the Keybuffer.
		mov mode,#3				; Address the oldnumbuffer.
		call inputnum				; Input the number 
		mov mode,#1				; Address the keybuffer.
		call inputnum				; Input the number.
		call retrieveop				; Execute the operation.
		call inputnum				; Input the result.
		jmp po_setflags
		
; Addition : Input the number from oldnumbuffer and execute.

po_add:		cjne R5,#02Bh,po_done			; Test for Addition.
		mov mode,#3				; Address the oldnumbuffer.
		call inputnum				; Input.
		mov mode,#1				; Address the Keybuffer.
		call getmode		
		call retrieveop				; Execute the operation and output.
		call inputnum				; Put the result back on the stack.
		jmp po_setflags
		
po_setflags:   	mov memopflag,#00h			; Set/Clear the appropriate flags.
		mov equalsflag,#00h
		mov arithopflag,#00h
		mov pctopflag,#01h
		
po_done:        mov status,#01h				; Set status as it's an operator press.
	        mov mode,#1
		call getmode				; Address the Keybuffer.
		ret
		
		
		
;============================================
;************ MEMORY OPERATORS **************
;============================================



		
;------------------------------------------------------------------------------
;MEMRECALL  - 1 press recalls the memory buffer - 2 consecutive presses clears
;	      the memory buffer.Memory Contents are stored in the MemoryBuffer. 				
;------------------------------------------------------------------------------

memrecall: 	mov R5,memocc			; Test for an empty buffer 
		cjne R5,#0,mr_validate		; If the memory buffer is empty and MRC is pressed
		mov mode,#2			; then initialise the buffer to zero and output to screen. 
		call getmode
		call bufferclear
		mov bufferctr,#00h
		mov digitcode,#030h
		call storedigit
		call bufferoutput
		jmp mr_setflags

mr_validate:	mov R5,memopflag
		cjne R5,#0,mr_consecutive	; test for two consecutive mrc presses.

		mov mode,#2			; Display the contents of the memorybuffer
		call getmode
		mov R4,memcounter		; Adjust the bufferctr.
		mov bufferctr,R4
		call bufferoutput		; Output to the LCD display.	
		
		mov mode ,#1			; Address the Keybuffer.
		call getmode			; Note that we do not input the number onto the stack here
		call bufferclear		; but rather copy it into the Keybuffer. The Stack handling operations 
		mov copyfrom,#2			; are handled inside the arithmetic op subroutine  or the percent subroutine
		mov copyto,#1			; which are the only cases where we might need this number on the stack.
		call buffercopy
		mov memopflag,#1		; Set the flag to indicate a MRC press. 
		jmp mr_setflags

mr_consecutive: mov mode,#2			; Consecutive MRC presses so clear the memorybuffer.
		call getmode
		call bufferclear
		mov status,#1
		call cancelop
		mov memopflag,#0		; Nothing in Memorybuffer so reset.
		mov memocc,#0			
		
mr_setflags:	mov equalsflag,#00h		; Clear opflags as appropriate.
		mov arithopflag,#00h
		mov pctopflag,#00h		


mr_done:	mov mode,#1			; Address the Keybuffer
		call getmode
		mov bufferctr,#00h		; Clear the Bufferctr.
		mov status,#1			; Operator press so set Status = 1.
		ret


;---------------------------------------------------------------------------------
;MEMPLUS - Subroutine to add the entered number to the value in the memory buffer.
;---------------------------------------------------------------------------------
memplus: 	mov mode,#2			; Address the MemoryBuffer as we are as performing 
		call getmode			; addition on it's contents.	
		
		call inputnum			; Input the contents of the memorybuffer (num1)
		call bufferclear
		
		mov mode,#1			; Input the number entered thru keypad  (num2)	
		call inputnum
		call bufferclear		; Clear the Keybuffer

		call floating_add		; Perform num1+num2 and output to memorybuffer
		mov mode,#2			; Address Memorybuffer.
		call getmode	
		call floating_point_output	
		mov R3,bufferctr
		mov memcounter,R3		; Take the length of the outputed number and put in memcounter.
		
		mov memocc,#1			; Indicate that a number is now in the memorybuffer. 
		
		mov mode ,#1			; Copy the result back into the keybuffer and display.
		call getmode
		call bufferclear			
		mov copyfrom,#2
		mov copyto,#1
		call buffercopy
		call bufferoutput				

mp_setflags:	mov memopflag,#00h		; Clear the appropriate flags.
		mov equalsflag,#00h
		mov arithopflag,#00h
		mov pctopflag,#00h	

mp_done:	mov status,#1			; Operator press so set Status = 1.
		ret	

;--------------------------------------------------------------------------------------
;MEMSUB - Subroutine to subtract the entered number from the value in the memorybuffer.
;---------------------------------------------------------------------------------------

memsub: 	mov mode,#2			
		call inputnum			;Input the contents of the memorybuffer (num1)
		call bufferclear		
		
		mov mode,#1			; Input the number entered thru keypad  (num2)	
		call inputnum
		call bufferclear

		call floating_sub		; Perform  num1 - num2		
		mov mode,#2
		call getmode
		call floating_point_output	
		mov R3,bufferctr
		mov memcounter,R3
		
		mov memocc,#1			; Indicate that a number is now in the memorybuffer. 
		
		mov mode ,#1			; Copy the result back into the keybuffer and display.
		call getmode			
		mov copyfrom,#2
		mov copyto,#1
		call bufferclear
		call buffercopy
		call bufferoutput				

ms_setflags:	mov memopflag,#00h
		mov equalsflag,#00h
		mov arithopflag,#00h
		mov pctopflag,#00h

ms_done:	mov status,#1
		ret	


;======================================
;******* UTILITY FUNCTIONS ************
;======================================

;---------------------------------------------------------------------------------
;STOREDIGIT - Subroutine to store a digit. DPTR addresses the KEYBUFFER.
;---------------------------------------------------------------------------------
storedigit:	call getmode	
		push DPH
		push DPL
		mov R5,bufferctr		; move the buffercounter into R5
		
		mov R4,mode			; If we are going into the errorbuffer we allow 25 digits
		cjne R4,#6,sd_entry		; This allows accurate comparison for bounds checking.
		cjne R5,#24,sd_test0
		jmp sd_continue
sd_test0:	jc sd_continue
		jmp sd_done

sd_entry:	cjne R5,#8,sd_test		; test to be sure that a maximum of 8 digits go in the buffer (sign + 8)
		jmp sd_decimal  
sd_test:	jc sd_decimal   
		
		
sd_decimal:	mov R4,digitcode		; Test for decimal points - only  allow one per number to be inputed.
		cjne R4,#02Eh,sd_continue
		inc decimalcnt			; We need to see whether this is the first decimal point to be inputed in this number.
		mov R3,decimalcnt		; This will be reset in the inputnum subroutine when we are finished with the number.
		cjne R3,#1,sd_done		; It is a decimal point and it is not the first so don't store it.
						; Fall through to sd_continue if it is the first decimal point.				
sd_continue:	cjne R5,#0,sd_loop		; If it is zero goto write
		jmp sd_write			;

sd_loop:	inc DPTR	
		djnz R5,sd_loop			; increment DPTR to DPTR + bufferctr
		
sd_write:	mov A,digitcode
		movx @DPTR,A			; Write the digit into the Buffer
		inc bufferctr			; we write digits from bufferctr pos 1 - 8.
						; Buffer position 0 is reserved for the sign.
			
sd_done:	pop DPL
		pop DPH
	        ret
;---------------------------------------------------------------------------------
;STATUSCHECK - Subroutine to test if this is the first key pressed after an operation
;	       and if so to clear the screen.	
;---------------------------------------------------------------------------------
statuscheck:	mov R2,status			
		cjne R2,#1,sc_done		
		mov R2,equalsflag
		cjne R2,#1,sc_clear

		mov R4,digitcode		; This caters for num - op - num - equ - num. A number after an equals 
		mov local1, R4			; signifies a new calculation.
		mov mode,#3
		call getmode
		call floating_point_output
		call bufferclear
		mov opcounter,#0
		mov bufferctr,#00h
		mov digitcode,#020h
		call storedigit
		mov R4,local1
		mov digitcode,R4
		mov mode,#1
		call getmode
		mov decimalcnt,#00h
sc_clear:	mov status,#00h		
			
				
sc_setflags:	mov memopflag,#00h
		mov equalsflag,#00h
		mov arithopflag,#00h
		mov pctopflag,#00h
sc_done:	ret				; clear status to indicate that a digit key has been pressed.
;---------------------------------------------------------------------------------
;BUFFEROUTPUT - Subroutine to write the keybuffer onto the screen.
;---------------------------------------------------------------------------------
bufferoutput:	push DPH			; preserve the Datapointer
		push DPL
		
		call clearscreen		; clears the screen and sets the LCD address to the far right.
		call getmode			; point at the keybuffer
		mov R3,bufferctr     		; We know the length of the number from the bufferctr
		mov bufferctr,#0
		mov R5,#0

bo_start:	mov R7,bufferctr
		clr A
bo_output:	movx A,@DPTR			; read the digit into A
		;cjne A,#02Eh,bo_write		; if a decimal point don't count that as one of the 8 output chars.
		;dec R5
		;jmp bo_write

bo_write:	call wrdata			; write out the digit.				
		
bo_test:	clr c				; need to clear the carry before the subb instruction.
		mov  A,R3			; test for the end of the string.
		subb A,R7
		mov R7,A			
		clr A
	        cjne R7,#01h,bo_test2		
		jmp bo_sign

		
bo_test2:   	cjne R5,#8,bo_test3
		jmp bo_increment			
bo_test3:	jc bo_increment
		jmp bo_sign

bo_increment:	inc bufferctr
		inc R5
		inc DPTR
		jmp bo_start
		
bo_sign:	mov R4,memocc
		cjne R4,#1,bo_done
		mov A,R3			; Draws the 'M' on the far left when there is a number
		add A,#LCD_SETDDADDR + 1	; in the memory buffer. We need to offset by the amount of the
		call wrcmd			; buffercounter because of the *shift* mode we have the LCD display
		mov A,#04Dh			; set up in.
		call wrdata

bo_done:	mov bufferctr,R3
		pop DPL
		pop DPH
		ret
;---------------------------------------------------------------------------------
;BUFFERCLEAR - Subroutine to clear the keybuffer.
;---------------------------------------------------------------------------------
bufferclear:	push DPH
		push DPL
		call getmode
		mov R2,#9			; Clear the buffer.
		mov A,#0					
bc_loop:	movx @DPTR,A			; Write a zero into the buffer position addressed by DPTR
		inc DPTR			; move the Datapointer along the databuffer. 
		djnz R2,bc_loop			; loop through the bufferlength		
		pop DPL
		pop DPH
		ret
;---------------------------------------------------------------------------------
;CLEARSCREEN - Subroutine to clear the screen and set the writing to the RHS.
;---------------------------------------------------------------------------------
clearscreen:	mov A,#LCD_CLS
		call wrcmd
		mov R4,memocc			; We need to account for the extra digit outputed ( 'M' ) when
		cjne R4,#1,cs_standard		; there is a number in memory. Due to the mode that we have set the 
		mov A,#LCD_SETDDADDR + 16	; display in (i.e. shifts display left) we need ddaddress set one further  
		call wrcmd			; right in this case.
		jmp cs_done
cs_standard:	mov A,#LCD_SETDDADDR + 15	; Standard case when nothing is in memory (output number only ) 
		call wrcmd
cs_done:	ret
;---------------------------------------------------------------------------------
;BUFFERCOPY - Subroutine to copy the keybuffer contents into  oldnumbuffer.
;---------------------------------------------------------------------------------
buffercopy:	push DPH			; Preserve the DataPointer
		push DPL
		mov R7,bufferctr		; Preserve the Bufferctr
		mov bufferctr,#00h
		mov R2,#8			; Set the counter to the buffer size

bc_transfer:	
		mov R5,bufferctr

		mov R6,copyfrom			; Get the copy info. 
		mov mode,R6			; ( set to a mode depending on which buffer we wish to access )
		call getmode			

		cjne R5,#0,bc_address1
		jmp bc_readin

bc_address1:	inc DPTR
		djnz R5,bc_address1

bc_readin:	movx A,@DPTR

		;cjne A,#020h,bc_continue
		;jmp bc_increment

bc_continue:	mov R6,copyto
		mov mode,R6
		call getmode

		mov R5,bufferctr
		cjne R5,#0,bc_address2
		jmp bc_writeout
bc_address2:	inc DPTR
		djnz R5,bc_address2

bc_writeout:	movx @DPTR,A 

bc_increment:	inc bufferctr			; loop through the Buffersize .
		djnz R2,bc_transfer
		
		mov bufferctr,R7		; Restore Bufferctr
		mov mode,#1
bc_done:        pop DPL
		pop DPH
		ret
;---------------------------------------------------------------------------------
;INPUTNUM - Subroutine to push the number onto the stack.
;---------------------------------------------------------------------------------
inputnum:	call getmode			; move the DPTR back to the beginning of the appropriate buffer
		call floating_point_input	; move the contents of the keybuffer onto the floating point stack	
		mov bufferctr,#00h		; move the buffercounter back to zero ready for the next operation
		mov decimalcnt,#00h		; Reset the decimal point counter for the next number.
		ret
;---------------------------------------------------------------------------------
;INITIALIZE - Subroutine to initialize the calc on startup.
;---------------------------------------------------------------------------------
initialize: 	mov mode,#1			; set the mode to default ( DPTR, points at the KEYBUFFER )
		call getmode			; Set the DPTR to address the appropriate buffer.
		call bufferclear
		mov bufferctr,#00h
		mov digitcode,#020h		; initialise with a space in position 0 to indicate a positive number.
		call storedigit
	
		mov signflag,#0
		mov status,#1			; start with status = 1 so if an op is pressed first 0 becomes the first no
						; on the stack - if a digit is pressed first the zero is trashed.
		call clearscreen		; sets the ddaddress to be at  the RHS
		mov digitcode,#30h		; start with 0 on the screen
		call storedigit
		call bufferoutput
		mov R4,bufferctr
		dec R4
		mov bufferctr,R4
		
		ret				
;---------------------------------------------------------------------------------
;RESETSIGN - Subroutine to ensure that every number starts as being positive.
;---------------------------------------------------------------------------------
resetsign:	call getmode			; point at the buffer to clear.
		call bufferclear
		
		mov bufferctr,#00h		; Clear the signfrom the buffer and set the buffer position.
		mov digitcode,#020h		; StoreDigit increments the buffer position *AFTER* storage.
		call storedigit			; This leaves us with position 0 clear and bufferctr set to 1.
		mov signflag,#00h
rs_done:	ret
;----------------------------------------------------------------------------------------------
;GETMODE - Subroutine to point the Datapointer at the required buffer - dependant on the mode.
;----------------------------------------------------------------------------------------------
getmode:	mov R4,mode
		cjne R4,#1,gm_memory
		mov DPTR,#KEYBUFFER
		jmp gm_done

gm_memory:	cjne R4,#2,gm_oldnum
		mov DPTR,#MEMORYBUFFER
		jmp gm_done

gm_oldnum:	cjne R4,#3,gm_const
		mov DPTR,#OLDNUMBUFFER
		jmp gm_done

gm_const:	cjne R4,#4,gm_bounds
		mov DPTR,#HUNDREDBUFF
		jmp gm_done
gm_bounds:	cjne R4,#5,gm_temp
		mov DPTR,#BOUNDBUFFER
		jmp gm_done
gm_temp:	cjne R4,#6,gm_done
		mov DPTR,#TEMPBUFFER			
gm_done:	ret

;----------------------------------------------------------------------------------------------
;BOUNDSBUFFER- Generate a buffer with the maximum permissable value i.e 99999999
;----------------------------------------------------------------------------------------------

boundsbuffer:	push DPH
		push DPL
		mov mode,#5
		mov R3,bufferctr		; Preserve the bufferctr.
		mov bufferctr,#0
		mov digitcode,#020h
		call storedigit			
bb_loop:	mov digitcode,#039h		; Enter  digit 9.
		call storedigit
		mov  R4,bufferctr
		cjne R4,#9, bb_loop
		mov digitcode,#0
		call storedigit
		mov bufferctr,R3
		
bb_done:	pop DPL
		pop DPH
		ret


;----------------------------------------------------------------------------------------------
;ERRORCHECK - Checks the upper and lower bounds and divide by zero. 
;----------------------------------------------------------------------------------------------

errorcheck:	jb ACC.3,ec_divide		; Result is on the stack.
		mov errorflag,#01h		; We set the error flag here and clear it if appropriate later.
		mov mode,#6			; output and then input result
		call getmode
		mov bufferctr,#0
		call floating_point_output
		call inputnum

		mov mode,#5
		mov bufferctr,#0
		mov digitcode,#020h
		call storedigit
		call inputnum			; input the UBound on the stack
		clr c
		call floating_comp		; Call floating_compare ( pops twice and returns status ).
		jc ec_lower			; Carry set so less than Ubound
		jmp ec_upperr			; Otherwise error - result too large.
		
ec_lower:	mov mode,#6
		call inputnum			; input result
		
		mov mode,#5			; make the max number negative ( i.e. the lower bound )
		mov bufferctr,#0
		mov digitcode,#02Dh
		call storedigit
		mov mode,#5
		call inputnum			; input the lower bound.	

		clr c
		call floating_comp
		jc ec_lowerr			; Error - result too low
		
		jmp ec_ok			; Greater than lower bound so o.k.

ec_divide:	call clearscreen
		mov DPTR,#errorstr		; Error message.
		call wrstr
		mov DPTR,#500
		call wtms
		mov mode,#6
		call inputnum			; input result.
		mov mode,#1
		call getmode
		mov status,#1
		jmp ec_done

ec_upperr:	call clearscreen
		mov DPTR,#errorstr		; Error message.
		call wrstr
		mov DPTR,#500
		call wtms
		mov mode,#6
		call inputnum			; input result.
		mov mode,#1
		call getmode
		mov status,#1
		jmp ec_done

ec_lowerr:	call clearscreen
		mov DPTR,#errorstr		; Error Message
		call wrstr
		mov DPTR,#500
		call wtms
		mov mode,#6
		call inputnum			; input result.
		mov mode,#1
		call getmode
		mov status,#1
		jmp ec_done


ec_ok:		mov mode,#6
		call inputnum			; input result.
		mov errorflag,#00h
	
ec_done:	mov mode,#6
		call getmode
		call bufferclear
		mov mode,#1
		call getmode
		ret


;----------------------------------------------------------------------------------------------
;BANNER - Exports a wraparound banner to the LCD screen. 
;----------------------------------------------------------------------------------------------

Banner:		
	
		call clearscreen
		mov A,#LCD_SETDDADDR+16		; Start at right hand side of the display
		call wrcmd
		
Reloop:		mov DPTR,#STRING1        			
                
Iterate:	call wrstr

		mov DPTR,#1000
		call wtms
	
		call clearscreen
		ret

END