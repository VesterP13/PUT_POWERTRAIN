#include <WiFiS3.h>
#include <string>
#include <OneWire.h>
#include <DallasTemperature.h>


#define TEMPERATURE_AMBIENT_PIN 8 // Pin czujnika DS18B20 nr.1
#define RPM_SENSOR1 18 // Pin, do którego podłączony jest czujnik TCRT5000 nr.1
#define RPM_SENSOR2 19 // Pin, do którego podłączony jest czujnik TCRT5000 nr.2
#define RPM_SENSOR3 20 // Pin, do którego podłączony jest czujnik TCRT5000 nr.3
#define RPM_SENSOR4 21 // Pin, do którego podłączony jest czujnik TCRT5000 nr.4

//#define TEMP_SENSOR1 11 // Pin czujnika DS18B20 nr.1
#define TEMP_SENSOR2 12 // Pin czujnika DS18B20 nr.2

#define CURRENTSENSING A2 // Pin od natężenia
#define VOLTAGESENSING A3 // Pin od napięcia

public volatile int rpm1count;
public volatile int rpm2count;
public volatile int rpm3count;
public volatile int rpm4count;
unsigned long rpm;
int rpm1, rpm2, rpm3, rpm4;

unsigned long previousMillis = 0;
const long interval = 1000;

const char* ssid = "ISSiN";
const char* pass = "issinoldlabo";

WiFiServer server(12345);

OneWire oneWire(TEMPERATURE_AMBIENT_PIN);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);

  pinMode(RPM_SENSOR1, INPUT);
  pinMode(RPM_SENSOR2, INPUT);
  pinMode(RPM_SENSOR3, INPUT);
  pinMode(RPM_SENSOR4, INPUT);

  attachInterrupt(digitalPinToInterrupt(RPM_SENSOR1), rpm_fun1, FALLING);
  attachInterrupt(digitalPinToInterrupt(RPM_SENSOR2), rpm_fun2, FALLING);
  attachInterrupt(digitalPinToInterrupt(RPM_SENSOR3), rpm_fun3, FALLING);
  attachInterrupt(digitalPinToInterrupt(RPM_SENSOR4), rpm_fun4, FALLING);
  sensors.begin();
  
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  server.begin();
}

void rpm_fun1(){
 &rpm1count = rpm1count + 1;
}
void rpm_fun2(){
 &rpm2count = rpm2count + 1;
}
void rpm_fun3(){
 &rpm3count = rpm3count + 1;
}
void rpm_fun4(){
 &rpm4count = rpm4count + 1;
}

void loop() {
  //Serial.println(WiFi.localIP());
  unsigned long currentMillis = millis();
  WiFiClient client = server.available();
  if (client) {
    while (client.connected()) {
      //Serial.print("Nowe połączenie");
      sensors.requestTemperatures();
      float temp_ambient = sensors.getTempCByIndex(0);
      if(currentMillis - previousMillis >= interval){
        previousMillis = currentMillis;
        &rpm1 = rpm1count * 30; // 60/2 bo minuta i 2 śmigła
        &rpm2 = rpm2count * 30;
        &rpm3 = rpm3count * 30;
        &rpm4 = rpm4count * 30;
      } 

      Serial.println("*1000;1100;1200;1300;20;30;10;20#");
      client.print("*");
      client.print(rpm1);
      client.print(";");
      client.print(rpm2);
      client.print(";");
      client.print(rpm3);
      client.print(";");
      client.print(rpm4);
      client.print(";");
      client.print(temp_ambient);
      client.println(";30;10;20#");
    }
    client.stop();
  }
}