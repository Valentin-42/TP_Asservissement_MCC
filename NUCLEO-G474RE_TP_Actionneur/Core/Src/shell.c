/*
 * shell.c
 *
 *  Created on: Oct 21, 2022
 *      Author: valen
 */
#include "main.h"
#include "gpio.h"
#include "usart.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern uint8_t uartRxBuffer[];
extern uint8_t uartTxBuffer[];

static uint8_t prompt[]="user@Nucleo-STM32G431>>";
static uint8_t cmdNotFound[]="Command not found\r\n";

const uint8_t help[]=""
		"\r\n*-----------------------------*"
		"\r\n| Commandes disponible        |"
		"\r\n| 1. pinout                   |"
		"\r\n| 2. start                    |"
		"\r\n| 3. stop                     |"
		"\r\n*-----------------------------*"
		"\r\n"; // contenant le message d'aide, la liste des fonctions

const uint8_t pinout[]=""
		"\r\n*-----------------------------*"
		"\r\n| Broches & fonctions         |"
		"\r\n| PA12 -> TIM1_CH2N           |"
		"\r\n| PA11 -> TIM1_CH1N           |"
		"\r\n| PA9 -> TIM2_CH2             |"
		"\r\n| PA8 -> TIM2_CH1             |"
		"\r\n| PC3 -> ISO_RESET            |"
		"\r\n*-----------------------------*"
		"\r\n"; // contenant la liste des pin utilisées

const uint8_t powerOn[]="POWER ON\r\n"; // contenant le message d'allumage du moteur
const uint8_t powerOff[]="POWER OFF\r\n";

void handle_command(char *argv[]){
	if(strcmp(argv[0],"set")==0){
		if(strcmp(argv[1],"PA5")==0){
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, atoi(argv[2]));
			sprintf(uartTxBuffer,"Switch on/off led : %d\r\n",atoi(argv[2]));
			HAL_UART_Transmit(&huart2, uartTxBuffer, 32, HAL_MAX_DELAY);
		}
		else{
			HAL_UART_Transmit(&huart2, cmdNotFound, sizeof(cmdNotFound), HAL_MAX_DELAY);
		}
	}
	else if(strcmp(argv[0],"get")==0)
	{
		HAL_UART_Transmit(&huart2, cmdNotFound, sizeof(cmdNotFound), HAL_MAX_DELAY);
	}
	else if(strcmp(argv[0],"help")==0)
	{
		HAL_UART_Transmit(&huart2, help, sizeof(help), HAL_MAX_DELAY);
	}
	else if(strcmp(argv[0],"pinout")==0)
	{
		HAL_UART_Transmit(&huart2, pinout, sizeof(pinout), HAL_MAX_DELAY);
	}
	else if(strcmp(argv[0],"start")==0)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
		HAL_UART_Transmit(&huart2, powerOn, sizeof(powerOn), HAL_MAX_DELAY);
	}
	else if(strcmp(argv[0],"stop")==0)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
		HAL_UART_Transmit(&huart2, powerOff, sizeof(powerOff), HAL_MAX_DELAY);
	}
	else{
		HAL_UART_Transmit(&huart2, cmdNotFound, sizeof(cmdNotFound), HAL_MAX_DELAY);
	}
	HAL_UART_Transmit(&huart2, prompt, sizeof(prompt), HAL_MAX_DELAY);
}
