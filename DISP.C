#include "znc.h"
#include <mem.h>
#include <dos.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <graphics.h>

#define OSC 0x246

void ShowWin();
void CheckTab();
void movmem1();
void ShowTB();
void Dispebar(int x,int y,int x1,int y1,int color,int bkcolor,int stx,char *str);
void Dispcbar(int x,int y,int x1,int y1,int color,int bkcolor,int stx,char *str);
void Dispbar3d();
void ShowDP1(char flag);
void getpix(int hb,int lb,char *buf);
void Disp(void)
{

   ShowTime(0);
   ShowTimeD(0);
   ShowFlag(0);
}
void ShowWin()
{
setcolor(0xb);
rectangle(X0,Y0,X1,Y1);
setfillstyle(1,BKCOLORA);
bar(AX0+1,AY0+1,AX1,AY1);
rectangle(AX0,AY0,AX1+1,AY1+1);
setfillstyle(1,BKCOLORB);
bar(BX0+1,BY0+1,BX1,BY1);
rectangle(BX0,BY0,BX1+1,BY1+1);
setfillstyle(1,BKCOLORC);
bar(CX0+1,CY0+1,CX1,CY1);
rectangle(EX0,EY0,EX1+1,EY1+1);
setfillstyle(1,BKCOLORE);
bar(EX0+1,EY0+1,EX1,EY1);
setfillstyle(1,BKCOLORG);
bar(GX0+1,GY0+1,GX1,GY1);
rectangle(GX0,GY0,GX1+1,GY1+1);
}
void ShowHT(){
    char str[5]="\0\0\0\0";
    int color,bkcolor,i;
    Dispbar3d(EX0+4,EY0+4+47*5,EX1-4,EY0+4+47*5+40,BKCOLORE1|COLORE1<<4,0,0x119,1,"跳   高");
    Dispebar(EX0+4+5,EY0+4+47*5+21,EX0+4+5+8,EY0+4+47*5+21+16,COLORE1,BKCOLORE1,1,"H");
    Dispebar((EX0+EX1)/2+1+5,EY0+4+47*5+21,(EX0+EX1)/2+1+5+8,EY0+4+47*5+21+16,COLORE1,BKCOLORE1,1,"T");
    if(F[7]==1){color=15-COLORE2;bkcolor=15-BKCOLORE2;}
    else{color=COLORE2;bkcolor=BKCOLORE2;}
    str[0]=F8H/10+'0';
    str[1]=F8H%10+'0';
    i=0;if(str[0]=='0')i++;
    Dispebar(EX0+4+5+8,EY0+4+47*5+21,(EX0+EX1)/2-1-9,EY0+4+47*5+21+16,color,bkcolor,1,str+i);

    if(F[7]==2){color=15-COLORE2;bkcolor=15-BKCOLORE2;}
    else{color=COLORE2;bkcolor=BKCOLORE2;}
    str[0]=F8T/10+'0';
    str[1]=F8T%10+'0';
    i=0;if(str[0]=='0')i++;
    Dispebar((EX0+EX1)/2+1+5+8,EY0+4+47*5+21,EX1-4-9,EY0+4+47*5+21+16,color,bkcolor,1,str+i);
}
void ShowFlag(char flags){
    int color,bkcolor,i;
    unsigned j;
    char flag=0,str2[10];
    char str[3][5]={" OFF"," ON ","G5\0\0"};
    char str1[3][5]={"1 ","10 ","100"};
    bkcolor=12;
    if(!flags)
      {
        Dispbar3d(FLAG_POSX0+4,FLAG_POSY0+6+30*3,FLAG_POSX1-2,FLAG_POSY0+6+30*3+26,BKCOLORE1|COLORE1<<4,0,0,1," 电 压 ");
      }
    if(flags==1||!flags)
      {
        if(F[1])flag=1;else flag=0;
        if(!flags)
        Dispbar3d(FLAG_POSX0+4,FLAG_POSY0+6+30*4,FLAG_POSX1-2,FLAG_POSY0+6+30*4+26,BKCOLORE1|COLORE1<<4,0,0,1," 电 源 ");
        Dispebar(FLAG_POSX1-44,FLAG_POSY0+6+30*4+2,FLAG_POSX1-6,FLAG_POSY0+6+30*4+24,COLORE2,BKCOLORE2,1,str[flag]);
      }
    if(flags==2||!flags)
      {
        flag=Pump_flag;
        if(!flags)
        Dispbar3d(FLAG_POSX0+4,FLAG_POSY0+6+30*5,FLAG_POSX1-2,FLAG_POSY0+6+30*5+26,BKCOLORE1|COLORE1<<4,0,0,1," 油 泵 ");
        Dispebar(FLAG_POSX1-44,FLAG_POSY0+6+30*5+2,FLAG_POSX1-6,FLAG_POSY0+6+30*5+24,COLORE2,BKCOLORE2,1,str[flag]);
      }
    if(flags==3||!flags)
      {
        if(K[8])flag=1;else flag=0;
        if(!flags)
        Dispbar3d(FLAG_POSX0+4,FLAG_POSY0+6+30*6,FLAG_POSX1-2,FLAG_POSY0+6+30*6+26,BKCOLORE1|COLORE1<<4,0,0,1,"反加工");
        Dispebar(FLAG_POSX1-44,FLAG_POSY0+6+30*6+2,FLAG_POSX1-6,FLAG_POSY0+6+30*6+24,COLORE2,BKCOLORE2,1,str[flag]);
      }
    if(flags==4||!flags)
      {
        if(K[6])flag=1;else flag=0;
        if(!flags)
        Dispbar3d(FLAG_POSX0+4,FLAG_POSY0+6+30*7,FLAG_POSX1-2,FLAG_POSY0+6+30*7+26,BKCOLORE1|COLORE1<<4,0,0,1," 锁 定 ");
        Dispebar(FLAG_POSX1-44,FLAG_POSY0+6+30*7+2,FLAG_POSX1-6,FLAG_POSY0+6+30*7+24,COLORE2,BKCOLORE2,1,str[flag]);
      }
    if(flags==5||!flags)
      {
        if(K[8])flag=1;else flag=0;
        if(!flags)
        Dispbar3d(FLAG_POSX0+4,FLAG_POSY0+6+30*8,FLAG_POSX1-2,FLAG_POSY0+6+30*8+26,BKCOLORE1|COLORE1<<4,0,0,1," 速 度 ");
        Dispebar(FLAG_POSX1-44,FLAG_POSY0+6+30*8+2,FLAG_POSX1-6,FLAG_POSY0+6+30*8+24,COLORE2,BKCOLORE2,1,str1[SWF_flag]);
      }
    if(flags==6||!flags)
      {
        if(F[9]==1){color=15-COLORE2;bkcolor=15-BKCOLORE2;}
        else{color=COLORE2;bkcolor=BKCOLORE2;}
        str[2][2]=XYZGroup%10+'0';
        str[2][3]=0;
        if(!flags)
        Dispbar3d(FLAG_POSX0+4,FLAG_POSY0+6+30*9,FLAG_POSX1-2,FLAG_POSY0+6+30*9+26,BKCOLORE1|COLORE1<<4,0,0,1," 坐 标 ");
        if(XYZGroup>=4)i=0;else i=2;
        Dispebar(FLAG_POSX1-44,FLAG_POSY0+6+30*9+2,FLAG_POSX1-6,FLAG_POSY0+6+30*9+24,color,bkcolor,1,str[2]+i);
      }
    if(flags==7||!flags){
        strcpy(str2,"积炭T000");
        j=AI_time&0x7fff;
        str2[5]+=j/100;
        str2[6]+=(j%100)/10;
        str2[7]+=j%10;
        Listbar3d(FLAG_POSX0+4,FLAG_POSY0+6+30*10,FLAG_POSX1-2,FLAG_POSY0+6+30*10+26,0x2a0f1c00,P[7]&&AI_time,0,1,str2);
    }
    if(flags==8||flags==0){
        strcpy(str2,"积炭H000");
        j=AI_high&0x7fff;
        str2[5]+=j/100;
        str2[6]+=(j%100)/10;
        str2[7]+=j%10;
        Listbar3d(FLAG_POSX0+4,FLAG_POSY0+6+30*11,FLAG_POSX1-2,FLAG_POSY0+6+30*11+26,0x2a0f1c00,P[7]&&AI_high,0,1,str2);
    }

    if(flags==9||flags==0){
        if(!P[7])
        Listbar3d(FLAG_POSX0+4,FLAG_POSY0+6+30*12,FLAG_POSX1-2,FLAG_POSY0+6+30*12+26,0x2a0f1c00,0,0,1,"积炭关闭");
        else{
        if(P[7]==1)
        Listbar3d(FLAG_POSX0+4,FLAG_POSY0+6+30*12,FLAG_POSX1-2,FLAG_POSY0+6+30*12+26,0x2a0f1c00,1,0,1,"积炭清除");
        if(P[7]==2)
        Listbar3d(FLAG_POSX0+4,FLAG_POSY0+6+30*12,FLAG_POSX1-2,FLAG_POSY0+6+30*12+26,0x2a0f1c00,1,0,1,"积炭停机");
        }
    }
    if(flags==10||flags==0){
        Listbar3d(FLAG_POSX0+4,FLAG_POSY0+6+30*13,FLAG_POSX1-2,FLAG_POSY0+6+30*13+26,0x2a0f1c00,P[3],0,1,"深孔加工");
      }
    if(flags==11||flags==0){
        strcpy(str2,"钢蚀钢00");
        str2[6]+=Steel_flag/10;
        str2[7]+=Steel_flag%10;
        Listbar3d(FLAG_POSX0+4,FLAG_POSY0+6+30*14,FLAG_POSX1-2,FLAG_POSY0+6+30*14+26,0x2a0f1c00,P[8]&&Steel_flag,0,1,str2);
      }
}
void ShowSWF(){
char str[3]={"\xf7\xfb\xfd"};
OTPS1|=0x0e;OTPS1&=str[SWF_flag];
outb(OSC,0x14);
outb(OSC+1,OTPS1);
ShowFlag(5);
}
void ShowKey(int kn,int ke){
    int i,j,k,l;
    int ii=0;
    char str[10];
    static int kn1,ke1;
    if(kn==0xff){
        ShowKey(kn1,ke1);
    }else{
        kn1=kn;ke1=ke;
    }
    if(!kn){
    for(j=0;j<8;j++){
        setcolor(COLORG);
        setfillstyle(1,BKCOLORG);
        rectangle(GX0+1+80*j,GY0+1,GX0+80*j+78,GY0+14);
       /* Disp8x16(GX0+80*(j+1)-8,GY0+7,2,COLORG,KeyStr(0,j));*/
    }
    }ii=0x1;if(kn==0)kn=1;k=0;
        if(!ke){j=1;i=8;}
        else{j=ke;i=ke;}
    for(l=kn<<8;j<=i;j++){
        l&=0xff00;l|=j;k=0;
        switch(l){
            case 0x101:k=F[1];break;
            case 0x102:k=F[2];break;
            case 0x107:k=K[8];break;
            case 0x201:k=F[4];break;
            case 0x202:k=F[8];break;
            case 0x203:k=F[9];break;
            case 0x204:k=F[5];break;
            case 0x205:k=K[3];break;
            case 0x302:k=F[8];break;
            case 0x303:k=F[9];break;
            case 0x304:k=K[5];break;
            case 0x305:k=F[7];break;
            case 0x306:k=P[8];break;
            case 0x402:k=P[4];break;
            case 0x403:k=K[4];break;
            case 0x404:k=P[3];break;
            case 0x405:k=P[2];break;
            case 0x406:k=K[6];break;
            case 0x501:k=F[3];break;
            case 0x502:k=K[1];break;
            case 0x503:k=K[2];break;
            case 0x504:k=K[9];break;
            case 0x505:k=P[1];break;
            case 0x506:k=P[9];break;
            case 0x604:k=P[10];break;
            case 0x801:k=P[5];break;
            case 0x802:k=P[6];break;
            case 0x803:k=P[7];break;
            case 0x901:if(F[4]==1)k=1;break;
            case 0x902:if(F[4]==2)k=2;;break;
            case 0x903:if(F[4]==3)k=3;;break;
            case 0x904:if(F[4]==4)k=4;;break;
            case 0x905:if(F[4]==5)k=5;;break;
            case 0x906:if(F[4]==6)k=6;;break;
            case 0xC02:k=C[0];break;
        }
        if(l!=0x108&&l!=0xc08){
        /*Dispbar3d(GX0+3+80*(j-1),GY0+20,X0-3+80*(j-1)+80,GY1-4,ii|0<<4|BKCOLORG<<8,k,1,2,KeyStr(kn,j-1));*/
        }
        else{
        Dispbar3d(GX0+3+80*(j-1),GY0+20,X0-3+80*(j-1)+80,GY1-4,ii|0<<4|BKCOLORG<<8,k,1,2,"");
           }
    }
}
void ShowTable(int lines){
 if(lines);
}
void ShowTimeD(int flag){
    char xyzs[11]=" 00:00:00";
    unsigned long ll;
    unsigned i;
    static long positiontt=0x8fffffff,positiont=0x8fffffff;
    if(flag==0x80){
        ll=position_tt/60;
        ll%=(long)60*24*1000;
        i=ll/(60*24);
        xyzs[0]='0'+i/100;i%=100;xyzs[1]+=i/10;i%=10;xyzs[2]+=i;
        ll%=60*24;
        i=ll/60;xyzs[4]+=i/10;i%=10;xyzs[5]+=i;
        i=ll%60;xyzs[7]+=i/10;i%=10;xyzs[8]+=i;
    }else{
        ll=position_t;
        ll%=(long)3600*1000;
        i=ll/3600;
        xyzs[0]=' ';i%=100;xyzs[1]+=i/10;i%=10;xyzs[2]+=i;
        ll%=3600;
        i=ll/60;xyzs[4]+=i/10;i%=10;xyzs[5]+=i;
        i=ll%60;xyzs[7]+=i/10;i%=10;xyzs[8]+=i;
    }
    if(positiontt!=position_tt||positiont!=position_t)
       {
         positiontt=position_tt;
         positiont=position_t;
         if(!flag)
           {
              Dispbar3d(FLAG_POSX0+2,FLAG_POSY0+2+47*1,FLAG_POSX1-2,FLAG_POSY0+2+47*1+40,BKCOLORE1|COLORE1<<4,0,0x114,1,"放电时间");
           }
         Dispebar(FLAG_POSX0+2+18,FLAG_POSY0+2+47*1+21,FLAG_POSX1-2-18,FLAG_POSY0+2+47*1+21+16,COLORE2,BKCOLORE2,1,xyzs);
       }
}
void ShowTime(char flag){
    static long stime=0;
    long ll;
    int i;
    char xyzs[11]=" 00:00:00";
    ll=GetSTime();
    if(stime!=ll){
        stime=ll;
        i=ll>>24;i&=0xff;xyzs[1]+=i/0x10;xyzs[2]+=i%0x10;
        i=ll>>16;i&=0xff;xyzs[4]+=i/0x10;xyzs[5]+=i%0x10;
        i=ll>>8;i&=0xff;xyzs[7]+=i/0x10;xyzs[8]+=i%0x10;
        if(!flag){
          Dispbar3d(FLAG_POSX0+2,FLAG_POSY0+2+47*0,FLAG_POSX1-2,FLAG_POSY0+2+47*0+40,BKCOLORE1|COLORE1<<4,0,0x114,1,"当前时间");
        }
        Dispebar(FLAG_POSX0+2+18,FLAG_POSY0+2+47*0+21,FLAG_POSX1-2-18,FLAG_POSY0+2+47*0+21+16,COLORE2,BKCOLORE2,1,xyzs);
    }
}
void ShowXYZ(){
    setcolor(0x1f);
    line(FLAG_POSX0,FLAG_POSY0,FLAG_POSX0,FLAG_POSY1);
    line(FLAG_POSX1,FLAG_POSY0,FLAG_POSX1,FLAG_POSY1);
    line(FLAG_POSX0,FLAG_POSY0,FLAG_POSX1,FLAG_POSY0);
    line(FLAG_POSX0,FLAG_POSY1,FLAG_POSX1,FLAG_POSY1);
    setfillstyle(1,7);
    bar(FLAG_POSX0+2,FLAG_POSY0+2,FLAG_POSX1-2,FLAG_POSY1-2);
}
void ShowVer(char *ver){
 if(ver);
}
void ShowMess(int value){
if(value);
}
void windows(int left,int top,int right,int bottom,int newattr){
    window(left,top,right,bottom);
    textattr(newattr);
    clrscr();
}

unsigned char ReadRAM(unsigned char page,unsigned int number){
    unsigned char far *pi;
    unsigned char a;
    int i;
    if(oldpage!=page){
        inb(0x280);
        pi=MK_FP(0xd000,0x888);
        *pi=page;
        Delay1(20);
        oldpage=page;
    }
    pi=MK_FP(0xd100,number);
    a=*pi;
    if(Argc)for(i=0;i<0x111;i++);
    return a;
}
void WriteRAM(unsigned char page,unsigned int number,unsigned char value){
    char far *pi;
    int i;
    if(oldpage!=page){
        inb(0x280);
        pi=MK_FP(0xd000,0x888);
        *pi=page;
        Delay1(20);
        oldpage=page;
    }
    pi=MK_FP(0xd100,number);
    *pi=value;
    if(Argc)for(i=0;i<0x111;i++);
}
void CheckTab(){
    int i;
    for(i=0;i<10;i++){if(Table.Index[i]!=1)continue;
    if(Table.Shendu[i]<StrTable.Shendu[0])Table.Shendu[i]=StrTable.Shendu[0];
    if(Table.Shendu[i]>StrTable.Shendu[1])Table.Shendu[i]=StrTable.Shendu[1];
    if(Table.Dianliu[i]<StrTable.Dianliu[0])Table.Dianliu[i]=StrTable.Dianliu[0];
    if(Table.Dianliu[i]>StrTable.Dianliu[1])Table.Dianliu[i]=StrTable.Dianliu[1];
    if(Table.Maikuan[i]<StrTable.Maikuan[0])Table.Maikuan[i]=StrTable.Maikuan[0];
    if(Table.Maikuan[i]>StrTable.Maikuan[1])Table.Maikuan[i]=StrTable.Maikuan[1];
    if(Table.Xiuzhi[i]<StrTable.Xiuzhi[0])Table.Xiuzhi[i]=StrTable.Xiuzhi[0];
    if(Table.Xiuzhi[i]>StrTable.Xiuzhi[1])Table.Xiuzhi[i]=StrTable.Xiuzhi[1];
    if(!Table.Jianxi[i]==0)
    if(Table.Jianxi[i]<StrTable.Jianxi[0])Table.Jianxi[i]=StrTable.Jianxi[0];
    if(Table.Jianxi[i]>StrTable.Jianxi[1])Table.Jianxi[i]=StrTable.Jianxi[1];
    if(Table.Sudu[i]<StrTable.Sudu[0])Table.Sudu[i]=StrTable.Sudu[0];
    if(Table.Sudu[i]>StrTable.Sudu[1])Table.Sudu[i]=StrTable.Sudu[1];
    if(Table.Shenggao[i]<StrTable.Shenggao[0])Table.Shenggao[i]=StrTable.Shenggao[0];
    if(Table.Shenggao[i]>StrTable.Shenggao[1])Table.Shenggao[i]=StrTable.Shenggao[1];
    if(Table.Gongshi[i]<StrTable.Gongshi[0])Table.Gongshi[i]=StrTable.Gongshi[0];
    if(Table.Gongshi[i]>StrTable.Gongshi[1])Table.Gongshi[i]=StrTable.Gongshi[1];
    if(Table.Mianji[i]<StrTable.Mianji[0])Table.Mianji[i]=StrTable.Mianji[0];
    if(Table.Mianji[i]>StrTable.Mianji[1])Table.Mianji[i]=StrTable.Mianji[1];
    if(Table.Jixin[i]<StrTable.Jixin[0])Table.Jixin[i]=StrTable.Jixin[0];
    if(Table.Jixin[i]>StrTable.Jixin[1])Table.Jixin[i]=StrTable.Jixin[1];
    if(Table.Gaoya[i]<StrTable.Gaoya[0])Table.Gaoya[i]=StrTable.Gaoya[0];
    if(Table.Gaoya[i]>StrTable.Gaoya[1])Table.Gaoya[i]=StrTable.Gaoya[1];
    if(Table.B_OFS[i]<StrTable.B_OFS[0])Table.B_OFS[i]=StrTable.B_OFS[0];
    if(Table.B_OFS[i]>StrTable.B_OFS[1])Table.B_OFS[i]=StrTable.B_OFS[1];
    if(Table.S_OFS[i]<StrTable.S_OFS[0])Table.S_OFS[i]=StrTable.S_OFS[0];
    if(Table.S_OFS[i]>StrTable.S_OFS[1])Table.S_OFS[i]=StrTable.S_OFS[1];
    if(Table.LNS[i]<StrTable.LNS[0])Table.LNS[i]=StrTable.LNS[0];
    if(Table.LNS[i]>StrTable.LNS[1])Table.LNS[i]=StrTable.LNS[1];
    if(Table.STEP[i]<StrTable.STEP[0])Table.STEP[i]=StrTable.STEP[0];
    if(Table.STEP[i]>StrTable.STEP[1])Table.STEP[i]=StrTable.STEP[1];
    if(Table.LMN[i]<StrTable.LMN[0])Table.LMN[i]=StrTable.LMN[0];
    if(Table.LMN[i]>StrTable.LMN[1])Table.LMN[i]=StrTable.LMN[1];
    if(Table.RY[i]<StrTable.RY[0])Table.RY[i]=StrTable.RY[0];
    if(Table.RY[i]>StrTable.RY[1])Table.RY[i]=StrTable.RY[1];
    }
}
void Read_Table(unsigned char group){ /*把group所指定组别的数据从SRAM传递给全局变量Table*/
    char    ia;
    char far *pi;
    int i,j;
    i=group+12;
    ia=i/0x10;
    j=(i%0x10)*0x100;
    pi=(char far * )&Table;
    for(i=0;i<sizeof(Table);i++){
    if(groupindex[group])
        *pi++=ReadRAM(ia,j++);
    else    *pi++=0;
    }
    for(i=0;i<10;i++){
        if(!Table.Index[i]){
            Table.Shendu[i]=StrTable.Shendu[2];
            Table.Dianliu[i]=StrTable.Dianliu[2];
            Table.Maikuan[i]=StrTable.Maikuan[2];
            Table.Xiuzhi[i]=StrTable.Xiuzhi[2];
            Table.Jianxi[i]=StrTable.Jianxi[2];
            Table.Sudu[i]=StrTable.Sudu[2];
            Table.Shenggao[i]=StrTable.Shenggao[2];
            Table.Gongshi[i]=StrTable.Gongshi[2];
            Table.Mianji[i]=StrTable.Mianji[2];
            Table.Jixin[i]=StrTable.Jixin[2];
            Table.Gaoya[i]=StrTable.Gaoya[2];
            Table.B_OFS[i]=StrTable.B_OFS[2];
            Table.S_OFS[i]=StrTable.S_OFS[2];
            Table.LNS[i]=StrTable.LNS[2];
            Table.STEP[i]=StrTable.STEP[2];
            Table.LMN[i]=StrTable.LMN[2];
            Table.RY[i]=StrTable.RY[2];
        }
    }CheckTab();
}
void movmem1(char *src,char *dest,unsigned length)
{
    unsigned i,j;
    for(i=0;i<length;i++){
    *(dest+i)=*(src+i);
    for(j=0;j<0x111;j++);
    }
}
void Write_Table(unsigned char group){ /*把group所指定组别的数据从SRAM传递给全局变量Table*/
    char    ia;
    char far *pi,far *pi1;
    register int i;
    int j;
    i=group+12;
    ia=i/0x10;
    j=(i%0x10)*0x100;
    pi=(char far * )&Table;
    groupindex[group]=1;
    WriteRAM(0,Group+0x80,1);
    pi1=MK_FP(0xd100,0);
    WriteRAM(ia,j,*pi);
    if(!Argc)movmem(pi,pi1+j,sizeof(Table));
    else    movmem1(pi,pi1+j,sizeof(Table));
/*  for(i=0;i<0xAA;i++)
        WriteRAM(ia,j++,*pi++);*/
}
long Read_RAM_XYZ(int number){
    unsigned long pi;
    pi=ReadRAM(0,number+3);
    pi<<=8;
    pi+=ReadRAM(0,number+2);
    pi<<=8;
    pi+=ReadRAM(0,number+1);
    pi<<=8;
    pi+=ReadRAM(0,number+0);
    return pi;
}
void Write_RAM_XYZ(int number,long value){
    WriteRAM(0,number++,value);
    value>>=8;
    WriteRAM(0,number++,value);
    value>>=8;
    WriteRAM(0,number++,value);
    value>>=8;
    WriteRAM(0,number++,value);
}
void ResetXYZ(){
    unsigned char far *pi;
    long far * pl;
    int i,j=0,k,a4=0x25,a5=0x70;
    register long ll;
    unsigned long l1;
    struct REGPACK iintr;
    iintr.r_ax=0x400;
    intr(0x1a,&iintr);
    k=iintr.r_cx;
    pi=CopyRight;
    for(i=0;i<12;i++)
        if((*pi++)!=(ReadRAM(0,i)^0xFF))j=1;
    pi=CopyRight;
    if(j&&k>8210){
        for(i=0;i<100;i++)groupindex[i]=0;
/*      for(i=0;i<12;i++)WriteRAM(0,i,(*pi++)^0xff);*/
        Write_RAM_XYZ(0x10,(long)0);
        Write_RAM_XYZ(0x14,(long)0);
        Write_RAM_XYZ(0x18,(long)0);
        Write_RAM_XYZ(0x1c,(long)0);
        WriteRAM(0,0x20,1);
        WriteRAM(0,0x22,1);
        for(i=0;i<30;i++)
            Write_RAM_XYZ(0x100+i*4,(long)0);
    }
    ll=Read_RAM_XYZ(0x10);
    if(ll<-9999999/DZC)ll=-9999999/DZC;
    if(ll>9999999/DZC)ll=9999999/DZC;
    offset_rx=XPosi(0);
    offset_x=ll-offset_rx;
    ll=Read_RAM_XYZ(0x14);
    if(ll<-9999999/DZC)ll=-9999999/DZC;
    if(ll>9999999/DZC)ll=9999999/DZC;
    offset_ry=YPosi(0);
    offset_y=ll-offset_ry;
    ll=Read_RAM_XYZ(0x18);
    if(ll<-9999999/DZC)ll=-9999999/DZC;
    if(ll>9999999/DZC)ll=9999999/DZC;
    offset_rz=ZPosi(0);
    offset_z=ll-offset_rz;
    position_t=Read_RAM_XYZ(0x1c);
    position_tt=Read_RAM_XYZ(0x78);
    for(i=0;i<10;i++){
        ll=Read_RAM_XYZ(0x100+i*12);
        if(ll<-9999999/DZC)ll=-9999999/DZC;
        if(ll>9999999/DZC)ll=9999999/DZC;
        offset_xx[i]=ll-XPosi(0);
        ll=Read_RAM_XYZ(0x104+i*12);
        if(ll<-9999999/DZC)ll=-9999999/DZC;
        if(ll>9999999/DZC)ll=9999999/DZC;
        offset_yy[i]=ll-YPosi(0);
        ll=Read_RAM_XYZ(0x108+i*12);
        if(ll<-9999999/DZC)ll=-99999999/DZC;
        if(ll>9999999/DZC)ll=9999999/DZC;
        offset_zz[i]=ll-ZPosi(0);
    }
    Group=ReadRAM(0,0x20);
    ll=Read_RAM_XYZ(0x24);
    OldID=ll;
    ll>>=16;
    OldDATE=ll;
    if(j>12)
    Testflag=j;
    XYZGroup=ReadRAM(0,0x22);
    #if CNC
    if(XYZGroup<4||XYZGroup>9)XYZGroup=4;
    #else
    if(XYZGroup<1||XYZGroup>9)XYZGroup=1;
    #endif
    if(Group<1||Group>99)Group=1;
    for(i=0;i<100;i++)groupindex[i]=ReadRAM(0,i+0x80);
    XYPosition();
    position_d=position_z;
    position_dx=position_x;
    position_dy=position_y;
    position_dz=position_z;
    position_px=position_x;
    position_py=position_y;
    iintr.r_ax=0x201;
    iintr.r_bx=FP_OFF(buf);
    iintr.r_cx=2;
    iintr.r_dx=0x80;
    iintr.r_es=FP_SEG(buf);
    intr(0x13,&iintr);
    l1=buf[0x35];l1<<=8;
    l1+=buf[0x34];l1<<=8;
    l1+=buf[0x33];l1<<=8;
    l1+=buf[0x32];disable();
    if(l1!=IDS)Testflag++;
    outportb(a5,0x80);
    outportb(a5++,a4--);
    l1=inportb(a5--)&0xff;l1<<=8;
    outportb(a5++,a4--);
    l1+=inportb(a5--)&0xff;l1<<=8;
    outportb(a5++,a4--);
    l1+=inportb(a5--)&0xff;l1<<=8;
    outportb(a5++,a4--);
    l1+=inportb(a5--)&0xff;;
    enable();
    if(l1!=IDS||(k>8218&&j))Testflag++;
    ReadRAM(0,0);
    i=0x10;
    l1=ReadRAM(0,--i);l1<<=8;
    l1+=ReadRAM(0,--i);l1<<=8;
    l1+=ReadRAM(0,--i);l1<<=8;
    l1+=ReadRAM(0,--i);
    if(l1!=IDS||(k>8220&&j))Testflag++;
}
void SaveXYZ(char flag){
    int i;
    long l;
    register long ol;
    static long positionx=0x7fffffff,positiony=0x7fffffff,positionz=0x7fffffff,positiont=0x7fffffff;
    XYPosition();
    if(!flag||positionx!=position_x){
        ol=position_x-offset_x;
        l=labs(positionx-position_x);
        Write_RAM_XYZ(0x10,position_x);
        for(i=0;i<10;i++)Write_RAM_XYZ(0x100+i*12,offset_xx[i]+ol);
        positionx=position_x;
        if(lock_x&&l>=5/DZC){
        /*   Position_control_x(offset_rx); */
        }
        offset_ds=0;
    }
    if(!flag||positiony!=position_y){
        ol=position_y-offset_y;
        l=labs(positiony-position_y);
        Write_RAM_XYZ(0x14,position_y);
        for(i=0;i<10;i++)Write_RAM_XYZ(0x104+i*12,offset_yy[i]+ol);
        positiony=position_y;
        if(lock_y&&l>=5/DZC){
        /*  Position_control_y(offset_ry); */
        }
        offset_ds=0;
    }
    if(!flag||positionz!=position_z){
        ol=position_z-offset_z;
        l=labs(positionz-position_z);
        Write_RAM_XYZ(0x18,position_z);
        for(i=0;i<10;i++)Write_RAM_XYZ(0x108+i*12,offset_zz[i]+ol);
        positionz=position_z;
        if(lock_z&&l>=5/DZC){
           /* Position_control_z(offset_rz);*/
        }
        if(Dis_flag&&C[8]<1){
        if(positionz<=position_d&&!K[8]){
            if(position_d<=position_d)position_d=position_z;
        }
        if(positionz>=position_d&&K[8]){
            if(position_d>=position_d)position_d=position_z;
        }
        }
    }
    if(positiont!=position_t){
        Write_RAM_XYZ(0x1c,position_t);
        positiont=position_t;
        Write_RAM_XYZ(0x78,position_tt);
    }
}
long XPosi(char flag){  /*读取X轴计数器的数据*/
    unsigned char   i0,i1,i2,i3;
    long    ll;
    outb(ICXaddr,0x21);
    i0=inportb(ICXaddr+1);
    outb(ICXaddr,0x22);
    i1=inportb(ICXaddr+1);
    outb(ICXaddr,0x23);
    i2=inportb(ICXaddr+1);
    if(i2&0x80)i3=0xff;
    else    i3=0;
    ll=i3;ll<<=8;ll+=i2;ll<<=8;ll+=i1;ll<<=8;ll+=i0;
    if(ll>9999999||ll<-9999999)return 0;
    if(flag)return ll;
    else return ll;
}
long YPosi(char flag){  /*读取Y轴计数器的数据*/
    unsigned char   i0,i1,i2,i3;
    long    ll;
    outb(ICYaddr,0x21);
    i0=inportb(ICYaddr+1);
    outb(ICYaddr,0x22);
    i1=inportb(ICYaddr+1);
    outb(ICYaddr,0x23);
    i2=inportb(ICYaddr+1);
    if(i2&0x80)i3=0xff;
    else    i3=0;
    ll=i3;ll<<=8;ll+=i2;ll<<=8;ll+=i1;ll<<=8;ll+=i0;
    if(ll>9999999||ll<-9999999)return 0;
    if(flag)return ll;
    else return ll;
}
long ZPosi(char flag)
{
    unsigned char   i0,i1,i2,i3;
    long    ll;
    outb(Address_Z,0x21);
    i0=inportb(Address_Z+1);
    outb(Address_Z,0x22);
    i1=inportb(Address_Z+1);
    outb(Address_Z,0x23);
    i2=inportb(Address_Z+1);
    if(i2&0x80)i3=0xff;
    else    i3=0;
    ll=i3;ll<<=8;ll+=i2;ll<<=8;ll+=i1;ll<<=8;ll+=i0;
    if(ll>9999999||ll<-9999999)return 0;
    if(flag)return ll;
    else return ll;
}
long Read_Velocity_x(char flag)
{
    unsigned char   i0,i1,i2,i3;
    long    ll;
    outb(ICXaddr,0x31);
    i0=inportb(ICXaddr+1);
    outb(ICXaddr,0x32);
    i1=inportb(ICXaddr+1);
    if(i1&0x80){i3=0xff;i2=0xff;}
    else    {i3=0;i2=0;}
    ll=i3;ll<<=8;ll+=i2;ll<<=8;ll+=i1;ll<<=8;ll+=i0;
    if(ll>9999999||ll<-9999999)return 0;
    if(flag)return ll;
    else return ll;
}
long Read_Velocity_y(char flag)
{
    unsigned char   i0,i1,i2,i3;
    long    ll;
    outb(ICYaddr,0x31);
    i0=inportb(ICYaddr+1);
    outb(ICYaddr,0x32);
    i1=inportb(ICYaddr+1);
    if(i1&0x80){i3=0xff;i2=0xff;}
    else    {i3=0;i2=0;}
    ll=i3;ll<<=8;ll+=i2;ll<<=8;ll+=i1;ll<<=8;ll+=i0;
    if(ll>9999999||ll<-9999999)return 0;
    if(flag)return ll;
    else return ll;
}
long Read_Velocity_z(char flag)
{
    unsigned char   i0,i1,i2,i3;
    long    ll;
    outb(Address_Z,0x31);
    i0=inportb(Address_Z+1);
    outb(Address_Z,0x32);
    i1=inportb(Address_Z+1);
    if(i1&0x80){i3=0xff;i2=0xff;}
    else    {i3=0;i2=0;}
    ll=i3;ll<<=8;ll+=i2;ll<<=8;ll+=i1;ll<<=8;ll+=i0;
    if(ll>9999999||ll<-9999999)return 0;
    if(flag)return ll;
    else return ll;
}
long XPosi_origin(char flag)
{
    unsigned char   i0,i1,i2,i3;
    long    ll;
    outb(ICXaddr,0x25);
    i0=inportb(ICXaddr+1);
    outb(ICXaddr,0x26);
    i1=inportb(ICXaddr+1);
    outb(ICXaddr,0x27);
    i2=inportb(ICXaddr+1);
    if(i2&0x80)i3=0xff;
    else    i3=0;
    ll=i3;ll<<=8;ll+=i2;ll<<=8;ll+=i1;ll<<=8;ll+=i0;
    if(ll>9999999||ll<-9999999)return 0;
    if(flag)return ll;
    else return ll;
}
long YPosi_origin(char flag)
{
    unsigned char   i0,i1,i2,i3;
    long    ll;
    outb(ICYaddr,0x25);
    i0=inportb(ICYaddr+1);
    outb(ICYaddr,0x26);
    i1=inportb(ICYaddr+1);
    outb(ICYaddr,0x27);
    i2=inportb(ICYaddr+1);
    if(i2&0x80)i3=0xff;
    else    i3=0;
    ll=i3;ll<<=8;ll+=i2;ll<<=8;ll+=i1;ll<<=8;ll+=i0;
    if(ll>9999999||ll<-9999999)return 0;
    if(flag)return ll;
    else return ll;
}
long ZPosi_origin(char flag)
{
    unsigned char   i0,i1,i2,i3;
    long    ll;
    outb(Address_Z,0x25);
    i0=inportb(Address_Z+1);
    outb(Address_Z,0x26);
    i1=inportb(Address_Z+1);
    outb(Address_Z,0x27);
    i2=inportb(Address_Z+1);
    if(i2&0x80)i3=0xff;
    else    i3=0;
    ll=i3;ll<<=8;ll+=i2;ll<<=8;ll+=i1;ll<<=8;ll+=i0;
    if(ll>9999999||ll<-9999999)return 0;
    if(flag)return ll;
    else return ll;
}
void XYPosition(){  /*设置X、Y轴的位置*/
    unsigned long far *pi;
    unsigned long lt;
/*  long ox,oy,oz;*/
    register int i;
    ox=XPosi(0);
    oy=YPosi(0);
    oz=ZPosi(0);
    position_x=offset_x+ox;
    position_y=offset_y+oy;
    position_z=offset_z+oz;
    if(C[8]>1||F12_flag>=1&&F12_flag<10){
        pi=MK_FP(0x40,0x6c);
        lt=(*pi)/18.2;
        if(offset_t<lt-2)return;
        if(offset_t>lt)offset_t=lt;
        if(offset_t<lt){position_t+=lt-offset_t;
                position_tt+=lt-offset_t;offset_t=lt;
        }
    }
}
unsigned char Current(void){    /*读取电流值*/
    int a,i,b;
    if(!OCSF)return 0;
    /*outb(OSC,0x02);   */
    a=0xff;
    switch(SGY){
    case 0:i=80;break;
    case 1:i=120;break;
    case 2:i=160;break;
    case 3:i=200;break;
    }b=i*MX;b+=20;
    if(a>=b)return 0;
    i=(b-a)*SDL/b;
    outb(ICXaddr,0x12);
    i=inb(ICXaddr+1);
    return i;
}
unsigned char Voltage(void){    /*读取电压值*/
    unsigned char a;
    outb(OSC,0x81);
    a=inb(OSC+1);   
    return a;
}

long GetInput(void){
    long l;
    long i;
    if(!MaxL){
        switch(Inputstr[0]){
            case ' ':KeyN=0x7fffffff;break;
            case '+':KeyN=0;break;
            case '-':KeyN=1;break;
        }
        return KeyN;
    }
    l=atoi(Inputstr+1);
    if(MaxL>3){l*=1000;
        i=atoi(Inputstr1+1);
        if(!Inputstr1[1])i=0;
        else if(!Inputstr1[2])i*=100;
        else if(!Inputstr1[3])i*=10;
        l+=i;
    }
    if(Inputstr[0]=='-')l=-l;
    KeyN=0x7fffffff;
    if(!Inputstr[1])l=KeyN;
    Inputstr[1]=0;
    Inputstr1[1]=0;
    if(l>=MinN&&l<=MaxN)KeyN=l;
    return l;
}
void ShowInput(char value){
    int i;
    int j;
    static int k=0;
    char a[]="\0\0\0\0";
    if(MaxL==-1)return;
    if(MaxL>3)j=MaxL-3;
    else j=MaxL;
    a[0]=value;
    if(!MaxL&&Inputstr[0]!='-')Inputstr[0]='+';
    switch(value){
        case 'G':
        case 'g':
          if(C[1]&&CXX==1){Inputstr[0]='G';Inputstr[1]=0;}break;
        case 'M':
        case 'm':
          if(C[1]&&CXX==1){Inputstr[0]='M';Inputstr[1]=0;}break;
        case '=':
        case '+':
            Inputstr[0]='+';break;
        case '-':
            Inputstr[0]='-';break;
        case 0:
            Inputstr[0]=' ';Inputstr[1]=0;Inputstr1[1]=0;k=0;MaxL=-1;KeyN=0x7fffffff;
            if(F[6]!=3)
            Dispebar(CX1-84,CY0+1,CX1-8,CY0+17,7,8,0+8,Inputstr);
            else Dispebar(F7X+150,F7Y+5+18,F7X+290,F7Y+5+18+17,0xc,3,1,"");
            return;
        case '.':
            if(!k&&MaxL>3){
            a[0]='0';
            if(strlen(Inputstr)==1){strcat(Inputstr,a);}
            k=1;}break;
        default:if(value<'0'||value>'9'){
            if(!k){
                i=strlen(Inputstr);
                if(i>=2)Inputstr[--i]=0;
            }else{
                i=strlen(Inputstr1);
                if(i>1)Inputstr1[i-1]=0;
                if(i==1)k=0;
            }
            break;
              }
        if(!k){strcat(Inputstr,a);
            if(strlen(Inputstr+1)>j)
            strcpy(Inputstr+1,Inputstr+2);}
        else{
            strcat(Inputstr1,a);
            if(strlen(Inputstr1)>4)
            strcpy(Inputstr1+1,Inputstr1+2);}
    }
    if(F[6]!=3){
        Dispebar(CX1-84,CY0+1,CX1-44+8,CY0+17,7,8,0+8,Inputstr);
        if(k)Dispcbar(CX1-43,CY0+1,CX1-8,CY0+17,7,8,0,Inputstr1);
    }else{
        Dispebar(F7X+150,F7Y+5+18,F7X+250+8,F7Y+5+18+17,0xc,3,1+8,Inputstr);
        if(k)Dispcbar(F7X+250,F7Y+5+18,F7X+290,F7Y+5+18+17,0xc,3,1,Inputstr1);
    }
}
void ShowTB(char value){
if(value);
   /* int i;
    char *TBstr[0x10]={"    ","Z+ ","Z- ","Y+ ","Y- ","X+ ","X- ","火警","油位","短路","积炭","归零","limit!"};
    setfillstyle(1,BKCOLORC);
    i=value&0xf;
    if(!i){bar(CX0+1,CY0+19,CX1-78,CY0+35);return;}
    if(value>0xf)value=0;
    if(i>6)
    Dispcbar(CX0+20+(i-7)*38+203,CY0+19,CX0+20+(i-7)*38+203+34,CY0+35,COLORC,BKCOLORC,1,TBstr[value]);
    else{
    Dispcbar(CX0+20+(i-1)*25,CY0+19,CX0+20+(i-1)*25+24,CY0+35,COLORC,BKCOLORC,1,TBstr[value]);
    if(value)value=0xc;
    Dispcbar(CX0+20+150,CY0+19,CX0+20+203,CY0+35,COLORC,BKCOLORC,1,TBstr[value]);
    }   */
}
void Ck8255(void){
    static int fires=0,fires1=4,fires2=4,fires3=5,fires4=0;
    unsigned char a,b;
    static int fires5=4,fires6=4,fires7=4,fires8=4;
    return;
    outb(OSC,0x11);
    a=inb(OSC+1);
    outb(Address_Z,0x11);
    b=inb(Address_Z+1);
    if(Dis_flag&&!(a&0x80)&&P[4]) 
    {   /*火警*/
    if(fires>5){
      SoundTime=SDST;
      Ck8255_flag|=0x80;
      FireFlag=1;
      ShowTB(0x7);
      if(Dis_flag){F11();
          Delay1(0x1000);
          return;
        }
      }else fires++;
    }else{
     if(Ck8255_flag&0x80){Ck8255_flag&=0x7f;}
     fires=0;
    }
    if(b&0x08)
        {   /*上限*/
     if(fires1>5){
      if(UP_K){
      Signal_1100();
      }
      SoundTime=TBST;
      if(Dis_flag){F11();SoundTime=SDST;}
      if(F[4]==5)F5(5);
      Ck8255_flag|=0x10;
      ShowTB(0x1);
      fires1=0;
     }else fires1++;
    }else{
     if(Ck8255_flag&0x10){ShowTB(0x11);Ck8255_flag&=0xef;}
     fires1=4;
    }
    if(b&0x04)
    {   /*下限*/
     if(fires2>5){
      if(DN_K){
      Signal_1100();
      }SoundTime=TBST;
      if(Dis_flag){F11();SoundTime=SDST;}
      if(F[4]==6)F5(6);
      Ck8255_flag|=0x20;
      ShowTB(0x2);
      fires2=0;
     }else fires2++;
    }else{
     if(Ck8255_flag&0x20){ShowTB(0x12);Ck8255_flag&=0xdf;}
     fires2=4;
    }
    outb(ICYaddr,0x11);
    b=inb(ICYaddr+1);
    if(b&0x80)
    {   /*Y+限*/
      if(fires5>5){
      if(YUP_K){
      Signal_1100();
      }SoundTime=TBST;
      if(Dis_flag){F11();SoundTime=SDST;}
      if(F[4]==3)F5(3);
      Ck8255_flag|=0x4;
      ShowTB(0x3);
      fires5=0;
     }else fires5++;
    }else{
     if(Ck8255_flag&0x4){ShowTB(0x13);Ck8255_flag&=0xfb;}
     fires5=4;
    }
    if(b&0x40)
    {       /*Y-限*/
     if(fires6>5){
      if(YDN_K){
      Signal_1100();
      }SoundTime=TBST;
          if(Dis_flag){F11();SoundTime=SDST;}
      if(F[4]==4)F5(4);
      Ck8255_flag|=8;
      ShowTB(0x4);
      fires6=0;
     }else fires6++;
    }else{
     if(Ck8255_flag&8){ShowTB(0x14);Ck8255_flag&=0xf7;}
     fires6=4;
    }
    outb(ICXaddr,0x11);
    b=inb(ICXaddr+1);
    if(b&0x80)
        {   /*X+限*/
     if(fires7>5){
      if(XUP_K){
      Signal_1100();
      }SoundTime=TBST;
      if(Dis_flag){F11();SoundTime=SDST;}
      if(F[4]==1)F5(1);
      Ck8255_flag|=0x01;
      ShowTB(0x5);
      fires7=0;
     }else fires7++;
    }else{
     if(Ck8255_flag&0x01){ShowTB(0x15);Ck8255_flag&=0xfe;}
     fires7=4;
    }
    if(b&0x40)
    {       /*X-限*/
     if(fires8>5){
      if(XDN_K){
      Signal_1100();
      }SoundTime=TBST;
          if(Dis_flag){F11();SoundTime=SDST;}
      if(F[4]==2)F5(2);
      Ck8255_flag|=0x02;
      ShowTB(0x6);
      if(Dis_flag)F11();
      fires8=0;
     }else fires8++;
    }else{
     if(Ck8255_flag&0x02){ShowTB(0x16);Ck8255_flag&=0xfd;}
     fires8=4;
    }
    if((!F[4]&&!Dis_flag)&&Voltage()<=6&&!ZeroFlag){/*短路*/
     if(fires3>=5){
      #if   CNC
      if((XDN_K&&MOVEXYZ!=1||XUP_K&&MOVEXYZ!=2)&&!K[3]&&SWF_K<10){Stop_x(0);}
      if((YDN_K&&MOVEXYZ!=3||YUP_K&&MOVEXYZ!=4)&&!K[3]&&SWF_K<10){Stop_y(0);}
      if(( DN_K&&MOVEXYZ!=5|| UP_K&&MOVEXYZ!=6)&&!K[3]&&SWF_K<10){Stop_z(0);}
      if(!K[3]){SoundTime|=0x4000;}
      #else
      if(!K[3]){
          if(DN_K&&!K[8]||UP_K&&K[8]){
          V=0;
          Stop_z(0);
          DN_K=0;UP_K=0;
          }
         SoundTime|=0x4000;
      }
      #endif
      V0ING=1;
      ShowTB(0x9);
     }else fires3++;
    }else{
     if(V0ING){if(SoundTime>=0x4000){SoundTime&=0x3fff;}else SoundTime=0;
     if(!SoundTime){CloseSound();}
     ShowTB(0x19);V0ING=0;
#if CNC
     if(!XDN_K&&!XUP_K&&!YDN_K&&!YUP_K&&!DN_K&&!UP_K)MOVEXYZ=0;
#endif
     }
     fires3=5;
    }
    if(Dis_flag&&K[4]&&OilSwitch()){
        if(fires4>5){
        ShowTB(0x8);Ck8255_flag|=0x40;
        }else fires4++;
    }else{
          if(Ck8255_flag&0x40){ShowTB(0x18);Ck8255_flag&=0xbf;}
    }
}

void Open_Elect(void){/*放电，开大变压器*/
        IO1|=0x20;
        outb(IO+1,IO1);
        if(Table.Dianliu[Dis_lines]>7)outb(IO+1,IO1|=0x40);/*>4A开风扇*/
    FS=0x1000;
}
void Close_Elect(void){
        IO1&=0xdf;
        outb(IO+1,IO1);
    FS=1092;  /*关风扇*/ 
}
void Open_OCSF(void){
    int i;
    OCSF=1;
    OTPS1|=0x80;
    outb(OSC,0x14);
    outb(OSC+1,OTPS1);
}
void Close_OCSF(void){
    int i;
    OCSF=0;
    OTPS1&=0x7f;
    outb(OSC,0x14);
    outb(OSC+1,OTPS1);
}

void Dispcbar(int x,int y,int x1,int y1,int color,int bkcolor,int stx,char *str){
    setfillstyle(1,bkcolor);
    bar(x,y,x1,y1);
    #if ZK
    List16x16F(x+stx,(y+y1)/2,color,str);
    #else
    List16x16M(x+stx,(y+y1)/2,color,str);
    #endif
}
void Dispebar(int x,int y,int x1,int y1,int color,int bkcolor,int stx,char *str){
    setfillstyle(1,bkcolor);
    bar(x,y,x1,y1);
    Disp8x16(x1-stx,(y+y1)/2,2,color,str);
}

void Dispbar3d(int x,int y,int x1,int y1,int color,int flag,int stx,int w,char *str){
    int colors;
    int i,j=0;
    colors=color&0xf;
    if(str[0]==0){setfillstyle(1,color>>8);bar(x,y,x1,y1);return;}
    if(stx>=0x100){j=stx>>8;stx&=0xff;}
    j<<=1;
    j++;
    if(flag){
        setcolor(0x8);
        for(i=0;i<w;i++){line(x,y+i,x1-i,y+i);line(x+i,y,x+i,y1-i);}
        setcolor(0xf);
        for(i=0;i<w;i++){line(x+i,y1-i,x1,y1-i);line(x1-i,y+i,x1-i,y1);}
        setfillstyle(1,colors);
        bar(x+w,y+w,x1-w,y1-w);
        #if ZK
        List16x16F(x+3+stx,(y*j+y1)/(1+j),0xf,str);
        #else
        List16x16M(x+3+stx,(y*j+y1)/(1+j),0xf,str);
        #endif
    }
    else{
        setcolor(0xf);
        for(i=0;i<w;i++){line(x,y+i,x1-i,y+i);line(x+i,y,x+i,y1-i);}
        setcolor(0x8);
        for(i=0;i<w;i++){line(x+i,y1-i,x1,y1-i);line(x1-i,y+i,x1-i,y1);}
        setfillstyle(1,0x7);
        bar(x+w,y+w,x1-w,y1-w);
        #if ZK
        List16x16F(x+3+stx,(y*j+y1)/(1+j),(color>>4)&0xf,str);
        #else
        List16x16M(x+3+stx,(y*j+y1)/(1+j),(color>>4)&0xf,str);
        #endif
    }
}
char DispF7(int flag){
if(flag);
}
void DispF8(int flag){
if(flag);
}
