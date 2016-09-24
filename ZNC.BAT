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
tcc -1 -ml -c -f87 main1
tcc -1 -ml -c -f87 cnc1
tcc -1 -ml -c -f87 Edit.c
tcc -1 -ml -c -f87 List
tcc -1 -ml -c -f87 RunLine
tcc -1 -ml -c -f87 nc.h
tcc -1 -ml -c -f87 cnc
tcc -1 -ml -c -f87 disp
tlink @nc.lnk
