# RGBWebSocketServer
This is a Web/Web-socket server for the ESP8266 used to control RGB LEDs, the code includes a desktop web page ../rgb.html,

![desktopchromebrowser](https://cloud.githubusercontent.com/assets/11177814/25767020/30aeaa42-31c4-11e7-9686-260a80b2de99.png)

and a mobile webpage .../rgbm.htlm using mobile jquery. 

![mobilepage](https://cloud.githubusercontent.com/assets/11177814/25767125/ddcf814c-31c4-11e7-9207-cdb236cda2ea.png)

The web server defaults to port 80 and a websocket server uses port 81. Both pages (above) are web socket clients that communicates with the WebSocket server so changes made are instantaneous and will be immediately reflected on all connected clients. 

The web server will accept http request to control the attached LEDs. To set the LEDs to red for example enter "http://192.168.4/1/rgb(255,0,0)" (replace example IP with local network IP).

![httprequest](https://cloud.githubusercontent.com/assets/11177814/25767634/4875b62a-31c9-11e7-9ced-df78f96ceb16.png)

The code also allows for "On" and "Off" time schedules using a remote NTP server. This section of the code may be commented out if scheduling is not desired or internet access is not available.

![timesserver](https://cloud.githubusercontent.com/assets/11177814/25767487/b1b71d6a-31c7-11e7-8252-658d7cbf1cc7.png)

For Simplicity I uploded the code to an off the shelf Magic ESP8266 based controller from Ebay. http://www.ebay.com/itm/ARILUX-Mini-WiFi-RGB-Controller-APP-Control-For-5050-LED-Strip-Light-DC-5-28V/401192213381?_trksid=p2047675.c100005.m1851&_trkparms=aid%3D222007%26algo%3DSIC.MBE%26ao%3D2%26asc%3D40130%26meid%3Dd070ccc3b20443d9ad0c444dfc06b1a8%26pid%3D100005%26rk%3D4%26rkt%3D6%26sd%3D262887039437

![rgbcontroller](https://cloud.githubusercontent.com/assets/11177814/25767714/e3ad6a84-31c9-11e7-9022-e4db67aba57d.png)

