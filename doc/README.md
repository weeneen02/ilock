# ILOCK


1. ### System Diagram
   
    * This is the Picture our system.
    ![beetle](/img/Blunobeetle.png)






1. ### SomeThing We should know.
    
    * println()     Prints data to the Serial ports as human-readable ASCII text
                    followed by a carriage return character (ASCII 13, or '\r')
                    and new line character (ASCII 10 or '\n').

    * Speed gap     The wireless programming can be done in bi-way between 
                    the CENTRAL and PERIPHERAL BLE devices, but the speed 
                    is different, i.e.
                    
                    CENTRAL -> PERIPHERAL 2KB/s
                    PERIPHERAL -> CENTRAL 4KB/s
                    
                    I am not sure but, is the reason why I(Central) got the massage
                    from your(Peripheral) vertically?
                    and your(Peripheral) got the massage horizontally?



    * AT Commands  



    1. "AT+FSM" change the working mode
    
        AT+FSM=FSM_TRANS_USB_COM_BLE<CR+LF>|USB-UART BLE transparent mode
        AT+FSM=FSM_HID_USB_COM_BLE_AT<CR+LF>|USB-UART BLE HID mode
        AT+FSM=?<CR+LF>|Request the working mode (default: FSM_TRANS_USB_COM_BLE)

    1. "AT+KEY" to simulate pressing some buttons in HID mode, read FAQ 16 below for How to use HID mode.

        AT+KEY=keyValue0<CR+LF>                 To simulate one button was pressed
        AT+KEY=keyValue0+keyValue1<CR+LF>       To simulate two buttons was pressed
        AT+KEY=keyValue0+keyValue1+keyValue2<CR+LF> To simulate three buttons was pressed

    1. "AT+ROLE" change the CENTRAL-PERIPHERAL configuration

        AT+ROLE=ROLE_CENTRAL<CR+LF>             BLE CENTRAL mode
        AT+ROLE=ROLE_PERIPHERAL<CR+LF>          BLE PERIPHERAL mode
        AT+ROLE=?<CR+LF>            Request the CENTRAL-PERIPHERAL configuration (default: ROLE_PERIPHERAL)

    1. "AT+MIN\_INTERVAL" change the minimum connection interval

        AT+MIN_INTERVAL=10<CR+LF>   Recommended minimum connection interval (10ms) for PC and Android
        AT+MIN_INTERVAL=20<CR+LF>   Recommended minimum connection interval (20ms) for IOS
        AT+MIN_INTERVAL=?<CR+LF>    Request the minimum connection interval (default: 10)

    1. "AT+MAX\_INTERVAL" change the maximum connection interval

        AT+MAX_INTERVAL=10<CR+LF>   Recommended maximum connection interval (10ms) for PC and Android
        AT+MAX_INTERVAL=40<CR+LF>   Recommended maximum connection interval (40ms) for IOS
        AT+MAX_INTERVAL=?<CR+LF>    Request the maximum connection interval (default: 10)

    1. "AT+UART" change the baud rate of UART

        AT+UART=115200<CR+LF>       Set the baud rate to 115200
        AT+UART=?<CR+LF>            Request the baud rate of UART (default: 115200,8,N,1)

    1. "AT+BIND" bind another BLE chip. BLE can only connect to the BLE chip with this MAC address

        AT+BIND=0x0017ea9397e1<CR+LF>   Set the BLE binding (destination) MAC address to 0x0017ea9397e1
        AT+BIND=?<CR+LF>    Request the binding (destination) MAC address (default: 0x8A6D3B8A6D3B)

    1. "AT+CMODE" set whether the connection of BLE is binding or arbitrary

        AT+CMODE=UNIQUE<CR+LF>  BLE can only connect to the BLE chip with binding(destination) MAC address (see "AT+BIND" command)
        AT+CMODE=ANYONE<CR+LF>  BLE can connect to any other BLE chips
        AT+CMODE=?<CR+LF>       Request the binding connection mode(default:ANYONE)

    1. "AT+MAC" Request MAC address

        AT+MAC=?<CR+LF> Request MAC address of the BLE

    1. "AT+NAME" Set the name

        AT+NAME=DFBLEduinoV1.0<CR+LF>   Set the name of BLE to "DFBLEduinoV1.0".The length is limited to 13 Bytes or below
        AT+NAME=?<CR+LF>                Request the name of the BLE (default: DFBLEduinoV1.0)

    1. "AT+RESTART" restart the BLE

        AT+RESTART<CR+LF>   Restart the BLE chip

    1. "AT+SETTING" change the default setting (new in BLE firmware 1.6)
        
        AT+SETTING=DEFAULT<CR+LF>   Restore the default settings, same as PERIPHERAL mode
        AT+SETTING=DEFPERIPHERAL<CR+LF> Restore the default settings for PERIPHERAL mode
        AT+SETTING=DEFCENTRAL<CR+LF>    Restore the default settings for CENTRAL mode
        AT+SETTING=?<CR+LF> Request the setting mode (default: DEFPERIPHERAL). If the settings are changed by AT command, "UNKNOWN" will be replied.

    1. "AT+BLUNODEBUG" When Bluetooth is connected and BLE chip(CC2540) received the UART message from MCU(ATMEGA328), send the UART message not only to the Bluetooth, but also to the USB port. So that when Bluetooth is connected, we can use the serial monitor to get the UART message. (new in BLE firmware 1.6)

        AT+BLUNODEBUG=ON<CR+LF> Turn on the BLUNO DEBUG so that when Bluetooth is connected, we can use the serial monitor to get the UART message.
        AT+BLUNODEBUG=OFF<CR+LF>    Turn off the BLUNO DEBUG so that wireless programming will be more stable.
        AT+BLUNODEBUG=?<CR+LF>  Request the BLUNO DEBUG state (default: ON)

    1. "AT+USBDEBUG" When Bluetooth is connected and BLE chip(CC2540) received the Bluetooth message from IOS or Android device, send the data not only to the UART, but also to the USB port. So that when Bluetooth is connected, we can use the serial monitor to directly get the Bluetooth message. (new in BLE firmware 1.6)

        AT+USBDEBUG=ON<CR+LF>   Turn on the BLUNO DEBUG So that when Bluetooth is connected, we can use the serial monitor to directly get the Bluetooth message from IOS or Android device.
        AT+USBDEBUG=OFF<CR+LF>  Turn off the USB DEBUG so that wireless programming will be more stable.
        AT+USBDEBUG=?<CR+LF>    Request the USB DEBUG state (default: OFF)

    1. "AT+TXPOWER" Change the Transmitted Power which will change the signal range. (new in BLE firmware 1.6)

        AT+TXPOWER=0<CR+LF> Change the Transmitted Power to fit the iBeacon calibration. (4, 0, -6 -23 is acceptable)
        AT+TXPOWER=?<CR+LF> Request the Transmitted Power (default: 0)

    1. "AT+IBEACONS" Enable the iBeacons feature(new in BLE firmware 1.6)

        AT+IBEACONS=ON<CR+LF>   Enable the iBeacons feature.
        AT+IBEACONS=OFF<CR+LF>  Disable the iBeacons feature.
        AT+IBEACONS=?<CR+LF>    Request whether the iBeacons feature is enabled. (default: ON)

    1. "AT+VERSION" the version of the firmware(new in BLE firmware 1.6)

        AT+VERSION=?<CR+LF> Request the version of the firmware.

    1. "AT+RSSI" Request the RSSI of the BLE (new in BLE firmware 1.6)

        AT+RSSI=?<CR+LF>    Request the RSSI of the BLE(if there is no connection, "-000" will be returned)

    1. "AT+MAJOR" Set the major number of the iBeacons (new in BLE firmware 1.6)
        
        AT+MAJOR=0<CR+LF>   Set the major number of the iBeacons to "0". (0 to 65535 is acceptable)
        AT+MAJOR=?<CR+LF>   Request the major number of the iBeacons.(default "0")

    1. "AT+MINOR" Set the minor number of the iBeacons (new in BLE firmware 1.6)

        AT+MINOR=0<CR+LF>   Set the minor number of the iBeacons to "0". (0 to 65535 is acceptable)
        AT+MINOR=?<CR+LF>   Request the minor number of the iBeacons.(default "0")

    1. "AT+EXIT" Exit the AT Command Mode (new in BLE firmware 1.8 )，

        AT+EXIT<CR+LF>  Exit the AT Command Mode.



1. ### Issues 

    1. What is that between the microcontroller(ATmega328P) and Bluetooth chip(CC2540);
    how to communicate between them

    2. How to code AT commands on the sketch in Arduino IDE?




This is a program for opening the door or closing.
by using bluetooth and wifi network.

it's gonna be separated into two things.
One is using raspberry pi.
the other one is using bluno beetle.
some files which are gonna be used by beetle have 
the things like .ino arduino files.

_copyright JaeHwan Lim weeneen02@gmail.com_

