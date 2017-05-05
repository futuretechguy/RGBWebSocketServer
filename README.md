# RGBWebSocketServer
This is a Web/Web-socket server for the ESP8266 used to control RGB LEDs, the code includes a desktop web page ../rgb.html,

![desktopchromebrowser](https://cloud.githubusercontent.com/assets/11177814/25767020/30aeaa42-31c4-11e7-9686-260a80b2de99.png)

and a mobile webpage .../rgbm.htlm using mobil jquery. 

![mobilepage](https://cloud.githubusercontent.com/assets/11177814/25767125/ddcf814c-31c4-11e7-9207-cdb236cda2ea.png)

The web server defaults to port 80 and a websocket server uses port 81. Both pages (above) are web socket clients that communicates with the WebSocket server so changes made are instanteneous and will be immediately reflected on all connected clients. 

The web server will accept http resuest to controll the attached LEDs. To set the LEDs to red for example enter "http://192.168.4/1/rgb(255,0,0)" (replace example IP with local network IP).

The code also allows for "On" and "Off" time schedules using a remote NTP server. This section of the code may be commented out if scheduling is not desired or internet access is not available.

![timesserver](https://cloud.githubusercontent.com/assets/11177814/25767487/b1b71d6a-31c7-11e7-8252-658d7cbf1cc7.png)
