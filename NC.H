#include "znc.h"
char * DispKey(int zu,int fkey);
unsigned char ReadRAM(unsigned char page,unsigned int number);
char CopyRight[]="\xF2\xC4\xCB\xC2\xEF\xCC\xC4\xCB\xFF\xCA\xCB\xC2";
struct Tab Table;
Queue DIS_que;
QNode *UP_node,*Current_DIS,*DIS_enode;
struct Tab StrTable={/*[4]校模参数*/
{0,1,1,0,1,0,0,0,0,0},          /*索引*/
{-9999999/DZC,9999999/DZC,-1,0,-9999999/DZC,0,0,0,0,0}, /*深度*/
#if DL32
{0,32*2,6,0,4,0,0,0,0,0},       /*电流*/
#else
{0,75*2,6,0,4,0,0,0,0,0},       /*电流*/
#endif
{1,999,200,0,20,0,0,0,0,0},     /*脉宽*/
{3,64000,100,0,30,0,0,0,0,0},       /*休止*/
#if NZNC
{20,80,55,0,50,0,0,0,0,0},      /*间隙*/
#else
{30,120,55,0,50,0,0,0,0,0},     /*间隙*/
#endif
{1,8,3,0,5,0,0,0,0,0},          /*速度*/
{0,99,6,0,0,0,0,0,0,0},         /*升高*/
{1,99,5,0,99,0,0,0,0,0},        /*工时*/
{1,99,1,0,1,0,0,0,0,0},         /*面积*/
{0,1,0,0,0,0,0,0,0,0},          /*极性*/
{0,3,1,0,2,0,0,0,0,0},          /*高压*/
{-9999/DZC,9999/DZC,-1,0,-9999/DZC,0,0,0,0,0},         /*底面残量*/
{-9999/DZC,9999/DZC,-1,0,-9999/DZC,0,0,0,0,0},         /*侧面残量*/
{0,9999,0,0,20,0,0,0,0,0},     /*放射分割数*/
{0,9999/DZC,0,0,20,0,0,0,0,0},     /*摇动步长*/
{0,9999,0,0,20,0,0,0,0,0},      /*摇动设定*/
{0,999,0,0,20,0,0,0,0,0},     /*精糙度*/
};
QNode *Process_node;
char F[10]={1,0,0,1,0,0,0,0,0,4};
char K[10]={0,0,2,0,0,0,0,0,0,0};
char P[15]={0,0,0,0,1,0,2,0,0,0,0,0,0};

char C[10]={0,0,0,0,0,0,0,0,0,0};
char CTabWidth[10]={40,40,78,78,78,78,40,40,40};
char CTabWidth1[10]={5,1,34,34,34,34,1,1,5};
char CTabWidth2[10]={10,6,2,2,2,2,10,12,12};
char G04=0;

char TabWidth[12]={27,88,40,40,40,40,40,40,40,40,40,40};
char TabWidth1[12]={1,7,4,5,5,5,5,5,5,5,5,5};
char TabWidth2[12]={8,1,4,4,4,8,8,8,8,8,12,4};
int GaoyaV[5]={100,160,200,260};
unsigned char PWS[17]={1,5,9,13,2,6,10,14,3,7,11,15,4,8,12,0};
#if NZNC
#else
unsigned char IO0,IO1,IO2,IO3,IO4,IO5,IO6,OTPS0,OTPS1,IOPS0,IOPZ0,OTPZ0,IOZ0,IOZ1,OTPZ1;/*OSC0 输出标志位，OSC1*/
unsigned char IOX0,IOYO,OTPBX0,OTPBX1,IOPBX0,OTPBY0,OTPBY1,IOPBY0;
char OCSF=0;
float MX=0;
unsigned FS=0;/*风扇*/
unsigned SGY=0,SDL=0;
#endif
char XYZ[]={0,0,0};
char F7auto[6];
char Pump_flag=0;
unsigned AI_time=0;
unsigned AI_high=50;
long MianJi=0;
char Sound_flag=0;
char V0ING=0;
char Steel_flag=0;
char G_Y=2;
char M_J=1;
char J_X=0;
long K_3=10;
char XX,YY,Y7Y;
char *Argv;
int Argc;
long Zpointer=0;
char Fx=0;      /*1:按下F?,0数字母*/
char *JixinS[2]={"+","-"};
char Inputstr[8]=" ",Inputstr1[8]=".";
char Group=0;/*当前组*/
char XYZGroup;
char groupindex[101];
char Dis_flag=0,Dis_first=0,Dis_lines=0,Dis_start=0,Dis_end=0;
char MaxL=-1;
long KeyN=0x7fffffff,MinN,MaxN;
char F11_flag=1,F12_flag=0;
char Position_flag=0;
char Profile=0;
unsigned ProtectTime=0;
long Work_time,Up_time,V_time; /*升高,工时*/
long Velocity=0,Velocity_Time=0;

char Profile_xflag=0; /*运行Profile操作时置1，运行完毕后置0*/
char Profile_yflag=0; /*运行Profile操作时置1，运行完毕后置0*/

char Profile_flag=0; /*运行Profile操作时置1，运行完毕后置0*/
#if DZCC
char Elect_speed=20;
#else
char Elect_speed=50;
#endif
unsigned char buf[0x200];
char SWF_flag=1;
char SWF_K=0;

char XUP_K=0;
char XDN_K=0;
char XDNUP=0;
long XUDL;
char YUP_K=0;
char YDN_K=0;
char YDNUP=0;
long YUDL;
char MOVEXYZ=0;

char save=0;
char Runline_stx=0;
char Runline_sty=0;
char Runline_stz=0;
char UP_K=0;
char DN_K=0;
char DNUP=0;
long UDL;
int F8H=0;
char F8T=0;
int steel,JX;
char Scr_flag=0;
char KESCF=0;
int A;
int V;
int KEYL=0;
int soundV=5;
int SMessage=0;
int Initsystem=0;
int DIS_OK;
int Testflag;
char oldpage=-1;
char Ck8255_flag=0;
char ZeroFlag=0,ZeroFlagX=0,ZeroFlagY=0;
char FireFlag=0;
char PassWordF=0x2d;
int SoundTime=0;
int ExitTime=0;
long Oldtime=0;
long ox,oy,oz;
/*long position_Sx=0x8fffffff,position_Sy=0x8fffffff,position_Sz=0x8fffffff,position_St=0;*/
long position_x = -9999999/DZC ,position_y = -9999999/DZC,position_z =0x8fffffff/DZC,position_t =0,position_tt;
long Gposition_x=0x7ffffff,Gposition_y=0x7ffffff,Gposition_z=0x7ffffff;
long Gposition_i=0x7ffffff,Gposition_j=0x7ffffff,Gposition_k=0x7ffffff;
long offset_x=0,offset_y=0,offset_z=0,offset_t=0,position_p=-9999999/DZC,position_d=9999999/DZC;
long position_dx,position_dy,position_dz,position_px,position_py,position_pz;
long offset_rx=0,offset_ry=0,offset_rz=0;
char lock_x=1,lock_y=1,lock_z=1;
long offset_xx[10],offset_yy[10],offset_zz[10];
long position_xdis,position_ydis,offset_d=1999999;
char offset_ds=0;
long lDeep;
char cAcreage,cEffect;
char ERR_XY=0;
int cCurrent;
int DIS_OK1;
unsigned OldID,OldDATE;
unsigned AI=0,AITime=0,AIHigh=1;

struct CncCode cnccode;
struct CncCode cncode;
struct CncChar cncchar;
char CodeC[CodeL][4]={"G00","G01","G02","G03","G04","G17","G18","G19","G40","G41","G42","G53","G54","G55","G56","G57","G58","G59","G63","G64","G65","G66","G67","G68","G69","G81","G85","G90","G91","G92",
"M00","M01","M02","M03","M07","M08","M98","M99","M30","   "};
unsigned char CodeN[CodeL]=
{00,01,02,03,04,17,18,19,40,41,42,53,54,55,56,57,58,59,63,64,65,66,67,68,69,81,85,90,91,92,100,101,102,103,107,108,198,199,130,0xff};
char *path="CODE";
char *tempfilename1="CNC.$1$";
char *tempfilename2="CNC.$2$";
char filename[0x10];
unsigned char CodeGroup,Nline,Sline;
unsigned char CodeGroupIndex[100];
unsigned char far *Code;
char CXX=1,CYY=0;
struct CncCode Ncnccode={0x80,-1,0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff,0x7f,0xff};
char RunLineFlag=0;
char matile_arr[6] = {-1,-1,-1,-1,-1,-1};
void InitCNC(void);
void ShowCode(int lines);
void CreatTempFile(void);
void CheckTempFile(void);
void Read_Code(unsigned char Group);
void Write_Code(unsigned char Group);

long ReadRegisterValue_X(char cLowReg);
long ReadRegisterValue_Y(char cLowReg);

long ReadRegisterValue_Z(char cLowReg);
void Open_Elect(void);
void Close_Elect(void);
void Open_OCSF(void);
void Close_OCSF(void);
void OpenScr(void);
void CloseScr(char);
int ProTime(void);
void Profile_z(int Acceleration,long position);
void ShowTable(int liness);
void ShowXYZ();
void ShowFlag(char flags);
void ShowHT(void);
void ShowMess(int value);
void ShowInput(char value);
void ShowKey(int kn,int ke);
long GetInput(void);
void InitKey(char flag);
void exitp(char flag);
char DispF7(int flag);
void ShowF7(int lines);
int CheckBMP(void);
unsigned GetDate();
void JiXin(int);/*极性,0=-,1=+*/
void ClearKey(int key);
void F8(char flag);
void F7(char flag);
void SetDianliu(unsigned dianliu);
int Cmp(unsigned,unsigned);
unsigned NTN0(unsigned);
unsigned N0TN(unsigned);
void ResetCop(void);
int SetWork(unsigned,unsigned);
int SetWork1(unsigned,unsigned);
void SaveXYZ(char flag);
long GetSTime(void);
void ShowTime(char flag);
void ShowTimeD(int flag);
void ShowVer(char *ver);

void XUp(void);
void XDown(void);
void YUp(void);
void YDown(void);

void Up(void);
void Down(void);
void Delay1(unsigned mill);
void SaveBMP(void);
void F13(void);

#ifndef __NC_H__
#define __NC_H__
unsigned char far *BMP;
/*struct _Color_ {
  unsigned char ht0,ht1,xyz,zb,g92,mess,crt,vol,sta0,sta1,sta2,edit,eblock,erun,hm0,hm1,hf0,hf1;
}*/
struct _Color_ Color=  {0x48,0x28,0x32,0x30,0x5d,0x25,0x35,0xfd,
0x00,0xe5,0xe5,0x35,0xc6,0xc7,0x00,0x3a,0x39,0x38,0x52};
struct _Color_ BkColor={0xc8,0xc8,0xaa,0xaa,0xc5,0xd5,0xd6,0xd7,
0xe5,0x2c,0x28,0xc5,0x2e,0x2a,0x7a,0x5a,0x59,0x48,0xac};
struct _XYZ_ oxyz={0,0,0};
struct _XYZ_ offset={0,0,0};
struct _XYZ_ src={0,0,0};
struct _XYZ_ dest={0,0,0};
struct _XYZ_ position={0,0,0};
char CdsSta=1,OilSta=1;
char DispStr[0x100];
char HM[10]="\1\1\0\0\0\0\0\0\0\0";/*手动*/
char HF[10]="\x0\x1\x3\x4\x7\x3\x06\x0d\x0f\x11";
char HF1[20][11]={
"\0\0\0\0\0\0\0\0\0\0\0","\0\0\0\0\0\0\0\0\0\0\0","\0\0\0\0\0\0\0\0\0\0\0",
"\0\0\0\0\0\0\0\0\0\0\0","0\0\0\2\2\2\0\0\0\0\0","\0\0\0\0\0\0\0\0\0\0\0",
"\0\0\0\0\0\0\0\0\0\0\0","\0\0\0\0\0\0\0\0\0\0\2","\0\0\0\0\2\4\4\4\4\4\0",
"\0\0\0\0\0\0\0\0\0\0\0","\0\0\0\0\0\0\0\0\0\0\0","\0\0\0\0\0\0\0\0\0\0\0",
"\0\0\0\0\0\0\0\0\0\0\0","\0\0\0\0\0\0\0\0\0\0\0","\0\0\0\0\0\0\0\0\0\0\0",
"\0\0\0\0\0\0\0\0\0\0\0","\0\0\0\0\0\0\0\0\0\0\0","\0\0\0\0\0\0\0\0\0\0\0",
};
char HF2[20][11]={
"\0\0\0\0\0\0\0\0\0\0\0","\0\0\0\0\0\0\0\0\0\0\0","\0\0\0\0\0\0\0\0\0\0\0",
"\0\0\0\0\0\0\0\0\0\0\0","0\0\0\2\2\2\0\0\0\0\0","\0\0\0\0\0\0\0\0\0\0\0",
"\0\0\0\0\0\0\0\0\0\0\0","\0\0\0\0\0\0\0\0\0\0\2","\0\0\0\0\2\4\4\4\4\4\0",
"\0\0\0\0\0\0\0\0\0\0\0","\0\0\0\0\0\0\0\0\0\0\0","\0\0\0\0\0\0\0\0\0\0\0",
"\0\0\0\0\0\0\0\0\0\0\0","\0\0\0\0\0\0\0\0\0\0\0","\0\0\0\0\0\0\0\0\0\0\0",
"\0\0\0\0\0\0\0\0\0\0\0","\0\0\0\0\0\0\0\0\0\0\0","\0\0\0\0\0\0\0\0\0\0\0",
};
/*Edit.c*/
struct _EDIT_ edit={0,1,0,0,0,0,0,0};
char far * EDITA;
char far * PROCONA;
char insert_sta=1;
char block_sta=0;
char bselect_sta=0; /*0:无1:已定开始点2:已定结束点*/
struct _PROCON_ procon={0,0,0,1,1,0,0,0,0,5,0,"+0.000","100","8.0","0.250","100","8.0","0.250"};
char proconfn=0;
/*keyboard*/
char Ks8=0;
char Ks6=0;
char Ks5=0;
char Ks4=0;
char Ks3=0;
char SlZB=0;
char *SlZBS[]={"机械坐标","绝对坐标","剩余坐标","当前深度","移动速度","     "};
char G92=0;
int G92S[60]={54,55,56,57,58,59,154,155,156,157,158,159,
254,255,256,257,258,259,354,355,356,357,358,359,454,455,456,457,458,459,
554,555,556,557,558,559,654,655,656,657,658,659,754,755,756,757,758,759,
854,855,856,857,858,859,954,955,956,957,958,959};
/*ListLN.c*/
char far * NM_Str;
char far * LN02A;
char far * LN034A;
char far * LN04A;
char far * LN05A;
char far * COMPL01;
char FFN[100];
char BFFN[100];
char PFFN[100];
char LN_NM[3]={9,0,0};/*0:max wide 1:修改数字第N个*/
char *strspace="                                                  ";
FILE *FP,*fp1;
char NCStr[9]="NC\\??.NC";
char BNCStr[9]="BK\\??.BK";
char PNCStr[9]="PC\\??.PC";
char TempFN[12]="Block.$$$";
char LN00K=0;
char LN01K=0;
char LN02K=0;
char LN030K=0,LN031K=0,LN032K=0,LN033K=0;
char LN04K=0;
char LN05K=0;
char ErrorS=0;
char LN01S[__LN01K__]={0,0,0,0,0,0,0,2,0,0,2,0,2,0};
char LN030S[__LN030K__]={0,0,0,2,2,2,2,0,0,0};
char LN031S[__LN031K__]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,2};
char LN032S[__LN032K__]={0,0,0,0,0,0,0,0,0,0,0,0,0,2};
char LN033S[__LN033K__]={0,0,0,0,0,0,2};
char LN04S[__LN04K__]={0,0,2,2,0,2};
struct _LNStr_ LNStr={0,0,0,0,0,0};
struct _LN01_ LN01={0,0,5,0,0,0,1,0,12,13,11,11,15,21,"-1.000","100","8.0","0.250","0.150","80","10"};
struct _LN02_ LN02={0,0,0,1,1};
struct _LN030_ LN030={0,0,1,0,1,17,0,0,0,0,"54","+0.000","+0.000","+0.000","+1.000"};
struct _LN031_ LN031={0,0,0,1,16,17,0,0,0,2,14,2,14,0,11,"1","54","+0.000","+0.000","+0.000","10.000","2","10.000","2","+1.000","0.000"};
struct _LN032_ LN032={0,0,0,1,17,0,0,0,18,2,2,14,1,11,"54","+0.000","+0.000","+0.000","1.000","0.000","0.000","1","+1.000","0.000"};
struct _LN033_ LN033={0,0,0,1,14,0,11,"1","+1.000","0.000"};
struct _LN034_ LN034={0,0,0,0,17,0,"54","+0.000"};
struct _LN04_ LN04={17,14,0,0,0,2,"","","","","","1.000"};
struct _LN041_ LN041={0,0,0,0,0,0,0,0,0,1000};
struct _TABFILENAME_ tabfilename={"TAB\\","AutoLN","Cu-St","Gr-St","CuW-WC"};
struct _EXTNAME_ extname={".NC",".TAB"};
struct _TABLEN_ tablen={79,4,1,5,3,3,3,2,2,2,2,2,2,5,5,4,5,3,5,2};
struct _TABLEN_ stablen={19,0,5,7,13,17,21,25,28,31,34,37,40,43,49,55,60,66,70,76};
struct _TABLEN_ tabtype={0,20,22,21,20,20,20,20,20,20,20,20,20,11,11,20,11,20,13,20};
struct _TABLEN_ pctablen={71,5,54,18,24,28,32,36,39,42,45,48,51,10,00,56,61,67,0,0};
struct _TAB_ *tab[100],*tabprocon[100];
struct _TAB_ tabpc={"C000","+","006.0","160","070","060","52","12","12","02","00","01","0.150","0.126","0000","0.000","300","45","0"};
char *tabname[]={"NO.","PL","IP","ON","OFF","SV","S","UP","DN","V","LC","PP","B_OFS","S_OFS","LNS","STEP","LNM","RY",""};
char *tabname1[]={"NO. ","PL","IP   ","ON ","OFF","SV ","S ","UP","DN","V ","LC","PP","B_OFS","S_OFS","LNS ","STEP ","LNM","RY",""};
char *tabnamepc[]={"加工","NO.","底面残量","IP","ON","OFF","SV","S","UP","DN","V","LC","PP","PL","LNS","STEP","LNM",""};
#endif
