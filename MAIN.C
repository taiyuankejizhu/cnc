#include "znc.h"
#include <dos.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
extern struct Dis_position{
    char XYZ;
    char Position_S;
    long Position_H;
    long Position_E;
    long Position_V;
}DIS;
void ClearAI(int flag);
void F11(void); /*停止放电*/
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
