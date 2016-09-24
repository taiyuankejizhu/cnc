#include "znc.h"
#include<stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <dos.h>
#include <string.h>
#include "filea.h"
/*******************************************
   Œƒº˛∑÷Œˆ(ø’≈‹)”Ô∑®∑÷Œˆ£
*******************************************/
void Analyse()
{
    FILE *fpout;
    char ch,temp[10],i,k,k1,k2,flag,type[25],number[25];/*jµ•¥ œ¬±Í£¨k1¥Ê¥¢√øÃı”Ôæ‰µ•¥  ˝£¨type¥Ê¿‡–Õ∫≈£¨number¥Êπÿº¸◊÷œ¬±Í*/
    char tempcncode[25][10],G_number[5],Word_temp[11],M_number,P_number; /*tempcncode¥Ê¥¢√øÃı”Ôæ‰÷–µƒµ•¥ ,G_number G÷∏¡Ó∫≈ M_number M÷∏¡Ó∫≈*/
    int j,Word_line[25],sentance=0;  /*sentance”Ôæ‰∫≈*/
    unsigned Z_fun[12],cnc=0,cnc1=0,c1,c2,word=0,C_number[100],C_num=0,Ana_line;/*cnc÷∏¡ÓÀ≥–Ú∫≈C_number£¨C÷∏¡Ó∫≈Ana_line C÷∏¡Ó––*/
    unsigned long l;
    double dianliu; /*µÁ¡˜*/
    unsigned char kzf = 0,Grammar_Num=0,Err_Gflag=0,C_group=0,C_index=0;/*Grammar_Num ”Ô∑®¥ÌŒÛ∫≈C_group C÷∏¡Óº”π§≤Œ ˝◊ÈC_index C÷∏¡Óº”π§≤Œ ˝––*/
    struct  CncCode Temp_cncode[1000];/*Temp_cncode cnc÷∏¡Ó¥Ê¥¢ ˝◊È*/
    struct Tab Ana_table; /*º”π§≤Œ ˝*/
    char function=1,f_Group=0;
    char test[10];
    init();            /*≥ı ºªØ*/
    if(scanner())         /*…®√Ë‘¥≥Ã–Ú*/
      {
         fpout=fopen("FILEA\\output.txt","w");
         if(!fpout)
            {ListMess1("ERROR");return ;}
          k1=0;
          COMPL01_Adress=0;
         for(word=0;word<Word_list;word++)
         {
           memcpy(Word_temp,COMPL01+COMPL01_Adress,12);
           COMPL01_Adress+=12;
           type[k1]=Word_temp[0];
           number[k1]=Word_temp[1];
           strncpy(tempcncode[k1],Word_temp+2,8);
           Word_line[k1]=Word_temp[10];
           Word_line[k1]+=Word_temp[11]<<8;
           fprintf(fpout,"%s\t\t%d\t\t%d\t\t%d\n",tempcncode[k1],type[k1],number[k1],Word_line[k1]);
         }
         fclose(fpout);
         word=0;

         COMPL01_Adress=0;
         while(word<Word_list&&!Err_Gflag)
           {
               k1=0;
               do
                 {
                    memcpy(Word_temp,COMPL01+COMPL01_Adress,12);
                    COMPL01_Adress+=12;
                    type[k1]=Word_temp[0];
                    number[k1]=Word_temp[1];
                    strncpy(tempcncode[k1],Word_temp+2,8);
                    Word_line[k1]=Word_temp[10];
                    Word_line[k1]+=Word_temp[11]<<8;
                    k1++;
                 }while(!(4==type[k1 - 1]&&5==number[k1-1])&&k1<25);/*¥¶¿Ì√ø“ªæ‰*/
               word+=k1;
               if(k1&&(type[0]!=1||number[0]>14)) /*number[0]>14¥Û”⁄14µƒπÿº¸◊÷≤ªƒ‹◊˜Œ™¥ÚÕ∑÷∏¡Ó*/
                 {Grammar_Num=1;sentance=Word_line[k1-1];Err_Gflag=1;}
               if(k1>=25)
                 {Grammar_Num=2;sentance=Word_line[k1-1];Err_Gflag=1;}
               flag=1;
               i=0;
               if(1==type[i]&&(number[i]==1||number[i]==2))/*¥¶¿ÌG÷∏¡Ó*/
               {
               while(i<k1)
                 {
                    if(type[i]!=1&&type[i])
                      flag=0;
                    if(type[i]==1&&!flag&&number[i]>0&&number[i]<15)
                      {Grammar_Num=3;sentance=Word_line[i];Err_Gflag=1;}
                    k2=0;
                    while(type[i]==1&&(number[i]==1||number[i]==2))
                       {
                          if(k2>4)
                            {Grammar_Num=4;Err_Gflag=1;sentance=Word_line[i-1];}
                          G_number[k2]=atoi(tempcncode[i]+1);
                          i++;
                          k2++;
                       }
                    if(type[i]==1&&number[i]>14&&number[i]<21&&(type[i+1]!=4||type[i+2]!=3||i+3>k1||number[i+1]>2))
                      {Grammar_Num=5;Err_Gflag=1;sentance=Word_line[i];}
                    if(k2)
                      {
                         for(j=0;j<k2;j++)
                           if(G_number[j]<30)
                             {Temp_cncode[cnc+j].Code=G_number[j];Temp_cncode[cnc+j].line=Word_line[j];}
                           else
                             {Grammar_Num=6;sentance=Word_line[j];Err_Gflag=1;}
                         cnc1=cnc;
                         cnc+=k2-1;
                         Temp_cncode[cnc].X=0x7fffffff;
                         Temp_cncode[cnc].Y=0x7fffffff;
                         Temp_cncode[cnc].Z=0x7fffffff;
                         Temp_cncode[cnc].R=0x7fffffff;
                         Temp_cncode[cnc].I=0x7fffffff;
                         Temp_cncode[cnc].J=0x7fffffff;
                         Temp_cncode[cnc].K=0x7fffffff;
                         Temp_cncode[cnc].N=cnc;
                         Temp_cncode[cnc].D=C_index;
                         Temp_cncode[cnc].G=C_group;

                      }
                    for(c2=cnc1;c2<=cnc;c2++)
                        Temp_cncode[c2].N=c2;
                    if(type[i]==1&&(number[i]==15||number[i]==16))  /*¥¶¿ÌX+????*/
                      {
                         l = stol(tempcncode[i+2]);
                         if(number[i+1]==2)Temp_cncode[cnc1].X=-l;else Temp_cncode[cnc1].X=l;
                         for(c2=cnc1;c2<=cnc;c2++)
                             {Temp_cncode[c2].X=Temp_cncode[cnc1].X;}
                         i+=3;
                         flag=0;
                       }
                    else if(type[i]==1&&(number[i]==17||number[i]==18)) /*¥¶¿ÌY+????*/
                      {
                         l = stol(tempcncode[i+2]);
                         if(number[i+1]==2)Temp_cncode[cnc1].Y=-l;else Temp_cncode[cnc1].Y=l;
                         for(c2=cnc1;c2<=cnc;c2++)
                             Temp_cncode[c2].Y=Temp_cncode[cnc1].Y;
                         i+=3;
                         flag=0;
                       }
                     else if(type[i]==1&&(number[i]==19||number[i]==20))  /*¥¶¿ÌZ+????*/
                      {
                         l = stol(tempcncode[i+2]);
                         if(number[i+1]==2)Temp_cncode[cnc1].Z=-l;else Temp_cncode[cnc1].Z=l;
                         for(c2=cnc1;c2<=cnc;c2++)
                             Temp_cncode[c2].Z=Temp_cncode[cnc1].Z;
                         i+=3;
                         flag=0;
                       }
                     else if(type[i]==1&&(number[i]==21||number[i]==22)) /*¥¶¿ÌR+????*/
                      {
                         l = stol(tempcncode[i+2]);
                         if(number[i+1]==2)Temp_cncode[cnc1].R=-l;else Temp_cncode[cnc1].R=l;
                         for(c2=cnc1;c2<=cnc;c2++)
                             Temp_cncode[c2].R=Temp_cncode[cnc1].R;
                         i+=3;
                         flag=0;
                       }
                     else if(type[i]==1&&(number[i]==23||number[i]==24)) /*¥¶¿ÌI+????*/
                      {
                         l = stol(tempcncode[i+2]);
                         if(number[i+1]==2)Temp_cncode[cnc1].I=-l;else Temp_cncode[cnc1].I=l;
                         for(c2=cnc1;c2<=cnc;c2++)
                             Temp_cncode[c2].I=Temp_cncode[cnc1].I;
                         i+=3;
                         flag=0;
                       }
                     else if(type[i]==1&&(number[i]==25||number[i]==26)) /*¥¶¿ÌI+????*/
                      {
                         l = stol(tempcncode[i+2]);
                         if(number[i+1]==2)Temp_cncode[cnc1].J=-l;else Temp_cncode[cnc1].J=l;
                         for(c2=cnc1;c2<=cnc;c2++)
                             Temp_cncode[c2].J=Temp_cncode[cnc1].J;
                         i+=3;
                         flag=0;
                       }
                     else if(type[i]==1&&(number[i]==27||number[i]==28)) /*¥¶¿ÌI+????*/
                      {
                         l = stol(tempcncode[i+2]);
                         if(number[i+1]==2)Temp_cncode[cnc1].K=-l;else Temp_cncode[cnc1].K=l;
                         for(c2=cnc1;c2<=cnc;c2++)
                             Temp_cncode[c2].K=Temp_cncode[cnc1].K;
                         i+=3;
                         flag=0;
                       }
                     else
                       i++;
                     }
                  }/*G end*/
               else if(1==type[i]&&(number[i]==5||number[i]==6))/*¥¶¿ÌC÷∏¡Ó*/
                 {
                   if(2!=k1&&20!=k1)
                    {Grammar_Num=7;Err_Gflag=1;sentance=Word_line[i];i+=k1;}
                   if(i<k1)
                     {
                       i++;
                       if(20==k1&&(4!=type[i]||(4==type[i]&&number[i]!=4)))
                         {Grammar_Num=8;Err_Gflag=1;sentance=Word_line[i];}
                       else if(20==k1)
                         {
                            Temp_cncode[cnc].Code=250;Temp_cncode[cnc].line=Word_line[i];/*C÷∏¡Ó*/
                            C_number[C_num++]=atoi(tempcncode[0]+1);
                            if(C_num>101)
                              {Grammar_Num=8;Err_Gflag=1;i+=k1;}
                            Read_Table(C_number[C_num-1]%100);
                            Ana_line=C_number[C_num-1]/100;
                            Table.Index[Ana_line]=1;
                            if('-'==tempcncode[2][0])
                              Table.Jixin[Ana_line]=0;
                            else
                              Table.Jixin[Ana_line]=1;
                            dianliu=CNC_atof(tempcncode[3])*(double)2;
                            Table.Dianliu[Ana_line]=(int)dianliu;
                            Table.Maikuan[Ana_line]=atoi(tempcncode[4]);
                            Table.Xiuzhi[Ana_line]=atoi(tempcncode[5]);
                            Table.Jianxi[Ana_line]=atoi(tempcncode[6]);
                            Table.Sudu[Ana_line]=atoi(tempcncode[7]);
                            Table.Shenggao[Ana_line]=atoi(tempcncode[8]);
                            Table.Gongshi[Ana_line]=atoi(tempcncode[9]);
                            Table.Mianji[Ana_line]=atoi(tempcncode[12]);
                            Table.Gaoya[Ana_line]=atoi(tempcncode[10]);
                            Table.B_OFS[Ana_line]=atoi(tempcncode[13]);
                            Table.S_OFS[Ana_line]=atoi(tempcncode[14]);
                            Table.LNS[Ana_line]=atoi(tempcncode[15]);
                            Table.STEP[Ana_line]=atol(tempcncode[16]);
                            Table.LMN[Ana_line]=atoi(tempcncode[17]);
                            Table.RY[Ana_line]=atoi(tempcncode[18]);
                            Write_Table(C_number[C_num-1]%100);
                         }
                       else if(2==k1)
                         {
                           Temp_cncode[cnc].Code=250;Temp_cncode[cnc].line=Word_line[i];/*C÷∏¡Ó*/
                           Ana_line=atoi(tempcncode[0]+1);
                           for(c2=0;c2<C_num;c2++)
                             if(Ana_line==C_number[c2])
                               {

                                 C_group=Ana_line%100;
                                 C_index=Ana_line/100;
                                 C_index<<=4;
                                 C_index+=Ana_line/100;
                                 c2=C_num+1;
                               }
                            if(c2==C_num)
                              {Grammar_Num=9;Err_Gflag=1;sentance=Word_line[i];}
                         }
                     }
                 }/*C end*/
               else if(1==type[i]&&(number[i]==3||number[i]==4))/*¥¶¿ÌM÷∏¡Ó*/
                 {
                   M_number=atoi(tempcncode[0]+1);
                   if(M_number>8)
                     {Grammar_Num=6;sentance=Word_line[j];Err_Gflag=1;}
                   else
                     {
                       if((M_number>=0&&M_number<=8&&M_number!=6&&M_number!=1&&2!=k1)||(M_number==6&&3!=k1))
                         {Grammar_Num=10;Err_Gflag=1;sentance=Word_line[i];}
                       M_number+=100;
                       Temp_cncode[cnc].Code=M_number;
                       Temp_cncode[cnc].N=0;
                       Temp_cncode[cnc].line=Word_line[i];
                     }
                   if(108==M_number||107==M_number)
                     if(function)
                       {
                          function=0;
                          if(!f_Group)
                            {Write_Code(f_Group);}/*÷˜∫Ø ˝¥Ê¥¢*/
                          else
                            Write_Code1(f_Group);/*◊”∫Ø ˝¥Ê¥¢*/
                          f_Group++;
                        }
                     else
                      {Grammar_Num=11;Err_Gflag=1;sentance=Word_line[i];}
                 }
               else if(1==type[i]&&(number[i]==9||number[i]==10))/*¥¶¿ÌN÷∏¡Ó*/
                 {
                   c2=0;
                   c2=atoi(tempcncode[0]+1);
                   Temp_cncode[cnc].Code=255;/*◊”∫Ø ˝µƒø™ º£¨º¥Œ™∏√––¥˙¬Îµƒœ¬“ª––*/
                   Temp_cncode[cnc].R = c2;/*¥Ê¥¢◊”∫Ø ˝µƒ±‡∫≈,*/
                   Temp_cncode[cnc].line=Word_line[i];
                   Z_fun[kzf++] = cnc + 1;/*¥Êœ¬◊”∫Ø ˝ø™ º––∫≈*/
                   if(f_Group==(c2)&&f_Group<12)
                     if(!function)
                       function=1;
                     else
                      {Grammar_Num=11;Err_Gflag=1;sentance=Word_line[i];}
                   else
                     {Grammar_Num=12;Err_Gflag=1;sentance=Word_line[i];}
                 }
               else if(1==type[i]&&(number[i]==13||number[i]==14))/*¥¶¿ÌP÷∏¡Ó*/
                 {
                   P_number=atoi(tempcncode[0]+1);
                   Temp_cncode[cnc].Code=106;
                   Temp_cncode[cnc].R=P_number; /*¥Ê¥¢◊”∫Ø ˝±‡∫≈*/
                   Temp_cncode[cnc].line=Word_line[i];
                   if(!(1==k1||2==k1||3==k1))
                     {
                       {Grammar_Num=13;Err_Gflag=1;sentance=Word_line[i];}
                     }
                   else if(P_number>=12)
                     {Grammar_Num=14;Err_Gflag=1;sentance=Word_line[i];}
                   else if(3==k1&&1==type[0]&&(number[0]==13||number[0]==14))
                     {
                        Temp_cncode[cnc].I=atoi(tempcncode[1]+1);/*¥Ê¥¢—≠ª∑¥Œ ˝*/
                     }
                 }
               Temp_cncode[cnc].N=cnc;
               cnc++;
           }
          if(function && !Err_Gflag)
            {
               Grammar_Num=11;
               Err_Gflag=1;
               memcpy(Word_temp,COMPL01+(Word_list-1)*12,12);
               sentance=Word_temp[10];
               sentance+=Word_temp[11]<<8;
            }
          for(c1=0;c1<cnc;c1++)
          {
            if(Temp_cncode[c1].Code == 106)
            {
                if(Temp_cncode[c1].R <= kzf && kzf && Temp_cncode[c1].R)
                  {
                    Temp_cncode[c1].I = Z_fun[Temp_cncode[c1].R - 1];/*I¥Ê¥¢◊”∫Ø ˝∆ ºµÿ÷∑*/
                    if(Temp_cncode[c1].I >= cnc)
                     {Grammar_Num=16;Err_Gflag=1;}
                  }
                else
                {Grammar_Num=16;Err_Gflag=1;}
            }
          }
          if(!Err_Gflag)
            {
              InitCNC();
              ListMess1("");
              memcpy(Code,&Temp_cncode,cnc*35);
              Write_Code(0);
              Dis_flag = 0;/*≥ı ºªØ◊º±∏∑≈µÁ*/
              fpout=fopen("FILEA\\output1.txt","w"); /*≤‚ ‘”√C÷∏¡Ó¥Ê¥¢Œƒº˛*/
              if(!fpout)
                {ListMess1("ERROR");return ;}
              for(c1=0;c1<cnc;c1++)
                {
                  memcpy(&Temp_cncode[c1],Code+c1*35,35);
                  fprintf(fpout,"%d\t%d\t%ld\t%ld\t%ld\t%ld\t%d\t%ld\t%ld\t%ld\n",Temp_cncode[c1].N,Temp_cncode[c1].Code,
                  Temp_cncode[c1].X,Temp_cncode[c1].Y,Temp_cncode[c1].Z,Temp_cncode[c1].R,Temp_cncode[c1].line
                  ,Temp_cncode[c1].I,Temp_cncode[c1].J,Temp_cncode[c1].K);
                }
              StartCode();
              fclose(fpout);
            }
          else
            st_manage(Grammar_Num,sentance);
       }
    else
      {
         ListHT(1,"PROGRAM_ERROR");
       }
    return ;
}
/********************************************
Ω´◊÷∑˚¥Æ◊™ªªŒ™≥§’˚–Õ
********************************************/
long stol(char str[])
{
    long l;
    unsigned c2 = 0,c3 = 0;
    char c1;
    while(str[c3] != '.' && str[c3] != '\0')
    {
        c1 = str[c3];if(c1>='0'&&c1<='9')c1-='0';else c1=0;c2*=10;c2+=c1;
        c3++;
    }
    l=(long)c2*1000;c2=0;
    if('.' == str[c3])
    {
        c3++;
        while(str[c3] != '\0')
        {
            c1=str[c3];if(c1>='0'&&c1<='9')c1-='0';else c1=0;c2*=10;c2+=c1;
            c3++;
        }
    }
    else
        c2=0;
    l+=c2;
    return l;
}
/*******************************************
 ≥ı ºªØ∫Ø ˝
********************************************/
int init()
{
     COMPL01_Adress=0;
     Program=BMP+80000;
     Word_list=0;
     Wrong_Num=0;
     return 0;
}
/*******************************************
 ∏˘æ›≤ªÕ¨√¸¡Ó≤È±ÌªÚ‘Ï±Ì∫Ø ˝
********************************************/
int find(char *buf,int type)
{             
    int number=0,K_num=-1,j=0;
    char c;
    int flag=0,i=0;
    switch(type)
    {
        case 1: K_num=KEYWORD_NUM;break;
        case 4: K_num=DELIMITERS_NUM;break;
    }
    if(-1==K_num)
      return 0;
    else if(KEYWORD_NUM==K_num)
      c=_KeyWords_[j];
    else
      c=_Delimiters_[j];
    while(j<=K_num)
    {
        flag=0;
        number++;
        if(type==1&&c==buf[0])
          {
            if(c=='G'||c=='g'||c=='M'||c=='m') /*G00-G99,M00-M99*/
              {
                if(strlen(buf)==3)
                  {
                    for(i=1;i<strlen(buf);i++)
                      if(buf[i]<'0'||buf[i]>'9')
                        flag=1;
                    if(!flag)
                      {
                        return(number);        /*»Ù’“µΩ£¨∑µªÿ‘⁄œ‡”¶±Ì÷–µƒ–Ú∫≈*/
                      }
                    else
                      {Wrong_Num=2;return 0;}
                   }
                Wrong_Num=3;
                return 0;
              }
            else if(c=='C'||c=='c') /*C000-C999*/
              {
                if(strlen(buf)==4)
                  {
                    for(i=1;i<strlen(buf);i++)
                      if(buf[i]<'0'||buf[i]>'9')
                        flag=1;
                    if(!flag)
                      {
                        return(number);        /*»Ù’“µΩ£¨∑µªÿ‘⁄œ‡”¶±Ì÷–µƒ–Ú∫≈*/
                      }
                    else
                      {Wrong_Num=4;return 0;}
                   }
                Wrong_Num=5;
                return 0;
              }
            else if(c=='H'||c=='h'||c=='O'||c=='o')
              {                           /*H0000-H9999,O0000-O9999*/
                if(strlen(buf)==5)
                  {
                    for(i=1;i<strlen(buf);i++)
                      if(buf[i]<'0'||buf[i]>'9')
                        flag=1;
                    if(!flag)
                      {
                        return(number);        /*»Ù’“µΩ£¨∑µªÿ‘⁄œ‡”¶±Ì÷–µƒ–Ú∫≈*/
                      }
                    else
                      {Wrong_Num=6;return 0;}
                   }
                Wrong_Num=7;
                return 0;
              }
            else if(c=='P'||c=='p'||c=='N'||c=='n')/*N00-N99,P00-P99*/
              {
                if(strlen(buf)==3)
                  {
                    for(i=1;i<strlen(buf);i++)
                      if(buf[i]<'0'||buf[i]>'9')
                        flag=1;
                    if(!flag)
                      {
                        return(number);        /*»Ù’“µΩ£¨∑µªÿ‘⁄œ‡”¶±Ì÷–µƒ–Ú∫≈*/
                      }
                    else
                      {Wrong_Num=11;return 0;}
                   }
                Wrong_Num=12;
                return 0;
              }
            else /*X,Y,Z,I,J,K*/
              {
                 if(strlen(buf)==1)
                   {
                     return(number);        /*»Ù’“µΩ£¨∑µªÿ‘⁄œ‡”¶±Ì÷–µƒ–Ú∫≈*/
                   }
                 Wrong_Num=8;
                 return 0;
              }
          }
        else if(type==4&&c==buf[0])
          {
            if(strlen(buf)==1)
              {
                 return(number);        /*»Ù’“µΩ£¨∑µªÿ‘⁄œ‡”¶±Ì÷–µƒ–Ú∫≈*/
              }
            Wrong_Num=9;
            return 0;
          }
        else
          {
            if(KEYWORD_NUM==K_num&&++j<=K_num)
              c=_KeyWords_[j];
            if(DELIMITERS_NUM==K_num&&++j<=K_num)
              c=_Delimiters_[j];
          }
     }
     return 0;                /*’“≤ªµΩ£¨µ±÷ª–Ë≤È±Ì£¨∑µªÿ0£¨∑Ò‘Úªπ–Ë‘Ï±Ì*/
}

/*******************************************
  ˝◊÷¥Æ¥¶¿Ì∫Ø ˝
********************************************/
int cs_manage(char *buffer)
{
    char Cs_temp[]={3,0};
    if(strlen(buffer)>7){Wrong_Num=1;return 0;}
    memcpy(COMPL01+COMPL01_Adress++,Cs_temp,1);/* ˝◊÷µƒµ⁄“ª∏ˆ◊÷Ω⁄–¥type*/
    memcpy(COMPL01+COMPL01_Adress++,Cs_temp+1,1);
    memcpy(COMPL01+COMPL01_Adress,buffer,strlen(buffer)+1);
    COMPL01_Adress+=8;
    Word_temp=F_line>>8;
    memcpy(COMPL01+COMPL01_Adress,&Word_temp,1);
    Word_temp=F_line&0xff;
    memcpy(COMPL01+COMPL01_Adress,&Word_temp,1);
    COMPL01_Adress+=2;
    Word_list++;
    return 1;
}

/*******************************************
 ◊÷∑˚¥Æ¥¶¿Ì∫Ø ˝ 
********************************************/
int ch_manage(char *buffer)
{                     
    int result;
    char Ch_temp=1;
    result=find(buffer,1);           /*œ»≤Èπÿº¸◊÷±Ì*/
    if(result!=0)
      {
        memcpy(COMPL01+COMPL01_Adress++,&Ch_temp,1);/*√¸¡Óµƒµ⁄“ª∏ˆ◊÷Ω⁄–¥type*/
        memcpy(COMPL01+COMPL01_Adress++,&result,1);
        memcpy(COMPL01+COMPL01_Adress,buffer,strlen(buffer)+1);
        COMPL01_Adress+=8;
        Word_temp=F_line>>8;
        memcpy(COMPL01+COMPL01_Adress,&Word_temp,1);
        Word_temp=F_line&0xff;
        memcpy(COMPL01+COMPL01_Adress,&Word_temp,1);
        COMPL01_Adress+=2;
        Word_list++;
      }
    return result;
}

/*******************************************
 µ•¥ ≥ˆ¥Ì¥¶¿Ì∫Ø ˝
********************************************/
void er_manage(char error,int lineno)
{
    char temp[10],temp1[50],temp2[2],temp3[]="WORD_ERROR??";
    strcpy(temp1,"error: ");
    itoa(lineno,temp,10);
    temp2[0]=error;
    temp2[1]='\0';
    strcat(temp1,temp2);
    strcat(temp1," ,line ");
    strcat(temp1,temp);
    strcat(temp1,",");
    temp3[10]=Wrong_Num/10+'0';
    temp3[11]=Wrong_Num%10+'0';
    strcat(temp1,temp3);
    ListMess1(temp1);    /*±®∏Ê≥ˆ¥Ì∑˚∫≈∫ÕÀ˘‘⁄–– ˝*/
    return ;
}
/*******************************************
”Ô∑®≥ˆ¥Ì¥¶¿Ì∫Ø ˝
********************************************/
void st_manage(unsigned char ErrNo,unsigned sentance)
{
    char sentan[10],Sen_temp[50];
    itoa(sentance,sentan,10);
    strcpy(Sen_temp,"Grammar_ERROR?? at line ");
    Sen_temp[13]=ErrNo/10+'0';
    Sen_temp[14]=ErrNo%10+'0';;
    strcat(Sen_temp,sentan);
    ListMess1(Sen_temp);
    return ;
}
/*******************************************
 …®√Ë≥Ã–Ú
********************************************/
char scanner()
{            
    FILE *fpin;
    char filename[50];
    char ch;
    int i=0,Err_flag=0,Errfl=0,cgetflag,j=0;
    unsigned int con=0,point=0;
    int count,result;
    char array[15];
    char Csc_temp=4;
    long len,len1;

    NCStr[3]=LN05K/10+'0';NCStr[4]=LN05K%10+'0';
    if((fpin=fopen(NCStr,"rb"))==NULL)
    {
        ListMess1("FILE_ERROR1");
        return 0;
    }

    fseek(fpin,0l,SEEK_END);       /*   ÷ÿ“™   */
    len=ftell(fpin);
    if(len>20000)
      {
        ListMess1("FILE_ERROR0");
        return 0;
      }
    fseek(fpin,-len,SEEK_CUR);
    fread(Program,1,len,fpin);
    len1=0;
    fclose(fpin);
    ch=Program[len1++];
    F_line=1;
    while(len1<=len&&!Err_flag&&con<=20000)
    {            /*∞¥◊÷∑˚“¿¥Œ…®√Ë‘¥≥Ã–Ú£¨÷±÷¡Ω· ¯*/
        i=0;
        cgetflag=0;
        if(((ch>='A')&&(ch<='Z'))||((ch>='a')&&(ch<='z')))
        {           /*“‘◊÷ƒ∏ø™Õ∑*/
            while((((ch>='A')&&(ch<='Z'))||((ch>='a')&&(ch<='z'))||((ch>='0')&&(ch<='9')))&&i<9)
            {
                array[i++]=ch;
                ch=Program[len1++];
                con++;
            }
            array[i]='\0';
            Errfl=ch_manage(array);
            if(!Errfl)
              Err_flag=1;
            else if(-1==Errfl)
              Err_flag=-1;
            if(ch!=EOF)
               cgetflag=1;
        }
        else if(ch>='0'&&ch<='9')
        {          /*“‘ ˝◊÷ø™Õ∑*/
            point=0;
            while(((ch>='0'&&ch<='9')||ch=='.')&&!Err_flag&&i<9)
            {
                con++;
                if((ch!='.'||(ch=='.'&&i))&&i<9)
                  {
                    if(ch=='.')
                      point++;
                    array[i++]=ch;
                    ch=Program[len1++];
                  }
                else
                  {
                    Err_flag=1;
                  }

            }
            array[i]='\0';
            if(array[i-1]=='.'||point>1)
              {Err_flag=1;array[0]='.';Wrong_Num=10;break;}
            j=1;
            while(array[j]!='.'&&j<i)
               {
                 j++;
               }
            if(j<i)
              {
                while(i-j<4)
                  array[i++]='0';
                array[i]='\0';
              }
            if(-1==cs_manage(array))Err_flag=-1;
            if(ch!=EOF)
               cgetflag=1;
        }
        else if((ch==' ')||(ch=='\t')||(10==ch))
            ;           /*œ˚≥˝ø’∏Ò∑˚∫ÕÀÆ∆Ω÷∆±Ì∑˚*/
        else if(ch=='\n'||13==ch)
          F_line++;          /*œ˚≥˝ªÿ≥µ≤¢º«¬º–– ˝*/
        else
          {         /*‘ÀÀ„œﬁΩÁ∑˚ªÚ∑«∑®◊÷∑˚*/
             if('('==ch)/*œ˚≥˝µ⁄“ª––*/
               {
                 while(ch!=';')
                  ch=Program[len1++];
                 ch=Program[len1++];
                 F_line++;
               }
           else
             {
               array[0]=ch;
               array[1]='\0';
               result=find(array,4);      /*ºÏÀ˜ «∑ÒŒ™µ•◊÷∑˚‘ÀÀ„°¢œﬁΩÁ∑˚*/
               if(result==0)
                 {
                    Err_flag=1;
                 }
               else
                 {
                     /*»ÙŒ™µ•◊÷∑˚‘ÀÀ„°¢œﬁΩÁ∑˚*/
                   memcpy(COMPL01+COMPL01_Adress++,&Csc_temp,1);/*ΩÁ∑˚µƒµ⁄“ª∏ˆ◊÷Ω⁄–¥type*/
                   memcpy(COMPL01+COMPL01_Adress++,&result,1);
                   memcpy(COMPL01+COMPL01_Adress,array,2);
                   COMPL01_Adress+=8;
                   Word_temp=F_line>>8;
                   memcpy(COMPL01+COMPL01_Adress,&Word_temp,1);
                   Word_temp=F_line&0xff;
                   memcpy(COMPL01+COMPL01_Adress,&Word_temp,1);
                   COMPL01_Adress+=2;
                   Word_list++;
                 }
             }
          }
        if(!cgetflag)
          ch=Program[len1++];
        con++; 
    }
    if(Err_flag)
      {
        er_manage(array[0],F_line);
        return 0;
      }
    if(con>=19999)
      ListMess1("FILE_ERROR2");
    if(0==Word_list)
      return 0;
    else
      return 1;
}
