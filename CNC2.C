#include "znc.h"
#include <dos.h>
#include <conio.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
extern char RunCodeGroups[1000];
extern char RunCodeLines[1000];
extern char M9899;
extern char RunCodeLine;/*当前代码行*/
extern char G0203;
extern char RunLineFlag;
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
/*void P_c_XYZ(int xyz,long l){
 if(MXYZ==xyz){Position_control_x(l);Signal_1100X();}
 else{
  if(xyz)xyz--;else xyz=2;
  if(MXYZ==xyz){Position_control_y(l);Signal_1100Y();}
  else{
   if(xyz)xyz--;else xyz=2;
   if(MXYZ==xyz){Position_control_z(l);Signal_1100();}
  }
 }
}*/
void InitCNC(void){
    register i,j;
    Code=BMP+0x4800;
    CodeGroup=ReadRAM(0,0x200);
    if(CodeGroup>99)CodeGroup=0;
    for(i=0;i<100;i++){
        j=ReadRAM(0,0x280+i);
        if(j!=1)j=0;
        CodeGroupIndex[i]=j;
    }
    Read_Code(CodeGroup);
    RunCodeGroups[0]=CodeGroup;
    RunCodeLines[0]=0;M9899=0;
}
void CreatTempFile(void){
    int i;
    mkdir(path);
    if(!chdir(path))chdir("..");
}
void CheckTempFile(void){
}

void Read_Code(unsigned char group){   /*主函数*/
    char    ia;
    char far *pi;
    register int i;
    if(CodeGroupIndex[group]){
        ia=group+2;
        pi=MK_FP(0xd100,0);
        ReadRAM(ia,0);
        if(!Argc)movmem(pi,Code,3500);
        else    movmem1(pi,Code,3500);
    }
    else    for(i=0;i<50;i++){Ncnccode.N=i|0x80;memcpy(Code+i*35,&Ncnccode,35);}
}
void Write_Code(unsigned char group){  /*主函数*/
    char    ia;
    char far *pi;
    register int i;
    ia=group+2;
    CodeGroupIndex[group]=1;
    WriteRAM(0,group+0x280,1);
    pi=MK_FP(0xd100,0);
    WriteRAM(ia,0,*Code);
    if(!Argc)movmem(Code,pi,3500);
    else    movmem1(Code,pi,3500);
}
void Read_Code1(unsigned char group){  /*子函数*/
    char    ia;
    char far *pi;
    register int i,j;
    if(CodeGroupIndex[group]){
        ia=(group-1)/3+3;
        j=((group-1)%3)*1750;
        pi=MK_FP(0xd100,0);
        ReadRAM(ia,j);
        if(!Argc)movmem(pi+j,Code,1750);
        else    movmem1(pi+j,Code,1750);
    }
    else    for(i=0;i<50;i++){Ncnccode.N=i|0x80;memcpy(Code+i*35,&Ncnccode,35);}
}
void Write_Code1(unsigned char group){ /*子函数*/
    char    ia;
    char far *pi;
    register int i,j;
    ia=(group-1)/3+3;
    j=((group-1)%3)*1750;
    CodeGroupIndex[group]=1;
    WriteRAM(0,group+0x280,1);
    pi=MK_FP(0xd100,0);
    WriteRAM(ia,j,*Code);
    if(!Argc)movmem(Code,pi+j,1750);
    else    movmem1(Code,pi+j,1750);
}
void C1(void){
    if(!C[0]){
    if(Dis_flag){ShowMess(15);return;}
    ClearKey(0);ShowMess(18);MaxL=2;MaxN=99;MinN=0;C[0]=1;}
    else {
        if(KeyN!=0x7fffffff&&KeyN!=CodeGroup){CodeGroup=(char)KeyN;
        if(!CodeGroup)
          Read_Code(CodeGroup);
        else
          Read_Code1(CodeGroup);
        WriteRAM(0,0x200,CodeGroup);}
    C[0]=0;ClearKey(0);}
}
void C2(char flag){
    int i,j;
    if(!flag)ClearKey(0);
    if(!C[1]){
    CXX=1;Nline=0;CYY=0;ShowMess(CXX+40);
    ClearCodeK();C[1]=1;
/*  DelLine();*/
    ShowCode(0);
    }
    else{if(!flag)C[1]=0;
    CXX=1;Nline=0;CYY=0;
    DelCodeLine();
    CodeGroupIndex[CodeGroup]=1;
    WriteRAM(0,CodeGroup+0x280,1);
    ShowCode(0);Write_Code(CodeGroup);
    if(!flag){/*ShowKey(2,0);*/ShowMess(0);}}
/*  if(!flag)ShowKey(1,0);*/
}
void C9(void){
ClearKey(0);
if(Dis_flag){ShowMess(15);return;}
if(K[2])K3();
if(C[8]==1)C[8]=0;
else if(!C[8])C[8]=1;
ShowFlag(3);
}
void C10(char flag){
    if(flag){
        ShowCode(-1);
    }
    else{
        ShowTable(-1);
    }
}

void ShowCode(int lines){
 /*  register i,j;
    int i0,j0,i1,j1;
    int l0,l1;
    int color;
    char far *cncstr;
    char str[10];
    if(KEYL<0xb)return;   */
/*  if(KEYL<0xb){KEYL=0xb;lines=-1;ShowKey(0xC,0);}*/
  /*  Sline=CYY;
    cncstr=(char far *)&cnccode;
    if(lines==-1){
    setfillstyle(1,BKCOLORB);
    bar(BX0+1,BY0+1,BX1,BY1);
    lines=0;
    }
    if(!lines){
        setfillstyle(1,BKCOLORB);
        bar(BX0+6,BY0+1+18*0,BX1,BY0+2+18*1);
        for(i=BX0+4,j=0;j<9;j++){
        #if ZK
        List16x16F(i+2+CTabWidth1[j],BY0+2+9,0xf,KeyStr(0xE,j));
        #else
        List16x16M(i+2+CTabWidth1[j],BY0+2+9,0xf,KeyStr(0xE,j));
        #endif
        i+=CTabWidth[j];
        }
        l0=1;l1=10;
    }else{
        l0=l1=lines;
    }
    j0=Nline-Sline;if(j0<0||j0>=50)
    j0=50;
    for(i=l0;i<=l1;i++){
    l0=j0+i-1;
    if(l0>=50)
    l0=49;
    memcpy(cncstr,Code+l0*21,21);
    CodetoC();
    for(i0=BX0+3,i1=0,j=0;j<9;j++){
        if(i==CYY+1&&j==CXX&&C[1]||(j0+i-1)==RunCodeLine&&C[8]>1){
        setfillstyle(1,0xf);
        color=0x4;
        }else{
        setfillstyle(1,8);
        color=0xf;
        }
        switch(j){
        case 0:strcpy(str,cncchar.N);break;
        case 1:strcpy(str,cncchar.Code);break;
        case 2:strcpy(str,cncchar.X);break;
        case 3:strcpy(str,cncchar.Y);break;
        case 4:strcpy(str,cncchar.Z);break;
        case 5:strcpy(str,cncchar.R);break;
        case 6:strcpy(str,cncchar.G);break;
        case 7:str[0]=cncchar.D0;str[1]=0;break;
        case 8:str[0]=cncchar.D1;str[1]=0;break;
        }
    i1=CTabWidth[j];
    bar(i0+4,BY0+2+(i)*18+1,i0+i1-2,BY0+2+(i+1)*18-2);
    Disp8x16(i0+i1-CTabWidth2[j],BY0+2+18*(i)+9,2,color,str);
    i0+=i1;
    }
    }   */
/*  bar(k1+TabWidth1[j],BY0+2+(i+1)*18+1,k1+k2,BY0+2+(i+2)*18-2);
    Disp8x16(k1+k2-TabWidth2[j],BY0+2+18*(i+1)+9,2,color,str);*/

}
void DelCodeLine(void){
    register unsigned i,j;
    for(i=0,j=0;i<50;i++){
        while(*(Code+i*20)&0x80&&i<50)i++;
        if(i<50){memcpy(Code+j*21,Code+i*21,21);*(Code+j*21)=j;j++;}
    }
    for(;j<50;j++){
        memcpy(Code+j*21,&Ncnccode,21);*(Code+j*21)=j|0x80;
    }
}
void CheckCode(void){
   /* register unsigned i,j;
    long l=0x7fffffff,l0=-9999999,l1=9999999;
    for(i=0,j=0;i<50;i++,j++){
        memcpy(Code+i*20,cnccode,20);
        if(cnccode.N!=i){i++;continue;}
        if(cnccode.G>=0x80)cnccode.G=0x04;
        if(cnccode.X<l0||cnccode.X>l1)cnccode.X=l;
        if(cnccode.Y<l0||cnccode.Y>l1)cnccode.Y=l;
        if(cnccode.Z<l0||cnccode.Z>l1)cnccode.Z=l;
        if(cnccode.R<l0||cnccode.X>l1)cnccode.R=l;
        if(cnccode.G>99)cnccode.G=-1;
        if(cnccode.D>=0xA0)cnccode.D|=0xf0;
        if(cnccode.D&0xf>=0x0A)cnccode.D|=0x0f;
    }
    for(;j<50;j++){
        cnccode.G=0x04;
        cnccode.X=l;
        cnccode.Y=l;
        cnccode.Z=l;
        cnccode.R=l;
        cnccode.D=-1;
    }    */
}
void CtoCode(void){
    register unsigned i,j;
    long l;
    i=cncchar.N[0]-'0'*10+cncchar.N[1]-'0';
    if(i>=100)i=-1;cnccode.N=i;
    for(j=-1,i=0;i<CodeL;i++)
        if(!strcmp(cncchar.Code,CodeC[i])){j=CodeN[i];break;}
    cnccode.Code=j;
    j=0;
    i=cncchar.X[1];if(i>='0'&&i<='9')i-='0';else i=0;j+=i;j*=10;
    i=cncchar.X[2];if(i>='0'&&i<='9')i-='0';else i=0;j+=i;j*=10;
    i=cncchar.X[3];if(i>='0'&&i<='9')i-='0';else i=0;j+=i;j*=10;
    i=cncchar.X[4];if(i>='0'&&i<='9')i-='0';else i=0;j+=i;
    l=(long)j*1000;j=0;
    i=cncchar.X[6];if(i>='0'&&i<='9')i-='0';else i=0;j+=i;j*=10;
    i=cncchar.X[7];if(i>='0'&&i<='9')i-='0';else i=0;j+=i;j*=10;
    i=cncchar.X[8];if(i>='0'&&i<='9')i-='0';else i=0;j+=i;
    l+=j;if(cncchar.X[0]=='-')cnccode.X=-l;else cnccode.X=l;
    j=0;
    i=cncchar.Y[1];if(i>='0'&&i<='9')i-='0';else i=0;j+=i;j*=10;
    i=cncchar.Y[2];if(i>='0'&&i<='9')i-='0';else i=0;j+=i;j*=10;
    i=cncchar.Y[3];if(i>='0'&&i<='9')i-='0';else i=0;j+=i;j*=10;
    i=cncchar.Y[4];if(i>='0'&&i<='9')i-='0';else i=0;j+=i;
    l=(long)j*1000;j=0;
    i=cncchar.Y[6];if(i>='0'&&i<='9')i-='0';else i=0;j+=i;j*=10;
    i=cncchar.Y[7];if(i>='0'&&i<='9')i-='0';else i=0;j+=i;j*=10;
    i=cncchar.Y[8];if(i>='0'&&i<='9')i-='0';else i=0;j+=i;
    l+=j;if(cncchar.Y[0]=='-')cnccode.Y=-l;else cnccode.Y=l;
    j=0;
    i=cncchar.Z[1];if(i>='0'&&i<='9')i-='0';else i=0;j+=i;j*=10;
    i=cncchar.Z[2];if(i>='0'&&i<='9')i-='0';else i=0;j+=i;j*=10;
    i=cncchar.Z[3];if(i>='0'&&i<='9')i-='0';else i=0;j+=i;j*=10;
    i=cncchar.Z[4];if(i>='0'&&i<='9')i-='0';else i=0;j+=i;
    l=(long)j*1000;j=0;
    i=cncchar.Z[6];if(i>='0'&&i<='9')i-='0';else i=0;j+=i;j*=10;
    i=cncchar.Z[7];if(i>='0'&&i<='9')i-='0';else i=0;j+=i;j*=10;
    i=cncchar.Z[8];if(i>='0'&&i<='9')i-='0';else i=0;j+=i;
    l+=j;if(cncchar.Z[0]=='-')cnccode.Z=-l;else cnccode.Z=l;
    j=0;
    i=cncchar.R[1];if(i>='0'&&i<='9')i-='0';else i=0;j+=i;j*=10;
    i=cncchar.R[2];if(i>='0'&&i<='9')i-='0';else i=0;j+=i;j*=10;
    i=cncchar.R[3];if(i>='0'&&i<='9')i-='0';else i=0;j+=i;j*=10;
    i=cncchar.R[4];if(i>='0'&&i<='9')i-='0';else i=0;j+=i;
    l=(long)j*1000;j=0;
    i=cncchar.R[6];if(i>='0'&&i<='9')i-='0';else i=0;j+=i;j*=10;
    i=cncchar.R[7];if(i>='0'&&i<='9')i-='0';else i=0;j+=i;j*=10;
    i=cncchar.R[8];if(i>='0'&&i<='9')i-='0';else i=0;j+=i;
    l+=j;if(cncchar.R[0]=='-')cnccode.R=-l;else cnccode.R=l;
    i=cncchar.G[0]-'0'*10+cncchar.G[1]-'0';
    if(i>=100)i=-1;cnccode.G=i;
    i=cncchar.D0-'0';
    if(i>=10)i=0;cnccode.D=i<<4;
    i=cncchar.D1-'0';
    if(i>=10)i=0;cnccode.D|=i;
}
void CodetoC(void){
    register unsigned i,j;
    register long l;
    long l1=0x7fffffff;
    char str[10],str0[10]="         ",str1[10]="-0000.000";
    i=cnccode.N&0x7f;
    if(i<50){cncchar.N[0]='0'+i/10;cncchar.N[1]='0'+i%10;}
    else    {cncchar.N[0]=' ';cncchar.N[1]=' ';}
    if(i&0x80){
        strcpy(cncchar.Code,str0+6);
        strcpy(cncchar.X,str0+1);
        strcpy(cncchar.Y,str0+1);
        strcpy(cncchar.Z,str0+1);
        strcpy(cncchar.R,str0+1);
        strcpy(cncchar.G,str0+7);
        cncchar.D0=str0[8];cncchar.D1=str0[8];
        return;
    }
    cncchar.Code[0]=0;
    if(cnccode.Code>CodeL)cnccode.Code=CodeL;
        strcpy(cncchar.Code,CodeC[cnccode.Code]);
    l=cnccode.X;
    if(l==l1)strcpy(str,str0);
    else{
        j=0;strcpy(str,str1);
        if(l<0)str[0]='-';else str[0]=' ';
        l=labs(l);
        if(l>=1000000){str[1]='0'+l/1000000;j++;l%=1000000;}
        else str[1]=' ';
        if(j||l>=100000){str[2]='0'+l/100000;j++;l%=100000;}
        else str[2]=' ';
        if(j||l>=10000){str[3]='0'+l/10000;j++;l%=10000;}
        else str[3]=' ';
        if(l>=1000){str[4]='0'+l/1000;l%=1000;}
        if(l>=100){str[6]='0'+l/100;l%=100;}
        if(l>=10){str[7]='0'+l/10;l%=10;}
        str[8]='0'+l%10;
    }
    strcpy(cncchar.X,str);
    l=cnccode.Y;
    if(l==l1)strcpy(str,str0);
    else{
        j=0;strcpy(str,str1);
        if(l<0)str[0]='-';else str[0]=' ';
        l=labs(l);if(l>9999999)l=9999999;
        if(l>=1000000){str[1]='0'+l/1000000;j++;l%=1000000;}
        else str[1]=' ';
        if(j||l>=100000){str[2]='0'+l/100000;j++;l%=100000;}
        else str[2]=' ';
        if(j||l>=10000){str[3]='0'+l/10000;j++;l%=10000;}
        else str[3]=' ';
        if(l>=1000){str[4]='0'+l/1000;l%=1000;}
        if(l>=100){str[6]='0'+l/100;l%=100;}
        if(l>=10){str[7]='0'+l/10;l%=10;}
        str[8]='0'+l%10;
    }
    strcpy(cncchar.Y,str);
    l=cnccode.Z;
    if(l==l1)strcpy(str,str0);
    else{
        j=0;strcpy(str,str1);
        if(l<0)str[0]='-';else str[0]=' ';
        l=labs(l);if(l>9999999)l=9999999;
        if(l>=1000000){str[1]='0'+l/1000000;j++;l%=1000000;}
        else str[1]=' ';
        if(j||l>=100000){str[2]='0'+l/100000;j++;l%=100000;}
        else str[2]=' ';
        if(j||l>=10000){str[3]='0'+l/10000;j++;l%=10000;}
        else str[3]=' ';
        if(l>=1000){str[4]='0'+l/1000;l%=1000;}
        if(l>=100){str[6]='0'+l/100;l%=100;}
        if(l>=10){str[7]='0'+l/10;l%=10;}
        str[8]='0'+l%10;
    }
    strcpy(cncchar.Z,str);
    l=cnccode.R;
    if(l==l1)strcpy(str,str0);
    else{
        j=0;strcpy(str,str1);
        if(l<0)str[0]='-';else str[0]=' ';
        l=labs(l);if(l>9999999)l=9999999;
        if(l>=1000000){str[1]='0'+l/1000000;j++;l%=1000000;}
        else str[1]=' ';
        if(j||l>=100000){str[2]='0'+l/100000;j++;l%=100000;}
        else str[2]=' ';
        if(j||l>=10000){str[3]='0'+l/10000;j++;l%=10000;}
        else str[3]=' ';
        if(l>=1000){str[4]='0'+l/1000;l%=1000;}
        if(l>=100){str[6]='0'+l/100;l%=100;}
        if(l>=10){str[7]='0'+l/10;l%=10;}
        str[8]='0'+l%10;
    }
    strcpy(cncchar.R,str);
    i=cnccode.G;
    if(i<100){cncchar.G[0]='0'+i/10;cncchar.G[1]='0'+i%10;}
    else{cncchar.G[0]=' ';cncchar.G[1]=' ';}
    i=cnccode.D>>4;
    if(i<10)cncchar.D0='0'+i;
    else    cncchar.D0=' ';
    i=cnccode.D&0xf;
    if(i<10)cncchar.D1='0'+i;
    else    cncchar.D1=' ';
}
