all : flash

TARGET:=usbrelay
CH32V003FUN:=../ch32v003fun/ch32v003fun
MINICHLINK?=../ch32v003fun/minichlink

ADDITIONAL_C_FILES+=../lib/rv003usb.S ../lib/rv003usb.c
EXTRA_CFLAGS:=-I../lib -I./usbrelay1

include ../ch32v003fun/ch32v003fun/ch32v003fun.mk

flash : cv_flash
clean : cv_clean


