#include "znc.h"
#include <dos.h>
#include <conio.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#if CNC
#define JD  5
char LoranFlag=0;
void ProfileXY(int Acceleration,long x1,long y1);
void Profile_xy(int Acceleration,long x0,long y0,long x,long y);
void Loran_01(char flag);
void Loran_02(char flag);
void Loran_03(char flag);
void Loran_04(char flag);
void Loran_05(char flag);
void Loran_06(char flag);
void Loran_07(char flag);
void Loran_01(char flag){   /*Ò¡¡ð*/
    double d1;
    static double d0;
    static long x,y,x0,y0,x2,y2;
    static int speed;
    static lbl;
    static char Maxv;
    static long x1,y1;
    unsigned char a,b,c;
    char test[10];
    if(!flag){Profile_xflag=0;Profile_yflag=0;
    x0=XPosi(1);y0=YPosi(1);Maxv=cncode.Z;
    lbl=0;x=labs(cncode.X);y=labs(cncode.Y);x1=0;y1=0;
    speed=cncode.R;d0=x*y;
    Runline_stx=1;
    Runline_sty=1;
    Profile_xflag=0;
    Profile_yflag=0;
    TiggerXY();
    }else{
     outb(ICXaddr,0x12);
     a=inb(ICXaddr+1);
     outb(ICYaddr,0x12);
     b=inb(ICYaddr+1);
     switch(lbl){
      case 0:
       if((!(a&0x80)||!(a&0x40))&&Runline_stx)
         {
           Profile_x(speed,x);
         }
       if((!(b&0x80)||!(b&0x40))&&Runline_sty)
         {
           Profile_y(speed,0);
         }
       if(!Runline_stx&&!Runline_sty)
         {lbl++;Profile_xflag=0;Profile_yflag=0;x2=x;y2=0;}
      break;
      case 1:
       if((!(a&0x80)||!(a&0x40))&&(!(b&0x80)||!(b&0x40))){
        if(y2>x2&&!Runline_stx&&!Runline_sty)
          {lbl++;Loran_01(1);}
        else if(!Runline_stx&&!Runline_sty)
          {
            x2-=5;x1=-5;if(x2>x){x2=x;x1=0;}
            d1=x2*x2;
            if(d0>d1){y1=sqrt(d0-d1)-y2;if(y1<0)y1=0;y2=sqrt(d0-d1);}else {y1=0;y2=0;}
            Runline_stx=1;
            Runline_sty=1;
            Profile_yflag=0;
            Profile_xflag=0;
          }
        if(Runline_stx||Runline_sty)
          ProfileXY(speed,x1,y1);
       }
      break;
      case 2:
       if((!(a&0x80)||!(a&0x40))&&(!(b&0x80)||!(b&0x40))){
        if(y2>=y&&!Runline_stx&&!Runline_sty)
          {lbl++;Loran_01(1);}
        else if(!Runline_stx&&!Runline_sty)
          {
            y2+=5;y1=5;if(y2>y){y2=y;y1=0;}
            d1=y2*y2;
            if(d0>d1){x1=sqrt(d0-d1)-x2;if(x1>0)x1=0;x2=sqrt(d0-d1);}else {x1=0;x2=0;}
            Runline_stx=1;
            Runline_sty=1;
            Profile_xflag=0;
            Profile_yflag=0;
          }
        if(Runline_stx||Runline_sty)
          ProfileXY(speed,x1,y1);
       }
      break;
      case 3:
        if((!(a&0x80)||!(a&0x40))&&(!(b&0x80)||!(b&0x40))){
        if(x2>y2&&!Runline_stx&&!Runline_sty)
          {lbl++;Loran_01(1);}
        else if(!Runline_stx&&!Runline_sty)
          {
            y2-=5;y1=-5;if(y2<0){y2=0;y1=0;}
            d1=y2*y2;
            if(d0>d1){x1=x2-sqrt(d0-d1);if(x1>0)x1=0;x2=sqrt(d0-d1);}else {x1=0;x2=0;}
            Runline_stx=1;
            Runline_sty=1;
            Profile_xflag=0;
            Profile_yflag=0;
          }
        if(Runline_stx||Runline_sty)
          ProfileXY(speed,x1,y1);
       }
      break;
      case 4:
        if((!(a&0x80)||!(a&0x40))&&(!(b&0x80)||!(b&0x40))){
        if(x2>=x&&!Runline_stx&&!Runline_sty)
          {lbl++;Loran_01(1);}
        else if(!Runline_stx&&!Runline_sty)
          {
            x2+=5;x1=-5;if(x2>x){x2=x;x1=0;}
            d1=x2*x2;
            if(d0>d1){y1=sqrt(d0-d1)-y2;if(y1>0)y1=0;y2=sqrt(d0-d1);}else {y1=0;y2=0;}
            Runline_stx=1;
            Runline_sty=1;
            Profile_xflag=0;
            Profile_yflag=0;
          }
        if(Runline_stx||Runline_sty)
          ProfileXY(speed,x1,y1);
       }
      break;
      case 5:
        if((!(a&0x80)||!(a&0x40))&&(!(b&0x80)||!(b&0x40))){
        if(y2>x2&&!Runline_stx&&!Runline_sty)
          {lbl++;Loran_01(1);}
        else if(!Runline_stx&&!Runline_sty)
          {
            x2-=5;x1=5;if(x2<0){x1=0;x2=0;}
            d1=x2*x2;
            if(d0>d1){y1=y2-sqrt(d0-d1);if(y1>0)y1=0;y2=sqrt(d0-d1);}else {y1=0;y2=0;}
            Runline_stx=1;
            Runline_sty=1;
            Profile_xflag=0;
            Profile_yflag=0;
          }
        if(Runline_stx||Runline_sty)
          ProfileXY(speed,x1,y1);
       }
      break;
      case 6:
        if((!(a&0x80)||!(a&0x40))&&(!(b&0x80)||!(b&0x40))){
        if(y2>=y&&!Runline_stx&&!Runline_sty)
          {lbl++;Loran_01(1);}
        else if(!Runline_stx&&!Runline_sty)
          {
            y2+=5;y1=-5;if(y2>y){y2=y;y1=0;}
            d1=y2*y2;
            if(d0>d1){x1=x2-sqrt(d0-d1);if(x1<0)x1=0;x2=sqrt(d0-d1);}else {x1=0;x2=0;}
            Runline_stx=1;
            Runline_sty=1;
            Profile_xflag=0;
            Profile_yflag=0;
          }
        if(Runline_stx||Runline_sty)
          ProfileXY(speed,x1,y1);
       }
      break;
      case 7:
        if((!(a&0x80)||!(a&0x40))&&(!(b&0x80)||!(b&0x40))){
        if(x2>y2&&!Runline_stx&&!Runline_sty)
          {lbl++;Loran_01(1);}
        else if(!Runline_stx&&!Runline_sty)
          {
            y2-=5;y1=5;if(y2<0){y1=0;y2=0;}
            d1=y2*y2;
            if(d0>d1){x1=sqrt(d0-d1)-x2;if(x1<0)x1=0;x2=sqrt(d0-d1);}else {x1=0;x2=0;}
            Runline_stx=1;
            Runline_sty=1;
            Profile_xflag=0;
            Profile_yflag=0;
          }
        if(Runline_stx||Runline_sty)
          ProfileXY(speed,x1,y1);
       }
      break;
      case 8:
        if((!(a&0x80)||!(a&0x40))&&(!(b&0x80)||!(b&0x40))){
        if(x2>=x&&!Runline_stx&&!Runline_sty)
         {lbl=1;Loran_01(1);}
        else if(!Runline_stx&&!Runline_sty)
          {
            x2+=5;x1=5;if(x2>x){x2=x;x1=0;}
            d1=x2*x2;
            if(d0>d1){y1=y2-sqrt(d0-d1);if(y1<0)y1=0;y2=sqrt(d0-d1);}else {y1=0;y2=0;}
            Runline_stx=1;
            Runline_sty=1;
            Profile_xflag=0;
            Profile_yflag=0;
         }
        if(Runline_stx||Runline_sty)
          ProfileXY(speed,x1,y1);
       }
      break;
     }
    }
}

void Loran_02(char flag){   /*Ò¡¡õ*/
    static long x,y;
    static int speed;
    static lbl;
    static char Maxv;
    unsigned char a,b,c;
    if(!flag){Profile_xflag=0;Profile_yflag=0;Maxv=cncode.Z;
    lbl=0;x=labs(cncode.X);y=labs(cncode.Y);
    speed=cncode.R;
    Runline_stx=1;
    Runline_sty=1;
    Profile_xflag=0;
    Profile_yflag=0;
    TiggerXY();
    }else{
     outb(ICXaddr,0x12);
     a=inb(ICXaddr+1);
     outb(ICYaddr,0x12);
     b=inb(ICYaddr+1);
     switch(lbl){
      case 0:
       if((!(a&0x80)||!(a&0x40))&&Runline_stx)
         {
           Profile_x(speed,x);
         }
       if((!(b&0x80)||!(b&0x40))&&Runline_sty)
         {
           Profile_y(speed,0);
         }
       if(!Runline_stx&&!Runline_sty)
         {lbl++;Runline_stx=1;Runline_sty=1;Profile_xflag=0;Profile_yflag=0;}
      break;
      case 1:
       if((!(b&0x80)||!(b&0x40))&&Runline_sty)
         Profile_y(speed,y);
       if(!Runline_sty){lbl++;Runline_sty=1;Profile_yflag=0;}
      break;
      case 2:
       if((!(a&0x80)||!(a&0x40))&&Runline_stx)
         Profile_x(speed,-x*2);
       if(!Runline_stx){lbl++;Runline_stx=1;Profile_xflag=0;}
      break;
      case 3:
       if((!(b&0x80)||!(b&0x40))&&Runline_sty)
         Profile_y(speed,-y*2);
       if(!Runline_sty){lbl++;Runline_sty=1;Profile_yflag=0;}
      break;
      case 4:
       if((!(a&0x80)||!(a&0x40))&&Runline_stx)
         Profile_x(speed,x*2);
       if(!Runline_stx){lbl++;Runline_stx=1;Profile_xflag=0;}
      break;
      case 5:
       if((!(b&0x80)||!(b&0x40))&&Runline_sty)
         Profile_y(speed,y*2);
       if(!Runline_sty){lbl=2;Runline_sty=1;Profile_yflag=0;}
      break;
     }
    }
}
void Loran_03(char flag){   /*Ò¡¡ó*/
    static long x,y,x0,y0,x2,y2;
    static int speed;
    static lbl;
    static char Maxv,flags;
    unsigned char a,b,c;
    static long x1,y1;
    long lx,ly;
    char test[10];
    if(!flag){
    x0=XPosi(1);y0=YPosi(1);Maxv=cncode.Z;
    lbl=0;x=labs(cncode.X);y=labs(cncode.Y);x1=0;y1=0;
    speed=cncode.R;flags=0;
    TiggerXY();
    Runline_stx=1;
    Runline_sty=1;
    }else{
     outb(ICXaddr,0x12);
     a=inb(ICXaddr+1);
     outb(ICYaddr,0x12);
     b=inb(ICYaddr+1);
     ltoa(x2,test,10);
     ListMess1(test);
     itoa(lbl,test,10);
     ListCRT1(test);
     itoa(a,test,16);
     ListHT(1,test);
     switch(lbl){
      case 0:
        if((!(a&80)||!(a&40))&&Runline_stx)
          {
            Profile_x(speed,x);Profile_y(speed,0);Profile_xflag=0;Profile_yflag=0;
          }
       if(!Runline_stx)
         {lbl++;x2=x0+x;y2=y0;x1=x2;}
        break;
      case 1:
       if((!(a&80)||!(a&40))&&(!(b&80)||!(b&40))){
        if(x2>x0&&!Runline_stx&&!Runline_sty)
          {lbl++;Loran_03(1);}
        else if(!Runline_stx&&!Runline_sty)
          {
            x2-=2;y2+=2;
            Runline_stx=1;
            Runline_sty=1;
          }
        if(Runline_stx||Runline_sty)
          {
            ProfileXY(speed,-2,2);Profile_xflag=0;Profile_yflag=0;
          }
       }
      break;
      case 2:
       if((!(a&80)||!(a&40))&&(!(b&80)||!(b&40))){
        if(y2>y0&&!Runline_stx&&!Runline_sty)
          {lbl++;Loran_03(1);}
        else if(!Runline_stx&&!Runline_sty)
          {
            y2-=2;x2-=2;
            Runline_stx=1;
            Runline_sty=1;
          }
        if(Runline_stx||Runline_sty)
          {
            ProfileXY(speed,-2,-2);Profile_xflag=0;Profile_yflag=0;
          }
       }
      break;
      case 3:
       if((!(a&80)||!(a&40))&&(!(b&80)||!(b&40))){
        if(x2<x0&&!Runline_stx&&!Runline_sty)
          {lbl++;Loran_03(1);}
        else if(!Runline_stx&&!Runline_sty)
          {
            x2+=2;y2-=2;
            Runline_stx=1;
            Runline_sty=1;
          }
        if(Runline_stx||Runline_sty)
          {
            ProfileXY(speed,2,-2);Profile_xflag=0;Profile_yflag=0;
          }
       }
      break;
      case 4:
       if((!(a&80)||!(a&40))&&(!(b&80)||!(b&40))){
        if(y2>=y0)
          {
            if((x2!=x1||y2!=y0)&&!Runline_stx&&!Runline_sty)
              {lx=x1-x2;ly=y0-y2;Runline_stx=1;Runline_sty=1;}
           }
        else if(y2<y0&&!Runline_stx&&!Runline_sty)
          {lbl=1;Loran_03(1);}
        else if(!Runline_stx&&!Runline_sty)
          {
            x2+=2;y2+=2;
            lx=2;ly=2;
            Runline_stx=1;
            Runline_sty=1;
          }
        if(Runline_stx||Runline_sty)
          {
            ProfileXY(speed,lx,ly);Profile_xflag=0;Profile_yflag=0;
          }
       }
      break;
     }
    }
}
void Loran_04(char flag){   /*Ò¡¡Á*/
    static long x,y,x0,y0,x2,y2;
    static int speed;
    static lbl;
    static char Maxv;
    static long x1,y1;
    unsigned char a,b,c;
    if(!flag){
    x0=XPosi(1);y0=YPosi(1);Maxv=cncode.Z;
    lbl=0;x=labs(cncode.X);y=labs(cncode.Y);x1=0;y1=0;
    speed=cncode.R;
    }else{
     outb(ICXaddr,0x12);
     a=inb(ICXaddr+1);
     outb(ICYaddr,0x12);
     b=inb(ICYaddr+1);
     switch(lbl){
      case 0:
       x2=x0;y2=y0;
       x1=x0+x;y1=y0+y;
       TiggerXY();
       lbl++;
      break;
      case 1:
       if((!(a&80)||!(a&40))&&(!(b&80)||!(b&40))){
        if(y2<y1&&x2<x1&&!Runline_stx&&!Runline_sty)
          {
            x2+=2;y2+=2;
            Profile_xflag=0;
            Profile_yflag=0;
            ProfileXY(speed,2,2);
          }
        else if((y2>=y1||x2>=x1))
          {
            if(x2!=x1||y2!=y1)
              {
                Profile_xflag=0;
                Profile_yflag=0;
                ProfileXY(speed,x1-x2,y1-y2);
              }
            else
              {lbl++;x1=x0;y1=y0;}
          }
        else
          {

          }
       }
      break;
      case 2:
       if((!(a&80)||!(a&40))&&(!(b&80)||!(b&40))){
        if(y2>y1&&x2>x1)
          {
            x2-=2;y2-=2;
            Profile_xflag=0;
            Profile_yflag=0;
            ProfileXY(speed,-2,-2);
          }
        else
          {
            if(x2!=x1||y2!=y1)
              {
                Profile_xflag=0;
                Profile_yflag=0;
                ProfileXY(speed,x1-x2,y1-y2);
              }
            else
              {lbl++;x1=x0-x;y1=y0+y;}
          }
       }
      break;
      case 3:
       if((!(a&80)||!(a&40))&&(!(b&80)||!(b&40))){
        if(y2<y1&&x2>x1)
          {
            x2-=2;y2+=2;
            Profile_xflag=0;
            Profile_yflag=0;
            ProfileXY(speed,-2,2);
          }
        else
          {
            if(x2!=x1||y2!=y1)
              {
                Profile_xflag=0;
                Profile_yflag=0;
                ProfileXY(speed,x1-x2,y1-y2);
              }
            else
              {lbl++;x1=x0;y1=y0;}
          }
       }
      break;
      case 4:
       if((!(a&80)||!(a&40))&&(!(b&80)||!(b&40))){
        if(y2>y1&&x2<x1)
          {
            x2+=2;y2-=2;
            Profile_xflag=0;
            Profile_yflag=0;
            ProfileXY(speed,2,-2);
          }
        else
          {
            if(x2!=x1||y2!=y1)
              {
                Profile_xflag=0;
                Profile_yflag=0;
                ProfileXY(speed,x1-x2,y1-y2);
              }
            else
              {lbl++;x1=x0-x;y1=y0-y;}
          }
       }
      break;
      case 5:
       if((!(a&80)||!(a&40))&&(!(b&80)||!(b&40))){
        if(y2>y1&&x2>x1)
          {
            x2-=2;y2-=2;
            Profile_xflag=0;
            Profile_yflag=0;
            ProfileXY(speed,-2,-2);
          }
        else
          {
            if(x2!=x1||y2!=y1)
              {
                Profile_xflag=0;
                Profile_yflag=0;
                ProfileXY(speed,x1-x2,y1-y2);
              }
            else
              {lbl++;x1=x0;y1=y0;}
          }
       }
      break;
      case 6:
       if((!(a&80)||!(a&40))&&(!(b&80)||!(b&40))){
        if(y2<y1&&x2<x1)
          {
            x2+=2;y2+=2;
            Profile_xflag=0;
            Profile_yflag=0;
            ProfileXY(speed,2,2);
          }
        else
          {
            if(x2!=x1||y2!=y1)
              {
                Profile_xflag=0;
                Profile_yflag=0;
                ProfileXY(speed,x1-x2,y1-y2);
              }
            else
              {lbl++;x1=x0+x;y1=y0-y;}
          }
       }
      break;
      case 7:
       if((!(a&80)||!(a&40))&&(!(b&80)||!(b&40))){
        if(y2>y1&&x2<x1)
          {
            x2+=2;y2-=2;
            Profile_xflag=0;
            Profile_yflag=0;
            ProfileXY(speed,2,-2);
          }
        else
          {
            if(x2!=x1||y2!=y1)
              {
                Profile_xflag=0;
                Profile_yflag=0;
                ProfileXY(speed,x1-x2,y1-y2);
              }
            else
              {lbl++;x1=x0;y1=y0;}
          }
       }
      break;
      case 8:
       if((!(a&80)||!(a&40))&&(!(b&80)||!(b&40))){
        if(y2<y1&&x2>x1)
          {
            x2-=2;y2+=2;
            Profile_xflag=0;
            Profile_yflag=0;
            ProfileXY(speed,-2,2);
          }
        else
          {
            if(x2!=x1||y2!=y1)
              {
                Profile_xflag=0;
                Profile_yflag=0;
                ProfileXY(speed,x1-x2,y1-y2);
              }
            else
              {lbl=1;x1=x0+x;y1=y0+y;}
          }
       }
      break;
     }
    }
}
void Loran_05(char flag){   /*Ò¡£«*/
    static long x,y,x0,y0;
    static int speed;
    static lbl;
    static char Maxv;
    unsigned char a,b,c;
    static long x1,y1;
    long l;
    if(!flag){Profile_xflag=0;Profile_yflag=0;
    x0=XPosi(1);y0=YPosi(1);Maxv=cncode.Z;
    lbl=0;x=labs(cncode.X);y=labs(cncode.Y);x1=0;y1=0;
    speed=cncode.R;
    }else{
     outb(ICXaddr,0x12);
     a=inb(ICXaddr+1);
     outb(ICYaddr,0x12);
     b=inb(ICYaddr+1);
     switch(lbl){
      case 0:
       x1=x0+x;
       TiggerXY();
       lbl++;
      break;
      case 1:
       l=XPosi(0);
       if(!(a&80)||!(a&40))
         Profile_x(speed,x);
       if(l<x1+JD&&l>x1-JD){
        lbl++;
        x1=x0;Profile_xflag=0;Profile_yflag=0;
       }
      break;
      case 2:
       l=XPosi(0);
       if(!(a&80)||!(a&40))
         Profile_x(-speed,-x);
       if(l<x1+JD&&l>x1-JD){
        lbl++;
        y1=y0+y;Profile_xflag=0;Profile_yflag=0;
       }
      break;
      case 3:
       l=YPosi(0);
       if(!(b&80)||!(b&40))
         Profile_y(speed,y);
       if(l<y1+JD&&l>y1-JD){
        lbl++;
        y1=y0;Profile_xflag=0;Profile_yflag=0;
       }
      break;
      case 4:
       l=YPosi(0);
       if(!(b&80)||!(b&40))
         Profile_y(-speed,-y);
       if(l<y1+JD&&l>y1-JD){
        lbl++;
        x1=x0-x;Profile_xflag=0;Profile_yflag=0;
       }
      break;
      case 5:
       l=XPosi(0);
       if(!(a&80)||!(a&40))
         Profile_x(-speed,-x);
       if(l<x1+JD&&l>x1-JD){
        lbl++;
        x1=x0;Profile_xflag=0;Profile_yflag=0;
       }
      break;
      case 6:
       l=XPosi(0);
       if(!(a&80)||!(a&40))
         Profile_x(speed,x);
       if(l<x1+JD&&l>x1-JD){
        lbl++;
        y1=y0-y;Profile_xflag=0;Profile_yflag=0;
       }
      break;
      case 7:
       l=YPosi(0);
       if(!(b&80)||!(b&40))
         Profile_y(-speed,-y);
       if(l<y1+JD&&l>y1-JD){
        lbl++;
        y1=y0;Profile_xflag=0;Profile_yflag=0;
       }
      break;
      case 8:
       l=YPosi(0);
       if(!(b&80)||!(b&40))
         Profile_y(speed,y);
       if(l<y1+JD&&l>y1-JD){
        lbl=1;
        x1=x0+x;Profile_xflag=0;Profile_yflag=0;
       }
      break;
     }
    }
}
void Loran_06(char flag){   /*Ò¡*/
if(flag) ;
}
void Loran_07(char flag){   /*Ò¡£«*/
if(flag) ;
}
void ProfileXY(int Acceleration,long x,long y){
 if(Runline_stx)
   {
     Profile_x(Acceleration,x);
   }
 if(Runline_sty)
  {
    Profile_y(Acceleration,y);
  }
}
void Profile_xy(int Acceleration,long x0,long y0,long x,long y){
  int speedx,speedy;
  double dx,dy,t;
  dx=(double)x0;
  dy=(double)y0;
  dx=x-dx;
  dy=y-dy;
  if(labs(dx)>=labs(dy)&&dx)
    {
       if(0==dx)
         {speedx=0;}
       else if(dx>0)
         {speedx=Acceleration;}
       else
         {speedx=-Acceleration;}
       t=dy/fabs(dx)*Acceleration;
       speedy=(int)t;
       if(0==speedy&&dy)
         speedy=(int)dy/fabs(dy);
    }
  else if(dy)
    {
       if(0==dy)
         {speedy=0;}
       else if(dy>0)
         {speedy=Acceleration;}
       else
         {speedy=-Acceleration;}
       t=dx/fabs(dy)*Acceleration;
       speedx=(int)t;
       if(0==speedx&&dx)
         speedx=(int )dx/fabs(dx);
    }

  Profile_x(speedx,x);
  Profile_y(speedy,y);
}
void TiggerXY(void)
{
}
#endif
