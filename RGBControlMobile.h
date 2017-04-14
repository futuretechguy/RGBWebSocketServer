#ifndef PAGE_RGBControlMobil_H
#define PAGE_RGBControlMobil_H

const char PAGE_RGBControlMobile[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>

    <meta charset="utf-8">
    <meta name="apple-mobile-web-app-capable" content="yes" />
    <meta http-equiv="X-UA-Compatible\" content="IE =edge,chrome =1">
    <meta name="viewport" content="width =device-width, user-scalable=no">

    <title>RGB Control</title>
    <!--
    <script src='spectrum.js'></script>
    <link rel='stylesheet' href='spectrum.css' />
     -->
    <script src="http://code.jquery.com/jquery-1.6.4.min.js"></script>
    <script src="http://code.jquery.com/mobile/1.0/jquery.mobile-1.0.min.js"></script>

    <link rel="stylesheet" href="http://code.jquery.com/mobile/1.0/jquery.mobile-1.0.min.css" />

    <style>
        .rTable {
            display: block;
            width: 100%;
        }

        .rTableCell, .rTableHead {
            border: 0px solid #999999;
            float: left;
            height: 20%;
            overflow: hidden;
            padding: 1px 0.3% 1px;
            width: 98%;
            text-align: center;
        }

        .rTableRow {
            width: 100%;
            height: 20%;
            padding: 4px;
        }

        div.row1, div.row2 {
            display: inline-block;
            float: left;
            width: 105px;
            padding: 2px 0.1%;
            text-align: center;
        }

        div.lrow1, div.lrow2 {
            display: inline-block;
            float: left;
            text-align: left;
            padding: 5px 0.6%;
        }

        div.srow1, div.srow2 {
            display: inline-block;
            float: left;
            text-align: left;
            padding: 1px 1% -10% 1%;
            margin-top: -9px;
            width: 100px;
            height: 100%;
        }
    </style>


</head>
<body>

    <script type="text/javascript">
        $(document).ready(function () {
            var ws = new WebSocket("ws://" + location.hostname + ":81/");  

            ws.onopen = function (evt) {
                var conn_status = document.getElementById('conn_text');
                conn_status.innerHTML = "status: Connected!"
            };

            ws.onmessage = function (evt) {
                var newMessage = document.createElement('p');
                newMessage.textContent = "Server: " + evt.data + "\r\n At:" + timenow();
                ProcessMessage(evt.data)

            };

            ws.onclose = function (evt) {
                alert("Connection closed");
            };

            $("#RedRange").change(function (evt) {
                evt.preventDefault();
                var message = String.fromCharCode("34") + ($("#RedRange").val()) * 4 + ", " + ($("#GreenRange").val()) * 4 + ", " + ($("#BlueRange").val()) * 4 + String.fromCharCode("34")
                //ws.send(message);

            });

            $("#GreenRange").change(function (evt) {
                evt.preventDefault();
                var message = String.fromCharCode("34") + ($("#RedRange").val) * 4 + ", " + $("#GreenRange").val * 4 + ", " + $("#BlueRange").val * 4 + String.fromCharCode("34");
                //ws.send(message);

            });

            $("#BlueRange").change(function (evt) {
                evt.preventDefault();
                var message = String.fromCharCode("34") + ($("#RedRange").val) * 4 + ", " + ($("#GreenRange").val) * 4 + ", " + ($("#BlueRange").val) * 4 + String.fromCharCode("34");
                //ws.send(message);

            });

            $("#btnsetcolor").click(function (evt) {
                evt.preventDefault();
                //document.getElementById("conn_text").innerHTML = "RGB Controller";
                var message = String.fromCharCode("34") + $("#RedRange").val() * 4 + ", " + $("#GreenRange").val() * 4 + ", " + $("#BlueRange").val() * 4 + String.fromCharCode("34");
                ws.send(message);

            });

            $("#btnsetstart").click(function (evt) {
                evt.preventDefault();
                var message;
                if (document.getElementById("on_time").value != "") {
                    message = "start@" + document.getElementById("on_time").value;
                } else {
                    message = "start@";
                }
                ws.send(message);

            });


            $("#btnsetstop").click(function (evt) {
                evt.preventDefault();              
                var message;
                if (document.getElementById("off_time").value != "") {
                    message = "stop@" + document.getElementById("off_time").value;
                } else {
                    message = "stop@";
                }
                ws.send(message);

            });


            $("#flip-Switch").unbind("change").change(function (evt) {
                evt.preventDefault();
                var message;
                if (document.getElementById("flip-Switch").value == "on") {
                    message = "1";
                } else if (document.getElementById("flip-Switch").value == "off") {
                    message = "0";
                };
                ws.send(message);

            });           

        });

        


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
            if (h < 10) h = '0' + h;
            if (m < 10) m = '0' + m;
            if (s < 10) s = '0' + s;
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
                $('#RedRange').val(ored).slider("refresh");
                $('#GreenRange').val(ogreen).slider("refresh");
                $('#BlueRange').val(oblue).slider("refresh");

                if (ored == 0 && ogreen == 0 && oblue == 0) {
                    $('#flip-Switch').val('off').slider("refresh");
                    //document.getElementById("txtrgb").innerHTML = "RGB(0,0,0)";
                    $("#selectedcolor").css("background-color", "#" + rgbToHex(0, 0, 0));
                    //document.getElementById("selectedcolor").style = "background-color:rgb(0,0,0)";


                } else {
                    $('#flip-Switch').val('on').slider("refresh");
                    //document.getElementById("txtrgb").innerHTML = "RGB(" + ored + "," + ogreen + "," + oblue + ")";
                    $("#selectedcolor").css("background-color", "#" + rgbToHex(ored, ogreen, oblue));
                }

            } else if (isNaN(parseInt(ostart)) == false) {

                if (obj.Start == "00:00") {
                    document.getElementById("on_time").value = "";
                } else {
                    //alert(obj.Start);
                    document.getElementById("on_time").value = obj.Start;
                }

            } else if (isNaN(parseInt(ostop)) == false) {

                if (obj.Stop == "00:00") {
                    document.getElementById("off_time").value = obj.Stop;
                } else {
                    //alert(obj.Stop);
                    document.getElementById("off_time").value = obj.Stop;
                }

            } else if (oled == "Off") {
                //alert("off")
                //document.getElementById("txtrgb").innerHTML = "RGB(0,0,0)";
                //document.getElementById("selectedcolor").style = "background-color:rgb(0,0,0)";
                $("#selectedcolor").css("background-color", "#" + rgbToHex(0, 0, 0));
                $('#RedRange').val(0).slider("refresh");
                $('#GreenRange').val(0).slider("refresh");
                $('#BlueRange').val(0).slider("refresh");

            }
        }

    </script>

    
    <Div class="rTable">
        <div class="rTableRow">
            <div class="rTableCell">
                <label id="conn_text">RGB Client</label>
            </div>
            <div class="rTableCell">
            </div>
            <div class="rTableCell" style="width:100%" />
            <svg id="selectedcolor" height="20" width="250" style="background-color:black">
                <text id="txtrgb" x="65" y="16" fill="black"></text>
            </svg>
            <div class="rTableCell">
                <input id="btnsetcolor" type="submit" value="Set Color" style="width:100%" />
            </div>
            <div class="rTableCell">
                <input id="RedRange" type="range" min="0" max="256" step="5" style="background:red" />
            </div>
            <div class="rTableCell">
                <input id="GreenRange" type="range" min="0" max="256" step="5" style="background:green" />
            </div>
            <div class="rTableCell">
                <input id="BlueRange" type="range" min="0" max="256" step="5" style="background:blue" />
            </div>
        </div>
        <div class="rTableRow">
            <div class="rTableCell" style="width:100%; align-items:center">
                <br />
                <div id="divflip" class="containing-element" style="width:70px; padding-bottom:2%; padding-left:25%">
                    <label for="flip-min"></label>
                    <select name="flip-min" id="flip-Switch" data-role="slider">
                        <option value="off">Off</option>
                        <option value="on">On</option>
                    </select>
                </div>
                <br />
            </div>
            <div class="lrow1">
                <label style="vertical-align:sub; padding-right:1px">Start:</label>
            </div>
            <div class="row1">
                <input id="on_time" type="time" Style="height:20px; width:90px;" />
            </div>
            <div class="srow1">
                <input id="btnsetstart" type="submit" value="Set" style="width:100%;" />
            </div>
        </div>
        <div class="rTableRow">
            <div class="rTableCell">
            </div>
            <div class="lrow1">
                <label style="vertical-align:sub; padding-left:0.3%">Stop:</label>
            </div>
            <div class="row2">
                <input id="off_time" type="time" Style="height:20px; width:90px;" />
            </div>
            <div class="srow2">
                <input id="btnsetstop" type="submit" value="Set" style="width:100%;" />
            </div>
        </div>
    </Div>
</body>
</html>
)=====";
#endif
