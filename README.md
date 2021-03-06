# RGBWebSocketServer
This is a Web/Web-socket server for the ESP8266 used to control RGB LEDs, the code includes a desktop web page ../rgb.html,

![desktopchromebrowser](https://cloud.githubusercontent.com/assets/11177814/25767020/30aeaa42-31c4-11e7-9686-260a80b2de99.png)

and a mobile webpage .../rgbm.htlm using mobile jquery. 

<img src="images/iPhoneRGB.png" width="300">

The web server defaults to port 80 and a websocket server uses port 81. Both pages (above) are web socket clients that communicates with the WebSocket server so changes made are instantaneous and will be immediately reflected on all connected clients. 

The web server will accept http request to control the attached LEDs. To set the LEDs to red for example enter "http://192.168.4/1/rgb(255,0,0)" (replace example IP with local network IP).

![httprequest](https://cloud.githubusercontent.com/assets/11177814/25767634/4875b62a-31c9-11e7-9ced-df78f96ceb16.png)

The code also allows for "On" and "Off" time schedules using a remote NTP server. This section of the code may be commented out if scheduling is not desired or internet access is not available.

![timesserver](https://cloud.githubusercontent.com/assets/11177814/25767487/b1b71d6a-31c7-11e7-8252-658d7cbf1cc7.png)

For Simplicity I uploded the code to an off the shelf Magic ESP8266 based controller from Ebay. See https://tinkerman.cat/magic-home-led-controller-espurnad/ for information on flashing.

![rgbcontroller](https://cloud.githubusercontent.com/assets/11177814/25767714/e3ad6a84-31c9-11e7-9022-e4db67aba57d.png)

Since flashing requires opening and soldering a FTDI to the device, consider enableling "Over-The-Air" updating in the code. This allows future updates without reopening the device.

![ota](https://cloud.githubusercontent.com/assets/11177814/25768112/c9e0de98-31cd-11e7-97d8-47869949c280.png)

The code uses the WiFiManager libery which requires special steps for connecting to the local network when the device boots for the first time after flashing. Please see link below.
https://github.com/tzapu/WiFiManager

If a static IP is not configured, the assigned IP and DNS hostname will be visible in the serial console after starting.

![serialmonitor](https://cloud.githubusercontent.com/assets/11177814/25768282/b21fb82c-31cf-11e7-877b-079713b47ab4.png)
