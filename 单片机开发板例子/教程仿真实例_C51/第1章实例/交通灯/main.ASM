ORG 0000H
A_BIT EQU 20H ;用于存放南北十位数
B_BIT EQU 21H ;用于存放南北十位数
C_BIT EQU 22H ;用于存放东西十位数
D_BIT EQU 23H ; 用于存放东西位数


TEMP1 EQU 24H  ;用于存放第一二南北状态要显示的时间
TEMP2 EQU 25H  ;用于存放第一二东西状态要显示的时间

TEMP3 EQU 26H ;用于存放第三第四南北状态要显示的时间
TEMP4 EQU 27H;用于存放第三第四南北状态要显示的时间
LJMP MAIN

ORG 0003H     ;外部中断0入口
LJMP INTA     ;跳转到外部0中断
ORG 0013H     ;外部中断1入口
LJMP INTB     ;跳转到外部1中断

INTA: MOV A,P1   ;外部0中断
      PUSH ACC
      MOV A,P2  ;中断保护
      PUSH ACC
MOV P1,#0FFH    ;清除先前状态
MOV P2,#0FFH
CLR P1.0
CLR P1.4      ;南北通行，东西禁止通行
CLR P1.6
CLR P2.3
JNB P3.2 ,$  ;判断是否还在中断状态
POP ACC
MOV P2,A     ;返回中断前状态
POP ACC
MOV P1,ACC
RETI          ;中断返回



INTB:MOV A,P1  ;外部1中断
     PUSH ACC   ;中断保护
     MOV A,P2
     PUSH ACC
MOV P1,#0FFH  ;清除先前状态
MOV P2,#0FFH
CLR P1.2
CLR P2.1
CLR P1.3    ;东西通行，南北禁止通行
CLR P1.5
JNB P3.3 ,$  ;判断是否还在中断状态
POP ACC
MOV P2,A    ;返回中断前状态
POP ACC
MOV P1,A
RETI         ;中断返回



MAIN:
ORG 0100H        ;初始情况
MOV P1,#0FFH
MOV P2,#0FFH   ;灭所有灯
MOV TMOD,#55H  ;计数方式方式1
MOV IE,#85H    ;开中断
MOV TEMP1,#20 ;
MOV TEMP2,#25
MOV TEMP3,#25
MOV TEMP4,#20

STAR:
MOV P1,#0FFH
MOV P2,#0FFH   ;灭所有灯
MOV A,24H    ;将显示时间送A
CJNE A,#20,T40T  ;判断时间，选初始值
T20T:       ;南北通行要显示的时间为20的计数器初始值
CLR TF0   ;清TF0
CLR TF1 ;清TF1
MOV TH1 ,#0FFH ;送20秒时的初始值
MOV TL1 ,#0FCH ;在些设计20秒6辆为多车
MOV TH0 ,#0FFH
MOV TL0 ,#0FCH
LJMP TEMP20   ;跳到20秒
T40T:     ;南北通行要显示的时间为40的计数器初始值
CLR TF0 ;清TF0
CLR TF1 ;清TF1
MOV TH1,#0FFH ;送40秒时的初始值
MOV TL1 ,#0F8H ;在些设计40秒8辆为多车
MOV TH0 ,#0FFH
MOV TL0 ,#0F8H
LJMP TEMP40  ;跳到40秒



TEMP20:     ;TEMP1=20情况
SETB TR0    ;开始计数
SETB TR1
CLR P1.2
CLR P2.1    ;南北通行，东西禁止通行
CLR P1.3
CLR P1.5
MOV TEMP1,#20  ;南北要显示的时间，
MOV TEMP2,#25 ;东西要显示的时间
STLOP:
ACALL DISPLAY1   ;调用显示
DEC TEMP1      ;时间够一秒显示时间减1
DEC TEMP2
MOV A,TEMP1
CJNE A,#0,NEXT ;若显示时间不为0保持现在状态
LJMP STAR2     ;若显示时间为 0跳到第二状态
NEXT: LJMP STLOP


STAR2:     ;状态1
SETB P1.2
CLR P1.1    ;南北黄灯，东西禁止通行
SETB P1.3
CLR P1.4
MOV TEMP1,#05  ;南北要显示的时间，
MOV TEMP2,#05 ;东西要显示的时间，

STLOP2:
ACALL DISPLAY1   ;调用显示
DEC TEMP1      ;时间够一秒显示时间减1
DEC TEMP2
MOV A,TEMP1
CJNE A,#0,NEXT2   ;若显示时间不为0保持现在状态
JB TF1 ,T40      ;判断南北是否多车
JB TF0 ,T40       ;判断北南是否多车
MOV TEMP1,#20    ;少车下次显示时间为20秒
LJMP STAR3       ;跳到状态3
T40:
MOV TEMP1,#40    ; 多车下次显示时间为40秒
LJMP STAR3        ;若显示时间为 0跳到第三状态
NEXT2:LJMP STLOP2


TEMP40:;TEM=40 程序
SETB TR0  ; 开始计数
SETB TR1
CLR P1.2
CLR P2.1    ;南北通行，东西禁止通行
CLR P1.3
CLR P1.5
MOV TEMP1,#40  ;南北要显示的时间，
MOV TEMP2,#45 ;东西要显示的时间
STLOP11:
ACALL DISPLAY1   ;调用显示
DEC TEMP1      ;时间够一秒显示时间减1
DEC TEMP2
MOV A,TEMP1
CJNE A,#0,NEXT11 ;若显示时间不为0保持现在状态
LJMP STAR22     ;若显示时间为 0跳到第二状态
NEXT11: LJMP STLOP11


STAR22:     ;状态1
SETB P1.2
CLR P1.1    ;南北黄灯，东西禁止通行
SETB P1.3
CLR P1.4
MOV TEMP1,#05  ;南北要显示的时间，
MOV TEMP2,#05 ;东西要显示的时间，

STLOP22:
ACALL DISPLAY1   ;调用显示
DEC TEMP1      ;时间够一秒显示时间减1
DEC TEMP2
MOV A,TEMP1
CJNE A,#0,NEXT22   ;若显示时间不为0保持现在状态
JB TF1 ,T401      ; 判断是否多车
JB TF0 ,T401
MOV TEMP1,#20  ;少车下次显示时间为20秒
LJMP STAR3
T401:MOV TEMP1,#40 ;多车下次显示时间为40秒
LJMP STAR3        ;若显示时间为 0跳到第三状态
NEXT22:LJMP STLOP22


STAR3:
MOV A,26H
CJNE A,#25,T40T1  ;判断时间，选初始值
T20T1:       ;南北通行要显示的时间为20的计数器初始值
CLR TF0        ; 清溢出位
CLR TF1
MOV TH1 ,#0FFH  ; 给初值
MOV TL1 ,#0FCH
MOV TH0 ,#0FFH
MOV TL0 ,#0FCH
LJMP TEMP320
T40T1:     ;南北通行要显示的时间为40的计数器初始值
CLR TF0  ;
CLR TF1
MOV TH1,#0FFH ;给初值
MOV TL1 ,#0F8H
MOV TH0 ,#0FFH
MOV TL0 ,#0F8H
LJMP TEMP340

TEMP320:;状态三
SETB TR1      ;南北停止计数
SETB TR0     ;东西开始计数
SETB P1.1    ;东西通行，南北禁止通行
CLR P1.0
SETB P1.1
CLR P1.0
SETB P1.5
CLR P1.6
SETB P2.1
CLR P2.3
MOV TEMP3,#25  ;南北要显示的时间，
MOV TEMP4,#20 ;东西要显示的时间，

STLOP33:
ACALL DISPLAY  ;调用显示
DEC TEMP3       ;时间够一秒显示时间减1
DEC TEMP4
MOV A,TEMP4
CJNE A,#0,NEXT33  ;若显示时间不为0保持现在状态
LJMP STAR34       ;若显示时间为 0跳到第四状态
NEXT33:LJMP STLOP33


STAR34:           ;状态四
SETB P2.3
CLR P2.2
SETB P1.6      ;东西黄灯，南北禁止通行
CLR P1.5




MOV TEMP3,#05   ;南北要显示的时间，
MOV TEMP4,#05   ;东西要显示的时间，
STLOP34:
ACALL DISPLAY  ;调用显示
DEC TEMP3       ;时间够一秒显示时间减1
DEC TEMP4
MOV A,TEMP4
CJNE A,#0,NEXT34  ;若显示时间不为0保持现在状态
JB TF1 ,T402
JB TF0 ,T402
MOV TEMP3,#25
LJMP STAR
T402:
MOV TEMP3,#45
LJMP STAR
NEXT34:   LJMP STLOP34


TEMP340 :
SETB TR1      ;南北停止计数
SETB TR0     ;东西开始计数
SETB P1.1    ;东西通行，南北禁止通行
CLR P1.0
SETB P1.1
CLR P1.0
SETB P1.5
CLR P1.6
SETB P2.1
CLR P2.3
MOV TEMP3,#45  ;南北要显示的时间，
MOV TEMP4,#40 ;东西要显示的时间，

STLOP43:
ACALL DISPLAY  ;调用显示
DEC TEMP3       ;时间够一秒显示时间减1
DEC TEMP4
MOV A,TEMP4
CJNE A,#0,NEXT43  ;若显示时间不为0保持现在状态
LJMP STAR44       ;若显示时间为 0跳到第四状态
NEXT43:LJMP STLOP43


STAR44:           ;状态四
SETB P2.3
CLR P2.2
SETB P1.6      ;东西黄灯，南北禁止通行
CLR P1.5




MOV TEMP3,#05   ;南北要显示的时间，
MOV TEMP4,#05   ;东西要显示的时间，
STLOP44:
ACALL DISPLAY  ;调用显示
DEC TEMP3       ;时间够一秒显示时间减1
DEC TEMP4
MOV A,TEMP3
CJNE A,#0,NEXT44  ;若显示时间不为0保持现在状态
JB TF1 ,T403
JB TF0 ,T403
MOV TEMP3,#25
LJMP STAR
T403:
MOV TEMP3,#45
 LJMP STAR
NEXT44:   LJMP STLOP44


;显示
DISPLAY1:
MOV A,TEMP1  ;将南北要显示的数存放到A
MOV B,#10  ;B=10
DIV AB     ;A除以B商存A，余数B
MOV B_BIT,A ; 将A放到20H
MOV A_BIT,B  ;将B放到21H

MOV A,TEMP2 ;将东西要显示的数存放到A
MOV B,#10   ;B=10
DIV AB       ;A除以B商存A，余数B
MOV C_BIT,A   ;将A放到22H
MOV D_BIT,B   ;将B放到23H

MOV DPTR ,#NUMT ;
MOV R0,#2    ;R0=2
DPL11: MOV R1,#250 ;R1=250
DPLOP1:
MOV A,A_BIT ;将南北要显示的10位数送A
MOVC A,@A+DPTR ;查表
MOV P0,A    ;显示南北10位数
CLR P2.7
ACALL D1MS;延时1MS
SETB P2.7 ;灭南北10位数
MOV A,B_BIT  ;将南北要显示的个位数送A
MOVC A,@A+DPTR   ;查表
MOV P0,A    ;显示南北个位数
CLR P2.6
ACALL D1MS ;延时1MS
SETB P2.6  ;灭南北个位数

MOV A,C_BIT  ;将东西要显示的10位数送A
MOVC A,@A+DPTR  ;查表
MOV P0,A    ;显示东西10位数
CLR P2.5
ACALL D1MS ;延时1MS
SETB P2.5   ;灭东西10位数

MOV A,D_BIT  ;将东西要显示的个位数送A
MOVC A,@A+DPTR ;查表
MOV P0,A    ;显示东西东西位数
CLR P2.4
ACALL D1MS ;延时1MS
SETB P2.4  ;灭东西个位数

DJNZ R1,DPLOP ;循环扫描
DJNZ R0,DPL1
RET     ;等待1秒返回

;显示
DISPLAY:
MOV A,TEMP3  ;将南北要显示的数存放到A
MOV B,#10  ;B=10
DIV AB     ;A除以B商存A，余数B
MOV B_BIT,A ; 将A放到20H
MOV A_BIT,B  ;将B放到21H

MOV A,TEMP4 ;将东西要显示的数存放到A
MOV B,#10   ;B=10
DIV AB       ;A除以B商存A，余数B
MOV C_BIT,A   ;将A放到22H
MOV D_BIT,B   ;将B放到23H

MOV DPTR ,#NUMT ;
MOV R0,#2    ;R0=2
DPL1: MOV R1,#250 ;R1=250
DPLOP:
MOV A,A_BIT ;将南北要显示的10位数送A
MOVC A,@A+DPTR ;查表
MOV P0,A    ;显示南北10位数
CLR P2.7
ACALL D1MS;延时1MS
SETB P2.7 ;灭南北10位数
MOV A,B_BIT  ;将南北要显示的个位数送A
MOVC A,@A+DPTR   ;查表
MOV P0,A    ;显示南北个位数
CLR P2.6
ACALL D1MS ;延时1MS
SETB P2.6  ;灭南北个位数

MOV A,C_BIT  ;将东西要显示的10位数送A
MOVC A,@A+DPTR  ;查表
MOV P0,A    ;显示东西10位数
CLR P2.5
ACALL D1MS ;延时1MS
SETB P2.5   ;灭东西10位数

MOV A,D_BIT  ;将东西要显示的个位数送A
MOVC A,@A+DPTR ;查表
MOV P0,A    ;显示东西东西位数
CLR P2.4
ACALL D1MS ;延时1MS
SETB P2.4  ;灭东西个位数

DJNZ R1,DPLOP ;循环扫描
DJNZ R0,DPL1
RET     ;等待1秒返回



D1MS: MOV R7,#250  ;1MS延时程序
DJNZ R7,$
RET
;1到10对应电路图数码管表
NUMT: DB    7EH,48H,67H,6BH,59H
      DB    3BH,3FH,68H,7FH,7BH
END