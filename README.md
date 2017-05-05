# RGBWebSocketServer
A Web-socket server for the ESP8266 used to control RGB LEDs, this includes a desktop web page ../rgb.html
and a mobile webpage .../rgbm.htlm using mobil jquery. 
The code includes a normal web server on port 80 and a websocket server on port 81. The web server will accept http resuest to controll the LEDs. To set the LEDs to red enter "http://192.168.4/1/rgb(255,0,0)" for example.

