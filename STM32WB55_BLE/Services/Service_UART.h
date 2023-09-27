/*! @file Service_UART.h
 * @brief UART service interface
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
 #ifndef __SERVICE_UART_H__
#define  __SERVICE_UART_H__

#include <stdint.h>
#include "ble_types.h"
//#include "common_blesvc.h"

//! Macro to copy 128 bits UUIDs
#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, 		\
					  uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, 	\
					  uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) 		\
do {																		\
    	uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; 					\
		uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; 					\
        uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; 					\
		uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; 					\
        uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; 					\
		uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; 				\
        uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; 				\
		uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; 				\
}while(0)

//! Definition of service UUID
#define COPY_SERVICE_UART_UUID(uuid_struct)            COPY_UUID_128(		\
														uuid_struct,0xAA,	\
														0x00,0x00,0x00,0x00,\
														0x00,0x00,0x00,0x00,\
														0x00,0x00,0x00,0x00,\
														0x00,0x00,0x00)

//! Definition of TX characteristic UUID
#define COPY_SERVICE_UART_UART_TX_UUID(uuid_struct)    COPY_UUID_128(		\
														uuid_struct,0xAA,	\
														0x01,0x00,0x00,0x00,\
														0x00,0x00,0x00,0x00,\
														0x00,0x00,0x00,0x00,\
														0x00,0x00,0x00)

//! Definition of RX characteristic UUID
#define COPY_SERVICE_UART_UART_RX_UUID(uuid_struct)    COPY_UUID_128(		\
														uuid_struct,0xAA,	\
														0x02,0x00,0x00,0x00,\
														0x00,0x00,0x00,0x00,\
														0x00,0x00,0x00,0x00,\
														0x00,0x00,0x00)

//! Attribute max number, 2 for characteristic + 1
#define SERVICE_UART_MAX_ATRIBUTOS 								6
//! TX data characteristic max length
#define SERVICE_UART_UART_TX_LONG_MAX							64
//! RX data characteristic max length
#define SERVICE_UART_UART_RX_LONG_MAX							64
//! Offset to characteristic with 1 attribute
#define SERVICE_UART_OFFSET_1_ATTRIBUTE							1

//! Encripted key size
#define ENCRIPTED_KEY_SIZE										10

/*! @enum eServiceUARTCharacteristic
 * @brief Characteristics UART service enumeration
 * @typedef tServiceUARTCharacteristic
 *  @brief  Data type associated to the enumeration eServiceUARTCharacteristic
 */
typedef enum eServiceUARTCharacteristic{
	//! UART TX characteristic
	SERVICE_UART_UART_TX,
	//! UART_RX characteristic
	SERVICE_UART_UART_RX,
	//! Max number of characteristics
	SERVICE_UART_MAX_NUMBER_OF_CHARACTERISTICS
}tServiceUARTCharacteristic;

/*! @struct sServiceUARTContext
 *  @brief List of service characteristics handlers
 *  @typedef tServiceUARTContext
 *  @brief  Data type associated to the structure sServicioUARTContexto
 */
typedef struct sServiceUARTContext
{
	//! Service UART handle
	uint16_t  ServiceUARTHandle;
	//! Service TX characteristic handle
	uint16_t  ServiceUartTxHandle;
	//! Service TX characteristic handle
	uint16_t  ServiceUartRxHandle;
}tServiceUARTContext;




/*! Function to initialize UART service and characteristics
 *
 * @brief  UART service initialization
 *
 */
void Service_UART_Init( void );

/*! Function to update the characteristic value
 * @brief  	   Check if the characteristic is valid, and update BLE stack.
 * @param[in]  characteristic characteristic to update
 * @param[in]  pData Pointer to buffer with the value to update
 * @param[in]  nData Data length in bytes
 * @return 	   value of type tBleStatus, BLE_STATUS_SUCCESS if update
 * 		       successfully. Reference ble_def.h
 */
tBleStatus Service_UART_Update_Characteristic(								\
										uint16_t characteristic, 			\
										const uint8_t *pData, 				\
										uint8_t nData );
/*! Function to test UART service
 * @brief  	   Echo application read the received data and send the same 
 * 			   information. It gets data from the received packet, and packing it 
 *			   to send in TX characteristic. So test characteristics and interpreter. 
 * @param[in]  pData Pointer to buffer with the value to update
 * @param[in]  nData Data length in bytes
 */
void Service_UART_Test_Echo(const uint8_t *pData, uint8_t nData );

#endif /* __SERVICE_UART_H__ */
