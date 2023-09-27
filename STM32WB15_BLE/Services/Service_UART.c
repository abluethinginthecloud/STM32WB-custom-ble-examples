/*! @file Service_UART.c
 * @brief Implementation of UART service, with initialization and update functions.
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

#include "common_blesvc.h"
#include "Service_UART.h"

/**
 * START of Section BLE_DRIVER_CONTEXT
 */

PLACE_IN_SECTION("BLE_DRIVER_CONTEXT") static tServiceUARTContext SUARTContext;

/**
 * END of Section BLE_DRIVER_CONTEXT
 */

/*! Function to manage UART service events
 * @brief  Events management function handler
 * @param[in] event Information event buffer
 * @return Return SVCCTL_EvtNotAck, if no ACK received
 * 				  SVCCTL_EvtAckFlowEnable, if flow enabled
 * 				  SVCCTL_EvtAckFlowDisable, if flow disable
 */
static SVCCTL_EvtAckStatus_t Service_UART_Handler_Event(void *event){

	SVCCTL_EvtAckStatus_t return_value;
	hci_event_pckt *event_pckt;
	evt_blecore_aci *blecore_evt;
	aci_gatt_attribute_modified_event_rp0 *attribute_modified;

	return_value = SVCCTL_EvtNotAck;
	event_pckt = (hci_event_pckt *)(((hci_uart_pckt*)event)->data);

	blecore_evt = (evt_blecore_aci*)event_pckt->data;

	attribute_modified =													\
			(aci_gatt_attribute_modified_event_rp0*)blecore_evt->data;

	if(attribute_modified->Attr_Handle == 									\
	  (SUARTContext.ServiceUartRxHandle + SERVICE_UART_OFFSET_1_ATTRIBUTE)){

		return_value = SVCCTL_EvtAckFlowEnable;

		// TX interruption management
		Service_UART_Update_Characteristic(									\
										SERVICE_UART_UART_RX, 				\
										attribute_modified->Attr_Data,		\
										attribute_modified->Attr_Data_Length);

		//Echo test
		Service_UART_Test_Echo(												\
										attribute_modified->Attr_Data,		\
										attribute_modified->Attr_Data_Length);
	}

	return(return_value);
}


/*! Function to initialize UART service and characteristics
 *
 * @brief  UART service initialization
 *
 */
void Service_UART_Init( void ){
	
	Char_UUID_t  uuid;
	tBleStatus result;

 	SVCCTL_RegisterSvcHandler(Service_UART_Handler_Event);

	memset ( &SUARTContext, 0, sizeof(tServiceUARTContext) );

	// Add UART service
	COPY_SERVICE_UART_UUID(uuid.Char_UUID_128);
	result = aci_gatt_add_service(											\
										UUID_TYPE_128,						\
								   	   	(Service_UUID_t *) &uuid,			\
										PRIMARY_SERVICE,					\
										SERVICE_UART_MAX_ATRIBUTOS,			\
										&(SUARTContext.ServiceUARTHandle));
	if (result == BLE_STATUS_SUCCESS)
    {
		BLE_DBG_DIS_MSG ("UART Service is added Successfully %04X\n",
                   	   	  SUARTContext.ServiceUARTHandle);
    }
    else
    {
		BLE_DBG_DIS_MSG ("FAILED to add UART service, Error: %02X !!\n",
    				     resultado);
    }

	// Add RX characteristic
	COPY_SERVICE_UART_UART_RX_UUID(uuid.Char_UUID_128);
    result = aci_gatt_add_char(												\
    									SUARTContext.ServiceUARTHandle,		\
										UUID_TYPE_128,						\
										&uuid ,								\
										SERVICE_UART_UART_RX_LONG_MAX,		\
										CHAR_PROP_WRITE,					\
										ATTR_PERMISSION_NONE,				\
										GATT_NOTIFY_ATTRIBUTE_WRITE,		\
										ENCRIPTED_KEY_SIZE, 				\
										CHAR_VALUE_LEN_VARIABLE,			\
										&(SUARTContext.ServiceUartRxHandle));

    if (result == BLE_STATUS_SUCCESS)
    {
      BLE_DBG_DIS_MSG ("Receive characteristic added successfully %04X \n",
    		  	  	   SUARTContexto.fechaHoraHandle);
    }
    else
    {
      BLE_DBG_DIS_MSG ("FAILED to add receive characteristic, Error: %02X !!\n",
    		  	  	  result);
    }

	// Add TX characteristic
    COPY_SERVICE_UART_UART_TX_UUID(uuid.Char_UUID_128);
    result = aci_gatt_add_char(												\
    									SUARTContext.ServiceUARTHandle,		\
										UUID_TYPE_128,						\
										&uuid ,								\
										SERVICE_UART_UART_TX_LONG_MAX,		\
										CHAR_PROP_READ | CHAR_PROP_NOTIFY,	\
										ATTR_PERMISSION_NONE,				\
										GATT_DONT_NOTIFY_EVENTS,			\
										ENCRIPTED_KEY_SIZE, 				\
										CHAR_VALUE_LEN_VARIABLE,			\
										&(SUARTContext.ServiceUartTxHandle));

    if (result == BLE_STATUS_SUCCESS)
    {
      BLE_DBG_DIS_MSG ("Transmission characteristic added successfully %04X \n",
    		  	  	   SUARTContexto.fechaHoraHandle);
    }
    else
    {
      BLE_DBG_DIS_MSG ("FAILED to add transmission characteristic, Error: %02X !!\n",
    		  	  	  resultado);
    }
}


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
										uint8_t nData ){
	
	tBleStatus BLEStatus = 0;

	if(characteristic >= SERVICE_UART_MAX_NUMBER_OF_CHARACTERISTICS)
	{
		return BLE_STATUS_ERROR;
	}
  
	switch(characteristic){
		case SERVICE_UART_UART_TX:
			if( nData < SERVICE_UART_UART_TX_LONG_MAX ){

				BLEStatus = aci_gatt_update_char_value(
													SUARTContext.ServiceUARTHandle,  	\
													SUARTContext.ServiceUartTxHandle,  	\
													0, 								 	\
													nData,    						 	\
													(uint8_t *) pData);
				}

		  break;
		case SERVICE_UART_UART_RX:
			if( nData < SERVICE_UART_UART_RX_LONG_MAX ){
				BLEStatus = aci_gatt_update_char_value(
													SUARTContext.ServiceUARTHandle,  	\
													SUARTContext.ServiceUartRxHandle, 	\
													0, 								 	\
													nData,    						 	\
													(uint8_t *) pData);
			}

		  break;
		default:
		  break;
		}
		
	return BLEStatus;
}

/*! Function to test UART service
 * @brief  	   Echo application read the received data and send the same 
 * 			   information.
 * @param[in]  pData Pointer to buffer with the value to update
 * @param[in]  nData Data length in bytes
 */
void Service_UART_Test_Echo(const uint8_t *pData, uint8_t nData )
{
	if( NULL != pData && nData < SERVICE_UART_UART_TX_LONG_MAX )
	{

		//Send data
		Service_UART_Update_Characteristic(								\
												SERVICE_UART_UART_TX, 	\
												pData,					\
												nData);
	}
}

