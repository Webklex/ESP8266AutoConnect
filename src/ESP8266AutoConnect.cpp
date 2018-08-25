/*
  ESP8266AutoConnect - auto connect library for ESP8266
  Copyright (c) 2018 Malte Goldenbaum. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "ESP8266AutoConnect.h"

ESP8266AutoConnect::ESP8266AutoConnect(uint8_t led_disconnected, uint8_t led_connecting, uint8_t led_connected) {
    
    _led_disconnected 	= 0;
    _led_connecting	= 0;
    _led_connected	= 0;

    _blink_delay	= 2500;

    _wifi_delay 	= 1500;
    _max_ssid_len 	= 32;
    _max_connect_time 	= 3000;

    //_ssid[32] = "";

    _ping_host	= "www.google.com";
    _ping_port	= 443;
    
    if(	led_disconnected > 0 )  setDisconnectLed(led_disconnected);
    if(	led_connecting > 0 ) 	setConnectingLed(led_connecting);
    if(	led_connected > 0 ) 	setConnectedLed(led_connected);
}

void ESP8266AutoConnect::autoConnect() {
    if(WiFi.status() != WL_CONNECTED) {
    	scanAndSort();
    }
}

void ESP8266AutoConnect::setDisconnectLed(uint8_t led_disconnected) {
    _led_disconnected = led_disconnected;
    pinMode(_led_disconnected, OUTPUT);
}

void ESP8266AutoConnect::setConnectingLed(uint8_t led_connecting) {
    _led_connecting = led_connecting;
    pinMode(_led_connecting, OUTPUT);
}

void ESP8266AutoConnect::setConnectedLed(uint8_t led_connected) {
    _led_connected = led_connected;
    pinMode(_led_connected, OUTPUT);
}

void ESP8266AutoConnect::setBlinkDelay(int blink_delay) {
    _blink_delay = blink_delay;
}

void ESP8266AutoConnect::setPingHost(String ping_host) {
    _ping_host = ping_host;
}

void ESP8266AutoConnect::setPingPort(int ping_port) {
    _ping_port = ping_port;
}

void ESP8266AutoConnect::errorBlink() {
    if( _led_disconnected > 0 ) {
  	blink(_led_disconnected, 150);
    }
}

void ESP8266AutoConnect::blink(uint8_t led, int int_delay) {
  digitalWrite(led, HIGH);
  delay(int_delay);
  digitalWrite(led, LOW);
}

/* Clear previous modes. */
void ESP8266AutoConnect::disconnect() {
    WiFi.softAPdisconnect();
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
}

/* Scan available networks and sort them in order to their signal strength. */
void ESP8266AutoConnect::scanAndSort() {
    if( _led_disconnected > 0 ) digitalWrite(_led_disconnected, HIGH);
    if( _led_connecting > 0 ) digitalWrite(_led_connecting, LOW);
    if( _led_connected > 0 ) digitalWrite(_led_connected, LOW);
    
    disconnect();
    
    strncpy(_ssid, "", _max_ssid_len);
    memset(_ssid, 0, _max_ssid_len);
    
    int n = WiFi.scanNetworks();

    bool check = false;
    if (n != 0) {
    	int indices[n];
    	for (int i = 0; i < n; i++) {
      	    indices[i] = i;
    	}
    	for (int i = 0; i < n; i++) {
      	    for (int j = i + 1; j < n; j++) {
            	if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
          	    std::swap(indices[i], indices[j]);
                }
            }
    	}
    	for (int i = 0; i < n; ++i) {
      
      	    if(WiFi.encryptionType(indices[i]) == ENC_TYPE_NONE) {
        	
        	if( _led_disconnected > 0 ) digitalWrite(_led_disconnected, HIGH);
		if( _led_connecting > 0 ) digitalWrite(_led_connecting, HIGH);
		
        	check = connect(WiFi.SSID(indices[i]).c_str());
        	if(check == true) {
          	    strncpy(_ssid, WiFi.SSID(indices[i]).c_str(), _max_ssid_len);
          	    memset(_ssid, 0, _max_ssid_len);
          	    break;
        	}
       
            }
      
	    if( _led_disconnected > 0 ) digitalWrite(_led_disconnected, HIGH);
	    if( _led_connecting > 0 ) digitalWrite(_led_connecting, LOW);
    	}
    }

    if(check == false) {
    	delay(_wifi_delay);
    	scanAndSort();
    }
    delay(_wifi_delay);
}

bool ESP8266AutoConnect::ping() {
  // Use WiFiClient class to create TCP connections
  bool check = false;
  if(_ping_port == 443) {
    WiFiClientSecure client;
    check = client.connect(_ping_host, _ping_port);

    if(check == true && client.connected()) {
      check = true;
    }else{
      check = false;
    }
    
    client.stop();
  }else{
    WiFiClient client;
    check = client.connect(_ping_host, _ping_port);

    if(check == true && client.connected()) {
      check = true;
    }else{
      check = false;
    }
    
    client.stop();
  }

  return check;
}

bool ESP8266AutoConnect::connect(const char* test_ssid) {

    /* No pass for WiFi. We are looking for non-encrypteds. */
    WiFi.begin(test_ssid);
    
    int try_cnt = 0;
    
    /* Wait until WiFi connection but do not exceed MAX_CONNECT_TIME */
    while (WiFi.status() != WL_CONNECTED && try_cnt < _max_connect_time / _wifi_delay) {
      delay(_wifi_delay);
      try_cnt++;
    }
    
    if(WiFi.status() == WL_CONNECTED) {
	if( _led_disconnected > 0 ) digitalWrite(_led_disconnected, LOW);
	if( _led_connected > 0 ) digitalWrite(_led_connected, HIGH);
	
	delay(_wifi_delay);

	bool ping_result = ping();
	if(ping_result == true) {
	    if( _led_connecting > 0 ) digitalWrite(_led_connecting, LOW);
	}else{
	    if( _led_disconnected > 0 ) digitalWrite(_led_disconnected, HIGH);
	    if( _led_connected > 0 ) digitalWrite(_led_connected, LOW);
	    if( _led_connecting > 0 ) digitalWrite(_led_connecting, LOW);
	}

    	return ping_result;
    }
    
    return false;
}
