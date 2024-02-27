#include <WiFiS3.h>
#include <string>

const char* ssid = "INEA-6414_2.4G";
const char* pass = "FcZZ5DyS";
WiFiServer server(12345);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  server.begin();
}

void loop() {
  //Serial.println(WiFi.localIP());
  WiFiClient client = server.available();
  if (client) {
    while (client.connected()) {
      //Serial.print("Nowe połączenie");
      Serial.println("*1000;1100;1200;1300;20;30;10;20#");
      client.print("*");
      client.print(speed_motor1);
      client.println(";1100;1200;1300;20;30;10;20#");
    }
    client.stop();
  }
}
