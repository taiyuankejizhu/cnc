#include "math.h"
#include "stdio.h"
#include "dir.h"
#include "ctype.h"
#define LNYL 22
#define LNCO0 (long)0x45    /*LN00前景色*/
#define LNCO1 (long)0x45    /*LN1前景色,提示*/
#define LNCO3 (long)0x27    /*选择框内前景色*/
#define LNCO4 (long)0x0     /*选择框内前景色 按键*/
#define LNCO5 (long)0x26    /*选择框内前景色 选中*/
#define LNCO6 (long)(0x6A)  /*选择框内前景色 无效*/
#define LNBC0N (long)0x37   /*     空白色*/
#define LNBC0S (long)0x28   /*LN00 弹出色*/
#define LNBC0P (long)0x2E   /*LN00 按下色*/
#define LNBC3 (long)0x60    /*选择框内背景色*/
#define LNBC4 (long)0x1c    /*选择框内背景色 按键*/
#define LNBC5 (long)0xc1    /*选择框内背景色 选中*/
#define LNBC6 (long)(BkColor.mstr-1)    /*选择框内背景色 无效*/
/*#define LNBCR (long)(0x90)*/  /*任意底色*/
/*#define LNCOR (long)(0x40)*/
/*#define LNBCR0 (long)(LNBCR+1)*/  /*序列底色*/
/*#define LNCOR0 (long)(LNCOR+1)*/  /*序列前色*/
/*#define LNBCR1 (long)(LNBCR+2)*/  /*字符底色*/
/*#define LNCOR1 (long)(LNCOR+2)*/  /*字符前色*/
/*#define LNBCR2 (long)(0x47)*/ /*当前字符底色*/
/*#define LNCOR2 (long)(0x37)*/ /*当前字符前色*/

#define LNX0 1
#define LNXE 688
#define LNY0 215
#define LNYE 538
#define LNX0_1 (LNX0+3)
#define LNXE_1 (LNXE-3)
#define LNYE_1 (LNYE-3)
#define LNY0_1 (LNYE-3-38)
#define LNX0_2 (LNX0+8)
#define LNY0_2 (LNY0+18)
#define LNX0_3 (LNX0+8+0x2b*8)  /*任意*/
#define LNY0_3 (LNY0+18)    /*任意*/
#define LNX0_4 (LNX0+8+200) /*图形*/
#define LNY0_4 (LNY0+18)    /*图形*/
#define LNX0_5 (LNX0+8+10)  /*条件详细*/
#define LNY0_5 (LNY0+18)    /*条件详细*/
#include "znc.h"

char *StrLN00(int flag){
  char *str[__LN00K__]={"柱形状、不通孔","柱、通孔","棱","锤","椎","楔","半球",
  "球/三维","竹枪","正多边形、不通孔","正多边形、通孔","浇口孔","压入",
  "芯杆","横伺服、袋","横伺服、上面开放","保留1","保留2"};
  return str[flag];
}
char *StrLN0(char flag){
  char *str[__LNSTR0__]={" 1孔","格子","圆周","任意"};
  return str[flag];
}
char *StrLN1(char flag){
  char *str[__LNSTR1__]={"X+","X-","Y+","Y-","Z+","Z-"};
  return str[flag];
}
char *StrLN2(char flag){
  char *str[__LNSTR2__]={"Cu-St","Gr-St","CuW-WC"};
  return str[flag];
}
char *StrLN3(char flag){
  char *str[__LNSTR3__]={"圆","正方体","长方体","任意"};
  return str[flag];
}
char *StrLN4(char flag){
  char *str[__LNSTR4__]={"思考","自由","HS","Z摇动"};
  return str[flag];
}
char *StrLN5(char flag){
  char *str[__LNSTR5__]={"粗","正常","精"};
  return str[flag];
}

void List_NM(int x,int y,int wide,char flag,char *str){
/*flag 0:正常 1:选中 2:无效*/
  register i;
  long l=(long)(LNBC3<<24|LNCO3<<16|LNBC5<<8|LNCO5);
  if(flag){flag&=1;if(flag)l<<=16;else l=(long)LNBC6<<24|LNCO6<<16;}
  x*=8;y*=LNYL;
  if(flag&&LN_NM[1]){strcpy(DispStr,str);strcat(DispStr,strspace);DispStr[wide]=0;}
  else{strcpy(DispStr,strspace);i=strlen(str);strcpy(DispStr+wide-i,str);DispStr[wide]=0;}
  Listbar3d(LNX0_2+x-1,LNY0_2+y,LNX0_2+x+8*wide+1,LNY0_2+y+16+2,(long)l,1,0,1,DispStr);
  if(flag){if(LN_NM[1])line(LNX0_2+x+(LN_NM[1]<<3)-8,LNY0_2+y+16+1,LNX0_2+x+(LN_NM[1]<<3),LNY0_2+y+16+1);}
}
void ListNM(int x,int y,char flag,char *str){
/*flag 0:正常 1:选中 2:无效*/
  int i;
  long l=(long)(LNBC3<<24|LNCO3<<16|LNBC5<<8|LNCO5);
  if(flag){flag&=1;if(flag)l<<=16;else l=(long)LNBC6<<24|LNCO6<<16;}
  x*=8;y*=LNYL;
  if(flag&&LN_NM[1]){strcpy(DispStr,str);strcat(DispStr,strspace);DispStr[10]=0;}
  else{strcpy(DispStr,strspace);i=strlen(str);strcpy(DispStr+10-i,str);}
  Listbar3d(LNX0_2+x-1,LNY0_2+y,LNX0_2+x+8*10+1,LNY0_2+y+16+2,(long)l,1,0,1,DispStr);
  if(flag){if(LN_NM[1])line(LNX0_2+x+(LN_NM[1]<<3)-8,LNY0_2+y+16+1,LNX0_2+x+(LN_NM[1]<<3),LNY0_2+y+16+1);}
}
void ListLNSW(int x,int y,char flag,char sw){
  long l=(long)(LNBC3<<24|LNCO3<<16|LNBC5<<8|LNCO5);
  if(flag)l<<=16;
  x*=8;y*=LNYL;
  if(sw)strcpy(DispStr,"●");else DispStr[0]=0;
  Listbar3d(LNX0_2+x-1,LNY0_2+y,LNX0_2+x+8*2+1,LNY0_2+y+16+1,(long)l,1,0,1,DispStr);
}
void ListStr1(int x,int y,char flag){
  long l=(long)(LNBC3<<24|LNCO3<<16|LNBC5<<8|LNCO5);
  if(flag)l<<=16;
  x*=8;y*=LNYL;
  strcpy(DispStr,StrLN1(LNStr.sl1));strcat(DispStr,"  ");
  Listbar3d(LNX0_2+x-1,LNY0_2+y,LNX0_2+x+8*3+18,LNY0_2+y+18,(long)l,1,0,1,DispStr);
  Listbar3d(LNX0_2+x+8*3+1,LNY0_2+y+1,LNX0_2+x+8*3+17,LNY0_2+y+17,(long)(LNBC4<<8|LNCO4),0,0,1,"");
}
void ListStr2(int x,int y,char flag){
  long l=(long)(LNBC3<<24|LNCO3<<16|LNBC5<<8|LNCO5);
  if(flag)l<<=16;
  x*=8;y*=LNYL;
  strcpy(DispStr,StrLN2(LNStr.sl2));strcat(DispStr,"  ");
  Listbar3d(LNX0_2+x-1,LNY0_2+y,LNX0_2+x+8*10+18,LNY0_2+y+18,(long)l,1,0,1,DispStr);
  Listbar3d(LNX0_2+x+8*10+1,LNY0_2+y+1,LNX0_2+x+8*10+17,LNY0_2+y+17,(long)(LNBC4<<8|LNCO4),0,0,1,"");
}
void ListStr3(int x,int y,char flag){
  long l=(long)(LNBC3<<24|LNCO3<<16|LNBC5<<8|LNCO5);
  if(flag)l<<=16;
  x*=8;y*=LNYL;
  strcpy(DispStr,StrLN3(LNStr.sl3));strcat(DispStr,"  ");
  Listbar3d(LNX0_2+x-1,LNY0_2+y,LNX0_2+x+8*10+18,LNY0_2+y+18,(long)l,1,0,1,DispStr);
  Listbar3d(LNX0_2+x+8*10+1,LNY0_2+y+1,LNX0_2+x+8*10+17,LNY0_2+y+17,(long)(LNBC4<<8|LNCO4),0,0,1,"");
}
void ListStr4(int x,int y,char flag){
  long l=(long)(LNBC3<<24|LNCO3<<16|LNBC5<<8|LNCO5);
  if(flag)l<<=16;
  x*=8;y*=LNYL;
  strcpy(DispStr,StrLN4(LNStr.sl4));strcat(DispStr,"  ");
  Listbar3d(LNX0_2+x-1,LNY0_2+y,LNX0_2+x+8*10+18,LNY0_2+y+18,(long)l,1,0,1,DispStr);
  Listbar3d(LNX0_2+x+8*10+1,LNY0_2+y+1,LNX0_2+x+8*10+17,LNY0_2+y+17,(long)(LNBC4<<8|LNCO4),0,0,1,"");
}
void ListStr5(int x,int y,char flag){
  long l=(long)(LNBC3<<24|LNCO3<<16|LNBC5<<8|LNCO5);
  if(flag)l<<=16;
  x*=8;y*=LNYL;
  strcpy(DispStr,StrLN5(LNStr.sl5));strcat(DispStr,"  ");
  Listbar3d(LNX0_2+x-1,LNY0_2+y,LNX0_2+x+8*10+18,LNY0_2+y+18,(long)l,1,0,1,DispStr);
  Listbar3d(LNX0_2+x+8*10+1,LNY0_2+y+1,LNX0_2+x+8*10+17,LNY0_2+y+17,(long)(LNBC4<<8|LNCO4),0,0,1,"");
}

void ListLNErr(char flag,int n){
  char *str[]={"孔001加工深度错误!","减寸量值太小!","粗糙度值太小!",
  "加工条件表太长!","无加工条件表!","","错误代码07!","错误代码08!"
  };
  static err=0;
  n++;
  switch(flag){
    case 0:if(err)ListHT(1,"");break;
    case 1:str[0][2]=n/100+'0';n%=100;str[0][3]=n/10+'0';
      str[0][4]=n%10+'0';/*ListLN1(str[0]);*/ListHT(1,str[0]);
      HF1[17][4]=2;ListHF0(HM[0],5);break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    ListHT(1,str[flag-1]);break;
  }err=flag;
}
void ListLN0(){
   int i;
   DispStr[0]=0;
   Listbar3d(LNX0,LNY0,LNXE,LNYE,(long)LNCO0|BkColor.mstr<<8,0,0,0,DispStr);
   ListLN1("");
}
void ListLN1(char *str){    /*提示*/
  register i,j;
  Listbar3(LNX0_1,LNY0_1,LNXE_1,LNYE_1,0);
  setfillstyle(1,BkColor.mstr);
  bar(LNX0_1+2,LNY0_1+2,LNXE_1-2,LNYE_1-2);
  for(i=0;i<84&&str[i]&&str[i]!='\n';i++){DispStr[i]=str[i];}
  j=i;DispStr[i]=0;i=0;
  Listbar(LNX0_1+2,LNY0_1+2,LNXE_1-2,(LNY0_1+LNYE_1)/2-1,BkColor.mstr<<8|LNCO1,2,DispStr);
  if(str[j]){
    if(str[j]=='\n')j++;
    for(;i<84&&str[i+j]&&str[i+j]!='\n';i++){DispStr[i]=str[i+j];}
  }
  DispStr[i]=0;
  Listbar(LNX0_1+2,(LNY0_1+LNYE_1)/2+1,LNXE_1-2,LNYE_1-2,BkColor.mstr<<8|LNCO1,2,DispStr);
}
void ListLN2(char *str){
  register unsigned x,y;
  x=str[0];y=str[1];str+=2;
  x<<=3;y*=LNYL;
  Listbar(LNX0_2+x,LNY0_2+y,LNX0_2+x+(strlen(str)<<3),LNY0_2+y+18,BkColor.mstr<<8|Color.mstr,2,str);
}
void InitLN(void){
  register i;
  char *str;
  str=HF1[8];
  str[1]=0;
  str[2]=0;
  str[3]=0;
  str[4]=2;
  str[5]=4;
  str[6]=4;
  str[7]=4;
  str[8]=4;
  str[9]=4;
  LN_NM[1]=0;
  LNStr.sl0=0;
  Ks8=0;
  LN02.topline=0;
  LN034.ty=0;
  for(i=0;i<11;i++)HF1[8][i]=HF2[8][i];
}

void ListLN00(char flag){
#define _LN_ __LN00K__
  register i,j;
  int x,y;
  unsigned long l;
  char flags[_LN_];
  if(!flag){InitLN();ListLN0();}
  l=(long)(LNBC0S<<8|(long)LNBC0P<<24|(long)LNCO0<<16|LNCO0);
  for(i=0;i<_LN_;i++){if(!flag||flag==i+1)flags[i]=1;else flags[i]=0;}
  for(i=0;i<_LN_;i++){
    if(!flags[i])continue;
    if(LN00K==i)ListLN1(StrLN00(i));
    x=i%8;y=i/8;
    x*=84;y*=84;
    x+=16;y+=16;
   Listbar3d(LNX0+x,LNY0+y,LNX0+x+64,LNY0+y+64,l,LN00K==i,0,1,"");
  }
#undef _LN_
}
void ListLN01(char flag){   /*加工计划*/
#define _LN_ __LN01K__
  register i,j;
  char flags[_LN_+1];
  char *str[]={"\x01\x00加工深度","\x01\x01材料组合","\x01\x02投影面积",
  "\x01\x03精糙度","\x01\x04摇动轨迹","\x01\x05减寸量","\x01\x06减寸量Y",
  "\x2b\x00腐蚀条件","\x2b\x01摇动选择","\x2b\x02任意类型","\x2b\x03IP限制",
  "\x2b\x04加工工程","\x1c\x00mm","\x1c\x02","\x1c\x03umRy",
  "\x1c\x05mm/Side","\x1c\x06mm/Size","\x3e\x00有","\x3e\x03有",""};
  if(!flag){ListLN0();LN_NM[1]=0;}
  if(!flag)for(i=0;*str[i];i++)ListLN2(str[i]);
  for(i=1;i<=_LN_;i++){if(!flag||i==flag)flags[i]=1;else flags[i]=0;}
  for(i=1;i<=_LN_;i++){
    if(!flags[i])continue;
    if(LN01K==(i-1))j=1;else j=0;j|=LN01S[i-1];
    switch(i){
      case 1:LNStr.sl1=LN01.sl0;ListStr1(11,0,j);break;
      case 2:ListNM(0x11,0,j,LN01.str0);break;
      case 3:LNStr.sl2=LN01.sl1;ListStr2(0x11,1,j);break;
      case 4:ListNM(0x11,2,j,LN01.str1);break;
      case 5:ListNM(0x11,3,j,LN01.str2);break;
      case 6:LNStr.sl3=LN01.sl2;ListStr3(0x11,4,j);break;
      case 7:ListNM(0x11,5,j,LN01.str3);break;
      case 8:ListNM(0x11,6,j,LN01.str4);break;
      case 9:ListLNSW(0x3b,0,j,LN01.sw0);break;
      case 10:LNStr.sl4=LN01.sl3;ListStr4(0x3b,1,j);break;
      case 11:ListNM(0x3b,2,j,LN01.str5);break;
      case 12:ListLNSW(0x3b,3,j,LN01.sw1);break;
      case 13:List_NM(0x3b+6,3,4,j,LN01.str6);break;
      case 14:LNStr.sl5=LN01.sl4;ListStr5(0x3b,4,j);break;
      case 15:
      default:
      break;
    }
  }
#undef _LN_
}
void ListLN02a(int x,int y){
  register i,j;
  int sx,sxl;
  union long_char l;
  char *str;
  l.c[3]=BkColor.tab0;
  l.c[1]=BkColor.tab1;l.c[0]=Color.tab1;
  Listbar3d(x,y,x+tablen.tablen*8+stablen.tablen+2,y+20*11,l.l,1,0,1,"");
  for(i=1;i<=__TABLEN__;i++){
    switch(i){
      case 1:sx=stablen.no;sxl=tablen.no;break;
      case 2:sx=stablen.pl;sxl=tablen.pl;break;
      case 3:sx=stablen.ip;sxl=tablen.ip;break;
      case 4:sx=stablen.on;sxl=tablen.on;break;
      case 5:sx=stablen.off;sxl=tablen.off;break;
      case 6:sx=stablen.sv;sxl=tablen.sv;break;
      case 7:sx=stablen.s;sxl=tablen.s;break;
      case 8:sx=stablen.up;sxl=tablen.up;break;
      case 9:sx=stablen.dn;sxl=tablen.dn;break;
      case 10:sx=stablen.v;sxl=tablen.v;break;
      case 11:sx=stablen.lc;sxl=tablen.lc;break;
      case 12:sx=stablen.pp;sxl=tablen.pp;break;
      case 13:sx=stablen.b_ofs;sxl=tablen.b_ofs;break;
      case 14:sx=stablen.s_ofs;sxl=tablen.s_ofs;break;
      case 15:sx=stablen.lns;sxl=tablen.lns;break;
      case 16:sx=stablen.step;sxl=tablen.step;break;
      case 17:sx=stablen.lnm;sxl=tablen.lnm;break;
      case 18:sx=stablen.ry;sxl=tablen.ry;break;
      case 19:sx=stablen.timer;sxl=tablen.timer;break;
    }
    str=tabname[i-1];
    j=sx;j<<=3;j+=i;j+=x;j++;
    Listbar3d(j,y+1,j+sxl*8+8,y+1+18,l.l,0,0,1,str);
  }
}
void ListLN02ab(int x,int y,int i){
  setfillstyle(1,BkColor.tab0);if(i>=10)return;
  bar(x+1,y+i*20+1,x+tablen.tablen*8+stablen.tablen+1,y+20*11-1);
}
void ListLN02b(int x,int y){
  register i,j;
  int sx,sxl;
  char flag=0;
  union long_char l;
  char *str;
  j=LN02.y+y-1;
  if(x==LN02.sx&&y==LN02.sy)flag=1;
  switch(x){
    case 0:for(j=1;j<=__TABLEN__;j++)ListLN02b(j,y);break;
    case 1:sx=stablen.no;sxl=tablen.no;str=tab[j]->no;break;
    case 2:sx=stablen.pl;sxl=tablen.pl;str=tab[j]->pl;break;
    case 3:sx=stablen.ip;sxl=tablen.ip;str=tab[j]->ip;break;
    case 4:sx=stablen.on;sxl=tablen.on;str=tab[j]->on;break;
    case 5:sx=stablen.off;sxl=tablen.off;str=tab[j]->off;break;
    case 6:sx=stablen.sv;sxl=tablen.sv;str=tab[j]->sv;break;
    case 7:sx=stablen.s;sxl=tablen.s;str=tab[j]->s;break;
    case 8:sx=stablen.up;sxl=tablen.up;str=tab[j]->up;break;
    case 9:sx=stablen.dn;sxl=tablen.dn;str=tab[j]->dn;break;
    case 10:sx=stablen.v;sxl=tablen.v;str=tab[j]->v;break;
    case 11:sx=stablen.lc;sxl=tablen.lc;str=tab[j]->lc;break;
    case 12:sx=stablen.pp;sxl=tablen.pp;str=tab[j]->pp;break;
    case 13:sx=stablen.b_ofs;sxl=tablen.b_ofs;str=tab[j]->b_ofs;break;
    case 14:sx=stablen.s_ofs;sxl=tablen.s_ofs;str=tab[j]->s_ofs;break;
    case 15:sx=stablen.lns;sxl=tablen.lns;str=tab[j]->lns;break;
    case 16:sx=stablen.step;sxl=tablen.step;str=tab[j]->step;break;
    case 17:sx=stablen.lnm;sxl=tablen.lnm;str=tab[j]->lnm;break;
    case 18:sx=stablen.ry;sxl=tablen.ry;str=tab[j]->ry;break;
    case 19:sx=stablen.timer;sxl=tablen.timer;str=tab[j]->timer;break;
  }
  if(!x)return;
  l.c[3]=BkColor.tab3;l.c[2]=Color.tab3;l.c[1]=BkColor.tab2;l.c[0]=Color.tab2;
  if(flag&&!LN_NM[1]){l.l>>=16;}
  if(flag&&LN_NM[1]){strcpy(DispStr,str);strcat(DispStr,strspace);DispStr[sxl+1]=0;}
  else{strcpy(DispStr,strspace);i=strlen(str);strcpy(DispStr+sxl+1-i,str);DispStr[sxl+1]=0;}
  j=sx;j<<=3;j+=x;j++;
  i=y*20;i++;
  Listbar3d(LNX0_5+j,LNY0_5+i,LNX0_5+j+sxl*8+8,LNY0_5+i+18,l.l,LN_NM[1]!=0&flag,0,1,DispStr);
  if(x==1)j+=8;
  if(flag){if(LN_NM[1])line(LNX0_5+j+(LN_NM[1]<<3)-8,LNY0_5+i+16+1,LNX0_5+j+(LN_NM[1]<<3),LNY0_5+i+16+1);}
}
void ListLN02(int x,int y){
#define _LN_ __LN00K__
  register i,j;
  int i1,j1,k;
  if(y==-1){ListLN0();ListLN02a(LNX0_5,LNY0_5);}
  if(!y||y==-1){i=1;j=LN02.topline-LN02.y;x=0;}
  else{i=y;j=y;}
  for(;i<=j;i++){
  if(i>10)break;ListLN02b(x,i);
  }if(!y)ListLN02ab(LNX0_5,LNY0_5,i);
#undef _LN_
}
void ListLN03(char flag){
#define _LN_ __LN00K__
  register i,j;
  if(!flag)ListLN0();
#undef _LN_
}
void ListLN030(char flag){  /*1孔*/
#define _LN_ __LN030K__
  register i,j;
  char flags[_LN_+1];
  char *str[]={"\x03\x00坐标设定后,开始加工。","\x03\x01移动后开始加工。",
  "\x03\x02从现在位置开始加工。","\x01\x04坐标系","\x05\x05Ｘ","\x05\x06Ｙ",
  "\x05\x07Ｚ","\x2b\x00Z轴干涉回避位置","\x2b\x01极限移动","\x2b\x02加工工程",
  "\x1c\x05mm","\x1c\x06mm","\x1c\x07mm","\x3e\x01有",""};
  if(!flag){ListLN0();LN_NM[1]=0;str[7][2]=*StrLN1(LN01.sl0);}
  if(!flag)for(i=0;*str[i];i++)ListLN2(str[i]);
  for(i=1;i<=_LN_;i++){if(!flag||i==flag)flags[i]=1;else flags[i]=0;}
  for(i=1;i<=_LN_;i++){
    if(!flags[i])continue;
    if(LN030K==(i-1))j=1;else j=0;j|=LN030S[i-1];
    switch(i){
      case 1:ListLNSW(0x1,0,j,LN030.sw0);break;
      case 2:ListLNSW(0x1,1,j,LN030.sw1);break;
      case 3:ListLNSW(0x1,2,j,LN030.sw2);break;
      case 4:ListNM(0x11,4,j,LN030.str0);break;
      case 5:ListNM(0x11,5,j,LN030.str1);break;
      case 6:ListNM(0x11,6,j,LN030.str2);break;
      case 7:ListNM(0x11,7,j,LN030.str3);break;
      case 8:ListNM(0x3b,0,j,LN030.str4);break;
      case 9:ListLNSW(0x3b,1,j,LN030.sw3);break;
      case 10:LNStr.sl5=LN030.sl0;ListStr5(0x3b,2,j);break;
      default:break;
    }
  }
#undef _LN_
}
void ListLN031(char flag){  /*格子*/
#define _LN_ __LN031K__
  register i,j;
  char flags[_LN_+1];
  char *str[]={"\x01\x00加工方向","\x01\x01坐标系","\x01\x02加工起始位置 X",
  "\xe\x03Y","\xe\x04Z","\x01\x05加工步距     X","\xe\x06Y",
  "\x2b\x00Z轴干涉回避位置","\x2b\x01极限移动","\x2b\x02坐标设定",
  "\x2b\x03加工工程","\x2b\x04消耗补正",
  "\x1c\x02mm","\x1c\x03mm","\x1c\x04mm","\x1c\x05mm","\x27\x05个",
  "\x1c\x06mm","\x27\x06个","\x47\x00mm","\x3e\x01有","\x3e\x02不设定",
  "\x3e\x04有","\x49\x04mm/1根.1孔",""};
  if(!flag){ListLN0();LN_NM[1]=0;
    switch(LN01.sl0){
      case 0:case 1:str[5][15]='Y';str[6][2]='Z';str[7][2]='X';break;
      case 2:case 3:str[5][15]='Z';str[6][2]='X';str[7][2]='Y';break;
      case 4:case 5:str[5][15]='X';str[6][2]='Y';str[7][2]='Z';break;
    }
  }
  if(!flag)for(i=0;*str[i];i++)ListLN2(str[i]);
  for(i=1;i<=_LN_;i++){if(!flag||i==flag)flags[i]=1;else flags[i]=0;}
  for(i=1;i<=_LN_;i++){
    if(!flags[i])continue;
    if(LN031K==(i-1))j=1;else j=0;j|=LN031S[i-1];
    switch(i){
      case 1:List_NM(0x11,0,3,j,LN031.str0);break;
      case 2:ListNM(0x11,1,j,LN031.str1);break;
      case 3:ListNM(0x11,2,j,LN031.str2);break;
      case 4:ListNM(0x11,3,j,LN031.str3);break;
      case 5:ListNM(0x11,4,j,LN031.str4);break;
      case 6:ListNM(0x11,5,j,LN031.str5);break;
      case 7:List_NM(0x22,5,4,j,LN031.str6);break;
      case 8:ListNM(0x11,6,j,LN031.str7);break;
      case 9:List_NM(0x22,6,4,j,LN031.str8);break;
      case 10:ListNM(0x3b,0,j,LN031.str9);break;
      case 11:ListLNSW(0x3b,1,j,LN031.sw0);break;
      case 12:ListLNSW(0x3b,2,j,LN031.sw1);break;
      case 13:LNStr.sl5=LN031.sl0;ListStr5(0x3b,3,j);break;
      case 14:ListLNSW(0x3b,4,j,LN031.sw2);break;
      case 15:List_NM(0x41,4,8,j,LN031.str10);break;
      default:break;
    }
  }
#undef _LN_
}

void ListLN032(char flag){  /*圆周*/
#define _LN_ __LN032K__
  register i,j;
  char flags[_LN_+1];
  char *str[]={"\x01\x00坐标系","\x01\x01中心位置     X","\x0e\x02Y",
  "\x01\x03加工开始位置 Z","\x01\x04圆半径","\x01\x05起始角度",
  "\x01\x06间隔角度","\x01\x07加工个数",
  "\x2b\x00Z轴干涉回避位置","\x2b\x01极限移动","\x2b\x02坐标设定",
  "\x2b\x03加工工程","\x2b\x04消耗补正",
  "\x1c\x01mm","\x1c\x02mm","\x1c\x03mm","\x1c\x04mm","\x1c\x05°",
  "\x1c\x06°","\x46\x00mm","\x3e\x01有","\x3e\x02不设定","\x3e\x03有",
  "\x3e\x04有","\x49\x04mm/1根.1孔",""};
  if(!flag){ListLN0();LN_NM[1]=0;
    switch(LN01.sl0){
      case 0:case 1:str[1][15]='Y';str[2][2]='Z';str[3][15]='X';str[8][2]='X';break;
      case 2:case 3:str[1][15]='Z';str[2][2]='X';str[3][15]='Y';str[8][2]='Y';break;
      case 4:case 5:str[1][15]='X';str[2][2]='Y';str[3][15]='Z';str[8][2]='Z';break;
    }
  }
  if(!flag)for(i=0;*str[i];i++)ListLN2(str[i]);
  for(i=1;i<=_LN_;i++){if(!flag||i==flag)flags[i]=1;else flags[i]=0;}
  for(i=1;i<=_LN_;i++){
    if(!flags[i])continue;
    if(LN032K==(i-1))j=1;else j=0;j|=LN032S[i-1];
    switch(i){
      case 1:ListNM(0x11,0,j,LN032.str0);break;
      case 2:ListNM(0x11,1,j,LN032.str1);break;
      case 3:ListNM(0x11,2,j,LN032.str2);break;
      case 4:ListNM(0x11,3,j,LN032.str3);break;
      case 5:ListNM(0x11,4,j,LN032.str4);break;
      case 6:ListNM(0x11,5,j,LN032.str5);break;
      case 7:ListNM(0x11,6,j,LN032.str6);break;
      case 8:ListNM(0x11,7,j,LN032.str7);break;
      case 9:ListNM(0x3b,0,j,LN032.str8);break;
      case 10:ListLNSW(0x3b,1,j,LN032.sw0);break;
      case 11:ListLNSW(0x3b,2,j,LN032.sw1);break;
      case 12:LNStr.sl5=LN032.sl0;ListStr5(0x3b,3,j);break;
      case 13:ListLNSW(0x3b,4,j,LN032.sw2);break;
      case 14:List_NM(0x41,4,8,j,LN032.str9);break;
      default:break;
    }
  }
#undef _LN_
}

void ListLN033(char flag){  /*任意*/
#define _LN_ __LN033K__
  register i,j;
  char flags[_LN_+1];
  char *str[]={"\x01\x00加工个数","\x01\x01Z轴干涉回避位置",
  "\x01\x02极限移动","\x01\x03坐标设定","\x01\x04加工工程",
  "\x01\x05消耗补正",
  "\x17\x00个数","\x1c\x01mm","\x14\x02有","\x14\x03不设定",
  "\x14\x05有","\x14\x06mm/1根.1孔",""};
  if(!flag){ListLN0();LN_NM[1]=0;str[1][2]=*StrLN1(LN01.sl0);ListLN034(0,0);}
  if(!flag)for(i=0;*str[i];i++)ListLN2(str[i]);
  for(i=1;i<=_LN_;i++){if(!flag||i==flag)flags[i]=1;else flags[i]=0;}
  for(i=1;i<=_LN_;i++){
    if(!flags[i])continue;
    if(LN033K==(i-1))j=1;else j=0;j|=LN033S[i-1];
    switch(i){
      case 1:List_NM(0x11,0,5,j,LN033.str0);break;
      case 2:ListNM(0x11,1,j,LN033.str1);break;
      case 3:ListLNSW(0x11,2,j,LN033.sw0);break;
      case 4:ListLNSW(0x11,3,j,LN033.sw1);break;
      case 5:LNStr.sl5=LN033.sl0;ListStr5(0x11,4,j);break;
      case 6:ListLNSW(0x11,5,j,LN033.sw2);break;
      case 7:List_NM(0xb,6,8,j,LN033.str2);break;
      default:break;
    }
  }
#undef _LN_
}
void ListLN034a(char y,int n){
  register i;
  union long_char l;
  l.c[1]=BkColor.tab1;l.c[0]=Color.tab1;
  itoa(n,DispStr,10);i=y*20;i++;
  Listbar3d(LNX0_3+1,LNY0_3+i,LNX0_3+1+8*3,LNY0_3+i+18,l.l,0,0,1,DispStr);
}
void ListLN034b(char y,char flag,char *str){
  register i;
  union long_char l;
  l.c[3]=BkColor.tab3;l.c[2]=Color.tab3;l.c[1]=BkColor.tab2;l.c[0]=Color.tab2;
  if(flag&&!LN_NM[1])(long)l.l>>=16;
  if(flag&&LN_NM[1]){strcpy(DispStr,str);strcat(DispStr,strspace);DispStr[4]=0;}
  else{strcpy(DispStr,strspace);i=strlen(str);strcpy(DispStr+4-i,str);DispStr[4]=0;}
  i=y*20;i++;
  Listbar3d(LNX0_3+1+4*7,LNY0_3+i,LNX0_3+1+4*7+4*8,LNY0_3+i+18,l.l,LN_NM[1]!=0&flag,0,1,DispStr);
  if(flag){if(LN_NM[1])line(LNX0_3+1+4*7+(LN_NM[1]<<3)-8,LNY0_3+i+16+1,LNX0_3+1+4*7+(LN_NM[1]<<3),LNY0_2+i+16+1);}
}
void ListLN034c(char x,char y,char flag,char *str){
  register i,j;
  union long_char l;
  l.c[3]=BkColor.tab3;l.c[2]=Color.tab3;l.c[1]=BkColor.tab2;l.c[0]=Color.tab2;
  if(flag&&!LN_NM[1])(long)l.l>>=16;
  if(flag&&LN_NM[1]){strcpy(DispStr,str);strcat(DispStr,strspace);DispStr[10]=0;}
  else{strcpy(DispStr,strspace);i=strlen(str);strcpy(DispStr+10-i,str);DispStr[10]=0;}
  i=y*20;i++;j=84*x;
  Listbar3d(LNX0_3+1+8*8+j,LNY0_3+i,LNX0_3+1+8*8+8*10+j,LNY0_3+i+18,(long)l.l,LN_NM[1]!=0&flag,0,1,DispStr);
  if(flag){if(LN_NM[1])line(LNX0_3+1+8*8+j+(LN_NM[1]<<3)-8,LNY0_3+i+16+1,LNX0_3+1+8*8+j+(LN_NM[1]<<3),LNY0_2+i+16+1);}
}
void ListLN034(char x,int y){
  register i,j;
  char *str[5]={"No","Ｇ","Ｘ","Ｙ","Ｚ"};
  char flags[4];
  union long_char l;
  l.c[1]=BkColor.tab1;l.c[0]=Color.tab1;
  if(!y){
    i=atoi(LN033.str0);
    for(j=LN034.ty;j<i;j++){strcpy(LN034A+j*48,LN034.str0);
      strcpy(LN034A+j*48+12,LN034.str1);strcpy(LN034A+j*48+24,LN034.str1);
      strcpy(LN034A+j*48+36,LN034.str1);
    }LN034.ty=i;
    Listbar3d(LNX0_3,LNY0_3,LNX0_3+39*8+2,LNY0_3+20*11,(long)BkColor.tab0<<24,1,0,1,"");
    Listbar3d(LNX0_3+1,LNY0_3+1,LNX0_3+1+8*3,LNY0_3+1+18,l.l,0,0,1,str[0]);
    Listbar3d(LNX0_3+1+4*7,LNY0_3+1,LNX0_3+1+4*7+4*8,LNY0_3+1+18,l.l,0,2,1,str[1]);
    for(j=0;j<3;j++)
    Listbar3d(LNX0_3+1+8*8+84*j,LNY0_3+1,LNX0_3+1+8*8+8*10+84*j,LNY0_3+1+18,(long)l.l,0,2,1,str[j+2]);

    for(i=1,j=LN034.y;i<=10;i++,j++){
      if(j>=LN034.ty)break;
      ListLN034a(i,j+1);
      ListLN034b(i,i==LN034.sy&&LN034.sx==1,LN034A+j*48);
      ListLN034c(0,i,i==LN034.sy&&LN034.sx==2,LN034A+j*48+12);
      ListLN034c(1,i,i==LN034.sy&&LN034.sx==3,LN034A+j*48+24);
      ListLN034c(2,i,i==LN034.sy&&LN034.sx==4,LN034A+j*48+36);
    }
  }
  else{j=LN034.y+y-1;
    for(i=0;i<4;i++){if(!x||i+1==x)flags[i]=1;else flags[i]=0;}
    for(i=0;i<4;i++){
      if(!flags[i])continue;
      switch(i){
    case 0:ListLN034b(y,y==LN034.sy&&LN034.sx==1,LN034A+j*48);break;
    case 1:ListLN034c(0,y,y==LN034.sy&&LN034.sx==2,LN034A+j*48+12);break;
    case 2:ListLN034c(1,y,y==LN034.sy&&LN034.sx==3,LN034A+j*48+24);break;
    case 3:ListLN034c(2,y,y==LN034.sy&&LN034.sx==4,LN034A+j*48+36);break;
      }
    }
  }
}
void ListLN04(char flag){
#define _LN_ __LN04K__
  register i,j,k;
  char flags[_LN_+1];
  long double d;
  long l;
  char *str[]={"\x01\x00加工孔设定","\x01\x01坐标系",
  "\x02\x02加工位置","\x02\x03孔No.","\x02\x04X","\x02\x05Y","\x02\x06Z",
  "\x01\x07比例",""};
  if(!flag){ListLN0();}
  if(!flag)for(i=0;*str[i];i++)ListLN2(str[i]);
  if(flag)flag--;
  for(i=1;i<=_LN_;i++){if(!flag||i==flag)flags[i]=1;else flags[i]=0;}
  k=atoi(LN04.str1);if(k)k--;LN041.n=k;k*=48;
  if(!flag)ListLN041(0);
  for(i=1;i<=_LN_;i++){
    if(!flags[i])continue;
    if(LN04K==(i-1))j=1;else j=0;j|=LN04S[i-1];
    switch(i){
      case 1:List_NM(0x0b,1,6,j,LN04A+k+0);break;
      case 2:ListNM(0x07,3,j,LN04.str1);break;
      case 3:ListNM(0x07,4,j,LN04A+k+12);break;
      case 4:ListNM(0x07,5,j,LN04A+k+24);break;
      case 5:ListNM(0x07,6,j,LN04A+k+36);break;
      case 6:ListNM(0x07,7,j,LN04.str5);break;
      default:break;
    }
  }
    switch(LN01.sl0){
      case 0:case 1:d=CNC_atof(LN04A+k+12);break;
      case 2:case 3:d=CNC_atof(LN04A+k+24);break;
      case 4:case 5:d=CNC_atof(LN04A+k+36);break;
    }
  d*=1000;l=d;
  if(LN01.sl0&1){if(l<LN041.z)ListLNErr(1,LN041.n);}else if(l>LN041.z)ListLNErr(1,LN041.n);
#undef _LN_
}
void ListLN041(int n){
  register i,j;
  int x0,y0,x,y;
  long l;
  double d;
  char *strx,*stry;
  x0=LNX0_4+205;y0=LNY0_4+105;
  switch(LN01.sl0){
    case 0:case 1:strx=LN04A+24;stry=LN04A+36;break;
    case 2:case 3:strx=LN04A+36;stry=LN04A+12;break;
    case 4:case 5:strx=LN04A+12;stry=LN04A+24;break;
  }
  if(!n){
    Listbar3(LNX0_4,LNY0_4,LNX0_4+410,LNY0_4+210,0);
    setfillstyle(1,BkColor.mstr);
    bar(LNX0_4+2,LNY0_4+2,LNX0_4+410-2,LNY0_4+210-2);
    for(i=0;i<LN041.tn;i++){
      d=CNC_atof(strx+i*48);l=d*1000;l-=LN041.x;l*=10;l/=LN041.xy;x=x0+l;
      d=CNC_atof(stry+i*48);l=d*1000;l-=LN041.y;l*=10;l/=LN041.xy;y=y0-l;
      setcolor(0x44);
      circle(x,y,3);
    }
  }i=LN041.n;
  d=CNC_atof(strx+i*48);l=d*1000;l-=LN041.x;l*=10;l/=LN041.xy;x=x0+l;
  d=CNC_atof(stry+i*48);l=d*1000;l-=LN041.y;l*=10;l/=LN041.xy;y=y0-l;
  setcolor(0x28);
  circle(x,y,3);
}
void FFileLN(char *fn,char *str){
  register i;
  FILE *fp;
  char a;
  for(i=0;i<100;i++){
    fn[3]=i/10+'0';fn[4]=i%10+'0';
    fp=fopen(fn,"rb+");
    if(fp!=NULL)str[i]=1;else str[i]=0;
    fclose(fp);
  }
}
void ListLN05(int flag){
  register i,j;
  int x,y;
  long l;
  char *str[3]={"\x7\x08已选中","\x1f\x08已使用","\x37\x08未使用"};
  char nstr[4];
  l=(long)(LNBC0S<<8|(long)LNBC0P<<24|(long)LNCO0<<16|LNCO0);
  nstr[2]=0;
  if(!flag){ListLN0();/*FFileLN(NCStr,LN05A);*/
    for(i=0,y=0;y<5;y++)for(x=0;x<20;x++,i++){
      l&=0xffff00FF;nstr[0]=i/10+'0';nstr[1]=i%10+'0';
      if(LN05A[i])l|=LNBC0S<<8;else l|=LNBC0N<<8;
      Listbar3d(LNX0_2+16+x*32,LNY0_2+y*32,LNX0_2+16+x*32+28,LNY0_2+y*32+28,l,LN05K==i,0,1,nstr);
    }
    l=(long)(LNBC0S<<8|(long)LNBC0P<<24|(long)LNCO0<<16|LNCO0);
    Listbar3d(LNX0_2+16,LNY0_2+160,LNX0_2+16+28,LNY0_2+160+28,l,1,0,1,"");
    Listbar3d(LNX0_2+16+192,LNY0_2+160,LNX0_2+16+192+28,LNY0_2+160+28,l,0,0,1,"");
    l=(long)(LNBC0N<<8|(long)LNBC0P<<24|(long)LNCO0<<16|LNCO0);
    Listbar3d(LNX0_2+16+384,LNY0_2+160,LNX0_2+16+384+28,LNY0_2+160+28,l,0,0,1,"");
    for(i=0;i<3;i++)ListLN2(str[i]);
  }else{
    i=flag-1;x=i%20;y=i/20;
    l&=0xffff00FF;nstr[0]=i/10+'0';nstr[1]=i%10+'0';
    if(LN05A[i])l|=LNBC0S<<8;else l|=LNBC0N<<8;
    Listbar3d(LNX0_2+16+x*32,LNY0_2+y*32,LNX0_2+16+x*32+28,LNY0_2+y*32+28,l,LN05K==i,0,1,nstr);
  }
}
void MakeLN20(void){
  register i;
  LN02.sx=1;LN02.sy=1;LN02.y=0;
  MakeLN21();
  if(FP==NULL){ListLNErr(7,0);return;}
/*  i=*/MakeLN22();
  MakeLN23();
 MakeLN24();
}
void MakeLN21(void){  /*打开文件输入流*/
  register i,j;
  long l;
  long double d;
  char far *fn;
  char temp[5];
  switch(LN01.sl1){
    case 0:fn=tabfilename.name1;break;  /*Cu-St*/
    case 1:fn=tabfilename.name2;break;  /*Gr-St*/
    case 2:fn=tabfilename.name3;break;  /*CuW-WC*/
  }
  itoa(LN00K,temp,10);
  strcat(fn,temp);
  strcpy(DispStr,tabfilename.path);
  strcat(DispStr,fn);strcat(DispStr,extname.tab);
  FP=fopen(DispStr,"rb");  /*打开文件*/
}
int MakeLN22(void){  /*读取加工条件*/
  register s;
  int i,j;
  int ii,k,buttom,top,mianji;
  int flagl[6];/*0 - 1表示粗加工所取参数在文件中的范围，2 - 3 正常，4 - 5细*/
  char temp[10],ch,temp1[100],succed = 0;
  for(i = 0;i < 6;i++){flagl[i] = -1;matile_arr[i] = -1;}
  for(i=0;i<100;i++)tab[i]=(struct _TAB_  *)((char far *)LN02A+80*i);
  mianji = CNC_atof(LN01.str1);
  /*size_t fread( void* buffer, size_t size,size_t count, FILE* stream );
   返回实际读到的个数count*/
  ch=fgetc(FP);
  i = 0;
  while(ch!=EOF && !succed)
  {
       while(ch!=EOF && ch != '<')ch = fgetc(FP);
       /*int fseek(FILE *stream, long offset, int fromwhere); 参数3:0是头 1是当前，2是尾*/
       i = fread(temp1,1,100,FP);
       j = 0;
       if(j < i - 2 && temp1[j] == 'a')/*判断面积*/
        {
            j += 1;/*指向a后面一个字符*/
            while(!isdigit(temp1[j]))j++;
            s = 0;
            while(isdigit(temp1[j]))temp[s++] = temp1[j++];
            temp[s] = '\0';
            buttom = atoi(temp);
            while(!isdigit(temp1[j]))j++;
            s = 0;
            while(isdigit(temp1[j]))temp[s++] = temp1[j++];
            temp[s] = '\0';
            top = atoi(temp);
            if(mianji >= buttom && mianji <= top)
                 {fseek(FP,-98,1);i=fread(LN02A+8000,1,10000,FP);i+=8000;succed = 1;}
        }
        fseek(FP,-99,1);
        ch = fgetc(FP);
  }
  fclose(FP);
  flagl[0]=8000;/*flga[0],从文件输入的数据在数组中的起始位置*/
  j=8000;
   while(LN02A[j] != '<' && j < i)j++;
   while(j <= i - 3 && flagl[1] == -1)/*粗<c> <ec*/
    {
        if(LN02A[j] == '<' && LN02A[j + 1] == 'a')/*下一精度范围的开始*/
            j = i;
        else if(LN02A[j] == '<' && LN02A[j + 1] == 'c' && LN02A[j + 2] == '>')
            {j += 3;flagl[0] = j;} /*开始*/
        else if(LN02A[j] == '<' && LN02A[j + 1] == 'e' && LN02A[j + 2] == 'c')
            {flagl[1] = j - 1;j +=3 ;} /*结束*/
        else
            j++;
    }
    while(j <= i - 3&&flagl[3] == -1)/*正常<n> <en*/
    {
        if(LN02A[j] == '<' && LN02A[j + 1] == 'a')/*下一精度范围的开始*/
            j = i;
        else if(LN02A[j] == '<' && LN02A[j + 1] == 'n' && LN02A[j + 2] == '>')
            {j += 3;flagl[2] = j;} /*开始*/
        else if(LN02A[j] == '<' && LN02A[j + 1] == 'e' && LN02A[j + 2] == 'n')
            {flagl[3] = j - 1;j += 3;} /*结束*/
        else
             j++;
    }
    while(j<= i - 3&&flagl[5] == -1)/*细<f> <ef*/
    {
        if(LN02A[j] == '<' && LN02A[j + 1] == 'a')/*下一精度范围的开始*/
            j = i;
        else if(LN02A[j] == '<' && LN02A[j + 1] == 'f' && LN02A[j + 2] == '>')
            {j += 3;flagl[4] = j;} /*开始*/
        else if(LN02A[j] == '<' && LN02A[j + 1] == 'e' && LN02A[j + 2] == 'f')
            {flagl[5] = j - 1;j  = i;} /*结束*/
        else
            j++;
    }
  k = 0;
  s = 0;
  while(s < 5)
  {
  if(flagl[s] != -1 && flagl[s+1] != -1)/*存在加工条件*/
  {
  matile_arr[s] = k;
  for(j=flagl[s];j<flagl[s + 1];){
    do{if(LN02A[j]==' '||LN02A[j]=='\t')j++;}
      while(LN02A[j]==' '||LN02A[j]=='\t');
    switch(LN02A[j]){
      case '(':
    if(LN02A[j]=='(')while(LN02A[j++]!=';');break;
      case 'C':
      case 'c':
    ii=j;strncpy(tab[k]->no,LN02A+j,tablen.no);while(LN02A[j++]!=' ');tab[k]->no[j-ii-1]=0;while(LN02A[j]==' ')j++;
    j++;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tab[k]->pl,LN02A+j,tablen.pl);while(LN02A[j++]!=' ');tab[k]->pl[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tab[k]->ip,LN02A+j,tablen.ip);while(LN02A[j++]!=' ');tab[k]->ip[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tab[k]->on,LN02A+j,tablen.on);while(LN02A[j++]!=' ');tab[k]->on[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tab[k]->off,LN02A+j,tablen.off);while(LN02A[j++]!=' ');tab[k]->off[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tab[k]->sv,LN02A+j,tablen.sv);while(LN02A[j++]!=' ');tab[k]->sv[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tab[k]->s,LN02A+j,tablen.s);while(LN02A[j++]!=' ');tab[k]->s[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tab[k]->up,LN02A+j,tablen.up);while(LN02A[j++]!=' ');tab[k]->up[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tab[k]->dn,LN02A+j,tablen.dn);while(LN02A[j++]!=' ');tab[k]->dn[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tab[k]->v,LN02A+j,tablen.v);while(LN02A[j++]!=' ');tab[k]->v[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tab[k]->lc,LN02A+j,tablen.lc);while(LN02A[j++]!=' ');tab[k]->lc[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tab[k]->pp,LN02A+j,tablen.pp);while(LN02A[j++]!=' ');tab[k]->pp[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tab[k]->lns,LN02A+j,tablen.lns);while(LN02A[j++]!=' ');tab[k]->lns[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tab[k]->step,LN02A+j,tablen.step);while(LN02A[j++]!=' ');tab[k]->step[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tab[k]->lnm,LN02A+j,tablen.lnm);while(LN02A[j++]!=' ');tab[k]->lnm[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tab[k]->b_ofs,LN02A+j,tablen.b_ofs);while(LN02A[j++]!=' ');tab[k]->b_ofs[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tab[k]->s_ofs,LN02A+j,tablen.s_ofs);while(LN02A[j++]!=' ');tab[k]->s_ofs[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tab[k]->ry,LN02A+j,tablen.ry);while(LN02A[j]!=' '&&LN02A[j]!=';')j++;tab[k]->ry[j-ii]=0;while(LN02A[j]==' ')j++;
    while(LN02A[j++]!=';');k++;
    break;
      default:j++;
    }
    do{if(LN02A[j]=='\r'||LN02A[j]=='\n')j++;}
      while(LN02A[j]=='\r'||LN02A[j]=='\n');
  }
  matile_arr[s+1] = k - 1;
  }
   s += 2;
  }
  LN02.fileline=k;
  return k;
}
int MakeLN23(void){
  register i,j,s;
  int ry,ip,ovc,ovcy,top,but;
  int mry,mip = 1000,movc,movcy,stry;
  int line0=-1,line1=-1;
  char temp[80],k;
  long l;
  double d;
  d=CNC_atof(LN01.str1);
  l=d;l/=10;l+=2;if(l>1000)l=1000;i=l;/*投影面积*/
  if(LN01.sw1){j=atoi(LN01.str6);j*=2;mip=j;}/*IP（电流）限制*/
  d=CNC_atof(LN01.str2);d*=10;mry=d;  /*精糙度*/
  d=CNC_atof(LN01.str3);d*=1000;movc=d;/*减寸量*/
  d=CNC_atof(LN01.str4);d*=1000;if(!LN01.st4)movcy=d;else movcy=0;/*减寸量Y*/
  /*根据减寸量选择第一组*/
  if(LN01.sl4 == 0)/*粗*/
  {
    but = matile_arr[0];top = matile_arr[1];

  }
  else if(LN01.sl4 == 1)/*正常*/
  {
     but = matile_arr[2];top = matile_arr[3];
  }
  else/*细*/
  {
    but = matile_arr[4];top = matile_arr[5];
  }
  for(j = but;j<= top && j>= 0;j++){
        d=CNC_atof(tab[j]->ip);d*=2;i=d;if(i>mip)continue;
        d=CNC_atof(tab[j]->b_ofs);d*=1000;i=d;
        if(i<=movc&&(!movcy||i<=movcy)){line0=j;break;}
   }
  LN02.topline=0;
  if(line0==-1)ListLNErr(2,0);
  else
  {
    /*根据精糙度选择最后一组*/
     if(LN01.sl4 == 0)/*粗*/
    {
        but = matile_arr[2];top = matile_arr[3];
    }
    else /*正常,细*/
    {
        but = matile_arr[4];top = matile_arr[5];
    }
    for(j = but;j <= top;j++)
    {
      d=CNC_atof(tab[j]->ry);d*=10;i=d;
      if(i<=mry){line1=j;break;}
    }
    if(line1==-1)ListLNErr(3,0);
    else
    {
        d = 1.4;
        stry = mry * d;

        k = 0;
        s = 0;
        if(line1 > line0 + 1)
        {
            for(j = line1;j > line0;j--)
                {
                    s = i;
                    d=CNC_atof(tab[j]->ry);d*=10;i=d;
                    if(i  >= stry )
                    {
                        if(j + 1 <= line1)
                        {
                            if(abs(s - stry) < abs(i - stry) && ((k && temp[k - 1] != j + 1)||!k))
                            {
                                temp[k++]=j + 1;stry = s;d = 1.4;stry *= d;
                            }
                            else
                            {temp[k++]=j;stry = i;d = 1.4;stry *= d;}
                        }
                        else
                        {temp[k++]=j;stry = i;d = 1.4;stry *= d;}
                    }
                }
        }
        LN02.topline=k+2;
    }
  }
  if(line1!=-1)
  {
    i = 0;
    memcpy(LN02A+10000+i*80,tab[line0],tablen.tablen);
    for(i=1;i<= k;i++)memcpy(LN02A+10000+i*80,tab[temp[k - i]],tablen.tablen);
    memcpy(LN02A+10000+i*80,tab[line1],tablen.tablen);
  }
  else  for(i=0;i<=80;i++)LN02A[i]=0;
  for(i=0;i<100;i++)tab[i]=(struct _TAB_  *)((char far *)LN02A+10000+80*i);
  return ;
}
void MakeLN24(void){
  register k;
  strcpy(DispStr,tabfilename.path);
  strcat(DispStr,tabfilename.name0);
  strcat(DispStr,extname.tab);
  FP=fopen(DispStr,"wb+");
  if(FP!=NULL){ListMess1("文件已保存");}
  else return;
  for(k=0;k<LN02.topline;k++){
    fprintf(FP,"%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s;\r\n",tab[k]->no,
    tab[k]->pl,tab[k]->ip,tab[k]->on,tab[k]->off,tab[k]->sv,tab[k]->s,tab[k]->up,
    tab[k]->dn,tab[k]->v,tab[k]->lc,tab[k]->pp,tab[k]->b_ofs,tab[k]->s_ofs,tab[k]->lns,
    tab[k]->step,tab[k]->lnm,tab[k]->ry,tab[k]->timer);
  }fclose(FP);
}
void MakeLN25(void){/*删除*/
  register i,j;
  for(i=LN02.y+LN02.sy;i<LN02.topline;i++)memcpy(tab[i-1],tab[i],tablen.tablen);
  if(LN02.y){LN02.y--;if(LN02.sy<10)LN02.sy++;}
  if(LN02.topline)LN02.topline--;if(LN02.sy>LN02.topline)LN02.sy=LN02.topline;
  ListLN02(0,0);
}
void MakeLN26(void){/*插入*/
  register i,j;
  if(LN02.topline>=100){ListLNErr(4,0);return;}
  if(!LN02.topline)memcpy(tab[0],&tabpc,tablen.tablen);
  for(i=LN02.topline;i>=LN02.y+LN02.sy;i--)if(i)memcpy(tab[i],tab[i-1],tablen.tablen);
  if(!LN02.sy)LN02.sy++;
  LN02.topline++;
  ListLN02(0,0);
}
int MakeLN0(void){
  register k,j;
  int i0,i1,ix,iy,ix1,iy1;
  long l,l0,l1;
  long double d,d0,d1,dx,dy,dz,dx1,dy1,dz1,dr;
/*  long double dd;
  dd=CNC_atof(LN01.str0);*/
  switch(LNStr.sl0){
    case 0:/*1孔*/
      if(LN030.sw0||LN030.sw1){
    strcpy(LN04A,LN030.str0);strcpy(LN04A+12,LN030.str1);
    strcpy(LN04A+24,LN030.str2);strcpy(LN04A+36,LN030.str3);
      }else{LN04A[0]=0;LN04A[12]=0;LN04A[24]=0;LN04A[36]=0;}
      j=1;
    break;
    case 1:/*格子*/
      j=atoi(LN031.str0);
      switch(LN01.sl0){
    case 0:case 1:
      dx=CNC_atof(LN031.str3);dy=CNC_atof(LN031.str4);dz=CNC_atof(LN031.str2);break;
    case 2:case 3:
      dx=CNC_atof(LN031.str4);dy=CNC_atof(LN031.str2);dz=CNC_atof(LN031.str3);break;
    case 4:case 5:
      dx=CNC_atof(LN031.str2);dy=CNC_atof(LN031.str3);dz=CNC_atof(LN031.str4);break;
      }
      dx1=CNC_atof(LN031.str5);dy1=CNC_atof(LN031.str7);
/*      if(LN031.sw2)dz1=CNC_atof(LN031.str10);else dz1=0;*/
      ix=atoi(LN031.str6);iy=atoi(LN031.str8);
      switch(j){
    case 1:dx1=-dx1;dy1=-dy1;break;
    case 2:dx1=-dx1;dy1= dy1;break;
    case 3:dx1= dx1;dy1=-dy1;break;
    case 4:dx1= dx1;dy1= dy1;break;
      }d=dz;
      for(d0=dx,j=0,k=0,i0=ix;i0;i0--){
    for(d1=dy,i1=iy;i1;i1--){
      if(j>=200)break;
      strcpy(LN04A+k,LN031.str1);
      switch(LN01.sl0){
      case 0:case 1:
        ftoa(d0,LN04A+k+24);ftoa(d1,LN04A+k+36);ftoa(d,LN04A+k+12);break;
      case 2:case 3:
        ftoa(d0,LN04A+k+36);ftoa(d1,LN04A+k+12);ftoa(d,LN04A+k+24);break;
      case 4:case 5:
        ftoa(d0,LN04A+k+12);ftoa(d1,LN04A+k+24);ftoa(d,LN04A+k+36);
      }
/*    if(LN01.sl0&1){if(d>dd)err=1;}else if(d<dd)err=1;*/
      d1+=dy1;
/*    if(LN01.sl0&1)d-=dz1; else d+=dz1;*/
      j++;k+=48;
    }d0+=dx1;
      }
    break;
    case 2:/*圆周*/
      j=atoi(LN032.str7);
      switch(LN01.sl0){
    case 0:case 1:
      dx=CNC_atof(LN032.str2);dy=CNC_atof(LN032.str3);dz=CNC_atof(LN032.str1);break;
    case 2:case 3:
      dx=CNC_atof(LN032.str3);dy=CNC_atof(LN032.str1);dz=CNC_atof(LN032.str2);break;
    case 4:case 5:
      dx=CNC_atof(LN032.str1);dy=CNC_atof(LN032.str2);dz=CNC_atof(LN032.str3);
          break;
      }
      dr=CNC_atof(LN032.str4);dx1=CNC_atof(LN032.str5);dy1=CNC_atof(LN032.str6);
/*      if(LN031.sw2)dz1=CNC_atof(LN031.str10);else dz1=0;*/
      for(k=0,i0=0;i0<j;i0++,k+=48){
    strcpy(LN04A+k,LN032.str0);
    d=dx1*PI;d/=180;
    d=sin(d);d*=dr;d0=d+dx;
    d=dx1*PI;d/=180;
    d=cos(d);d*=dr;d+=dx;
    switch(LN01.sl0){
      case 0:case 1:
        ftoa(d0,LN04A+k+24);ftoa(d,LN04A+k+36);ftoa(dz,LN04A+k+12);break;
      case 2:case 3:
        ftoa(d0,LN04A+k+36);ftoa(d,LN04A+k+12);ftoa(dz,LN04A+k+24);break;
      case 4:case 5:
        ftoa(d0,LN04A+k+12);ftoa(d,LN04A+k+24);ftoa(dz,LN04A+k+36);break;
    }
/*    if(LN01.sl0&1){if(dz>dd)err=1;}else if(dz<dd)err=1;*/
/*  if(LN01.sl0&1)dz-=dz1;else dz+=dz1;*/
    dx1+=dy1;
      }
    break;
    case 3:/*任意*/
      j=atoi(LN033.str0);
/*      if(LN033.sw2)dz1=CNC_atof(LN033.str2);else dz1=0;*/
      for(k=0,i0=0;i0<j;i0++,k+=48){
    strcpy(LN04A+k,LN034A+k);
    strcpy(LN04A+k+12,LN034A+k+12);
    strcpy(LN04A+k+24,LN034A+k+24);
    strcpy(LN04A+k+36,LN034A+k+36);
/*  dz=CNC_atof(LN034A+k+36);
    if(LN01.sl0&1)dz-=dz1;else dz+=dz1;
    ftoa(dz,LN04A+k+36);*/
/*  switch(LN01.sl0){
      case 0:case 1:dz=CNC_atof(LN034A+k+12);break;
      case 2:case 3:dz=CNC_atof(LN034A+k+24);break;
      case 4:case 5:dz=CNC_atof(LN034A+k+36);break;
    }
    if(LN01.sl0&1){if(dz>dd)err=1;}else if(dz<dd)err=1;*/
      }
    break;
  }
  LN041.tn=j;
  return j;
}
void MakeLN1(void){
  register i,j,k;
  double d,dx,dx1,dy,dy1;
  long lx,ly,l;
  char *strx,*stry;
  switch(LN01.sl0){
    case 0:case 1:strx=LN04A+24;stry=LN04A+36;break;
    case 2:case 3:strx=LN04A+36;stry=LN04A+12;break;
    case 4:case 5:strx=LN04A+12;stry=LN04A+24;break;
  }
  i=MakeLN0();
  dx=CNC_atof(strx);dx1=dx;dy=CNC_atof(stry);dy1=dy;
  for(k=0,j=0;j<i;j++,k+=48){
    d=CNC_atof(strx+k);if(d<dx)dx=d;else if(d>dx1)dx1=d;
    d=CNC_atof(stry+k);if(d<dy)dy=d;else if(d>dy1)dy1=d;
  }
  d=CNC_atof(LN01.str0);d*=1000;LN041.z=d;
  LN041.x0=dx*1000;LN041.x1=dx1*1000;
  LN041.y0=dy*1000;LN041.y1=dy1*1000;
  MakeLN2();
  MakeLN3();
}
void MakeLN2(void){
  long l,lx,ly;
  LN041.x=(LN041.x0+LN041.x1)/2;LN041.y=(LN041.y0+LN041.y1)/2;
  lx=LN041.x1-LN041.x0;ly=LN041.y1-LN041.y0;ly<<=1;
  if(lx>ly)l=lx;else l=ly;l+=40;LN041.xy=l/40;
}
void MakeLN3(void){
  double d;
  d=LN041.xy;d/=100;
  ftoa(d,DispStr);
  strcpy(LN04.str5,DispStr+1);
}
void MakeLN5(void)
{
  NCStr[3]=LN05K/10+'0';NCStr[4]=LN05K%10+'0';
  FP=fopen(NCStr,"wb+");
}
void MakeLN6(void){/*生成代码*/
  register i,j;
  char xyz[4];
  char gstr[11],strs[11];
  char *str;
  char *sstr="NC程序已生成,位置是00";
  char ssw,zsw,*zstr,*bstr;
  long l,l0;
  long double d;
  if(!LN02.topline){ListLNErr(5,0);return;}
  MakeLN5();
  if(FP==NULL){ListLNErr(8,0);return;}
  MakeLN61();
  i=LNStr.sl1/2;
  switch(i){
    case 0:strcpy(xyz,"YZX");break;
    case 1:strcpy(xyz,"ZXY");break;
    case 2:strcpy(xyz,"XYZ");break;
  }
  str=LN04A;
  if(!str[0])gstr[0]=0;else{gstr[0]='G';strcpy(gstr+1,str);gstr[1]-=4;gstr[2]-=2;}
  if(LNStr.sl0==0){ /*1孔*/
    if(!LN030.sw2){
      if(LN030.sw1){
    if(LN030.sw3)fprintf(FP,"G25 %s;\r\n",StrLN1(LNStr.sl1^1));
    else fprintf(FP,"%s G00 %c%s;\r\n",gstr,xyz[2],LN030.str4);
    fprintf(FP,"%s G00 G27 %c%s %c%s %c%s;\r\n",gstr,xyz[0],str+12,xyz[1],str+24,xyz[2],LN030.str4);
      }
      else{
    fprintf(FP,"%s G29 %c%s %c%s %c%s;\r\n",gstr,xyz[0],str+12,xyz[1],str+24,xyz[2],str+36);
      }
    }
    fprintf(FP,"P01;\r\nM08;\r\n");
/*    fprintf(FP,"%s G01 G91 %s%s",gstr,StrLN1(LN01.sl0),LN01.str0);*//**/
  }
  else{
    if(LNStr.sl0==1){bstr=LN031.str10;zstr=LN031.str9;zsw=LN031.sw0;ssw=LN031.sw1;}
    if(LNStr.sl0==2){bstr=LN032.str9;zstr=LN032.str8;zsw=LN032.sw0;ssw=LN032.sw1;}
    if(LNStr.sl0==3){bstr=LN033.str2;zstr=LN033.str1;zsw=LN033.sw0;ssw=LN033.sw1;}
    d=CNC_atof(bstr);if(LNStr.sl1&1)d=-d;/*d0=CNC_atof(LN01.str0);*/
    if(!ssw){   /*设定坐标*/
    fprintf(FP,"%s G29 %c%s %c%s %c%s;\r\n",gstr,xyz[0],str+12,xyz[1],str+24,xyz[2],str+36);
    }
    for(j=0,i=0;i<LN041.tn;i++,j+=48){
      str=LN04A+j;
      if(!str[0])gstr[0]=0;else{gstr[0]='G';strcpy(gstr+1,str);gstr[1]-=4;gstr[2]-=2;}
      if(ssw||i){
    if(zsw)fprintf(FP,"G25 %s;\r\n",StrLN1(LNStr.sl1^1));
    else fprintf(FP,"%s G00 G27 %c%s;\r\n",gstr,xyz[2],zstr);
    fprintf(FP,"%s G00 G27 %c%s %c%s %c%s;\r\n",gstr,xyz[0],str+12,xyz[1],str+24,xyz[2],str+36);
      }
      fprintf(FP,"P01;\r\n");
    }
    fprintf(FP,"M08;\r\n");
  }
  MakeLN62(StrLN1(LNStr.sl1^1));
  fclose(FP);
  LN05A[LN05K]=1;
  i=strlen(sstr);sstr[i-2]=LN05K/10+'0';sstr[i-1]=LN05K%10+'0';
  ListLN1(sstr);
}
void MakeLN61(void){
  register k;
  double d0;
  int a,b,s;
  char temp[4];
  fprintf(FP,"(");
  for(k=0;k<__TABLEN__;k++){fprintf(FP,"%s",tabname1[k]);if(k<__TABLEN__-1)fprintf(FP," ");};
  fprintf(FP,");\r\n");
  d0 = CNC_atof(LN01.str3);
  a = d0 * 1000 + 0.1;
  for(k=0;k<LN02.topline;k++){
    tab[k]->lns[0] = LN01.sl2 + '1';
    d0 = CNC_atof(tab[k]->s_ofs);
    b = d0 * 1000 + 0.1; /*加上0.1是为了避免精度丢失*/
    b = abs(a - b);
    temp[0] = b / 100 + '0';b %= 100;temp[1] = b / 10 +'0';temp[2] = b % 10 + '0';
    for(s = 0;s < 3;s++)tab[k]->step[s + 2] = temp[s];
    fprintf(FP,"%s = %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s;\r\n",tab[k]->no,
    tab[k]->pl,tab[k]->ip,tab[k]->on,tab[k]->off,tab[k]->sv,tab[k]->s,tab[k]->up,
    tab[k]->dn,tab[k]->v,tab[k]->lc,tab[k]->pp,tab[k]->b_ofs,tab[k]->s_ofs,tab[k]->lns,
    tab[k]->step,tab[k]->lnm,tab[k]->ry,tab[k]->timer);
 }
}
void MakeLN62(char *sz){  /*生成子程序*/
  register i,j;
  char str[20];
  long double d,d1,dz;
  dz=CNC_atof(LN01.str0);
  fprintf(FP,"N01;\r\n");
  for(i=0;i<LN02.topline - 1;i++){
    d1=CNC_atof(tab[i + 1]->b_ofs);
    if(sz[1]=='+')d=dz+d1;else d=dz-d1;
    ftoa(d,str);
    fprintf(FP,"%s;\r\nG27 G01 %c%s;\r\n",tab[i]->no,sz[0],str);
  }
  ftoa(dz,str);
  fprintf(FP,"%s;\r\nG27 G01 %c%s;\r\n",tab[i]->no,sz[0],str);
  fprintf(FP,"M07;\r\n");
}
void ftoa(long double d,char *str){
  register i;
  long l;
  char a[2]=".";
  if(d>9999.999)d=9999.999;
  if(d<-9999.999)d=-9999.999;
  d*=10000;l=d;
  if(l>=0){str[0]='+';l+=5;}else{str[0]='-';l-=5;}
  l=labs(l);i=l/10000;
  itoa(i,str+1,10);i=l%10000;i/=10;strcat(str,a);
  a[0]=i/100+'0';i%=100;strcat(str,a);
  a[0]=i/10+'0';i%=10;strcat(str,a);
  a[0]=i+'0';strcat(str,a);
}
void xltoa(long l,char *str){
  register i,j;
  long ll;
  str[0]=(l<0)?'-':' ';
  ll=9999999+1;
  l=labs(l);i=(l>=ll)?1:0;ll/=10;j=l%1000;
  str[1]=(i||l>=ll)?l/(long)ll+'0':' ';l%=ll;ll/=10;
  str[2]=(i||l>=ll)?l/(long)ll+'0':' ';l%=ll;ll/=10;
  str[3]=(i||l>=ll)?l/(long)ll+'0':' ';l%=ll;ll/=10;
  str[4]=l/(long)ll+'0';
  str[5]='.';
  str[6]=j/100+'0';j%=100;
  str[7]=j/10+'0';j%=10;
  str[8]=j%10+'0';
  str[9]=0;
}

