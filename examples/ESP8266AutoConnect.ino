/*
 * With this library an ESP8266 can automatically connect to an open and to the internet connected 
 * network.
 */

#include <ESP8266AutoConnect.h>

/** GPIO Mapping https://cdn.instructables.com/FIR/Y0M6/JH8JBTNR/FIRY0M6JH8JBTNR.LARGE.jpg?auto=webp&width=428 */
uint8_t LED_DISCONNECTED = 15; //D8
uint8_t LED_CONNECTING   = 13; //D7
uint8_t LED_CONNECTED    = 12; //D6

ESP8266AutoConnect ac(LED_DISCONNECTED, LED_CONNECTING, LED_CONNECTED);

void setup() {
    Serial.begin(115200);
    delay(10);
}

void loop() {
    // put your main code here, to run repeatedly:
    ac.autoConnect();
    delay(1000);
}
