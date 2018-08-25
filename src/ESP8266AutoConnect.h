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

#ifndef ESP8266AutoConnect_h
#define ESP8266AutoConnect_h

#include "ESP8266WiFi.h"

class ESP8266AutoConnect {
  public:
    ESP8266AutoConnect(uint8_t led_disconnected = 0, uint8_t led_connecting = 0, uint8_t led_connected = 0);
    void autoConnect();
    bool ping();
    void setDisconnectLed(uint8_t led_disconnected);
    void setConnectingLed(uint8_t led_connecting);
    void setConnectedLed(uint8_t led_connected);
    void setBlinkDelay(int blink_delay);
    void setPingHost(String ping_host);
    void setPingPort(int ping_port);
    void errorBlink();
  private:
    void disconnect();
    void blink(uint8_t led, int int_delay);
    void scanAndSort();
    bool connect(const char* test_ssid);
    uint8_t _led_disconnected;
    uint8_t _led_connecting;
    uint8_t _led_connected;
    int _blink_delay;
    int _wifi_delay;
    int _max_ssid_len;
    int _max_connect_time;
    char _ssid[32];
    String _ping_host;
    int _ping_port;
};

#endif
