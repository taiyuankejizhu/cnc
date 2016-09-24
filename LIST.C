#include "znc.h"
#define HTX0 1
#define HTX1 456
#define HTXE 688
#define HTY0 1
#define HTYE 21
#define XYZY0 (HTYE+2)
#define XYZY1 (HTYE+2)
#define XYZYE0 (XYZY0+150)
#define XYZYE1 (XYZY1+112)
#define G92XE  (HTX1)
#define G92X0  (G92XE-80)
#define G92Y0  (XYZY0+150+1)
#define G92YE  (G92Y0+40)
#define MessX0 (HTX0)
#define MessY0 (G92Y0)
#define MessXE (G92X0-1)
#define MessYE ((G92Y0+G92YE)/2-1)
#define CRTX0 (HTX0)
#define CRTYE (G92YE)
#define CRTXE (G92X0-1)
#define CRTY0 (G92Y0+G92YE)/2
#define StaX0 (HTX1+1)
#define StaY0 (XYZYE1+1)
#define StaXE (HTXE)
#define StaYE (StaY0+56)
#define VolX0 (HTX1+1)
#define VolY0 (StaYE+1)
#define VolXE (HTXE)
#define VolYE (G92YE)
#define HMX1 799-20
#define HMY1 599-50
#define HMX0 HMX1-80
#define HMY0 5
#define HFX1 799-4
#define HFY1 599-2-14
#define HFX0 4
#define HFY0 HFY1-42

Disp16x32(int x,int y,int lr,int color,unsigned char *c){
/*lr=0左齐,=1归中,=2右齐*/
        unsigned char a[]="\x20\x0";
        register i;
        settextjustify(1,1);
        settextstyle(3,0,4);
        setcolor(color);
        if(lr==2){
            for(x-=9,i=strlen(c);i;i--,x-=18){
                a[0]=c[i-1];
                outtextxy(x,y,a);
            }
        }
        if(lr==0){
            for(x+=9,i=0;c[i];i++,x+=18){
                a[0]=c[i];
                outtextxy(x,y,a);
            }
        }
        if(lr==1)
            outtextxy(x,y,c);
        settextstyle(2,0,5);
}


/*void Dispcbar(int x,int y,int x1,int y1,int color,int bkcolor,int stx,char *str){
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
*/
void Listbar3d(int x,int y,int x1,int y1,long color,unsigned char flag,unsigned sty,unsigned char w,char *str1){
/*
color   (31-24B)按下背色
    (23-16B)按下前色
    (15-8B)弹起背色
    (7-0B) 弹起前色
flag(bit)
    0:0弹起状态 1按下状态
    1:0能操作   1不能操作
    2:0有显示   1无显示
    3:0单行     1双行
    4:0 5 6无效 1 5 6有效
    5:0下能操作 1下行不能操作
    6:0下有显示 1下行无显示
    7:0无中间线 1有中间线
sty +-调整Y
w
    (7-0B)边框宽度
*/
    int colors,bkcolors,color1;
    register i,j;
    signed char ya,yb;
    int xx,yy,ww,k;
    char str[0x80];
    strcpy(str,str1);
    if(flag&1)color>>=0x10;
    colors=color&0xff;
    bkcolors=color>>8;
    ya=sty&0xff;yb=sty>>8;
/*  if(str[0]==0){setfillstyle(1,color>>8);bar(x,y,x1,y1);return;}*/
    ww=w&0xff;
    yy=(y1+y+1)/2;
    if(flag&1&&!(flag&4)){
        setcolor(0x8);
        for(i=0;i<ww;i++){line(x,y+i,x1-i,y+i);line(x+i,y,x+i,y1-i);}
        setcolor(0xf);
        for(i=0;i<ww;i++){line(x+i,y1-i,x1,y1-i);line(x1-i,y+i,x1-i,y1);}
        setfillstyle(1,bkcolors);
        bar(x+ww,y+ww,x1-ww,y1-ww);
    }
    else{
        setcolor(0xf);
        for(i=0;i<ww;i++){line(x,y+i,x1-i,y+i);line(x+i,y,x+i,y1-i);}
        setcolor(0x8);
        for(i=0;i<ww;i++){line(x+i,y1-i,x1,y1-i);line(x1-i,y+i,x1-i,y1);}
        setfillstyle(1,bkcolors);
        bar(x+ww,y+ww,x1-ww,y1-ww);
        if(flag&4)return;
    }
    if(flag&8){
        for(j=0;str[j]&&str[j]!='\n';j++);
        str[j]=0;
        yy-=8;
    }
    i=strlen(str)<<3;
    xx=(x1+x-i+1)/2;
    if(flag&0x80){
        setcolor(0x17);line(x+ww+2,(y+y1)/2,x1-ww-2,(y+y1)/2);
        setcolor(0x1f);line(x+ww+2,1+(y+y1)/2,x1-ww-2,1+(y+y1)/2);
    }
    if(flag&0x2)color1=0x1b;
    else color1=colors;
    #if ZK
    List16x16F(xx,yy+ya,color1,str);
    #else
    List16x16M(xx,yy+ya,color1,str);
    #endif
    if(flag&8&&!(flag&0x40)){
        j++;
        i=strlen(str+j)<<3;
        xx=(x1+x-i)/2;
        yy+=16;
    }else return;
    if(flag&0x10){
        if(flag&0x20)color1=0x1b;
        else color1=colors;
    }
    #if ZK
    List16x16F(xx,yy+yb,color1,str+j);
    #else
    List16x16M(xx,yy+yb,color1,str+j);
    #endif
}

void Listbark3d(int x,int y,int x1,int y1,unsigned colors,char flag,char *str){
/*
colors  (15-8)边框
    (7-0) 前景
flag    (0):弹起状态,能操作
    (1):按下状态,能操作
    (3):不能操作(无效)
    (4):无显示
*/
    unsigned color,bkcolor,color1=0x1f;
    register i,j;
    signed char ya,yb;
    int xx,yy,ww,k;
    color=colors&0xff;
    bkcolor=colors>>8;
    if(flag&2){color=0x18;bkcolor=0x19;color1=0x1d;}
    else{color=colors&0xff;bkcolor=colors>>8;}
    if((flag&1))color1--;
    setfillstyle(1,color1);bar(x+1,y+1,x1-1,y1-1);
    setcolor(color);
    i=strlen(str)<<3;xx=(x1+x-i)/2;
    yy=(y+y1)>>1;
    #if ZK
    List16x16F(xx,yy,color,str);
    #else
    List16x16M(xx,yy,color,str);
    #endif
    setcolor(bkcolor);
    rectangle(x,y,x1,y1);
    if(!flag){
    setcolor(0x36);
    rectangle(x+1,y+1,x1-1,y1-1);
    rectangle(x+2,y+2,x1-2,y1-2);
    }
}
void Listbar(int x,int y,int x1,int y1,unsigned color,char stx,char *str)
{
/*
color:(15-8B)BK
      (7-0B)Color
*/
    register i,j;
    setfillstyle(1,color>>8);
    bar(x,y,x1,y1);
    i=x+stx;j=y+y1+1;j>>=1;
    #if ZK
    List16x16F(i,j,color,str);
    #else
    List16x16M(i,j,color,str);
    #endif
}
void Listbar3(int x0,int y0,int x1,int y1,char flag){
    struct linesettingstype OldLineSetting;
if(flag){
    getlinesettings(&OldLineSetting);
    setlinestyle(USERBIT_LINE,0xAAAA,NORM_WIDTH);
}
setcolor(0x11);
rectangle(x0,y0,x1-1,y1-1);
setcolor(0x1e);
rectangle(x0+1,y0+1,x1,y1);
if(flag){
     setlinestyle(  OldLineSetting.linestyle,
            OldLineSetting.upattern,
            OldLineSetting.thickness);
}
}
char far *HFStr(int x,int y)
{
    char stron1[]="清除",stron2[]="停机",stroff[]="关闭";
    char *strs;
    char *Str[22][11]={
/*0坐标设置*/   " 坐 标 "," X 轴 "," Y 轴 "," Z 轴 ","","","","",""," 返 回","",
/*1定位*/    "坐标设置"," 移 动 "," 归 零 "," 孔中心 ","归原点",""," ",""," 校 模 "," 返 回 ","",
/*2移动*/  " X "," Y "," Z ","","","","","",""," 返 回 ","",
    "\2DRY\n0","\2单步\nOFF","\2镜像Ｘ\nOFF","\2镜像Ｙ\nOFF","\2ＸＹ变换\nOFF","\2开始\nG30","\2精算时间\nSTART","\2精算时间\nRESET","\2图形\nOFF",/*"\1坐标选择\n画面",*/" 返 回 ","",
/*4*/   "\2插入\n改写","\1块\n范围选择","选择开始","复制","剪切","粘贴","\1文件\n插入","\1文件\n选项"," 清 除 "," 返 回 ","",
    "\1文件\n备份","\1文件\n恢复","\1文件\n删除","装 载","保 存","\1编译\n执行","","",""," 返 回 ","",
/*6*/   "增加行","删除行","组别变更","\1选择\n加工轴","加工深度","保存","\1加工\n计划","\1生成\n自动表",""," 返 回 ","",
    "加工选择","状态","","","","","","",""," 返 回 ","",
/*8*/   "加工计划","条件详细","\1位置设定\n任意","位置图形","NC生成","1孔","格子","圆周","任意","编辑","主菜单",
/*9归零*/   "X+","X-","Y+","Y-","Z+","Z-","X 中心","Y 中心","Z 中心","返回","",
/*10孔中心*/  "X+","X-","Y+","Y-","Z+","Z-","","",""," 返 回 ","",
    "\2DRY\n0","\2单步\nOFF","\2电源遮断\n2","\2跳过\nOFF","\2SCALE\n00001000","\2镜像Ｘ\nOFF","\2镜像Ｙ\nOFF","\2ＸＹ变换\nOFF","\2开始\nG30","\2精算时间\nSTART","\2精算时间\nRESET",
/*12*/"积炭时000","积炭高000","积炭停机","深孔加工","钢蚀钢00","副电源","油 泵","",""," 返 回 ","",
    "显示","","","","","","","","","","",
/*14*/  "","","","","","","","","","","",
    "机械","放电","动作","马达","各轴","通信","加工","\1SEIKA\nnet","纲络","操作","ON/OFF",
/*16*/  "\1数据\n备份","\1数据\n恢复","\1磁盘\n初始化","\1用户\n复制","\1自动\n装载","","","","","","",
    "C","","","","","","","","","","",
/*18*/  "","","","","","","","","","","",
    "","","","","","","","","","","",
/*20*/  "块","行","选择开始","选择结束","取消选择","插入","删除","保存","","","",
    };
    if(x==4&&y==1){strs=Str[20][block_sta];strncpy(Str[x][y]+1,strs,2);}
    if(x==4&&y==2){return Str[20][2+bselect_sta];}
    if(x==8&&y==2){*(Str[x][y]+10)=0;strcat(Str[x][y],StrLN0(LNStr.sl0));}
    if(x==8&&Ks8==1&&(y==5||y==6||y==7)){HF1[x][y]=0;return Str[20][y];}
    if(x==12&&y==0){Str[x][y][6]=AI_time/100+'0';Str[x][y][7]=(AI_time%100)/10+'0';Str[x][y][8]=AI_time%10+'0';}
    if(x==12&&y==1){Str[x][y][6]=AI_high/100+'0';Str[x][y][7]=(AI_high%100)/10+'0';Str[x][y][8]=AI_high%10+'0';}
    if(x==12&&y==2){Str[x][y][4]=0;
        if(!P[7])strcat(Str[x][y],stroff);
        else{
            if(P[7]==1)strcat(Str[x][y],stron1);
            if(P[7]==2)strcat(Str[x][y],stron2);
         }
    }
    if(x==12&&y==4){Str[x][y][6]=Steel_flag/10+'0';Str[x][y][7]=Steel_flag%10+'0';}
    return Str[x][y];
}
void ListF12(void){
  register i,j;
  int color;
  char *str[10]={"F1","F2","F3","F4","F5","F6","F7","F8","F9","F10"};
  color=BKCR12<<8|CR12;
    setfillstyle(1,BKCR0);
    bar(HMX1+2,HMY0+8+0,HMX1+14,HMY0+8+56*8+64+8);
    bar(HFX0+2,HFY1+2,HFX0+2+72*10+68,HFY1+14);
    for(i=0;i<10;i++)
    Listbar3d(HFX0+80*i,HFY1+2,HFX0+2+80*i+73,HFY1+14,color,0,0,0,str[i]);
}
ListHT(char flag,char *str){
  register i,j;
  unsigned color;
  if(!flag)j=56;else j=28;
  memcpy(DispStr,str,j);DispStr[j]=0;
  i=strlen(DispStr);
  for(;i<j;i++)strcat(DispStr," ");
  if(!flag){
    color=BkColor.ht0;color<<=8;color|=Color.ht0;
    Listbar3d(HTX0,HTY0,HTX1,HTYE,(long)color,0,0,1,DispStr);
  }else{
    color=BkColor.ht1;color<<=8;color|=Color.ht1;
    Listbar3d(HTX1+1,HTY0,HTXE,HTYE,(long)color,0,0,1,DispStr);
  }
}
ListHF0(int x,int y){
    static OldLine=-1;
    if(x!=OldLine)y=0;
    OldLine=x;
    ListHF(HF[x],y);
}
ListHF(int x,int y){
char str[80];
register int i;
char flags;
int w,sty;
flags=0;
for(i=0;i<10;i++){
  flags=HF1[x][i];
  if(!y||y==i+1){
    w=2;sty=0;
    strcpy(str,HFStr(x,i));
    if(str[0]=='\1'){flags|=0x08;sty=0x2fe;strcpy(str,str+1);}
    if(str[0]=='\2'){flags|=0x88;sty=0x2fe;strcpy(str,str+1);}
    Listbar3d(HFX0+80*i,HFY0,HFX0+2+80*i+73,HFY1-2,0x2a0f1d00,flags,sty,w,str);
    }
   }
}
ListHM(char flag){
register int i,j;
char far * str_temp;
char flags;
int w,sty;
char *str[11]={" 定 位 ","    ","编辑文件","\1LORAN\n辅助工具","\1加工  \n  文  件","\1加工  \n程序控制","显示","设定","\1加工条件\nC000","",""};
for(i=0;i<10;i++){
    flags=0;
    w=2;sty=0;
    str_temp=str[i];
    if(str_temp[0]=='\1'){flags|=0x08;sty=0x2fe;str_temp++;}
    if(str_temp[0]=='\2'){flags|=0x88;sty=0x2fe;str_temp++;}
    Listbar3d(HFX0+80*i,HFY0,HFX0+2+80*i+73,HFY1-2,0x2a0f1d00,flags,sty,w,str_temp);
    }
}
ListHM1(char flag){
char *str[6][3]={
    "","","",
    "无代码","","ＭＤＩ",
    "文件","图形","\1LORAN\n辅助工具",
    "文件","外部装置","PROCON",
    "远方画面","维护","全坐标系",
    "磁盘","检查","加工数据"
};
char flags;
int i,j;
return;
/*i=HM[0];
if(!flag||flag==1){
flags=HM1[i][1];
Listbar3d(HMX0+6,HMY0+12+44*5,HMX1-6,HMY0+12+44*5+42,0x360f1d00,flags,0,0x0002,str[i][0]);
}
if(!flag||flag==2){
flags=HM1[i][2];
Listbar3d(HMX0+6,HMY0+12+44*6,HMX1-6,HMY0+12+44*6+42,0x360f1d00,flags,0,0x0002,str[i][1]);
}
if(!flag||flag==3){
flags=HM1[i][3];
if(*str[i][2]!='\1')
Listbar3d(HMX0+6,HMY0+12+44*7,HMX1-6,HMY0+12+44*7+42,0x360f1d00,flags,0,0x0002,str[i][2]);
else
Listbar3d(HMX0+6,HMY0+12+44*7,HMX1-6,HMY0+12+44*7+42,0x360f1d00,flags,0x2fe,0x1002,str[i][2]+1);
}*/
}
char far * gstr(char *fmt,...){
 /* va_list  argptr;*/          /* Argument list pointer    */
 /* char str[140];*/            /* Buffer to build sting into   */
/*  int cnt;*/              /* Result of SPRINTF for return */

 /* va_start( argptr, fmt ); */     /* Initialize va_ functions */

/*  cnt = vsprintf( str, fmt, argptr ); */  /* prints string to buffer  */
  /*va_end( argptr );*/         /* Close va_ functions      */
  /*return str; */
}
int gprintf( int xloc, int yloc, int color, char *fmt, ... )
{
    char *str;
    str=gstr(fmt,...);
    #if ZK
    List16x16F(xloc,yloc,color,str);
    #else
    List16x16M(xloc,yloc,color,str);
    #endif


/*  return( cnt );  */      /* Return the conversion count  */

}
void ListXYZ00(char flag){
  register i,j;
  unsigned color;
  char xyzstr[4]="XYZ",str[7];
  static long positionx=7777,positiony=777,positionz=7777;
  static long Velocity_x=0x7fff,Velocity_y=0x7fff,Velocity_z=0x7fff;
  static int a=-1,v=-1;
  char av[5];
  color=BkColor.xyz;color<<=8;color|=Color.xyz;
  if(!flag)
  {
    color=BkColor.xyz;color<<=8;color|=Color.xyz;
    Listbar3d(HTX0,XYZY0-22,HTX1,XYZYE0,(long)color,0,0,1,"");
    for(i=0;i<3;i++){
      List40x40(HTX0+2,XYZY0+i*54-18,color,xyzstr[i]);
      List40x40(HTX0+2+420,XYZY0+i*54-18,color,'M');
     }
   }
   if(positionx!=position_x)
     {
        positionx=position_x;
        ListXYZ1(0,positionx*DZC);
     }
  if(positiony!=position_y)
    {
        positiony=position_y;
        ListXYZ1(1,positiony*DZC);
    }
  if(positionz!=position_z)
    {
       positionz=position_z;
       ListXYZ1(2,positionz*DZC);
    }
  if(Velocity_x!=abs(Read_Velocity_x(0)))
    {
       Velocity_x=abs(Read_Velocity_x(0));
       ltoa(Velocity_x,str,10);
       Dispebar(HTX0+5,XYZY0+28,HTX0+25,XYZY0+38,color,BkColor.xyz,0,str);
    }
  if(Velocity_y!=abs(Read_Velocity_y(0)))
    {
        Velocity_y=abs(Read_Velocity_y(0));
        ltoa(Velocity_y,str,10);
        Dispebar(HTX0+5,XYZY0+28+54,HTX0+25,XYZY0+38+54,color,BkColor.xyz,0,str);
    }
  if(Velocity_z!=abs(Read_Velocity_z(0)))
    {
       Velocity_z=abs(Read_Velocity_z(0));
       ltoa(Velocity_z,str,10);
       Dispebar(HTX0+5,XYZY0+26+54*2,HTX0+25,XYZY0+36+54*2,color,BkColor.xyz,0,str);
    }
  if(v!=V){
        itoa(V,av,10);
        v=V;
        Dispebar(FLAG_POSX1-44,FLAG_POSY0+6+30*3+2,FLAG_POSX1-6,FLAG_POSY0+6+30*3+24,COLORE2,BKCOLORE2,1,av);
   }
}
void ListXYZ1(char flag,long l){
  register i,j,k;
  unsigned color;
  char str1[12];
  str1[0]=(l<0)?'-':' ';
  l=labs(l);
  j=l%1000;l/=1000;
  for(i=0;i<4;i++){
    str1[4-i]=(l||!i)?l%10+'0':' ';l/=10;
  }
  for(i=0;i<3;i++){
    str1[8-i]=j%10+'0';j/=10;
  }
  str1[5]='.';str1[9]=0;
  color=BkColor.xyz;color<<=8;color|=Color.xyz;
  for(j=0,i=0;i<9;i++,j+=44){
    if(i==5)j-=12;
    List40x40(HTX0+2+40+j,XYZY0+flag*54-18,color,str1[i]);
  }
}
void ListZB(char zb,char flag,long l){

  if(zb!=SlZB)return;
  ListZB1(flag,l);
}
void ListZB1(char flag,long l){
  char str[12];
  xltoa(l,str);
  setfillstyle(1,BkColor.zb);
  bar(HTX1+5+24,XYZY0+flag*30+24,HTXE-2,XYZY0+flag*30+24+26);
  Disp16x32(HTX1+5+48,XYZY0+flag*30+16+18,0,Color.zb,str);
}
void ListZB00(char flag){
  register i,j;
  unsigned color;
  char *str[]={"X","Y","Z"};
  color=BkColor.zb;color<<=8;color|=Color.zb;
  Listbar3d(HTX1+1,XYZY1,HTXE,XYZYE1,(long)color,0,0,1,"");
  for(i=0;i<3;i++)
    Disp16x32(HTX1+5,XYZY0+i*30+16+18,0,color,str[i]);
  Listbar(HTX1+5+72,XYZY0+4,HTXE-5,XYZY0+4+16,color,8,SlZBS[flag]);
  SlZB=flag;
}
void ListXYZ2(char flag,long l){
  char str[8];
  unsigned color;
  color=BkColor.zb;color<<=8;color|=Color.zb;
  str[6]=0;str[5]=l%10+'0';l/=10;str[4]=(l)?l%10+'0':' ';l/=10;
  str[3]=(l)?l%10+'0':' ';l/=10;str[2]=(l)?l%10+'0':' ';l/=10;
  str[1]=(l)?l%10+'0':' ';l/=10;str[0]=(l)?l%10+'0':' ';
  Listbar(HTX1+(HTXE-HTX1)/2+3+40,XYZY1+78+flag*14,HTXE-3,XYZY1+78+flag*14+14,color,8,str);
}
void ListG92(void){
  register i;
  unsigned color;
  char str[6];
  i=G92S[G92];i%=1000;
  str[0]='G';
  str[1]=i/100+'0';i%=100;
  str[2]=i/10+'0';
  str[3]=i%10+'0';
  str[4]=0;
  color=BkColor.g92;color<<=8;color|=Color.g92;
  Listbar3d(G92X0,G92Y0,G92XE,G92YE,(long)color,0,0,1,"");
  Disp16x32((G92X0+G92XE)/2+2,(G92Y0+G92YE)/2-2,1,color,str);
}
void ListMess1(char *str){
#define _LEN_ 46
  char MessStr[_LEN_+1];
  register unsigned color,i;
  strncpy(MessStr,str,_LEN_);
  i=strlen(MessStr);if(i>_LEN_){i=_LEN_;MessStr[i]=0;}strncat(MessStr,strspace,_LEN_-i);
  color=BkColor.mess;color<<=8;color|=Color.mess;
  Listbar3d(MessX0,MessY0,MessXE,MessYE,(long)color,0,0,1,MessStr);
#undef _LEN_
}
void ListMess(char flag){
  char *str[]={""};
  ListMess1(str[flag]);
}
void ListCRT1(char *str){
#define _LEN_ 46
  char CRTStr[_LEN_+1];
  register unsigned color,i;
  strncpy(CRTStr,str,_LEN_);
  i=strlen(CRTStr);if(i>_LEN_){i=_LEN_;CRTStr[i]=0;}strncat(CRTStr,strspace,_LEN_-i);
  color=BkColor.crt;color<<=8;color|=Color.crt;
  Listbar3d(CRTX0,CRTY0,CRTXE,CRTYE,(long)color,0,0,1,CRTStr);
#undef _LEN_
}
void ListErr(char flag){
  char *str[]={"NC程序长度超过32K,超出部分被取消!",
  "NC程序长度超过32K,操作无效!","编辑行太长!","硬盘错误读!","硬盘错误写!",
  "","加工条件表太长!"
  };
  ListMess1(str[flag]);
}
void ListVol0(void){
  register unsigned color;
  color=BkColor.vol;color<<=8;color|=Color.vol;
  Listbar3d(VolX0,VolY0,VolXE,VolYE,(long)color,0,0,1,"");
}
void Set_disp(void){
setfillstyle(1,BKCR0);
bar(1,215,688,538);
}
void ListVol(unsigned char vol){
  register unsigned color,i,j;
  static unsigned oldvol=0;
  if(oldvol==0){ListVol0();
    setfillstyle(1,Color.vol);
    bar(VolX0+4,VolY0+7,VolX0+3+0xe0,VolYE-7);
  }
  i=vol;i++;i*=7;i>>=3;
  if(i<oldvol){
    setfillstyle(1,Color.vol);
    bar(VolX0+4+i,VolY0+7,VolX0+3+0xe0,VolYE-7);
  }
  if(i>oldvol){
    if(oldvol<8){
      j=i>8?8:i;
      setfillstyle(1,0x28);
      bar(VolX0+4,VolY0+7,VolX0+3+j,VolYE-7);
    }
    if(i<=8){oldvol=i;return;}
    if(oldvol<88){
      j=i>88?88:i;
      setfillstyle(1,0x2C);
      bar(VolX0+4+8,VolY0+7,VolX0+3+j,VolYE-7);
    }
    if(i<=88){oldvol=i;return;}
    setfillstyle(1,0x2F);
    bar(VolX0+4+88,VolY0+7,VolX0+3+i,VolYE-7);
  }oldvol=i;
}

void ListStatus(unsigned char flag){
  register unsigned color,i,j;
  int flags=0;
  char *str[]={" X+ "," X- "," Y+ "," Y- ","上限","下限","火警","油位","DRY1"," ST ","电源","油泵"};
  if(flag&0x80)flags++;
  flag&=0x7f;
  if(!flag||flag>12||flags){
    color=BkColor.sta0;color<<=8;color|=Color.sta0;
    if(!flags){Listbar3d(StaX0,StaY0,StaXE,StaYE,(long)color,0,0,1,"");return;}
  }
  flag--;
  i=flag%4;i*=((StaXE-StaX0)/4);i+=3;i+=StaX0;
  j=flag/4;j*=((StaYE-StaY0)/3);j+=2;j+=StaY0;
  if(!flags){
  if(flag>=8||flag==6&&!CdsSta||flag==7&&!OilSta){color=BkColor.sta1;color<<=8;color|=Color.sta1;}
  else{color=BkColor.sta2;color<<=8;color|=Color.sta2;}
  Listbar(i,j,i+((StaXE-StaX0)/4-3),j+((StaYE-StaY0)/3-2),color,12,str[flag]);
  }
  else Listbar(i,j,i+((StaXE-StaX0)/4-3),j+((StaYE-StaY0)/3-2),color,12,"");
}

