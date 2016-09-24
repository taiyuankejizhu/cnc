#include "motion.h"
#include "znc.h"
void Set_lengthXYZ(){ /*设置输入范围*/
  MaxL=7;
  MaxN=9999999;
  MinN=-9999999;
}
void Set_lengthAI(){
  MaxL=3;
  MaxN=999;
  MinN=-999;
}
void Modify_XYZ(char k){    /*修改XYZ轴坐标*/
     long l;
     int i;
     if(!(k&0x80))return;
     CNC_Getinput();
     k&=0x7f;
     if(k==0)
       {
          if(KeyN!=0x7fffffff){KeyN/=DZC;l=KeyN-XPosi(0);position_xdis=KeyN;
          offset_x=l;SaveXYZ(0);position_px=position_x;}
          ListXYZ00(1);return;
       }
     else if(k==1)
       {
          if(KeyN!=0x7fffffff){KeyN/=DZC;l=KeyN-YPosi(0);position_ydis=KeyN;
          SaveXYZ(0);position_py=position_y;offset_y=l;}
          ListXYZ00(1);return;
       }
     else if(k==2)
       {
          if(KeyN!=0x7fffffff){
          KeyN/=DZC;l=KeyN-ZPosi(0);
          if(Dis_flag)position_d-=offset_z-l;
          else position_d=KeyN;
          offset_z=l;
          if(Dis_flag){i=F12_flag;F12_flag=9;
           /* if(!K[8])F12();else F13();  */
            F12_flag=i;}
                  SaveXYZ(0);
           }
          ListXYZ00(1);
          return;
       }
}
void CNC_Showinput(char value){ /*显示输入信息，只对数字*/
    int i;
    int j;
    static int k=0;
    char a[]="\0\0\0\0";
    char temp[10];
    if(MaxL==-1)return;
    if(MaxL>3)j=MaxL-3;
    else j=MaxL;
    a[0]=value;
    if(!MaxL&&Inputstr[0]!='-')Inputstr[0]='+';
    switch(value){
        case '=':
        case '+':
            Inputstr[0]='+';break;
        case '-':
            Inputstr[0]='-';break;
        case 0:
            Inputstr[0]=' ';Inputstr[1]=0;Inputstr1[1]=0;k=0;MaxL=-1;KeyN=0x7fffffff;
            ListCRT1(" ");
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
                /*return;*/}
        if(!k){strcat(Inputstr,a);
            if(strlen(Inputstr+1)>j)
            strcpy(Inputstr+1,Inputstr+2);}
        else{
            strcat(Inputstr1,a);
            if(strlen(Inputstr1)>4)
            strcpy(Inputstr1+1,Inputstr1+2);}
    }
    strcpy(temp,Inputstr);
    if(k){
          for(i=0;i<10;i++)
          temp[i]='\0';
          strcpy(temp,Inputstr);strcat(temp,Inputstr1);
         }
     ListCRT1(temp);
}
long CNC_Getinput(void){      /*取得输入信息，转到KeyN变量中*/
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
void Hand_MoveXYZ(char k,long p){   /*定点跑*/
    long ol;
    k&=0x7f;
    if(Dis_flag){ ListMess1("无效操作");return;}
    if(k==0)
      {
         set_PIDX(10,0,4,0x10);
         ol=p-offset_x;
         lock_x=0;
         Position_control_x(ol);
      }
    else if(k==1)
      {
         set_PIDY(10,0,4,0x10);
         ol=p-offset_y;
         lock_y=0;
         Position_control_y(ol);
      }
    else if(k==2)
      {
         set_PIDZ(10,0,4,0x10);
         ol=p-offset_z;
         lock_z=0;
         Position_control_z(ol);
      }
}
void ZeroX_positive(void){   /*X+归零*/
  if(Dis_flag){ListMess1("无效操作");return;}
  K[8]=1;
  StopFx();
}
void ZeroX_negative(void){   /*X-归零*/
  if(Dis_flag){ListMess1("无效操作");return;}
  K[8]=0;
  StopFx();
}
void StopFx(void){
  Stop_x(0);
  ZeroX(1);
  ShortSound();
}
void ZeroX(char k){
    static char byte_2159=0,byte_215A=0;
    lock_x=0;
    if(k==0){byte_2159=0;byte_215A=0;return;}
    offset_ds=0;ZeroFlagX=0;/*X轴需要重新定义*/
  if(byte_2159==0)
    {
       if(!K[8])
        {
          if(Voltage()<6){
               Velocity_control_x(0x10);
               byte_215A=1;
               delay(0x180);
               Velocity_control_x(0xFFFf);
           }
         else 
           Velocity_control_x(0xFFFe);
            }
      else
        {
           if(Voltage()<6)
             {
                 Velocity_control_x(0xffe0);
                byte_215A=1;
                delay(0x180);
                Velocity_control_x(0x1);
             }
             else 
            Velocity_control_x(0x2);
        }
      byte_2159=1;
    }
  else
    {
      if(byte_215A==0){
            if(Voltage()<6){
              if(!K[8]){
              Velocity_control_x(0x10);
              byte_215A=1;
              delay(0x180);
              Velocity_control_x(0xFFFf);
           }
         else
             {
              Velocity_control_x(0xffe0);
              byte_215A=1;
              delay(0x180);
              Velocity_control_x(0x1);
           }
      }
    }else
    if(Voltage()<=1){
      ZeroFlagX=1;
      StopFx();
      SoundTime=0x7f;
      byte_2159=0;
      byte_215A=0;
      XYPosition();
      position_x=position_x;
      offset_d=position_x-offset_x;  /*X轴需要重新定义*/
      set_PIDX(0x14,0,1,0x20);
      ZeroFlagX=1; /*X轴需要重新定义*/
    }
  }
}
void ZeroY_positive(void){ /*Y+归零*/
  if(Dis_flag){ListMess1("无效操作");return;}
  K[8]=1;
  StopFy();
}
void ZeroY_negative(void){ /*Y-归零*/
  if(Dis_flag){ListMess1("无效操作");return;}
  K[8]=0;
  StopFy();
}
void StopFy(void){
  Stop_y(0);
  ZeroY(1);
  ShortSound();
}
void ZeroY(char k){
    static char byte_2159=0,byte_215A=0;
    if(k==0){byte_2159=0;byte_215A=0;return;}
    lock_y=0;
    offset_ds=0;ZeroFlagY=0;/*Y轴需要重新定义*/
  if(byte_2159==0)
    {
       if(!K[8])
        {
          if(Voltage()<6){
               Velocity_control_y(0x10);
               byte_215A=1;
               delay(0x180);
               Velocity_control_y(0xFFFf);
           }
         else 
               Velocity_control_y(0xFFFe);
            }
      else
        {
           if(Voltage()<6)
             {
                Velocity_control_y(0xffe0);
                byte_215A=1;
                delay(0x180);
                Velocity_control_y(0x1);
             }
             else 
            Velocity_control_y(0x2);
        }
      byte_2159=1;
    }
  else
    {
      if(byte_215A==0){
            if(Voltage()<6){
              if(!K[8]){
              Velocity_control_y(0x10);
              byte_215A=1;
              delay(0x180);
              Velocity_control_y(0xFFFf);
           }
         else
             {
              Velocity_control_y(0xffe0);
              byte_215A=1;
              delay(0x180);
              Velocity_control_y(0x1);
           }
      }
    }else
    if(Voltage()<=1){
      ZeroFlagY= 1;
      StopFy();
      SoundTime=0x7f;
      byte_2159=0;
      byte_215A=0;
      XYPosition();
      position_y=position_y;
      offset_d=position_y-offset_y;  /*Y轴需要重新定义*/
      set_PIDY(0x14,0,1,0x20);
      ZeroFlagY=1; /*Y轴需要重新定义*/
    }
  }
}
void ZeroZ_positive(void){  /*Z+归零*/
  if(Dis_flag){ListMess1("无效操作");return;}
  K[8]=1;
  StopFz();
}
void ZeroZ_negative(void){  /*Z-归零*/
  if(Dis_flag){ListMess1("无效操作");return;}
  K[8]=0;
  StopFz();
}
void StopFz(void){
  Stop_z(0);
  ZeroZ(1);
  ShortSound();
}
void ZeroZ(char k){
    static char byte_2159=0,byte_215A=0;
    if(k==0){byte_2159=0;byte_215A=0;return;}
    lock_z=0;
    offset_ds=0;ZeroFlag=0;
  if(byte_2159==0)
    {
       if(!K[8])
        {
          if(Voltage()<6){
               Velocity_control_z(0x10);
               byte_215A=1;
               delay(0x180);
               Velocity_control_z(0xFFFf);
           }
         else 
           Velocity_control_z(0xFFFe);
            }
      else
        {
           if(Voltage()<6)
             {
                 Velocity_control_z(0xffe0);
                byte_215A=1;
                delay(0x180);
                Velocity_control_z(0x1);
             }
             else 
            Velocity_control_z(0x2);
        }
      byte_2159=1;
    }
  else
    {
      if(byte_215A==0){
            if(Voltage()<6){
              if(!K[8]){
              Velocity_control_z(0x10);
              byte_215A=1;
              delay(0x180);
              Velocity_control_z(0xFFFf);
           }
         else
             {
              Velocity_control_z(0xffe0);
              byte_215A=1;
              delay(0x180);
              Velocity_control_z(0x1);
           }
      }
    }else
    if(Voltage()<=1){
      ZeroFlag=1;
      StopFz();
      SoundTime=0x7f;
      byte_2159=0;
      byte_215A=0;
      XYPosition();
      position_z=position_z;
      offset_d=position_z-offset_z;
      set_PIDZ(0x14,0,1,0x20);
      ZeroFlag=1;
    }
  }
}
void Stop_xyz(void){
Stop_x(0);
Stop_y(0);
Stop_z(0);
}
void Modify_Set(char k){    /*设定*/
     if(!(k&0x80))return;
     CNC_Getinput();
     k&=0x7f;
     if(0==k)
       {
         if(KeyN<1000&&KeyN>=0)AI_time=KeyN;else AI_time=0;
         if(!P[7]||!AI_time)HF1[12][0]=0;
         ShowFlag(7);
       }
     else if(1==k)
       {
         if(KeyN<1000&&KeyN>=0)AI_high=KeyN;else AI_high=0;
         if(!P[7]||!AI_high)HF1[12][1]=0;
         ShowFlag(8);
       }
     else if(2==k)
       {
         P[7]++;
         if(P[7]>2)
           P[7]=0;
         if(!P[7])
           {HF1[12][2]=0;}
         else
           {
             HF1[12][2]=1;
             if(AI_high)HF1[12][1]=1;
             else HF1[12][1]=0;
             if(AI_time)HF1[12][0]=1;
             else HF1[12][0]=0;
           }
         ShowFlag(7);
         ShowFlag(8);
         ShowFlag(9);
       }
     else if(3==k)
       {
         if(P[3]){HF1[12][3]=0;P[3]=0;}
         else {HF1[12][3]=1;P[3]=1;}
         ShowFlag(10);
       }
     else if(4==k)
       {
          if(!P[8]&&KeyN<1000&&KeyN>0){P[8]=1;Steel_flag=KeyN;}
          else {P[8]=0;Steel_flag=0;}
          ShowFlag(11);
       }
}
