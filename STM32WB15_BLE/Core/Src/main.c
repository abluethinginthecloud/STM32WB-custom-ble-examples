/*! @file main.c
 * @brief This file is the main implementation of BLE example
 *
 *
 * @author A BLUE THING IN THE CLOUD S.L.U.
 *    ===  When the technology becomes art ===
 *
 * http://abluethinginthecloud.com
 * j.longares@abluethinginthecloud
 *
 * (c) A BLUE THING IN THE CLOUD S.L.U.
 *
 *
 *
        ██████████████    ██    ██    ██  ██████    ██████████████
        ██          ██      ████████████████  ██    ██          ██
        ██  ██████  ██  ██████  ██    ██        ██  ██  ██████  ██
        ██  ██████  ██    ██████    ██      ██      ██  ██████  ██
        ██  ██████  ██      ██      ████  ██████    ██  ██████  ██
        ██          ██    ██      ██████    ████    ██          ██
        ██████████████  ██  ██  ██  ██  ██  ██  ██  ██████████████
                        ██████  ████  ██████  ████
        ██████  ██████████  ████    ████████      ████      ██
        ██  ████  ██    ██  ████        ████    ████████  ██    ██
            ██  ██  ████  ██      ██      ██      ██  ████  ██████
        ████  ████    ██      ██          ████  ██  ██        ██
            ██████████          ██      ██    ██  ████    ██  ████
          ██  ████    ██      ██████    ██  ██████████    ██    ██
        ██  ████  ████████████████  ██    ██        ████████  ████
                ████        ██  ██████  ██████████      ████  ██
        ██████  ████████████████    ████  ██    ██████    ██  ████
            ████████  ██████  ██    ██████      ██        ████  ██
        ██    ██  ████████  ██    ██        ██    ██          ████
          ████  ████          ██      ████████████  ██  ████  ██
        ██  ██████  ████  ██    ██      ████    ██████████
                        ██    ██████    ██      ██      ██  ██████
        ██████████████  ██  ██████  ██  ████  ████  ██  ████  ████
        ██          ██  ██      ████████  ██    ██      ████  ████
        ██  ██████  ██  ████  ██    ██████      ██████████    ████
        ██  ██████  ██    ██████    ██  ██  ████      ████  ██████
        ██  ██████  ██  ████      ██    ████  ██        ████    ██
        ██          ██  ██    ██      ██████████████  ██      ██
        ██████████████  ██████  ██        ██  ████    ██████  ████



*/

#include "main.h"

/*! IPCC handle definition by default.
 *	It is used in stm32wbxx_it.c as an extern variable.
 */
IPCC_HandleTypeDef hipcc;

//! RNG handle definition by default.
RNG_HandleTypeDef hrng;

/*! IPCC handle definition by default.
 *	It is used in app_entry.c, standby_stm32wb15.c, stm32_lpm_if.c and
 *	stm32_lpm_it.c as an extern variable.
 */
RTC_HandleTypeDef hrtc;

//! Function to configure default clocks used by system
void SystemClock_Config(void);
//! Function to configure default clock used by peripherals
void PeriphCommonClock_Config(void);

//! Statuc function used for initialize RNG
static void Random_Number_Generator_Init(void);

/*!  @brief  The application entry point.
 */
int main(void) {

	/* Reset of all peripherals, Initializes the Flash interface and the
	 *  Systick.
	 */
	HAL_Init();

	// BLE Default config. Must be called before SystemClock_Config()
	MX_APPE_Config();

	SystemClock_Config();
	PeriphCommonClock_Config();
	Interprocessor_Communication_Controller_Init();
	GPIO_Init();
	RTC_Init();
	Random_Number_Generator_Init();

	// Appication BLE init
	MX_APPE_Init();

	while (1)
	{
		MX_APPE_Process();

	}
}

/*!  Function to configure default clocks used by system
 * @brief System Clock Configuration by default
 *
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	// Configure LSE Drive Capability
	HAL_PWR_EnableBkUpAccess();
	__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

	/* Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI
			| RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/* Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
	 */
	RCC_ClkInitStruct.ClockType = 	RCC_CLOCKTYPE_HCLK4  | 					\
								 	RCC_CLOCKTYPE_HCLK2  | 					\
									RCC_CLOCKTYPE_HCLK   | 					\
									RCC_CLOCKTYPE_SYSCLK |					\
									RCC_CLOCKTYPE_PCLK1	 |					\
									RCC_CLOCKTYPE_PCLK2;

	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.AHBCLK2Divider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.AHBCLK4Divider = RCC_SYSCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
		Error_Handler();
	}
}

/*! Function to configure default clock used by peripherals
 * @brief Peripherals Common Clock Configuration by default
 *
 */
void PeriphCommonClock_Config(void) {
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };

	// Initializes the peripherals clock
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SMPS |			\
											   RCC_PERIPHCLK_RFWAKEUP;
	PeriphClkInitStruct.RFWakeUpClockSelection = RCC_RFWKPCLKSOURCE_LSE;
	PeriphClkInitStruct.SmpsClockSelection = RCC_SMPSCLKSOURCE_HSE;
	PeriphClkInitStruct.SmpsDivSelection = RCC_SMPSCLKDIV_RANGE1;

	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
		Error_Handler();
	}
}

/*!  Function that initializes the IPCC peripheral and creates the associated handle
 * @brief Inter-processor communication controller (IPCC)
 */
void Interprocessor_Communication_Controller_Init(void) {

	hipcc.Instance = IPCC;
	if (HAL_IPCC_Init(&hipcc) != HAL_OK) {
		Error_Handler();
	}
}

/*! Function that initializes the RNG peripheral and creates the associated handle
 * @brief Random number generator (RNG)
*/
static void Random_Number_Generator_Init(void) {

	hrng.Instance = RNG;
	hrng.Init.ClockErrorDetection = RNG_CED_ENABLE;
	if (HAL_RNG_Init(&hrng) != HAL_OK) {
		Error_Handler();
	}
}

/*! Function that initializes the RTC peripheral and creates the associated handle
 * @brief Real time clock (RTC)
 *         Use the configuration by default
 */
void RTC_Init(void) {

	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv = CFG_RTC_ASYNCH_PRESCALER;
	hrtc.Init.SynchPrediv = CFG_RTC_SYNCH_PRESCALER;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
	if (HAL_RTC_Init(&hrtc) != HAL_OK) {
		Error_Handler();
	}
	// Enable the WakeUp
	if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 0, RTC_WAKEUPCLOCK_RTCCLK_DIV16)
			!= HAL_OK) {
		Error_Handler();
	}
	// Disable RTC registers write protection
	LL_RTC_DisableWriteProtection(RTC);

	LL_RTC_WAKEUP_SetClock(RTC, CFG_RTC_WUCKSEL_DIVIDER);

	// Enable RTC registers write protection
	LL_RTC_EnableWriteProtection(RTC);

}

/*! Function that enables GPIO PORT C clock needed by system
 *  @brief GPIO Initialization Function by default
 */
void GPIO_Init(void) {

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();

}

/*!  @brief  This function is executed in case of error occurrence.
*/
void Error_Handler(void)
{
	// Error should be catch here

}
