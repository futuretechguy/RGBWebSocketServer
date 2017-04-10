#ifndef PAGE_RGBControlPage_H
#define PAGE_RGBControlPage_H

const char PAGE_RGBControlPage[] PROGMEM = R"=====(
<!doctype html>
<html>
<head>
    <title>ESP8266 Websocket</title>
    <script src="http://code.jquery.com/jquery-2.0.0.js"></script>
    <style>
        .onoffswitch {
            position: relative;
            width: 67px;
            -webkit-user-select: none;
            -moz-user-select: none;
            -ms-user-select: none;
        }

        .onoffswitch-checkbox {
            display: none;
        }

        .onoffswitch-label {
            display: block;
            overflow: hidden;
            cursor: pointer;
            border: 2px solid #999999;
            border-radius: 20px;
        }

        .onoffswitch-inner {
            display: block;
            width: 200%;
            margin-left: -100%;
            transition: margin 0.3s ease-in 0s;
        }

            .onoffswitch-inner:before, .onoffswitch-inner:after {
                display: block;
                float: left;
                width: 50%;
                height: 19px;
                padding: 0;
                line-height: 19px;
                font-size: 12px;
                color: white;
                font-family: Trebuchet, Arial, sans-serif;
                font-weight: bold;
                box-sizing: border-box;
            }

        .onoffswitchT-inner:before {
            content: "ON";
            padding-left: 10px;
            background-color: #34A7C1;
            color: #FFFFFF;
        }

        .onoffswitch-inner:before {
            content: "ON";
            padding-left: 10px;
            background-color: #34A7C1;
            color: #FFFFFF;
            text-align: left;
        }

        .onoffswitch-inner:after {
            content: "OFF";
            padding-right: 10px;
            background-color: #EEEEEE;
            color: #999999;
            text-align: right;
        }

        .onoffswitch-switch {
            display: block;
            width: 18px;
            margin: 0.5px;
            background: #FFFFFF;
            position: absolute;
            top: 0;
            bottom: 0;
            right: 44px;
            border: 2px solid #999999;
            border-radius: 20px;
            transition: all 0.3s ease-in 0s;
        }

        .onoffswitch-checkbox:checked + .onoffswitch-label .onoffswitch-inner {
            margin-left: 0;
        }

        .onoffswitch-checkbox:checked + .onoffswitch-label .onoffswitch-switch {
            right: 0px;
        }

        .rTable {
            display: block;
            width: 100%;
        }

        .rTableHeading, .rTableBody, .rTableFoot, .rTableRow {
            clear: both;
        }

        .rTableHead, .rTableFoot {
            background-color: #DDD;
            font-weight: bold;
        }

        .rTableCell, .rTableHead {
            border: 0px solid #999999;
            float: left;
            height: 20px;
            overflow: hidden;
            padding: 4px 0.3%;
            width: 5%;
            text-align: right;
        }

        .rTable:after {
            visibility: hidden;
            display: block;
            font-size: 0;
            content: " ";
            clear: both;
            height: 0;
        }
    </style>
</head>
<body>

    <div>
        <h1>ESP8266 RGB Controller</h1>
        <label id="conn_text"></label><br /><br /><br />


        <!--
        <input type="text" id="input_text" />
        <input type="submit" id="button" value="Send" />
        <ul>
            <li>Item 1 <span class="right">(1)</span></li>
            <li>Item 2 <span class="right">(2)</span></li>
        </ul>
   -->
        <div class="rTable">
            <div class="rTableRow">
                <div class="rTableCell" style="width:70px;">
                    <label for="onschedule">Auto On : </label>
                </div>
                <div class="rTableCell" style="width:70px; vertical-align:top">
                    <div class="onoffswitch">
                        <input type="checkbox" name="Startswitch" class="onoffswitch-checkbox" id="MyStartSwitch">
                        <label class="onoffswitch-label" for="MyStartSwitch">
                            <span class="onoffswitch-inner"></span>
                            <span class="onoffswitch-switch"></span>
                        </label>
                    </div><br />
                </div>
                <div class="rTableCell" style="width:100px">
                    <input  id="on_time" type="time" hidden />
                </div>
                <div class="rTableCell" style="width:50px">
                    <input type="submit" id="btnstart" value="Send" hidden />
                </div>             
                <div class="rTableCell" style="width:50px">
                    <input type="submit" id="btnstartclear" value="Clear" hidden/>
                </div>                   
            </div>
            <div class="rTableRow">
                <div class="rTableCell" style="width:70px">
                    <label for="onschedule">Auto Off : </label>
                </div>
                <div class="rTableCell" style="width:70px">
                    <div class="onoffswitch">
                        <input type="checkbox" name="StopSwitch" class="onoffswitch-checkbox" id="MyStopSwitch">
                        <label class="onoffswitch-label" for="MyStopSwitch">
                            <span class="onoffswitch-inner"></span>
                            <span class="onoffswitch-switch"></span>
                        </label>
                    </div>
                </div>
                <div class="rTableCell" style="width:100px">
                    <input id="off_time" type="time" hidden />
                </div>
                <div class="rTableCell" style="width:50px" >
                    <input type="submit" id="btnstop" value="Send" hidden />
                </div>
                <div class="rTableCell" style="width:50px">
                    <input type="submit" id="btnstopclear" value="Clear" hidden/>
                </div>
            </div>

            <div class="rTableRow">

                <div class="rTableCell" style="width:70px">
                    <label id="oDevice">Led:</label>
                </div>
                <div class="rTableCell" style="width:70px">
                    <div class="onoffswitch">
                        <input type="checkbox" name="onoffswitch" class="onoffswitch-checkbox" id="myonoffswitch">
                        <label class="onoffswitch-label" for="myonoffswitch">
                            <span class="onoffswitch-inner"></span>
                            <span class="onoffswitch-switch"></span>
                        </label>
                    </div>

                </div>
                <div class="rTableCell" style="text-align:center">
                    <label id="oManual"></label>
                </div>

            </div>


        </div><br /><br />
     
        <label id="oRed">Red:</label>
        <span id="Rrange">0</span><br />
        <input id="RedRange" type="range" min="0" max="1023" value="0" step="5" onchange="showRedValue(this.value)" /><br />

        <label id="oGreen">Green:</label>
        <span id="Grange">0</span><br />
        <input id="GreenRange" type="range" min="0" max="1023" value="0" step="5" onchange="showGreenValue(this.value)" /><br />

        <label id="oGreen">Blue:</label>
        <span id="Brange">0</span><br />
        <input id="BlueRange" type="range" min="0" max="1023" value="0" step="5" onchange="showBlueValue(this.value)" /><br /><br /><br /><br />

    </div><br />
    <div>

        <textarea id="selectedcolor" cols="30" readonly style="resize:none;background-color:white;width:230px; height: 30px"></textarea><br />
        <textarea id="rgb" cols="30" readonly style="resize:none;background-color:white;width:230px; height: 25px; border:none"></textarea><br />
        <textarea id="rgbcolor" cols="30" readonly style="resize:none;background-color:white;width:230px; height: 25px; border:none"></textarea> <br />
        <textarea id="ClientSend" cols="30" readonly style="resize:none;background-color:white;width:230px; height: 25px; border:none"></textarea> <br />
        <div id="ResponseArea"></div><br />
    </div>


    <script>
        $(document).ready(function () { 
                       
            var ws = new WebSocket("ws://" + location.hostname + ":81/");      
            ws.onopen = function (evt) {
                var conn_status = document.getElementById('conn_text');
                conn_status.innerHTML = "Connection status: Connected!"
            };

            ws.onmessage = function (evt) {
                var newMessage = document.createElement('p');
                newMessage.textContent = "Server: " + evt.data + "\r\n At:" + timenow();
                document.getElementById("ResponseArea").appendChild(newMessage);
                ProcessMessage(evt.data)

            };

            ws.onclose = function (evt) {
                alert("Connection closed");
            };

            $("#RedRange").change(function (evt) {
                evt.preventDefault();
                var message = String.fromCharCode("34") + $("#RedRange").val() + ", " + $("#GreenRange").val() + ", " + $("#BlueRange").val() + String.fromCharCode("34") //$("#RedRange").val();;
                ws.send(message);
               
            });

            $("#GreenRange").change(function (evt) {
                evt.preventDefault();
                var message = String.fromCharCode("34") + $("#RedRange").val() + ", " + $("#GreenRange").val() + ", " + $("#BlueRange").val() + String.fromCharCode("34") //$("#GreenRange").val();;
                ws.send(message);
                
            });

            $("#BlueRange").change(function (evt) {
                evt.preventDefault();
                var message = String.fromCharCode("34") + $("#RedRange").val() + ", " + $("#GreenRange").val() + ", " + $("#BlueRange").val() + String.fromCharCode("34") //$("#BlueRange").val();;
                ws.send(message);
                
            });

            $("#myonoffswitch").change(function (evt) {
                evt.preventDefault();
                var message;
                if (document.getElementById("myonoffswitch").checked) {
                    message = "1";
                } else {
                    message = "0";
                }
                ws.send(message);
                
            });

            $("#MyStartSwitch").change(function (evt) {
                var TimeNow = new Date();
                evt.preventDefault();
                var message;
                if (document.getElementById("MyStartSwitch").checked) {              
                    document.getElementById("on_time").value = timenow();
                    document.getElementById("on_time").hidden = false;
                    document.getElementById("btnstart").hidden = false;
                    document.getElementById("btnstartclear").hidden = false;
                    message = "start@" + document.getElementById("on_time").value
                    
                } else {
                    document.getElementById("on_time").value = "";
                    document.getElementById("on_time").hidden = true;
                    document.getElementById("btnstart").hidden = true;
                    document.getElementById("btnstartclear").hidden = true;
                    message = "start@"
                }
                
            });

            $("#MyStopSwitch").change(function (evt) {
                var TimeNow = new Date();
                evt.preventDefault();
                var message;
                if (document.getElementById("MyStopSwitch").checked) {
                    document.getElementById("off_time").value = timenow();
                    document.getElementById("off_time").hidden = false;
                    document.getElementById("btnstop").hidden = false;
                    document.getElementById("btnstopclear").hidden = false;
                    message = "stop@" + document.getElementById("off_time").value                  
                    
                } else {
                    document.getElementById("off_time").value = "";
                    document.getElementById("off_time").hidden = true;
                    document.getElementById("btnstop").hidden = true;
                    document.getElementById("btnstopclear").hidden = true;
                    message = "stop@"
                }
                
            });

            $("#btnstart").click(function (evt) {              
                evt.preventDefault();
                var message;
                message = "start@" + document.getElementById("on_time").value;
               
                ws.send(message);               
            });

            $("#btnstop").click(function (evt) {
                evt.preventDefault();
                var message;
                message = "stop@" + document.getElementById("off_time").value;
               
                ws.send(message);
            });

            $("#btnstartclear").click(function (evt) {             
                evt.preventDefault();
                var message;
                document.getElementById("on_time").value = "";
                
            });

            $("#btnstopclear").click(function (evt) {
                evt.preventDefault();
                var message;
                document.getElementById("off_time").value = "";
                
            });

            //$(document).ready( function() {
               //$('#on_time').val(new Date().toDateInputValue());
            //});​

        });

       

    </script>

    <script type="text/javascript">
        
        //RGB-to-Hex Conversion
        function rgbToHex(R, G, B) { return toHex(R) + toHex(G) + toHex(B) }
        function toHex(n) {
            n = parseInt(n, 10);
            if (isNaN(n)) return "00";
            n = Math.max(0, Math.min(n, 255));
            return "0123456789ABCDEF".charAt((n - n % 16) / 16) + "0123456789ABCDEF".charAt(n % 16);
        }

        function setBgColorById(id, sColor) {
            var elem;
            if (document.getElementById) {
                if (elem = document.getElementById(id)) {
                    if (elem.style) {
                        elem.style.backgroundColor = sColor;
                        return 1;  // success
                    }
                }
            }
            return 0;  // failure
        }

        function timenow() {
            var now = new Date(),
            ampm = 'am',
            h = now.getHours(),
            m = now.getMinutes(),
            s = now.getSeconds();
            //if (h >= 12) {
            //    if (h > 12) h -= 12;
            //    ampm = 'pm';
            //}
            if (h < 10) h = '0' + h;
            if (m < 10) m = '0' + m;         
            if (s < 10) s = '0' + s;          
            //return now.toLocaleDateString() + ' ' + h + ':' + m + ':' + s + ' ' + ampm;
            return h + ':' + m
        }
        
        function ProcessMessage(oMsgvalue) {
            obj = JSON && JSON.parse(oMsgvalue) || $.parseJSON(oMsgvalue);
            ored = Math.ceil(obj.Red / 4);
            ogreen = Math.ceil(obj.Green / 4);
            oblue = Math.ceil(obj.Blue / 4);
            oled = obj.LED;
            var ostart = obj.Start;
            var ostop = obj.Stop;

            
            if (isNaN(ored) == false || isNaN(ogreen) == false || isNaN(oblue) == false) {
            
                document.getElementById("selectedcolor").style = "resize:none;background-color:" + "rgb(" + ored + "," + ogreen + "," + oblue + ")" + ";width:230px; height: 30px";
                document.getElementById("rgbcolor").innerHTML = "RGB:(" + ored + "," + ogreen + "," + oblue + ")";
                document.getElementById("rgb").innerHTML = "HEX: #" + rgbToHex(ored, ogreen, oblue);
                document.getElementById("Rrange").innerHTML = ored;
                document.getElementById("Grange").innerHTML = ogreen;
                document.getElementById("Brange").innerHTML = oblue;
                document.getElementById("RedRange").value = (ored * 4);
                document.getElementById("GreenRange").value = (ogreen * 4);
                document.getElementById("BlueRange").value = (oblue * 4);
                document.getElementById('ClientSend').value = "Client: " + "\"" + obj.Red + "," + obj.Green + "," + obj.Blue + "\"";
                if (ored == 0 && ogreen == 0 && oblue == 0) {
                    //alert("Off");
                    document.getElementById("myonoffswitch").checked = false;
                    document.getElementById('ClientSend').value = "Client: Off";
                } else {
                    //alert("On");
                    document.getElementById("myonoffswitch").checked = true;
                }

            } else if (isNaN(parseInt(ostart)) == false) {             
                if (obj.Start == "00:00") {

                } else {
                    //alert(obj.Start);
                    document.getElementById("on_time").value = obj.Start;
                    document.getElementById("on_time").hidden = false;
                    document.getElementById("btnstart").hidden = false;
                    document.getElementById("btnstartclear").hidden = false;
                    document.getElementById("MyStartSwitch").checked = true
                    
                }

            } else if (isNaN(parseInt(ostop)) == false) {               
                if (obj.Stop == "00:00") {

                } else {
                    //alert(obj.Stop);
                    document.getElementById("off_time").value = obj.Stop;
                    document.getElementById("off_time").hidden = false;
                    document.getElementById("btnstop").hidden = false;
                    document.getElementById("btnstopclear").hidden = false;
                    document.getElementById("MyStopSwitch").checked = true
                }
            } else if (oled == "Off") {
                document.getElementById("selectedcolor").style = "resize:none;background-color:rgb(0,0,0);width:230px; height: 30px";
                document.getElementById("rgbcolor").innerHTML = "RGB:(0,0,0)";
                document.getElementById("rgb").innerHTML = "HEX: #" + rgbToHex(0, 0, 0);
            }          
        }              
    </script>
</body>
</html>
)=====";

#endif
