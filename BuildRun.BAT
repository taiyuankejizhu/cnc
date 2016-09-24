del bmp.obj
del cnc1.obj
del cnc2.obj
del cnc.obj
del disp1.obj
del disp.obj
del edit.obj
del filea.obj
del graphic.obj
del key.obj
del keyboard.obj
del list.obj
del listln.obj
del loran.obj
del main1.obj
del main.obj
del motion.obj
del runline.obj
del nc.obj
del svga256.obj
del nc.map
del *.bak
del nc.exe

@echo off
tcc -1 -ml -c -f87 main
tcc -1 -ml -c -f87 table
tcc -1 -ml -c -f87 bmp
tcc -1 -ml -c -f87 svga256
tcc -1 -ml -c -f87 graphic
tcc -1 -ml -c -f87 cnc2
tcc -1 -ml -c -f87 Motion

tcc -1 -ml -c -f87 key
tcc -1 -ml -c -f87 KeyBoard
tcc -1 -ml -c -f87 Loran
tcc -1 -ml -c -f87 filea
tcc -1 -ml -c -f87 ListLN
tcc -1 -ml -c -f87 cnc1
tcc -1 -ml -c -f87 Edit.c
tcc -1 -ml -c -f87 List
tcc -1 -ml -c -f87 RunLine
tcc -1 -ml -c -f87 nc.h
tcc -1 -ml -c -f87 cnc
tcc -1 -ml -c -f87 disp
tlink @nc.lnk
nc.exe