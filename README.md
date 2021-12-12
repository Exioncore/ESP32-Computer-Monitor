# ESP32-Computer-Monitor
A display driven by an ESP32 to display computer information received via Serial Port. Additionaly, adalight is integrated through its own serial port and an IR receiver.

The project is built via PlatformIO. Adalight mode of operation can be used via [Prismatik](https://github.com/psieg/Lightpack) on the computer side. Application on the computer 
side to send data to the ESP32 for viewing and for switching audio preset not provided.

Computer monitoring pages:\
![](https://i.imgur.com/cYhqmPe.png)
![](https://i.imgur.com/8VUoE8q.png)

Page when connection to computer is not established:\
![](https://i.imgur.com/5Ih3r04.png)

Settings pages:\
![](https://i.imgur.com/idlPaVJ.png)
![](https://i.imgur.com/vA0yT5d.png)
![](https://i.imgur.com/ILD5rrH.png)

Components used:
- [ESP32 DoIt devkit v1](https://www.ebay.com/itm/254138283456)
- [2.2 inch TFT Display 240*320 pixels - ILI9341](https://www.tinytronics.nl/shop/en/displays/tft/2.2-inch-tft-display-240*320-pixels-ili9341)
- [Level Shifter](https://www.ebay.com/itm/253048505379)
- [Micro USB to UART TTL - CP2102](https://www.ebay.com/itm/253285074727)
- [IR receiver](https://www.ebay.com/itm/252753278127)
