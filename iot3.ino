#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

#define DHTPIN D4
#define DHTTYPE DHT11

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

DHT dht(DHTPIN, DHTTYPE);
ESP8266WebServer server(80);

String webpage() {

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  String html = R"====(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">

<title>DHT11 Monitoring System</title>

<style>

*{
margin:0;
padding:0;
box-sizing:border-box;
font-family:'Segoe UI',sans-serif;
}

body{
height:100vh;
display:flex;
justify-content:center;
align-items:center;
background:linear-gradient(135deg,#0d47a1,#1976d2,#42a5f5);
}

.container{
text-align:center;
width:90%;
max-width:700px;
}

.title{
color:white;
font-size:38px;
font-weight:bold;
margin-bottom:30px;
}

.cards{
display:flex;
justify-content:center;
gap:30px;
flex-wrap:wrap;
}

.card{
background:white;
padding:30px;
width:250px;
border-radius:20px;
box-shadow:0 10px 25px rgba(0,0,0,0.3);
transition:0.3s;
}

.card:hover{
transform:translateY(-5px);
}

.label{
font-size:24px;
font-weight:bold;
color:#1565c0;
margin-bottom:15px;
}

.value{
font-size:42px;
font-weight:bold;
color:#333;
}

.footer{
margin-top:25px;
color:white;
font-size:16px;
}

</style>

<meta http-equiv="refresh" content="5">

</head>

<body>

<div class="container">

<div class="title">
🌡️ DHT11 Weather Monitor
</div>

<div class="cards">

<div class="card">
<div class="label">Temperature</div>
<div class="value">
)====";

  html += String(t);
  html += R"====( °C</div></div>

<div class="card">
<div class="label">Humidity</div>
<div class="value">
)====";

  html += String(h);
  html += R"====( %</div></div>

</div>

<div class="footer">
ESP8266 Local Web Server | Auto Refresh Every 5 Seconds
</div>

</div>

</body>
</html>
)====";

  return html;
}

void handleRoot() {
  server.send(200, "text/html", webpage());
}

void setup() {

  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);

  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}


