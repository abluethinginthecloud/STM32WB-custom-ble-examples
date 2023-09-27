# README #

BLE echo test. 
Version 1.0

###  Set up with versions  ###

* Development environment:  STM32CubeIDE
* Development environment version: 1.9.0
* STM32CubeWB Firmware Package version: 1.13.2

This project has been tested with an STMicroelectronics STM32WB15CC-Nucleo
    board, specifically STM32WB15CCU6 development kit, with bluetooth 5.2. 


### Project explain ###
To use this project it is neccessary a board with STM32WB15 nucleo, 
and a device with BLE connection. 

In order to make the program work, you must do the following:
 - Open your toolchain 
 - Rebuild all files and flash the board with the executable file

This project has a BLE connection with capatibility of connecting/distoconnecting to any device, 
with UART service and data service includes. 

UART SERVICE INFORMATION: 
All definitions and functions neccesaries to the UART service are in Service_UART.c and Service_UART.h.
UUIDs are defined in Service_UART.h, and can be changed:
    Service UUID is defined in COPY_SERVICE_UART_UUID 
    TX characteristic UUID is defined in COPY_SERVICE_UART_UART_TX_UUID
    RX characteristic UUID is defined in COPY_SERVICE_UART_UART_RX_UUID
Characteristics max length are defined in Service_UART.h, and can be changed:
Note: This values should not exceed 248 bytes. Right now they are set to 64 bytes. 
    RX characteristic: SERVICE_UART_UART_RX_LONG_MAX
    TX characteristic: SERVICE_UART_UART_TX_LONG_MAX    

UART service has 2 characteristics, receive (RX) (write permission) and transmit (TX) 
(read and notification permission). 
TX characteristic is the one in charge of transmitting the information to the user, 
so it has both read and notify permission. If the user is registered for notifications 
data will be sent each time the characteristic value is updated. If the user is not 
registered for notifications, value should be read in order to get the updated data. 
RX characteristic is the one in charge of receiving the data sent by the user, thus 
it has write permissions. 

In order to use the data received from BLE inside the application, some user code should be added 
in Service_Uart.c, line 88. right now, an echo is implemented: 

		// Only for testing, is added an echo app
void Service_UART_Test_Echo(const uint8_t *pData, uint8_t nData ); 

Device advertising name is a configurable parameter in Configuration/Device_Configuration.h

@par Keywords

Connectivity, BLE, BLE protocol, BLE pairing, BLE profile


### Test ###

To test UART service, echo application has been implemented, to use 
it you must do the following: 
- Run the project
- Connect BLE
- Register for TX notifications to receive new data automatically as it is updated. 
- Write in RX characteristic  value. E.g. : 20 21 22
- You will receive the same data in TX characteristic. E.g.: Same 20 21 22 


### Author information ###

 @author A BLUE THING IN THE CLOUD
 == When The Technology Becomes Art ==
 www.abluethinginthecloud.com
 email: j.longares@abluethinginthecloud.com