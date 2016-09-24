#include <graphics.h>
#include "alloc.h"
#include "time.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mem.h>

#define DZC 1   /*um*//*50 or 11*/
#define STEPL 5
#define DZCC    1
#define ZK  1
#define TESTING 1
#define NZNC    0
#define CNC 1
#define PNC 0
#define SIONV   0
#define DL32    0   /*0:电流75A;1:电流32A*/
#define MAXDL   75
#if SIONV
#define VER "3.11"
#else
#define VER "3.10"
#endif
#define IO  0x248
#define Address_Z 0x244
#define OSC 0x246
#define ICXaddr 0x240
#define ICYaddr 0x242

#define Inter 0x250

#define UDKN 2
#define IDS 0xd3b33C5A
#define CLK0 0
#define BKCOLORA 0
#define BKCOLORB 0
#define BKCOLORC 8
#define BKCOLORD 0
#define BKCOLORE 7
#define BKCOLORG 7
#define BKCOLORE1 7 /*3D*/
#define BKCOLORE2 0 /*NUL*/

#define COLORA 10
#define COLORA1 11
#define COLORA2 12
#define COLORB 11
#define COLORC 12
#define COLORD 10
#define COLORD1 11
#define COLORE 11
#define COLORG 9
#define COLORE1 0   /*3D*/
#define COLORE2 15  /*NUL*/
#define AX0 0
#define AX1 429
#define AY0 0
#define AY1 186
#define BX0 0
#define BX1 519
#define BY0 187
#define BY1 387
#define CX0 0
#define CX1 519
#define CY0 388
#define CY1 423
#define DX0 430
#define DX1 519
#define DY0 0
#define DY1 186
#define EX0 520
#define EX1 638
#define EY0 0
#define EY1 423
#define GX0 0
#define GX1 638
#define GY0 424
#define GY1 478

#define X01 440
#define X0 0
#define X1 639
#define X2 550
#define X3 280
#define Y0 0
#define Y1 479
#define Y2 52
#define Y3 190
#define F7X 133
#define F7Y 255
#define F8X 133
#define F8Y 255
#define FLAG_POSX0 690
#define FLAG_POSY0 3
#define FLAG_POSX1 795
#define FLAG_POSY1 538
#define ComputerType 2    /* 1 for 486,2 for 586*/
#define DELAY 10


void outb(int portid,unsigned char valud);
unsigned char inb(int portid);
void DeepProcess(void);
void InitKABT(void);



struct ThreeByte
{
    unsigned char cLowByte;
    unsigned char cMidByte;
    unsigned char cHighByte;
    unsigned char cNoUse;
    int cFlag;

};

extern struct Tab
{
    char    Index[10];
    long          Shendu[10]; /*深度*/
    unsigned int  Dianliu[10]; /*电流*/
    unsigned int  Maikuan[10]; /*脉宽*/
    unsigned int  Xiuzhi[10];  /*休止*/
    unsigned char Jianxi[10];  /*间隙*/
    unsigned char Sudu[10];    /*速度*/
    unsigned char Shenggao[10];    /*升高*/
    unsigned char Gongshi[10];    /*工时*/
    unsigned char Mianji[10];    /*面积*/
    unsigned char Jixin[10];    /*极性*/
    unsigned char Gaoya[10];    /*高压*/
    unsigned int  B_OFS[10];   /*底面残量*/
    unsigned int  S_OFS[10];   /*侧面残量*/
    unsigned int  LNS[10];     /*放射分割数*/
    long  STEP[10];    /*摇动步长*/
    unsigned int  LMN[10];     /*摇动设定*/
    unsigned int  RY[10];    /*精糙度*/
}Table;
typedef struct QDNode{      /*抬头节点*/
  char xyz,MXYZ;/*xyz表示插补类型,MXYZ表示圆弧插补时的插补平面直线插补时无效*/
  long x0,y0,z0;/*G01时表示起点,G02,03时,x0,y0表示对应平面内起点，z0表示圆心cI*/
  long x1,y1,z1;/*G01时表示终点,G02,03时,x1,y1表示对应平面内终点，z1表示圆心cJ*/
  struct QDNode *prior,*next;
}QNode;
typedef struct QDueue{
  struct QDNode *front;
  struct QDNode *rear;
  int item;/*队列元素个数*/
  int qsize;/*队列最大元素个数*/
}Queue;
extern QNode *UP_node,*Current_DIS,*DIS_enode;/*UP_node当前抬高点，Current_DIS当前放电结点*/
extern Queue DIS_que;
extern struct Tab StrTable;
    extern char Dis_flag;   /*放电标志。正在放电为1，停止放电为0。*/
    extern char Dis_first;
    extern long position_x; /*当前X轴的位置*/
    extern long offset_x;   /*当前X轴的辅助位置,POSITION=OFFSET+COUNTER*/
    extern long position_y; /*当前Y轴的位置*/
    extern long offset_y;   /*当前Y轴的辅助位置,POSITION=OFFSET+COUNTER*/
    extern long position_z; /*当前Z轴的位置*/
    extern long offset_z;   /*当前Z轴的辅助位置,POSITION=OFFSET+COUNTER*/
    extern long position_d; /*当前的最深深度*/
    extern long position_p; /*当前的目标深度*/
    extern long offset_rx;/*当前X轴寄存器坐标*/
    extern long offset_ry;
    extern long offset_rz;
    extern long position_dx;
    extern long position_dy;
    extern long position_dz;
    extern long position_px;
    extern long position_py;
    extern long position_pz;
    extern char lock_x,lock_y,lock_z;
    extern long position_t;
    extern long position_tt;
extern long offset_xx[],offset_yy[],offset_zz[];

    extern char Dis_lines;   /*当前放电的行*/
    extern char Dis_start;  /*定段的首行*/
    extern char Dis_end;    /*定段的末行*/

    extern char Profile_xflag; /*运行Profile操作时置1，运行完毕后置0*/
    extern char Profile_yflag; /*运行Profile操作时置1，运行完毕后置0*/

    extern char Profile_flag; /*运行Profile操作时置1，运行完毕后置0*/
    extern char Group;   /*当前所处的组别*/
    extern char XYZGroup;
    extern char Elect_State;
    extern int Elect_time;
    extern char Elect_speed;   /*放电时机头上下的速度*/
    extern int soundV;  /*调节音量*/

    extern char AI_Flag;       /*人工智能标志*/
    extern unsigned AI_time;
    extern unsigned AI_high;
    extern struct tm stTime;
    extern char cClearFlag;
    extern char RiseFlag;
    extern char F11_flag;
    extern long steel_time;
    extern char SteelFlag;
    extern char steel_flag;
    extern char Sound_flag;
    extern char Ep_Short;
    extern int V;
    extern int A;/*电流*/
    extern int DIS_OK;
    extern int DIS_OK1;
    extern long Velocity;
    extern unsigned int ProtectTime;
    extern char Error1100_flag;
    extern char KESCF;
extern unsigned char IO0,IO1,IO2,IO3,IO4,IO5,IO7,IO6,OTPS0,OTPS1,IOPS1,IOZ0,IOZ1,OTPZ0,IOPZ1;
extern unsigned char IOX0,IOYO,OTPBX0,OTPBX1,IOPBX0,OTPBY0,OTPBY1,IOPBY0;
extern char OCSF;
extern float MX;
extern unsigned FS;/*风扇*/
extern unsigned SGY,SDL;
void OpenPower(void);      /*打开副电源*/
void ClosePower(void);   /*关闭副电源*/
void CloseAmplifier(void);  /*关闭放大器*/

void Speed1100_x(char speed);
void Speed1100_y(char speed);
void Signal_1100X(void);     /*发放HTCL-1100信号*/
void Signal_JianxiX(char flag);    /*发放间隙电压信号*/
void Signal_1100Y(void);     /*发放HTCL-1100信号*/
void Signal_JianxiY(char flag);    /*发放间隙电压信号*/

void Speed1100_z(char speed);
void Signal_1100(void);     /*发放HTCL-1100信号*/
void Signal_Jianxi(char flag);    /*发放间隙电压信号*/
void Open_OCSF(void);
void Close_OCSF(void);
void Open_Elect(void);
void Close_Elect(void);
void Open_Pump(void);   /*开油泵*/
void Close_Pump(void);  /*关油泵*/
void CloseAllPower(void); /*关闭所有电源*/

extern void Set_disp(void);/*List初始化屏幕*/
extern char Read1100_x(char reg);         /* Read register of HTCL-1100 */
extern void Write1100_x(char reg,char data);  /* Write data to register of HTCL-1100  */
extern void Init1100_x(void);    /* Entry Initialization/Idle mode */
extern void Reset1100_x(void);             /* Reset HTCL-1100 */
extern char Read1100_y(char reg);         /* Read register of HTCL-1100 */
extern void Write1100_y(char reg,char data);  /* Write data to register of HTCL-1100  */
extern void Init1100_y(void);    /* Entry Initialization/Idle mode */
extern void Reset1100_y(void);             /* Reset HTCL-1100 */

char Read1100_z(char reg);         /* Read register of HTCL-1100 */
void Write1100_z(char reg,char data);  /* Write data to register of HTCL-1100  */
void Init1100_z(void);    /* Entry Initialization/Idle mode */
void Reset1100_z(void);             /* Reset HTCL-1100 */
unsigned char Current(void);   /*读取电流值*/
unsigned char Voltage(void);   /*读取电压值*/
/*void Voltage_Start(void);
void Current_Start(void);*/

void Position_control_x(long position); /**(struct ThreeByte position);**/ /* Entry position control mode */
void Velocity_control_x(int v);           /* proportional velocity control mode */
void Position_control_y(long position); /**(struct ThreeByte position);**/ /* Entry position control mode */
void Velocity_control_y(int v);           /* proportional velocity control mode */
void Profile_x(int Acceleration,long position);
void Profile_y(int Acceleration,long position);

void Position_control_z(long position); /**(struct ThreeByte position);**/ /* Entry position control mode */
void Velocity_control_z(int v);           /* proportional velocity control mode */
void Profile_z(int Acceleration,long position);
struct ThreeByte input(char color,char back_color,int x,int y,char integer,char dot,int divalue);
struct ThreeByte LongToThreeByte(long lLongValue);
long int ThreeByteToLong(struct ThreeByte stThreeByte);

long ReadRegisterValue_X(char cLowReg);
long ReadRegisterValue_Y(char cLowReg);

long ReadRegisterValue_Z(char cLowReg);
unsigned char ReadRAM(unsigned char page,unsigned int number);
void WriteRAM(unsigned char page,unsigned int number,unsigned char value);
void Read_Table(unsigned char group); /*把group所指定组别的数据从SRAM传递给全局变量Table*/
void Write_Table(unsigned char group); /*根据group所指定的组别，把全局变量Table的数据传递给SRAM*/
void Init_IO(void);
void Check8255(void);
void Duty_ratio(unsigned int occupy,unsigned int empty,unsigned clk0);
void SetTable(char lines);    /*根据数据表格(全局结构Table)中的数据设置各放电参数*/
void PositionParameter(void);          /*更新全局变量position_z,position_d,position_p*/
long AbsLong(long Value);
char ProfileEnd(void);  /*Profile模式运行完毕返回1，否则返回0*/
int PositionEndXYZ(unsigned char flag);
void Electricity(void);
int KeyPress(void); /*没按键时返回0，有按键反回非0*/
int KeyAss(void);
void DefSeg(void);
void Key(void);
void char16x16(int iFrontColor,int iBackColor,int x,int y,int iChar);
void char16x24(int iFrontColor,int iBackColor,int x,int y,char cChar);
void char26x32(int iFrontColor,int iBackColor,int x,int y,int iChar);
void DisplayNumber(char cFontColor,char cBlackColor,int x,int y,char cDot,
long lNumberIn,char cChar);
void Display(char cFontColor,char cFontBack_color,int x,int y,
char cDot,long lValue);
void DispLong_Small(char cFontColor,char cFontBack_color,int x,int y,
char cDot,long lValue);
void DispLong(char cFontColor,char cFontBack_color,int x,int y,
char cDot,long lValue);
void DrawLevin(int iPosX,int iPosY,int iLongX,int iLongY,int iColor,int iBackColor);
void DrawColu(int iDepth,int iFlag);
void DispLogo(void);
void ConvexButton(int x1,int y1,int x2,int y2,int col);
void ConcaveButton(int x1,int y1,int x2,int y2,int col);
void GraphicButton(int iPosX1,int iPosY1,int iPosX2,int iPosY2);
void InvertGraphic(int iPosX1,int iPosY1,int iPosX2,int iPosY2);
void DispFace(void);
void AllButton(char cChar,int iFlag);
void Zero(void); /*归零功能函数*/
void High(void);  /*高度功能函数*/
void AlterMessage(void);/****信息****/
void OpenSound(void);
void CloseSound(void);
void ShortSound(void);
void EntrySleep(void);
void RiseAxes(void);/*上升功能*/
void Steel(void);/*刚蚀刚功能*/
char OilSwitch(void);

void Stop_x(char flag);
void Stop_y(char flag);

void Stop_z(char flag);
long XPosi(char flag);/*读取X轴计数器的数据*/
long YPosi(char flag);/*读取Y轴计数器的数据*/
long ZPosi(char flag);/*读取Y轴计数器的数据*/
void XYPosition(void);/*设置X、Y轴的位置*/
void ResetXYZ(void);
struct Tab ProcessTable(long lDeep,unsigned char cCurrent,char cAcreage,char cEffect);
void ScreenProtect(void);
char Test(void);
void SetPassword(void);
void Password(void);
void DisplayNumber2(int cFontColor,int cBlackColor,int x,int y,char cDot,long lNumberIn,char cChar);
void StopError();
void RiseError();
void F13(void);










#define SoundTIME 3
#define SDST (18*30)
#define TBST 2
extern void KeyNumber(int k);

extern unsigned char IB[];
extern char Fx;     /*1:按下F?,0数字母*/
extern char * DispKey(int zu,int fkey);
extern unsigned char ReadRAM(unsigned char page,unsigned int number);
extern char CopyRight[];
extern char F[10];
extern char K[10];
extern char P[15];

extern char C[10];
extern char CTabWidth[10];
extern char CTabWidth1[10],CTabWidth2[10];
extern char G04;

extern long K_3;
extern char TabWidth[12];
extern char TabWidth1[12],TabWidth2[12];
extern int GaoyaV[5];
extern unsigned char PWS[];
extern char Inputstr[8],Inputstr1[8];
extern char XYZ[];
extern char *Argv;
extern int Argc;
extern long Zpointer;
extern char *JixinS[2];
extern char Group;/*当前组*/
extern char groupindex[101];
extern char MaxL;
extern long KeyN,MaxN,MinN;
extern char F11_flag,F12_flag;
extern char XX,YY,Y7Y;
extern char SWF_flag;
extern char SWF_K;

extern char XUP_K;
extern char XDN_K;
extern char XDNUP;
extern long XUDL;
extern char YUP_K;
extern char YDN_K;
extern char YDNUP;
extern long YUDL;
extern char MOVEXYZ;

extern char UP_K;
extern char DN_K;
extern char DNUP;
extern long UDL;
extern char Pump_flag;
extern long MianJi;
extern char Sound_flag;
extern char V0ING;
extern char Steel_flag;
extern int steel,JX;
extern int F8H;
extern char F8T;
extern char Scr_flag;
extern int A;
extern int V;
extern int KEYL;
extern int SMessage;
extern int Testflag;
extern int Initsystem;
extern char oldpage;
extern unsigned char buf[];
extern int ExitTime;
extern long ox,oy,oz;
extern int SoundTime;
extern char Ck8255_flag;
extern char ZeroFlag,ZeroFlagX,ZeroFlagY;
extern char FireFlag;
extern char PassWordF;
extern long Oldtime;
extern long Work_time,Up_time,V_time,Velocity,Velocity_Time;
extern long offset_x,offset_y,offset_z,offset_t,position_p,position_d;
extern long position_xdis,position_ydis,offset_d;
extern char offset_ds;
extern long lDeep;
extern char cAcreage,cEffect;
extern char ERR_XY;
extern char Position_flag;
extern int cCurrent;
extern unsigned char far *_BMP_();
extern unsigned char far *BMP;
extern struct Tab Table;
extern unsigned OldID,OldDATE;
extern unsigned AI,AITime,AIHigh;

extern struct CncCode{
    unsigned  N;
    unsigned char Code;
    long X;
    long Y;
    long Z;
    long R;
    long I;
    long J;
    long K;
    unsigned char G;
    unsigned char D;
    int line;
}cnccode;
extern struct CncCode cncode;
extern struct CncChar{
    char N[3];
    char Code[4];
    char X[10];
    char Y[10];
    char Z[10];
    char R[10];
    char G[3];
    char D0;
    char D1;
}cncchar;
#define CodeL 40
extern char CodeC[CodeL][4];
extern unsigned char CodeN[CodeL];
extern char *path;
extern char *tempfilename1;
extern char *tempfilename2;
extern char filename[0x10];
extern unsigned char CodeGroup,Nline,Sline;
extern unsigned char CodeGroupIndex[100];
extern unsigned char far *Code;
extern char CXX,CYY;
extern struct CncCode Ncnccode;

extern void InitCNC(void);
extern void ShowCode(int lines);
extern void CreatTempFile(void);
extern void CheckTempFile(void);
extern void Read_Code(unsigned char Group);
extern void Write_Code(unsigned char Group);
extern void Read_Code1(unsigned char Group);
extern void Write_Code1(unsigned char Group);

extern void windows(int left,int top,int right,int bottom,int newattr);
extern void ShowTable(int lines);
extern void ShowKey(int kn,int ke);
extern void Key(void);
extern void ShowSWF(void);

extern void XDown(void);
extern void XUp(void);
extern void YDown(void);
extern void YUp(void);
extern InitScr256(int flag);

extern void Down(void);
extern void Up(void);
extern void ShowXYZ();
extern char SetTime(long Time);
extern unsigned GetDate();
extern void ShowFlag(char flags);
extern void ShowHT(void);
extern void ShowMess(int value);
extern void ShowInput(char value);
extern long GetInput(void);
extern void InitKey(char flag);
extern void exitp(char flag);
extern char DispF7(int flag);
extern void ShowF7(int lines);
extern int CheckBMP(void);
extern void JiXin(int);
extern void ClearKey(int key);
extern void F8(char flag);
extern void F7(char flag);
extern void OpenScr(void);
extern void CloseScr(char);
extern int ProTime(void);
extern int Cmp(unsigned,unsigned);
extern unsigned NTN0(unsigned);
extern unsigned N0TN(unsigned);
extern void ResetCop(void);
extern int SetWork(unsigned,unsigned);
extern int SetWork1(unsigned,unsigned);
extern void SetDianliu(unsigned dianliu);
extern void SaveXYZ(char flag);
extern long GetSTime(void);
extern void ShowTime(char flag);
extern void ShowTimeD(int flag);
extern void ShowVer(char *ver);
extern void SaveBMP(void);
extern void Delay1(unsigned mill);
extern unsigned char far *HZK16F_A1(char value);
extern unsigned char far *HZK16F_A2(char value);
extern unsigned char far *HZK16F_A3(char value);
extern unsigned char far *HZK16F_A4(char value);
extern unsigned char far *HZK16F_A5(char value);
extern unsigned char far *HZK16F_A6(char value);
extern unsigned char far *HZK16F_A7(char value);
extern unsigned char far *HZK16F_A8(char value);
extern unsigned char far *HZK16F_A9(char value);
extern unsigned char far *HZK16F_AA(char value);
extern unsigned char far *HZK16F_AB(char value);
extern unsigned char far *HZK16F_AC(char value);
extern unsigned char far *HZK16F_AD(char value);
extern unsigned char far *HZK16F_AE(char value);
extern unsigned char far *HZK16F_AF(char value);
extern unsigned char far *HZK16F_B0(char value);
extern unsigned char far *HZK16F_B1(char value);
extern unsigned char far *HZK16F_B2(char value);
extern unsigned char far *HZK16F_B3(char value);
extern unsigned char far *HZK16F_B4(char value);
extern unsigned char far *HZK16F_B5(char value);
extern unsigned char far *HZK16F_B6(char value);
extern unsigned char far *HZK16F_B7(char value);
extern unsigned char far *HZK16F_B8(char value);
extern unsigned char far *HZK16F_B9(char value);
extern unsigned char far *HZK16F_BA(char value);
extern unsigned char far *HZK16F_BB(char value);
extern unsigned char far *HZK16F_BC(char value);
extern unsigned char far *HZK16F_BD(char value);
extern unsigned char far *HZK16F_BE(char value);
extern unsigned char far *HZK16F_BF(char value);
extern unsigned char far *HZK16F_C0(char value);
extern unsigned char far *HZK16F_C1(char value);
extern unsigned char far *HZK16F_C2(char value);
extern unsigned char far *HZK16F_C3(char value);
extern unsigned char far *HZK16F_C4(char value);
extern unsigned char far *HZK16F_C5(char value);
extern unsigned char far *HZK16F_C6(char value);
extern unsigned char far *HZK16F_C7(char value);
extern unsigned char far *HZK16F_C8(char value);
extern unsigned char far *HZK16F_C9(char value);
extern unsigned char far *HZK16F_CA(char value);
extern unsigned char far *HZK16F_CB(char value);
extern unsigned char far *HZK16F_CC(char value);
extern unsigned char far *HZK16F_CD(char value);
extern unsigned char far *HZK16F_CE(char value);
extern unsigned char far *HZK16F_CF(char value);
extern unsigned char far *HZK16F_D0(char value);
extern unsigned char far *HZK16F_D1(char value);
extern unsigned char far *HZK16F_D2(char value);
extern unsigned char far *HZK16F_D3(char value);
extern unsigned char far *HZK16F_D4(char value);
extern unsigned char far *HZK16F_D5(char value);
extern unsigned char far *HZK16F_D6(char value);
extern unsigned char far *HZK16F_D7(char value);
extern unsigned char far *HZK16F_D8(char value);
extern unsigned char far *HZK16F_D9(char value);
extern unsigned char far *HZK16F_DA(char value);
extern unsigned char far *HZK16F_DB(char value);
extern unsigned char far *HZK16F_DC(char value);
extern unsigned char far *HZK16F_DD(char value);
extern unsigned char far *HZK16F_DE(char value);
extern unsigned char far *HZK16F_DF(char value);
extern unsigned char far *HZK16F_E0(char value);
extern unsigned char far *HZK16F_E1(char value);
extern unsigned char far *HZK16F_E2(char value);
extern unsigned char far *HZK16F_E3(char value);
extern unsigned char far *HZK16F_E4(char value);
extern unsigned char far *HZK16F_E5(char value);
extern unsigned char far *HZK16F_E6(char value);
extern unsigned char far *HZK16F_E7(char value);
extern unsigned char far *HZK16F_E8(char value);
extern unsigned char far *HZK16F_E9(char value);
extern unsigned char far *HZK16F_EA(char value);
extern unsigned char far *HZK16F_EB(char value);
extern unsigned char far *HZK16F_EC(char value);
extern unsigned char far *HZK16F_ED(char value);
extern unsigned char far *HZK16F_EE(char value);
extern unsigned char far *HZK16F_EF(char value);
extern unsigned char far *HZK16F_F0(char value);
extern unsigned char far *HZK16F_F1(char value);
extern unsigned char far *HZK16F_F2(char value);
extern unsigned char far *HZK16F_F3(char value);
extern unsigned char far *HZK16F_F4(char value);
extern unsigned char far *HZK16F_F5(char value);
extern unsigned char far *HZK16F_F6(char value);
extern unsigned char far *HZK16F_F7(char value);
#define aF1 0x3B
#define aF2 0x3C
#define aF3 0x3D
#define aF4 0x3E
#define aF5 0x3F
#define aF6 0x40
#define aF7 0x41
#define aF8 0x42
#define aF9 0x43
#define aF10 0x44
#define aF11 0x85
#define aF12 0x86
#define LEFT  0x4b
#define RIGHT 0x4d
#define KUP 0x48
#define KDN 0x50
#define kSP 0x20

#ifndef __ZNC_H__
#define __ZNC_H__
#define kF1 0x3B00
#define kF2 0x3C00
#define kF3 0x3D00
#define kF4 0x3E00
#define kF5 0x3F00
#define kF6 0x4000
#define kF7 0x4100
#define kF8 0x4200
#define kF9 0x4300
#define kF10 0x4400
#define kF11 0x8500
#define kF12 0x8600
#define kLF 0x4b00
#define kRT 0x4d00
#define kUP 0x4800
#define kDN 0x5000
#define kINS    0x5200
#define kDEL    0x5300
#define kHOME   0x4700
#define kEND    0x4F00
#define kPGUP   0x4900
#define kPGDN   0x5100
#define kESC    0x1B
#define kENT    0x0D
#define kBS 0x08
#define BKCR0   0xaf
#define BKCR12  0x77
#define CR12    0x2a

#define kMUL    0x2a
#define kA0 0x5230
#define kA1 0x4F31
#define kA2 0x5032
#define kA3 0x5133
#define kA4 0x4B34
#define kA5 0x4C35
#define kA6 0x4D36
#define kA7 0x4737
#define kA8 0x4838
#define kA9 0x4939


union long_char{
  unsigned long l;
  unsigned i[2];
  unsigned char c[4];
};
union int_char{
  unsigned i;
  unsigned char c[2];
};
struct _XYZ_{
  long x,y,z;
};extern struct _XYZ_ oxyz,offset,src,dest,position;

struct _Color_ {
  unsigned char ht0,ht1,xyz,zb,g92,mess,crt,vol;
  unsigned char sta0,sta1,sta2,edit,eblock,erun,tab0,tab1;
  unsigned char tab2,tab3,mstr;
  unsigned char hm0,hm1;
  unsigned char hf0,hf1;
};
extern struct _Color_ Color,BkColor;
extern char CdsSta,OilSta;

extern char DispStr[0x100];
extern char HM[10];
extern char HF[10];
extern char HF1[20][11];
extern char HF2[20][11];

extern char Pump_flag;
/*List.c*/
void Listbar3d(int,int,int,int,long color,unsigned char,unsigned,unsigned char,char *);
void Listbark3d(int x,int y,int x1,int y1,unsigned colors,char flag,char *str);
void Listbar(int,int,int,int,unsigned,char,char *);
void Listbar3(int x,int y,int x1,int y1,char flag);
char far *HFStr(int x,int y);
int gprintf( int xloc, int yloc, int color, char *fmt, ... );
char far * gstr(char *fmt,...);
void ListF12(void);
ListHT(char flag,char *str);
ListHF0(int x,int y);
ListHF(int x,int y);
ListHM(char flag);
ListHM1(char flag);
void ListXYZ00(char flag);
void ListXYZ1(char,long);
void ListZB(char,char,long);
void ListZB1(char,long);
void ListZB00(char);
void ListXYZ2(char,long);
void ListG92(void);
void ListMess1(char *);
void ListCRT1(char *);
void ListMess(char);
void ListCRT(char);
void ListStatus(unsigned char);
void ListVol0(void);
void ListVol(unsigned char);
/*Edit.c*/
#define EDL  84
struct _EDIT_ {
  unsigned length;
  int topline;
  int ty,tp;
  int x,y,pointer;
  unsigned bx0,bx1,by0,by1,bl0,bl1;
};extern struct _EDIT_ edit;
extern char far * EDITA;
extern char far * PROCONA;
extern char insert_sta;
extern char block_sta;
extern char bselect_sta;
struct _PROCON_ {
    int fileline,topline;
    int y;
    char sx,sy,sl0,sl1,sl;/*sl0:材料组合,sl1：切换*/
    char name;
    char xyz;
    signed long l;
    char str[12],str1[11],str2[11],str3[11],str4[11],str5[11],str6[11];
};extern struct _PROCON_ procon;
extern char proconfn;
#define __PCTABLEN__ 17
void InitEdit(void);
void InitBlock(void);
FILE *OpenF(char *,char);
void OpenFIle(char);
void InsertFile(char);
void ListEdit(char lines,char flag);
void EditBlock(char);
void BlockInsert(int);
void BlockDel(char,int);
void BlockConvert(char);
void BlockCut(void);
void BlockCopy(void);
void BlockPasce(void);
void FileBackup(void);
void FileRestore(void);
void FileErase(void);
void InitProCon(void);
void InsertProCon(void);
void EraseProCon(void);
void ListProCona(int x,int y);
void ListProConab(int,int,int);
void ListProConb(int,int);
void ListProConc(int y,char n);
void ListProCon(int x,int y);
void LoadProCon(void);
void SaveProCon(void);


/*KeyBoard.c*/

extern char Ks8;
extern char Ks6;
extern char Ks5;
extern char Ks4;
extern char Ks3;
extern char SlZB;
extern char *SlZBS[];
extern char G92;
extern int G92S[60];
void KeyBoard1(char);
void KeyBoard(void);
int KeyBoardPress(void);
char LNNM(void);
void InputChar(int);
void InputCharE();
void Key0(int k);
void Key1(int k);
void Key2(int k);
void Key11(int k);
void Key12(int k);
void Key13(int k);
void Key14(int k);
void Key30(int k);
void Key3(int k);
void Key4(int k);
void Key40(int k);
void Key5(int k);
void Key9(int k);
void Key6(int k);
void Key60(int k);
void Key63(int k);
void Key64(int k);
void Key67(int k);
void Keya(int k);
void Keyd(int k);
void Keyf(int k);
void Key7(int k);
void Key8(int k);
void Key8a(int k);
void Key8b(int k);
void Key8c(int k);
void Key8d(int k);
void Key81(int k);
void Key82(int k);
void Key83(int k);
void Key830(int k);
void Key831(int k);
void Key832(int k);
void Key833(int k);
void Key834(int k);
void Key84(int k);
void Key85(int k);
void FFileLN(char *,char *);

/*ListLN.c*/
#define __LN00K__ 18
#define __LN01K__ 14
#define __LN030K__ 10
#define __LN031K__ 15
#define __LN032K__ 14
#define __LN033K__ 7
#define __LN04K__ 6
#define __LN05K__ 100
#define __LNSTR0__ 4
#define __LNSTR1__ 6    /*轴+-选择*/
#define __LNSTR2__ 3    /*材料组合*/
#define __LNSTR3__ 4    /*摇动轨迹*/
#define __LNSTR4__ 4    /*摇动选择*/
#define __LNSTR5__ 3    /*加工工程*/
extern char far * NM_Str;
extern char far * LN02A;
extern char far * COMPL01;
extern char far * LN034A;
extern char far * LN04A;
extern char far * LN05A;
extern char FFN[100];
extern char BFFN[100];
extern char PFFN[100];
extern char LN_NM[3];
extern FILE *FP,*fp1;
extern char NCStr[9];
extern char BNCStr[9];
extern char PNCStr[9];
extern char TempFN[12];
extern char *strspace;
extern char LN00K;
extern char LN01K;
extern char LN02K;
extern char LN030K,LN031K,LN032K,LN033K;
extern char LN04K;
extern char LN05K;
extern char ErrorS;
extern char LN01S[__LN01K__];
extern char LN030S[__LN030K__],LN031S[__LN031K__],LN032S[__LN032K__],LN033S[__LN033K__],LN04S[__LN04K__];
struct _LNStr_ {char sl0,sl1,sl2,sl3,sl4,sl5;};
extern struct _LNStr_ LNStr;
struct _LN01_ {
    char sw0,sw1,sl0,sl1,sl2,sl3,sl4;
    char st0,st1,st2,st3,st4,st5,st6;
    char str0[11],str1[11],str2[11],str3[11],
    str4[11],str5[11],str6[11];
};extern struct _LN01_ LN01;
struct _LN02_ {
    int fileline,topline;
    int y;
    char sx,sy;
};extern struct _LN02_ LN02;
struct _LN030_  {
    char sw0,sw1,sw2,sw3,sl0;
    char st0,st1,st2,st3,st4;
    char str0[11],str1[11],str2[11],str3[11],str4[11];
};extern struct _LN030_ LN030;
struct _LN031_ {
    char sw0,sw1,sw2,sl0;
    char st0,st1,st2,st3,st4,st5,st6,st7,st8,st9,st10;
    char str0[11],str1[11],str2[11],str3[11],str4[11],
    str5[11],str6[11],str7[11],str8[11],str9[11],str10[11];
};extern struct _LN031_ LN031;
struct _LN032_ {
    char sw0,sw1,sw2,sl0;
    char st0,st1,st2,st3,st4,st5,st6,st7,st8,st9;
    char str0[11],str1[11],str2[11],str3[11],str4[11],
    str5[11],str6[11],str7[11],str8[11],str9[11];
};extern struct _LN032_ LN032;
struct _LN033_ {
    char sw0,sw1,sw2,sl0;
    char st0,st1,st2;
    char str0[11],str1[11],str2[11];
};extern struct _LN033_ LN033;
struct _LN034_ {
    int y;
    char sx,sy;
    int ty;
    char st0,st1;
    char str0[11],str1[11];
};extern struct _LN034_ LN034;
struct _LN04_ {
    char st0,st1,st2,st3,st4,st5;
    char str0[11],str1[11],str2[11],str3[11],str4[11],str5[11];
};extern struct _LN04_ LN04;
struct _LN041_{
    int n,tn;
    long x,y,z,x0,y0,x1,y1;
    long xy;
};extern struct _LN041_ LN041;
struct _TABFILENAME_{
  char path[5],name0[12],name1[12],name2[12],name3[12],name4[12],name5[12];
};extern struct _TABFILENAME_ tabfilename;
struct _EXTNAME_{
  char nc[5],tab[5];
};extern struct _EXTNAME_ extname;
#define __TABLEN__ 19
struct _TABLEN_{
  char tablen;
  char no,pl,ip,on,off,sv,s,up,dn,v,lc,pp;
  char b_ofs,s_ofs,lns,step,lnm,ry,timer;
};extern struct _TABLEN_ tablen,stablen,tabtype,pctablen;
struct _TAB_{/*len 79*/
  char no[5],pl[2],ip[6],on[4],off[4],sv[4],s[3],up[3],dn[3],v[3],lc[3],pp[3];
  char b_ofs[6],s_ofs[6],lns[5],step[6],lnm[4],ry[6],timer[3];
};extern struct _TAB_ *tab[100],*tabprocon[100];
extern struct _TAB_ tabpc;
extern char *tabname[];
extern char *tabname1[];
extern char *tabnamepc[];

#define _K8_ 5
char *StrLN0(char);
char *StrLN1(char); /*轴+-选择*/
char *StrLN2(char); /*材料组合*/
char *StrLN3(char); /*摇动轨迹*/
char *StrLN4(char); /*摇动选择*/
char *StrLN5(char); /*加工工程*/

void InitLN(void);
void ListLN0(void);
void ListLN1(char *);
void ListLN2(char *);
void ListLN3(void);
void ListLN4(void);
void ListLN5(void);
void ListLN6(void);
void ListLN7(void);
void ListLN8(void);
void ListLN9(void);
void ListLNErr(char,int);
void ListLN00(char);
void ListLN01(char);
void ListLN02a(int,int);
void ListLN02b(int,int);
void ListLN02(int,int);
void ListLN030(char);
void ListLN031(char);
void ListLN032(char);
void ListLN033(char);
void ListLN034(char,int);
void ListLN04(char);
void ListLN041(int);
void ListLN05(int);
void MakeLN20(void);
void MakeLN21(void);
int MakeLN22(void);
int MakeLN23(void);
void MakeLN21P(void);
int MakeLN22P(void);/*PC加工条件*/
int MakeLN23P(void);
void MakeLN24(void);/*保存*/
void MakeLN25(void);/*删除*/
void MakeLn26(void);/*插入*/
int MakeLN0(void);
void MakeLN1(void);
void MakeLN2(void);
void MakeLN3(void);
void MakeLN4(void);
void MakeLN5(void);
void MakeLN6(void);
void MakeLN61(void);
void MakeLN62(char *);
void ftoa(long double,char *);
void xltoa(long,char *);
#define PI 3.14159265358979323846
void SaveBMP256(void);
#endif   /* __ZNC_H__ */
/*Main.c*/
extern void set_PIDZ(unsigned Pid_kp,unsigned Pid_ki,unsigned Pid_kd,unsigned Pid_st);
extern void set_PIDX(unsigned Pid_kp,unsigned Pid_ki,unsigned Pid_kd,unsigned Pid_st);
extern void set_PIDY(unsigned Pid_kp,unsigned Pid_ki,unsigned Pid_kd,unsigned Pid_st);
extern void Opens_V(unsigned v);
/*Disp.c*/
long Read_Velocity_x(char flag);
long Read_Velocity_y(char flag);
long Read_Velocity_z(char flag);
long Read_Velocity_x(char flag);
long Read_Velocity_y(char flag);
long Read_Velocity_z(char flag);
extern char save;
extern char Runline_stx;
extern char Runline_sty;
extern char Runline_stz;
extern double CNC_atof(const char *str);
extern long Gposition_x,Gposition_y,Gposition_z;
extern long Gposition_i,Gposition_j,Gposition_k;
extern char Queue_int(Queue *que);
extern char Queue_empty(Queue *que);
extern char Queue_isfull(Queue *que);
extern int Queue_count(Queue *que);
extern char Queue_en(Queue *que,QNode *node);
extern char Queue_del(Queue *que);
QNode *Qnode_int(char xyz,char MXYZ,long x0,long y0,long z0,long x1,long y1,long z1,QNode *node);
extern char Queue_del(Queue *que);

extern char RunLineFlag;
extern char matile_arr[6];
extern QNode *Process_node;
void TiggerXY(void);
extern void Discharge(long x1,long y1,long z1);
extern void Write_target(long x,long y,long z);
extern void Discharge1(long x1,long y1);
extern void Write_targetarc(long x,long y,long ix,long jy);
