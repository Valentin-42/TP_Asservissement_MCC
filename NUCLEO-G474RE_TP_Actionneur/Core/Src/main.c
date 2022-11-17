/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * Contrôle de la vitesse de rotation d'une machine à courant continu par l'intérmédiaire d'un shell communiquant sur liaison UART.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "shell.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define UART_TX_BUFFER_SIZE 64
#define UART_RX_BUFFER_SIZE 1
#define CMD_BUFFER_SIZE 64
#define MAX_ARGS 9
#define ASCII_LF 0x0A
#define ASCII_CR 0x0D
#define ASCII_DEL 0x7F
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t prompt[]="user@Nucleo-STM32G431>>";
uint8_t started[]=
		"\r\n*-----------------------------*"
		"\r\n| Welcome on Nucleo-STM32G431 |"
		"\r\n*-----------------------------*"
		"\r\n";
uint8_t newline[]="\r\n";
uint8_t cmdNotFound[]="Command not found\r\n";
uint32_t uartRxReceived;
uint8_t uartRxBuffer[UART_RX_BUFFER_SIZE];
uint8_t uartTxBuffer[UART_TX_BUFFER_SIZE];
static int  ButtonState=0;
float adc_value;
adc_mean_cnt=0;
char adc_value_txt[25];
int encodeur_value1;
int encodeur_value2;
int overflow=0;
int state=0;
int ticks;
float freq;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/**
 * @brief Fonction de gestion des interruptions des timers.
 * @param TIM_HandleTypeDef *htim
 * @return None
 *
 * Interruption de TIM2 : gestion de la lecture de l'encodeur pour mesurer la vitesse de rotation du moteur et lancement d'une conversion de l'UART
 * Interruption de TIM3 : gestion des dépassements du registre de comptage de TIM3 en mode encodeur.
 */
void_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2){
		HAL_ADC_Start_IT(&hadc1);

		//Encodeur
		if(state==0){
			encodeur_value1=TIM3->CNT;//CCR1;
			overflow=0;
			state=1;
		}
		else if(state==1){
			encodeur_value2=TIM3->CNT;//CCR1;
			ticks=encodeur_value2 + (overflow * 65535) - encodeur_value2;
			freq = (float) 10000 / (float) ticks;
			state=0;
		}
	}
	if(htim->Instance == TIM3){
		overflow++;
	}
}
/**
 * @brief Fonction de gestion des interruptions de l'ADC.
 * @param ADC_HandleTypeDef *hadc
 * @return None
 *
 * Interruption lors de la fin d'une conversion de l'ADC. Moyennage sur 100 acquisitions et affichage de la valeur convertie.
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc){
	adc_value+=((((float)HAL_ADC_GetValue(&hadc1)*3.3)/4096)-2.5)*12;
	if(adc_mean_cnt>100){
		adc_value/=100;
		sprintf(adc_value_txt,"adc value : %f A\r\n", adc_value);
		adc_mean_cnt=0;
		adc_value=0;
	}
	adc_mean_cnt++;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  *
  * Initialisation du microcontrôleur.
  * Gestion de la communication avec un shell sur l'UART2.
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	char	 	cmdBuffer[CMD_BUFFER_SIZE];
	int 		idx_cmd;
	char* 		argv[MAX_ARGS];
	int		 	argc = 0;
	char*		token;
	int 		newCmdReady = 0;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);


	memset(argv,NULL,MAX_ARGS*sizeof(char*));
	memset(cmdBuffer,NULL,CMD_BUFFER_SIZE*sizeof(char));
	memset(uartRxBuffer,NULL,UART_RX_BUFFER_SIZE*sizeof(char));
	memset(uartTxBuffer,NULL,UART_TX_BUFFER_SIZE*sizeof(char));


	HAL_Delay(10);
	HAL_UART_Transmit(&huart2, started, sizeof(started), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart2, prompt, sizeof(prompt), HAL_MAX_DELAY);

	HAL_UART_Receive_IT(&huart2, uartRxBuffer, UART_RX_BUFFER_SIZE);

	uint16_t speed = 50*( (uint16_t) TIM_MAX/100);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,speed);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,TIM_MAX-speed);

	//CODE ADC
	HAL_TIM_Base_Start_IT(&htim2);

	//CODE ENCODEUR
	HAL_TIM_Encoder_Start(&htim3,  TIM_CHANNEL_ALL);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
		// uartRxReceived is set to 1 when a new character is received on uart 1
		if(uartRxReceived){
			switch(uartRxBuffer[0]){
			// Nouvelle ligne, instruction à traiter
			case ASCII_CR:
				HAL_UART_Transmit(&huart2, newline, sizeof(newline), HAL_MAX_DELAY);
				cmdBuffer[idx_cmd] = '\0';
				argc = 0;
				token = strtok(cmdBuffer, " ");
				while(token!=NULL){
					argv[argc++] = token;
					token = strtok(NULL, " ");
				}

				idx_cmd = 0;
				newCmdReady = 1;
				break;
				// Suppression du dernier caractère
			case ASCII_DEL:
				cmdBuffer[idx_cmd--] = '\0';
				HAL_UART_Transmit(&huart2, uartRxBuffer, UART_RX_BUFFER_SIZE, HAL_MAX_DELAY);
				break;
				// Nouveau caractère
			default:
				cmdBuffer[idx_cmd++] = uartRxBuffer[0];
				HAL_UART_Transmit(&huart2, uartRxBuffer, UART_RX_BUFFER_SIZE, HAL_MAX_DELAY);
			}
			uartRxReceived = 0;
		}

		if(newCmdReady){
			handle_command(argv);
			newCmdReady = 0;
		}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV3;
  RCC_OscInitStruct.PLL.PLLN = 25;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/**
 * @brief Fonction de gestion des interruptions lors de la réception de l'UART.
 * @param UART_HandleTypeDef * huart
 * @return None
 *
 * Passage de la variable uartRxReceived à 1 et lancement d'une nouvelle acquisition sur interruption.
 */
void HAL_UART_RxCpltCallback (UART_HandleTypeDef * huart){
	uartRxReceived = 1;
	HAL_UART_Receive_IT(&huart2, uartRxBuffer, UART_RX_BUFFER_SIZE);
}

/**
 * @brief Fonction de gestion des interruptions lors de l'appui sur le bouton bleu de la carte STM32.
 * @param uint16_t GPIO_Pin
 * @return None
 *
 * Envoi d'un créneau d'une microseconde sur la pin PC3 pour démarrage du hacheur.
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	/* Prevent unused argument(s) compilation warning */
	if(GPIO_Pin == GPIO_PIN_13){
		//ButtonState = 1-ButtonState;
		//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, ButtonState);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
		HAL_Delay(1);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);

}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
