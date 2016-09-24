#include "znc.h"
#include <dos.h>
#include <conio.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#define JD  5
char RunCodeGroups[100];
char RunCodeLines[100];
int RunCodenumbers[100];
char M9899=0;
struct Run_Pi{
    int PE;
    int PV;
    int PT;
    char XYZ[1];
    long XYZV0[1];
    long XYZV1[1];
}RunPi;
char G0203;
extern char LoranFlag;
char offsetxyz=1;   /*0机床,1当前坐标*/
char CodeAbs=0; /*0绝对,1相对坐标编程*/
char RunCodeLine;/*当前代码行*/
char RunCodeGroup;/*当前代码组*/
char MXYZ=0;        /*平面选择*/
char End_flag=0;
char Runline_sta=0;
void C1(void);      /*Code Group*/
void C2(char flag); /*Edit Code*/
void C9(void);      /*CNC*/
void C10(char flag);
void DelCodeLine(void);
void CheckCode(void);
void CtoCode(void);
void CodetoC(void);
unsigned char Diss(void);
void RunCode(char flag);
extern void Loran_01(char flag);
extern void Loran_02(char flag);
extern void Loran_03(char flag);
extern void Loran_04(char flag);
extern void Loran_05(char flag);
extern void Loran_06(char flag);
extern void Loran_07(char flag);
Read_StartEnd(){
    unsigned i;
    if(cncode.G<100)Read_Table(cncode.G);
    i=cncode.D;i>>=4;if(i<9)Dis_start=i;else Dis_start=0;
    i=cncode.D;i&=0xf;
    if(i<9){Dis_end=i;if(cncode.D>=0xb0)Dis_start=Dis_end;}
    else Dis_end=Dis_start;
    Dis_lines=Dis_start;
}
SRunPi(char xyz,long v0,long v1){
    RunPi.XYZ[0]=xyz;
    RunPi.XYZV0[0]=v0;/*起点*/
    RunPi.XYZV1[0]=v1;/*终点*/
}
struct Dis_position{
    char XYZ;   /*X:0 Y:1 Z:2*/
    char Position_S; /*0：正向放电（向下），1：反向放电（向上）*/
    long Position_H; /*放电起点*/
    long Position_E; /*放电终点*/
    long Position_V; /*当前位置*/
}DIS;
char SRunDIS(char flag)
{
    FILE *fp;
    QNode *S_enode;
    S_enode = NULL;
    if(flag);
    DIS.XYZ=RunPi.XYZ[0];
    DIS.Position_H=RunPi.XYZV0[0];
    DIS.Position_E=RunPi.XYZV1[0];
/*    fp=fopen("FILEA\\work.txt","a+");
    fprintf(fp,"%d\t%ld\t%ld\n",DIS.XYZ,DIS.Position_H,DIS.Position_E);
    fclose(fp);   */
    return 0;
}
Run3DLine(){

}
Pause()
{
     /* Close_OCSF();
      Close_Elect();
      Dis_flag=0;   */
      Position_control_x(cncode.X);
      Position_control_y(cncode.Y);
      Position_control_z(cncode.Z);
    /*  Signal_1100X();
      Signal_1100Y();
      Signal_1100();     */
}
long XYZPosi(char flag){
if(DIS.XYZ==0)return XPosi(flag);
else if(DIS.XYZ==1)return YPosi(flag);
else if(DIS.XYZ==2)return ZPosi(flag);
}
long XYZPosif(char flag){
if(flag==0)return XPosi(0);
else if(flag==1)return YPosi(0);
else if(flag==2)return ZPosi(0);
}
Position_control_xyz(long l)
{
if(DIS.XYZ==0)Position_control_x(l);
else if(DIS.XYZ==1)Position_control_y(l);
else if(DIS.XYZ==2)Position_control_z(l);
}
Position_control_xyzf(char flag,long l)
{
if(flag==0)Position_control_x(l);
else if(flag==1)Position_control_y(l);
else if(flag==2)Position_control_z(l);
}
Signal_JianxiXYZ(char flag)
{
if(DIS.XYZ==0)Signal_JianxiX(flag);
else if(DIS.XYZ==1)Signal_JianxiY(flag);
else if(DIS.XYZ==2)Signal_Jianxi(flag);
}
Velocity_control_xyz(int i)
{
if(DIS.XYZ==0)Velocity_control_x(i);
else if(DIS.XYZ==1)Velocity_control_y(i);
else if(DIS.XYZ==2)Velocity_control_z(i);
}
Signal_1100XYZ()
{
if(DIS.XYZ==0)Signal_1100X();
else if(DIS.XYZ==1)Signal_1100Y();
else if(DIS.XYZ==2)Signal_1100();
}
Init1100_XYZ()
{
}
void Feedback() /*抬头*/
{
    long x0,y0,z0,cX,cY,cI,cJ;
    if(Current_DIS -> xyz == 1)
    {
        x0 = XPosi(0);y0 = YPosi(0);z0 = ZPosi(0);
        Write_target(Current_DIS->x0-x0,Current_DIS->y0-y0,Current_DIS->z0-z0);
            /*切换为速度模式回退*/
        Discharge(Current_DIS->x0-x0,Current_DIS->y0-y0,Current_DIS->z0-z0);
    }
    else if(Current_DIS -> xyz == 2 || Current_DIS -> xyz == 3)
    {
        cX=Current_DIS->x0;cY=Current_DIS->y0;
        cI=Current_DIS->z0;cJ=Current_DIS->z1;
        if(MXYZ==0)
        {
            x0=XPosi(1);y0=YPosi(1);
        }
        else  if(MXYZ==1)
        {
            x0=ZPosi(1);y0=XPosi(1);
        }
        else  if(MXYZ==2)
        {
            x0=YPosi(1);y0=ZPosi(1);
        }
        Write_targetarc(cX - x0,cY - y0,cI - x0,cJ - y0);
        Discharge1(cX - x0,cY - y0);
        delay(0x800);
        outb((Inter),0x14);
        if(!(Current_DIS -> xyz-2))
            outb((Inter+1),0x0b);/*顺圆进给，则逆圆回退*/
        else
            outb((Inter+1),0x09);/*逆圆进给，则顺圆回退*/
    }
}
void Feed()/*进给*/
{
    long x0,y0,z0,cX,cY,cI,cJ;
    if(Current_DIS -> xyz == 1)
    {
        x0 = XPosi(0);y0 = YPosi(0);z0 = ZPosi(0);
        Write_target(Current_DIS->x1-x0,Current_DIS->y1-y0,Current_DIS->z1-z0);
        Discharge(Current_DIS->x1-x0,Current_DIS->y1-y0,Current_DIS->z1-z0);/*切换为速度模式回退*/
    }
    else if(Current_DIS -> xyz == 2 || Current_DIS -> xyz == 3)
    {
        cX=Current_DIS->x1;cY=Current_DIS->y1;
        cI=Current_DIS->z0;cJ=Current_DIS->z1;
        if(MXYZ==0)
        {
            x0=XPosi(1);y0=YPosi(1);
        }
        else  if(MXYZ==1)
        {
            x0=ZPosi(1);y0=XPosi(1);
        }
        else  if(MXYZ==2)
        {
            x0=YPosi(1);y0=ZPosi(1);
        }
        Write_targetarc(cX - x0,cY - y0,cI - x0,cJ - y0);
        Discharge1(cX - x0,cY - y0);
        delay(0x800);
        outb((Inter),0x14);
        if(!(Current_DIS -> xyz-2))
            outb((Inter+1),0x09);/*顺圆进给*/
        else
            outb((Inter+1),0x0b);/*逆圆进给*/
    }
}
void StopXYZ()
{
    OTPBX1&=0xf0;
    outb((ICXaddr),0x14);
    outb((ICXaddr+1),OTPBX1);
    OTPBY1&=0xf0;
    outb((ICYaddr),0x14);
    outb((ICYaddr+1),OTPBY1);
    IOZ1&=0xf0;
    outb((Address_Z),0x14);
    outb((Address_Z+1),IOZ1);
}
unsigned char Diss(void)
{
    FILE *fp;
    register i,j;
    static char a;
    static long ld,lh,up_distence;
    static int flagt;/*标记是否更新DIS*/
    static DY;
    long ll,ls,x0,y0,z0;
    char test[10];
    DIS_enode=NULL;
    itoa(Dis_flag,test,10);
    ListHT(1,test);
    switch(Dis_flag){
      case 0:
        DY=Table.Jianxi[Dis_lines]/2+41+
        Table.Gaoya[Dis_lines]*15;
        ld=100 + Table.Dianliu[Dis_lines] + MianJi / 3;
        lh=Table.Shenggao[Dis_lines]*20+ld+1;
        Dis_flag++;
        AI=0;AITime=0;AIHigh=0;ZeroFlag=0;
        up_distence=(long)MianJi*11;
        SetTable(Dis_lines);
        if(KEYL<0xB)ShowTable(0);
        else ShowCode(0);
        Open_Elect();  /*开变压器*/
        Open_OCSF();
        Delay1(0x1000);
        outb(OSC,0x14);
        outb(OSC+1,OTPS1&=0xfe);   /*DIS LED*/
        offset_t=Oldtime/18.2;/*获得加工时间*/
        SetDianliu(Table.Dianliu[Dis_lines]);
        Queue_edel(&DIS_que);/*清空放电轨迹队列*/
        if(Pump_flag){F[2]=1;if(KEYL==0)ShowKey(1,2);Open_Pump();}
        if(K[6]){Dis_flag=9;ListMess1("机器已被锁定！");return 0;}
        break;
      case 1:Dis_flag++;
      case 2:
        Open_OCSF();/*开振荡*/
        Dis_flag++;
        break;
      case 3:
        x0=XPosi(0);
        y0=YPosi(0);
        z0=ZPosi(0);
        outb(Inter,0x11);
        a = inportb(Inter+1);
        a &= 0xc0;
        if((!(a&0x80)||!(a&0x40)) && Current_DIS == DIS_que.rear)/*终点判别*/
        {
            if(labs(x0 - Current_DIS->x1) <= 5 && labs(y0 - Current_DIS->y1) <= 5 && labs(z0 - Current_DIS->z1) <= 5)
                return 50;
           /* else if()   */
                
        }
        if(Voltage()<=Table.Jianxi[Dis_lines]){GetTime((long)0); Dis_flag++;}/*开始放电，并计时*/
        break;
      case 4:
        x0=XPosi(0);
        y0=YPosi(0);
        z0=ZPosi(0);
        outb(Inter,0x11);
        a = inportb(Inter+1);
        a &= 0xc0;
        if((!(a&0x80)||!(a&0x40)) && labs(x0 - cncode.X) <= 5 && labs(y0 - cncode.Y) <= 5 && labs(z0 - cncode.Z) <= 5)/*终点判别*/
            return 50;
         if(GetTime(Work_time)){
        Close_OCSF();/*抬头*/
        GetTime((long)0);
        Dis_flag++;
       }
         break;
    case 5:/*抬头*/
      x0=XPosi(0);
      y0=YPosi(0);
      z0=ZPosi(0);
      outb(Inter,0x11);
      a = inportb(Inter+1);
      if(GetTime(Up_time))
      {
        StopXYZ();
        if(Current_DIS != DIS_que.rear)/*非当前段*/
            Feed();    
        Dis_flag++;
      }
      else
      {
        if((!(a&0x80)||!(a&0x40)) && labs(Current_DIS->x0 - x0) <= 5 && labs(Current_DIS->y0 - y0) <= 5 &&
         labs(Current_DIS->z0 - z0) <= 5 && Current_DIS != DIS_que.front)/*退到位*/
        {
            Current_DIS = Current_DIS -> prior;
            Feedback();
        }
      }
      break;
    case 6: /*下降*/
      x0=XPosi(0);
      y0=YPosi(0);
      z0=ZPosi(0);
      outb(Inter,0x11);
      a = inportb(Inter+1);
      if(Current_DIS != DIS_que.rear && (!(a&0x80)||!(a&0x40))  && labs(Current_DIS->x1 - x0) <= 5 &&
      labs(Current_DIS->y1 - y0) <= 5 && labs(Current_DIS->z1 - z0) <= 5)
         {
            Current_DIS = Current_DIS -> next;
            if(Current_DIS != DIS_que.rear)
                Feed();
            else
            {
                Open_OCSF();
                Dis_flag = 3;
            }
         }
      break;
    case 7:
    break;
    case 9: break;
    }
    return 0;
}
void RunCodeEnd(char flag){
    /*F11(); */
    RunCodeLine=0;RunCodeGroup=CodeGroup;
    if(C[8]>1)C[8]=1;ShowCode(0);
    if(!flag){CodeAbs=0;offsetxyz=1;MXYZ=0;}
}
void StartCode(){
    Runline_sta=0;
    C[8]=2;RunCodeLine=0;RunCodeGroup=CodeGroup;/*CodeAbs=0;*/
    RunPi.PT=0;RunPi.PV=-1;
}
void RunCode(char flag){
  register long l=0x7fffffff;
  int sta=0;
  int i;
  static char G04_RunCodeGroup,G04_RunCodeLine,G04_RunLineFlag,G04_Dis_flag,G05_RunCodeLine;
  static char G04_Group,G04_XYZGroup;
  static long G04_X,G04_Y,G04_Z;
  long offsetx=0,offsety=0,offsetz=0;
  static int Runline=-1;
  unsigned char a,b,c;
  char test[10];
  ShowCode(0);
  if(offsetxyz){offsetx=offset_x;offsety=offset_y;offsetz=offset_z;}
  if(!flag){
    memcpy(&cncode,Code+RunCodeLine*35,35);
    if(Runline!=cncode.line)
      {
        if(Runline>0)
        ListEdit(Runline,0);
        ListEdit(cncode.line,1);
        Runline=cncode.line;
      }
    if(cncode.X!=l)cncode.X/=DZC;if(cncode.Y!=l)cncode.Y/=DZC;
    if(cncode.Z!=l)cncode.Z/=DZC;if(cncode.R!=l)cncode.R/=DZC;
    if(!CodeAbs){
      if(cncode.X!=l)position_px=cncode.X;
      if(cncode.Y!=l)position_py=cncode.Y;
      if(cncode.Z!=l)position_pz=cncode.Z;
    }else{
      if(cncode.X!=l)position_px=position_x+cncode.X;
      if(cncode.Y!=l)position_py=position_y+cncode.Y;
      if(cncode.Z!=l)position_pz=position_z+cncode.Z;
    }
  }
      switch(cncode.Code){
    case 00:        /*G00快速点定位*/
      lock_x=0;lock_y=0;lock_z=0;
      if(!CodeAbs){
            if(0==Runline_sta)
              { if(cncode.X!=l)
                  {Gposition_x=(cncode.X-offsetx)-XPosi(0);Runline_stx=1;}
                else
                  Gposition_x=0;
                if(cncode.Y!=l)
                  {Gposition_y=(cncode.Y-offsety)-YPosi(0);Runline_sty=1;}
                else
                  Gposition_y=0;
                if(cncode.Z!=l)
                  {Gposition_z=(cncode.Z-offsetz)-YPosi(0);Runline_stz=1;}
                else
                  Gposition_z=0;
                Runline_sta=1;
              }
      }else{
        if(0==Runline_sta)
          {  if(cncode.X!=l)
               {Gposition_x=cncode.X + XPosi(0);Runline_stx=1;}
             else
               Gposition_x=0;
             if(cncode.Y!=l)
               {Gposition_y=cncode.Y + YPosi(0);Runline_sty=1;}
             else
               Gposition_y=0;
             if(cncode.Z!=l)
               {Gposition_z=cncode.Z + ZPosi(0);Runline_stz=1;}
             else
               Gposition_z=0;
             Runline_sta=1;
           }
      }
      if(Runline_stx)
        {Position_control_x(Gposition_x);Runline_stx = 0;}
      if(Runline_sty)
        {Position_control_y(Gposition_y);Runline_sty = 0;}
      if(Runline_stz)
        {Position_control_z(Gposition_z);Runline_stz = 0;}
      if(!PositionEndXYZ(0x84))
        {
          End_flag++;
          if(3==End_flag)
            {sta++;Runline_sta=0;End_flag=0;}
        }
      else
        End_flag=0;
      RunPi.PT=0;
    break;
    case 01:        /*G01直线插补*/
      Read_StartEnd();
      if(C[8]==3)
        {
           sta++;Runline_sta=0;
        }
      else{
        C[8]=3;
        RunLineFlag=0;/*RunPi.PT=0;*/
        if(!CodeAbs){
            if(0==Runline_sta)
              { if(cncode.X!=l)
                  Gposition_x=cncode.X-offsetx;
                else
                  Gposition_x=XPosi(0);
                if(cncode.Y!=l)
                  Gposition_y=cncode.Y-offsety;
                else
                  Gposition_y=YPosi(0);
                if(cncode.Z!=l)
                  Gposition_z=cncode.Z-offsetz;
                else
                  Gposition_z=ZPosi(0);
              }
        }
        else{
         if(0==Runline_sta)
          {
             if(cncode.X!=l)
               Gposition_x=XPosi(0)+cncode.X;
             else
               Gposition_x=XPosi(0);
             if(cncode.Y!=l)
               Gposition_y=YPosi(0)+cncode.Y;
             else
               Gposition_y=YPosi(0);
             if(cncode.Z!=l)
               Gposition_z=ZPosi(0)+cncode.Z;
             else
               Gposition_z=ZPosi(0);
           }
      }
      Runline_sta=1;
      cncode.X=Gposition_x;
      cncode.Y=Gposition_y;
      cncode.Z=Gposition_z;
      }
    break;
    case 02:        /*G02圆弧插补(顺圆)*/
      G0203=0;
    case 03:        /*G03圆弧插补(逆圆)*/
      if(C[8]==5)sta++;
      else{
        C[8]=5;RunLineFlag=0;
        if(!CodeAbs){
                if(cncode.X!=l)
                  Gposition_x=cncode.X-offsetx;
                else
                  Gposition_x=XPosi(0);
                if(cncode.Y!=l)
                  Gposition_y=cncode.Y-offsety;
                else
                  Gposition_y=YPosi(0);
                if(cncode.Z!=l)
                  Gposition_z=cncode.Z-offsetz;
                else
                  Gposition_z=ZPosi(0);
                if(cncode.I!=l)
                  Gposition_i=cncode.I-offsetx;
                else
                  Gposition_i=XPosi(0);
                if(cncode.J!=l)
                  Gposition_j=cncode.J-offsety;
                else
                  Gposition_j=YPosi(0);
                if(cncode.K!=l)
                  Gposition_k=cncode.K-offsetz;
                else
                  Gposition_k=ZPosi(0);
        }
        else{
             if(cncode.X!=l)
               Gposition_x=XPosi(0)+cncode.X;
             else
               Gposition_x=XPosi(0);
             if(cncode.Y!=l)
               Gposition_y=YPosi(0)+cncode.Y;
             else
               Gposition_y=YPosi(0);
             if(cncode.Z!=l)
               Gposition_z=ZPosi(0)+cncode.Z;
             else
               Gposition_z=ZPosi(0);
             if(cncode.I!=l)
                Gposition_i=XPosi(0)+cncode.I;
             else
                Gposition_i=XPosi(0);
             if(cncode.J!=l)
                Gposition_j=YPosi(0)+cncode.J;
             else
                Gposition_j=YPosi(0);
             if(cncode.K!=l)
                Gposition_k=ZPosi(0)+cncode.K;
             else
                Gposition_k=ZPosi(0);
          }
          if(MXYZ==0){if(cncode.X==l && cncode.Y==l)sta++;}
          else{ if(MXYZ==1){if(cncode.Z==l && cncode.X==l)sta++;}
          else  if(MXYZ==2){if(cncode.Y==l && cncode.Z==l)sta++;}
          }/*缺坐标*/
          cncode.X=Gposition_x;
          cncode.Y=Gposition_y;
          cncode.Z=Gposition_z;
          cncode.I=Gposition_i;
          cncode.J=Gposition_j;
          cncode.K=Gposition_k;
          if(cncode.Code==03)G0203=1;
        }
    break;
    case 0xfe:
    case 04:        /*G04暂停*/
    G04_Group=Group;
    G04_XYZGroup=XYZGroup;
        G04_Dis_flag=Dis_flag;
    G04_RunCodeGroup=RunCodeGroup;
    G04_RunCodeLine=RunCodeLine;
    if(cncode.Code!=0xfe)G04_RunCodeLine++;
    if(G04_RunCodeLine>=50)G04_RunCodeGroup++;
    if(G04_RunCodeGroup>=100){cncode.Code=103;RunCode(1);}
    G04_X=XPosi(1);G04_Y=YPosi(1);G04_Z=ZPosi(1);
    if(cncode.Code==4){delay(100);F11();
    offset_rx=G04_X;offset_ry=G04_Y;offset_rz=G04_Z;
    Position_control_x(offset_rx);
    Position_control_y(offset_ry);
    Position_control_z(offset_rz);
        }
    G04_RunLineFlag=RunLineFlag;
    break;
    case 0xfd:
    if(G04>1){
    Position_control_x(G04_X);Position_control_y(G04_Y);Position_control_z(G04_Z);
    RunLineFlag=G04_RunLineFlag;
    Dis_flag=G04_Dis_flag;
    KeyN=G04_Group;F[8]=1;F9();
    KeyN=G04_XYZGroup;F[9]=1;F10();ShowFlag(6);
    KeyN=G04_RunCodeGroup;C[0]=1;C1();
    if(Dis_flag){Open_OCSF();Open_Elect();SetTable(Dis_lines);
    if(Dis_flag==3)Dis_flag=1;}
    RunCodeLine=G04_RunCodeLine;
    if(RunCodeGroup!=G04_RunCodeGroup){
        RunCodeGroup=G04_RunCodeGroup;
    Read_Code(RunCodeGroup);
    }C[8]=G04;cncode.Code=0;
    sta++;
    }else   StartCode();
    break;
    #if PNC
    #else
    case 05:        /*G17选择XY平面0*/
    MXYZ=0;
    sta++;
    break;
    case 06:        /*G18选择ZX平面1*/
    MXYZ=1;
    sta++;
    break;
    case 07:        /*G19选择YZ平面2*/
    MXYZ=2;
    sta++;
    break;
    case 8:         /*G40补偿撤消*/
    sta++;
    break;
    case 9:         /*G41左偏补偿*/
    sta++;
    break;
    case 10:        /*G42右偏补偿*/
    sta++;
    break;
    #endif
    case 11:        /*G53选择机床坐标*/
      offsetxyz=0;sta++;
    break;
    case 12:        /*G54选择第一工件坐标*/
      F[9]=1;KeyN=4;F10();ShowFlag(6);sta++;
    break;
    case 13:        /*G55选择第二工件坐标*/
      F[9]=1;KeyN=5;F10();ShowFlag(6);sta++;
    break;
    case 14:        /*G56选择第三工件坐标*/
      F[9]=1;KeyN=6;F10();ShowFlag(6);sta++;
    break;
    case 15:        /*G57选择第四工件坐标*/
      F[9]=1;KeyN=7;F10();ShowFlag(6);sta++;
    break;
    case 16:        /*G58选择第五工件坐标*/
      F[9]=1;KeyN=8;F10();ShowFlag(6);sta++;
    break;
    case 17:        /*G59选择第六工件坐标*/
      F[9]=1;KeyN=9;F10();ShowFlag(6);sta++;
    break;
    #if PNC
    #else
    case 18:        /*G63镗圆(顺圆)*/
      if(C[8]==4)sta++;
      else{
        if(cncode.R==l)cncode.R=0;  /*半径*/
       C[8]=4;RunLineFlag=0;/*RunPi.PT=0;*/
       if(cncode.R<0)cncode.R=-cncode.R;
        if(!CodeAbs){
          if(cncode.X!=l)cncode.X-=offsetx;else cncode.X=XPosi(1);
          if(cncode.Y!=l)cncode.Y-=offsety;else cncode.Y=YPosi(1);
          if(cncode.Z!=l)cncode.Z-=offsetz;else cncode.Z=ZPosi(1);
        }
        else{
          if(cncode.X!=l)cncode.X+=XPosi(1);else cncode.X=ZPosi(1);
          if(cncode.Y!=l)cncode.Y+=YPosi(1);else cncode.Y=ZPosi(1);
          if(cncode.Z!=l)cncode.Z+=ZPosi(1);else cncode.Z=ZPosi(1);
          }
        }
    break;
    case 19:        /*G64镗圆(逆圆)*/
      if(C[8]==4)sta++;
      else{
        if(cncode.R==l)cncode.R=0;  /*半径*/
       C[8]=4;RunLineFlag=0;/*RunPi.PT=0;*/
       if(cncode.R>0)cncode.R=-cncode.R;
        if(!CodeAbs){
          if(cncode.X!=l)cncode.X-=offsetx;else cncode.X=XPosi(1);
          if(cncode.Y!=l)cncode.Y-=offsety;else cncode.Y=YPosi(1);
          if(cncode.Z!=l)cncode.Z-=offsetz;else cncode.Z=ZPosi(1);
        }
        else{
          if(cncode.X!=l)cncode.X+=XPosi(1);else cncode.X=ZPosi(1);
          if(cncode.Y!=l)cncode.Y+=YPosi(1);else cncode.Y=ZPosi(1);
          if(cncode.Z!=l)cncode.Z+=ZPosi(1);else cncode.Z=ZPosi(1);
          }
        }
    break;
    case 20:        /*G65镗方(顺方)*/
      if(C[8]==6)sta++;
      else{
        if(cncode.R==l)cncode.R=0;  /*半径*/
       C[8]=6;RunLineFlag=0;/*RunPi.PT=0;*/
       if(cncode.R<0)cncode.R=-cncode.R;
        if(!CodeAbs){
          if(cncode.X!=l)cncode.X-=offsetx;else cncode.X=XPosi(1);
          if(cncode.Y!=l)cncode.Y-=offsety;else cncode.Y=YPosi(1);
          if(cncode.Z!=l)cncode.Z-=offsetz;else cncode.Z=ZPosi(1);
        }
        else{
          if(cncode.X!=l)cncode.X+=XPosi(1);else cncode.X=ZPosi(1);
          if(cncode.Y!=l)cncode.Y+=YPosi(1);else cncode.Y=ZPosi(1);
          if(cncode.Z!=l)cncode.Z+=ZPosi(1);else cncode.Z=ZPosi(1);
          }
        if(cncode.R){cncode.X=cncode.R;cncode.Y=cncode.R;}
        }
    break;
    case 21:        /*G66镗方(逆方)*/
      if(C[8]==6)sta++;
      else{
        if(cncode.R==l)cncode.R=0;  /*半径*/
       C[8]=6;RunLineFlag=0;/*RunPi.PT=0;*/
       if(cncode.R>0)cncode.R=-cncode.R;
        if(!CodeAbs){
          if(cncode.X!=l)cncode.X-=offsetx;else cncode.X=XPosi(1);
          if(cncode.Y!=l)cncode.Y-=offsety;else cncode.Y=YPosi(1);
          if(cncode.Z!=l)cncode.Z-=offsetz;else cncode.Z=ZPosi(1);
        }
        else{
          if(cncode.X!=l)cncode.X+=XPosi(1);else cncode.X=ZPosi(1);
          if(cncode.Y!=l)cncode.Y+=YPosi(1);else cncode.Y=ZPosi(1);
          if(cncode.Z!=l)cncode.Z+=ZPosi(1);else cncode.Z=ZPosi(1);
          }
        if(cncode.R){cncode.X=cncode.R;cncode.Y=cncode.R;}
        }
    break;
    case 22:        /*G67镗十*/
    LoranFlag=cncode.D;
    switch(LoranFlag){
        case 1:Loran_01(0);Diss();break;   /*○*/
        case 2:Loran_02(0);Diss();break;   /*□*/
        case 3:Loran_03(0);Diss();break;   /*◇*/
        case 4:Loran_04(0);Diss();break;   /*×*/
        case 5:Loran_05(0);Diss();break;   /*＋*/
        case 6:Loran_06(0);Diss();break;   /*△*/
        case 7:Loran_07(0);Diss();break;   /*＊*/
        default:LoranFlag=0;
    }
    C[8]=100;
    /*sta++; */
    break;
    case 23:        /*G68镗＊*/
    break;
    case 24:        /*G69镗？*/
    break;
    case 25:        /*G81钻孔*/
    break;
    case 26:        /*G85镗孔*/
      if(C[8]==4)sta++;
      else{
        if(cncode.R==l)cncode.R=0;  /*半径*/
       C[8]=4;RunLineFlag=0;/*RunPi.PT=0;*/
        if(!CodeAbs){
          if(cncode.X!=l)cncode.X-=offsetx;else cncode.X=XPosi(1);
          if(cncode.Y!=l)cncode.Y-=offsety;else cncode.Y=YPosi(1);
          if(cncode.Z!=l)cncode.Z-=offsetz;else cncode.Z=ZPosi(1);
        }
        else{
          if(cncode.X!=l)cncode.X+=XPosi(1);else cncode.X=XPosi(1);
          if(cncode.Y!=l)cncode.Y+=YPosi(1);else cncode.Y=YPosi(1);
          if(cncode.Z!=l)cncode.Z+=ZPosi(1);else cncode.Z=ZPosi(1);
          }
        }
    break;
    #endif
    case 27:        /*G90绝对方式编程*/
      CodeAbs=0;sta++;
    break;
    case 28:        /*G91增量方式编程*/
      CodeAbs=1;sta++;
    break;
    case 29:        /*G92设定工件坐标系*/
    sta++;
    break;
    case 100:        /*M00程序停*/
      G04=0;
      RunCodeEnd(1);
    break;
    case 101:        /*M01选择停*/
    sta++;
    break;
    case 102:        /*M02程序结束*/
      G04=0;
      RunCodeEnd(1);
    break;
    case 103:        /*M03程序结束返回*/
    G04=0;
    F11();
      C[8]=0;
      KEYL=0;ShowKey(1,0);ShowTable(-1);
    break;
    case 104:        /*M04开油泵*/
      if(!F[2])F3();sta++;
    break;
    case 105:        /*M05关油泵*/
      if(F[2])F3();sta++;
    break;
    case 106:        /*M06调用子程序*/
      G05_RunCodeLine = RunCodeLine;
      RunCodeLine = cncode.I;
    sta++;
    break;
    case 107:        /*M07子程序结束[可带返回点]*/
      RunCodeLine = G05_RunCodeLine;
      sta++;
      break;
    case 108:        /*M08主程序结束*/
      G04=0;
      Runline=-1;
      RunCodeEnd(0);
      KEYL=4;/*ShowTable(-1);K10();ShowKey(5,0);*/
    break;
    case 250:
      sta++;break;
    default:sta++;G04=0;Runline=-1;RunCodeEnd(0);return;
      }
      if(sta){
    C[8]=2;sta=RunCodeGroup;
    do{
      Nline=RunCodeLine;
      RunCodeLine++;
          memcpy(&cncode,Code+RunCodeLine*35,35);
      if(Runline!=cncode.line&&Runline!=-1)
      {
        if(Runline>0)
        ListEdit(Runline,0);
        ListEdit(cncode.line,1);
        Runline=cncode.line;
      }
    }while((RunCodeLine<100&&!RunCodeGroup||RunCodeLine<50&&RunCodeGroup)&&(cncode.N&0x80));
    if(RunCodeLine>=50){
    /*F11();*/C[8]=1;}
      CYY=Nline;if(CYY>9)CYY=9;
      ShowCode(0);
      }
}

