# RGBWebSocketServer
This is a Web/Web-socket server for the ESP8266 used to control RGB LEDs, the code includes a desktop web page ../rgb.html,

![desktopchromebrowser](https://cloud.githubusercontent.com/assets/11177814/25767020/30aeaa42-31c4-11e7-9686-260a80b2de99.png)

and a mobile webpage .../rgbm.htlm using mobil jquery. 


The code includes a normal web server on port 80 and a websocket server on port 81. The web server will accept http resuest to controll the LEDs. To set the LEDs to red enter "http://192.168.4/1/rgb(255,0,0)" (replace example IP with local network IP) for example.

