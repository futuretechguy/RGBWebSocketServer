#include <WiFiManager.h>
#include <NTPtimeESP.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include "RGBControl.h"
#include <EEPROM.h>
#include "RGBControlMobile.h"



#define LED_R 2
#define DBG_OUTPUT_PORT Serial

#define REDPIN 14  //12
#define GREENPIN 5  //13
#define BLUEPIN 12  //15

const byte interruptPin = 0;
const char* host = "rgbGlassDoor";  //rgbserver

boolean device_one_state = false;

unsigned long StartTime = 0;  //millis();
unsigned long Interval = 60000;  //5 minutes

NTPtime NTPch("us.pool.ntp.org");

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
int wsClientNumber[5] = { -1, -1, -1, -1, -1};
int lastClientIndex = 0;
const int max_ws_client = 5;

int red = 0;
int green = 0;
int blue = 0;

String startH = "";
String startM = "";
String stopH = "";
String stopM = "";

char* oPinState;

strDateTime dateTime;

//holds the current upload
File fsUploadFile;
volatile int state = LOW;

void setStatus(boolean st)
{
  if (st)
  {
    device_one_state = true;
    Serial.print("Change state:");
    Serial.println(state);
    digitalWrite(LED_R, LOW); // Xuất trạng thái  ra chân GPIO16
  }
  else
  {
    device_one_state = false;
    Serial.print("Change state:");
    Serial.println(st);
    digitalWrite(LED_R, HIGH); //
  }
}



//format bytes
String formatBytes(size_t bytes) {
  if (bytes < 1024) {
    return String(bytes) + "B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0) + "KB";
  } else if (bytes < (1024 * 1024 * 1024)) {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  } else {
    return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
  }
}

String getContentType(String filename) {
  if (server.hasArg("download")) return "application/octet-stream";
  else if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(String path) {
  DBG_OUTPUT_PORT.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.htm";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
    if (SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

void SendTimeSchedule() {
  String json;
  if (startH != "00") {
    json = GetTimeJson("Start", startH, startM);
    int numcl = 0;
    for (numcl = 0; numcl < max_ws_client; numcl++)
    {
      if (wsClientNumber[numcl] != -1)
        webSocket.sendTXT(wsClientNumber[numcl], json);
    }
    json = String();
  }

  if (stopH != "00") {
    json = GetTimeJson("Stop", stopH, stopM);
    int numcl = 0;
    for (numcl = 0; numcl < max_ws_client; numcl++)
    {
      if (wsClientNumber[numcl] != -1)
        webSocket.sendTXT(wsClientNumber[numcl], json);
    }
    json = String();
  }

}

//send RGB pin state on connect
void SendRGBState() {
  int oRVal = digitalRead(REDPIN);
  int oGVal = digitalRead(GREENPIN);
  int oBVal = digitalRead(BLUEPIN);
  DBG_OUTPUT_PORT.println("Pin State: " + String(oRVal) + "," + String(oRVal) + "," + String(oRVal) );  ///for debuging

  if (oRVal == 1 || oGVal == 1 || oBVal == 1) {
    device_one_state = true;
    oPinState = "On";
    wsSendRGBState();
  } else {
    device_one_state = false;
    oPinState = "Off";

    String json = GetPinJson("LED", oPinState);
    int numcl = 0;
    for (numcl = 0; numcl < max_ws_client; numcl++)
    {
      if (wsClientNumber[numcl] != -1)
        webSocket.sendTXT(wsClientNumber[numcl], json);
    }
    json = String();
  }
}

void wsSendState() {  //update state "on connect" set color or on/off

  if (device_one_state) {
    oPinState = "On";

    //Restore last state
    EEPROM.get(0, red);
    EEPROM.get(5, green);
    EEPROM.get(10, blue);

    if (GetOnState() == false) { // if LEDs were not previously set (in memory), then set to white
      red = 1024;
      green = 1024;
      blue = 1024;
    }
    //set LED/Pin color/state
    analogWrite(REDPIN, red);
    analogWrite(GREENPIN, green);
    analogWrite(BLUEPIN, blue);

  }
  else if (oPinState = "Off") {

    EEPROM.put(0, red); // Writes the value to EEPROM
    EEPROM.put(5, green);
    EEPROM.put(10, blue);
    EEPROM.commit();

    red = 0;
    green = 0;
    blue = 0;

    // Set LED/Pin off state
    analogWrite(REDPIN, red);
    analogWrite(GREENPIN, green);
    analogWrite(BLUEPIN, blue);
  }
  String json = GetRGBJson(red, green, blue);
  int numcl = 0;
  for (numcl = 0; numcl < max_ws_client; numcl++)
  {
    if (wsClientNumber[numcl] != -1)
      webSocket.sendTXT(wsClientNumber[numcl], json); //send to socket client
  }
  json = String();
}

//Sends the Start/Stop time when it is set or updated
void wsSendTimeJson( String oObjName, String oValH, String oValM) {
  String json = GetTimeJson(oObjName, oValH, oValM);
  int numcl = 0;
  for (numcl = 0; numcl < max_ws_client; numcl++)
  {
    if (wsClientNumber[numcl] != -1)
      webSocket.sendTXT(wsClientNumber[numcl], json);
  }
  json = String();
}


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
  switch (type) {
    case WStype_DISCONNECTED:
      DBG_OUTPUT_PORT.printf("[%u] Disconnected!n", num);
      break;

    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        DBG_OUTPUT_PORT.printf("[%u] Connected from %d.%d.%d.%d url: %sn", num, ip[0], ip[1], ip[2], ip[3], payload);
        int index = (num % max_ws_client);
        if (index <= 0) index = 0;
        wsClientNumber[index] = num;
        DBG_OUTPUT_PORT.printf("Save client index %d :%un", index, num);
        SendRGBState();
        SendTimeSchedule();
        //wsSendState();             // send message to client
      }
      break;

    case WStype_TEXT:
      DBG_OUTPUT_PORT.printf("[%u] get Text: %sn", num, payload);
      String strPayload = (char*)payload;
      String oSize = "00";

      if (payload[0] == '1') {
        setStatus(true);
        wsSendState();
      }
      else if (payload[0] == '0') {
        setStatus(false);
        wsSendState();
      }
      else if (strPayload.indexOf(",") > -1 && strPayload.lastIndexOf(",") > -1) {
        ProcessRGBwsRequest(strPayload);
        wsSendRGBState();

      } else if (strPayload.indexOf(":") > -1 && strPayload.substring(0, strPayload.indexOf("@")) == "start") {   //if valud start time is received
        startH = strPayload.substring(strPayload.indexOf("@") + 1, strPayload.indexOf(":"));                      // Extract hours
        startM = strPayload.substring(strPayload.indexOf(":") + 1);                                               // Extract minutes
        wsSendTimeJson("Start", startH, startM);  //bounce back to client                                         //bounce back start time to client

        //save start time to memory
        EEPROM.write(15, startH[0]);
        EEPROM.write(16, startH[1]);
        EEPROM.write(17, startM[0]);
        EEPROM.write(18, startM[1]);
        EEPROM.commit();
      }

      else if (strPayload.indexOf(":") > -1 && strPayload.substring(0, strPayload.indexOf("@")) == "stop") {  //if valud stop time is received
        stopH = strPayload.substring(strPayload.indexOf("@") + 1, strPayload.indexOf(":"));                   // Extract hours
        stopM = strPayload.substring(strPayload.indexOf(":") + 1);                                            // Extract minutes
        wsSendTimeJson("Stop", stopH , stopM);                                                                //bounce back stop time to client

        //save start time to memory
        EEPROM.write(19, stopH[0]);
        EEPROM.write(20, stopH[1]);
        EEPROM.write(21, stopM[0]);
        EEPROM.write(22, stopM[1]);
        EEPROM.commit();
      }

      else if (strPayload.indexOf(":") == -1 && strPayload.substring(0, 5) == "start") {                      //if start time cancel received
        startH = "00";                                                                                        // set hours variable to 00
        startM = "00";                                                                                        // set minutes variable to 00
        wsSendTimeJson("Stop", startH , startM);                                                              //bounce back start cancal client

        EEPROM.write(15, startH[0]);                                                                          //save 00 to start memory
        EEPROM.write(16, startH[1]);
        EEPROM.write(17, startM[0]);
        EEPROM.write(18, startM[1]);
        EEPROM.commit();
      }

      else if (strPayload.indexOf(":") == -1 && strPayload.substring(0, 4) == "stop") {                         //if stop time cancel received
        stopH = "00";                                                                                           // set stop hours variable to 00
        stopM = "00";                                                                                           // set stop minutes variable to 00
        wsSendTimeJson("Stop", stopH , stopM);                                                                  //bounce back stop cancal to client

        EEPROM.write(19, stopH[0]);                                                                             //save 00 to stop memory
        EEPROM.write(20, stopH[1]);
        EEPROM.write(21, stopM[0]);
        EEPROM.write(22, stopM[1]);
        EEPROM.commit();
      }
      else {
        webSocket.sendTXT(num, "message received");
      }
      break;
  }

}
void changestate() {
  state = !state;
  setStatus(state);
  wsSendState();
}

char GetBitArrayAsByte(const char inputArray[8])
{
  char result = 0;
  for (int idx = 0; idx < 8; ++idx)
  {
    result |= (inputArray[7 - idx] << idx);
  }
  return result;
}


// set RGB pin values when they are changed by the sliders
void ProcessRGBwsRequest(String rgbcmd) {
  String json;
  if (rgbcmd.indexOf(",") > -1 && rgbcmd.lastIndexOf(",") > -1) { //?,?,?
    Serial.println("This is a valid rgb request");
    red = rgbcmd.substring(1, rgbcmd.indexOf(',')).toInt();
    green = rgbcmd.substring(rgbcmd.indexOf(',') + 1, rgbcmd.lastIndexOf(',')).toInt();
    blue = rgbcmd.substring(rgbcmd.lastIndexOf(',') + 1).toInt();

    if (red > -1 && red <= 1024) {
      analogWrite(REDPIN, red);
      Serial.println("Red is set: ");
      Serial.print(red);
    }
    if (green > -1 && green <= 1024) {
      analogWrite(GREENPIN, green);
      Serial.println("Green is set: ");
      Serial.print(green);
    }
    if (blue > -1 && blue <= 1024) {
      analogWrite(BLUEPIN, blue);
      Serial.println("Blue is set: ");
      Serial.print(blue);
    }
  }
}

//returns the RGB on or off state
bool GetOnState() {
  if (red > 0 || green > 0 || blue  > 0) {
    return true;
  } else {
    return false;
  }
}

//sends the rgb states
String wsSendRGBState() {

  String json = GetRGBJson(red, green, blue);
  int numcl = 0;
  for (numcl = 0; numcl < max_ws_client; numcl++)
  {
    if (wsClientNumber[numcl] != -1)
      webSocket.sendTXT(wsClientNumber[numcl], json);
  }
  json = String();

}

//if start hourt matches the actual hour return true
bool StartTimeSync() {
  if (dateTime.hour == startH.toInt() && dateTime.minute == startM.toInt()) {
    return true;
  } else {
    return false;
  }
}

//if start minute matches the actual minute return true
bool StopTimeSync() {
  if (dateTime.hour == stopH.toInt() && dateTime.minute == stopM.toInt()) {
    return true;
  } else {
    return false;
  }
}


void setup(void) {

  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(LED_R, OUTPUT);

  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);


  DBG_OUTPUT_PORT.begin(115200);
  DBG_OUTPUT_PORT.print("n");
  DBG_OUTPUT_PORT.setDebugOutput(true);

  SPIFFS.begin();
  {
    Dir dir = SPIFFS.openDir(" / ");
    while (dir.next()) {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      DBG_OUTPUT_PORT.printf("FS File: % s, size: % sn", fileName.c_str(), formatBytes(fileSize).c_str());
    }
    DBG_OUTPUT_PORT.printf("n");
  }

  EEPROM.begin(512);

  //=======[ WiFiManager Wifi init ]======================================
  WiFiManager wifiManager;
  //wifiManager.resetSettings();
  //wifiManager.setSTAStaticIPConfig(IPAddress(192,168,1,99), IPAddress(192,168,1,1), IPAddress(255,255,255,0));
  wifiManager.autoConnect("AutoConnectAP");
  //======================================================================

  DBG_OUTPUT_PORT.println("");
  DBG_OUTPUT_PORT.print("Connected! IP address: ");
  DBG_OUTPUT_PORT.println(WiFi.localIP());

  MDNS.begin(host);
  DBG_OUTPUT_PORT.print("Open http://");
  DBG_OUTPUT_PORT.print(host);
  DBG_OUTPUT_PORT.println(".local/edit to see the file browser");

  //SERVER INIT
  server.on("/rgb.html", []() {                        //load RGB control page
    Serial.println("rgb.html");
    server.send (200, "text/html", PAGE_RGBControlPage);
  });

  server.on("/rgbm.html", []() {                        //load RGB mobile control page
    Serial.println("rgbm.html");
    server.send (200, "text/html", PAGE_RGBControlMobile);
  });

  server.onNotFound([]() {                               //called when the url is not defined here

    if (isRGBrequest(server.uri())) {
      ProcessRGBGetRequest(server.uri());  //process RGB GET commands

    } else if (!handleFileRead(server.uri())) {
      server.send(404, "text/plain", "Command Not Found");    //use it to load content from SPIFFS

    }
  });

  server.on("/all", HTTP_GET, []() {                      //get heap status, analog input value and all GPIO statuses in one json call

    String json = GetESPJson();
    server.send(200, "text/json", json);
    json = String();
  });

  server.begin();
  webSocket.begin();  // start webSocket server
  webSocket.onEvent(webSocketEvent);

  MDNS.addService("http", "tcp", 80);  // Add service to MDNS
  MDNS.addService("ws", "tcp", 81);
  DBG_OUTPUT_PORT.println("HTTP server started");

  attachInterrupt(digitalPinToInterrupt(interruptPin), changestate, FALLING);

  GetScheduleFromEEPROM();                                                                           //restore On/Off schedules from eeprom

  //restore RGB variabled from eeprom
  EEPROM.get(0, red);
  EEPROM.get(5, green);
  EEPROM.get(10, blue);

  analogWrite(REDPIN, red);
  analogWrite(GREENPIN, green);
  analogWrite(BLUEPIN, blue);
}

void loop(void) {
  unsigned long CurrentTime = millis();
  server.handleClient();
  webSocket.loop();

  if (CurrentTime - StartTime > Interval) {
    dateTime = NTPch.getNTPtime(-5.0, 1);
    NTPch.printDateTime(dateTime);

    //DBG_OUTPUT_PORT.println(ShowTimeComparison());
    
    if (StartTimeSync()) {
      setStatus(true);
      wsSendState();
    } else if (StopTimeSync()) {
      setStatus(false);
      wsSendState();
    }
    StartTime = CurrentTime;
  }
}

void GetScheduleFromEEPROM() {

  char val1 = EEPROM.read(15);
  char val2 = EEPROM.read(16);
  char val3 = EEPROM.read(17);
  char val4 = EEPROM.read(18);
  char val5 = EEPROM.read(19);
  char val6 = EEPROM.read(20);
  char val7 = EEPROM.read(21);
  char val8 = EEPROM.read(22);

  startH += val1;
  startH += val2;
  startM += val3;
  startM += val4;
  stopH += val5;
  stopH += val6;
  stopM += val7;
  stopM += val8;

  //Test if the stored value is numeric
  if (!isValidNumber (startH)) {
    startH = "00";
  }
  if (!isValidNumber (startM)) {
    startM = "00";
  }
  if (!isValidNumber(stopH)) {
    stopH = "00";
  }
  if (!isValidNumber(stopM)) {
    stopM = "00";
  }
}


String GetESPJson() {
  String json;
  json += "{";
  json += "\"heap\"";
  json += ":";
  json += "\"" + String(ESP.getFreeHeap()) + "\"";
  json += ",";
  json += "\"analog\"";
  json += ":";
  json += "\"" + String(analogRead(A0)) + "\"";
  json += ",";
  json += "\"gpio\"";
  json += ":";
  json += "\"" + String((uint32_t)(((GPI | GPO) & 0xFFFF) | ((GP16I & 0x01) << 16))) + "\"";
  json += "}";
  return json;
}


String GetPinJson(String oDevice, String oValue)
{
  String json;
  json += "{";
  json += "\"" + oDevice + "\"";
  json += ":";
  json += "\"" + oValue + "\"";
  json += "}";
  return json;
}

String GetTimeJson(String objName, String oValH, String oValM)
{
  String json;
  json += "{";
  json += "\"" + objName + "\"";
  json += ":";
  json += "\"" + oValH + ":" + oValM + "\"";
  json += "}";
  return json;
}

String GetRGBJson(int oRed, int oGreen, int oBlue)
{
  String json;
  json += "{";
  json += "\"Red\"";
  json += ":";
  json += "\"" + String(oRed) + "\"";
  json += ",";
  json += "\"Green\"";
  json += ":";
  json += "\"" + String(oGreen) + "\"";
  json += ",";
  json += "\"Blue\"";
  json += ":";
  json += "\"" + String(oBlue) + "\"";
  json += "}";
  return json;
}

String ShowTimeComparison() {

  String TimeComp = "Actual Time: = " ;
  TimeComp += String(dateTime.hour);
  TimeComp += ":";
  TimeComp += String(dateTime.minute);
  TimeComp += "\n";
  TimeComp += "Saved Start Time = ";
  TimeComp += String(startH);
  TimeComp += ":";
  TimeComp += String(startM);
  TimeComp += "\n";
  TimeComp += "Saved Stop Time = ";
  TimeComp += String(stopH);
  TimeComp += ":";
  TimeComp += String(stopM);
  return TimeComp;
}

bool isRGBrequest(String message) {
  String rgbcmd = message.substring(1, 4);
  //Serial.println(rgbcmd);
  if (rgbcmd == "rgb") {
    return true;
  } else {
    return false;
  }
}

void ProcessRGBGetRequest (String message) {
  String rgbcmd = message.substring(5, message.indexOf(')'));
  int red = rgbcmd.substring(0, rgbcmd.indexOf(',')).toInt();
  int green = rgbcmd.substring(rgbcmd.indexOf(',') + 1, rgbcmd.lastIndexOf(',')).toInt();
  int blue = rgbcmd.substring(rgbcmd.lastIndexOf(',') + 1).toInt();

  String response;
  response += "Red=";
  response += red;
  response += "\n";
  response += "Green=";
  response += green;
  response += "\n";
  response += "Blue=";
  response += blue;
  server.send (200, "text/plain", response);

  String rgbcmdmod;
  rgbcmdmod += "\"";
  rgbcmdmod += red * 4;
  rgbcmdmod += ",";
  rgbcmdmod += green * 4;
  rgbcmdmod += ",";
  rgbcmdmod += blue * 4;
  rgbcmdmod += "\"";

  ProcessRGBwsRequest(rgbcmdmod);
  wsSendState;
}


String getjsonfilelist(String filetype, String fileName) {
  String json;
  json += "{";
  json += "\"File Type:\"";
  json += ":";
  json += "\"" + filetype + "\"";
  json += ",";
  json += "\"File Name:\"";
  json += ":";
  json += "\"" + fileName + "\"";
  json += "}";
  return json;
}

boolean isValidNumber(String str) {
  boolean isNum = false;
  for (byte i = 0; i < str.length(); i++)
  {
    isNum = isDigit(str.charAt(i)) || str.charAt(i) == '+' || str.charAt(i) == '.' || str.charAt(i) == '-';
    if (!isNum) return false;
  }
  return isNum;
}
