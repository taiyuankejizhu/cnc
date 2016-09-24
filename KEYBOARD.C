#include "znc.h"
#include <stdlib.h>
#include <graphics.h>
#include <dos.h>
#include <math.h>
#include "motion.h"
double CNC_atof(const char *str){  /*字符串到浮点数的转换*/
  double l=0;
  char *temp,*temp1,*s;
  int k=10;
  s=(char *) malloc(sizeof(strlen(str)+1));
  temp=(char *) malloc(sizeof(strlen(str)+1));
  strcpy(s,str);
  while(!('+'==*s||'-'==*s||*s>='0'&&*s<='9'||'.'==*s))
    if(0==*s||'\0'==*s)
      return l;
    else
      *s++;
  if(('+'==*s||'-'==*s)&&!(*(s+1)>='0'&&*(s+1)<='9'))
    return l;
    temp1=temp;
  while('+'==*s||'-'==*s||*s>='0'&&*s<='9')
    {
       *temp++=*s++;
     }
     *temp='\0';
     l=(double)atol(temp1);
     if('.'==*s)
       {
          *s++;
          while(*s>='0'&&*s<='9')
            {
              if(l>=0)
                l+=(double)(*s++-'0')/k;
              else
                l-=(double)(*s++-'0')/k;
              k*=10;
            }
       }
       return l;
}
void Set_HF1(char i,char j,char init){
 int k;
 for(k=0;k<10;k++)
   {
     if(k!=j)
       HF1[i][k]=0;
     else if(init==0&&k==j)
       HF1[i][j]^=1;
     else
       HF1[i][k]=0;
   }
}
void KeyBoard1(char k){
  register i,j;
  char test[10];
/*  char *str[10]={"","手动:无代码","手动:ＭＤＩ","编辑:文件","LORAN辅助工具","加工:文件","加工:程序控制","显示","设定","加工条件"};*/
  InputCharE();
  if(HM[k]>1)return;if(Ks3)Ks3=0;
  if(Ks5){LN05A=FFN;Ks5=0;}
  for(j=0;j<11;j++)
  {
    HF1[HF[k]][j]=HF2[HF[k]][j];
    if(j == 4)
    {
        itoa(HF1[HF[k]][j],test,10);
        ListMess1(test);
    }
  }

/*  ListHT(0,str[k]);ListHT(1,"");*/
  switch(k){
    case 2:HF[2]=3;InitEdit();
      HF1[HF[2]][6]=4;HF1[HF[2]][7]=4;HF1[HF[2]][8]=4;HF1[HF[2]][9]=4;
    break;
    case 3:InitEdit();
    break;
    case 4:InitLN();HF[4]=7;
    break;
    case 5:
      InitEdit();Ks3=1;Ks5=0;ListLN05(0);
      HF1[HF[5]][6]=0;HF1[HF[5]][7]=0;HF1[HF[5]][8]=0;HF1[HF[5]][9]=0;
      for(j=0;j<11;j++)HF1[HF[5]][j]|=2;HF1[HF[5]][10]=4;
    break;
    case 6:InitProCon();LoadProCon();break;
  }
  HM[HM[0]]=0;HM[k]=1;HM[0]=k;/*ListHM(i);ListHM(k);*/
}
void KeyBoard(void){
   register unsigned k,i;
   int UDK,UDK1,UDKX1,UDKY1;
/*  if(Ck8255_flag&0x80){Ck8255_flag&=0x7f;ShowTB(0x17);}*/
    outb(OSC,0x11);
    UDK=inb(OSC+1);/*速度切换*/
    outb(Address_Z,0x11);
    UDK1=inb(Address_Z+1);/*Z轴手控*/
    outb(ICXaddr,0x11);
    UDKX1=inb(ICXaddr+1); /*X轴手控*/
    outb(ICYaddr,0x11);
    UDKY1=inb(ICYaddr+1); /*Y轴手控*/
    if(!(UDK&0x04))/* SWF 三种状态转换 */
      {
          if(!SWF_K)
             {
                 SoundTime=SoundTIME;
                 SWF();
             }
         if(SWF_K<=40)SWF_K++;
         if(SWF_K==40)OpenScr();
      }
    else
      SWF_K=0;
    if(!XDNUP)
      {
        if(!(UDKX1&0x10))XDNUP=1;   /*DN*/
        if(!(UDKX1&0x20))XDNUP=2;   /*UP*/
      }
    else
      {
        if((UDKX1&0x20))XDNUP=0;
      }
    if(XDN_K&&(UDKX1&0x10))
      {
         set_PIDX(0x14,0,1,0x20);
         if(XDN_K>=UDKN)
           Stop_x(SWF_flag+1);
         else 
           Stop_x(-1);
         XDN_K=0;
         XUP_K=0;
      }
   if(XUP_K&&(UDKX1&0x20))
     {
        set_PIDX(0x14,0,1,0x20);
        if(XUP_K>=UDKN)Stop_x(SWF_flag+1);else Stop_x(-1);XUP_K=0;XDN_K=0;
     }
   if(!(UDKX1&0x10))
     {if(UDKX1&0x20)XDNUP=1; XDown();}    /* DN 键  */
   else if(!(UDKX1&0x20))/* UP 键  */
          {
            if((UDKX1&0x10))XDNUP=2;
            XUp();
           }
    if(!YDNUP)
      {
        if(!(UDKY1&0x10))YDNUP=1;    /*YDN*/
        if(!(UDKY1&0x20))YDNUP=2; /*YUP*/
      }
    else
     { if(UDKY1&0x20)YDNUP=0;}
    if(YDN_K&&(UDKY1&0x10))
      {
        set_PIDY(0x14,0,1,0x20);
        if(YDN_K>=UDKN)
          Stop_y(SWF_flag+1);
        else
          Stop_y(-1);
          YDN_K=0;
          YUP_K=0;
      }
    if(YUP_K&&(UDKY1&0x20))
        {set_PIDY(0x14,0,1,0x20);if(YUP_K>=UDKN)Stop_y(SWF_flag+1);else Stop_y(-1);YUP_K=0;YDN_K=0;}
      if(!(UDKY1&0x10)&&((UDKY1&0x20)||YDNUP==2))
        {if(UDKY1&0x20)YDNUP=1;YDown();}      /* DN 键  */
      else
        if(!(UDKY1&0x20)){if(UDKY1&0x01)YDNUP=2;YUp();}  /* UP 键  */
    if(!DNUP)
      {
        if(!(UDK1&0x10))DNUP=1;   /*DN*/
        if(!(UDK1&0x20))DNUP=2;   /*UP*/
      }
    else
      {
        if((UDK1&0x20))DNUP=0;
      }
    if(DN_K&&(UDK1&0x10))
      {
         set_PIDZ(0x14,0,1,0x20);
         if(DN_K>=UDKN)
           Stop_z(SWF_flag+1);
         else 
           Stop_z(-1);
         DN_K=0;
         UP_K=0;
      }
   if(UP_K&&(UDK1&0x20))
     {
        set_PIDZ(0x14,0,1,0x20);
        if(UP_K>=UDKN)Stop_z(SWF_flag+1);else Stop_z(-1);UP_K=0;DN_K=0;
     }
   if(!(UDK1&0x10)&&((UDK1&0x20)||DNUP==2))
     {if(Dis_flag&&K[8])F11();if(UDK1&0x20)DNUP=1;Down();}    /* DN 键  */
   else if(!(UDK1&0x20))/* UP 键  */
          {
             if((UDK1&0x01))DNUP=2;
             if(Dis_flag&&!K[8]) F11();
             Up();
           }
    if(!Dis_flag)if(!(UDK&0x10))
      {
         SoundTime=SoundTIME;
         if(!F[1])
           {ShowMess(15);}
         else
           {
             ZeroFlag=0;
             lock_z=0;DN_K=0;UP_K=0;lock_x=0; XDN_K=0;XUP_K=0;lock_y=0; YDN_K=0;YUP_K=0;
             position_xdis=position_x;position_ydis=position_y;
             cncode.Code=0xfd;
             RunCode(1);
           }
       }/* DIS键  */
    if(!(UDK&0x20)&&!F11_flag){SoundTime=SoundTIME;F11();}  /* 停止开关 */
    if(UDK&0x20)F11_flag=0;
    k=KeyBoardPress();
    if(k)
      {
        if(Ck8255_flag&0x80){Ck8255_flag&=0x7f;ShowTB(0x17);}
        if(Ck8255_flag&0x40){Ck8255_flag&=0xbf;ShowTB(0x18);}
        if(Scr_flag){SoundTime=SoundTIME;return;}
        if(k<=0xa0||k>0xff)while(KeyBoardPress());
        SoundTime=SoundTIME;
        OpenSound();
        if(k==kINS){K11();}
        if(k==kMUL){SaveBMP256();return;}
        if(Fx&&k==0x52){K11();}
        #if TESTING
        if(Fx&&k==0x53){K12();}
        if(Fx&&k==0x47){SWF_K++;SWF();return;}
        if(Fx&&k==0x49)Velocity_control_z(0x7fff);
        if(Fx&&k==0x51)Velocity_control_z(0x8000);
        if(Fx&&k==0x4F)Stop_z(0);
        #endif
        if((Fx&&k==aF11)||(!Fx&&(k=='P'||k=='p'))){F11();}
        if(Fx&&k==aF12&&!Dis_flag)
          {
              if(!F[1]||F[6]>1||KEYL==6)
                {ShowMess(15);return;}
              else
                {
                  lock_z=0;DN_K=0;UP_K=0;lock_x=0; XDN_K=0;XUP_K=0;lock_y=0; YDN_K=0;YUP_K=0;
                  position_xdis=position_x;position_ydis=position_y;
                    cncode.Code=0xfd;
                     RunCode(1);
                }
         }
     }
     if(k)
         {
            switch(KESCF)
                  {
                     case   0:ListHM(0);Key0(k);break;
                     case   1:Key1(k); break;
                     case   2:Key2(k); break;
                     case   3:Key4(k);break;
                     case   4:Key7(k); break;
                     case   5:Key5(k); break;
                     case   6:Key6(k); break;
                     case   8:Keya(k); break;
                     case  10:Key8(k); break;
                     case  11:Key11(k); break;
                     case  12:Key12(k); break;
                     case  13:Key13(k); break;
                     case  14:Key14(k); break;
                    /*case  30:Key30(k); break;*/
                  }
            }
     else
       switch(KESCF)
         {
             case 13:Key13(0); break;
         }
}
void Key0(int k){  /*主菜单*/
  switch(k){
    case kF1:KESCF=1;HM[0]=1;HF[1]=1;ListHF0(HM[0],0);break;
    case kF2:KESCF=2;ListHF0(HM[0],0);break;
    case kF3:KESCF=3;HM[0]=3;HF[1]=4;ListHF0(HM[0],0);KeyBoard1(3);ListEdit(0,0);
             break;
    case kF4:KESCF=4;HM[0]=4;ListHF0(HM[0],0);KeyBoard1(4);ListLN00(0);break;
    case kF5:KESCF=5;HM[0]=5;ListHF0(HM[0],0);KeyBoard1(5);break;
    case kF6:KESCF=6;HM[0]=6;ListHF0(HM[0],0);KeyBoard1(6);ListProCon(0,-1);break;
    case kF7:break;
    case kF8:KESCF=8;HM[0]=8;HF[8]=12;ListHF0(HM[0],0);/*KeyBoard1(8);*/break;
    case kF9:break;
    case kF10:HM[1]=0;HM[0]=0;ListHM(0);KESCF=0;break;
  }
}
void Key1(int k){ /*定位*/
  switch(k){
    case kF1:KESCF=11;HM[0]=1;HF[1]=0;ListHF0(HM[0],0);break;
    case kF2:KESCF=12;HM[0]=1;HF[1]=2;ListHF0(HM[0],0);break;
    case kF3:Set_HF1(9,0,1);KESCF=13;HM[0]=1;HF[1]=9;ListHF0(HM[0],0);break;
    case kF4:KESCF=14;HM[0]=1;HF[1]=10;ListHF0(HM[0],0);break;
    case kF5:break;
    case kF6:break;
    case kF7:break;
    case kF8:break;
    case kF9:break;
    case kF10:HM[1]=0;HM[0]=0;ListHM(0);KESCF=0;break;
  }
}
void Key11(int k){/*坐标设置*/
  static char Key_i;
  switch(k){
    case kF1:break;
    case kF2:Set_HF1(0,1,0);ListHF0(HM[0],0);Key_i=0;Key_i+=HF1[0][1]*0x80;Set_lengthXYZ();break;
    case kF3:Set_HF1(0,2,0);ListHF0(HM[0],0);Key_i=1;Key_i+=HF1[0][2]*0x80;Set_lengthXYZ();break;
    case kF4:Set_HF1(0,3,0);ListHF0(HM[0],0);Key_i=2;Key_i+=HF1[0][3]*0x80;Set_lengthXYZ();break;
    case kF5:break;
    case kF6:break;
    case kF7:break;
    case kF8:break;
    case kF9:break;
    case kF10:Set_HF1(2,0,1);KESCF=1;HM[0]=1;HF[1]=1;ListHF0(HM[0],0);break;
    case 0xd:Set_HF1(0,0,1);ListHF0(HM[0],0);Modify_XYZ(Key_i);Key_i=0;CNC_Showinput(0);break;
    default:if(Key_i&0x80)CNC_Showinput(k);else {Key_i=0;CNC_Showinput(0);}break;
  }
}
void Key12(int k){/*移动*/
  long ll=0;
  static char Key_i;
  switch(k){
    case kF1:Set_HF1(2,0,0);ListHF0(HM[0],0);Key_i=0;Key_i+=HF1[2][0]*0x80;Set_lengthXYZ();break;
    case kF2:Set_HF1(2,1,0);ListHF0(HM[0],0);Key_i=1;Key_i+=HF1[2][1]*0x80;Set_lengthXYZ();break;
    case kF3:Set_HF1(2,2,0);ListHF0(HM[0],0);Key_i=2;Key_i+=HF1[2][2]*0x80;Set_lengthXYZ();break;
    case kF4:break;
    case kF5:break;
    case kF6:break;
    case kF7:break;
    case kF8:break;
    case kF9:break;
    case kF10:Set_HF1(2,0,1);KESCF=1;HM[0]=1;HF[1]=1;ListHF0(HM[0],0);break;
    case 0xd:Set_HF1(2,0,1);ListHF0(HM[0],0);ll=CNC_Getinput();Hand_MoveXYZ(Key_i,ll);Key_i=0;CNC_Showinput(0);break;
    default:if(Key_i&0x80)CNC_Showinput(k);else {Key_i=0;CNC_Showinput(0);}break;
  }
}
void Key13(int k){/*归零*/
  static char i=0;
  switch(k){
    case kF1:Set_HF1(9,0,0);ListHF0(HM[0],0);if(HF1[9][0]==1){i=1;Stop_xyz();ZeroX(0);ZeroX_positive();}else {if(i==1)Stop_x(0);i=0;ZeroX(0);}break;
    case kF2:Set_HF1(9,1,0);ListHF0(HM[0],0);if(HF1[9][1]==1){i=2;Stop_xyz();ZeroX(0);ZeroX_negative();}else {if(i==2)Stop_x(0);i=0;ZeroX(0);}break;
    case kF3:Set_HF1(9,2,0);ListHF0(HM[0],0);if(HF1[9][2]==1){i=3;Stop_xyz();ZeroY(0);ZeroY_positive();}else {if(i==3)Stop_y(0);i=0;ZeroY(0);}break;
    case kF4:Set_HF1(9,3,0);ListHF0(HM[0],0);if(HF1[9][3]==1){i=4;Stop_xyz();ZeroY(0);ZeroY_negative();}else {if(i==4)Stop_y(0);i=0;ZeroY(0);}break;
    case kF5:Set_HF1(9,4,0);ListHF0(HM[0],0);if(HF1[9][4]==1){i=5;Stop_xyz();ZeroZ(0);ZeroZ_positive();}else {if(i==5)Stop_z(0);i=0;ZeroZ(0);}break;
    case kF6:Set_HF1(9,5,0);ListHF0(HM[0],0);if(HF1[9][5]==1){i=6;Stop_xyz();ZeroZ(0);ZeroZ_negative();}else {if(i==6)Stop_z(0);i=0;ZeroZ(0);}break;
    case kF7:Set_HF1(9,6,0);ListHF0(HM[0],0);break;
    case kF8:Set_HF1(9,7,0);ListHF0(HM[0],0);break;
    case kF9:Set_HF1(9,8,0);ListHF0(HM[0],0);break;
    case kF10:Set_HF1(9,0,1);KESCF=1;HM[0]=1;HF[1]=1;ListHF0(HM[0],0);i=0;break;
  }
  if(k==0)
    {
    if(i==1&&HF1[9][0]==1)
        {
          ZeroX(1);
          if(ZeroFlagX==1)
            {
              Set_HF1(9,0,1);
              KESCF=13;HM[0]=1;HF[1]=9;ListHF0(HM[0],0);
              i=0;
            }
        }
    if(i==2&&HF1[9][1]==1)
        {
          ZeroX(1);
          if(ZeroFlagX==1)
            {
              Set_HF1(9,0,1);
              KESCF=13;HM[0]=1;HF[1]=9;ListHF0(HM[0],0);
              i=0;
            }
        }
    if(i==3&&HF1[9][2]==1)
        {
          ZeroY(1);
          if(ZeroFlagY==1)
          {
            Set_HF1(9,0,1);
            KESCF=13;HM[0]=1;HF[1]=9;ListHF0(HM[0],0);
            i=0;
          }
        }
    if(i==4&&HF1[9][3]==1)
        {
          ZeroY(1);
          if(ZeroFlagY==1)
          {
            Set_HF1(9,0,1);
            KESCF=13;HM[0]=1;HF[1]=9;ListHF0(HM[0],0);
            i=0;
          }
        }
    if(i==5&&HF1[9][4]==1)
        {
          ZeroZ(1);
          if(ZeroFlag==1)
          {
            Set_HF1(9,0,1);
            KESCF=13;HM[0]=1;HF[1]=9;ListHF0(HM[0],0);
            i=0;
          }
        }
    if(i==6&&HF1[9][5]==1)
        {
          ZeroZ(1);
          if(ZeroFlag==1)
          {
            Set_HF1(9,0,1);
            KESCF=13;HM[0]=1;HF[1]=9;ListHF0(HM[0],0);
            i=0;
          }
        }
    }
}
void Key14(int k){/*中心*/
  switch(k){
    case kF1:break;
    case kF2:break;
    case kF3:break;
    case kF4:break;
    case kF5:break;
    case kF6:break;
    case kF7:break;
    case kF8:break;
    case kF9:break;
    case kF10:KESCF=1;HM[0]=1;HF[1]=1;ListHF0(HM[0],0);break;
  }
}
void Key2(int k){
   switch(k){
    case kF1:break;
    case kF2:break;
    case kF3:break;
    case kF4:break;
    case kF5:break;
    case kF6:break;
    case kF7:break;
    case kF8:break;
    case kF9:break;
    case kF10:KESCF=1;HM[0]=1;HF[1]=1;ListHF0(HM[0],0);break;
    }
}
void Key3(int k){/*手动 ＭＤＩ 1*/
  if(Ks3){Key85(k);return;}
  switch(k){
    case kF1:break;
    case kF2:break;
    case kF3:break;
    case kF4:break;
    case kF5:break;
    case kF6:break;
    case kF7:break;
    case kF8:break;
    case kF9:break;
    case kF10:HM[1]=0;HM[0]=0;ListHM(0);ListMess1("定位");KESCF=0;break;
    case kF11:HF[HM[0]]++;ListHF0(HM[0],0);Ks4=0;break;
    default:Key40(k);break;
  }
}
void Key9(int k){/*加工 文件*/
  switch(k){
    case kF1:break;
    case kF2:break;
    case kF3:break;
    case kF4:break;
    case kF5:break;
    case kF6:break;
    case kF7:break;
    case kF8:break;
    case kF9:break;
    case kF10:break;
    case kF11:break;
  }
}
void Key60xx(int k){/*加工 PROCON*/
  switch(k){
    case kF1:break;
    case kF2:break;
    case kF3:break;
    case kF4:break;
    case kF5:break;
    case kF6:break;
    case kF7:break;
    case kF8:break;
    case kF9:break;
    case kF10:break;
    case kF11:break;
  }
}
void Keya(int k){/*设定*/
  static char Key_i;
  switch(k){
    case kF1:HF1[12][0]^=1;Key_i=0;Key_i+=HF1[12][0]*0x80;if(HF1[12][0])Set_lengthAI();else {AI_time=0;ShowFlag(7);}ListHF0(HM[0],0);break;
    case kF2:HF1[12][1]^=1;Key_i=1;Key_i+=HF1[12][1]*0x80;if(HF1[12][1])Set_lengthAI();else {AI_high=0;ShowFlag(8);}ListHF0(HM[0],0);break;
    case kF3:Key_i=0x82;Modify_Set(Key_i);ListHF0(HM[0],0);Key_i=0;break;
    case kF4:Key_i=0x83;Modify_Set(Key_i);ListHF0(HM[0],0);Key_i=0;break;
    case kF5:HF1[12][4]^=1;Key_i=4;Key_i+=HF1[12][4]*0x80;if(HF1[12][4]){MaxL=2;MaxN=99;MinN=1;}else {Steel_flag=0;ShowFlag(11);}ListHF0(HM[0],0);break;
    case kF6:F2();HF1[12][5]^=1;Key_i=5;Key_i+=HF1[12][5]*0x80;ListHF0(HM[0],0);break;
    case kF7:F3();HF1[12][6]^=1;Key_i=6;Key_i+=HF1[12][6]*0x80;ListHF0(HM[0],0);break;
    case kF8:break;
    case kF9:break;
    case kF10:HM[1]=0;HM[0]=0;ListHM(0);KESCF=0;break;
    case 0xd:if( Key_i != 0x82&& Key_i != 0x83){Modify_Set(Key_i);ListHF0(HM[0],0);Key_i=0;CNC_Showinput(0);}break;
    default:if(Key_i&0x80)CNC_Showinput(k);else {Key_i=0;CNC_Showinput(0);}break;
  }
}
void Keyd(int k){/*显示*/
  switch(k){
    case kF1:break;
    case kF2:break;
    case kF3:break;
    case kF4:break;
    case kF5:break;
    case kF6:break;
    case kF7:break;
    case kF8:break;
    case kF9:break;
    case kF10:break;
    case kF11:break;
  }
}
void Keyf(int k){/*设定*/
switch(k){
    case kF1:break;
    case kF2:break;
    case kF3:break;
    case kF4:break;
    case kF5:break;
    case kF6:break;
    case kF7:break;
    case kF8:break;
    case kF9:break;
    case kF10:break;
    case kF11:break;
  }
}
void Key7(int k){/*LORAN 辅助工具 1*/
  int i;i=LN00K+1;
  switch(k){
    case kLF:if(!LN00K)LN00K=__LN00K__;LN00K--;ListLN00(i);ListLN00(LN00K+1);break;
    case kRT:LN00K++;if(LN00K>=__LN00K__)LN00K=0;ListLN00(i);ListLN00(LN00K+1);break;
    case kUP:if(LN00K<8)LN00K+=(__LN00K__-__LN00K__%8);else LN00K-=8;if(LN00K>=__LN00K__)LN00K-=8;ListLN00(i);ListLN00(LN00K+1);break;
    case kDN:LN00K+=8;if(LN00K>=__LN00K__)LN00K%=8;ListLN00(i);ListLN00(LN00K+1);break;
    case kENT:
    case kF1:KESCF=10;HF1[8][Ks8]=1;
      HF[HM[0]]=8;ListHF0(HM[0],0);
      switch(Ks8){
        case 0:ListLN01(0);break;
        case 1:ListLN02(0,0);break;
        case 2:
          switch(LNStr.sl0){
        case 0:ListLN030(0);break;
        case 1:ListLN031(0);break;
        case 2:ListLN032(0);break;
        case 3:ListLN033(0);break;
          }
        break;
      }
    break;
    case kF2:break;
    case kF3:break;
    case kF4:break;
    case kF5:break;
    case kF6:break;
    case kF7:break;
    case kF8:break;
    case kF9:break;
    case kF10:HM[1]=0;HM[0]=0;ListHM(0);KESCF=0;Set_disp();break;
  }
/*  switch(HF[HM[0]]){
      case 16:Key7(k);break;*/

}
void Key8(int k){/*LORAN 辅助工具 2*/
  unsigned int i;
  if(LN_NM[1])switch(k){
    case kF1:
    case kF2:
    case kF3:
    case kF4:
    case kF5:
    case kF6:
    case kF7:
    case kF8:
    case kF9:
    case kF10:
    case kF11:
    InputCharE();
    break;
  }
  switch(k){
    case kF1:/*if(Ks8==0)break;*/
      if(HF1[8][9]!=4){HF1[8][9]=4;LN034.sy=0;ListHF0(HM[0],10);}
      HF1[8][Ks8]=0;ListHF0(HM[0],Ks8+1);
      if(Ks8==2||Ks8==1){for(i=_K8_;i<_K8_+4;i++)HF1[8][i]=4;
        ListHF0(HM[0],0);}
      if(HF1[8][4]!=2){HF1[8][4]=2;ListHF0(HM[0],5);}
      Ks8=0;HF1[8][Ks8]=1;ListHF0(HM[0],Ks8+1);
      ListLN01(0);break;
    case kF2:/*if(Ks8==1)break;*/
      if(HF1[8][9]!=4){HF1[8][9]=4;LN034.sy=0;ListHF0(HM[0],10);}
      HF1[8][Ks8]=0;ListHF0(HM[0],Ks8+1);
      if(Ks8==2){for(i=_K8_+3;i<_K8_+4;i++)HF1[8][i]=4;
        ListHF0(HM[0],i);}
      Ks8=1;HF1[8][Ks8]=1;ListHF0(HM[0],Ks8+1);
      for(i=_K8_;i<_K8_+3;i++){HF1[8][i]=0;
        ListHF0(HM[0],i+1);}
      if(HF1[8][4]!=2){HF1[8][4]=2;ListHF0(HM[0],5);}
      MakeLN20();
      ListLN02(0,-1);break;
    case kF3:/*if(Ks8==2)break;*/
      if(LNStr.sl0==3){HF1[8][9]=0;/*ListHF0(HM[0],10);*/}
      HF1[8][Ks8]=0;ListHF0(HM[0],Ks8+1);
      if(Ks8!=2){Ks8=2;for(i=_K8_;i<_K8_+4;i++){if(LNStr.sl0==i-_K8_)HF1[8][i]=1;
        else HF1[8][i]=0;}ListHF0(HM[0],0);}
      if(HF1[8][4]!=2){HF1[8][4]=2;ListHF0(HM[0],5);}
      Ks8=2;HF1[8][Ks8]=1;ListHF0(HM[0],Ks8+1);
      switch(LNStr.sl0){
        case 0:ListLN030(0);break;
        case 1:ListLN031(0);break;
        case 2:ListLN032(0);break;
        case 3:ListLN033(0);break;
      }
      break;
    case kF4:if(Ks8==3)break;
          HF1[8][4]=0;ListHF0(HM[0],5);
      if(HF1[8][9]!=4){HF1[8][9]=4;LN034.sy=0;ListHF0(HM[0],10);}
      HF1[8][Ks8]=0;ListHF0(HM[0],Ks8+1);
      if(Ks8==2||Ks8==1){for(i=_K8_;i<_K8_+4;i++)HF1[8][i]=4;
      ListHF0(HM[0],0);}
      Ks8=3;HF1[8][Ks8]=1;ListHF0(HM[0],Ks8+1);
      MakeLN1();itoa(1,LN04.str1,10);
      if(!LNStr.sl0&&LN030.sw2){LN04S[2]=2;LN04S[3]=2;}
      else{LN04S[2]=0;LN04S[3]=0;}
      ListLN04(0);break;
    case kF5:if(HF1[8][4])break;
       HF1[8][Ks8]=0;ListHF0(HM[0],Ks8+1);
          Ks8=4;HF1[8][Ks8]=1;ListHF0(HM[0],Ks8+1);
      ListLN05(0);
      break;
    case kF6:if(Ks8==1){MakeLN26();}if(Ks8!=2)break;
      if(HF1[8][9]!=4){HF1[8][9]=4;LN034.sy=0;ListHF0(HM[0],10);}
      HF1[8][_K8_+LNStr.sl0]=0;ListHF0(HM[0],LNStr.sl0+_K8_+1);
      LNStr.sl0=0;ListHF0(HM[0],3);
      HF1[8][_K8_+LNStr.sl0]=1;ListHF0(HM[0],LNStr.sl0+_K8_+1);
      ListLN030(0);
      break;
    case kF7:if(Ks8==1){MakeLN25();}if(Ks8!=2)break;
      if(HF1[8][9]!=4){HF1[8][9]=4;LN034.sy=0;ListHF0(HM[0],10);}
      HF1[8][_K8_+LNStr.sl0]=0;ListHF0(HM[0],LNStr.sl0+_K8_+1);
      LNStr.sl0=1;ListHF0(HM[0],3);
      HF1[8][_K8_+LNStr.sl0]=1;ListHF0(HM[0],LNStr.sl0+_K8_+1);
      ListLN031(0);
      break;
    case kF8:if(Ks8==1){MakeLN24();}if(Ks8!=2)break;
      if(HF1[8][9]!=4){HF1[8][9]=4;LN034.sy=0;ListHF0(HM[0],10);}
      HF1[8][_K8_+LNStr.sl0]=0;ListHF0(HM[0],LNStr.sl0+_K8_+1);
      LNStr.sl0=2;ListHF0(HM[0],3);
      HF1[8][_K8_+LNStr.sl0]=1;ListHF0(HM[0],LNStr.sl0+_K8_+1);
      ListLN032(0);
      break;
    case kF9:if(Ks8!=2)break;
      {HF1[8][9]=0;ListHF0(HM[0],10);}
      HF1[8][_K8_+LNStr.sl0]=0;ListHF0(HM[0],LNStr.sl0+_K8_+1);
      LNStr.sl0=3;ListHF0(HM[0],3);
      HF1[8][_K8_+LNStr.sl0]=1;ListHF0(HM[0],LNStr.sl0+_K8_+1);
      ListLN033(0);
      break;
    case kF10:if(Ks8!=2||LNStr.sl0!=3)break;
      HF1[8][9]^=1;ListHF0(HM[0],10);
      if(HF1[8][9]==1){LN034.y=0;LN034.sy=1;LN034.sx=1;}
      else{LN034.y=0;LN034.sy=0;LN034.sx=0;}
      if(!LN033K){LN_NM[1]=0;ListLN033(1);}
      ListLN034(0,0);
      break;
    case kF11:KESCF=4;HM[0]=4;KeyBoard1(4);ListHF0(HM[0],0);ListLN00(0);
      break;
    default:
    switch(Ks8){
      case 0:Key81(k);break;
      case 1:Key82(k);break;
      case 2:Key83(k);break;
      case 3:Key84(k);break;
      case 4:Key85(k);break;
    }
  }
}
void Key81(int k){
  register i,j;
  i=LN01K;j=0;
  switch(i){
    case  1:NM_Str=LN01.str0;LN_NM[2]=LN01.st0;j++;break;
    case  3:NM_Str=LN01.str1;LN_NM[2]=LN01.st1;j++;break;
    case  4:NM_Str=LN01.str2;LN_NM[2]=LN01.st2;j++;break;
    case  6:NM_Str=LN01.str3;LN_NM[2]=LN01.st3;j++;break;
    case  7:NM_Str=LN01.str4;LN_NM[2]=LN01.st4;j++;break;
    case 10:NM_Str=LN01.str5;LN_NM[2]=LN01.st5;j++;break;
    case 12:NM_Str=LN01.str6;LN_NM[2]=LN01.st6;j++;break;
  }
  switch(k){
    case kUP:if(LN_NM[1])InputCharE();LN01K--;ListLN01(i+1);
      do{i--;if(i<0)i=__LN01K__-1;}while(LN01S[i]);
      LN01K=i;LN_NM[1]=0;
    break;
    case kDN:if(LN_NM[1])InputCharE();LN01K++;ListLN01(i+1);
      do{i++;if(i>=__LN01K__)i=0;}while(LN01S[i]);
      LN01K=i;LN_NM[1]=0;
    break;
    case kLF:
      switch(i){
    case 0:if(!LN01.sl0)LN01.sl0=__LNSTR1__;LN01.sl0--;break;
    case 2:if(!LN01.sl1)LN01.sl1=__LNSTR2__;LN01.sl1--;break;
    case 5:if(!LN01.sl2)LN01.sl2=__LNSTR3__;
      if(LN01.sl2==__LNSTR3__)LN01S[10]=0;if(LN01.sl2==__LNSTR3__-1)LN01S[10]=2;
      if(LN01.sl2==__LNSTR3__-1)LN01S[7]=0;if(LN01.sl2==__LNSTR3__-2)LN01S[7]=2;
      ListLN01(11);ListLN01(8);LN01.sl2--;break;
    case 9:if(!LN01.sl3)LN01.sl3=__LNSTR4__;LN01.sl3--;break;
    case 13:if(!LN01.sl4)LN01.sl4=__LNSTR5__;LN01.sl4--;break;
    case 8:
    case 11:break;
      }
      if(j)
    if(!LN_NM[1])LN_NM[1]=1;else if(LN_NM[1]>1)LN_NM[1]--;
    break;
    case kRT:
      switch(i){
    case 0:LN01.sl0++;if(LN01.sl0>=__LNSTR1__)LN01.sl0=0;break;
    case 2:LN01.sl1++;if(LN01.sl1>=__LNSTR2__)LN01.sl1=0;break;
    case 5:LN01.sl2++;if(LN01.sl2==__LNSTR3__)LN01S[10]=2;
      if(LN01.sl2==__LNSTR3__-1)LN01S[10]=0;ListLN01(11);
      if(LN01.sl2==__LNSTR3__-1)LN01S[7]=2;
      if(LN01.sl2==__LNSTR3__-2)LN01S[7]=0;ListLN01(8);
      if(LN01.sl2>=__LNSTR3__)LN01.sl2=0;break;
    case 9:LN01.sl3++;if(LN01.sl3>=__LNSTR4__)LN01.sl3=0;break;
    case 13:LN01.sl4++;if(LN01.sl4>=__LNSTR5__)LN01.sl4=0;break;
    case 8:
    case 11:break;
      }
      if(j)
    if(LN_NM[1]<=strlen(NM_Str)&&LN_NM[1]<LN_NM[0])LN_NM[1]++;
    break;
    case kENT:
      switch(i){
    case 0:LN01.sl0++;if(LN01.sl0>=__LNSTR1__)LN01.sl0=0;break;
    case 2:LN01.sl1++;if(LN01.sl1>=__LNSTR2__)LN01.sl1=0;break;
    case 5:LN01.sl2++;if(LN01.sl2==__LNSTR3__)LN01S[10]=2;
      if(LN01.sl2==__LNSTR3__-1)LN01S[10]=0;ListLN01(11);
      if(LN01.sl2==__LNSTR3__-1)LN01S[7]=2;
      if(LN01.sl2==__LNSTR3__-2)LN01S[7]=0;ListLN01(8);
      if(LN01.sl2>=__LNSTR3__)LN01.sl2=0;break;
    case 9:LN01.sl3++;if(LN01.sl3>=__LNSTR4__)LN01.sl3=0;break;
    case 13:LN01.sl4++;if(LN01.sl4>=__LNSTR5__)LN01.sl4=0;break;
    case 8:LN01.sw0^=1;break;
    case 11:LN01.sw1^=1;if(LN01.sw1)LN01S[i+1]=0;else LN01S[i+1]=2;
        ListLN01(i+2);break;
      }
    if(j){
    if(!LN_NM[1])LN_NM[1]=1;
    else{
      InputCharE();LN_NM[1]=0;
      LN01K++;ListLN01(i+1);
      do{i++;if(i>=__LN01K__)i=0;}while(LN01S[i]);
      LN01K=i;
    }
      }
    break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '.':
    case '+':
    case '=':
    case '-':
    case kBS:
    case kDEL:
    InputChar(k);
    break;
  }
  ListLN01(++i);
}
void Key82(int k){
  register i,j;
  i=LN02.y+LN02.sy-1;
  switch(LN02.sx){
    case  1:NM_Str=tab[i]->no;LN_NM[0]=tablen.no-1;LN_NM[2]=tabtype.no;
        NM_Str[0]='C';NM_Str++;break;
    case  2:NM_Str=tab[i]->pl;LN_NM[0]=tablen.pl;LN_NM[2]=tabtype.pl;break;
    case  3:NM_Str=tab[i]->ip;LN_NM[0]=tablen.ip;LN_NM[2]=tabtype.ip;break;
    case  4:NM_Str=tab[i]->on;LN_NM[0]=tablen.on;LN_NM[2]=tabtype.on;break;
    case  5:NM_Str=tab[i]->off;LN_NM[0]=tablen.off;LN_NM[2]=tabtype.off;break;
    case  6:NM_Str=tab[i]->sv;LN_NM[0]=tablen.sv;LN_NM[2]=tabtype.sv;break;
    case  7:NM_Str=tab[i]->s;LN_NM[0]=tablen.s;LN_NM[2]=tabtype.s;break;
    case  8:NM_Str=tab[i]->up;LN_NM[0]=tablen.up;LN_NM[2]=tabtype.up;break;
    case  9:NM_Str=tab[i]->dn;LN_NM[0]=tablen.dn;LN_NM[2]=tabtype.dn;break;
    case 10:NM_Str=tab[i]->v;LN_NM[0]=tablen.v;LN_NM[2]=tabtype.v;break;
    case 11:NM_Str=tab[i]->lc;LN_NM[0]=tablen.lc;LN_NM[2]=tabtype.lc;break;
    case 12:NM_Str=tab[i]->pp;LN_NM[0]=tablen.pp;LN_NM[2]=tabtype.pp;break;
    case 13:NM_Str=tab[i]->b_ofs;LN_NM[0]=tablen.b_ofs;LN_NM[2]=tabtype.b_ofs;break;
    case 14:NM_Str=tab[i]->s_ofs;LN_NM[0]=tablen.s_ofs;LN_NM[2]=tabtype.s_ofs;break;
    case 15:NM_Str=tab[i]->lns;LN_NM[0]=tablen.lns;LN_NM[2]=tabtype.lns;break;
    case 16:NM_Str=tab[i]->step;LN_NM[0]=tablen.step;LN_NM[2]=tabtype.step;break;
    case 17:NM_Str=tab[i]->lnm;LN_NM[0]=tablen.lnm;LN_NM[2]=tabtype.lnm;break;
    case 18:NM_Str=tab[i]->ry;LN_NM[0]=tablen.ry;LN_NM[2]=tabtype.ry;break;
    case 19:NM_Str=tab[i]->timer;LN_NM[0]=tablen.timer;LN_NM[2]=tabtype.timer;break;
  }
  switch(k){
    case kPGUP:if(LN_NM[1]){InputCharE();LN_NM[1]=0;}if(LN02.sy+LN02.y==1)break;
    if(LN02.y>=10){LN02.y-=10;ListLN02(0,0);}
    else{LN02.sy+=LN02.y-10;if(LN02.sy<1)LN02.sy=1;LN02.y=0;ListLN02(0,0);}
    break;
    case kPGDN:if(LN_NM[1]){InputCharE();LN_NM[1]=0;}if(LN02.sy+LN02.y>=LN02.topline)break;
    if(LN02.topline<=10)LN02.sy=LN02.topline;
    else{if(LN02.y<=LN02.topline-20)LN02.y+=10;
      else{LN02.sy+=20-(LN02.topline-LN02.y);
      if(LN02.sy>10)LN02.sy=10;LN02.y=LN02.topline-10;}
    }ListLN02(0,0);
    break;
    case kUP:if(LN_NM[1]){InputCharE();LN_NM[1]=0;}if(LN02.sy+LN02.y==1)break;
    if(LN02.sy>1){ListLN02(LN02.sx,LN02.sy--);ListLN02(LN02.sx,LN02.sy);}
    else{LN02.y--;ListLN02(0,0);}
    break;
    case kDN:if(LN_NM[1]){InputCharE();LN_NM[1]=0;}if(LN02.sy+LN02.y>=LN02.topline)break;
    if(LN02.sy<10){ListLN02(LN02.sx,LN02.sy++);ListLN02(LN02.sx,LN02.sy);}
    else{LN02.y++;ListLN02(0,0);}
    break;
    case kLF:if(LN_NM[1]){if(LN_NM[1]>1)LN_NM[1]--;
    ListLN02(LN02.sx,LN02.sy);break;}
    if(!LN02.topline)break;
    if(LN02.sy+LN02.y==1&&LN02.sx==1)break;
    ListLN02(LN02.sx--,LN02.sy);if(!LN02.sx){LN02.sx=__TABLEN__;LN02.sy--;}
    if(!LN02.sy){LN02.sy=1;LN02.y--;ListLN02(0,0);}
    else ListLN02(LN02.sx,LN02.sy);break;
    case kRT:if(LN_NM[1]){if(LN_NM[1]<=strlen(NM_Str)&&LN_NM[1]<LN_NM[0])LN_NM[1]++;
    ListLN02(LN02.sx,LN02.sy);break;}
    if(!LN02.topline)break;
    if(LN02.sy+LN02.y==LN02.topline&&LN02.sx==__TABLEN__)break;
    ListLN02(LN02.sx++,LN02.sy);if(LN02.sx>__TABLEN__){LN02.sx=1;LN02.sy++;}
    if(LN02.sy>10){LN02.sy=10;LN02.y++;ListLN02(0,0);}
    else ListLN02(LN02.sx,LN02.sy);break;
    case kHOME:if(LN_NM[1])break;if(LN02.sx==1||!LN02.topline)break;
    LN02.sx=1;ListLN02(0,LN02.sy);break;
    case kEND:if(LN_NM[1])break;if(LN02.sx==__TABLEN__||!LN02.topline)break;
    LN02.sx=__TABLEN__;ListLN02(0,LN02.sy);break;
    case kENT:if(!LN02.topline)break;
      if(!LN_NM[1]){LN_NM[1]=1;ListLN02(LN02.sx,LN02.sy);}
    else{
      InputCharE();LN_NM[1]=0;
      if(LN02.sy+LN02.y>=LN02.topline&&LN02.sx==__TABLEN__){ListLN02(LN02.sx,LN02.sy);break;}
      ListLN02(LN02.sx++,LN02.sy);if(LN02.sx>__TABLEN__){LN02.sx=1;LN02.sy++;}
      if(LN02.sy>10){LN02.sy=10;LN02.y++;ListLN02(0,0);}
      else ListLN02(LN02.sx,LN02.sy);
      LN_NM[1]=0;
    }break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '.':
    case '+':
    case '=':
    case '-':
    case kBS:
    case kDEL:
    if(!LN02.topline)break;
    InputChar(k);ListLN02(LN02.sx,LN02.sy);
    break;
  }
}
void Key83(int k){
  switch(LNStr.sl0){
    case 0:Key830(k);break; /*1孔*/
    case 1:Key831(k);break; /*格子*/
    case 2:Key832(k);break; /*圆周*/
    case 3:if(!HF1[8][9])Key833(k);
    else Key834(k);break;   /*任意*/
  }
}
void Key830(int k){
  register i,j;
  int ii;
  i=LN030K;j=0;
  switch(i){
    case  3:NM_Str=LN030.str0;LN_NM[2]=LN030.st0;j++;break;
    case  4:NM_Str=LN030.str1;LN_NM[2]=LN030.st1;j++;break;
    case  5:NM_Str=LN030.str2;LN_NM[2]=LN030.st2;j++;break;
    case  6:NM_Str=LN030.str3;LN_NM[2]=LN030.st3;j++;break;
    case  7:NM_Str=LN030.str4;LN_NM[2]=LN030.st4;j++;break;
  }
  switch(k){
    case kUP:if(LN_NM[1])InputCharE();LN030K--;ListLN030(i+1);
      do{i--;if(i<0)i=__LN030K__-1;}while(LN030S[i]);
      LN030K=i;LN_NM[1]=0;
    break;
    case kDN:if(LN_NM[1])InputCharE();LN030K++;ListLN030(i+1);
      do{i++;if(i>=__LN030K__)i=0;}while(LN030S[i]);
      LN030K=i;LN_NM[1]=0;
    break;
    case kLF:
      switch(i){
    case 9:if(!LN030.sl0)LN030.sl0=__LNSTR5__;LN030.sl0--;break;
    case 0:
    case 1:
    case 2:
    case 8:break;
      }
      if(j)
    if(!LN_NM[1])LN_NM[1]=1;else if(LN_NM[1]>1)LN_NM[1]--;
    break;
    case kRT:
      switch(i){
    case 9:LN030.sl0++;if(LN030.sl0>=__LNSTR5__)LN030.sl0=0;break;
    case 0:
    case 1:
    case 2:
    case 8:break;
      }
      if(j)
    if(LN_NM[1]<=strlen(NM_Str)&&LN_NM[1]<LN_NM[0])LN_NM[1]++;
    break;
    case kENT:
      switch(i){
    case 0:if(LN030.sw1){LN030.sw1=0;ListLN030(2);}
      if(LN030.sw2){LN030.sw2=0;ListLN030(3);
        for(ii=3;ii<7;){LN030S[ii++]=0;ListLN030(ii);}}
      LN030.sw0=1;break;
    case 1:if(LN030.sw0){LN030.sw0=0;ListLN030(1);}
      if(LN030.sw2){LN030.sw2=0;ListLN030(3);
        for(ii=3;ii<7;){LN030S[ii++]=0;ListLN030(ii);}}
      LN030.sw1=1;break;
    case 2:if(LN030.sw0){LN030.sw0=0;ListLN030(1);}
      if(LN030.sw1){LN030.sw1=0;ListLN030(2);}
      if(!LN030.sw2){
        for(ii=3;ii<7;){LN030S[ii++]=2;ListLN030(ii);}}
      LN030.sw2=1;break;
    case 8:LN030.sw3^=1;break;
    case 9:LN030.sl0++;if(LN030.sl0>=__LNSTR5__)LN030.sl0=0;break;
      }if(j){
    if(!LN_NM[1])LN_NM[1]=1;
    else{
      InputCharE();LN_NM[1]=0;
      LN030K++;ListLN030(i+1);
      do{i++;if(i>=__LN030K__)i=0;}while(LN030S[i]);
      LN030K=i;
    }
      }
    break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '.':
    case '+':
    case '=':
    case '-':
    case kBS:
    case kDEL:
    InputChar(k);
    break;
  }
  ListLN030(++i);
}

void Key831(int k){
  register i,j;
  i=LN031K;j=0;
  switch(i){
    case  0:NM_Str=LN031.str0;LN_NM[2]=LN031.st0;j++;break;
    case  1:NM_Str=LN031.str1;LN_NM[2]=LN031.st1;j++;break;
    case  2:NM_Str=LN031.str2;LN_NM[2]=LN031.st2;j++;break;
    case  3:NM_Str=LN031.str3;LN_NM[2]=LN031.st3;j++;break;
    case  4:NM_Str=LN031.str4;LN_NM[2]=LN031.st4;j++;break;
    case  5:NM_Str=LN031.str5;LN_NM[2]=LN031.st5;j++;break;
    case  6:NM_Str=LN031.str6;LN_NM[2]=LN031.st6;j++;break;
    case  7:NM_Str=LN031.str7;LN_NM[2]=LN031.st7;j++;break;
    case  8:NM_Str=LN031.str8;LN_NM[2]=LN031.st8;j++;break;
    case  9:NM_Str=LN031.str9;LN_NM[2]=LN031.st9;j++;break;
    case 14:NM_Str=LN031.str10;LN_NM[2]=LN031.st10;j++;break;
  }
  switch(k){
    case kUP:if(LN_NM[1])InputCharE();LN031K--;ListLN031(i+1);
      do{i--;if(i<0)i=__LN031K__-1;}while(LN031S[i]);
      LN031K=i;LN_NM[1]=0;
    break;
    case kDN:if(LN_NM[1])InputCharE();LN031K++;ListLN031(i+1);
      do{i++;if(i>=__LN031K__)i=0;}while(LN031S[i]);
      LN031K=i;LN_NM[1]=0;
    break;
    case kLF:
      switch(i){
    case 12:if(!LN031.sl0)LN031.sl0=__LNSTR5__;LN031.sl0--;break;
    case 10:
    case 11:
    case 13:break;
      }
      if(j)
    if(!LN_NM[1])LN_NM[1]=1;else if(LN_NM[1]>1)LN_NM[1]--;
    break;
    case kRT:
      switch(i){
    case 12:LN031.sl0++;if(LN031.sl0>=__LNSTR5__)LN031.sl0=0;break;
    case 10:
    case 11:
    case 13:break;
      }
      if(j)
    if(LN_NM[1]<=strlen(NM_Str)&&LN_NM[1]<LN_NM[0])LN_NM[1]++;
    break;
    case kENT:
      switch(i){
    case 12:LN031.sl0++;if(LN031.sl0>=__LNSTR5__)LN031.sl0=0;break;
    case 10:LN031.sw0^=1;break;
    case 11:LN031.sw1^=1;break;
    case 13:LN031.sw2^=1;if(LN031.sw2)LN031S[14]=0;
      else LN031S[14]=2;ListLN031(15);
    break;
      }if(j){
    if(!LN_NM[1])LN_NM[1]=1;
    else{
      InputCharE();LN_NM[1]=0;
      LN031K++;ListLN031(i+1);
      do{i++;if(i>=__LN031K__)i=0;}while(LN031S[i]);
      LN031K=i;
    }
      }
    break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '.':
    case '+':
    case '=':
    case '-':
    case kBS:
    case kDEL:
    InputChar(k);
    break;
  }
  ListLN031(++i);
}
void Key832(int k){
  register i,j;
  i=LN032K;j=0;
  switch(i){
    case  0:NM_Str=LN032.str0;LN_NM[2]=LN032.st0;j++;break;
    case  1:NM_Str=LN032.str1;LN_NM[2]=LN032.st1;j++;break;
    case  2:NM_Str=LN032.str2;LN_NM[2]=LN032.st2;j++;break;
    case  3:NM_Str=LN032.str3;LN_NM[2]=LN032.st3;j++;break;
    case  4:NM_Str=LN032.str4;LN_NM[2]=LN032.st4;j++;break;
    case  5:NM_Str=LN032.str5;LN_NM[2]=LN032.st5;j++;break;
    case  6:NM_Str=LN032.str6;LN_NM[2]=LN032.st6;j++;break;
    case  7:NM_Str=LN032.str7;LN_NM[2]=LN032.st7;j++;break;
    case  8:NM_Str=LN032.str8;LN_NM[2]=LN032.st8;j++;break;
    case 13:NM_Str=LN032.str9;LN_NM[2]=LN032.st9;j++;break;
  }
  switch(k){
    case kUP:if(LN_NM[1])InputCharE();LN032K--;ListLN032(i+1);
      do{i--;if(i<0)i=__LN032K__-1;}while(LN032S[i]);
      LN032K=i;LN_NM[1]=0;
    break;
    case kDN:if(LN_NM[1])InputCharE();LN032K++;ListLN032(i+1);
      do{i++;if(i>=__LN032K__)i=0;}while(LN032S[i]);
      LN032K=i;LN_NM[1]=0;
    break;
    case kLF:
      switch(i){
    case 11:if(!LN032.sl0)LN032.sl0=__LNSTR5__;LN032.sl0--;break;
    case  9:
    case 10:
    case 12:break;
      }
      if(j)
    if(!LN_NM[1])LN_NM[1]=1;else if(LN_NM[1]>1)LN_NM[1]--;
    break;
    case kRT:
      switch(i){
    case 11:LN032.sl0++;if(LN032.sl0>=__LNSTR5__)LN032.sl0=0;break;
    case  9:
    case 10:
    case 12:break;
      }
      if(j)
    if(LN_NM[1]<=strlen(NM_Str)&&LN_NM[1]<LN_NM[0])LN_NM[1]++;
    break;
    case kENT:
      switch(i){
    case 11:LN032.sl0++;if(LN032.sl0>=__LNSTR5__)LN032.sl0=0;break;
    case  9:LN032.sw0^=1;break;
    case 10:LN032.sw1^=1;break;
    case 12:LN032.sw2^=1;if(LN032.sw2)LN032S[13]=0;
      else LN032S[13]=2;ListLN032(14);
    break;
      }if(j){
    if(!LN_NM[1])LN_NM[1]=1;
    else{
      InputCharE();LN_NM[1]=0;
      LN032K++;ListLN032(i+1);
      do{i++;if(i>=__LN032K__)i=0;}while(LN032S[i]);
      LN032K=i;
    }
      }
    break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '.':
    case '+':
    case '=':
    case '-':
    case kBS:
    case kDEL:
    InputChar(k);
    break;
  }
  ListLN032(++i);
}

void Key833(int k){
  register i,j;
  i=LN033K;j=0;
  switch(i){
    case  0:NM_Str=LN033.str0;LN_NM[2]=LN033.st0;j++;break;
    case  1:NM_Str=LN033.str1;LN_NM[2]=LN033.st1;j++;break;
    case  6:NM_Str=LN033.str2;LN_NM[2]=LN033.st2;j++;break;
  }
  switch(k){
    case kUP:if(LN_NM[1]){InputCharE();if(!LN033K)ListLN034(0,0);}LN033K--;ListLN033(i+1);
      do{i--;if(i<0)i=__LN033K__-1;}while(LN033S[i]);
      LN033K=i;LN_NM[1]=0;
    break;
    case kDN:if(LN_NM[1]){InputCharE();if(!LN033K)ListLN034(0,0);}LN033K++;ListLN033(i+1);
      do{i++;if(i>=__LN033K__)i=0;}while(LN033S[i]);
      LN033K=i;LN_NM[1]=0;
    break;
    case kLF:
      switch(i){
    case  4:if(!LN033.sl0)LN033.sl0=__LNSTR5__;LN033.sl0--;break;
    case  2:
    case  3:
    case  5:break;
      }
      if(j)
    if(!LN_NM[1])LN_NM[1]=1;else if(LN_NM[1]>1)LN_NM[1]--;
    break;
    case kRT:
      switch(i){
    case  4:LN033.sl0++;if(LN033.sl0>=__LNSTR5__)LN033.sl0=0;break;
    case  2:
    case  3:
    case  5:break;
      }
      if(j)
    if(LN_NM[1]<=strlen(NM_Str)&&LN_NM[1]<LN_NM[0])LN_NM[1]++;
    break;
    case kENT:
      switch(i){
    case  4:LN033.sl0++;if(LN033.sl0>=__LNSTR5__)LN033.sl0=0;break;
    case  2:LN033.sw0^=1;break;
    case  3:LN033.sw1^=1;break;
    case  5:LN033.sw2^=1;if(LN033.sw2)LN033S[6]=0;
      else LN033S[6]=2;ListLN033(7);
    break;
      }if(j){
    if(!LN_NM[1])LN_NM[1]=1;
    else{
      InputCharE();if(!LN033K)ListLN034(0,0);LN_NM[1]=0;
      LN033K++;ListLN033(i+1);
      do{i++;if(i>=__LN033K__)i=0;}while(LN033S[i]);
      LN033K=i;
    }
      }
    break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '.':
    case '+':
    case '=':
    case '-':
    case kBS:
    case kDEL:
    InputChar(k);
    break;
  }
  ListLN033(++i);
}
void Key834(int k){
  register i;
  switch(LN034.sx){
    case  1:i=0;LN_NM[2]=LN034.st0;break;
    case  2:i=12;LN_NM[2]=LN034.st1;break;
    case  3:i=24;LN_NM[2]=LN034.st1;break;
    case  4:i=36;LN_NM[2]=LN034.st1;break;
  }NM_Str=LN034A+(LN034.y+LN034.sy-1)*48+i;
  switch(k){
    case kPGUP:if(LN_NM[1]){InputCharE();LN_NM[1]=0;}if(LN034.sy+LN034.y==1)break;
    if(LN034.y>=10)LN034.y-=10;else{LN034.sy+=LN034.y-10;LN034.y=0;}
    if(LN034.sy<1){LN034.sy=1;LN034.y=0;}ListLN034(0,0);
    break;
    case kPGDN:if(LN_NM[1]){InputCharE();LN_NM[1]=0;}if(LN034.sy+LN034.y>=LN034.ty)break;
    if(LN034.ty<=10)LN034.sy=LN034.ty;
    else{if(LN034.y<=LN034.ty-20)LN034.y+=10;
      else{LN034.sy+=20-(LN034.ty-LN034.y);if(LN034.sy>10)LN034.sy=10;
      LN034.y=LN034.ty-10;}
    }ListLN034(0,0);
    break;
    case kUP:if(LN_NM[1]){InputCharE();LN_NM[1]=0;}if(LN034.sy+LN034.y==1)break;
    if(LN034.sy>1){ListLN034(LN034.sx,LN034.sy--);ListLN034(LN034.sx,LN034.sy);}
    else{LN034.y--;ListLN034(0,0);}
    break;
    case kDN:if(LN_NM[1]){InputCharE();LN_NM[1]=0;}if(LN034.sy+LN034.y>=LN034.ty)break;
    if(LN034.sy<10){ListLN034(LN034.sx,LN034.sy++);ListLN034(LN034.sx,LN034.sy);}
    else{LN034.y++;ListLN034(0,0);}
    break;
    case kLF:if(LN_NM[1]){if(LN_NM[1]>1)LN_NM[1]--;
    ListLN034(LN034.sx,LN034.sy);break;}
    if(LN034.sy+LN034.y==1&&LN034.sx==1)break;
    ListLN034(LN034.sx--,LN034.sy);if(!LN034.sx){LN034.sx=4;LN034.sy--;}
    if(!LN034.sy){LN034.sy=1;LN034.y--;ListLN034(0,0);}
    else ListLN034(LN034.sx,LN034.sy);break;
    case kRT:if(LN_NM[1]){if(LN_NM[1]<=strlen(NM_Str)&&LN_NM[1]<LN_NM[0])LN_NM[1]++;
    ListLN034(LN034.sx,LN034.sy);break;}
    if(LN034.sy+LN034.y>=LN034.ty&&LN034.sx==4)break;
    ListLN034(LN034.sx++,LN034.sy);if(LN034.sx>4){LN034.sx=1;LN034.sy++;}
    if(LN034.sy>10){LN034.sy=10;LN034.y++;ListLN034(0,0);}
    else ListLN034(LN034.sx,LN034.sy);break;
    case kENT:if(!LN_NM[1]){LN_NM[1]=1;ListLN034(LN034.sx,LN034.sy);}
    else{
      InputCharE();LN_NM[1]=0;
      if(LN034.sy+LN034.y>=LN034.ty&&LN034.sx==4){ListLN034(LN034.sx,LN034.sy);break;}
      ListLN034(LN034.sx++,LN034.sy);if(LN034.sx>4){LN034.sx=1;LN034.sy++;}
      if(LN034.sy>10){LN034.sy=10;LN034.y++;ListLN034(0,0);}
      else ListLN034(LN034.sx,LN034.sy);
/*    if(!LN033K)ListLN034(0,0);*/LN_NM[1]=0;
    }break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '.':
    case '+':
    case '=':
    case '-':
    case kBS:
    case kDEL:
    InputChar(k);ListLN034(LN034.sx,LN034.sy);
    break;
  }
}

void Key84(int k){
  register i,j,ii,i0;
  double d;
  long l;
  i=LN04K;j=0;
  ii=atoi(LN04.str1);i0=ii-1;i0*=48;
  switch(i){
    case  0:NM_Str=LN04A+i0+0;LN_NM[2]=LN04.st0;j++;break;
    case  1:NM_Str=LN04.str1;LN_NM[2]=LN04.st1;j++;break;
    case  2:NM_Str=LN04A+i0+12;LN_NM[2]=LN04.st2;j++;break;
    case  3:NM_Str=LN04A+i0+24;LN_NM[2]=LN04.st3;j++;break;
    case  4:NM_Str=LN04A+i0+36;LN_NM[2]=LN04.st4;j++;break;
  }
  switch(k){
    case kPGUP:if(LN_NM[1]){InputCharE();}LN_NM[1]=0;
      if(ii--<=1)break;
      itoa(ii,LN04.str1,10);
      ListLN04(1);
    break;
    case kPGDN:if(LN_NM[1]){InputCharE();}LN_NM[1]=0;
      if(ii++>=LN041.tn)break;
      itoa(ii,LN04.str1,10);
      ListLN04(1);
    break;
    case kUP:if(LN_NM[1]){InputCharE();
      switch(i){
        case 1:if(atoi(LN04.str1)>LN041.tn)itoa(LN041.tn,LN04.str1,10);
          ListLN04(1);break;
        case 2:d=CNC_atof(NM_Str);l=d*1000;if(l<LN041.x0)LN041.x0=l;
          if(l>LN041.x1)LN041.x1=l;MakeLN2();ListLN041(0);break;
        case 3:d=CNC_atof(NM_Str);l=d*1000;if(l<LN041.y0)LN041.y0=l;
          if(l>LN041.y1)LN041.y1=l;MakeLN2();ListLN041(0);break;
      }
    }LN04K--;ListLN04(i+2);
      do{i--;if(i<0)i=__LN04K__-1;}while(LN04S[i]);
      LN04K=i;LN_NM[1]=0;
    break;
    case kDN:if(LN_NM[1]){InputCharE();
      switch(i){
        case 1:if(atoi(LN04.str1)>LN041.tn)itoa(LN041.tn,LN04.str1,10);
          ListLN04(1);break;
        case 2:d=CNC_atof(NM_Str);l=d*1000;if(l<LN041.x0)LN041.x0=l;
          if(l>LN041.x1)LN041.x1=l;MakeLN2();ListLN041(0);break;
        case 3:d=CNC_atof(NM_Str);l=d*1000;if(l<LN041.y0)LN041.y0=l;
          if(l>LN041.y1)LN041.y1=l;MakeLN2();ListLN041(0);break;
      }
    }LN04K++;ListLN04(i+2);
      do{i++;if(i>=__LN04K__)i=0;}while(LN04S[i]);
      LN04K=i;LN_NM[1]=0;
    break;
    case kLF:
/*      switch(i){
    case  0:
    case  1:
    case  2:
    case  3:
    case  4:break;
      }*/
      if(j)
    if(!LN_NM[1])LN_NM[1]=1;else if(LN_NM[1]>1)LN_NM[1]--;
    break;
    case kRT:
/*      switch(i){
    case  0:
    case  1:
    case  2:
    case  3:
    case  4:break;
      }*/
      if(j)
    if(LN_NM[1]<=strlen(NM_Str)&&LN_NM[1]<LN_NM[0])LN_NM[1]++;
    break;
    case kENT:
/*      switch(i){
    case  0:
    case  1:
    case  2:
    case  3:
    case  4:break;
      }*/
      if(j){
    if(!LN_NM[1])LN_NM[1]=1;
    else{
      InputCharE();LN_NM[1]=0;
      switch(i){
        case 1:
        if(atoi(LN04.str1)>LN041.tn)itoa(LN041.tn,LN04.str1,10);
          ListLN04(1);break;
        case 2:d=CNC_atof(NM_Str);l=d*1000;if(l<LN041.x0)LN041.x0=l;
          if(l>LN041.x1)LN041.x1=l;MakeLN2();ListLN041(0);break;
        case 3:d=CNC_atof(NM_Str);l=d*1000;if(l<LN041.y0)LN041.y0=l;
          if(l>LN041.y1)LN041.y1=l;MakeLN2();ListLN041(0);break;
      }
      LN04K++;ListLN04(i+2);
      do{i++;if(i>=__LN04K__)i=0;}while(LN04S[i]);
      LN04K=i;
    }
      }
    break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '.':
    case '+':
    case '=':
    case '-':
    case kBS:
    case kDEL:
    InputChar(k);
    break;
  }
  ListLN04(i+2);
}
void Key850(int k){
  register i;i=LN05K+1;
  switch(k){
    case kLF:if(!LN05K)LN05K=__LN05K__;LN05K--;ListLN05(i);ListLN05(LN05K+1);break;
    case kRT:LN05K++;if(LN05K>=__LN05K__)LN05K=0;ListLN05(i);ListLN05(LN05K+1);break;
    case kUP:if(LN05K<20)LN05K+=(__LN05K__-__LN05K__%20);else LN05K-=20;if(LN05K>=__LN05K__)LN05K-=20;ListLN05(i);ListLN05(LN05K+1);break;
    case kDN:LN05K+=20;if(LN05K>=__LN05K__)LN05K%=20;ListLN05(i);ListLN05(LN05K+1);break;
/*    case kENT:MakeLN6();break;*/
  }
}
void Key85(int k){
  register i;
  Key850(k);
  if(k==kENT){if(HM[0]==4){MakeLN6();return;}
    if(HM[0]==2||HM[0]==3||HM[0]==6){
      if(HM[0]==3&&Ks5){
    switch(Ks5){
      case 1:FileBackup();Key5a(0);break;
      case 2:FileRestore();Key5a(0);break;
      case 3:FileErase();Key5a(0);break;
      case 4:LoadFile();Ks5=0;save=1;HF1[HF[HM[0]]][3]=0;ListHF0(HM[0],4);break;
      case 5:SaveFile();Ks5=0;save=1;HF1[HF[HM[0]]][4]=0;ListHF0(HM[0],5);break;
    }
      }
      if(HM[0]==6&&Ks6){
    switch(Ks6){
      case 3:procon.name=LN05K;LoadProCon();Key6a(0);break;
    }
      }
      switch(Ks4){
        case 6:InsertFile(0);Key4a(0);Ks4=0;break;
      }
       return;
    }
    if(HM[0]==5){
      LoadFile();for(i=0;i<11;i++)HF1[HF[5]][i]&=0xfd;
      ListHF(HF[5],0);
      Ks3=0;
    }
  }
}
int KeyBoardPress(void){
    register unsigned i,j;
    struct REGPACK iintr;
    iintr.r_ax=0x1100;
    intr(0x16,&iintr);
    i=iintr.r_ax;
    j=i&0xff;
    if(!(iintr.r_flags&0x40)){
        if(bioskey(1))bioskey(0x10);
        if(!j)return i;
        if(j==0xe0&&i>>8){i&=0xff00;return i;}
        if(j>='0'&&j<='9'&&(i>>8)>0x40)return i;
        i&=0xff;return i;
    }
    else return 0;
}
void InputChar(k){
  register i,j;
  char str[16];
  if((Ks8!=1||(HF[HM[0]]!=8))&&(HF[HM[0]])!=6)
  LN_NM[0]=LNNM();
  strncpy(str,NM_Str,15);i=LN_NM[1];
  switch(k){
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      if(i){
    if(i==1&&(str[0]=='+'||str[0]=='-'))i++;
    strcpy(str+i,NM_Str+i-1);
    str[i-1]=k;if(i<=LN_NM[0])i++;
      }else{str[i++]=k;str[i++]=0;}
    break;
    case '.':
      if(i){
    if(i==1&&(str[0]=='+'||str[0]=='-'))i++;
    if(strchr(str,k))break;
    if(i==2&&(str[0]=='+'||str[0]=='-')||i==1){
      strcpy(str+i,NM_Str+i-1);
      str[i-1]='0';if(i<=LN_NM[0])i++;
      strcpy(NM_Str,str);
    }
    strcpy(str+i,NM_Str+i-1);
    str[i-1]=k;if(i<=LN_NM[0])i++;
      }else{str[i++]='0';str[i++]=k;str[i++]=0;}
    break;
    case '=':
    case '+':
    case '-':
      if(k=='=')k='+';
      if(i){
    if(str[0]!='+'&&str[0]!='-'){
      strcpy(str+1,NM_Str);
      if(i<=LN_NM[0])i++;
    }str[0]=k;
      }else{str[i++]=k;str[i++]=0;}
    break;
    case kBS:
      if(i==1)break;
      if(i){i--;strcpy(str+i-1,NM_Str+i);}
      else str[i++]=0;
    break;
    case kDEL:
      if(i){
      if(str[i-1])strcpy(str+i-1,NM_Str+i);
      }else str[i++]=0;
    break;
  }LN_NM[1]=i;str[LN_NM[0]]=0;strcpy(NM_Str,str);
}
char LNNM(void){
  register i;
  switch(LN_NM[2]){
    case 8: /*0.000~9.999*/
    case 11:
      i=7;break;
    case 14:    /*1~200*/
    case 15:    /*0~99*/
    case 17:    /*54~959*/
    case 21:
      i=3;break;
    case 16:    /*1~4*/
      i=1;break;
  default:i=9; break;
  }return i;
}
void InputCharE(){
  long l;
  register i,j;
  float f;
  char str[16];
  if(!LN_NM[1])return;
  f=CNC_atof(NM_Str);
  switch(LN_NM[2]){
    case 0: /*-9999.999~+9999.999*/
      f*=1000;if(f>9999999)f=9999999;if(f<-9999999)f=-9999999;
      if(f<0)NM_Str[0]='-';else NM_Str[0]='+';
      l=labs(f);i=l/1000;itoa(i,NM_Str+1,10);i=(long)l%1000;
      str[0]='.';str[1]=i/100+'0';i%=100;str[2]=i/10+'0';str[3]=i%10+'0';
      str[4]=0;strcat(NM_Str,str);
    break;
    case 1: /*0.000~+9999.999*/
      f*=1000;if(f>9999999)f=9999999;if(f<0)f=0;
      NM_Str[0]='+';
      l=labs(f);i=l/1000;itoa(i,NM_Str+1,10);i=(long)l%1000;
      str[0]='.';str[1]=i/100+'0';i%=100;str[2]=i/10+'0';str[3]=i%10+'0';
      str[4]=0;strcat(NM_Str,str);
    break;
    case 2: /*0.000~9999.999*/
      f*=1000;if(f>9999999)f=9999999;if(f<0)f=0;
      l=labs(f);i=l/1000;itoa(i,NM_Str,10);i=(long)l%1000;
      str[0]='.';str[1]=i/100+'0';i%=100;str[2]=i/10+'0';str[3]=i%10+'0';
      str[4]=0;strcat(NM_Str,str);
    break;
    case 3: /*-99.999~+99.999/
      f*=1000;if(f>99999)f=99999;if(f<-99999)f=-99999;
      if(f<0)NM_Str[0]='-';else NM_Str[0]='+';
      l=labs(f);i=l/1000;itoa(i,NM_Str+1,10);i=(long)l%1000;
      str[0]='.';str[1]=i/100+'0';i%=100;str[2]=i/10+'0';str[3]=i%10+'0';
      str[4]=0;strcat(NM_Str,str);
    break;
    case 4: /*0.000~+99.999*/
      f*=1000;if(f>99999)f=99999;if(f<0)f=0;
      NM_Str[0]='+';
      l=labs(f);i=l/1000;itoa(i,NM_Str+1,10);i=(long)l%1000;
      str[0]='.';str[1]=i/100+'0';i%=100;str[2]=i/10+'0';str[3]=i%10+'0';
      str[4]=0;strcat(NM_Str,str);
    break;
    case 5: /*0.000~99.999*/
      f*=1000;if(f>99999)f=99999;if(f<0)f=0;
      l=labs(f);i=l/1000;itoa(i,NM_Str,10);i=(long)l%1000;
      str[0]='.';str[1]=i/100+'0';i%=100;str[2]=i/10+'0';str[3]=i%10+'0';
      str[4]=0;strcat(NM_Str,str);
    break;
    case 6: /*-9.999~+9.999/
      f*=1000;if(f>9999)f=9999;if(f<-9999)f=-9999;
      if(f<0)NM_Str[0]='-';else NM_Str[0]='+';
      l=labs(f);i=l/1000;itoa(i,NM_Str+1,10);i=(long)l%1000;
      str[0]='.';str[1]=i/100+'0';i%=100;str[2]=i/10+'0';str[3]=i%10+'0';
      str[4]=0;strcat(NM_Str,str);
    break;
    case 7: /*0.000~+9.999*/
      f*=1000;if(f>9999)f=9999;if(f<0)f=0;
      NM_Str[0]='+';
      l=labs(f);i=l/1000;itoa(i,NM_Str+1,10);i=(long)l%1000;
      str[0]='.';str[1]=i/100+'0';i%=100;str[2]=i/10+'0';str[3]=i%10+'0';
      str[4]=0;strcat(NM_Str,str);
    break;
    case 8: /*0.000~9.999*/
      f*=1000;if(f>9999)f=9999;if(f<0)f=0;
      l=labs(f);i=l/1000;itoa(i,NM_Str,10);i=(long)l%1000;
      str[0]='.';str[1]=i/100+'0';i%=100;str[2]=i/10+'0';str[3]=i%10+'0';
      str[4]=0;strcat(NM_Str,str);
    break;
    case 9: /*-0.999~+0.999/
      f*=1000;if(f>999)f=999;if(f<-999)f=-999;
      if(f<0)NM_Str[0]='-';else NM_Str[0]='+';
      l=labs(f);i=l/1000;itoa(i,NM_Str+1,10);i=(long)l%1000;
      str[0]='.';str[1]=i/100+'0';i%=100;str[2]=i/10+'0';str[3]=i%10+'0';
      str[4]=0;strcat(NM_Str,str);
    break;
    case 10:    /*0.000~+0.999*/
      f*=1000;if(f>999)f=999;if(f<0)f=0;
      NM_Str[0]='+';
      l=labs(f);i=l/1000;itoa(i,NM_Str+1,10);i=(long)l%1000;
      str[0]='.';str[1]=i/100+'0';i%=100;str[2]=i/10+'0';str[3]=i%10+'0';
      str[4]=0;strcat(NM_Str,str);
    break;
    case 11:    /*0.000~0.999*/
      f*=1000;if(f>999)f=999;if(f<0)f=0;
      l=labs(f);i=l/1000;itoa(i,NM_Str,10);i=(long)l%1000;
      str[0]='.';str[1]=i/100+'0';i%=100;str[2]=i/10+'0';str[3]=i%10+'0';
      str[4]=0;strcat(NM_Str,str);
    break;
    case 12:    /*1~999999*/
      if(f>999999)f=999999;if(f<1)f=1;
      l=labs(f);ltoa(l,NM_Str,10);
    break;
    case 13:    /*1.0~500.0*/
      f*=10;if(f>5000)f=5000;if(f<10)f=10;
      l=labs(f);i=l/10;itoa(i,NM_Str,10);i=(long)l%10;
      str[0]='.';itoa(i,str+1,10);strcat(NM_Str,str);
    break;
    case 14:    /*1~200*/
      if(f>200)f=200;if(f<1)f=1;
      l=labs(f);ltoa(l,NM_Str,10);
    break;
    case 15:    /*0~99*/
      if(f>99)f=99;if(f<0)f=0;
      l=labs(f);ltoa(l,NM_Str,10);
    break;
    case 16:    /*1~4*/
      if(f>4)f=4;if(f<1)f=1;
      l=labs(f);ltoa(l,NM_Str,10);
    break;
    case 17:    /*54~954*/
      l=f;
      for(i=0;i<60;i++)if(l==G92S[i])break;
      if(i>=60)l=G92S[0];
      itoa(l,NM_Str,10);
    break;
    case 18:    /*0.001~9999.999*/
      f*=1000;if(f>9999999)f=9999999;if(f<1)f=1;
      l=labs(f);i=l/1000;itoa(i,NM_Str,10);i=(long)l%1000;
      str[0]='.';str[1]=i/100+'0';i%=100;str[2]=i/10+'0';str[3]=i%10+'0';
      str[4]=0;strcat(NM_Str,str);
    break;
    case 20:    /*00(0..)~99(0..)*/
      l=f;j=LN_NM[0];NM_Str[j]=0;
      for(i=0;i<j;i++){NM_Str[j-i-1]=l%10+'0';l/=10;}
    break;
    case 21:    /*0.0~MAXDL*/
      f*=10;if(f>MAXDL*10)f=MAXDL*10;if(f<0)f=0;
      l=labs(f);i=l/10;NM_Str[0]='0'+i/100;i%=100;NM_Str[1]='0'+i/10;
      NM_Str[2]='0'+i%10;NM_Str[3]='.';i=(long)l%10;i/=5;i*=5;NM_Str[4]='0'+i;
      NM_Str[5]=0;
    break;
    case 22:    /*+~-*/
      if(NM_Str[0]!='-')NM_Str[0]='+';
    break;
    case 23: /*1.0-1000.0*/
      f*=10;if(f>10000)f=10000;if(f<10)f=10;
      l=labs(f);i=l/10;itoa(i,NM_Str,10);i=(long)l%10;
      str[0]='.';itoa(i,str+1,10);strcat(NM_Str,str);
    break;
    case 24:/*5.0 - 1000.0*/
      f*=10;if(f>10000)f=10000;if(f<50)f=50;
      l=labs(f);i=l/10;itoa(i,NM_Str,10);i=(long)l%10;
      str[0]='.';itoa(i,str+1,10);strcat(NM_Str,str);
    break;
  }
  LN_NM[1]=0;
}

