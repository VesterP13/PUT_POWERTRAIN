#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFiS3.h>
#include <string>

//WI-FI
const char* ssid = "ISSiN";
const char* pass = "issinoldlabo";
WiFiServer server(12345);

//CZUJNIKI 
#define TEMPERATURE_AMBIENT_PIN 8 // Pin czujnika temp otoczenia (DS18B20)
#define TEMPERATURE_BATERRY_PIN 9 //Pin czujnika temp akumulatora (DS18B20)

#define RPM_SENSOR1 18 // Pin, do którego podłączony jest czujnik TCRT5000 nr.1
#define RPM_SENSOR2 19 // Pin, do którego podłączony jest czujnik TCRT5000 nr.2
#define RPM_SENSOR3 20 // Pin, do którego podłączony jest czujnik TCRT5000 nr.3
#define RPM_SENSOR4 21 // Pin, do którego podłączony jest czujnik TCRT5000 nr.4

#define CURRENTSENSING A2 // Pin od natężenia
#define VOLTAGESENSING A3 // Pin od napięcia

volatile int rpm1interrupts; //ILOSC PRZERWAN
volatile int rpm2interrupts;
volatile int rpm3interrupts;
volatile int rpm4interrupts;
int rpm1, rpm2, rpm3, rpm4;
int loopcount = 0;
int loopDelay = 100;

OneWire oneWire(TEMPERATURE_AMBIENT_PIN);
DallasTemperature sensors(&oneWire);

void setup()
{
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
    delay(300);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  server.begin();
}

void rpm_fun1()
{
  rpm1interrupts = rpm1interrupts + 1;
}

void rpm_fun2()
{
  rpm2interrupts = rpm2interrupts + 1;
}

void rpm_fun3()
{
  rpm3interrupts = rpm3interrupts + 1;
}

void rpm_fun4()
{
  rpm4interrupts = rpm4interrupts + 1;
}

void SendToApp(WiFiClient client, int rpm1count, int rpm2count, int rpm3count, int rpm4count, float tempA, float tempB, int voltage, int current)
{
  client.print("*");
  client.print(rpm1count);
  client.print(";");
  client.print(rpm2count);
  client.print(";");
  client.print(rpm3count);
  client.print(";");
  client.print(rpm4count);
  client.print(";");
  client.print(tempA);
  client.print(";");
  client.print(tempB);
  client.print(";");
  client.print(voltage);
  client.print(";");
  client.print(current);
  client.println("#");
}

void PrintOnSerial(int rpm1count, int rpm2count, int rpm3count, int rpm4count, float tempA, float tempB, int voltage, int current)
{
  Serial.print("*");
  Serial.print(rpm1count);
  Serial.print(";");
  Serial.print(rpm2count);
  Serial.print(";");
  Serial.print(rpm3count);
  Serial.print(";");
  Serial.print(rpm4count);
  Serial.print(";");
  Serial.print(tempA);
  Serial.print(";");
  Serial.print(tempB);
  Serial.print(";");
  Serial.print(voltage);
  Serial.print(";");
  Serial.print(current);
  Serial.println("#");
}


void loop()
{
  //Serial.println(WiFi.localIP());

  WiFiClient client = server.available();
  if (client) {
    while (client.connected()) 
    {
      delay(loopDelay);

      //sensors.requestTemperatures();
      //float temp_ambient = sensors.getTempCByIndex(0);

    if (loopcount > 4)
      {
        loopcount = 0;
        rpm1 = (rpm1interrupts/(5*loopDelay)) * 60000;
        SendToApp(client, rpm1,1000,1000,1000,20,40,30,70);
        PrintOnSerial(rpm1,1000,1000,1000,20,40,30,70);
      }
    else
      {
        SendToApp(client, 1000,1000,1000,1000,20,40,30,70);
        PrintOnSerial(1000,1000,1000,1000,20,40,30,70);
      }
    loopcount++;
    }
  }
}
