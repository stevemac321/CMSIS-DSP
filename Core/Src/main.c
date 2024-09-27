/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "arm_math.h"
#include "arm_const_structs.h"  // Contains pre-defined FFT instances
#include "dsp/transform_functions.h"
#include "arm_common_tables.h"
#include "arm_mve_tables.h"
#include <stdio.h>
#include <math.h>
#include "pqueue.h"
#include "graph.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FFT_LENGTH  16
#define PI 3.14159265358979323846
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
ADC_HandleTypeDef hadc1;

/* USER CODE BEGIN PV */

float32_t FFT_Input_Q15_f[FFT_LENGTH * 2];
float32_t fftOutput[FFT_LENGTH];

struct priority_queue pq;
struct Graph graph;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_ADC1_Init(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

/* USER CODE BEGIN PFP */
static void test_fft();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void print_fft();
void print_mag();
void print_voltage_samples();
void analyze_results();
void print_graph(const struct Graph* graph);
void print_bfs_results(const struct Graph* graph);

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
  MX_ADC1_Init();
  MX_USART2_UART_Init();

  test_fft();
  analyze_results();
  /* USER CODE BEGIN 2 */

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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */
	__HAL_RCC_USART2_CLK_ENABLE();
  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);
}

/* USER CODE BEGIN MX_GPIO_Init_2 */

/* USER CODE BEGIN ADC1_Init 2 */
static void MX_ADC1_Init(void)
{

	/* USER CODE BEGIN ADC1_Init 0 */
	  __HAL_RCC_ADC1_CLK_ENABLE();

	/* USER CODE END ADC1_Init 0 */

	ADC_ChannelConfTypeDef sConfig = {0};

	/* USER CODE BEGIN ADC1_Init 1 */

	/* USER CODE END ADC1_Init 1 */

	/** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	*/
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ScanConvMode = DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
	  Error_Handler();
	}

	/** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	*/
	sConfig.Channel = ADC_CHANNEL_VREFINT;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
	  Error_Handler();
	}
}
/* USER CODE END ADC1_Init 2 */


/* USER CODE END MX_GPIO_Init_2 */



/* USER CODE BEGIN 4 */
static void test_fft()
{
	for (int i = 0; i < FFT_LENGTH * 2; i += 2) {
	        // Capture ADC data for real part
	        HAL_ADC_Start(&hadc1);
	        if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK) {
	            FFT_Input_Q15_f[i] = (float32_t)HAL_ADC_GetValue(&hadc1);  // Real part
	        }
	        HAL_ADC_Stop(&hadc1);

	        // Set imaginary part to 0
	        FFT_Input_Q15_f[i + 1] = 0.0f;  // Imaginary part
	    }
		print_voltage_samples();
		arm_cfft_instance_f32 S;
		arm_cfft_init_f32(&S, FFT_LENGTH);
	    arm_cfft_f32(&S, FFT_Input_Q15_f, 0, 1);  // Perform FFT (forward, bit-reversal enabled)
	    print_fft();
	    // You can now calculate magnitudes if needed
	    arm_cmplx_mag_f32(FFT_Input_Q15_f, fftOutput, FFT_LENGTH);
	    print_mag();
}
void print_voltage_samples()
{
	printf("\nVoltage Samples:\r\n");
	for(size_t i = 0; i < (FFT_LENGTH * 2); i += 2) {
		printf("Real: %.6f, Imag: %.6f\r\n", FFT_Input_Q15_f[i], FFT_Input_Q15_f[i+1] );
}

}
void print_fft()
{
	printf("\nFFT Results\r\n");
	for(size_t i = 0; i < (FFT_LENGTH * 2); i++) {
		printf("Real: %.6f, Imag: %.6f\r\n", FFT_Input_Q15_f[i], FFT_Input_Q15_f[i++] );
	}
}
void print_mag()
{
	printf("\nMag Results\r\n");
	for(size_t i = 0; i < (FFT_LENGTH); i++) {
		printf("Mag: %.6f\r\n", fftOutput[i] );
	}
}
void analyze_results()
{
	priority_queue_from_array(&pq, fftOutput, FFT_LENGTH);
	priority_queue_build_max_heap(&pq);
	priority_queue_print_heap(&pq);

	create_graph(&graph, FFT_LENGTH);
	for(int i=0; i < pq.heap_size; i++) {
			add_vertex(&graph, pq.heap[i]);
		}
		for (int i = 1; i < graph.graph_capacity - 1; i++) {
		float prev_val = graph.vertices[i - 1].magnitude;  // Assume `magnitude` is a field in each vertex
		float curr_val = graph.vertices[i].magnitude;
		float next_val = graph.vertices[i + 1].magnitude;

		// Check for peak
		if (curr_val > prev_val && curr_val > next_val) {
			// Add edge between current peak and its previous valley
			add_edge(&graph, i, i - 1);
			// Add edge between current peak and its next valley
			add_edge(&graph, i, i + 1);
		}
		// Check for valley
		else if (curr_val < prev_val && curr_val < next_val) {
			// Add edge between current valley and its previous peak
			add_edge(&graph, i, i - 1);
			// Add edge between current valley and its next peak
			add_edge(&graph, i, i + 1);
		}
	}
	print_graph(&graph);
}
void print_graph(const struct Graph* graph)
{
	printf("\nGraph edge connections: \r\n");
	for (size_t i = 0; i < graph->numVertices; i++) {
		printf("Vertex %d: ", i);

		// Print each bit of the adjacency list
		for (size_t j = 0; j < sizeof(uint16_t) * 8; j++) {  // 16 bits
			// Shift and mask to get the j-th bit (0 or 1)
			uint16_t edge = (graph->vertices[i].adjList >> j) & 1;
			printf("%d", edge);
		}
		printf("\r\n");
	}
}
int _write(int file, char *ptr, int len) {
    HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, HAL_MAX_DELAY);
    return len;
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
