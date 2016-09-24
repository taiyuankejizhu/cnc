#include "znc.h"
#include <dos.h>
#include <conio.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#if CNC
extern char RunCodeGroups[1000];
extern char RunCodeLines[1000];
extern char M9899;
extern char RunCodeLine;/*当前代码行*/
extern char G0203;
extern char RunLineFlag;
extern char offsetxyz;  /*0机床,1当前坐标*/
extern char CodeAbs;    /*0绝对,1相对坐标编程*/
extern char RunCodeLine;/*当前代码行*/
extern char RunCodeGroup;/*当前代码组*/
extern char MXYZ;       /*平面选择*/
extern struct Dis_position{
    char XYZ;   /*X:0 Y:1 Z:2*/
    char Position_S;
    long Position_H;
    long Position_E;
    long Position_V;
}DIS;
extern struct Run_Pi{
    int PE;
    int PV;
    int PT;
    char XYZ[1000];
    long XYZV0[1000];
    long XYZV1[1000];
}RunPi;
void DelCodeLine(void);
void CheckCode(void);
void CtoCode(void);
void CodetoC(void);
void C1(void);      /*Code Group*/
