#include "znc.h"
#include <dos.h>
#include <conio.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
long XPos[1000];
long YPos[1000];
extern char RunCodeGroups[1000];
extern char RunCodeLines[1000];
extern char M9899;
extern char RunCodeLine;/*当前代码行*/
extern char G0203;
extern char offsetxyz;  /*0机床,1当前坐标*/
extern char CodeAbs;    /*0绝对,1相对坐标编程*/
extern char RunCodeLine;/*当前代码行*/
extern char RunCodeGroup;/*当前代码组*/
extern char MXYZ;       /*平面选择*/
extern struct Dis_position{
    char XYZ;   /*X:0 Y:1 Z:2*/
    char Position_S;
    long Position_H;
    long Position_E;
    long Position_V;
}DIS;
extern struct Run_Pi{
    int PE;
    int PV;
    int PT;
    char XYZ[1000];
    long XYZV0[1000];
    long XYZV1[1000];
}RunPi;
void DelCodeLine(void);
void CheckCode(void);
void CtoCode(void);
void CodetoC(void);
void C1(void);      /*Code Group*/
char Queue_int(Queue *que)
{
  que->front=NULL;
  que->rear=NULL;
  que->item=0;
  que->qsize=1000;
  return 1;
}
char Queue_empty(Queue *que)
{
    if(0==que->item)
      return 1;
    else
      return 0;
}
char Queue_isfull(Queue *que)
{
    if(que->qsize==que->item)
      return 1;
    else
      return 0;
}
int Queue_count(Queue *que)
{
   return que->item;
}
char Queue_en(Queue *que,QNode *node)
{
   QNode *temp;
   if(Queue_empty(que))
     {
       que->front=node;
       que->rear=node;
       que->item++;
     }
   else if(Queue_isfull(que))
     {
        temp=que->front;
        que->front=que->front->next;
        que->rear->next=node;
        node->prior=que->rear;
        que->rear=node;
        que->front->prior=NULL;
        node->next = NULL;
        free(temp);
     }
   else
     {
        que->rear->next=node;
        node->prior=que->rear;
        que->rear=que->rear->next;
        que->item++;
     }
   return 1;
}
char Queue_del(Queue *que)
{
   QNode *temp;
   if(Queue_empty(que))
     return 0;
   else if(1==que->item)
     {
       free(que->front);
       que->front=NULL;
       que->rear=NULL;
       que->item--;
       return 1;
     }
   else
     {
       temp=que->rear->prior;
       que->rear=temp;
       temp=temp->next;
       que->rear->next=NULL;
       que->item--;
       free(temp);
       return 1;
     }
}
char Queue_edel(Queue *que)/*清空队列*/
{
  while(!Queue_empty)
  {
     Queue_del(que);
  }
  Queue_int(que);
  return 0;
}
QNode *Qnode_int(char xyz,char MXYZ,long x0,long y0,long z0,long x1,long y1,long z1,QNode *node)/*申请一个新结点*/
{
   node=(QNode *)malloc(sizeof(QNode));
   node->xyz=xyz;
   node->MXYZ=MXYZ;
   node->x0=x0;
   node->y0=y0;
   node->z0=z0;
   node->x1=x1;
   node->y1=y1;
   node->z1=z1;
   node->next=NULL;
   node->prior=NULL;
   return node;
}
char Fij(double R,long xi, long yi,long ox,long oy)
{   
    if(((xi-ox)*(xi-ox)+(yi-oy)*(yi-oy)-R*R)>=0)
        return 1;   
    else   
        return 0;   
}
void Write_targetarc(long x,long y,long ix,long jy)
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

    outb(ICXaddr,0x61);
    outb((ICXaddr+1),ix&0xff);
    outb(ICXaddr,0x62);
    outb((ICXaddr+1),((ix>>8)&0xff));
    outb(ICXaddr,0x63);
    outb((ICXaddr+1),((ix>>16)&0xff));

    outb(ICYaddr,0x61);
    outb((ICYaddr+1),jy&0xff);
    outb(ICYaddr,0x62);
    outb((ICYaddr+1),((jy>>8)&0xff));
    outb(ICYaddr,0x63);
    outb((ICYaddr+1),((jy>>16)&0xff));

}

void Discharge1(long x1,long y1)
{
    unsigned int inter = 0,vx,vy,vz;
    double d =100.0;
    char a;
    inter &= 0xc0;

   if(MXYZ==0){inter |= 0x34;}
   else if(MXYZ==1){inter |= 0x32;}
   else  if(MXYZ==2){inter |= 0x39;}

    vx = (Table.Sudu[Dis_lines]-1)&0x7;
    vy = (Table.Sudu[Dis_lines]-1)&0x7;
    vz = (Table.Sudu[Dis_lines]-1)&0x7;

    OTPBX1&=0xf0;
    outb((ICXaddr),0x14);
    outb((ICXaddr+1),OTPBX1);
    outb(ICXaddr,0x51);
    outb(ICXaddr+1,(unsigned )vx&0xff);
    outb(ICXaddr,0x52);
    outb(ICXaddr+1,(unsigned)vx>>8);
    OTPBX1|=0xe;
    outb((ICXaddr),0x14);
    outb((ICXaddr+1),OTPBX1);

    OTPBY1&=0xf0;
    outb((ICYaddr),0x14);
    outb((ICYaddr+1),OTPBY1);
    outb(ICYaddr,0x51);
    outb(ICYaddr+1,(unsigned)vy&0xff);
    outb(ICYaddr,0x52);
    outb(ICYaddr+1,(unsigned)vy>>8);
    outb((ICYaddr),0x53);
    OTPBY1|=0xe;
    outb((ICYaddr),0x14);
    outb((ICYaddr+1),OTPBY1);

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

    outb((Inter),0x13);
    outb((Inter+1),inter);
}  
RunArc(){
  double d0,d1,d2;
  static double R;/*半径*/
  static long x0,y0;
  static long cX,cY,cZ,cI,cJ,cK;
  static char Mx,My,Mz;
  static unsigned char xyz,End_flagadl=0;
  long x_disten,y_disten;
  int i,Axis_flag;
  static int ks = 0;
  FILE *fp;
  switch(RunLineFlag){
    case 0:
      if(MXYZ==0)
      {
        Mx=0;My=1;Mz=2;cX=cncode.X;cY=cncode.Y;cZ=cncode.Z;cI=cncode.I;cJ=cncode.J;cK=cncode.K;
        x0=XPosi(1);y0=YPosi(1);
      }
      else  if(MXYZ==1)
      {
        Mx=2;My=0;Mz=1;cX=cncode.Z;cY=cncode.X;cZ=cncode.Y;cI=cncode.K;cJ=cncode.I;cK=cncode.J;
        x0=ZPosi(1);y0=XPosi(1);
      }
      else  if(MXYZ==2)
      {
        Mx=1;My=2;Mz=0;cX=cncode.Y;cY=cncode.Z;cZ=cncode.X;cI=cncode.J;cJ=cncode.K;cK=cncode.I;
        x0=YPosi(1);y0=ZPosi(1);
      }
      d0  = x0 - cI;
      d0 *= d0;
      d1  = y0 - cJ;
      d1 *= d1;
      d0 += d1; /*d0起点与圆心的距离*/
      d0 = sqrt(d0);
      d2  = cX - cI;
      d2 *= d2;
      d1  = cY - cJ;
      d1 *= d1;
      d2 += d1;
      d2 = sqrt(d2);/*d2终点与圆心的距离*/
      if(fabs(d2 - d0)>=2)
      {/*半径不相等*/
          ListMess1("起点与终点不在同一圆上!");
          RunLineFlag=12;
         if(MXYZ==0){cncode.X=x0;cncode.Y=y0;}
         else{   if(MXYZ==1){cncode.Z=x0;cncode.X=y0;}
         else  if(MXYZ==2){cncode.Y=x0;cncode.Z=y0;}
             }
         return;
      }
      R = d0;
      Read_StartEnd();
      if(!Dis_flag)
        Diss();
      DIS_enode=Qnode_int(2+G0203,MXYZ,x0,y0,cI,cX,cY,cJ,DIS_enode);
      Queue_en(&DIS_que,DIS_enode);
      Current_DIS = DIS_que.rear;
      Write_targetarc(cX - x0,cY - y0,cI - x0,cJ - y0);
      Discharge1(cX - x0,cY - y0);
      delay(0x800);
      outb((Inter),0x14);
      if(!G0203)
        outb((Inter+1),0x09);
      else
        outb((Inter+1),0x0b);
      xyz = 0;
      RunLineFlag++;
    break;
    case 1:
        if(MXYZ==0){Position_control_z(cncode.Z);}
        else if(MXYZ==1){Position_control_y(cncode.Y);}
        else if(MXYZ==2){Position_control_x(cncode.X);}
        RunLineFlag++;
        break;
    case 2:
      if(ks < 1000)
      {
        XPos[ks] = XPosi(0);
        YPos[ks] = YPosi(0);
        ks ++;
      }
      else if(ks == 1000)
      {
        fp = fopen("FILEA\\test.txt","w");
        fprintf(fp,"XPosi\tYPosi\n");
        for(i = 0;i < ks;i++)
        fprintf(fp,"%ld\t%ld\n",XPos[i],YPos[i]);
        ks++;
        ListMess1("OK");
        fclose(fp);
      }
      xyz+=Diss();
      if(xyz>49){RunLineFlag=12;}
      break;
    case 12:
      if(!PositionEndXYZ(0x84))
        {
          End_flagadl++;
          if(3==End_flagadl)
            {
              Pause();
              RunCode(0);
              End_flagadl=0;
            }
        }
      else
       {
         Position_control_x(cncode.X);
         Position_control_y(cncode.Y);
         Position_control_z(cncode.Z);
         End_flagadl=0;
       }
    break;
  }
}
RunCircle(){    /*镗圆*/
  double d0,d1;
  static long x,y,z,x0,y0,x1,y1,z1,x2,y2,z0;
  static long cX,cY,cZ;
  static char Mx,My,Mz;
  static char xyz,zs,R;
  long a=0x7fffffff;
  int i;
  switch(RunLineFlag){
    case 0:
     if(MXYZ==0){Mx=0;My=1;Mz=2;z1=ZPosi(1);cX=XPosi(1);cY=YPosi(1);cZ=cncode.Z;}
      else{   if(MXYZ==1){Mx=2;My=0;Mz=1;z1=YPosi(1);cX=ZPosi(1);cY=XPosi(1);cZ=cncode.Y;}
    else  if(MXYZ==2){Mx=1;My=2;Mz=0;z1=XPosi(1);cX=YPosi(1);cY=ZPosi(1);cZ=cncode.X;}
      }
      x1=cX;y1=cY;
      if(cZ!=a)z=cZ;else z=z1;
      Read_StartEnd();
/*      if(cncode.G<100)Read_Table(cncode.G);
      i=cncode.D;i>>=4;if(i<9)Dis_start=i;else Dis_start=0;
      i=cncode.D;i&=0xf;if(i<9)Dis_end=i;else Dis_end=Dis_start;*/
      if(z1==z)zs=0;if(z1>z)zs=1;if(z1<z)zs=-1;
/*      RunPi.PE=0;RunPi.PT=0;*/Dis_flag=0;Diss();
      if(zs){Dis_lines=Dis_start;
/*  DIS.Position_H=z1;DIS.XYZ=2;*/
    if(z1>z)SRunPi(Mz,(long)9999999,z);
    else    SRunPi(Mz,(long)-9999999,z);
    SRunDIS(0);
      }

      RunLineFlag++;
      if(!zs)RunLineFlag=4;
    break;
    case 1:
      if(Dis_lines>Dis_end){RunLineFlag=4;Dis_lines--;
    if(MXYZ==0){Position_control_z(cncode.Z);Signal_1100();}
    else{   if(MXYZ==1){Position_control_y(cncode.Y);Signal_1100Y();}
      else  if(MXYZ==2){Position_control_x(cncode.X);Signal_1100X();}
    }
      }
      else{
      if(zs==1){
    RunPi.XYZV1[RunPi.PV]=Table.Shendu[Dis_lines]-Table.Shendu[Dis_end]+z;
    SRunDIS(0);
      }
      else{
    RunPi.XYZV1[RunPi.PV]=Table.Shendu[Dis_end]-Table.Shendu[Dis_lines]+z;
    SRunDIS(0);
      }
      Dis_flag=1;xyz=0;Diss();
      RunLineFlag++;RunCircle();
      }
    break;
    case 2:
      xyz+=Diss();
      if(xyz>49){
      Dis_lines++;RunLineFlag=1;}
    break;
    case 3:
    break;
    case 4:
      if(cncode.R==0){RunLineFlag=12;break;}
      if(cncode.R<0)R=-1;else R=0;
      x=x1-cncode.R;y=y1;
      SRunPi(Mx,x1,x);
      SRunDIS(0);
      Dis_flag=1;xyz=0;Diss();
      RunLineFlag++;RunCircle();
    break;
    case 5:
      xyz+=Diss();
      if(xyz>49){RunLineFlag++;}
    break;
    case 6:
      x0=x-x1;y0=y-y1;z0=x0;x2=labs(x0);y2=labs(y0);
      if(y2==0){RunLineFlag=8;break;}
/*      if(x2==0){if(y0>0)y=cncode.Y+cncode.R;else y=cncode.Y-cncode.R;y0=y-y1;}*/
      if(x2>y2){
    if(y0>0){x0=x;if(!R&&z0<cncode.R)x++;if(R&&z0>cncode.R)x--;}
    else{x0=x;if(!R&&z0>-cncode.R)x--;if(R&&z0<-cncode.R)x++;}
    if(x0==x){RunLineFlag=8;break;}
      }
      else{
    if(x0>0||(x0==0&&(y0>0&&!R||y0<0&&R))){
      if(y0>0&&!R)y0--;if(y0<0&&R)y0++;d0=cncode.R;d0*=d0;
        d1=y0;d1*=d1;d0=d0-d1;if(d0>0)d0=sqrt(d0);else{d0=0;}
        x0=x;x=(long)(d0)+cX;}
    else{if(y0<0&&!R)y0++;if(y0>0&&R)y0--;d0=cncode.R;d0*=d0;
        d1=y0;d1*=d1;d0=d0-d1;if(d0>0)d0=sqrt(d0);else{d0=0;}
        x0=x;x=cX-(long)(d0);}
      }
      if(MXYZ==0){Position_control_y(y);Signal_1100Y();}
      else{   if(MXYZ==1){Position_control_x(y);Signal_1100X();}
    else  if(MXYZ==2){Position_control_z(y);Signal_1100();}
      }
      SRunPi(Mx,x0,x);
      SRunDIS(0);
      Dis_flag=1;xyz=0;Diss();
      RunLineFlag++;RunCircle();
      break;
    case 7:
      xyz+=Diss();
      if(xyz>49){
    RunLineFlag++;
      }
      break;
    case 8:
      x0=x-x1;y0=y-y1;z0=y0;x2=labs(x0);y2=labs(y0);
      if(x2==0){RunLineFlag=6;break;}
/*      if(y2==0){if(x0>0)x=cncode.X+cncode.R;else x=cncode.X-cncode.R;x0=x-x1;}*/
      if(x2>=y2){
    if(y0>0||(y0==0&&(x0<0&&!R||x0>0&&R))){
      if(x0<0&&!R)x0++;if(x0>0&&R)x0--;d0=cncode.R;
    d0*=d0;
        d1=x0;d1*=d1;d0=d0-d1;if(d0>0)d0=sqrt(d0);else{d0=0;}
        y0=y;y=(long)(d0)+cY;}
    else{if(x0>0&&!R)x0--;if(x0<0&&R)x0++;d0=cncode.R;d0*=d0;
        d1=x0;d1*=d1;d0=d0-d1;if(d0>0)d0=sqrt(d0);else{d0=0;}
        y0=y;y=cY-(long)d0;}
      }
      else{
    if(x0>0){y0=y;if(!R&&z0>-cncode.R)y--;if(R&&z0<-cncode.R)y++;}
    else{y0=y;if(!R&&z0<cncode.R)y++;if(R&&z0>cncode.R)y--;}
    if(y0==y){RunLineFlag=6;break;}
      }
      if(MXYZ==0){Position_control_x(x);Signal_1100X();}
      else{   if(MXYZ==1){Position_control_z(x);Signal_1100();}
    else  if(MXYZ==2){Position_control_y(x);Signal_1100Y();}
      }
      SRunPi(My,y0,y);
      SRunDIS(0);
      Dis_flag=1;xyz=0;Diss();
      RunLineFlag++;RunCircle();
      break;
    case 9:
      xyz+=Diss();
      if(xyz>49){
    if(x==cX-cncode.R&&y==cY)RunLineFlag=12;
    else    RunLineFlag=6;}
      break;
    case 12:
      if(cncode.R==0){RunLineFlag=16;break;}
      if(cncode.R<0)R=-1;else R=0;
      x=x1-cncode.R;y=y1;
      SRunPi(Mx,x,x1);
      SRunDIS(0);
      Dis_flag=1;xyz=0;Diss();
      RunLineFlag++;RunCircle();
    break;
    case 13:
      xyz+=Diss();
      if(xyz>49){RunLineFlag=16;}
    break;
    case 16:
    Position_control_x(cX);
    Position_control_y(cY);
    Position_control_z(cZ);
    Signal_1100X();
    Signal_1100Y();
    Signal_1100();
    Pause();
    RunCode(0);
    break;
  }

}
RunBar(){   /*镗方*/
  double d0,d1;
  static long x,y,z,x2,y2,x1,y1,z1;
  static long cX,cY,cZ;
  static char Mx,My,Mz;
  static char zs;
  static unsigned char xyz;
  long a=0x7fffffff;
  int i;
  switch(RunLineFlag){
    case 0:
     if(MXYZ==0)    {Mx=0;My=1;Mz=2;z1=ZPosi(1);x1=XPosi(1);y1=YPosi(1);cZ=cncode.Z;cY=cncode.Y;cX=cncode.X;}
    else{if(MXYZ==1){Mx=2;My=0;Mz=1;z1=YPosi(1);x1=ZPosi(1);y1=XPosi(1);cZ=cncode.Y;cY=cncode.X;cX=cncode.Z;}
    else if(MXYZ==2){Mx=1;My=2;Mz=0;z1=XPosi(1);x1=YPosi(1);y1=ZPosi(1);cZ=cncode.X;cY=cncode.Z;cX=cncode.Y;}
      }
      if(cZ!=a)z=cZ;else z=z1;
      Read_StartEnd();
/*      if(cncode.G<100)Read_Table(cncode.G);
      i=cncode.D;i>>=4;if(i<9)Dis_start=i;else Dis_start=0;
      i=cncode.D;i&=0xf;if(i<9)Dis_end=i;else Dis_end=Dis_start;*/
      if(z1==z)zs=0;if(z1>z)zs=1;if(z1<z)zs=-1;
/*      RunPi.PE=0;RunPi.PT=0;*/Dis_flag=0;Diss();
      if(zs){Dis_lines=Dis_start;
/*  DIS.Position_H=z1;DIS.XYZ=2;*/
    if(z1>z)SRunPi(Mz,(long)9999999,z);
    else    SRunPi(Mz,(long)-9999999,z);
    SRunDIS(0);
      }
      RunLineFlag++;
      if(!zs)RunLineFlag=4;
    break;
    case 1:
      if(Dis_lines>Dis_end){
      RunLineFlag=4;Dis_lines--;
    if(MXYZ==0){Position_control_z(cncode.Z);Signal_1100();}
    else{   if(MXYZ==1){Position_control_y(cncode.Y);Signal_1100Y();}
      else  if(MXYZ==2){Position_control_x(cncode.X);Signal_1100X();}
    }
      }
      else{
      if(zs==1){
    RunPi.XYZV1[RunPi.PV]=Table.Shendu[Dis_lines]-Table.Shendu[Dis_end]+z;
    SRunDIS(0);
      }
      else{
    RunPi.XYZV1[RunPi.PV]=Table.Shendu[Dis_end]-Table.Shendu[Dis_lines]+z;
    SRunDIS(0);
      }
      Dis_flag=1;xyz=0;Diss();
      RunLineFlag++;RunBar();
      }
    break;
    case 2:
      xyz+=Diss();
      if(xyz>49){
      Dis_lines++;RunLineFlag=1;}
    break;
    case 3:
    break;
    case 4:
      x=x1-cX;y=y1;
      SRunPi(Mx,x1,x);
      SRunDIS(0);
      Dis_flag=1;xyz=0;Diss();
      RunLineFlag++;RunBar();
    break;
    case 5:
      xyz+=Diss();
      if(xyz>49)RunLineFlag++;
    break;
    case 6:
      x=x1-cX;y=y1+cY;
      SRunPi(My,y1,y);
      SRunDIS(0);
      Dis_flag=1;xyz=0;Diss();
      RunLineFlag++;RunBar();
    break;
    case 7:
      xyz+=Diss();
      if(xyz>49)RunLineFlag++;
    break;
    case 8:
      x2=x1+cX;x=x1-cX;y=y1+cY;
      SRunPi(Mx,x,x2);
      SRunDIS(0);
      Dis_flag=1;xyz=0;Diss();
      RunLineFlag++;RunBar();
    break;
    case 9:
      xyz+=Diss();
      if(xyz>49)RunLineFlag++;
    break;
    case 10:
      x=x1+cX;y=y1+cY;y2=y1-cY;
      SRunPi(My,y,y2);
      SRunDIS(0);
      Dis_flag=1;xyz=0;Diss();
      RunLineFlag++;RunBar();
    break;
    case 11:
      xyz+=Diss();
      if(xyz>49)RunLineFlag++;
    break;
    case 12:
      x2=x1-cX;x=x1+cX;y=y1-cY;
      SRunPi(Mx,x,x2);
      SRunDIS(0);
      Dis_flag=1;xyz=0;Diss();
      RunLineFlag++;RunBar();
    break;
    case 13:
      xyz+=Diss();
      if(xyz>49)RunLineFlag++;
    break;
    case 14:
      x=x1-cX;y=y1-cY;
      SRunPi(My,y,y1);
      SRunDIS(0);
      Dis_flag=1;xyz=0;Diss();
      RunLineFlag++;RunBar();
    break;
    case 15:
      xyz+=Diss();
      if(xyz>49)RunLineFlag++;
    break;
    case 16:
      x=x1-cX;y=y1;
      SRunPi(Mx,x,x1);
      SRunDIS(0);
      Dis_flag=1;xyz=0;Diss();
      RunLineFlag++;RunBar();
    break;
    case 17:
      xyz+=Diss();
      if(xyz>49)RunLineFlag++;
    break;
    case 18:
    Signal_1100X();
    Signal_1100Y();
    Signal_1100();
    Position_control_x(x1);
    Position_control_y(y1);
    Position_control_z(z1);
    Pause();
    RunCode(0);
    break;
  }

}
