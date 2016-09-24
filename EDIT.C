#include "znc.h"
#include <ctype.h>
#define EDX0 1
#define EDXE 688
#define EDY0 215
#define EDYE 538
#define EDX1 (EDX0+2)
#define EDY1 (EDY0+2)
#define EDX2 (EDX0+8+10+18) /*procon*/
#define EDY2 (EDY0+18+22)   /*procon*/
void InitEdit(void){
  register i;
  for(i=0;i<11;i++)HF1[4][i]=HF2[4][i];
  edit.length=0;
  edit.topline=1;
  edit.ty=0;
  edit.tp=0;
  edit.x=0;edit.y=0;edit.pointer=0;
  edit.bx0=0;edit.bx1=0;edit.by0=0;edit.by1=0;
  bselect_sta=0;
  Ks4=0;
  Ks5=0;
  EDITA[0]=0;EDITA[1]=0;edit.length=2;
  InitBlock();
}
void InitBlock(void){
  edit.bx0=0;edit.bx1=0;edit.by0=0;edit.by1=0;edit.bl0=0;edit.bl1=0;
  HF1[4][2]=0;
  bselect_sta=0;
}
void ListED0(){
   int i;
   DispStr[0]=0;
   Listbar3d(EDX0,EDY0,EDXE,EDYE,(long)BkColor.edit<<8,0,0,1,DispStr);
}
void ListED1(char lines,char *str,char flag){
  register i,j;
  int ii,jj;
  unsigned color,color1;
  char ln[10],col[10],temp[50];
  if(!flag)
    color=Color.edit|BkColor.edit<<8;
  else
    color=15;
  color1=color;
  if(lines+edit.ty>edit.by0&&lines+edit.ty<edit.by1)
  color=Color.eblock|BkColor.eblock<<8;
  i=lines*16;j=0;jj=0;
  strncpy(DispStr,str,EDL);DispStr[EDL]=0;
  if((lines+edit.ty==edit.by0||lines+edit.ty==edit.by1)&&edit.bl1){
    color=Color.eblock|BkColor.eblock<<8;
    j=EDX1;
    if(lines+edit.ty==edit.by0){
      j=edit.bx0;if(j>EDL)j=EDL;DispStr[j]=0;jj=j;j<<=3;
      j+=EDX1;if(j>EDXE-2)j=EDXE-2;
      Listbar(EDX1,EDY1+i,j,EDY1+i+15,color1,2,DispStr);
      strncpy(DispStr,str+edit.bx0,EDL);
    }
    if(lines+edit.ty==edit.by1){
      if(lines+edit.ty!=edit.by0)jj=0;
      ii=j;
      j=edit.bx1-jj;if(j>EDL-jj)j=EDL-jj;DispStr[j]=0;j<<=3;
      j+=ii;if(j>EDXE-2)j=EDXE-2;
      Listbar(ii,EDY1+i,j,EDY1+i+15,color,2,DispStr);
      strncpy(DispStr,str+edit.bx1,EDL);
      if(edit.bx1<EDL){
    DispStr[EDL-edit.bx1]=0;
    Listbar(j,EDY1+i,EDXE-2,EDY1+i+15,color1,2,DispStr);
      }
    }else{
      if(edit.bx0<EDL){
    DispStr[EDL-edit.bx0]=0;
    Listbar(j,EDY1+i,EDXE-2,EDY1+i+15,color,2,DispStr);
      }
    }
  }else{
    Listbar(EDX1,EDY1+i,EDXE-2,EDY1+i+15,color,2,DispStr);
  }
  if(lines==edit.y){
    j=strlen(str);
    if(edit.x>j)edit.x=j;if(edit.x>EDL)edit.x=EDL;
    setcolor(BkColor.erun);
    line(EDX1+(edit.x<<3),EDY1+i+15,EDX1+(edit.x<<3)+8,EDY1+i+15);
    itoa(edit.ty+edit.y+1,ln,10);
    itoa(edit.x+1,col,10);
    strcpy(temp,"行：");
    strcat(temp,ln);
    strcat(temp,"，列：");
    strcat(temp,col);
    ListCRT1(temp);
  }
}
void ListEdit(char lines,char flag){
  register i,j;
  char far *str;
  if(!lines){ListED0();}
  for(i=0,j=edit.tp;i<20;i++){
    if(i+edit.ty<edit.topline){
      if(i==lines-1||!lines)ListED1(i,EDITA+j,flag);j+=strlen(EDITA+j)+2;
    }
  }
}
FILE * OpenF(char *str,char flag){
  char *openstr[2]={"rb","wb+"};
  return fopen(str,openstr[flag]);
}
OpenFile(char flag){
  if(flag==0){
    NCStr[3]=LN05K/10+'0';NCStr[4]=LN05K%10+'0';
    FP=OpenF(NCStr,0);
  }
  if(flag==1){
    NCStr[3]=LN05K/10+'0';NCStr[4]=LN05K%10+'0';
    FP=OpenF(NCStr,1);
    LN05A[LN05K]=1;
  }
  return ;
}
void InsertFile(char flag){
  register unsigned i,j;
  unsigned k,kk,ii;
  long l,ll;
  if(!flag)OpenFile(0);
  else{    FP=OpenF(TempFN,0);  }
  if(FP==NULL){ListEdit(0,0);return;}
  fseek(FP,0,SEEK_END);
  l=ftell(FP);ll=l+edit.length;
  if(ll>=0x8000){ListErr(1);fclose(FP);  ListEdit(0,0);return;}
  i=edit.pointer+edit.x;k=l;
  movmem(EDITA+i,EDITA+i+k,edit.length-i);
  fseek(FP,0,SEEK_SET);
  fread(EDITA+i,1,k,FP);
  fclose(FP);
  edit.length=ll;
  for(kk=i,j=0,ii=0;i<k+kk;i++,ii++){
    if(EDITA[i]==0x0d||EDITA[i]==0x0a||!EDITA[i]){
      j++;EDITA[i++]=0;
      if(EDITA[i]==0x0a||EDITA[i]==0x0d||!EDITA[i])EDITA[i]=0;
      else i--;
      if(ii>EDL)ListErr(2);ii=0;
    }
  }if(ii>EDL)ListErr(2);
  edit.topline+=j;
  BlockInsert(k);
  ListEdit(0,0);
}
void LoadFile(void){
  register unsigned i,j;
  unsigned k;
  InitEdit();
  OpenFile(0);
  if(FP==NULL){ListEdit(0,0);return;}
  k=fread(EDITA,1,0x8000,FP);
  fclose(FP);
  if(k>=0x8000){ListErr(0);}
  edit.length=k;
  for(i=0,j=0;i<k;i++){
    if(EDITA[i]==0x0d||EDITA[i]==0x0a){
      j++;EDITA[i++]=0;if(EDITA[i]==0x0a||EDITA[i]==0x0d)EDITA[i]=0;else i--;
    }
  }edit.topline=j;
  ListEdit(0,0);
}
void SaveFile(void){
  register unsigned i,j;
  unsigned k;
  k=edit.length;
  OpenFile(1);
  if(FP==NULL){ListEdit(0,0);return;}
  for(j=0;j<k;){fprintf(FP,"%s\r\n",EDITA+j);
  while(EDITA[j++]);if(!EDITA[j])j++;
  }
  fclose(FP);
  ListEdit(0,0);
}
void EditBlock(char flag){
  register i,j;
  if(!flag){
    edit.bx0=edit.x;edit.by0=edit.y+edit.ty;
    edit.bx1=edit.x;edit.by1=edit.y+edit.ty;
    if(edit.x==strlen(EDITA+edit.pointer))edit.bx1++;
  }else{
    if((edit.bx0==edit.x||block_sta)&&edit.by0==edit.y+edit.ty){
     InitBlock();return;
    }if(edit.y+edit.ty<=edit.by0){
      edit.by1=edit.by0;edit.by0=edit.y+edit.ty;
      if(edit.by0==edit.by1){
    if(edit.x<edit.bx0){edit.bx0=edit.x;}
    else{edit.bx1=edit.x;
      if(edit.x==strlen(EDITA+edit.pointer))edit.bx1++;
    }
      }
    }else{edit.bx1=edit.x;edit.by1=edit.y+edit.ty;}
    if(block_sta){
      edit.bx0=0;edit.bx1=0;
    }
    BlockConvert(0);
  }
}
void BlockConvert(char flag){
  register i,j;
  int k;
  if(!flag){
    i=edit.by0;j=0;
    while(i--){j+=strlen(EDITA+j)+2;}
    edit.bl0=j+edit.bx0;
    i=edit.by1-edit.by0;
    while(i--){j+=strlen(EDITA+j)+2;}
    edit.bl1=j+edit.bx1;
/*    if(edit.x==strlen(EDITA+j))edit.bl1++;*/
  }else{
    for(i=0,k=0;i<edit.bl0;i++,j++){
      if(EDITA[i]==0){k++;i++;j=0;}
    }edit.by0=k;edit.bx0=j-1;
    for(;i<edit.bl1;i++,j++){
      if(EDITA[i]==0){k++;i++;j=0;}
    }edit.by1=k;edit.bx1=j-1;
  }
}
void BlockInsert(int length){
  register i,j;
  unsigned k;
  if(strlen(EDITA+edit.pointer)>EDL)ListErr(2);
  if(!edit.bl1)return;
  i=edit.y+edit.ty;j=0;
  while(i--)j+=strlen(EDITA+j)+2;j+=edit.x;
  if(j<edit.bl0){edit.bl0+=length;edit.bl1+=length;}
  else{if(j<edit.bl1)edit.bl1+=length;}
  BlockConvert(1);
}
void BlockDel(char flag,int length){ /*块删除*/
  register i,j;
  if(!edit.bl1)return;
  flag=flag;
  i=edit.y+edit.ty;j=0;
  while(i--){j+=strlen(EDITA+j)+2;}j+=edit.x;
/*  if(!flag)j+=length;*/
  if(j<edit.bl0){edit.bl0-=length;edit.bl1-=length;}
  else{if(j<edit.bl1)edit.bl1-=length;}
  BlockConvert(1);
}
void BlockCopy(void){   /*块拷贝*/
  FP=OpenF(TempFN,1);
  if(FP==NULL)return;
  fwrite(EDITA+edit.bl0,1,edit.bl1-edit.bl0,FP);
  fclose(FP);
}
void BlockCut(void){ /*块剪切*/
  register i,j;
  BlockCopy();
  i=edit.y+edit.ty;
  if(i>=edit.by0&&i<=edit.by1)edit.x=edit.bx0;
  i=edit.bl1-edit.bl0;j=edit.by1-edit.by0;
  memcpy(EDITA+edit.bl0,EDITA+edit.bl1,edit.length-edit.bl1+1);
  edit.length-=i;edit.topline-=j;
  if(edit.ty>edit.by0){
    if(edit.tp<edit.bl1)edit.tp+=edit.bl1-edit.tp;edit.tp-=i;
    if(edit.pointer<edit.bl1)edit.pointer+=edit.bl1-edit.pointer;edit.pointer-=i;
    edit.y+=edit.ty;
    if(edit.y<edit.by1)edit.y+=edit.by1-(edit.y);edit.y-=j;
    if(edit.ty<edit.by1)edit.ty+=edit.by1-edit.ty;edit.ty-=j;
    edit.y-=edit.ty;
  }else{
    edit.y+=edit.ty;
    if(edit.y<edit.by1)edit.y+=edit.by1-(edit.y);edit.y-=j;
    edit.y-=edit.ty;
  }
  InitBlock();
  ListEdit(0,0);
}
void BlockPasce(void){/*块插入*/
  InsertFile(1);
}
void FileBackup(void){ /*文件替换*/
  register unsigned i,j;
  OpenFile(0);
  if(FP==NULL){return;}
  BNCStr[3]=LN05K/10+'0';BNCStr[4]=LN05K%10+'0';
  fp1=OpenF(BNCStr,1);
  BFFN[LN05K]=1;
  if(fp1==NULL){return;}
  do{
    i=fread(DispStr,1,0x100,FP);
    fwrite(DispStr,1,i,fp1);
  }while(i==0x100);
  fclose(FP);
  fclose(fp1);
}
void FileRestore(void){
  register unsigned i,j;
  BNCStr[3]=LN05K/10+'0';BNCStr[4]=LN05K%10+'0';
  fp1=OpenF(BNCStr,0);
  if(fp1==NULL)return;
  OpenFile(1);
  if(FP==NULL){return;}
  FFN[LN05K]=1;
  do{
    i=fread(DispStr,1,0x100,fp1);
    fwrite(DispStr,1,i,FP);
  }while(i==0x100);
  fclose(FP);
  fclose(fp1);
}
void FileErase(void){
  NCStr[3]=LN05K/10+'0';NCStr[4]=LN05K%10+'0';
  remove(NCStr);
  FFN[LN05K]=0;
}
void InitProCon(void){
  register i;
  procon.fileline=0;
  procon.topline=0;
  procon.y=0;procon.sx=1;procon.sy=1;
  Ks6=0;
}
void EraseProCon(void){/*删除*/
  register i,j;
  for(i=procon.y+procon.sy;i<procon.topline;i++)memcpy(tabprocon[i-1],tabprocon[i],tablen.tablen);
  if(procon.y){procon.y--;if(procon.sy<10)procon.sy++;}
  if(procon.topline)procon.topline--;if(procon.sy>procon.topline)procon.sy=procon.topline;
  ListProCon(0,0);
}
void InsertProCon(void){/*插入*/
  register i,j;
  if(procon.topline>=100){ListErr(6);return;}
  for(i=procon.topline;i>procon.y+procon.sy;i--)if(i)memcpy(tabprocon[i],tabprocon[i-1],tablen.tablen);
  memcpy(tabprocon[i],&tabpc,tablen.tablen);
  if(!procon.sy)procon.sy++;
  procon.topline++;
  ListProCon(0,0);
}
void LoadProCon(void){
  register i;
  long double d;
  PNCStr[3]=procon.name/10+'0';PNCStr[4]=procon.name%10+'0';
  FP=OpenF(PNCStr,0);
  InitProCon();
  if(FP==NULL){
    procon.topline=0;return;
  }
  fseek(FP,1,SEEK_SET);
  fread(&procon.xyz,1,1,FP);
  fread(&procon.l,1,4,FP);
  fseek(FP,1,SEEK_CUR);
  for(i=0;i<100;i++){if(!fread(tabprocon[i],1,tablen.tablen,FP))break;}
  fclose(FP);
  procon.topline=i;
  d=procon.l;d/=1000;
  ftoa(d,procon.str);
}
void SaveProCon(void){
  register i;
  char test[10];
  PNCStr[3]=procon.name/10+'0';PNCStr[4]=procon.name%10+'0';
  if(procon.topline){
  FP=OpenF(PNCStr,1);
  fprintf(FP,"(");
  fwrite(&procon.xyz,1,1,FP);
  fwrite(&procon.l,1,4,FP);
  fprintf(FP,")");
  for(i=0;i<procon.topline;i++){fwrite(tabprocon[i],1,tablen.tablen,FP);}
  fclose(FP);
  PFFN[procon.name]=1;
  }else{
    remove(PNCStr);PFFN[procon.name]=0;
  }
}
void ListProCona(int x,int y){
  register i,j;
  int sx,sxl;
  union long_char l;
  char *str;
  l.c[3]=BkColor.tab0;
  l.c[1]=BkColor.tab1;l.c[0]=Color.tab1;
  Listbar3d(x,y,x+pctablen.tablen*8+__PCTABLEN__+2+24,y+20*11,l.l,1,0,1,"");
  ListProConc(0,0);
  for(i=1;i<=__PCTABLEN__;i++){
    switch(i){
      case 1:sx=pctablen.timer;sxl=tablen.timer+2;break;
      case 2:sx=pctablen.no;sxl=tablen.no;break;
      case 3:sx=pctablen.b_ofs;sxl=tablen.b_ofs+2;break;
      case 4:sx=pctablen.ip;sxl=tablen.ip;break;
      case 5:sx=pctablen.on;sxl=tablen.on;break;
      case 6:sx=pctablen.off;sxl=tablen.off;break;
      case 7:sx=pctablen.sv;sxl=tablen.sv;break;
      case 8:sx=pctablen.s;sxl=tablen.s;break;
      case 9:sx=pctablen.up;sxl=tablen.up;break;
      case 10:sx=pctablen.dn;sxl=tablen.dn;break;
      case 11:sx=pctablen.v;sxl=tablen.v;break;
      case 12:sx=pctablen.lc;sxl=tablen.lc;break;
      case 13:sx=pctablen.pp;sxl=tablen.pp;break;
      case 14:sx=pctablen.pl;sxl=tablen.pl;break;
      case 15:sx=pctablen.lns;sxl=tablen.lns;break;
      case 16:sx=pctablen.step;sxl=tablen.step;break;
      case 17:sx=pctablen.lnm;sxl=tablen.lnm;break;
    }
    str=tabnamepc[i-1];
    j=sx;j<<=3;j+=i;j+=x;j++;j+=24;
    Listbar3d(j,y+1,j+sxl*8+8,y+1+18,l.l,0,0,1,str);
  }
}
void ListProConab(int x,int y,int i){
  setfillstyle(1,BkColor.tab0);if(i>=10)return;
  bar(x+1,y+i*20+1,x+pctablen.tablen*8+__PCTABLEN__+1+23,y+20*11-1);
}
void ListSW(int x,int y,unsigned color,char sw){
  union long_char l;l.i[1]=color;
  if(sw)strcpy(DispStr,"●");else DispStr[0]=0;
  Listbar3d(x,y,x+15,y+15,(long)l.l,1,0,1,DispStr);
}
void ListProConb(int x,int y){
  register i,j;
  int sx,sxl;
  char flag=0;
  union long_char l;
  char *str;
  j=procon.y+y-1;
  if(x==procon.sx&&y==procon.sy)flag=1;
  switch(x){
    case 0:ListProConc(y,procon.sy+y);
      for(j=1;j<=__PCTABLEN__;j++)ListProConb(j,y);break;
    case 1:sx=pctablen.timer;sxl=tablen.timer+2;str=tabprocon[j]->timer;break;
    case 2:sx=pctablen.no;sxl=tablen.no;str=tabprocon[j]->no;break;
    case 3:sx=pctablen.b_ofs;sxl=tablen.b_ofs+2;str=tabprocon[j]->b_ofs;break;
    case 4:sx=pctablen.ip;sxl=tablen.ip;str=tabprocon[j]->ip;break;
    case 5:sx=pctablen.on;sxl=tablen.on;str=tabprocon[j]->on;break;
    case 6:sx=pctablen.off;sxl=tablen.off;str=tabprocon[j]->off;break;
    case 7:sx=pctablen.sv;sxl=tablen.sv;str=tabprocon[j]->sv;break;
    case 8:sx=pctablen.s;sxl=tablen.s;str=tabprocon[j]->s;break;
    case 9:sx=pctablen.up;sxl=tablen.up;str=tabprocon[j]->up;break;
    case 10:sx=pctablen.dn;sxl=tablen.dn;str=tabprocon[j]->dn;break;
    case 11:sx=pctablen.v;sxl=tablen.v;str=tabprocon[j]->v;break;
    case 12:sx=pctablen.lc;sxl=tablen.lc;str=tabprocon[j]->lc;break;
    case 13:sx=pctablen.pp;sxl=tablen.pp;str=tabprocon[j]->pp;break;
    case 14:sx=pctablen.pl;sxl=tablen.pl;str=tabprocon[j]->pl;break;
    case 15:sx=pctablen.lns;sxl=tablen.lns;str=tabprocon[j]->lns;break;
    case 16:sx=pctablen.step;sxl=tablen.step;str=tabprocon[j]->step;break;
    case 17:sx=pctablen.lnm;sxl=tablen.lnm;str=tabprocon[j]->lnm;break;
  }
  if(!x)return;
  l.c[3]=BkColor.tab3;l.c[2]=Color.tab3;l.c[1]=BkColor.tab2;l.c[0]=Color.tab2;
  if(flag&&!LN_NM[1]){l.l>>=16;}
  if(flag&&LN_NM[1]){strcpy(DispStr,str);strcat(DispStr,strspace);DispStr[sxl+1]=0;}
  else{strcpy(DispStr,strspace);i=strlen(str);strcpy(DispStr+sxl+1-i,str);DispStr[sxl+1]=0;}
  j=sx;j<<=3;j+=x;j++;j+=24;
  i=y*20;i++;
  if(x==1){DispStr[0]=0;if(*str)strcpy(DispStr,"√");}
  Listbar3d(EDX2+j,EDY2+i,EDX2+j+sxl*8+8,EDY2+i+18,l.l,LN_NM[1]!=0&flag,0,1,DispStr);
  if(x==2)j+=8;
  if(x!=1)
  if(flag){if(LN_NM[1])line(EDX2+j+(LN_NM[1]<<3)-8,EDY2+i+16+1,EDX2+j+(LN_NM[1]<<3),EDY2+i+16+1);}
}
void ListProConc(int y,char n){
  union long_char l;
  register i;
  l.c[1]=BkColor.tab1;l.c[0]=Color.tab1;
  if(!y){strcpy(DispStr,"No");}
  else{
    DispStr[0]=n/10+'0';DispStr[1]=n%10+'0';DispStr[2]=0;
  }
  i=y*20;i++;
  Listbar3d(EDX2+2,EDY2+i,EDX2+2+21,EDY2+i+18,l.l,0,0,1,DispStr);
}
void ListProCon0(char flag){
  union long_char l;
  register i;
  char strzb[]="组别 ??";
  char *strs="加工深度";
  char far *str1;
  char str2[11];
  strzb[5] = procon.name/10+'0';
  strzb[6] = procon.name%10+'0';
  l.c[1]=BkColor.mstr;l.c[0]=Color.mstr;
  l.c[3]=BkColor.mstr;l.c[2]=Color.mstr;
  if(!flag)
  {
    strcpy(DispStr,strzb);
    Listbar3d(EDX2+150,EDY2+220,EDX2+230,EDY2+240,(long)l.l,0,0,1,DispStr);
    Listbar(EDX2+250,EDY2+220,EDX2+320,EDY2+240,(long)l.i[0],0,strs);
    Listbar3d(EDX2+323,EDY2+220,EDX2+437,EDY2+240,(long)l.l,1,0,1,procon.str);
    Listbar(EDX2-30,EDY2-10-20,EDX2+40,EDY2-10,(long)l.i[0],0,"加工工程");
    Listbar3d(EDX2+45,EDY2-10-20,EDX2+105,EDY2-10,(long)l.l,1,0,1,StrLN5(procon.sl));
    Listbar(EDX2+110,EDY2-10-20,EDX2+180,EDY2-10,(long)l.i[0],0,"材料组合");
    Listbar3d(EDX2+185,EDY2-10-20,EDX2+245,EDY2-10,(long)l.l,1,0,1,StrLN2(procon.sl0));
    Listbar(EDX2+250,EDY2-10-20,EDX2+320,EDY2-10,(long)l.i[0],0,"投影面积");
    Listbar3d(EDX2+325,EDY2-10-20,EDX2+385,EDY2-10,(long)l.l,1,0,1,procon.str1);
    Listbar(EDX2+390,EDY2-10-20,EDX2+450,EDY2-10,(long)l.i[0],0,"精糙度");
    Listbar3d(EDX2+455,EDY2-10-20,EDX2+515,EDY2-10,(long)l.l,1,0,1,procon.str2);
    Listbar(EDX2+520,EDY2-10-20,EDX2+580,EDY2-10,(long)l.i[0],0,"减寸量");
    Listbar3d(EDX2+585,EDY2-10-20,EDX2+645,EDY2-10,(long)l.l,1,0,1,procon.str3);
  }
  if(flag==1){l.c[1]=BkColor.tab3,l.c[0]=Color.tab3;}
  if(flag==2){l.c[3]=BkColor.tab3,l.c[2]=Color.tab3;}
  Listbar(EDX2+324,EDY2+220+1,EDX2+344,EDY2+240-1,l.i[0],1,StrLN1(procon.xyz));
  if(flag==2&&LN_NM[1])
    {strcpy(DispStr,procon.str);strcat(DispStr,strspace);DispStr[10]=0;}
  else
  {strcpy(DispStr,strspace);i=strlen(procon.str);strcpy(DispStr+10-i,procon.str);DispStr[10]=0;}
  Listbar(EDX2+348,EDY2+220+1,EDX2+435,EDY2+240-1,l.i[1],1,DispStr);
  if(flag==2&&LN_NM[1])
  {line(EDX2+340+(LN_NM[1]<<3),EDY2+240-2,EDX2+348+(LN_NM[1]<<3),EDY2+240-2);}
  if(flag == 3)
  {
    l.c[1]=BkColor.tab3,l.c[0]=Color.tab3;
    Listbar(EDX2+45,EDY2-10-20+1,EDX2+105-1,EDY2-10-1-1,l.i[0],1,StrLN5(procon.sl));
  }
  else if(flag == 4)
  {
    l.c[1]=BkColor.tab3,l.c[0]=Color.tab3;
    Listbar(EDX2+185,EDY2-10-20+1,EDX2+245-1,EDY2-10-1-1,l.i[0],1,StrLN2(procon.sl0));
  }
  else if(flag == 5)
  {
    l.c[1]=BkColor.tab3,l.c[0]=Color.tab3;
    if(LN_NM[1])
        {strcpy(DispStr,procon.str1);strcat(DispStr,strspace);DispStr[7]=0;}
    else
        {strcpy(DispStr,strspace);i=strlen(procon.str1);
        if(i)strcpy(procon.str4,procon.str1);
        else
           {strcpy(procon.str1,procon.str4);i=strlen(procon.str1);}
        strcpy(DispStr+7-i,procon.str1);DispStr[7]=0;}
    Listbar(EDX2+325,EDY2-10-20+1,EDX2+385-1,EDY2-10-1,l.i[0],1,DispStr);
    if(LN_NM[1])
        line(EDX2+317+(LN_NM[1]<<3),EDY2-10-2,EDX2+325+(LN_NM[1]<<3),EDY2-10-2);
  }
  else if(flag == 6)
  {
    l.c[1]=BkColor.tab3,l.c[0]=Color.tab3;
    if(LN_NM[1])
        {strcpy(DispStr,procon.str2);strcat(DispStr,strspace);DispStr[7]=0;}
    else
        {strcpy(DispStr,strspace);i=strlen(procon.str2);
        if(i)strcpy(procon.str5,procon.str2);
        else
           {strcpy(procon.str2,procon.str5);i=strlen(procon.str2);}
        strcpy(DispStr+7-i,procon.str2);DispStr[7]=0;}
    Listbar(EDX2+455,EDY2-10-20+1,EDX2+515-1,EDY2-10-1,l.i[0],1,DispStr);
    if(LN_NM[1])
        line(EDX2+447+(LN_NM[1]<<3),EDY2-10-2,EDX2+455+(LN_NM[1]<<3),EDY2-10-2);
  }
  else if(flag == 7)
  {
    l.c[1]=BkColor.tab3,l.c[0]=Color.tab3;
    if(LN_NM[1])
        {strcpy(DispStr,procon.str3);strcat(DispStr,strspace);DispStr[7]=0;}
    else
        {strcpy(DispStr,strspace);i=strlen(procon.str3);
        if(i)strcpy(procon.str6,procon.str3);
        else
           {strcpy(procon.str3,procon.str6);i=strlen(procon.str3);}
        strcpy(DispStr+7-i,procon.str3);DispStr[7]=0;}
    Listbar(EDX2+585,EDY2-10-20+1,EDX2+645-1,EDY2-10-1,l.i[0],1,DispStr);
    if(LN_NM[1])
        line(EDX2+577+(LN_NM[1]<<3),EDY2-10-2,EDX2+585+(LN_NM[1]<<3),EDY2-10-2);
  }
  else if(flag == 0x43)
  {
    l.c[1]=BkColor.mstr;l.c[0]=Color.mstr;
    Listbar3d(EDX2+45,EDY2-10-20,EDX2+105,EDY2-10,(long)l.l,1,0,1,StrLN5(procon.sl));
  }
  else if(flag == 0x44)
  {
    l.c[1]=BkColor.mstr;l.c[0]=Color.mstr;
    Listbar3d(EDX2+185,EDY2-10-20,EDX2+245,EDY2-10,(long)l.l,1,0,1,StrLN2(procon.sl0));
  }
  else if(flag == 0x45)
  {
    l.c[1]=BkColor.mstr;l.c[0]=Color.mstr;
    Listbar3d(EDX2+325,EDY2-10-20,EDX2+385,EDY2-10,(long)l.l,1,0,1,procon.str1);
  }
  else if(flag == 0x46)
  {
    l.c[1]=BkColor.mstr;l.c[0]=Color.mstr;
    Listbar3d(EDX2+455,EDY2-10-20,EDX2+515,EDY2-10,(long)l.l,1,0,1,procon.str2);
  }
  else if(flag == 0x47)
  {
    l.c[1]=BkColor.mstr;l.c[0]=Color.mstr;
    Listbar3d(EDX2+585,EDY2-10-20,EDX2+645,EDY2-10,(long)l.l,1,0,1,procon.str3);
  }
}
void ListProCon(int x,int y){
#define _LN_ __LN00K__
  register i,j;
  int i1,j1,k;
  if(y==-1){ListLN0();ListProCon0(0);ListProCona(EDX2,EDY2);}
  if(!y||y==-1){i=1;j=procon.topline-procon.y;x=0;}
  else{i=y;j=y;}
  for(;i<=j;i++){
  if(i>10)break;ListProConb(x,i);
  }if(!y)ListProConab(EDX2,EDY2,i);
#undef _LN_
}
void MakeLN21P(void){  /*打开文件输入流*/
  register i,j;
  long l;
  long double d;
  char far *fn;
  char temp[5],Disp[100],temp1[15];
  switch(procon.sl0){
    case 0:strcpy(temp1,tabfilename.name1);break;  /*Cu-St*/
    case 1:strcpy(temp1,tabfilename.name2);break;  /*Gr-St*/
    case 2:strcpy(temp1,tabfilename.name3);break;  /*CuW-WC*/
  }
  itoa(0,temp,10);
  strcat(temp1,temp);
  strcpy(Disp,tabfilename.path);
  strcat(Disp,temp1);strcat(Disp,extname.tab);
  FP=fopen(Disp,"rb");  /*打开文件*/
}

int MakeLN22P(void){  /*读取加工条件*/
  register s;
  int i,j;
  int ii,k,buttom,top,mianji;
  int flagl[6];/*0 - 1表示粗加工所取参数在文件中的范围，2 - 3 正常，4 - 5细*/
  char temp[10],ch,temp1[100],succed = 0;
  FILE *fp;
  for(i = 0;i < 6;i++){flagl[i] = -1;matile_arr[i] = -1;}
  for(i=0;i<100;i++)tabprocon[i]=(struct _TAB_  *)((char far *)LN02A+80*i);
  mianji = CNC_atof(procon.str1);
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
    ii=j;strncpy(tabprocon[k]->no,LN02A+j,tablen.no);while(LN02A[j++]!=' ');tabprocon[k]->no[j-ii-1]=0;while(LN02A[j]==' ')j++;
    j++;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tabprocon[k]->pl,LN02A+j,tablen.pl);while(LN02A[j++]!=' ');tabprocon[k]->pl[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tabprocon[k]->ip,LN02A+j,tablen.ip);while(LN02A[j++]!=' ');tabprocon[k]->ip[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tabprocon[k]->on,LN02A+j,tablen.on);while(LN02A[j++]!=' ');tabprocon[k]->on[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tabprocon[k]->off,LN02A+j,tablen.off);while(LN02A[j++]!=' ');tabprocon[k]->off[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tabprocon[k]->sv,LN02A+j,tablen.sv);while(LN02A[j++]!=' ');tabprocon[k]->sv[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tabprocon[k]->s,LN02A+j,tablen.s);while(LN02A[j++]!=' ');tabprocon[k]->s[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tabprocon[k]->up,LN02A+j,tablen.up);while(LN02A[j++]!=' ');tabprocon[k]->up[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tabprocon[k]->dn,LN02A+j,tablen.dn);while(LN02A[j++]!=' ');tabprocon[k]->dn[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tabprocon[k]->v,LN02A+j,tablen.v);while(LN02A[j++]!=' ');tabprocon[k]->v[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tabprocon[k]->lc,LN02A+j,tablen.lc);while(LN02A[j++]!=' ');tabprocon[k]->lc[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tabprocon[k]->pp,LN02A+j,tablen.pp);while(LN02A[j++]!=' ');tabprocon[k]->pp[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tabprocon[k]->lns,LN02A+j,tablen.lns);while(LN02A[j++]!=' ');tabprocon[k]->lns[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tabprocon[k]->step,LN02A+j,tablen.step);while(LN02A[j++]!=' ');tabprocon[k]->step[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tabprocon[k]->lnm,LN02A+j,tablen.lnm);while(LN02A[j++]!=' ');tabprocon[k]->lnm[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tabprocon[k]->b_ofs,LN02A+j,tablen.b_ofs);while(LN02A[j++]!=' ');tabprocon[k]->b_ofs[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tabprocon[k]->s_ofs,LN02A+j,tablen.s_ofs);while(LN02A[j++]!=' ');tabprocon[k]->s_ofs[j-ii-1]=0;while(LN02A[j]==' ')j++;
    ii=j;strncpy(tabprocon[k]->ry,LN02A+j,tablen.ry);while(LN02A[j]!=' '&&LN02A[j]!=';')j++;tabprocon[k]->ry[j-ii]=0;while(LN02A[j]==' ')j++;
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
  procon.fileline=k;
  return k;
}
int MakeLN23P(void){
  register i,j,s;
  int ry,ip,ovc,ovcy,top,but;
  int mry,mip = 1000,movc,movcy,stry;
  int line0=-1,line1=-1;
  char temp[80],k,test[10];
  long l;
  double d;
  d=CNC_atof(procon.str1);
  l=d;l/=10;l+=2;if(l>1000)l=1000;i=l;/*投影面积*/
  d=CNC_atof(procon.str2);d*=10;mry=d;  /*精糙度*/
  d=CNC_atof(procon.str3);d*=1000;movc=d;/*减寸量*/
  if(procon.sl == 0)/*粗*/
  {
    but = matile_arr[0];top = matile_arr[1];

  }
  else if(procon.sl == 1)/*正常*/
  {
     but = matile_arr[2];top = matile_arr[3];
  }
  else/*细*/
  {
    but = matile_arr[4];top = matile_arr[5];
  }
  for(j = but;j<= top && j>= 0;j++){
        d=CNC_atof(tabprocon[j]->ip);d*=2;i=d;if(i>mip)continue;
        d=CNC_atof(tabprocon[j]->b_ofs);d*=1000;i=d;
        if(i<=movc){line0=j;break;}
   }
  procon.topline=0;
  if(line0==-1)ListLNErr(2,0);
  else
  {
    /*根据精糙度选择最后一组*/
     if(procon.sl == 0)/*粗*/
    {
        but = matile_arr[2];top = matile_arr[3];
    }
    else /*正常,细*/
    {
        but = matile_arr[4];top = matile_arr[5];
    }
    for(j = but;j <= top;j++)
    {
      d=CNC_atof(tabprocon[j]->ry);d*=10;i=d;
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
                    d=CNC_atof(tabprocon[j]->ry);d*=10;i=d;
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
        procon.topline=k+2;
    }
  }
  if(line1!=-1)
  {
    i = 0;
    memcpy(LN02A+10000+i*80,tabprocon[line0],tablen.tablen);
    for(i=1;i<= k;i++)memcpy(LN02A+10000+i*80,tabprocon[temp[k - i]],tablen.tablen);
    memcpy(LN02A+10000+i*80,tabprocon[line1],tablen.tablen);
  }
  else  for(i=0;i<=80;i++)LN02A[i]=0;
  for(i=0;i<100;i++)tabprocon[i]=(struct _TAB_  *)((char far *)LN02A+10000+80*i);
  return ;
}

void Key4a(char flag){
  register i;
  static char str[12]="\0";
  if(flag){str[0]++;
    for(i=0;i<11;i++){
      str[i+1]=HF1[4][i];
      if(i&&i!=6&&i!=7&&i!=8)HF1[4][i]|=2;
    }
  }else{
    if(!str[0])return;
    for(i=0;i<11;i++)HF1[4][i]=str[i+1];
  }ListHF(4,0);
}
void Key4(int k){
/*手动 ＭＤＩ 2*/
  switch(k){
    case kF1:if(Ks4)break;Ks4=0;insert_sta^=1;if(!insert_sta)HF1[HF[HM[0]]][0]=0x12;
    else HF1[HF[HM[0]]][0]=0x30;ListHF0(HM[0],1);
    break;
    case kF2:if(Ks4)break;Ks4=0;block_sta^=1;ListHF0(HM[0],2);
    break;
    case kF3:if(Ks4)break;Ks4=0;bselect_sta++;if(bselect_sta>=3)bselect_sta=0;
      switch(bselect_sta){
    case 0:InitBlock();ListEdit(0,0);/*取消*/
    break;
    case 1:EditBlock(0);/*块首*/
    break;
    case 2:EditBlock(1);ListEdit(0,0);/*块尾*/
    break;
      }
      if(!bselect_sta)HF1[HF[HM[0]]][2]=0;else HF1[HF[HM[0]]][2]=0x01;
      if(bselect_sta==2){HF1[HF[HM[0]]][3]=0;HF1[HF[HM[0]]][4]=0;}
      else{HF1[HF[HM[0]]][3]=2;HF1[HF[HM[0]]][4]=2;}
      ListHF0(HM[0],4);ListHF0(HM[0],5);ListHF0(HM[0],3);
    break;
    case kF4:if(Ks4||!bselect_sta)break;
      BlockCopy();HF2[4][5]=0;HF1[4][5]=0;Ks4=0;
      ListHF0(HM[0],6);break;
    case kF5:if(Ks4||!bselect_sta)break;
      BlockCut();ListHF0(HM[0],3);HF2[4][5]=0;HF1[4][5]=0;Ks4=0;
      ListHF0(HM[0],6);break;
    case kF6:if(Ks4||HF2[4][5])break;
      BlockPasce();Ks4=0;break;
    case kF7:if(Ks4){Key4a(0);}
      if(Ks4==6){Ks4=0;ListEdit(0,0);}
      else{Key4a(1);Ks4=6;HF1[HF[HM[0]]][6]=1;ListHF(4,7);ListLN05(0);}break;
    case kF8:if(Ks4)break;KESCF=5;HF[HM[0]]=5;ListHF0(HM[0],0);break;
    case kF9:InitEdit();ListEdit(0,0);break;
    case kF10:HM[1]=0;HM[0]=0;ListHM(0);KESCF=0;break;
    default:
      switch(Ks4){
    case 6:Key85(k);return;
    default:Key40(k);
    break;
      }
    break;
  }
}
Key401(){
  if(edit.pointer){
    edit.pointer-=2;while(EDITA[--edit.pointer]);edit.pointer++;
    if(edit.pointer<0)edit.pointer=0;
  }
  return ;
}
Key402(){
  edit.pointer+=strlen(EDITA+edit.pointer);edit.pointer+=2;
   return ;
}
Key403(){
    edit.tp-=2;while(EDITA[--edit.tp]);edit.tp++;
    if(edit.tp<0)edit.tp=0;
      edit.pointer-=2;while(EDITA[--edit.pointer]);edit.pointer++;
      if(edit.pointer<0)edit.pointer=0;
       return ;
}
Key404(){
  edit.tp+=strlen(EDITA+edit.tp);edit.tp+=2;
  edit.pointer+=strlen(EDITA+edit.pointer);edit.pointer+=2;
   return ;
}
Key405(int k){
  char far *str;
  register i,j=1;
  i=edit.pointer+edit.x;
  str=EDITA+i;
  switch(k){
    case kDEL:if(i+2>=edit.length)break;
      if(edit.x==strlen(EDITA+edit.pointer)){j++;edit.topline--;}
      memcpy(str,str+j,edit.length-i);edit.length-=j;
      BlockDel(0,j);
      if(j>1)ListEdit(0,0);else ListEdit(edit.y+1,0);
    break;
    case kBS:if(!i)break;
      if(!edit.x){j++;edit.topline--;
    if(edit.y){edit.y--;Key401();}
    else{if(edit.ty){edit.ty--;Key403();}}
    edit.x=strlen(EDITA+edit.pointer)+1;
      }edit.x--;
      memcpy(str-j,str,edit.length-i);edit.length-=j;
      BlockDel(1,j);
      if(j>1)ListEdit(0,0);else ListEdit(edit.y+1,0);
    break;
    case kENT:if(edit.length>0x7ff0){ListErr(1);break;}
      j++;edit.topline++;edit.length+=j;
      movmem(str,str+j,edit.length-i);str[0]=0;str[1]=0;
      BlockInsert(j);
      if(edit.y<20-1){
    if(edit.y+edit.ty<edit.topline-1){edit.y++;Key402();edit.x=0;}
      }else{
    if(edit.ty+edit.y<edit.topline-1){edit.ty++;Key404();edit.x=0;}
      }ListEdit(0,0);
    break;
    default:
    if(k>0xff)return;
    if(edit.length>0x7ff0){ListErr(1);break;}
      if(insert_sta||!str[0]){
    edit.length++;movmem(str,str+j,edit.length-i);
      }BlockInsert(j);
      str[0]=k;edit.x++;
      ListEdit(edit.y+1,0);
    break;
  }
   return ;
}
void Key40(int k){
  register signed i,j;
  char far * str;
  switch(k){
    case kUP:
    if(edit.y){edit.y--;Key401();ListEdit(edit.y+2,0);ListEdit(edit.y+1,0);}
      else{if(edit.ty){edit.ty--;Key403();ListEdit(0,0);}}
    break;
    case kDN:i=edit.ty;j=edit.y;
      if(edit.y<20-1){if(edit.y+edit.ty<edit.topline-1){
      edit.y++;Key402();ListEdit(edit.y,0);ListEdit(edit.y+1,0);}}
      else{if(edit.ty+edit.y<edit.topline-1){edit.ty++;Key404();ListEdit(0,0);}}
    break;
    case kPGUP:if(!edit.y&&!edit.ty)break;
      i=edit.ty;j=edit.y;edit.ty-=(i>=20)?20:i;
      edit.y-=(edit.ty+20>i)?20-(i-edit.ty):0;
      if(edit.y<0)edit.y=0;
      i-=edit.ty;j-=edit.y;
      while(i--){Key403();}
      while(j--){Key401();}
    ListEdit(0,0);
    break;
    case kPGDN:i=edit.ty;j=edit.y;if(i+j>=edit.topline-1)break;
      edit.ty+=(i+20<edit.topline)?20:0;
      edit.y+=(edit.ty+edit.y+20>=edit.topline)?20-(edit.ty-i):0;
      if(edit.y+edit.ty>=edit.topline)edit.y=edit.topline-edit.ty-1;
      i=edit.ty-i;j=edit.y-j;
      if(j>=0)while(j--){Key402();}
      else{j=-j;while(j--){Key401();}}
      while(i--){Key404();}
    ListEdit(0,0);
    break;
    case kLF:if(edit.x){edit.x--;ListEdit(edit.y+1,0);}
      else{
    if(edit.ty||edit.y){
      if(edit.y){edit.y--;Key401();edit.x=strlen(EDITA+edit.pointer);
        ListEdit(edit.y+2,0);ListEdit(edit.y+1,0);}
      else{if(edit.ty){edit.ty--;Key403();
        edit.x=strlen(EDITA+edit.pointer);ListEdit(0,0);}}
    ListEdit(edit.y+1,0);}
      }
    break;
    case kRT:if(edit.x<strlen(EDITA+edit.pointer)&&edit.x<EDL){edit.x++;ListEdit(edit.y+1,0);}
      else{i=edit.ty;j=edit.y;
    if(edit.y<20-1){if(edit.y+edit.ty<edit.topline-1){
    edit.y++;Key402();edit.x=0;ListEdit(edit.y,0);ListEdit(edit.y+1,0);}}
    else{if(edit.ty+edit.y<edit.topline-1){edit.ty++;Key404();edit.x=0;
    ListEdit(0,0);}}
      }
    break;
    case kHOME:edit.x=0;ListEdit(edit.y+1,0);break;
    case kEND:edit.x=strlen(EDITA+edit.pointer);ListEdit(edit.y+1,0);break;
    default:switch(HF[HM[0]]){
     case 4:
     case 5:
     if(k&0xff||k==kDEL)Key405(k);
     break;
     }
    break;
  }
   return ;
}
Key5a(char flag){
  if(!flag){
    if(HF1[HF[HM[0]]][0]){HF1[HF[HM[0]]][0]=0;ListHF0(HM[0],1);}
    if(HF1[HF[HM[0]]][1]){HF1[HF[HM[0]]][1]=0;ListHF0(HM[0],2);}
    if(HF1[HF[HM[0]]][2]){HF1[HF[HM[0]]][2]=0;ListHF0(HM[0],3);}
    if(HF1[HF[HM[0]]][3]){HF1[HF[HM[0]]][3]=0;ListHF0(HM[0],4);}
    if(HF1[HF[HM[0]]][4]){HF1[HF[HM[0]]][4]=0;ListHF0(HM[0],5);}
    LN05A=FFN;
    ListEdit(0,0);
  }else{
    HF1[HF[HM[0]]][flag-1]=1;ListHF0(HM[0],flag);
    if(flag==2)LN05A=BFFN;
    else LN05A=FFN;
    ListLN05(0);
  }
  Ks5=flag;
  return ;
}
void Key5(int k){
/* 编辑 文件 */
  switch(k){
    case kF1:if(Ks5==1){Key5a(0);break;}if(Ks5){HF1[HF[HM[0]]][Ks5-1]=0;
      ListHF0(HM[0],Ks5);}Key5a(1);break;
    case kF2:if(Ks5==2){Key5a(0);break;}if(Ks5){HF1[HF[HM[0]]][Ks5-1]=0;
      ListHF0(HM[0],Ks5);}Key5a(2);break;
    case kF3:if(Ks5==3){Key5a(0);break;}if(Ks5){HF1[HF[HM[0]]][Ks5-1]=0;
      ListHF0(HM[0],Ks5);}Key5a(3);break;
    case kF4:if(Ks5==4){Key5a(0);break;}if(Ks5){HF1[HF[HM[0]]][Ks5-1]=0;
      ListHF0(HM[0],Ks5);}Key5a(4);break;
    case kF5:if(Ks5==5){Key5a(0);break;}if(Ks5){HF1[HF[HM[0]]][Ks5-1]=0;
      ListHF0(HM[0],Ks5);}Key5a(5);break;
    case kF6:if(!save)ListMess1("请重新装载或保存文件后再执行该操作！");
             else{ListHT(1,"正在编译执行...");HF1[HF[HM[0]]][5]=1;ListHF0(HM[0],Ks5);Analyse();HF1[HF[HM[0]]][5]=0;ListHF0(HM[0],Ks5);}
             break;
    case kF7:break;
    case kF8:break;
    case kF9:break;
    case kF10:Key5a(0);KESCF=3;HF[HM[0]]=4;ListHF0(HM[0],0);save=0;break;
    default:
      switch(Ks5){
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:Key85(k);return;
    default:Key40(k);
    break;
      }
    break;
  }
}

void Key6a(char flag){
  register i,j;
  Ks5=flag;
  if(!flag){
    for(i=0;i<10;i++)
    if(HF1[HF[HM[0]]][i]){HF1[HF[HM[0]]][i]=0;/*if(i==0)HF1[HF[HM[0]]][i]=1;*/ListHF0(HM[0],i+1);}
    LN05A=FFN;
    ListProCon(0,-1);
  }
  else{
    for(i=0;i<10;i++){
      if(i!=2)j=2;else j=1;
      HF1[HF[HM[0]]][i]=j;/*if(i==0)HF1[HF[HM[0]]][i]=1;*/ListHF0(HM[0],i+1);
    }LN05A=PFFN;
    SaveProCon();
    ListLN05(0);
  }
  Ks6=flag;
}
Key6b(void){
if(Ks6){Set_HF1(6,0,1);ListHF0(HM[0],0);}
 return ;
}
void Key60(int k){
  register i,j;
  i=procon.y+procon.sy-1;
  switch(procon.sx){
    case  1:NM_Str=tabprocon[i]->timer;LN_NM[0]=tablen.timer;LN_NM[2]=tabtype.timer;break;
    case  2:NM_Str=tabprocon[i]->no;LN_NM[0]=tablen.no-1;LN_NM[2]=tabtype.no;
        NM_Str[0]='C';NM_Str++;break;
    case  3:NM_Str=tabprocon[i]->b_ofs;LN_NM[0]=tablen.b_ofs;LN_NM[2]=tabtype.b_ofs;break;
    case  4:NM_Str=tabprocon[i]->ip;LN_NM[0]=tablen.ip;LN_NM[2]=tabtype.ip;break;
    case  5:NM_Str=tabprocon[i]->on;LN_NM[0]=tablen.on;LN_NM[2]=tabtype.on;break;
    case  6:NM_Str=tabprocon[i]->off;LN_NM[0]=tablen.off;LN_NM[2]=tabtype.off;break;
    case  7:NM_Str=tabprocon[i]->sv;LN_NM[0]=tablen.sv;LN_NM[2]=tabtype.sv;break;
    case  8:NM_Str=tabprocon[i]->s;LN_NM[0]=tablen.s;LN_NM[2]=tabtype.s;break;
    case  9:NM_Str=tabprocon[i]->up;LN_NM[0]=tablen.up;LN_NM[2]=tabtype.up;break;
    case 10:NM_Str=tabprocon[i]->dn;LN_NM[0]=tablen.dn;LN_NM[2]=tabtype.dn;break;
    case 11:NM_Str=tabprocon[i]->v;LN_NM[0]=tablen.v;LN_NM[2]=tabtype.v;break;
    case 12:NM_Str=tabprocon[i]->lc;LN_NM[0]=tablen.lc;LN_NM[2]=tabtype.lc;break;
    case 13:NM_Str=tabprocon[i]->pp;LN_NM[0]=tablen.pp;LN_NM[2]=tabtype.pp;break;
    case 14:NM_Str=tabprocon[i]->pl;LN_NM[0]=tablen.pl;LN_NM[2]=tabtype.pl;break;
    case 15:NM_Str=tabprocon[i]->lns;LN_NM[0]=tablen.lns;LN_NM[2]=tabtype.lns;break;
    case 16:NM_Str=tabprocon[i]->step;LN_NM[0]=tablen.step;LN_NM[2]=tabtype.step;break;
    case 17:NM_Str=tabprocon[i]->lnm;LN_NM[0]=tablen.lnm;LN_NM[2]=tabtype.lnm;break;
  }
  switch(k){
    case kPGUP:if(LN_NM[1]){InputCharE();LN_NM[1]=0;}if(procon.sy+procon.y==1)break;
        if(procon.y>=10){procon.y-=10;ListProCon(0,0);}
        else{procon.sy+=procon.y-10;if(procon.sy<1)procon.sy=1;procon.y=0;ListProCon(0,0);}
    break;
    case kPGDN:if(LN_NM[1]){InputCharE();LN_NM[1]=0;}if(procon.sy+procon.y>=procon.topline)break;
        if(procon.topline<=10)procon.sy=procon.topline;
        else{if(procon.y<=procon.topline-20)procon.y+=10;
          else{procon.sy+=20-(procon.topline-procon.y);
          if(procon.sy>10)procon.sy=10;procon.y=procon.topline-10;}
        }ListProCon(0,0);
    break;
    case kUP:if(LN_NM[1]){InputCharE();LN_NM[1]=0;}if(procon.sy+procon.y==1)break;
        if(procon.sy>1){ListProCon(procon.sx,procon.sy--);ListProCon(procon.sx,procon.sy);}
        else{procon.y--;ListProCon(0,0);}
    break;
    case kDN:if(LN_NM[1]){InputCharE();LN_NM[1]=0;}if(procon.sy+procon.y>=procon.topline)break;
        if(procon.sy<10){ListProCon(procon.sx,procon.sy++);ListProCon(procon.sx,procon.sy);}
        else{procon.y++;ListProCon(0,0);}
    break;
    case kLF:if(LN_NM[1]){if(LN_NM[1]>1)LN_NM[1]--;
    ListProCon(procon.sx,procon.sy);break;}
    if(!procon.topline)break;
        if(procon.sy+procon.y==1&&procon.sx==1)break;
    ListProCon(procon.sx--,procon.sy);if(!procon.sx){procon.sx=__PCTABLEN__;procon.sy--;}
        if(!procon.sy){procon.sy=1;procon.y--;ListProCon(0,0);}
        else ListProCon(procon.sx,procon.sy);break;
    case kRT:if(LN_NM[1]){if(LN_NM[1]<=strlen(NM_Str)&&LN_NM[1]<LN_NM[0])LN_NM[1]++;
    ListProCon(procon.sx,procon.sy);break;}
    if(!procon.topline)break;
    if(procon.sy+procon.y>=procon.topline&&procon.sx==__PCTABLEN__)break;
    ListProCon(procon.sx++,procon.sy);if(procon.sx>__PCTABLEN__){procon.sx=1;procon.sy++;}
        if(procon.sy>10){procon.sy=10;procon.y++;ListProCon(0,0);}
        else ListProCon(procon.sx,procon.sy);break;
    case kHOME:if(LN_NM[1])break;if(procon.sx==1||!procon.topline)break;
        procon.sx=1;ListProCon(0,procon.sy);break;
    case kEND:if(LN_NM[1])break;if(procon.sx==__PCTABLEN__||!procon.topline)break;
    procon.sx=__PCTABLEN__;ListProCon(0,procon.sy);break;
    case kENT:
    if(!procon.topline)break;
      if(procon.sx==1){if(NM_Str[0])NM_Str[0]=0;else NM_Str[0]='0';
    ListProCon(procon.sx,procon.sy);break;}
      if(!LN_NM[1]){LN_NM[1]=1;ListProCon(procon.sx,procon.sy);}
    else{
      InputCharE();LN_NM[1]=0;
      if(procon.sy+procon.y>=procon.topline&&procon.sx==__PCTABLEN__){ListProCon(procon.sx,procon.sy);break;}
      ListProCon(procon.sx++,procon.sy);if(procon.sx>__PCTABLEN__){procon.sx=1;procon.sy++;}
          if(procon.sy>10){procon.sy=10;procon.y++;ListProCon(0,0);}
          else ListProCon(procon.sx,procon.sy);
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
    case kDEL:if(!procon.topline)break;
    InputChar(k);ListProCon(procon.sx,procon.sy);
    break;
  }
}
void Key64(int k){
  register i;
  i=0;
  switch(k){
    case kLF:if(procon.xyz)procon.xyz--;else procon.xyz=5;break;
    case kRT:procon.xyz++;if(procon.xyz>=6)procon.xyz=0;break;
    case kHOME:procon.xyz=0;break;
    case kEND:procon.xyz=5;break;
    case '+':procon.xyz&=0xfe;break;
    case '-':procon.xyz|=1;break;
    case 'x':
    case 'X':procon.xyz&=1;break;
    case 'y':
    case 'Y':procon.xyz&=1;procon.xyz+=2;break;
    case 'z':
    case 'Z':procon.xyz&=1;procon.xyz+=4;break;
    case kENT:i++;break;
  }
  ListProCon0(1);
  if(i)Key6(kF4);
}
void Key65(int k){
  register i;
  LN_NM[2]=0;
  LN_NM[0]=9;
  NM_Str=procon.str;
  i=0;
  switch(k){
    case kLF:
      if(!LN_NM[1])LN_NM[1]=1;else if(LN_NM[1]>1)LN_NM[1]--;break;
    case kRT:
      if(LN_NM[1]<=strlen(NM_Str)&&LN_NM[1]<LN_NM[0])LN_NM[1]++;
    break;
    case kENT:
      i++;
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
  ListProCon0(2);
  if(i){Key6(kF5);}
}
void Key67(int k){
  register i;
    i=0;
  switch(k){    
    case kUP:
    if(procon.sl1 == 2)
        strcpy(procon.str1,procon.str4);
    else if(procon.sl1 == 3)
        strcpy(procon.str2,procon.str5);
    else if(procon.sl1 == 4)
        strcpy(procon.str3,procon.str6);
    ListProCon0(0x43+procon.sl1);
    if(procon.sl1 > 0)procon.sl1--;else procon.sl1 = 4;ListProCon0(procon.sl1+3);
    if(procon.sl1 == 2)
        {procon.str1[0] = 0;NM_Str=procon.str1;LN_NM[2]=23;LN_NM[0]=6;LN_NM[1]=1;}
    else if(procon.sl1 == 3)
        {procon.str2[0] = 0;NM_Str=procon.str2;LN_NM[2]=24;LN_NM[0]=6;LN_NM[1]=1;}
    else if(procon.sl1 == 4)
        {procon.str3[0] = 0;NM_Str=procon.str3;LN_NM[2]=11;LN_NM[0]=6;LN_NM[1]=1;}
    break;
    case kDN:
    if(procon.sl1 == 2)
        strcpy(procon.str1,procon.str4);
    else if(procon.sl1 == 3)
        strcpy(procon.str2,procon.str5);
    else if(procon.sl1 == 4)
        strcpy(procon.str3,procon.str6);
    ListProCon0(0x43+procon.sl1);
    if(procon.sl1 < 4)procon.sl1++;else procon.sl1 = 0;ListProCon0(procon.sl1+3);
    if(procon.sl1 == 2)
        {procon.str1[0] = 0;NM_Str=procon.str1;LN_NM[2]=23;LN_NM[0]=6;LN_NM[1]=1;}
    else if(procon.sl1 == 3)
        {procon.str2[0] = 0;NM_Str=procon.str2;LN_NM[2]=24;LN_NM[0]=6;LN_NM[1]=1;}
    else if(procon.sl1 == 4)
        {procon.str3[0] = 0;NM_Str=procon.str3;LN_NM[2]=11;LN_NM[0]=6;LN_NM[1]=1;}
    break;
    case kLF:if(procon.sl1>1){if(!LN_NM[1])LN_NM[1]=1;else if(LN_NM[1]>1)LN_NM[1]--;}
    else if(procon.sl1 == 0){if(procon.sl > 0)procon.sl--;else procon.sl = 2;ListProCon0(3);}
    else if(procon.sl1 == 1){if(procon.sl0 > 0)procon.sl0--;else procon.sl0 = 2;ListProCon0(4);}
    break;
    case kRT:if(procon.sl1>1){if(LN_NM[1]<=strlen(NM_Str)&&LN_NM[1]<LN_NM[0])LN_NM[1]++;}
    else if(procon.sl1 == 0){if(procon.sl < 2)procon.sl++;else procon.sl = 0;ListProCon0(3);}
    else if(procon.sl1 == 1){if(procon.sl0 < 2)procon.sl0++;else procon.sl0 = 0;ListProCon0(4);}
    break;
    case kENT:
     InputCharE();
    i++;
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
    if(procon.sl1 == 2)ListProCon0(5);
    else if(procon.sl1 == 3)ListProCon0(6);
    else if(procon.sl1 == 4)ListProCon0(7);
}
void Key6(int k){
  long double d;
  register i;
  i=0;
  switch(k){
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
    case kF11:if(LN_NM[1]){InputCharE();if(k != kF5)ListProCon(procon.sx,procon.sy);}i++;
    break;
  }
  if(Ks6==5&&i){
      d=CNC_atof(procon.str);d*=1000;procon.l=d;ListProCon0(2);
  }
  if(Ks6==4&&i)ListProCon0(0);
  if(Ks6==3&&k!=kF3){Key85(k);return;}
  switch(k){
    case kF1:if(Ks6==3)break;Key6b();Ks6=0;ListProCon0(0);InsertProCon();break;
    case kF2:if(Ks6==3)break;Key6b();Ks6=0;ListProCon0(0);EraseProCon();break;
    case kF3:Key6b();if(Ks6==3)Key6a(0);else {Key6a(3);ListProCon0(0);}break;
    case kF4:if(Ks6==3)break;if(Ks6!=4){ListProCon0(0);
    Ks6=4;Set_HF1(6,0,1);HF1[6][3]=1;ListHF0(HM[0],0);
    ListProCon0(1);
      }else{Ks6=0;Set_HF1(6,0,1);ListHF0(HM[0],Ks6);}
    break;
    case kF5:if(Ks6==3)break;if(Ks6!=5){ListProCon0(0);
    Ks6=5;Set_HF1(6,0,1);HF1[6][4]=1;ListHF0(HM[0],0);
    LN_NM[1]=1;LN_NM[2]=0;LN_NM[0]=9;NM_Str=procon.str;
    procon.str[0]=0;ListProCon0(2);
    }else{Ks6=0;Set_HF1(6,0,1);ListHF0(HM[0],0);}
    break;
    case kF6:if(Ks6==3)break;Key6b();Ks6=0;SaveProCon();break;
    case kF7:if(Ks6==3)break;if(Ks6 != 7){
      Ks6 = 7;Set_HF1(6,0,1);HF1[HF[HM[0]]][Ks6-1]=1;ListHF0(HM[0],0);procon.sl1 = 0;ListProCon0(3);
    }else{Ks6 = 0;Set_HF1(6,0,1);ListHF0(HM[0],0);ListProCon0(0);}
    break;
    case kF8:if(Ks6 != 7){ListMess1("请选择加工计划！");break;}
    Ks6 = 0;Set_HF1(6,0,1);ListHF0(HM[0],Ks6);ListProCon0(0);
    MakeLN21P();
    if(FP==NULL){ListLNErr(7,0);break;}
    MakeLN22P();
    MakeLN23P();
    SaveProCon();
    LoadProCon();
    ListProCon(0,-1);
    break;
    case kF10:HM[1]=0;HM[0]=0;ListHM(0);KESCF=0;Set_disp();break;
    default:
      switch(Ks6){
    case 4:Key64(k);break;
    case 5:Key65(k);break;
    case 7:Key67(k);break;
    default:
    Key60(k);break;
      }
    }
}
#include "stdio.h"
#include "graphics.h"
void SaveBMP256(void){
#if TESTING
char *R=
"\x00\x00\x00\x00\x27\x27\x27\x27\x13\x13\x13\x13\x3f\x3f\x3f\x3f"
"\x00\x03\x07\x0b\x0f\x13\x17\x1b\x1f\x23\x27\x2b\x2f\x33\x37\x3b"
"\x00\x0f\x1f\x2f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x2f\x1f\x0f"
"\x00\x00\x00\x00\x00\x00\x00\x00\x1f\x27\x2f\x37\x3f\x3f\x3f\x3f"
"\x3f\x3f\x3f\x3f\x3f\x37\x2f\x27\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f"
"\x2f\x33\x37\x3b\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3b\x37\x33"
"\x2f\x2f\x2f\x2f\x2f\x2f\x2f\x2f\x00\x07\x0f\x17\x1f\x1f\x1f\x1f"
"\x1f\x1f\x1f\x1f\x1f\x17\x0f\x07\x00\x00\x00\x00\x00\x00\x00\x00"
"\x0f\x13\x17\x1b\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1b\x17\x13"
"\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x13\x15\x17\x19\x1b\x1b\x1b\x1b"
"\x1b\x1b\x1b\x1b\x1b\x19\x17\x15\x13\x13\x13\x13\x13\x13\x13\x13"
"\x03\x07\x0b\x0f\x13\x13\x13\x13\x13\x13\x13\x13\x13\x0f\x0b\x07"
"\x03\x03\x03\x03\x03\x03\x03\x03\x07\x09\x0b\x0d\x0f\x0f\x0f\x0f"
"\x0f\x0f\x0f\x0f\x0f\x0d\x0b\x09\x07\x07\x07\x07\x07\x07\x07\x07"
"\x09\x0a\x0b\x0c\x0d\x0d\x0d\x0d\x0d\x0d\x0d\x0d\x0d\x0c\x0b\x0a"
"\x09\x09\x09\x09\x09\x09\x09\x09";
char *G=
"\x00\x00\x27\x27\x00\x00\x13\x27\x13\x13\x3f\x3f\x13\x13\x3f\x3f"
"\x00\x03\x07\x0b\x0f\x13\x17\x1b\x1f\x23\x27\x2b\x2f\x33\x37\x3b"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0f\x1f\x2f\x3f\x3f\x3f\x3f"
"\x3f\x3f\x3f\x3f\x3f\x2f\x1f\x0f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f"
"\x1f\x27\x2f\x37\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x37\x2f\x2f"
"\x2f\x2f\x2f\x2f\x2f\x2f\x2f\x2f\x2f\x33\x37\x3b\x3f\x3f\x3f\x3f"
"\x3f\x3f\x3f\x3f\x3f\x3b\x37\x33\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x07\x0f\x17\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x17\x0f\x07"
"\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x13\x17\x1b\x1f\x1f\x1f\x1f"
"\x1f\x1f\x1f\x1f\x1f\x1b\x17\x13\x13\x13\x13\x13\x13\x13\x13\x13"
"\x13\x15\x17\x19\x1b\x1b\x1b\x1b\x1b\x1b\x1b\x1b\x1b\x19\x17\x15"
"\x03\x03\x03\x03\x03\x03\x03\x03\x03\x07\x0b\x0f\x13\x13\x13\x13"
"\x13\x13\x13\x13\x13\x0f\x0b\x07\x07\x07\x07\x07\x07\x07\x07\x07"
"\x07\x09\x0b\x0d\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0d\x0b\x09"
"\x09\x09\x09\x09\x09\x09\x09\x09\x09\x0a\x0b\x0c\x0d\x0d\x0d\x0d"
"\x0d\x0d\x0d\x0d\x0d\x0c\x0b\x0a";
char *B=
"\x00\x27\x00\x27\x00\x27\x00\x27\x13\x3f\x13\x3f\x13\x3f\x13\x3f"
"\x00\x03\x07\x0b\x0f\x13\x17\x1b\x1f\x23\x27\x2b\x2f\x33\x37\x3b"
"\x3f\x3f\x3f\x3f\x3f\x2f\x1f\x0f\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x0f\x1f\x2f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x3f\x37\x2f\x27"
"\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x1f\x27\x2f\x37\x3f\x3f\x3f\x3f"
"\x3f\x3f\x3f\x3f\x3f\x3b\x37\x33\x2f\x2f\x2f\x2f\x2f\x2f\x2f\x2f"
"\x2f\x33\x37\x3b\x3f\x3f\x3f\x3f\x1f\x1f\x1f\x1f\x1f\x17\x0f\x07"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x07\x0f\x17\x1f\x1f\x1f\x1f"
"\x1f\x1f\x1f\x1f\x1f\x1b\x17\x13\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0f"
"\x0f\x13\x17\x1b\x1f\x1f\x1f\x1f\x1b\x1b\x1b\x1b\x1b\x19\x17\x15"
"\x13\x13\x13\x13\x13\x13\x13\x13\x13\x15\x17\x19\x1b\x1b\x1b\x1b"
"\x13\x13\x13\x13\x13\x0f\x0b\x07\x03\x03\x03\x03\x03\x03\x03\x03"
"\x03\x07\x0b\x0f\x13\x13\x13\x13\x0f\x0f\x0f\x0f\x0f\x0d\x0b\x09"
"\x07\x07\x07\x07\x07\x07\x07\x07\x07\x09\x0b\x0d\x0f\x0f\x0f\x0f"
"\x0d\x0d\x0d\x0d\x0d\x0c\x0b\x0a\x09\x09\x09\x09\x09\x09\x09\x09"
"\x09\x0a\x0b\x0c\x0d\x0d\x0d\x0d";
char BMP_HEAD[]={
"\x42\x4D\x36\x57\x07\x00\x00\x00\x00\x00\x36\x04\x00\x00\x28\x00"
"\x00\x00\x20\x03\x00\x00\x58\x02\x00\x00\x01\x00\x08\x00\x00\x00"
"\x00\x00\x00\x53\x07\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x80\x00\x00\x80"
"\x00\x00\x00\x80\x80\x00\x80\x00\x00\x00\x80\x00\x80\x00\x80\x80"
"\x00\x00\xC0\xC0\xC0\x00\xC0\xDC\xC0\x00\xF0\xCA\xA6\x00\x00\x20"
"\x40\x00\x00\x20\x60\x00\x00\x20\x80\x00\x00\x20\xA0\x00\x00\x20"
"\xC0\x00\x00\x20\xE0\x00\x00\x40\x00\x00\x00\x40\x20\x00\x00\x40"
"\x40\x00\x00\x40\x60\x00\x00\x40\x80\x00\x00\x40\xA0\x00\x00\x40"
"\xC0\x00\x00\x40\xE0\x00\x00\x60\x00\x00\x00\x60\x20\x00\x00\x60"
"\x40\x00\x00\x60\x60\x00\x00\x60\x80\x00\x00\x60\xA0\x00\x00\x60"
"\xC0\x00\x00\x60\xE0\x00\x00\x80\x00\x00\x00\x80\x20\x00\x00\x80"
"\x40\x00\x00\x80\x60\x00\x00\x80\x80\x00\x00\x80\xA0\x00\x00\x80"
"\xC0\x00\x00\x80\xE0\x00\x00\xA0\x00\x00\x00\xA0\x20\x00\x00\xA0"
"\x40\x00\x00\xA0\x60\x00\x00\xA0\x80\x00\x00\xA0\xA0\x00\x00\xA0"
"\xC0\x00\x00\xA0\xE0\x00\x00\xC0\x00\x00\x00\xC0\x20\x00\x00\xC0"
"\x40\x00\x00\xC0\x60\x00\x00\xC0\x80\x00\x00\xC0\xA0\x00\x00\xC0"
"\xC0\x00\x00\xC0\xE0\x00\x00\xE0\x00\x00\x00\xE0\x20\x00\x00\xE0"
"\x40\x00\x00\xE0\x60\x00\x00\xE0\x80\x00\x00\xE0\xA0\x00\x00\xE0"
"\xC0\x00\x00\xE0\xE0\x00\x40\x00\x00\x00\x40\x00\x20\x00\x40\x00"
"\x40\x00\x40\x00\x60\x00\x40\x00\x80\x00\x40\x00\xA0\x00\x40\x00"
"\xC0\x00\x40\x00\xE0\x00\x40\x20\x00\x00\x40\x20\x20\x00\x40\x20"
"\x40\x00\x40\x20\x60\x00\x40\x20\x80\x00\x40\x20\xA0\x00\x40\x20"
"\xC0\x00\x40\x20\xE0\x00\x40\x40\x00\x00\x40\x40\x20\x00\x40\x40"
"\x40\x00\x40\x40\x60\x00\x40\x40\x80\x00\x40\x40\xA0\x00\x40\x40"
"\xC0\x00\x40\x40\xE0\x00\x40\x60\x00\x00\x40\x60\x20\x00\x40\x60"
"\x40\x00\x40\x60\x60\x00\x40\x60\x80\x00\x40\x60\xA0\x00\x40\x60"
"\xC0\x00\x40\x60\xE0\x00\x40\x80\x00\x00\x40\x80\x20\x00\x40\x80"
"\x40\x00\x40\x80\x60\x00\x40\x80\x80\x00\x40\x80\xA0\x00\x40\x80"
"\xC0\x00\x40\x80\xE0\x00\x40\xA0\x00\x00\x40\xA0\x20\x00\x40\xA0"
"\x40\x00\x40\xA0\x60\x00\x40\xA0\x80\x00\x40\xA0\xA0\x00\x40\xA0"
"\xC0\x00\x40\xA0\xE0\x00\x40\xC0\x00\x00\x40\xC0\x20\x00\x40\xC0"
"\x40\x00\x40\xC0\x60\x00\x40\xC0\x80\x00\x40\xC0\xA0\x00\x40\xC0"
"\xC0\x00\x40\xC0\xE0\x00\x40\xE0\x00\x00\x40\xE0\x20\x00\x40\xE0"
"\x40\x00\x40\xE0\x60\x00\x40\xE0\x80\x00\x40\xE0\xA0\x00\x40\xE0"
"\xC0\x00\x40\xE0\xE0\x00\x80\x00\x00\x00\x80\x00\x20\x00\x80\x00"
"\x40\x00\x80\x00\x60\x00\x80\x00\x80\x00\x80\x00\xA0\x00\x80\x00"
"\xC0\x00\x80\x00\xE0\x00\x80\x20\x00\x00\x80\x20\x20\x00\x80\x20"
"\x40\x00\x80\x20\x60\x00\x80\x20\x80\x00\x80\x20\xA0\x00\x80\x20"
"\xC0\x00\x80\x20\xE0\x00\x80\x40\x00\x00\x80\x40\x20\x00\x80\x40"
"\x40\x00\x80\x40\x60\x00\x80\x40\x80\x00\x80\x40\xA0\x00\x80\x40"
"\xC0\x00\x80\x40\xE0\x00\x80\x60\x00\x00\x80\x60\x20\x00\x80\x60"
"\x40\x00\x80\x60\x60\x00\x80\x60\x80\x00\x80\x60\xA0\x00\x80\x60"
"\xC0\x00\x80\x60\xE0\x00\x80\x80\x00\x00\x80\x80\x20\x00\x80\x80"
"\x40\x00\x80\x80\x60\x00\x80\x80\x80\x00\x80\x80\xA0\x00\x80\x80"
"\xC0\x00\x80\x80\xE0\x00\x80\xA0\x00\x00\x80\xA0\x20\x00\x80\xA0"
"\x40\x00\x80\xA0\x60\x00\x80\xA0\x80\x00\x80\xA0\xA0\x00\x80\xA0"
"\xC0\x00\x80\xA0\xE0\x00\x80\xC0\x00\x00\x80\xC0\x20\x00\x80\xC0"
"\x40\x00\x80\xC0\x60\x00\x80\xC0\x80\x00\x80\xC0\xA0\x00\x80\xC0"
"\xC0\x00\x80\xC0\xE0\x00\x80\xE0\x00\x00\x80\xE0\x20\x00\x80\xE0"
"\x40\x00\x80\xE0\x60\x00\x80\xE0\x80\x00\x80\xE0\xA0\x00\x80\xE0"
"\xC0\x00\x80\xE0\xE0\x00\xC0\x00\x00\x00\xC0\x00\x20\x00\xC0\x00"
"\x40\x00\xC0\x00\x60\x00\xC0\x00\x80\x00\xC0\x00\xA0\x00\xC0\x00"
"\xC0\x00\xC0\x00\xE0\x00\xC0\x20\x00\x00\xC0\x20\x20\x00\xC0\x20"
"\x40\x00\xC0\x20\x60\x00\xC0\x20\x80\x00\xC0\x20\xA0\x00\xC0\x20"
"\xC0\x00\xC0\x20\xE0\x00\xC0\x40\x00\x00\xC0\x40\x20\x00\xC0\x40"
"\x40\x00\xC0\x40\x60\x00\xC0\x40\x80\x00\xC0\x40\xA0\x00\xC0\x40"
"\xC0\x00\xC0\x40\xE0\x00\xC0\x60\x00\x00\xC0\x60\x20\x00\xC0\x60"
"\x40\x00\xC0\x60\x60\x00\xC0\x60\x80\x00\xC0\x60\xA0\x00\xC0\x60"
"\xC0\x00\xC0\x60\xE0\x00\xC0\x80\x00\x00\xC0\x80\x20\x00\xC0\x80"
"\x40\x00\xC0\x80\x60\x00\xC0\x80\x80\x00\xC0\x80\xA0\x00\xC0\x80"
"\xC0\x00\xC0\x80\xE0\x00\xC0\xA0\x00\x00\xC0\xA0\x20\x00\xC0\xA0"
"\x40\x00\xC0\xA0\x60\x00\xC0\xA0\x80\x00\xC0\xA0\xA0\x00\xC0\xA0"
"\xC0\x00\xC0\xA0\xE0\x00\xC0\xC0\x00\x00\xC0\xC0\x20\x00\xC0\xC0"
"\x40\x00\xC0\xC0\x60\x00\xC0\xC0\x80\x00\xC0\xC0\xA0\x00\xF0\xFB"
"\xFF\x00\xA4\xA0\xA0\x00\x80\x80\x80\x00\x00\x00\xFF\x00\x00\xFF"
"\x00\x00\x00\xFF\xFF\x00\xFF\x00\x00\x00\xFF\x00\xFF\x00\xFF\xFF"
"\x00\x00\xFF\xFF\xFF\x00"};
unsigned ix,iy,y;
register unsigned x,i;
FILE *fpbmp;
  ix=getmaxx()+1;iy=getmaxy()+1;
  BMP_HEAD[0x12]=ix&0xff;  BMP_HEAD[0x13]=ix>>8;
  BMP_HEAD[0x16]=iy&0xff;  BMP_HEAD[0x17]=iy>>8;
  fpbmp=fopen("NC4.BMP","wr+");
  if(fpbmp==NULL)return;
  for(i=0;i<0xf7;i++)BMP_HEAD[0x36+(i<<2)+0]=B[i]<<2;
  for(i=0;i<0xf7;i++)BMP_HEAD[0x36+(i<<2)+1]=G[i]<<2;
  for(i=0;i<0xf7;i++)BMP_HEAD[0x36+(i<<2)+2]=R[i]<<2;
  fwrite(BMP_HEAD,1,0x436,fpbmp);
  for(y=iy;y>0;y--){
    for(i=0;i<=ix;i++){
      BMP_HEAD[i]=getpixel(i,y-1);
    }fwrite(BMP_HEAD,1,ix,fpbmp);
  }
  fclose(fpbmp);
#endif
}
/*600 258 800 320*/
