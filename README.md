# ESP8266AutoConnect

[![Software License][ico-license]](LICENSE.md)

Let the ESP8266 auto connect to an open wifi network.

With this library an ESP8266 can automatically connect to an open wifi network which has a working internet connection.
This library should not be considered save since it is a bad thing to have your iot autoconnect to everything it can get a hold of. However I developed it for a personal use case where exactly that was required. So here we go :)

## Usage

First, include the library in your sketch and initialize a new instance:

```arduino
#include <ESP8266AutoConnect.h>

int LED_DISCONNECTED = 15; //D8
int LED_CONNECTING = 13;   //D7
int LED_CONNECTED = 12;    //D6
#int LED_CONNECTED = 0;    //No indicator led for an online connection

ESP8266AutoConnect ac(LED_DISCONNECTED, LED_CONNECTING, LED_CONNECTED);
```

If you don't want to use any indicator led just ignore them:
```arduino
#include <ESP8266AutoConnect.h>

ESP8266AutoConnect ac;
```

Next, simply call the `ac.autoConnect()` function. It won't continue until it has established a connection.

```arduino
void loop() {
    ac.autoConnect();
    delay(1000);
}
```

All available methods:

```arduino
void ac.autoConnect();

bool ac.ping(String host, int port);

void ac.setDisconnectLed(int); //Default = 0 (unset)
void ac.setConnectingLed(int); //Default = 0 (unset)
void ac.setConnectedLed(int);  //Default = 0 (unset)
void ac.setBlinkDelay(int);    //Default = 250
void ac.setPingHost(String);   //Default = "www.google.com"
void ac.setPingPort(int);      //Default = 443
```

## Change log

Please see [CHANGELOG](CHANGELOG.md) for more information what has changed recently.

## Security

If you discover any security related issues, please email github@webklex.com instead of using the issue tracker.

## Credits

- [Webklex][link-author]
- [All Contributors][link-contributors]

## License

The MIT License (MIT). Please see [License File](LICENSE.md) for more information.

[ico-license]: https://img.shields.io/badge/license-MIT-brightgreen.svg?style=flat-square
[link-author]: https://github.com/webklex
[link-contributors]: https://github.com/Webklex/laravel-imap/graphs/contributors#
