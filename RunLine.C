#include "znc.h"
#include <dos.h>
#include <conio.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
void Write_target(long x,long y,long z)
{
    outb(ICXaddr,0x41);
    outb((ICXaddr+1),x&0xff);
    outb(ICXaddr,0x42);
    outb((ICXaddr+1),((x>>8)&0xff));
    outb(ICXaddr,0x43);
    outb((ICXaddr+1),((x>>16)&0xff));

    outb(ICYaddr,0x41);
    outb((ICYaddr+1),y&0xff);
    outb(ICYaddr,0x42);
    outb((ICYaddr+1),((y>>8)&0xff));
    outb(ICYaddr,0x43);
    outb((ICYaddr+1),((y>>16)&0xff));

    outb(Address_Z,0x41);
    outb((Address_Z+1),z&0xff);
    outb(Address_Z,0x42);
    outb((Address_Z+1),((z>>8)&0xff));
    outb(Address_Z,0x43);
    outb((Address_Z+1),((z>>16)&0xff));
}
void Discharge(long x1,long y1,long z1)
{
    unsigned int inter = 0,vx,vy,vz,a;
    double d =100.0;
    char test[10];
      if(labs(x1)>=labs(y1))   /*3个坐标轴按距离排序*/
        {
          if(labs(x1)>=labs(z1))
            {
              if(labs(y1)>=labs(z1))
                {inter = 0x24;}
              else
                {inter = 0x18;}
                vx = (Table.Sudu[Dis_lines]-1)&0x7;d = labs(y1);d /= labs(x1);vy = d * vx + 1;
                d = labs(z1);d /= labs(x1);vz = d * vx + 1;
            }
          else
            {
              inter = 0x12;
              vz = (Table.Sudu[Dis_lines]-1)&0x7;d = labs(y1);d /= labs(z1);vy = d * vz + 1;
              d = labs(x1);d /= labs(z1);vx = d * vz + 1;
            }
        }
      else
        {
          if(labs(y1)>=labs(z1))
            {
              if(labs(x1)>=labs(z1))
                {inter = 0x21;}
              else
                {inter = 0x9;}
                vy = (Table.Sudu[Dis_lines]-1)&0x7;d = labs(x1);d /= labs(y1);vx = d * vy + 1;
                d = labs(z1);d /= labs(y1);vz = d * vy + 1;
            }
          else
            {
                inter = 0x6;
                vz = (Table.Sudu[Dis_lines]-1)&0x7;d = labs(y1);d /= labs(z1);vy = d * vz + 1;
                d = labs(x1);d /= labs(z1);vx = d * vz + 1;
            }
        }
    OTPS0&=0xf8;
    OTPS0|=vz;
    outb(OSC,0x13);
    outb(OSC+1,OTPS0);

    a = vy << 3;
    a += vx;
    outb(OSC,0x12);
    outb((OSC+1),a);

    outb((Inter),0x14);
    outb((Inter+1),0x00);

    OTPBX1&=0xf0;
    outb((ICXaddr),0x14);
    outb((ICXaddr+1),OTPBX1);
    outb(ICXaddr,0x51);
    outb(ICXaddr+1,(unsigned )vx&0xff);
    outb(ICXaddr,0x52);
    outb(ICXaddr+1,(unsigned)vx>>8);
    OTPBX1|=0xa;
    outb((ICXaddr),0x14);
    outb((ICXaddr+1),OTPBX1);

    OTPBY1&=0xf0;
    outb((ICYaddr),0x14);
    outb((ICYaddr+1),OTPBY1);
    outb(ICYaddr,0x51);
    outb(ICYaddr+1,(unsigned )vy&0xff);
    outb(ICYaddr,0x52);
    outb(ICYaddr+1,(unsigned)vy>>8);
    OTPBY1|=0xa;
    outb((ICYaddr),0x14);
    outb((ICYaddr+1),OTPBY1);

    IOZ1&=0xf0;
    outb((Address_Z),0x14);
    outb((Address_Z+1),IOZ1);
    outb(Address_Z,0x51);
    outb(Address_Z+1,(unsigned )vz&0xff);
    outb(Address_Z,0x52);
    outb(Address_Z+1,(unsigned)vz>>8);
    IOZ1|=0xa;
    outb((Address_Z),0x14);
    outb((Address_Z+1),IOZ1);

    outb((Inter),0x13);
    outb((Inter+1),inter);
    delay(0x800);
    outb((Inter),0x14);
    outb((Inter+1),0x05);
}
RunLine(){
  static long x1,y1,z1,x0,y0,z0;/*pi是XY平面的偏差值，qi是XZ平面的偏差值*/
  static char End_flagl=0; /*ax1,ax2,ax3分别表示第一二三坐标轴*/
  static unsigned char xyz;
  switch(RunLineFlag){
    case 0:
      x0=XPosi(1);y0=YPosi(1);z0=ZPosi(1);
      x1=cncode.X;
      y1=cncode.Y;
      z1=cncode.Z;
      DIS_enode=Qnode_int(1,0,x0,y0,z0,x1,y1,z1,DIS_enode);
      Queue_en(&DIS_que,DIS_enode);
      Current_DIS = DIS_que.rear;
      x1 -= x0;
      y1 -= y0;
      z1 -= z0;
      Read_StartEnd();
      if(x0==cncode.X&&y0==cncode.Y&&z0==cncode.Z)
        {RunLineFlag=12;break;}
      if(!Dis_flag)
        Diss();
      Write_target(x1,y1,z1);
      SRunPi(0,x1,y1);
      SRunDIS(0);
      Discharge(x1,y1,z1);
      RunLineFlag++;
      xyz = 0;
      break;
    case 1:
        if(x0==cncode.X)
            Position_control_x(cncode.X);
        if(y0==cncode.Y)
            Position_control_y(cncode.Y);
        if(z0==cncode.Z)
            Position_control_z(cncode.Z);
        RunLineFlag++;
        break;
    case 2:
      xyz+=Diss();
      if(xyz>49)
      {
          RunLineFlag = 12;
      }
      break;
    case 12:
      if(!PositionEndXYZ(0x84))
        {
          End_flagl++;
          if(3==End_flagl)
            {
              Pause();
              RunCode(0);
              End_flagl=0;
            }
        }
      else
       {
              Position_control_x(cncode.X);
              Position_control_y(cncode.Y);
              Position_control_z(cncode.Z);
              End_flagl=0;
       }
    break;
  }
}
