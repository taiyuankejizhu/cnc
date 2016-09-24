#include <graphics.h>
#include <stdio.h>
#include "znc.h"
#include <alloc.h>
#include "svga256.h"
int  far *Svga256_fdriver;
int huge DetectVGA256_0()
{
/*  int Vid;

  printf("Which video mode would you like to use? \n");
  printf("  0) 320x200x256\n");
  printf("  1) 640x400x256\n");
  printf("  2) 640x480x256\n");
  printf("  3) 800x600x256\n");
  printf("  4) 1024x768x256\n");
  printf("  5) 640x480x256\n");
  printf("  6) 1280x1024x256\n");
  printf("\n> ");
  scanf("%d",&Vid);
  return Vid;*/
  return 0;
}
int huge DetectVGA256_1(){return 1;}
int huge DetectVGA256_2(){return 2;}
int huge DetectVGA256_3(){return 3;}
int huge DetectVGA256_4(){return 4;}
int huge DetectVGA256_5(){return 5;}
int huge DetectVGA256_6(){return 6;}
InitScr256(int flag){
    int  Gd = DETECT, Gm;
    int  Drv;
    char GrErr;
    int graphdriver,graphmode;
        switch(flag){
        case 0:installuserdriver("Svga256",DetectVGA256_0);break;
        case 1:installuserdriver("Svga256",DetectVGA256_1);break;
        case 2:installuserdriver("Svga256",DetectVGA256_2);break;
        case 3:installuserdriver("Svga256",DetectVGA256_3);break;
        case 4:installuserdriver("Svga256",DetectVGA256_4);break;
        case 5:installuserdriver("Svga256",DetectVGA256_5);break;
        case 6:installuserdriver("Svga256",DetectVGA256_6);break;
        default:installuserdriver("Svga256",DetectVGA256_2);
        }
/*    registerfarbgidriver(Svga256_fdriver);*/
  registerbgidriver(EGAVGA_driver);
    initgraph(&Gd,&Gm,"");
/*  registerbgifont(triplex_font);*/
    registerbgifont(small_font);
      registerbgifont(sansserif_font);
/*  registerbgifont(gothic_font);*/
    GrErr = graphresult();
    if (GrErr != grOk) {
        printf("Graphics System Error: %s\n",grapherrormsg(GrErr));
        exit(1);
    }
    settextjustify(1,1);
    settextstyle(2,0,5);
    cleardevice();
    setviewport(0,0,getmaxx(),getmaxy(),0);
}
