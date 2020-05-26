/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f0xx.h"
#include "stm32f0308_discovery.h"


#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "display.h"
#include "comm.h"
#include "usb.h"
#include "process.h"
extern int keypad_init(void);


unsigned int  gState 	= EJACQUARD_STATE_IDLE;
unsigned int  gFileType	= EJACQUARD_FILE_TYPE_PALLU; /* Assuming default FileType is pallu */


#define LED_PIN GPIO_Pin_9

void delay(int counter)
{
	int i;
	for(i = 0; i < counter*1000; i++)
	{

	}

}




/**

Main entry point of the program.

*/



int main(void)
{

	//Initialize the GPIO data structre
		GPIO_InitTypeDef gpio;
		/* Enable the peripheral Clock */
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);
		//Fills each GPIO_InitStruct member with its default value.
		/* Reset GPIO init structure parameters values */
		GPIO_StructInit(&gpio);
		// Assign pin number
		gpio.GPIO_Pin = LED_PIN;
		// make as out put pin
		gpio.GPIO_Mode = GPIO_Mode_OUT;
		//Initializes the GPIOx peripheral according to the specified
		 // * parameters in the GPIO_InitStruct.
		GPIO_Init(GPIOC,&gpio);
		GPIO_SetBits(GPIOC,LED_PIN);



		while(1)
		{
			GPIO_SetBits(GPIOC,LED_PIN);
			delay(200);
			GPIO_ResetBits(GPIOC,LED_PIN);
			delay(200);
		}








	int ret = 0;

	ret = display_init();
	if (ret != 0 ) return -1;

	ret = comm_init();
	if (ret != 0 ) return -2;

	ret = usb_init();
	if (ret != 0 ) return -3;

	ret = keypad_init();
	if (ret != 0 ) return -4;

	ret = process_init();
	if (ret != 0 ) return -5;

	while (1) {

		switch(gState)
		{
			case EJACQUARD_STATE_IDLE:
				/* Do Nothing in Keypad intrrupt mode*/
				/* else we need to poll for keypad events*/
				break;
			case EJACQUARD_STATE_USB:
				usb_process_files();
				CHANGE_STATE(EJACQUARD_STATE_USB_CONFIG);
				break;
			case EJACQUARD_STATE_USB_CONFIG:
				/* Do Nothing in Keypad intrrupt mode*/
				/* else we need to poll for keypad events*/
				break;
			case EJACQUARD_STATE_RUNNING:
				ret = comm_process_cmd();
				if (ret != 0) {
				 	// TODO : Need to thing error case
					// We may restart the comm_reinit and check
					printf("Comm read failed , may need to re init\n");
				}
				break;
			default:
				break;
		}
	}
	return 0;

}



