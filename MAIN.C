#include "znc.h"
#include <dos.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
 /*电源   油泵   声音   归零   中心   参数   高度   组别   坐标*/
 /*睡眠   上升   校模   油位   速度   锁定   讯     反加工 关机*/
 /*清时   XY保护 深孔   火警   积炭时 积炭高 积炭   钢蚀钢 口令 修底*/
extern struct Dis_position{
    char XYZ;
    char Position_S;
    long Position_H;
    long Position_E;
    long Position_V;
}DIS;
extern char LoranFlag;
void OpenPower(void);       /*打开副电源*/
void ClosePower(void);      /*关闭副电源*/
void Open_Pump(void);       /*开油泵*/
void Close_Pump(void);      /*关油泵*/
void OpenSound(void);       /*打开声音*/
void CloseSound(void);      /*关闭声音*/
void ClearAI(int flag);
void F11(void); /*停止放电*/
void StopF5();
void P2(void);  /*清时*/
void P3(void);  /*XY保护*/
void P5(void);  /*火警*/
void P6(void);  /*积炭时*/
void P7(void);  /*积炭高*/
void P8(void);  /*积炭*/
void P9(void);  /*钢蚀钢*/
void P10(void); /*口令*/
void P11(void); /*修底*/
void F2(void);  /*电源*/
void F3(void);  /*泵浦*/
void F4(void);  /*声音*/
void F5(char flag); /*归零*/
void F6(void);  /*中心*/
void F7(char flag); /*参数*/
void F8(char flag); /*高度*/
void F9(void);  /*组*/
void F10(void); /*坐标*/
void K2(void);  /*睡眠*/
void K3(void);  /*上升*/
void K4(void);  /*校模*/
void K5(void);  /*油位*/
void K6(void);  /*速度*/
void K7(void);  /*锁定*/
void K8(void);  /*讯息*/
void K9(void);  /*反加工*/
void K10(void); /*关机*/
long ReadRegisterValue_W(char cLowReg);
long ReadRegisterValue_V(char cLowReg);
void InitKABT(){
    set_PIDZ(8,0,4,0x10);
    set_PIDX(8,0,4,0x10);
    set_PIDY(8,0,4,0x10);
}
void Init_IO(void)
{
   OTPS1=0x3b;
   OTPS0=0x00;
   IOZ1=0x0;
   IOZ0=0xf0;
   OTPBX0=0xe0;
   OTPBX1=0x0;
   OTPBY0=0xe0;
   OTPBY1=0x00;
   IO0=0x10;       /*开24V*/
   IO1=0x80;
   IO2=0x00;
   IO3=0x00;       /*开高压*/
   IO4=0x00;
   outb(Address_Z,0x13);
   outb(Address_Z+1,IOZ0);
   outb(Address_Z,0x14);
   outb(Address_Z+1,IOZ1);
   outb(ICYaddr,0x13);
   outb(ICYaddr+1,OTPBY0);
   outb(ICYaddr,0x14);
   outb(ICYaddr+1,OTPBY1);
   outb(ICXaddr,0x13);
   outb(ICXaddr+1,OTPBX0);
   outb(ICXaddr,0x14);
   outb(ICXaddr+1,OTPBX1);
   outb(OSC,0x13);
   outportb(OSC+1,OTPS0);
   outb(OSC,0x14);
   outportb(OSC+1,OTPS1);
   outportb(IO,IO0);
   outportb(IO+1,IO1);
   outportb(IO+2,IO2);
   outportb(IO+3,IO3);
   outportb(IO+4,IO4);
}
void main(int argc,char *argv[]){
    register unsigned i;
    int k;
    char far *pi;
    long ll,lt;
    long    xll,yll;
    unsigned char a;
    char test[10];
    BMP=_BMP_();
    LN02A=BMP+10000; /*len 18000*/
    LN034A=BMP;/*len 9600*/
    LN04A=BMP+10000;/*len 10000*/
    LN05A=FFN;
    EDITA=BMP;
    PROCONA=BMP+0x8000;
    COMPL01=BMP+29000;
    for(i=0;i<100;i++)tabprocon[i]=(struct _TAB_  *)((char far *)PROCONA+i*(tablen.tablen)+1);
    FFileLN(BNCStr,BFFN);
    FFileLN(NCStr,LN05A);
    FFileLN(PNCStr,PFFN);
    Argc=argc-1;
    if(argc)Argv=argv[0];
    CreatTempFile();
    Init_IO();
    InitKABT();
 /*   Position_control_z(ZPosi(0));
    Speed1100_z(Elect_speed);
    Position_control_x(XPosi(0));
    Position_control_y(YPosi(0)); */
    Speed1100_x(Elect_speed);
    Speed1100_y(Elect_speed);
    ResetXYZ();
    Read_Table(Group);
    CheckTab();
    SetTable(Dis_lines);
    InitScr256(4);
    setfillstyle(1,BKCR0);
    bar(0,0,800-1,600-1);
    OpenFileM();
    ListHM(0);
    ListF12();
    ListHT(1,"华烽HF01234567890123456789012345678");
    ListXYZ00(0);
    ListZB00(0);
    ListZB(0,0,100);
    ListZB(0,1,9999999);
    ListZB(0,2,-9999999);
    ListG92();
    ListMess1("0123abc中文ABC");
    ListCRT1("0123456789012345678901234567890123456789012345");
    for(i=0;i<=12;i++)ListStatus(i);
    ListStatus(0x83);
    ListVol(120);
    Close_OCSF();
    Signal_1100X();
    Signal_1100Y();
    Signal_1100();
    outb(OSC,0xbf);
    outportb(OSC+1,0x3d);
    outb(OSC,0xdf);
    outportb(OSC+1,0x7c);
    Opens_V(soundV);
    ShowXYZ();
    ShowFlag(0);
    for(;;){
        if(ProTime()){
        ShowTime(1);
        V=Voltage();
        A=Current();
/*      if(V<10)WriteRAM(0x1f,0,0);*/
/*      Ck8255();*/
/*      Key();*/
        KeyBoard();
        if(!Scr_flag &&ProtectTime>18*540)CloseScr(0);
        if(Scr_flag<2&&ProtectTime>18*600)CloseScr(1);
/*      if(Testflag){ShowMess(34);F11();}
        else
        if(DIS_OK!=1&&DIS_OK1!=1){ShowMess(33);if(Dis_flag)F11();}ABS();
*/
        }
        /*if(F[4])Zero();*/
      switch(LoranFlag){
          case 1:if(Dis_flag){lock_x=0;lock_y=0;Loran_01(1);}break;
          case 2:if(Dis_flag){lock_x=0;lock_y=0;Loran_02(1);}break;
          case 3:if(Dis_flag){lock_x=0;lock_y=0;Loran_03(1);}break;
          case 4:if(Dis_flag){lock_x=0;lock_y=0;Loran_04(1);}break;
          case 5:if(Dis_flag){lock_x=0;lock_y=0;Loran_05(1);}break;
        }
         switch(C[8]){
          /*case 0:if(Dis_flag){if(!K[8])F12();else F13();}break; */
          case 1:/*if(Dis_flag)F11();*/break;
          case 2:RunCode(0);break;
          case 3:RunLine();break;
          case 4:RunCircle();break;
          case 5:RunArc();break;
          case 6:RunBar();break;
        }
        SaveXYZ(1);
        ShowTime(0);
        ShowTimeD(0);
        ListXYZ00(1);
    }
}

void P2(void){  /*清时*/
    if(P[1]){
        if(KeyN!=0x7FFFFFFF){
            if(KeyN==3140)ShowTimeD(0x80);
            if(KeyN==321000)position_t=0;
            if(KeyN==8888000)PassWordF=0;
        }
        P[1]=0;
        ShowInput(0);
    }
    else{
    ClearKey(0);ShowMess(29);MaxL=7;MaxN=9999999;MinN=-9999999;P[1]=1;
    }
}

void P3(void){  /*XY保护*/
    ClearKey(0);
    if(P[2])P[2]=0;
    else P[2]=1;
}
void P4(void){  /*深孔*/
    ClearKey(0);
    if(P[3])P[3]=0;
    else P[3]=1;
}
void P5(void){  /*火警*/
    ClearKey(0);
    if(P[4])P[4]=0;
    else P[4]=1;
    ShowTB(0x17);
}

void P6(void){  /*积炭时*/
AI_time=0;
if(P[5]==2){ClearKey(0);P[5]=0;return;}
if(P[5]==1){P[5]=2;
    if(KeyN!=0x7fffffff)AI_time=KeyN;
    if(!AI_time){P[5]=0;}ClearKey(0);
}
else{ClearKey(0);ShowMess(38);P[5]=1;MaxL=3;MaxN=999;MinN=1;}
}
void P7(void){  /*积炭高*/
AI_high=0;
if(P[6]==2){ClearKey(0);P[6]=0;return;}
if(P[6]==1){P[6]=2;
    if(KeyN!=0x7fffffff)AI_high=KeyN;
    if(!AI_high){P[6]=0;}ClearKey(0);
}
else{ClearKey(0);ShowMess(39);P[6]=1;MaxL=3;MaxN=999;MinN=1;}
}
void P8(void){  /*积炭开关*/
    ClearKey(0);
    if(P[7]==2)P[7]=0;
    else{if(!P[6]&&!P[5]){P[6]=2;AI_high=250/DZC;} P[7]++;}
    if(Dis_flag){if(AI)ClearAI(1);}
}

void P9(void){  /*钢蚀钢*/
steel=0;
Steel_flag=0;
if(P[8]==2){ClearKey(0);P[8]=0;if(Dis_flag)JiXin(JX=Table.Jixin[Dis_lines]);/*ShowSteel();*/return;}
if(P[8]==1){P[8]=2;
    if(KeyN!=0x7fffffff)Steel_flag=KeyN;
    if(!Steel_flag){P[8]=0;}ClearKey(0);
}
else{ClearKey(0);ShowMess(21);P[8]=1;MaxL=2;MaxN=99;MinN=1;}
/*ShowSteel();*/
}
void P10(void){ /*口令*/
    long l;
    l=KeyN;
    if(PassWordF==0x6d)return;
    if(P[9]){
        P[9]=0;ShowInput(0);
        if(l!=0x7FFFFFFF){
            if(l==3140){l=321000;Initsystem=1;}
            if(l==(long)321000){SetPass(0,0);KEYL=8;
            ShowMess(0);return;}
            else{ShowMess(30);}
        }else{ShowMess(0);}
    }
    else{
        ClearKey(0);
    if(Dis_flag){ShowMess(15);return;}
    else    if(F[1])F2();
    ShowMess(29);MaxL=7;MaxN=9999999;MinN=-9999999;P[9]=1;
    }
}
void P11(void){ /*修底*/
    ClearKey(0);
    if(P[10])P[10]=0;
    else P[10]=1;
}
void F2(void){
    ClearKey(0);
    if(F[1]){if(Dis_flag||F[4]){F11();}ClosePower();F[1]=0;}
    else if(!ExitTime){OpenPower();F[1]=1;}
    ShowFlag(1);
}
void OpenPower(void){       /*打开副电源*/
        IO1|=0x80;
        outb(IO+1,IO1);
}
void ClosePower(void){      /*关闭副电源*/
        IO1&=0x7f;
        outb(IO+1,IO1);
}
void F3(void){
        ClearKey(0);
    if(F[2]){Close_Pump();F[2]=0;Pump_flag = 0;}
    else{Open_Pump();F[2]=1;Pump_flag = 1;}
    ShowFlag(2);
}
void Open_Pump(void){       /*开油泵*/
        IO1|=0x10;
        outb(IO+1,IO1);
}
void Close_Pump(void){      /*关油泵*/
        IO1&=0xef;
        outb(IO+1,IO1);
}
void F4(void){
        ClearKey(0);
    if(F[3]){CloseSound();F[3]=0;}
    else{F[3]=1;}
}
void OpenSound(void){       /*打开声音*/
        if(F[3])
            {
             IO0|=0x08;
             outb(IO,IO0);
             Sound_flag=1;
            }
    ProtectTime=0;
    if(Scr_flag){OpenScr();}
}
void Opens_V(unsigned v){       /*声音大小*/ 
          if(F[3])
              {
                 IO0&=0xf0;
                 v&=0x07;
                 IO0|=v;
                 outb(IO,IO0);
              }
    ProtectTime=0;
    if(Scr_flag){OpenScr();}
}
void CloseSound(void){      /*关闭声音*/
          outb(IO,IO0&0xf7);Sound_flag=0;
}
void OpenScr(void){
    disable();
    outportb(0x3C4,1);
    outportb(0x3C5,inportb(0x3C5)&0xd7);
    enable();
    Scr_flag=0;
    if(FireFlag&&!(Ck8255_flag&0x80))FireFlag++;
    if(KeyPress())KeyAss();
    InitKey(1);
}
void CloseScr(char flag){
    disable();
    outportb(0x3C4,1);
    if(!flag)
    outportb(0x3C5,inportb(0x3C5)|0x20);
    else
    outportb(0x3C5,inportb(0x3C5)|0x08);
    enable();
    Scr_flag++;
}
void F5(char flag){
    register int i;
    int j;
    i=Ck8255_flag;
    ClearKey(0);
    j=F[4];
        StopF5();
    if(j==flag){
        if(F[4]==1||F[4]==2)Speed1100_x(Elect_speed);
        if(F[4]==3||F[4]==4)Speed1100_y(Elect_speed);
        if(F[4]==5||F[4]==6)Speed1100_z(Elect_speed);
    }
    else{
        if((flag==1&&i&0x01)||(flag==2&&i&0x02)||
        (flag==3&&i&0x04)||(flag==4&&i&0x08)||
        (flag==5&&i&0x10)||(flag==6&&i&0x20)||
        Dis_flag||!F[1]){ShowMess(15);return;}
  if(Voltage()<10){
    switch(flag){
    case 1:
    case 2:
        if(MOVEXYZ!=1&&MOVEXYZ!=2){ShowTB(0xb);return;}
        if(MOVEXYZ==1){Position_control_x(XPosi(0)-50/DZC);}
        if(MOVEXYZ==2){Position_control_x(XPosi(0)+50/DZC);}
        break;
    case 3:
    case 4:
        if(MOVEXYZ!=3&&MOVEXYZ!=4){ShowTB(0xb);return;}
        if(MOVEXYZ==3)Position_control_y(YPosi(0)-50/DZC);
        if(MOVEXYZ==4)Position_control_y(YPosi(0)+50/DZC);
        break;
    case 5:
    case 6:
        if(MOVEXYZ!=5&&MOVEXYZ!=6){ShowTB(0xb);return;}
        if(MOVEXYZ==5)Position_control_z(ZPosi(0)-50/DZC);
        if(MOVEXYZ==6)Position_control_z(ZPosi(0)+50/DZC);
        break;
    }
  }
        if(flag==1||flag==2)Speed1100_x(20);
        if(flag==3||flag==4)Speed1100_y(20);
        if(flag==5||flag==6)Speed1100_z(20);
        F[4]=flag;
    }
}

long Velocity_GetTime(long Time){
    long ll;
    long far * pi;
    char la;
    pi=MK_FP(0x40,0x6c);
    ll=*pi;
    if(ll<Velocity_Time){
        if(ll>=Velocity_Time+Time-0x1800B0){Velocity_Time=ll;return 1;}
        else return 0;
    }
    else{
        if(ll>=Velocity_Time+Time){Velocity_Time=ll;return 1;}
        else return 0;
    }
}
void F11(void){
/*  ClearKey(0);*/
    Dis_first=0;
    if(P[10])P[10]=1;
    if(F[4]){   /* 归零 */
        StopF5();
    }
    F11_flag=1;
/*  SoundTime=SoundTime*1.5;*/
    if(C[8]>1){
    G04=C[8];
    C[8]=1;
    Pause();
    cncode.Code=0xfe;
    RunCode(1);
    }
    if(Dis_flag){
    F12_flag=0;
    Dis_flag=0;
    AI=0;
        Close_Elect();
        Close_OCSF();
        if(F[2]||Pump_flag){Close_Pump();ShowFlag(2);/*F[2]=0;if(KEYL==0)ShowKey(1,2);*/}
          outb(OSC,0x14);
          outb(OSC+1,OTPS1|=0x01);     /*DIS 指示灯*/
          outb(IO+1,IO1&=0xfb);  /*极性恢复+ */
/*      #if CNC
        #else
        Delay1(0x100);
        if(Voltage()<10&&!K[3]){if(!K[8])ll=2500/DZC;else ll=50/DZC;}
        #endif
*/
        if(!C[8])ShowTable(0);

    }
        Profile_xflag=0;
        Profile_yflag=0;
    MOVEXYZ=0;
/*  if(C[8]>1){
    Signal_1100X();
    Signal_1100Y();
    Signal_1100();
    RunCodeEnd();
    }*/
    offset_rx=XPosi(0);
    offset_ry=YPosi(0);
    offset_rz=ZPosi(0);
    Position_control_x(offset_rx);
    Position_control_y(offset_ry);
    Position_control_z(offset_rz);
    lock_x=1;lock_y=1;lock_z=1;
    outb(OSC,0x14);
    outb(OSC+1,OTPS1|=0x01);     /*DIS 指示灯*/
    outb(IO+1,IO1&=0xfb);  /*极性恢复+ */
    F11_flag=1;
}
int ProTime(void){
    static long far *pi;
    static long OLDTIME,OLDST,px=0,py=0,pz=0,px1=0,py1=0,pz1=0;
    static long PROTIME;
    static int i=0,j=0;
    static int kx=0,ky=0,kz=0;
    static int T00=0;
    long ll,l1=0;
    int ii;
    if(!i){
        pi=MK_FP(0x40,0x6c);
        i=1;
        disable();
        OLDTIME=*pi;
        enable();
        if(OLDTIME>=0xE0066)T00=1;
        else T00=0;
        PROTIME=OLDTIME;
        l1=GetDate();
        ii=Cmp(OldDATE,l1);
        if(ii<0){
        WriteRAM(0,0x26,l1&0xff);
        WriteRAM(0,0x27,l1>>8);
        OldDATE=l1;
        l1^=*pi;
        OldID+=l1;
        WriteRAM(0,0x24,OldID&0xff);
        WriteRAM(0,0x25,OldID>>8);
        }if(ii>0){Testflag=1;PassWordF&=0x20;}
        CheckDate();
        return 1;
    }
    else{
    disable();
    ll=*pi;
    enable();
    if(!T00&&ll>=0xE0066)i=0;
    if(ll<OLDTIME){OLDTIME=ll;i=0;}
    if(ll>OLDTIME)l1=ll-OLDTIME;
    else l1=0;
    if(l1>120)l1=120;
    if(l1){if(ProtectTime<11000)ProtectTime+=l1;
        OLDTIME=ll;
        }
    }j+=l1;
    if(j>=1){
        j=0;
        pz=position_z-offset_z;
        py=position_y-offset_y;
        px=position_x-offset_x;
        if(px!=px1){
            px1=px;kx=1;
        }
        if(py!=py1){
            py1=py;ky=1;
        }
        if(pz!=pz1){
            pz1=pz;kz=1;
        }{if(kz);
            else{if(ExitTime>36){ExitTime=36;if(F[1])F2();}}
        }

    #if NZNC
    #else
    if(FS&&FS<0x800){
        if(FS<=l1){FS=0;outb(IO+4,IO4|=0x02);/*>4A关风扇*/}
        else FS-=l1;
    }
    #endif
    if(ExitTime){
        ShowMess(31);
        if(ExitTime<=l1){CloseAllPower();}
        else{ExitTime-=l1;}
    }
    if(SoundTime>0){
        if(!(V0ING)||K[3]){/*no 短路*/
        if(OLDST!=SoundTime){if(SoundTime<SoundTIME)l1=0;if(Sound_flag)CloseSound();}
        if(!Sound_flag&&l1)OpenSound();
        SoundTime-=l1;
        if(!SoundTime)SoundTime--;
        OLDST=SoundTime;
        }else OpenSound();
    }
    else if(SoundTime<0){SoundTime=0;CloseSound();}
    if(OLDTIME>PROTIME){PROTIME=OLDTIME;
    if(SMessage>0){SMessage-=l1;if(SMessage<=0)ShowMess(0xff);}
    if(Dis_flag){
    if(AI){if(AITime<30000)AITime+=l1;
    if(AI==1&&((AITime>AI_time*18&&P[5]&&AIHigh*DZC>50)
        ||(AIHigh*DZC>(AI_high+50)&&P[6]&&AITime>182))){
        if(P[7]==1)ClearAI(0);
        if(P[7]==2){F11();ShowTB(0xa);}
        AITime=0;
        }
    }
    if(AI==2){
/*  if(AITime<10||AIHigh<4)ClearAI(1);*/
    if(AITime>60*18){F12_flag=0;}
    /*if(AITime>300*18){F11();ShowMess(37);}*/
    }
    }
    }
    if(PROTIME>OLDTIME){PROTIME=OLDTIME;i=0;}
    return 1;}
    return 0;
}

int CkErrZero(){
    delay(0x100);
    if(Voltage()<10){
        ShowMess(50);MOVEXYZ=0x10;F5(F[4]);return 1;
    }return 0;
}
void Zero(){
    static char byte_2159=0,byte_215A=0,zerofirst=0;
    if(!F[4]){byte_2159=0;byte_215A=0;zerofirst=0;return;}
    if(!zerofirst){
        if(!PositionEndXYZ(0)){
            zerofirst=1;
            if(CkErrZero())return;
        }
    }
    if(F[4]==1||F[4]==2)lock_x=0;
    if(F[4]==3||F[4]==4)lock_y=0;
    if(F[4]==5||F[4]==6)lock_z=0;
    offset_ds=0;ZeroFlag=0;
  if(byte_2159==0){
    ShowTB(0x1b);
    Signal_1100();
    if(F[4]==1){
      if(Voltage()<10&&MOVEXYZ==F[4]){
    Velocity_control_x(0xff80);
    byte_215A=1;
    if(CkErrZero())return;
    Velocity_control_x(0x5);
/*  Delay1(180);*/
      }else Velocity_control_x(0xA);
    }
    if(F[4]==2){
      if(Voltage()<10&&MOVEXYZ==F[4]){
    Velocity_control_x(0x80);
    byte_215A=1;
    if(CkErrZero())return;
    Velocity_control_x(0xFFFd);
/*  Delay1(180);*/
      }else Velocity_control_x(0xFFF8);
    }
    if(F[4]==3){
      if(Voltage()<10&&MOVEXYZ==F[4]){
    Velocity_control_y(0xff80);
    byte_215A=1;
    if(CkErrZero())return;
    Velocity_control_y(0x5);
/*  Delay1(180);*/
      }else Velocity_control_y(0xA);
    }
    if(F[4]==4){
      if(Voltage()<10&&MOVEXYZ==F[4]){
    Velocity_control_y(0x80);
    byte_215A=1;
    if(CkErrZero())return;
    Velocity_control_y(0xFFFd);
/*  Delay1(180);*/
      }else Velocity_control_y(0xFFF8);
    }
    if(F[4]==5){
      if(Voltage()<10&&MOVEXYZ==F[4]){
    Velocity_control_z(0xff80);
    byte_215A=1;
    if(CkErrZero())return;
    Velocity_control_z(0x5);
/*  Delay1(180);*/
      }else Velocity_control_z(0xA);
    }
    if(F[4]==6){
      if(Voltage()<10&&MOVEXYZ==F[4]){
    Velocity_control_z(0x80);
    byte_215A=1;
    if(CkErrZero())return;
    Velocity_control_z(0xFFFd);
/*  Delay1(180);*/
      }else Velocity_control_z(0xFFF8);
    }

    byte_2159=1;
    MOVEXYZ=F[4];
  }else{
    if(byte_215A==0){
      if(Voltage()<10){
    if(F[4]==1){
      Velocity_control_x(0xff80);
      byte_215A=1;
      delay(0x180);
      Velocity_control_x(0x5);
    }
    if(F[4]==2){
      Velocity_control_x(0x80);
      byte_215A=1;
      delay(0x180);
      Velocity_control_x(0xFFFd);
    }
    if(F[4]==3){
      Velocity_control_y(0xff80);
      byte_215A=1;
      delay(0x180);
      Velocity_control_y(0x5);
    }
    if(F[4]==4){
      Velocity_control_y(0x80);
      byte_215A=1;
      delay(0x180);
      Velocity_control_y(0xFFFd);
    }
    if(F[4]==5){
      Velocity_control_z(0xff80);
      byte_215A=1;
      delay(0x180);
      Velocity_control_z(0x5);
    }
    if(F[4]==6){
      Velocity_control_z(0x80);
      byte_215A=1;
      delay(0x180);
      Velocity_control_z(0xFFFd);
    }

      }
    }else
    if(Voltage()<=1){
    MOVEXYZ=F[4];
      ZeroFlag= 1;
      StopF5();
      SoundTime=0x7f;
      byte_2159=0;
      byte_215A=0;
      ShowTB(0xb);
      XYPosition();
      if(F[4]==1&&F[4]==2)
    Speed1100_x(Elect_speed);
      if(F[4]==3&&F[4]==4)
    Speed1100_y(Elect_speed);
      if(F[4]==5&&F[4]==6){
      position_d=position_z;
      offset_d=position_z-offset_z;
      Speed1100_z(Elect_speed);
      }
      ZeroFlag=1;
    }
  }
}
void StopF5(void){  /* 停止归零 */
        Stop_x(0);
        Stop_y(0);
        Stop_z(0);
        F[4]=0;
        Zero();
        ShortSound();
        if(KEYL==10)ShowKey(9,0);
}

void Position_control_z(long position){
    /* Position control mode */
    unsigned char far *pi;
    long l;
    int v = 2047;
    if(K[6])return;
    position-=ZPosi(0);
    pi=(char far *)&position;
    IOZ1&=0xf0;
    outb((Address_Z),0x14);
    outb((Address_Z+1),IOZ1);
    outb(Address_Z,0x41);
    outb((Address_Z+1),*pi);
    outb(Address_Z,0x42);
    outb((Address_Z+1),*(pi+1));
    outb(Address_Z,0x43);
    outb((Address_Z+1),*(pi+2));
    outb((Address_Z),0x44);
    outb(Address_Z,0x51);
    outb(Address_Z+1,(unsigned )v&0xff);
    outb(Address_Z,0x52);
    outb(Address_Z+1,(unsigned)v>>8);
    IOZ1|=0x1;
    outb((Address_Z),0x14);
    outb((Address_Z+1),IOZ1);
}
void Velocity_control_z(int v){           /* proportional velocity control mode */
    int v1;
    if(K[6])return;
    IOZ1&=0xf0;
    v1 = abs(v);
    outb((Address_Z),0x14);
    outb((Address_Z+1),IOZ1);
    outb(Address_Z,0x51);
    outb(Address_Z+1,(unsigned )v1&0xff);
    outb(Address_Z,0x52);
    outb(Address_Z+1,(unsigned)v1>>8);
    if(v > 0)
       IOZ1 |= 0x02;
    else
       IOZ1 |= 0x03;
    outb((Address_Z),0x14);
    outb((Address_Z+1),IOZ1);
}
void Profile_z(int Acceleration,long position){
    unsigned char far *pi,a;
    position-=ZPosi(0);
    pi=(unsigned char far *)&position;
    IOZ1&=0xf0;
    outb((Address_Z),0x14);
    outb((Address_Z+1),IOZ1);
    outb(Address_Z,0x41);
    outb((Address_Z+1),*pi);
    outb(Address_Z,0x42);
    outb((Address_Z+1),*(pi+1));
    outb(Address_Z,0x43);
    outb((Address_Z+1),*(pi+2));
    outb(Address_Z,0x51);
    outb(Address_Z+1,(unsigned )Acceleration&0xff);
    outb(Address_Z,0x52);
    outb(Address_Z+1,(unsigned)Acceleration>>8);
    IOZ1|=0x01;
    outb((Address_Z),0x14);
    outb((Address_Z+1),IOZ1);
}
void Position_control_x(long position){
    unsigned char far *pi;
    int v =2047;
    if(K[6])return;
    position-=XPosi(0);
    pi=(char far *)&position;
    OTPBX1&=0xf0;
    outb((ICXaddr),0x14);
    outb((ICXaddr+1),OTPBX1);
    outb(ICXaddr,0x41);
    outb((ICXaddr+1),*pi);
    outb(ICXaddr,0x42);
    outb((ICXaddr+1),*(pi+1));
    outb(ICXaddr,0x43);
    outb((ICXaddr+1),*(pi+2));
    outb(ICXaddr,0x51);
    outb(ICXaddr+1,(unsigned )v&0xff);
    outb(ICXaddr,0x52);
    outb(ICXaddr+1,(unsigned)v>>8);
    OTPBX1|=0x1;
    outb((ICXaddr),0x14);
    outb((ICXaddr+1),OTPBX1);
}

void Velocity_control_x(int v){           /* proportional velocity control mode */
    int v1;
    if(K[6])return;
    v1 = abs(v);
    OTPBX1&=0xf0;
    outb((ICXaddr),0x14);
    outb((ICXaddr+1),OTPBX1);
    outb(ICXaddr,0x51);
    outb(ICXaddr+1,(unsigned )v1&0xff);
    outb(ICXaddr,0x52);
    outb(ICXaddr+1,(unsigned)v1>>8);
    if(v > 0)
        OTPBX1|=0x02;
    else
        OTPBX1|=0x03;
    outb((ICXaddr),0x14);
    outb((ICXaddr+1),OTPBX1);
}
void Position_control_y(long position){
    unsigned char far *pi;
    int v =2047;
    if(K[6])return;
    position-=YPosi(0);
    pi=(char far *)&position;
    OTPBY1&=0xf0;
    outb((ICYaddr),0x14);
    outb((ICYaddr+1),OTPBY1);
    outb(ICYaddr,0x41);
    outb((ICYaddr+1),*pi);
    outb(ICYaddr,0x42);
    outb((ICYaddr+1),*(pi+1));
    outb(ICYaddr,0x43);
    outb((ICYaddr+1),*(pi+2));
    outb(ICYaddr,0x51);
    outb(ICYaddr+1,(unsigned )v&0xff);
    outb(ICYaddr,0x52);
    outb(ICYaddr+1,(unsigned)v>>8);
    OTPBY1|=0x1;
    outb((ICYaddr),0x14);
    outb((ICYaddr+1),OTPBY1);
}

void Velocity_control_y(int v){           /* proportional velocity control mode*/
    int v1 = abs(v);
    if(K[6])return;
    OTPBY1&=0xf0;
    outb((ICYaddr),0x14);
    outb((ICYaddr+1),OTPBY1);
    outb(ICYaddr,0x51);
    outb(ICYaddr+1,(unsigned )v1&0xff);
    outb(ICYaddr,0x52);
    outb(ICYaddr+1,(unsigned)v1>>8);
    if(v > 0)
        OTPBY1|=0x02;
    else
        OTPBY1|=0x03;
    outb((ICYaddr),0x14);
    outb((ICYaddr+1),OTPBY1);
}
void Profile_x(int Acceleration,long position){
    unsigned char far *pi,a;
    position-=XPosi(0);
    pi=(unsigned char far *)&position;
    OTPBX1&=0xf0;
    outb((ICXaddr),0x14);
    outb((ICXaddr+1),OTPBX1);
    outb(ICXaddr,0x41);
    outb((ICXaddr+1),*pi);
    outb(ICXaddr,0x42);
    outb((ICXaddr+1),*(pi+1));
    outb(ICXaddr,0x43);
    outb((ICXaddr+1),*(pi+2));
    outb(ICXaddr,0x51);
    outb(ICXaddr+1,(unsigned)Acceleration&0xff);
    outb(ICXaddr,0x52);
    outb(ICXaddr+1,(unsigned)Acceleration>>8);
    OTPBX1|=0x01;
    outb((ICXaddr),0x14);
    outb((ICXaddr+1),OTPBX1);
}

void Profile_y(int Acceleration,long position){
    unsigned char far *pi,a;
    position-=YPosi(0);
    pi=(unsigned char far *)&position;
    OTPBY1&=0xf0;
    outb((ICYaddr),0x14);
    outb((ICYaddr+1),OTPBY1);
    outb(ICYaddr,0x41);
    outb((ICYaddr+1),*pi);
    outb(ICYaddr,0x42);
    outb((ICYaddr+1),*(pi+1));
    outb(ICYaddr,0x43);
    outb((ICYaddr+1),*(pi+2));
    outb(ICYaddr,0x51);
    outb(ICYaddr+1,(unsigned )Acceleration&0xff);
    outb(ICYaddr,0x52);
    outb(ICYaddr+1,(unsigned)Acceleration>>8);
    OTPBY1|=0x01;
    outb((ICYaddr),0x14);
    outb((ICYaddr+1),OTPBY1);
}
int KeyPress(void){
    struct REGPACK iintr;
    iintr.r_ax=0x1100;
    intr(0x16,&iintr);
    iintr.r_flags=~iintr.r_flags;
    return iintr.r_flags&0x40;
}
int KeyAss(void){
    int ak,ak1;
    ak=bioskey(0x10);
    Fx=0;
    ak1=ak&0xff;
    if(!ak1||ak1==0xe0){
        ak>>=8;
        Fx=1;
    }
    return ak&0xff;
}
void Speed1100_z(char speed)
{
    set_PIDZ(speed,0,4,0x10);
}
void Speed1100_x(char speed)
{
    set_PIDX(speed,0,4,0x10);
}
void Speed1100_y(char speed)
{
    set_PIDY(speed,0,4,0x10);
}
void Signal_1100(void){     /*发放HTCL-1100信号*/
        IOZ1&=0xf0;
        outb(Address_Z,0x14);
        outb(Address_Z+1,IOZ1);
}
void Signal_Jianxi(char flag){
        if(!flag){IOZ1&=0xf0;IOZ1|=0x06;}  /*01:向负方向运动，00：向正方向运动*/
        else {IOZ1&=0xf0;IOZ1|=0x07;}
        outb(Address_Z,0x14);
        outb(Address_Z+1,IOZ1);
}
void Signal_1100X(void){     /*发放HTCL-1100信号*/
        OTPBX1&=0xf0;
        outb(ICXaddr,0x14);
        outb(ICXaddr+1,OTPBX1);
}
void Signal_JianxiX(char flag){
        if(!flag){OTPBX1&=0xf0;OTPBX1|=0x06;}   /*01:向负方向运动，00：向正方向运动*/
        else {OTPBX1&=0xf0;OTPBX1|=0x07;}
        outb(ICXaddr,0x14);
        outb(ICXaddr+1,OTPBX1);
}
void Signal_1100Y(void){     /*发放HTCL-1100信号*/
        OTPBY1&=0xf0;
        outb(ICYaddr,0x14);
        outb(ICYaddr+1,OTPBY1);
}
void Signal_JianxiY(char flag){
        if(!flag){OTPBY1&=0xf0;OTPBY1|=0x06;}
        else {OTPBY1&=0xf0;OTPBY1|=0x07;}
        outb(ICYaddr,0x14);
        outb(ICYaddr+1,OTPBY1);
}

void ClearAI(int flag){
    unsigned char ia;
    int i,j;
    if(!flag){
    if(Table.Gaoya[Dis_lines]>0){
    IO5|=0xe0;outb(IO+5,IO5);/* 停高压 */
    IO5&=0xdf;outb(IO+5,IO5);}          /* 高压 */
    Work_time=5;
    i=Table.Maikuan[Dis_lines];j=Table.Xiuzhi[Dis_lines];
    if(i>=j/3)j+=i/2;
    else if(i>=j/2)j+=j/2;
    if(j<200)j=200;
    Duty_ratio(i,j,0);          /* 脉冲 */
    MianJi=(1); /* 面积 */
    if(Table.Jianxi[Dis_lines]<40)
    outb(OSC,0x82);
    outb(OSC+1,(Table.Jianxi[Dis_lines])+10);  /* 间隙电压 */
    AI=2;
    }
    if(flag==1){SetTable(Dis_lines);AI=0;AIHigh=0;AITime=0;
    }
}
void SetTable(char lines){
    unsigned char ia;
    if(Table.Index[lines]!=1||lines>=10)return;
    AI=0;
    JX=Table.Jixin[lines];
    if(C[8]>1){switch(DIS.XYZ){
        case 0:position_p=DIS.Position_E+offset_x;break;
        case 1:position_p=DIS.Position_E+offset_y;break;
        case 2:position_p=DIS.Position_E+offset_z;break;
        }
    }
    else
    position_p=Table.Shendu[lines];
    if(position_p<-9999999/DZC)position_p=-9999999/DZC;
    if(position_p>9999999/DZC)position_p=9999999/DZC;
    position_pz=position_p;
    SetDianliu(Table.Dianliu[lines]);   /*电流*/
    SGY=Table.Gaoya[lines];
        IO3&=0xf8;outb(IO+3,IO3);       /* 停高压 */
        if(Table.Gaoya[lines]==1)IO3|=0x04;
        if(Table.Gaoya[lines]==2)IO3|=0x02;
        if(Table.Gaoya[lines]==3)IO3|=0x01;
        outb(IO+3,IO3);         /* 高压 */
    Duty_ratio(Table.Maikuan[lines],
    Table.Xiuzhi[lines],0);     /* 脉冲 */
    OTPS0&=0xf8;
        OTPS0|=(Table.Sudu[lines]-1)&0x7;
        outb(OSC,0x13);
        outb(OSC+1,OTPS0);
    Up_time=Table.Shenggao[lines] * 1.82;   /* 升高 */
    Work_time=(Table.Gongshi[lines]*1.82);  /* 工时 */
    outb(OSC,0x82);
    outb(OSC+1,(Table.Jianxi[Dis_lines]));  /* 间隙电压 */
    MianJi=Table.Mianji[lines];       /* 面积 */
    if(Table.Jixin[lines]==1&&Dis_flag)     /* 极性 */
               outb(IO+1,IO1|=0x04);
    else
               outb(IO+1,IO1&=0xfb);
}
void JiXin(int JX1){
    if(!Dis_flag)return ;
    #if NZNC
    if(JX1==1)outb(IC2+0,inb(IC2+0)&0xef);
    else    outb(IC2+0,inb(IC2+0)|0x10);
    #else
    if(JX1==1)
        outb(IO+4,IO4&=0xb7);
    else
        outb(IO+4,IO4|=0x48);
    #endif
}
void Duty_ratio(unsigned maikuan,unsigned xiuzhi,unsigned clk0){
    if(!clk0){
        switch(maikuan){
        case 991:maikuan=1000;break;
        case 992:maikuan=1250;break;
        case 993:maikuan=1500;break;
        case 994:maikuan=1750;break;
        case 995:maikuan=2000;break;
        case 996:maikuan=2250;break;
        case 997:maikuan=2500;break;
        case 998:maikuan=2750;break;
        case 999:maikuan=3000;
    }
    maikuan*=4;
    xiuzhi*=4;
    outb(OSC,0xa1);
    outb(OSC+1,xiuzhi&0xff);
    outb(OSC,0xa2);
    outb(OSC+1,xiuzhi>>8);
    outb(OSC,0x91);
    outb(OSC+1,maikuan&0xff);
    outb(OSC,0x92);
    outb(OSC+1,maikuan>>8);
    MX=maikuan;MX/=(maikuan+xiuzhi);
    }
}
void Delay1(unsigned mill)
{
delay(mill>>3);
}
void set_PIDZ(unsigned Pid_kp,unsigned Pid_ki,unsigned Pid_kd,unsigned Pid_st)
{
    outb(Address_Z,0x17);
    outb((Address_Z+1),10);/*,输入kp*/
    outb(Address_Z,0x16);
    if(5==DZC)
    outb((Address_Z+1),64);/*输入st*/
    else
    outb((Address_Z+1),240);/*输入st*/
}
void set_PIDX(unsigned Pid_kp,unsigned Pid_ki,unsigned Pid_kd,unsigned Pid_st)
{
    outb(ICXaddr,0x17);
    outb((ICXaddr+1),10);/*,输入kp*/
    outb(ICXaddr,0x16);
    if(5==DZC)
    outb((ICXaddr+1),64);/*输入st*/
    else
    outb((ICXaddr+1),240);/*输入st*/
}
void set_PIDY(unsigned Pid_kp,unsigned Pid_ki,unsigned Pid_kd,unsigned Pid_st)
{
    outb(ICYaddr,0x17);
    outb((ICYaddr+1),10);/*,输入kp*/
    outb(ICYaddr,0x16);
    if(5==DZC)
    outb((ICYaddr+1),64);/*输入st*/
    else
    outb((ICYaddr+1),240);/*输入st*/
}

void outb(int portid,unsigned char valud){
    disable();
    outportb(portid,valud);
    enable();
    Delay1(DELAY);
}
unsigned char inb(int portid){
    unsigned char a;
    disable();
    a=inportb(portid);
    enable();
    Delay1(DELAY);
    return a;
}
void InitKey(char flag){
    struct REGPACK iintr;
    disable();
    outb(0x64,0xad);
    outb(0x60,0xff);
    outb(0x64,0xae);
    enable();
    if(flag)return;
}

GetTime(long Time){
    long ll;
    long far * pi;
    char la;
    pi=MK_FP(0x40,0x6c);
    ll=*pi;
    if(ll<Oldtime){
        if(ll>=Oldtime+Time-0x1800B0){Oldtime=ll;return 1;}
        else return 0;
    }
    else{
        if(ll>=Oldtime+Time){Oldtime=ll;return 1;}
        else return 0;
    }
}

void ShortSound(void){
    OpenSound();
    SoundTime=SoundTIME*2;
}
long ReadRegisterValue_X(char cLowReg){
    unsigned char   i0,i1,i2,i3=cLowReg;
    long    ll;
    if(cLowReg) ;
    outb(ICXaddr,0x81);
    i0=inportb(ICXaddr+1);
    outb(ICXaddr,0x82);
    i1=inportb(ICXaddr+1);
    outb(ICXaddr,0x83);
    i2=inportb(ICXaddr+1);
    if(i2&0x80)i3=0xff;
    else    i3=0;
    ll=i3;ll<<=8;ll+=i2;ll<<=8;ll+=i1;ll<<=8;ll+=i0;
    if(ll>9999999||ll<-9999999)return 0;
  return ll;

}
long ReadRegisterValue_Y(char cLowReg){
    unsigned char   i0,i1,i2,i3 = cLowReg;
    long    ll;
    if(cLowReg) ;
    outb(ICYaddr,0x81);
    i0=inportb(ICYaddr+1);
    outb(ICYaddr,0x82);
    i1=inportb(ICYaddr+1);
    outb(ICYaddr,0x83);
    i2=inportb(ICYaddr+1);
    if(i2&0x80)i3=0xff;
    else    i3=0;
    ll=i3;ll<<=8;ll+=i2;ll<<=8;ll+=i1;ll<<=8;ll+=i0;
    if(ll>9999999||ll<-9999999)return 0;
    return ll;
}
long ReadRegisterValue_Z(char cLowReg)
{
    unsigned char   i0,i1,i2,i3;
    long    ll;
    if(cLowReg) ;
    outb(ICYaddr,0x61);
    i0=inportb(ICYaddr+1);
    outb(ICYaddr,0x62);
    i1=inportb(ICYaddr+1);
    outb(ICYaddr,0x63);
    i2=inportb(ICYaddr+1);
    if(i2&0x80)i3=0xff;
    else    i3=0;
    ll=i3;ll<<=8;ll+=i2;ll<<=8;ll+=i1;ll<<=8;ll+=i0;
    if(ll>9999999||ll<-9999999)return 0;
  return ll;
}
long ReadRegisterValue_W(char cLowReg)
{
    unsigned char   i0,i1,i2,i3;
    long    ll;
    if(cLowReg) ;
    outb(ICYaddr,0x41);
    i0=inportb(ICYaddr+1);
    outb(ICYaddr,0x42);
    i1=inportb(ICYaddr+1);
    outb(ICYaddr,0x43);
    i2=inportb(ICYaddr+1);
    if(i2&0x80)i3=0xff;
    else    i3=0;
    ll=i3;ll<<=8;ll+=i2;ll<<=8;ll+=i1;ll<<=8;ll+=i0;
    if(ll>9999999||ll<-9999999)return 0;
  return ll;
}
long ReadRegisterValue_V(char cLowReg)
{
    unsigned char   i0,i1,i2,i3;
    long    ll;
    if(cLowReg) ;
    outb(ICYaddr,0x71);
    i0=inportb(ICYaddr+1);
    outb(ICYaddr,0x72);
    i1=inportb(ICYaddr+1);
    outb(ICYaddr,0x73);
    i2=inportb(ICYaddr+1);
    if(i2&0x80)i3=0xff;
    else    i3=0;
    ll=i3;ll<<=8;ll+=i2;ll<<=8;ll+=i1;ll<<=8;ll+=i0;
    if(ll>9999999||ll<-9999999)return 0;
  return ll;
}
int PositionEndXYZ(unsigned char flags){
    long l;
    register long l1=0;
    int i=0;
    unsigned char flag;
    flag=flags&0x7f;
    if(flag==1||!flag){
        l=XPosi(0)-l1;
        if(flags<0x80){if(l<-5/DZC||l>5/DZC)i++;}
        else    if(l<-1||l>1)i++;
    }
    if(flag==2||!flag){
        l1=YPosi(1);
        l=YPosi(0)-l1;
        if(flags<0x80){if(l<-5/DZC||l>5/DZC)i++;}
        else    if(l<-1||l>1)i++;
    }
    if(flag==3||!flag){
        l1=ZPosi(1);
        l=ZPosi(0)-l1;
        if(flags<0x80){if(l<-5/DZC||l>5/DZC)i++;}
        else    if(l<-1||l>1)i++;
    }
    if(4==flag)
      {
         l=XPosi(0);
         if(l!=Gposition_x)
           i++;
         l=YPosi(0);
         if(l!=Gposition_y)
           i++;
         l=ZPosi(0);
         if(l!=Gposition_z)
           i++;
       }
       return i;
}
int PositionEnd(void){
    long l;
    if(!Position_flag){
        l=ZPosi(0);
        l-=ZPosi(1);
        Position_flag=0;
        l=labs(l);
    }
    if(l<5/DZC)return 1;
    else return 0;
}
void F6(void){
    long l;
/*  ClearKey(0);*/
    F[5]=1;
    if(XYZ[0]){
        offset_x=position_x/2-XPosi(0);
        XYZ[0]=0;
    }
    if(XYZ[1]){
        offset_y=position_y/2-YPosi(0);
        XYZ[1]=0;
    }
    if(XYZ[2]&&!Dis_flag){
        l=position_z/2-ZPosi(0);
        offset_d-=offset_z-l;
        offset_ds=0;
        position_d-=offset_z-l;
        offset_z=l;
        XYZ[2]=0;
    }
    SaveXYZ(0);
    ClearKey(0);
    F[5]=0;
}
void F7(char flag){
    int i,j;
    if(!flag)ClearKey(0);
    if(!F[6]){
    XX=0;YY=0;ShowMess(XX+1);
    ClearF7();F[6]=1;
    DelLine();
    ShowTable(YY+1);
    /*ShowKey(0x70,0);*/}
    else{if(!flag)F[6]=0;groupindex[Group]=1;
    WriteRAM(0,Group+0x80,1);
    DelLine();
    if(C[8]<2){i=F12_flag;j=Dis_lines;if(!Dis_flag)i=0;F12_flag=0;SetTable(j);F12_flag=i;Dis_lines=j;}
    else{i=Dis_flag;j=Dis_lines;/*Dis_flag=0;*/SetTable(j);/*Diss();*/Dis_flag=i;Dis_lines=j;}
    ShowTable(YY+1);Write_Table(Group);
    if(!flag){/*ShowKey(2,0);*/ShowMess(0);}}
/*  if(!flag)ShowKey(1,0);*/
}
void F8(char flag){
if(!flag){
    if(F[7]){F[7]=0;F8H=0;F8T=0;ShowHT();ShowMess(0);}
    else {ClearKey(0);ShowMess(16);F[7]=1;MaxL=2;MaxN=99;MinN=1;ShowHT();}
}
else{
if(F[7]==1){
    if(KeyN!=0x7fffffff)F8H=KeyN;else{F8(0);return;}
    ShowMess(0);
    ShowMess(17);MaxL=2;MaxN=99;MinN=1;
    F[7]=2;
    ShowHT();
}else
if(F[7]==2){
    if(KeyN!=0x7fffffff)F8T=KeyN;else{F8(0);return;}
    F[7]=3;
    ShowHT();
}
if(F[7]==3)ClearKey(0);
}
}
void F9(void){
    if(!F[8]){
    if(Dis_flag||K[3]){ShowMess(15);return;}
    ClearKey(0);ShowMess(18);MaxL=2;MaxN=99;MinN=1;F[8]=1;}
    else {
        if(KeyN!=0x7fffffff&&KeyN!=Group){Group=(char)KeyN;
        Dis_start=Dis_end=0;Read_Table(Group);ShowTable(0);
        SetTable(0);WriteRAM(0,0x20,Group);}
    F[8]=0;ClearKey(0);}
}
void CloseAllPower(void){   /*关闭所有电源*/
    if(F[1]){F2();delay(10000);}
        IO1|=0x02;
        outb(IO+1,IO1);
    Delay1(10000);
}
void K10(void){
ClearKey(0);
    if(!K[9]){if(Dis_flag||F[4]){ShowMess(15);return;}
    if(F[1]){F2();}ExitTime=36;K[9]=1;}
    else{K[9]=0;ExitTime=0;
        IO1&=0xfd;
        outb(IO+1,IO1); /*OpenAllPower*/
    }
}
void K2(void){
ClearKey(0);
if(K[1]){K[1]=0;}
else{K[1]=1;}
}
void K3(void){
K_3=0;
if(K[2]==2){ClearKey(0);K[2]=0;/*ShowSS();*/return;}
if(K[2]==1){K[2]=2;
    if(KeyN!=0x7fffffff)K_3=KeyN;
    if(!K_3){K[2]=0;}ClearKey(0);
}
else{ClearKey(0);ShowMess(19);K[2]=1;MaxL=2;MaxN=99;MinN=1;}
/*ShowSS();*/
}
void K4(void){
static zu;
int i;
ClearKey(0);
if(Dis_flag){ShowMess(15);return;}
MOVEXYZ=0;
if(K[3]){K[3]=0;KeyN=zu;F[8]=1;F9();}
else{zu=Group;KeyN=0;Read_Table(0);
    if(Table.Index[0]!=1){
    Table.Dianliu[0]=StrTable.Dianliu[4];
    Table.Maikuan[0]=StrTable.Maikuan[4];
    Table.Xiuzhi[0]=StrTable.Xiuzhi[4];
    Table.Jianxi[0]=StrTable.Jianxi[4];
    Table.Sudu[0]=StrTable.Sudu[4];
    Table.Shenggao[0]=StrTable.Shenggao[4];
    Table.Gongshi[0]=StrTable.Gongshi[4];
    Table.Mianji[0]=StrTable.Mianji[4];
    Table.Jixin[0]=StrTable.Jixin[4];
    Table.Gaoya[0]=StrTable.Gaoya[4];
    }
    for(i=1;i<10;i++)Table.Index[i]=0;
    Table.Index[0]=StrTable.Index[4];
    if(!K[8])
    Table.Shendu[0]=StrTable.Shendu[4];
    else
    Table.Shendu[0]=-StrTable.Shendu[4];
    Write_Table(0);
    F[8]=1;F9();K[3]=1;
}
}
char OilSwitch(){
 /*   outb(OSC,0x11);
    return(inb(OSC+1)&0x40);    */
}
void K5(void){
ClearKey(0);
if(K[4])K[4]=0;
else K[4]=1;
ShowTB(0x18);
}
void K6(void){
if(K[5]){
if(KeyN!=0x7FFFFFFF)Elect_speed=KeyN;
Speed1100_x(Elect_speed);
Speed1100_y(Elect_speed);
Speed1100_z(Elect_speed);
K[5]=0;
ClearKey(0);
}
#if DZCC
else{ClearKey(0);ShowMess(20);K[5]=1;MaxL=2;MinN=5;MaxN=50;}
#else
else{ClearKey(0);ShowMess(20);K[5]=1;MaxL=2;MinN=10;MaxN=99;}
#endif
/*ShowSpeed();*/
}
void K7(void){
ClearKey(0);
    if(K[6])
      {
         K[6]=0;
         IOZ0|=0x20;
         outb(Address_Z,0x01);
         outb(Address_Z+1,IOZ0);           /*ENZ*/
         if(Dis_flag)F12_flag=1;
      }
    else
        {
           K[6]=1;
         IOZ0&=0xdf;
         outb(Address_Z,0x01);
             outb(Address_Z+1,IOZ0);           /*DISZ*/
           if(Dis_flag)
              F12_flag=1;
      }
ShowFlag(4);
}
void K8(void){
ClearKey(0);
if(K[7]){K[7]=0;}
else{K[7]=1;}
}
void K9(void){
ClearKey(0);
if(Dis_flag){ShowMess(15);return;}
if(K[2])K3();
if(K[8])K[8]=0;
else K[8]=1;
if(K[3]){   if(!K[8])
    Table.Shendu[0]=StrTable.Shendu[4];
    else
    Table.Shendu[0]=-StrTable.Shendu[4];
    ShowTable(1);SetTable(0);
}
offset_ds=0;
ShowFlag(3);
}
void K10b(void){
    ClearKey(0);
    if(Dis_flag){ShowMess(15);return;}
    position_d=position_z/*1999999*/;
if(!K[9]){K[9]=1;
        offset_xx[1]=offset_x;
        offset_yy[1]=offset_y;
        offset_zz[1]=offset_z;
        XYZGroup=2;
        WriteRAM(0,0x22,XYZGroup);
        offset_x=offset_xx[2];
        offset_y=offset_yy[2];
        offset_z=offset_zz[2];
        offset_ds=0;
}
else{K[9]=0;
    offset_xx[2]=offset_x;
    offset_yy[2]=offset_y;
    offset_zz[2]=offset_z;
    XYZGroup=1;
    WriteRAM(0,0x22,XYZGroup);
    offset_x=offset_xx[1];
    offset_y=offset_yy[1];
    offset_z=offset_zz[1];
}
}

void F10(void){

if(!F[9]){ClearKey(0);if(Dis_flag){ShowMess(15);return;}
ShowMess(26);F[9]=1;MaxL=1;MaxN=9;
MinN=4;
}
else    {F[9]=0;
    if(KeyN!=0x7fffffff){
        offset_xx[XYZGroup]=offset_x;
        offset_yy[XYZGroup]=offset_y;
        offset_zz[XYZGroup]=offset_z;
        XYZGroup=(char)KeyN;
        WriteRAM(0,0x22,XYZGroup);
        offset_x=offset_xx[XYZGroup];
        offset_y=offset_yy[XYZGroup];
        offset_z=offset_zz[XYZGroup];
        XYPosition();
        position_d=position_z/*1999999*/;
    }else   ShowMess(0);
    ClearKey(0);
}
ShowFlag(6);
}
void K11(){
exitp(1);
}
#if TESTING
void K12(){
SaveBMP();
}
void SaveBMP(){
/*  FILE *fp;
    int i,j;
    unsigned k,k0;
unsigned char BMP0[]={
"\x42\x4D\xFE\x13\x00\x00\x00\x00\x00\x00\x76\x00\x00\x00\x28\x00"
"\x00\x00\xC8\x00\x00\x00\x32\x00\x00\x00\x01\x00\x04\x00\x00\x00"
"\x00\x00\x88\x13\x00\x00\x12\x0B\x00\x00\x12\x0B\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x80\x00\x00\x80"
"\x00\x00\x00\x80\x80\x00\x80\x00\x00\x00\x80\x00\x80\x00\x80\x80"
"\x00\x00\x80\x80\x80\x00\xC0\xC0\xC0\x00\x00\x00\xFF\x00\x00\xFF"
"\x00\x00\x00\xFF\xFF\x00\xFF\x00\x00\x00\xFF\x00\xFF\x00\xFF\xFF"
"\x00\x00\xFF\xFF\xFF\x00\xFF"};
    fp=fopen("HUAFENG.BMP","wb+");
    for(i=0;i<0x76;i++)BMP[i]=BMP0[i];
    BMP[2]=0x76;BMP[3]=0x58;BMP[4]=0x2;
    BMP[0x12]=640&0xff;BMP[0x13]=0x2;BMP[0x16]=480&0xff;BMP[0x17]=0x1;
    fwrite(BMP,1,0x76,fp);
    for(i=480;i;i--){
        for(j=0;j<640;j+=2){
            k0=getpixel(j,i-1);
            k0&=0xf;
            if(k0==7)k0=8;
            else if(k0==8)k0=7;
            k=k0<<4;
            k0=getpixel(j+1,i-1);
            k0&=0xf;
            if(k0==7)k0=8;
            else if(k0==8)k0=7;
            k|=k0;
            BMP[j/2]=k;
        }fwrite(BMP,1,320,fp);
    }
    fclose(fp);
*/
}
#endif
void exitp(char flag){
  if(F[1])F2();
  IO0=0x00;
  outportb(IO,IO0);
    /*WriteRAM(0x1,0,0);*/
    if(flag){
        InitKey(0);
        closegraph();
        exit(0);
    }
}
