#include <WiFiS3.h>
#include <string>

const char* ssid = "INEA-6414_2.4G";
const char* pass = "FcZZ5DyS";
WiFiServer server(12345);

int speed_motor1 = 1000, speed_motor2 = 1000, speed_motor3 = 1000, speed_motor4 = 1000;

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
      Serial.print(speed_motor1);
      Serial.print(speed_motor2);
      Serial.print(speed_motor3);
      Serial.println(speed_motor4);
      if (client.available()) {
        String message = client.readStringUntil('#');
        if (message.substring(0,1) == "1"){
          speed_motor1 = message.substring(1,5).toInt();
        }
        if (message.substring(0,1) == "2"){
          speed_motor2 = message.substring(1,5).toInt();
        }
        if (message.substring(0,1) == "3"){
          speed_motor3 = message.substring(1,5).toInt();
        }
        if (message.substring(0,1) == "4"){
          speed_motor4 = message.substring(1,5).toInt();
        }
        if (message.substring(0,1) == "9"){
          speed_motor1 = message.substring(1,5).toInt();
          speed_motor2 = message.substring(1,5).toInt();
          speed_motor3 = message.substring(1,5).toInt();
          speed_motor4 = message.substring(1,5).toInt();
        }
      }
    }
    client.stop();
  }
}
