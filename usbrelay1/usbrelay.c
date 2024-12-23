#include "ch32v003fun.h"
#include <stdio.h>
#include <string.h>
#include "rv003usb.h"

#define RL_NUM 1
#define DATA_LEN 8
#define SERIAL_LEN 6
#define ON FUN_LOW
#define OFF FUN_HIGH

uint8_t RL[] = {PD6}; // can add more if has more than 1 relay
uint8_t handled_relay = 0;
uint8_t data_to_host[DATA_LEN];
uint8_t data_to_device[DATA_LEN];

void handle_relay(uint8_t cmd, uint8_t port)
{
	if (cmd == 1)
	{
		if ((port >= 1) && (port <= RL_NUM))
		{
			funDigitalWrite(RL[port - 1], ON);
		}
		else
		{
			for (uint8_t i = 1; i <= RL_NUM; i++)
			{
				funDigitalWrite(RL[i - 1], ON);
			}
		}
	}
	else if (cmd == 0)
	{
		if ((port >= 1) && (port <= RL_NUM))
		{
			funDigitalWrite(RL[port - 1], OFF);
		}
		else
		{
			for (uint8_t i = 1; i <= RL_NUM; i++)
			{
				funDigitalWrite(RL[i - 1], OFF);
			}
		}
	}
}

int main()
{
	SystemInit();
	Delay_Ms(1);
	usb_setup();
	funGpioInitAll();
	for (uint8_t i = 0; i < RL_NUM; i++)
	{
		funPinMode(RL[i], GPIO_Speed_10MHz | GPIO_CNF_OUT_PP);
		funDigitalWrite(RL[i], OFF);
	}
	memcpy(data_to_host, DEVICE_ID, SERIAL_LEN);
	while (1)
	{
		if (handled_relay == 0)
		{
			handled_relay = 1;
			if (data_to_device[0] == 0xFF)
				handle_relay(1, data_to_device[1]);
			else if (data_to_device[0] == 0xFD)
				handle_relay(0, data_to_device[1]);
			else if (data_to_device[0] == 0xFE)
				handle_relay(1, RL_NUM + 1);
			else if (data_to_device[0] == 0xFC)
				handle_relay(0, RL_NUM + 1);
			else if (data_to_device[0] == 0xFA) 
				handled_relay = 1;
				// TODO: write new serial to flash
		}
	}
}

void usb_handle_user_in_request(struct usb_endpoint *e, uint8_t *scratchpad, int endp, uint32_t sendtok, struct rv003usb_internal *ist)
{
	usb_send_data(data_to_host, DATA_LEN, 0, sendtok);
	if (endp)
	{
		usb_send_data(data_to_host, DATA_LEN, 0, sendtok);
	}
	else
	{
		usb_send_empty(sendtok);
	}
}

void usb_handle_user_data(struct usb_endpoint *e, int current_endpoint, uint8_t *data, int len, struct rv003usb_internal *ist)
{
	handled_relay = 0;
	memcpy(data_to_device, data, DATA_LEN);
}

void usb_handle_hid_get_report_start(struct usb_endpoint *e, int reqLen, uint32_t lValueLSBIndexMSB)
{
	if (reqLen > sizeof(data_to_host))
	{
		reqLen = sizeof(data_to_host);
	}
	e->opaque = data_to_host;
	e->max_len = reqLen;
}

void usb_handle_hid_set_report_start(struct usb_endpoint *e, int reqLen, uint32_t lValueLSBIndexMSB)
{
	if (reqLen > sizeof(data_to_host))
		reqLen = sizeof(data_to_host);
	e->max_len = reqLen;
}

void usb_handle_other_control_message(struct usb_endpoint *e, struct usb_urb *s, struct rv003usb_internal *ist)
{
	LogUEvent(SysTick->CNT, s->wRequestTypeLSBRequestMSB, s->lValueLSBIndexMSB, s->wLength);
}
