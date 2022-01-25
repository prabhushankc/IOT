#define BLYNK_PRINT Serial
#define sensor A0

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
char auth[] =  "vVMlPq6PZx1cynZX2CRTdHi6yTMkhDnh";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "HashtagTheCafe";
char pass[] = "Hashtag@123";

#define DHTPIN D4          // What digital pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11     // DHT 11
#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

int gas, co2lvl;
int maxCO2 = 600;
int maxHum = 85;
int minHum = 30;

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  Serial.print(h);
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2500);
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
  
  if (h >= maxHum) {
    Serial.print("Humidity level is high, please follow safety procedure");
    Blynk.notify("Humidity level is high, please follow safety procedure");
  }

  if (h <= minHum) {
    Serial.print("Humidity level is high, please follow safety procedure");
    Blynk.notify("Humidity level is low, please follow safety procedure");
  }
}
int gasSensor() {

  gas = analogRead(sensor);
  co2lvl = gas - 84;
  co2lvl = map(co2lvl, 0, 1024, 400, 5000);
  Serial.print("CO2 level: ");
  Serial.print(co2lvl);
  Serial.print("  ppm");
  Serial.println();

  if (co2lvl >= maxCO2) {
    Serial.print("CO2 level is high, please follow safety procedure");
    Blynk.notify("C02 is high, please follow safety procedure");
  }
  return co2lvl;
}
void myTimerEvent() {
  Blynk.virtualWrite(V0, gasSensor());
  sendSensor();
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  dht.begin();

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run();
}
