//
// Created by Leon Boshuizen on 14/11/2021.
//

#ifndef TOUCHCONTROLLER_ESP8266_01_H
#define TOUCHCONTROLLER_ESP8266_01_H

#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/watchdog.h"

#include <stdio.h>
#include <string.h>

#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

#define RESPONSEBUFFERLEN 1000 // size of buffer to receive data from web client(max 2048)
#define SENDBUFFERLEN 500 // size of buffer to send data to web client (max 2048)
#define MICROS 1000000

// see: https://github.com/ekstrand/ESP8266wifi/blob/master/SerialESP8266wifi.cpp

class esp8266_01 {
private:
    char responseBuffer[RESPONSEBUFFERLEN] = {0} ;

public:

    static void doRestart(const char* fatalMsg) {
        // something went wrong, so restart
        printf("*** fatal, restart in 2 secs: ");
        puts(fatalMsg);
        sleep_ms(2000);
        watchdog_reboot(0, 0, 0);
        sleep_ms(2000);
    }

    bool sendATcommandOK(const char* command, int64_t allowTime) {
        return sendATcommand(command, allowTime, "OK");
    }

    bool sendATcommand(const char* command, int64_t allowTime, const char* successMsg) {
        // send AT command and check response
        allowTime *= MICROS; // convert to micro secs
        absolute_time_t start = get_absolute_time();
        bool runCommand = true;
        int buffPtr = 0;
        char sendBuffer[SENDBUFFERLEN];
        snprintf(sendBuffer, SENDBUFFERLEN, "AT+%s\r\n", command);

        // loop until have required response or exceed allowed time
        while (absolute_time_diff_us(start, get_absolute_time()) < allowTime) {
            if (runCommand && strlen(command) > 0) {
                // send required AT command
                uart_puts(UART_ID, sendBuffer);
                printf("->AT: %s\n", command);
                runCommand = false;
                buffPtr = 0;
            }
            buffPtr = getATdata(buffPtr);
            if (buffPtr == -1) {
                // abort if response is too long
                printf("*** Response to command %s is too long: [%s]\n", command, responseBuffer);
                return false;
            }

            //printf("<-: %s\n", responseBuffer);

            if ((strlen(successMsg) > 0) && (strstr(responseBuffer, successMsg) != nullptr)) {
                // have required response
                // printf("Success: [%s]\n", responseBuffer);
                return true;
            }
            // expected response not found, check if busy processing
            if (strstr(responseBuffer, "busy p...") != nullptr) {
                printf("ESP8266 busy, retry command %s\n", command);
                sleep_ms(1000); // ESP8266 not ready for command, so retry by relooping
                //runCommand = true;
                continue;
            }
            // ignore error due to web page being closed
            if (strstr(responseBuffer, "link is not valid") != nullptr) return false;
        }

        // timed out, required response not found
        if (strlen(successMsg) > 0) {
            if (buffPtr > 0) {
                if (strstr(responseBuffer, "link is not valid") == nullptr) {// ignore error due to web page being closed
                    if (strstr(responseBuffer, "busy p...") != nullptr) doRestart("Timed out waiting on ESP8266 busy");
                    if (strlen(responseBuffer) == 0) doRestart("No ESP8266 response");
                    if (strstr(responseBuffer, "busy s...") != nullptr) doRestart("ESP8266 unable to receive");
                    if (strstr(responseBuffer, "ERROR") != nullptr) doRestart("ESP8266 out of sync with Pico");
                    printf("*** Command %s got unexpected response: [%s]\n", command, responseBuffer);
                }
            } else doRestart("Timed out waiting for ESP8266 response");
        } else return (buffPtr > 0); // where successMsg is ignored
        return false;
    }

    int getATdata(int buffPtr) {
        // obtain response from ESP8266
        bool tooLong = false;
        while (uart_is_readable(UART_ID)) {
            responseBuffer[buffPtr] = uart_getc(UART_ID);  // save response into buffer
            if (buffPtr >= RESPONSEBUFFERLEN-1) tooLong = true;
            else buffPtr++;
        }
        responseBuffer[buffPtr] = 0; // string terminator
        return (tooLong) ? -1 : buffPtr;
    }

    void init(){
        printf("init\n");
        uart_set_format(uart0, 8, 1, UART_PARITY_NONE);
        uart_init(UART_ID, BAUD_RATE);
        gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
        gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

        //uart_puts(UART_ID, "+++\r\n");

        printf("pending uart\n");
        while(uart_is_readable(UART_ID)){
            uart_getc(UART_ID);
        }
        printf("completed\n");
        sendATcommandOK("CWMODE=1", 1);
        sleep_ms(1000);
    }

    void check(){

        printf("check\n");
        sendATcommandOK("CWLAP", 5);
        printf("done\n");
    }

};


#endif //TOUCHCONTROLLER_ESP8266_01_H
