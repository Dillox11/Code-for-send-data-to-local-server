#include<WiFi.h>
#include<HTTPClient.h>
#include <DHT11.h>
DHT11 dht11(2);

const char*ssid = "Manzi";
const char*password ="dillox12";

String serverName ="http://192.168.1.76/temp/temp.php";
void setup() {
Serial.begin(115200);
WiFi.begin(ssid,password);

Serial.print("connecting to wifi...");
while(WiFi.status() != WL_CONNECTED)
{
  delay(500);
  Serial.print(".");
}
Serial.println("\n connected to wifi");
}
void loop() {
float temperature=dht11.readTemperature();
float humidity=dht11.readHumidity();

if(isnan(temperature) || isnan(humidity))
{
 Serial.println("Failed to read from dht sensor!"); 
}
else
{
  sendToServer(temperature, humidity);
}
delay(10000);
}
void sendToServer(float temperature,float humidity)
{
  if(WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("content-Type","application/json");

    String jsonData = "{\"temperature\": " + String (temperature, 2) + ",\"humidity\": " + String(humidity, 2) + "}";

    int httpResponseCode = http.POST(jsonData);

    if(httpResponseCode > 0)
    {
      String response = http.getString();
      Serial.print("server Response: ");
      Serial.println(httpResponseCode);
    }
    http.end();
   }
     else
    {
      Serial.println("Wifi Disconnected");
    }
}
