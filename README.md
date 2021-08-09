# IRLib

A library written in C++ language to encode and decode IR codes of different Air Conditioners including Chigo, Aux and TCL

I added demo apps source code for Native Mobile Apps Android & IOS. I provided Web App demo as well. 


Examples to send commands over serial:

{ "cmd":"12", "temp":"23", "mode":"auto", "fan": "auto" }
{ "cmd":"23" }
{ "cmd":"32" }
{ "cmd":"43" }
{ "cmd":"54" }
{ "cmd":"12" }
{ "cmd": "54"}
Program will acknowledge the command by sending it back as received, and will report status in JSON, e.g:

{ "cmd": "12", "status": "ok"}
{"status": "fail", "error":"unknown command"}
