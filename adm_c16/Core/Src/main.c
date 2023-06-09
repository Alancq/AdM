/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "asm_func.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

ETH_TxPacketConfig TxConfig;
ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */

ETH_HandleTypeDef heth;

UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ETH_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/*EJERCICIO I
 *Realizar una función que inicialice un vector con ceros. La función debe tener el siguiente
prototipo:

void zeros (uint32_t * vector, uint32_t longitud);*/
void zeros(uint32_t *vector, uint32_t longitud) {
    for (uint32_t i = 0; i < longitud; i++) {
        vector[i] = 0;
    }
}

/*EJERCICIO II
 * 2) Realizar una función que realice el producto de un vector y un escalar (por ejemplo, podría servir
para cambiar el nivel de amplitud de una señal). void productoEscalar32 (uint32_t * vectorIn, uint32_t * vectorOut uint32_t longitud, uint32_t escalar);*/

void productoEscalar32(uint32_t *vectorIn, uint32_t *vectorOut, uint32_t longitud, uint32_t escalar) {
    for (uint32_t i = 0; i < longitud; i++) {
        vectorOut[i] = vectorIn[i] * escalar;
    }
}
/*EJERCICIO 3
 * Adapte la función del ejercicio 2 para realizar operaciones sobre vectores de 16 bits:
void productoEscalar16 (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint16_t escalar);*/
void productoEscalar16(uint16_t *vectorIn, uint16_t *vectorOut, uint32_t longitud, uint16_t escalar) {
    for (uint32_t i = 0; i < longitud; i++) {
        vectorOut[i] = vectorIn[i] * escalar;
    }
}
/*EJERCICIO 4
 *4) Adapte la función del ejercicio 3 para saturar el resultado del producto a 12 bits:
void productoEscalar12 (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint16_t escalar); */
void productoEscalar12(uint16_t *vectorIn, uint16_t *vectorOut, uint32_t longitud, uint16_t escalar) {
	uint32_t producto;
    for (uint32_t i = 0; i < longitud; i++) {
        producto = vectorIn[i] * escalar;
        if (producto > 4095) {
            vectorOut[i] = 4095;
        } else {
            vectorOut[i] = producto;
        }
    }
}

/*EJERCICIO 5
 * Realice una función que implemente un filtro de ventana móvil de 10 valores sobre un vector de
muestras.

void filtroVentana10(uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitudVectorIn);*/

void filtroVentana10(uint16_t *vectorIn, uint16_t *vectorOut, uint32_t longitudVectorIn) {

	uint32_t i;
	int32_t j,aux;
    for ( i = 0; i < longitudVectorIn; i++) {
    	uint32_t sum = 0;
         j= i-5;
         aux=i;

        // Calcular el promedio de los valores de la ventana
        while(j<=aux+5) {
            if (j >= 0 && j < longitudVectorIn) {
                sum += vectorIn[j];
            }
            j++;
        }

        // Asignar el promedio al vector de salida
        vectorOut[i] = sum /11;
    }
}
/*EJERCICIO 6
 * 6) Realizar una función que reciba un vector de números signados de 32 bits y los “empaquete” en
otro vector de 16 bits. La función deberá adecuar los valores de entrada a la nueva precisión.

void pack32to16 (int32_t * vectorIn, int16_t *vectorOut, uint32_t longitud);*/
void pack32to16(int32_t *vectorIn, int16_t *vectorOut, uint32_t longitud) {
	for (uint32_t i = 0; i < longitud; i++) {
		vectorOut[i] = (vectorIn[i] >> 16);
	}
}
/*EJERCICIO 7
 * 7) Realizar una función que reciba un vector de números signados de 32 bits y devuelva la posición
del máximo del vector.

int32_t max (int32_t * vectorIn, uint32_t longitud);*/

int32_t max(int32_t *vectorIn, uint32_t longitud) {
	int32_t maxVal = vectorIn[0];
	uint32_t maxPos = 0;

	for (uint32_t i = 1; i < longitud; i++) {
		if (vectorIn[i] > maxVal) {
			maxVal = vectorIn[i];
			maxPos = i;
		}
	}

	return maxPos;
}

/*EJERCICIO 8
 * 8) Realizar una función que reciba un vector de muestras signadas de 32 bits y lo decime
descartando una cada N muestras.

void downsampleM (int32_t * vectorIn, int32_t * vectorOut, uint32_t longitud, uint32_t N);*/


void downsampleM(int32_t *vectorIn, int32_t *vectorOut, uint32_t longitud, uint32_t N) {
    uint32_t s = 0;
    for (uint32_t i = N-1; i < longitud; i+=N) {
        vectorOut[s] = vectorIn[i];
        s++;
    }

}

/*EJERCICIO 9
 * 9) Realizar una función que reciba un vector de muestras no signadas de 16 bits e invierta su orden.

void invertir (uint16_t * vector, uint32_t longitud);*/
void invertir(uint16_t *vector, uint32_t longitud) {
    uint32_t inicio = 0;
    uint32_t fin = longitud - 1;

    while (inicio < fin) {
        uint16_t temp = vector[inicio];
        vector[inicio] = vector[fin];
        vector[fin] = temp;

        inicio++;
        fin--;
    }
}
/*EJERCICIO 11
 * 11) La correlación cruzada (del inglés cross correlation) es una operación usada para estudiar el
grado de similitud de dos señales y su fase relativa, aún en presencia de ruido.*/
void corr(int16_t *vectorX, int16_t *vectorY, int16_t *vectorCorr, uint32_t longitud) {
    for (uint32_t l = 0; l < longitud; l++) {
        int32_t sum = 0;
        for (uint32_t n = l; n < longitud; n++) {
            sum += vectorX[n] * vectorY[n - l];
        }
        vectorCorr[l] =sum;
    }
}
/*SIMD*/


void corrS(int16_t *vectorX, int16_t *vectorY, int16_t *vectorCorr, uint32_t longitud) {
    for (uint32_t l = 0; l < longitud; l++) {
        int32_t simdSum = 0;

        for (uint32_t n = 0; n < longitud - 1; n += 2) {
            if (l <= n) {
                uint32_t j = n - l;

                int32_t simdX = __PKHBT(vectorX[n], vectorX[n + 1], 16);
                int32_t simdY = __PKHBT(vectorY[j], vectorY[j + 1], 16);
                simdSum += __SMUAD(simdX, simdY);
            }
        }

        if (longitud % 2 != 0) {
            simdSum += (int32_t)vectorX[longitud - 1] * (int32_t)vectorY[longitud - 1 - l];
        }

        vectorCorr[l] = (int16_t)(simdSum);
    }
}

/* USER CODE BEGIN 0 */
static void PrivilegiosSVC (void)
{
    // Obtiene valor del registro de 32 bits del procesador llamado "control".
    // El registro guarda los siguientes estados:
    // bit 2: Uso de FPU en el contexto actual. Usado=1, no usado=0.
    // bit 1: Mapeo del stack pointer(sp). MSP=0, PSP=1.
    // bit 0: Modo de ejecucion en Thread. Privilegiado=0, No privilegiado=1.
    //        Recordar que este valor solo se usa en modo Thread. Las
    //        interrupciones siempre se ejecutan en modo Handler con total
    //        privilegio.
    uint32_t x = __get_CONTROL ();

    // Actividad de debug: Ver registro "control" y valor de variable "x".
    //__BKPT (0);

    x |= 1;
    // bit 0 a modo No privilegiado.
    __set_CONTROL (x);

    // En este punto se estaria ejecutando en modo No privilegiado.
    // Lectura del registro "control" para confirmar.
    x = __get_CONTROL ();

    // Actividad de debug: Ver registro "control" y valor de variable "x".
    //__BKPT (0);

    x &= ~1u;
    // Se intenta volver a modo Privilegiado (bit 0, valor 0).
    __set_CONTROL (x);

    // Confirma que esta operacion es ignorada por estar ejecutandose en modo
    // Thread no privilegiado.
    x = __get_CONTROL ();

    // Actividad de debug: Ver registro "control" y valor de variable "x".
    //__BKPT (0);

    // En este punto, ejecutando en modo Thread no privilegiado, la unica forma
    // de volver a modo privilegiado o de realizar cualquier cambio que requiera
    // modo privilegiado, es pidiendo ese servicio a un hipotetico sistema
    // opertivo de tiempo real.
    // Para esto se invoca por software a la interrupcion SVC (Supervisor Call)
    // utilizando la instruccion "svc".
    // No hay intrinsics para realizar esta tarea. Para utilizar la instruccion
    // es necesario implementar una funcion en assembler. Ver el archivo
    // asm_func.S.
    asm_svc ();

    // El sistema operativo (el handler de SVC) deberia haber devuelto el modo
    // de ejecucion de Thread a privilegiado (bit 0 en valor 0).
    x = __get_CONTROL ();

    // Fin del ejemplo de SVC
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_ETH_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  /* USER CODE BEGIN 2 */
  PrivilegiosSVC ();
  // Activa contador de ciclos (iniciar una sola vez)
  DWT->CTRL |= 1 << DWT_CTRL_CYCCNTENA_Pos;
  uint32_t Ciclos,Ciclos2;
  const uint32_t Resultado = asm_sum (5, 3);
  //int32_t Vector[11]={0,1,2,3,4,5,6,7,8,9,10};
  //int32_t Vector_p2[11]={0,0,0,0,0,0,0,0,0,0};

  //zeros(Vector,10);
  //asm_zeros(Vector,10);

  //2
  //productoEscalar32(Vector,Vector_p2,10,100);
  //asm_oductoEscalar32(Vector,Vector_p2,10,100);
  //3
  //productoEscalar16(Vector,Vector_p2,10,100);
  //asm_productoEscalar16(Vector,Vector_p2,10,100);
  //4
  //productoEscalar12(Vector,Vector_p2,10,100);
  //asm_productoEscalar12(Vector,Vector_p2,10,100);
  //5
  //uint16_t Vector[10]={1,1,1,1,1,1,1,1,1,1};
  //uint16_t Vector_p2[10]={1,2,3,4,5,6,7,8,9,10};
  //filtroVentana10(Vector,Vector_p2,10);
  //6
  //int32_t Vector[10]={-10,5161,-4545,212,1856,-9898,-1234,65,-942,100};
  //int16_t Vector_p2[10];
  //pack32to16(Vector,Vector_p2,10);
  //asm_pack32to16(Vector,Vector_p2,10);
  //7
  //int32_t Vector[10]={1,200,3,4000,5,6,7,8,9,10};
  //max(Vector,sizeof(Vector))/sizeof(Vector[0]);
  //uint32_t maxPos = max(Vector,sizeof(Vector)/sizeof(Vector[0]));
  //uint32_t maxPos = asm_max(Vector,sizeof(Vector)/sizeof(Vector[0]));

  //8
  //int32_t Vector[11]={0,1,2,3,4,5,6,7,8,9,10};
  //int32_t Vector_p2[11]={0,0,0,0,0,0,0,0,0,0};
  //downsampleM(Vector,Vector_p2,11,3);
  //asm_downsampleM(Vector,Vector_p2,11,3);
  //9

  //uint16_t Vector[5]={1,2,3,4,5};
  //uint16_t Vector[5]={5,2,3,1,5};
  //invertir(Vector,sizeof(Vector)/sizeof(Vector[0]));
  //DWT->CYCCNT = 0;
  //asm_invertir(Vector,sizeof(Vector)/sizeof(Vector[0]));
  //Ciclos = DWT->CYCCNT;
//11
  int16_t Vector[11]={0,1,2,3,4,5,6,7,8,9,10};
  int16_t Vector_p2[11]={0,1,2,3,4,5,6,7,8,9,10};
  int16_t Vector_corr[11];
  DWT->CYCCNT = 0;
  corr(Vector, Vector_p2, Vector_corr, 11);
  Ciclos = DWT->CYCCNT;
  int16_t Vector1[11]={0,1,2,3,4,5,6,7,8,9,10};
  int16_t Vector1_p2[11]={0,1,2,3,4,5,6,7,8,9,10};
  int16_t Vector1_corr[11];
  DWT->CYCCNT = 0;
  asm_corr(Vector1, Vector1_p2, Vector1_corr, 11);
  Ciclos2 = DWT->CYCCNT;
  int16_t Vector2[11]={0,1,2,3,4,5,6,7,8,9,10};
  int16_t Vector2_p2[11]={0,1,2,3,4,5,6,7,8,9,10};
  int16_t Vector2_corr[11];
  DWT->CYCCNT = 0;
  corrS(Vector2, Vector2_p2, Vector2_corr, 11);
  uint32_t Ciclos3 = DWT->CYCCNT;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ETH Initialization Function
  * @param None
  * @retval None
  */
static void MX_ETH_Init(void)
{

  /* USER CODE BEGIN ETH_Init 0 */

  /* USER CODE END ETH_Init 0 */

   static uint8_t MACAddr[6];

  /* USER CODE BEGIN ETH_Init 1 */

  /* USER CODE END ETH_Init 1 */
  heth.Instance = ETH;
  MACAddr[0] = 0x00;
  MACAddr[1] = 0x80;
  MACAddr[2] = 0xE1;
  MACAddr[3] = 0x00;
  MACAddr[4] = 0x00;
  MACAddr[5] = 0x00;
  heth.Init.MACAddr = &MACAddr[0];
  heth.Init.MediaInterface = HAL_ETH_RMII_MODE;
  heth.Init.TxDesc = DMATxDscrTab;
  heth.Init.RxDesc = DMARxDscrTab;
  heth.Init.RxBuffLen = 1524;

  /* USER CODE BEGIN MACADDRESS */

  /* USER CODE END MACADDRESS */

  if (HAL_ETH_Init(&heth) != HAL_OK)
  {
    Error_Handler();
  }

  memset(&TxConfig, 0 , sizeof(ETH_TxPacketConfig));
  TxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
  TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
  TxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;
  /* USER CODE BEGIN ETH_Init 2 */

  /* USER CODE END ETH_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_FS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */

  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 4;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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
