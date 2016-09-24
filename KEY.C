#include "znc.h"
#include <stdlib.h>
#include <graphics.h>
#include <dos.h>
#if CNC
extern char RunCodeLine;/*当前代码行*/
extern char RunCodeGroup;/*当前代码组*/
extern char RunCodeGroups[100];
extern char RunCodeLines[100];
extern char M9899;
extern char LoranFlag;
#endif
void InitSYS(unsigned);
void X();
void Y();
void Z();
void SWF();
void DelLine();
void SaveF7V();
void ClearF7();
#if CNC
void ClearCodeK();
void SaveCodeV();
#endif
void SetPass(char,int);
void ErrXY();
void Key(void){
}

void ClearKey(){
    if(XYZ[0]||XYZ[1]||XYZ[2]){
    XYZ[0]=0;XYZ[1]=0;XYZ[2]=0;
    }
/*  if(F[7]>=1&&F[7]<3)F8(0);if(F[8])F9();if(K[2]==1)K3();
    if(K[5]==1)K6();if(F[9])F10();*/
    ShowInput(0);
    ShowMess(0);
}
void SWF(){ ClearKey(0);
    SWF_K=1;
    SWF_flag++;
    if(SWF_flag>=3)SWF_flag=0;
    ShowSWF();
    DN_K=0;UP_K=0;
    XDN_K=0;YDN_K=0;XUP_K=0;YUP_K=0;
}
void XUp(void){
    long ll;
    /*if(Dis_flag){SoundTime=SoundTIME;ListMess1("无效操作！");return;} */
    lock_x=0;
    if(!XUP_K){XDN_K=0;
    ClearKey(0);
    SoundTime=SoundTIME;
   /* if(!F[1]){ListMess1("无效操作！");return;}  */
    if(V>6)MOVEXYZ=1;
    ZeroFlag=0;ShowTB(0);
    XUDL=XPosi(0);
    set_PIDX(10,0,4,0x10);
   if(SWF_flag==0)
      {Position_control_x(XUDL+1);}
    if(SWF_flag==1)
      {Position_control_x(XUDL+50);}
    if(SWF_flag==2)
      {Position_control_x(XUDL+100);}
    }
    if(XUP_K==UDKN){
    set_PIDX(10,0,4,0x10);
    if(SWF_flag==0)
        Velocity_control_x((int)3);
    if(SWF_flag==1)
        Velocity_control_x((int)1024);
    if(SWF_flag==2){
        Velocity_control_x((int)2047);
    }
    }
    if(XUP_K<UDKN)XUP_K++;
}
void XDown(void){
    long ll;
   /* if(Dis_flag){SoundTime=SoundTIME;ListMess1("无效操作！");return;} */
    lock_x=0;
    if(!XDN_K){XUP_K=0;
    ClearKey(0);
    SoundTime=SoundTIME;
  /* if(!F[1]){ListMess1("无效操作！");return;} */
    if(V>6)MOVEXYZ=2;
    ZeroFlag=0;ShowTB(0);
    XUDL=XPosi(0);
    set_PIDX(10,0,4,0x10);
    if(SWF_flag==0)
      {Position_control_x(XUDL-1);}
    if(SWF_flag==1)
      {Position_control_x(XUDL-50);}
    if(SWF_flag==2)
      {Position_control_x(XUDL-100);}
    }
    if(XDN_K==UDKN){
    set_PIDX(10,0,4,0x10);
    if(SWF_flag==0)
        Velocity_control_x((int)-3);
    if(SWF_flag==1)
        Velocity_control_x((int)-1024);
    if(SWF_flag==2){
        Velocity_control_x((int)-2047);
    }
    }
    if(XDN_K<UDKN)XDN_K++;
}
void YUp(void){
    long ll;
    /*if(Dis_flag){SoundTime=SoundTIME;ListMess1("无效操作！");return;} */
    lock_y=0;
    if(!YUP_K){YDN_K=0;
    ClearKey(0);
    SoundTime=SoundTIME;
   /* if(!F[1]){ListMess1("无效操作！");return;}  */
    if(V>6)MOVEXYZ=3;
    ZeroFlag=0;ShowTB(0);
    YUDL=YPosi(0);
    set_PIDY(10,0,4,0x10);
    if(SWF_flag==0)
      {Position_control_y(YUDL+1);}
    if(SWF_flag==1)
      {Position_control_y(YUDL+50);}
    if(SWF_flag==2)
      {Position_control_y(YUDL+100);}
    }
    if(YUP_K==UDKN){
    set_PIDY(10,0,4,0x10);
    if(SWF_flag==0)
        Velocity_control_y((int)3);
    if(SWF_flag==1)
        Velocity_control_y((int)1024);
    if(SWF_flag==2){
        Velocity_control_y((int)2047);
    }
    }
    if(YUP_K<UDKN)YUP_K++;
}
void YDown(void){
    long ll;
    /*if(Dis_flag){SoundTime=SoundTIME;ListMess1("无效操作！");return;}  */
    lock_y=0;
    if(!YDN_K){
    ClearKey(0);YUP_K=0;
    SoundTime=SoundTIME;
   /* if(!F[1]){ListMess1("无效操作！");return;}*/
    ZeroFlag=0;ShowTB(0);
    YUDL=YPosi(0);
    set_PIDY(10,0,4,0x20);
    if(SWF_flag==0)
      {Position_control_y(YUDL-1);}
    if(SWF_flag==1)
      {Position_control_y(YUDL-50);}
    if(SWF_flag==2)
      {Position_control_y(YUDL-100);}
    }
    if(YDN_K==UDKN){
    set_PIDY(10,0,4,0x10);
    if(SWF_flag==0)
        Velocity_control_y((int)-3);
    if(SWF_flag==1)
        Velocity_control_y((int)-1024);
    if(SWF_flag==2){
        Velocity_control_y((int)-2047);
    }
    }
    if(YDN_K<UDKN)YDN_K++;
}
void Up(void){
    long ll;
   /* if(Dis_flag||Ck8255_flag&0x10||(!K[3]&&(V<10&&K[8])&&SWF_K<10)){ListMess1("无效操作！");return;} */
    lock_z=0;
    if(!UP_K){DN_K=0;
    ClearKey(0);
    SoundTime=SoundTIME;
    /*if(!F[1])ListMess1("无效操作！"); */
    ZeroFlag=0;ShowTB(0);
    UDL=ZPosi(0);
    set_PIDZ(10,0,4,0x10);
    if(SWF_flag==0)
      {Position_control_z(UDL+1);}
    if(SWF_flag==1)
      {Position_control_z(UDL+50);}
    if(SWF_flag==2)
      {Position_control_z(UDL+100);}
    }
    if(UP_K==UDKN){
    set_PIDZ(10,0,4,0x20);
    if(SWF_flag==0)
        Velocity_control_z((int)3);
    if(SWF_flag==1)
        Velocity_control_z((int)1024);
    if(SWF_flag==2){
        Velocity_control_z((int)2047);
    }
    }
    if(UP_K<UDKN)UP_K++;
}
void Down(void){
    long ll;
   /* if(Dis_flag||Ck8255_flag&0x20||(!K[3]&&(V<10&&!K[8])&&SWF_K<10)){ListMess1("无效操作！");return;}  */
    lock_z=0;
    if(!DN_K){UP_K=0;
    ClearKey(0);
    SoundTime=SoundTIME;
   /* if(!F[1])ListMess1("无效操作！"); */
    ZeroFlag=0;ShowTB(0);
    UDL=ZPosi(0);
   set_PIDZ(10,0,4,0x10);
   if(SWF_flag==0)
      {Position_control_z(UDL-1);}
    if(SWF_flag==1)
      {Position_control_z(UDL-50);}
    if(SWF_flag==2)
      {Position_control_z(UDL-100);}
    }
    if(DN_K==UDKN){
    set_PIDZ(10,0,4,0x10);
    if(SWF_flag==0)
        Velocity_control_z((int)-3);
    if(SWF_flag==1)
        Velocity_control_z((int)-1024);
    if(SWF_flag==2){
        Velocity_control_z((int)-2047);

    }
    }
    if(DN_K<UDKN)DN_K++;
}
#if CNC
void Stop_x(char flag){
    int i;
    long l;
    long l1=0;
    if(flag==-1){l=XPosi(1);if(V<=6)flag=0;}
    if(flag!=-1){
    if(V<=6)flag=0;
    switch(flag){
        case 1:i=1;break;
        case 2:i=50/DZC;break;
        case 3:i=500/DZC;break;
        default:i=0;
    }
    l=XPosi(0);
    l1=l-XUDL;
    if(i){
        l1%=i;
        if(l1>0){l+=i;l-=l1;}
        if(l1<0){l-=i;l-=l1;}
    }
    }
    Position_control_x(l);
    lock_x=1;offset_rx=l;
}
void Stop_y(char flag){
    int i;
    long l;
    long l1=0;
    if(flag==-1){l=YPosi(1);if(V<=6)flag=0;}
    if(flag!=-1){
    if(V<=6)flag=0;
    switch(flag){
        case 1:i=1;break;
        case 2:i=50/DZC;break;
        case 3:i=500/DZC;break;
        default:i=0;
    }
    l=YPosi(0);
    l1=l-YUDL;
    if(i){
        l1%=i;
        if(l1>0){l+=i;l-=l1;}
        if(l1<0){l-=i;l-=l1;}
    }
    }
    Position_control_y(l);
    lock_y=1;offset_ry=l;
}
#endif
void Stop_z(char flag){
    int i;
    long l;
    long l1=0;
    if(flag==-1){l=ZPosi(1);if(V<=6)flag=0;}
    if(flag!=-1){
    if(V<=6)flag=0;
    switch(flag){
        case 1:i=1;break;
        case 2:i=50/DZC;break;
        case 3:i=500/DZC;break;
        default:i=0;
    }
    l=ZPosi(0);
    l1=l-UDL;
    if(i){
        l1%=i;
        if(l1>0){l+=i;l-=l1;}
        if(l1<0){l-=i;l-=l1;}
    }
    }
    Position_control_z(l);
    lock_z=1;offset_rz=l;
}
void X(){
    long l;
    #if CNC
    if(C[8]>1){ShowMess(15);return;}
    #endif
    if(XYZ[0]){
        if(KeyN!=0x7fffffff){KeyN/=DZC;l=KeyN-XPosi(0);position_xdis=KeyN;
        #if CNC
/*      if(DIS.XYZ==0)DIS.Position_E+=l-offset_x;*/
        if(G04>1)G04=1;
        #endif
        offset_x=l;SaveXYZ(0);position_px=position_x;}
        XYZ[0]=0;ClearKey(0);
        /*ShowXYZ(1);*/return;
    }
    ShowInput(0);/*if(Dis_flag){ShowMess(15);return;}*/
    ShowMess(12);
    MaxL=7;MaxN=9999999;MinN=-9999999;
    XYZ[0]=1;XYZ[1]=0;XYZ[2]=0;/*ShowXYZ(1);*/
}
void Y(){
    long l;
    #if CNC
    if(C[8]>1){ShowMess(15);return;}
    #endif
    if(XYZ[1]){
        if(KeyN!=0x7fffffff){KeyN/=DZC;l=KeyN-YPosi(0);position_ydis=KeyN;
        #if CNC
/*      if(DIS.XYZ==1)DIS.Position_E+=l-offset_y;*/
        if(G04>1)G04=1;
        #endif
        SaveXYZ(0);position_py=position_y;offset_y=l;}
        XYZ[1]=0;ClearKey(0);return;
    }
    ShowInput(0);/*if(Dis_flag){ShowMess(15);return;}*/
    ShowMess(13);
    MaxL=7;MaxN=9999999;MinN=-9999999;
    XYZ[0]=0;XYZ[1]=1;XYZ[2]=0;/*ShowXYZ(2);  */
}
void Z(){
    int i;
    long l;
    #if CNC
    if(C[8]>1){ShowMess(15);return;}
    #endif
    if(XYZ[2]){
        if(KeyN!=0x7fffffff){
          KeyN/=DZC;l=KeyN-ZPosi(0);
          if(Dis_flag)position_d-=offset_z-l;
          else position_d=KeyN;
        #if CNC
/*      if(DIS.XYZ==1)DIS.Position_E+=l-offset_z;*/
        if(G04>1)G04=1;
        #endif
          offset_z=l;
          if(Dis_flag){i=F12_flag;F12_flag=9;
            F12_flag=i;}
                  SaveXYZ(0);
        }
        XYZ[2]=0;
       /* ShowXYZ(3);*/
        return;
    }
    ShowInput(0);/*if(Dis_flag){ShowMess(15);return;}*/
    ShowMess(14);
    MaxL=7;MaxN=9999999;MinN=-9999999;
    XYZ[0]=0;XYZ[1]=0;XYZ[2]=1;/*ShowXYZ(3);*/
}
void DelLine(){
    int i,j,k;
    char TabZ[12];
    int l=0;
    for(i=0;i<10;i++)if(!Table.Index[i])break;
    for(j=0;i<10;i++)if(Table.Index[i])j=1;
    if(!j)return;
    for(i=0;i<Dis_start;i++)TabZ[i]=0;
    for(;i<=Dis_end;i++)TabZ[i]=1;
    for(;i<10;i++)TabZ[i]=0;
    for(i=0,j=0;j<10;i++,j++){
        k=0;
        if(Table.Index[j]!=1){
            for(;Table.Index[j]!=1&&j<10;j++);
            k=1;
            if(j==10)break;
            l=k;
        }if(l){
        TabZ[i]=TabZ[j];
        Table.Index[i]=Table.Index[j];
        Table.Shendu[i]=Table.Shendu[j];
        Table.Dianliu[i]=Table.Dianliu[j];
        Table.Maikuan[i]=Table.Maikuan[j];
        Table.Xiuzhi[i]=Table.Xiuzhi[j];
        Table.Jianxi[i]=Table.Jianxi[j];
        Table.Sudu[i]=Table.Sudu[j];
        Table.Shenggao[i]=Table.Shenggao[j];
        Table.Gongshi[i]=Table.Gongshi[j];
        Table.Mianji[i]=Table.Mianji[j];
        Table.Jixin[i]=Table.Jixin[j];
        Table.Gaoya[i]=Table.Gaoya[j];
        }
        if(k){i--;j--;}
    }for(;i<10;i++){Table.Index[i]=0;TabZ[i]=0;}
    for(i=0;i<10;i++){if(TabZ[i])break;}Dis_start=i;
    for(;i<10;i++){if(!TabZ[i])break;}Dis_end=--i;
    if(Dis_start==10){Dis_start=0;Dis_end=0;}
    if(l)ShowTable(0);
}

void SaveF7V(){
    if(KeyN!=0x7fffffff){
        if(Table.Index[YY]!=1){
        Table.Index[YY]=1;
        Table.Shendu[YY]=StrTable.Shendu[2];
        Table.Dianliu[YY]=StrTable.Dianliu[2];
        Table.Maikuan[YY]=StrTable.Maikuan[2];
        Table.Xiuzhi[YY]=StrTable.Xiuzhi[2];
        Table.Jianxi[YY]=StrTable.Jianxi[2];
        Table.Sudu[YY]=StrTable.Sudu[2];
        Table.Shenggao[YY]=StrTable.Shenggao[2];
        Table.Gongshi[YY]=StrTable.Gongshi[2];
        Table.Mianji[YY]=StrTable.Mianji[2];
        Table.Jixin[YY]=StrTable.Jixin[2];
        Table.Gaoya[YY]=StrTable.Gaoya[2];
        }
        switch(XX){
            case 0: Table.Shendu[YY]=KeyN/DZC;break;
            case 1: Table.Dianliu[YY]=KeyN/500;break;
            case 2: Table.Maikuan[YY]=KeyN;break;
            case 3: Table.Xiuzhi[YY]=KeyN;break;
            case 4: if(!KeyN||KeyN>=StrTable.Jianxi[0])Table.Jianxi[YY]=KeyN;break;
            case 5: Table.Sudu[YY]=KeyN;break;
            case 6: Table.Shenggao[YY]=KeyN;break;
            case 7: Table.Gongshi[YY]=KeyN;break;
            case 8: Table.Mianji[YY]=KeyN;break;
            case 9: Table.Jixin[YY]=KeyN;break;
            case 10:Table.Gaoya[YY]=KeyN;break;
        }
        groupindex[Group]=1;
        CheckTab();
        ShowInput(0);
    }
}
void ABS(void){
    int i,b=0;
    unsigned char a[]="\x9A\x8C\x8F\xA7\x8B\x8B\x9D\xAE\xD1";
    unsigned char far *aa;
    if(Argc==2){
        for(i=0;Argv[i];i++);
        for(;!Argv[i];i++);
        aa=Argv+i;
        for(i=0;a[i];i++){
        a[8-i]++;if((aa[i]^a[8-i])!=0xff)b++;}
        if(!b){Testflag=0;DIS_OK=1;}
        Argc--;
    }

}
void ClearF7(){
    ShowInput(0);
    switch(XX){
        case 0:MaxL=7;MaxN=9999999;MinN=-9999999;break;
#if DL32
        case 1:MaxL=5;MaxN=35000;MinN=0;break;
#else
        case 1:MaxL=5;MaxN=75000;MinN=0;break;
#endif
        case 2:MaxL=3;MaxN=999;MinN=1;break;
        case 3:MaxL=3;MaxN=999;MinN=03;break;
#if NZNC
        case 4:MaxL=2;MaxN=80;MinN=0;break;
#else
        case 4:MaxL=3;MaxN=120;MinN=0;break;
#endif
        case 5:MaxL=1;MaxN=8;MinN=1;break;
        case 6:MaxL=2;MaxN=99;MinN=0;break;
        case 7:
        case 8:MaxL=2;MaxN=99;MinN=1;break;
        case 9:MaxL=0;MaxN=0;MinN=0;break;
        case 10:MaxL=1;MaxN=3;MinN=0;break;
        default:MaxL=0;MaxN=0x7fffffff;MinN=0x7fffffff;break;
    }
}
#if CNC
void SaveCodeV(){
    register i;
    if(KeyN!=0x7fffffff){
        switch(CXX){
        case 1:if(Inputstr[0]=='M')KeyN+=100;
            for(i=0;i<CodeL;i++)if(KeyN==CodeN[i]){
        cnccode.Code=i;break;}
        break;
        case 2:cnccode.X=KeyN;break;
        case 3:cnccode.Y=KeyN;break;
        case 4:cnccode.Z=KeyN;break;
        case 5:cnccode.R=KeyN;break;
        case 6:cnccode.G=KeyN;break;
        case 7:cnccode.D=(KeyN<<4)|cnccode.D&0x0f;break;
        case 8:cnccode.D=(KeyN)|cnccode.D&0xf0;break;
        }
        cnccode.N=Nline;
        memcpy(Code+(20*Nline),&cnccode,20);
    }
    ShowInput(0);
}
void ClearCodeK(){
    ShowInput(0);
    switch(CXX){
        case 0:MaxL=2;MaxN=50;MinN=0;break;
        case 1:MaxL=2;MaxN=99;MinN=0;Inputstr[0]='G';Inputstr[1]=0;break;
        case 2:MaxL=7;MaxN=9999999;MinN=-9999999;break;
        case 3:MaxL=7;MaxN=9999999;MinN=-9999999;break;
        case 4:MaxL=7;MaxN=9999999;MinN=-9999999;break;
        case 5:MaxL=7;MaxN=9999999;MinN=-9999999;break;
        case 6:MaxL=2;MaxN=99;MinN=0;break;
        case 7:
        case 8:MaxL=1;MaxN=9;MinN=0;break;
        default:MaxL=0;MaxN=0x7fffffff;MinN=0x7fffffff;break;
    }
}
#endif
void SetPass(char flag,int key){
    static unsigned i,j;
    static int i1=0,pa=0;
    static unsigned long l,l1=1000000000,l2;
    static char str[15],str1[3];
    struct REGPACK iintr;
    if(!flag){pa=0;strcpy(str,"0000000000");l1=1000000000;
    iintr.r_ax=0x200;
    intr(0x1a,&iintr);
    i=N0TN(OldDATE);
    if(iintr.r_cx>=0x1400)i--;
    j=NTN0(i);
    i=OldID;
    l=i&0xaaaa;
    l<<=16;
    l+=i&0x5555;
    i=j&0x5555;
    l+=(unsigned long)i<<16;
    l+=j&0xaaaa;
    l^=0xfa5fa5fa;
    l2=l;
    if(Initsystem)l2^=0x5a05a05a;
    for(i=0;i<10;i++){
    str[i]+=l2/l1;l2%=l1;l1/=10;
    }
    if(Initsystem)str[0]+=5;
    DispF8(0);
    ShowMess(0x91);
    ShowMess(0x90);
    Dispcbar(F8X+112,F8Y+20+5,F8X+200,F8Y+5+20+16,15,3,1,str);
    setfillstyle(1,8);i=0;
    ShowMess(0x92);
    itoa(9-i1,str1,10);
    Dispcbar(F8X+112,F8Y+5,F8X+212,F8Y+5+16,15,3,1,str1);
    str[0]=0;
    }
    if(flag==1){
        j=key;
        if(i1<9){
        if(i<=10){
            if(j==0xd){
    pa=0;
    if(strlen(str)==10){
        if(str[0]>'4'){pa=1;str[0]-=5;}
        l=atol(str+1);l1=str[0]-'0';l1*=1000000000;
        l+=l1;l1=l;l2=0;
        for(i=0;i<8;i++){
            j=l1&0xf;l1>>=4;
            j=PWS[j];l2|=j;
            if(i<7)l2<<=4;
        }
        l1=l2;
        if(Initsystem){
        l2=0;
        for(i=0;i<8;i++){
            j=l1&0xf;l1>>=4;
            j=PWS[j];l2|=j;
            if(i<7)l2<<=4;
        }
        l1=l2;
        l2=0;
        for(i=0;i<8;i++){
            j=l1&0xf;l1>>=4;
            j=PWS[j];l2|=j;
            if(i<7)l2<<=4;
        }
        l1=l2;
        }
        l1&=0x55555555;l2&=0xaaaaaaaa;
        l2>>=1;l1<<=1;l2|=l1;
        i=l2>>16;
        j=l2;
        if(!Initsystem){
        if(!pa){
            if(i==OldID){
                if(!SetWork(GetDate(),j))
                ResetCop();
            }
        }else{
            j^=0x5432;i^=0x876;
            if(i==OldID){
                if(!SetWork1(GetDate(),j))
                ResetCop();
            }
        }
        }
        else{
        j^=0x5432;i^=0x876;
        if(!pa){
        if(i==OldID){
            InitSYS(j);
            ResetCop();
        }
        }
        }
            i=0;i1++;
        itoa(9-i1,str1,10);
    Dispcbar(F8X+112,F8Y+5,F8X+212,F8Y+5+16,15,3,1,str1);
    str1[0]=0;
    Dispcbar(F8X+112,F8Y+5+2*20,F8X+212,F8Y+5+2*20+16,15,3,1,str1);
            return;
        }   else return;
            }
            if(j>='0'&&j<='9'){
            str[i]=j;str[i+1]=0;
            }else{
                if(i<1)i=1;if(strlen(str)==10)i--;
                else i-=2;str[i+1]=0;
            }
        Dispcbar(F8X+112,F8Y+5+2*20,F8X+212,F8Y+5+2*20+16,15,3,1,str);
        if(i==9)i--;
        i++;
        }
    }else{
    ResetCop();
    }
    }
}
unsigned GetDate(){
    register unsigned i,j;
    int T00;
    struct REGPACK iintr;
    iintr.r_ax=0x200;
    intr(0x1a,&iintr);
    if(iintr.r_cx>=0x1400)T00=1;else T00=0;
    iintr.r_ax=0x400;
    intr(0x1a,&iintr);
    i=iintr.r_dx;
    j=i>>12;j*=10;i&=0xfff;j+=i>>8;j--;j*=31;i&=0xff;j+=(i>>4)*10;j+=i&0xf;j--;
    i=iintr.r_cx;
    i-=0x2000;i*=372;j+=i;i=j;if(T00)i++;
    j=NTN0(i);
    if(j)
    return j;
    j<<=13;
    i>>=3;
    i|=j;
    i^=0x55aa;
    j=i;
    i>>=7;
    j<<=9;
    i|=j;
    return i^0xaa55;
}
long GetSTime(void){
    struct REGPACK iintr;
    long l;
    iintr.r_ax=0x200;
    intr(0x1A,&iintr);
    l=iintr.r_cx;
    l<<=16;
    l+=iintr.r_dx;
    return l;
}
int SetWork(unsigned i1,unsigned i2){
    register unsigned i,j=0;
    Argc=1;
    i=N0TN(i1);
    i--;
    i2++;
    #if SIONV
    i2-=0x100;
    #endif
    if(i2>100)return i;
    i1=NTN0(i);
    i2=NTN0(i2+i);
    WriteRAM(0,0x7c,i1&0xff);
    WriteRAM(0,0x7d,i1>>8);
    WriteRAM(0,0x7e,i2&0xff);
    WriteRAM(0,0x7f,i2>>8);
    return j;
}
int SetWork1(unsigned i1,unsigned i2){
    register unsigned i,j;
    Argc=1;
    i=N0TN(i1);
    i--;
    if(i2>21234||i2<20000)return i;
    i2-=19999;
    i1=NTN0(i);
    i2+=i;
    i2=NTN0(i2);
    i1^=0x7a;
    i=i1;
    i<<=1;
    i1>>=15;
    i|=i1;
    i^=0x6a57;
    i2^=0x7a;
    j=i2;
    j<<=1;
    i2>>=15;
    j|=i2;
    j^=0x6a57;
    WriteRAM(0,0x1ac,i&0xff);
    WriteRAM(0,0x1ad,i>>8);
    WriteRAM(0,0x1ae,j&0xff);
    WriteRAM(0,0x1af,j>>8);
    return 0;
}
void ResetCop(void){
    CloseSound();
    exitp(0);
    ShowMess(0x93);
    WriteRAM(0x1f,0,0);
    delay(0x0fff);
    for(;;)outb(0x64,0xfe);
}
void ErrXY(int flag){
    ERR_XY=1;
    if(K[3]||!P[2])return;
    if(flag==1){
        F11();
        ShowMess(35);
        SoundTime=SDST;
    }
    if(flag==2){
        F11();
        ShowMess(36);
        SoundTime=SDST;
    }
}
unsigned NTN0(unsigned ii){
    register unsigned i,j;
    i=ii;j=i;
    j<<=13;
    i>>=3;/*i&=0x1fff;*/
    i|=j;
    i^=0x55aa;
    j=i;
    i>>=7;/*i&=0x1ff;*/
    j<<=9;
    i|=j;
    return i^0xaa55;
}
void SetDianliu(unsigned dianliu){
    unsigned ib;
    #if NZNC
    unsigned char DL=0;
    ib=dianliu/2;
    if(ib>100){DL|=0x01;ib-=50;};
    if(ib>=50){DL|=0x02;ib-=25;};
    if(ib>=25){DL|=0x04;ib-=25;};
    if(ib>=12){DL|=0x08;ib-=12;};
    if(ib>=6){DL|=0x10;ib-=6;};
    if(ib>=3){DL|=0x20;ib-=3;};
    if(ib>=2){DL|=0x40;ib-=2;};
    if(ib>=1){DL|=0x80;ib-=1;}DL=~DL;
    outb(IC3,DL);       /* 电流 */
    if((dianliu&1)&&dianliu<20)
    outb(IC3+2,inb(IC3+2)&0x7f);
    else    outb(IC3+2,inb(IC3+2)|0x80);
    #else
    ib=dianliu;
    SDL=ib;
#if DL32
    if(ib>63)ib=63;
#else
    if(ib>127)ib=127;
#endif
    IO2=ib;
    outb(IO+2,IO2);     /* 电流 */
    if(Table.Dianliu[Dis_lines]>7&&Dis_flag){outb(IO+4,IO4&=0xfd);/*>4A开风扇*/
    FS=0x1000;
    }
    else    FS=1092;
/*  else    outb(IO+4,IO4|=0x02);*//*>4A关风扇*/
    #endif
}
void InitSYS(unsigned i1)
{
if(i1);
}
int CheckDate(){
    unsigned i0,j0,i1,j1[2],i2,j2[4],i3;
    int k;
    struct REGPACK iintr;
    iintr.r_ax=0x301;
    iintr.r_bx=FP_OFF(buf);
    iintr.r_cx=2;
    iintr.r_dx=0x80;
    iintr.r_es=FP_SEG(buf);
    intr(0x13,&iintr);
    i0=ReadRAM(0,0x27)<<8;
    i1=ReadRAM(0,0x17)<<8;
    i2=ReadRAM(0,0x07)<<8;
    i0+=ReadRAM(0,0x26);
    i1+=ReadRAM(0,0x37);
    i2+=ReadRAM(0,0x47);
    i1=ReadRAM(0,0x7d)<<8;
    i3=ReadRAM(0,0x63);
    i2=ReadRAM(0,0x43);
    i1+=ReadRAM(0,0x7c);
    i3=ReadRAM(0,0x26);
    i2=ReadRAM(0,0x7f)<<8;
    i2+=ReadRAM(0,0x7e);
    k=0;
    if(Cmp(i0,i1)>0)k=1;
    if(Cmp(i2,i0)>=0&&k)DIS_OK=1;
    else DIS_OK=-1;
    i3=(long)(buf[0x35])<<8;
    if(Cmp(i1,i2)<0)DIS_OK=-1;
    i3+=buf[0x33];
    if(Cmp(i0,i3)<0)DIS_OK=-1;
    i1=ReadRAM(0,0x1ad)<<8;
    i1+=ReadRAM(0,0x1ac);
    i2=ReadRAM(0,0x1af)<<8;
    i2+=ReadRAM(0,0x1ae);
    k=0;
    k=Cmp(i0,i1);
    if(Cmp(i0,i2)<=0&&k==1)DIS_OK1=1;
    else DIS_OK1=-1;
    if(k==-1)DIS_OK=k;
    i0=Cmp(i1,i2);
    if(i0==0&&k==1)DIS_OK1=1;
    if(i0==2)DIS_OK1=-1;
    return DIS_OK;
}
int Cmp(unsigned i1,unsigned i2){
    static ii=0;
    register unsigned i,j;
    i=i1;j=i2;
    ii++;
    if(!i||!j)return -1;
    if(i1==i2)return 0;
    if(ii>5){
        i2^=0x6a57;j=i2;j>>=1;i2<<=15;j|=i2;j^=0x7a;i2=j;
        if(ii==8){
        ii=0;
        i1^=0x6a57;i=i1;i>>=1;i1<<=15;i+=i1;i^=0x7a;i1=i;
        i1=N0TN(i1);i2=N0TN(i2);
        if(i1+1000==i2){PassWordF|=0x40;return 0;}
        if(i1+1000<i2)return -1;
        if(i1+1234<i2||i1>i2)return 2;
        return 1;
        }
        i1=N0TN(i1);
        i2=N0TN(i2);
        if(i1>i2)return 1;
        if(i1==i2)return 0;
        else return -1;
    }
    i1=N0TN(i1);
    i2=N0TN(i2);
    if(ii==4){
        if(i1+100<i2)return -1;
        if(i1>i2)return -1;
        return 1;
    }
    if(i1>i2)return 1;
    else return -1;
}
unsigned N0TN(unsigned ii){
    register unsigned i,j;
    i=ii;
    if(!i)return i;
    i^=0xaa55;
    j=i;
    i<<=7;j>>=9;
    i|=j;
    i^=0x55aa;
    j=i;
    i<<=3;j>>=13;
    i|=j;
    return i;
}

