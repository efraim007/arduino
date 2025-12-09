#include "DHT.h"

#define DHTPIN 2     // DHT11 DATA láb -> D2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("DHT11 indul...");
  dht.begin();
}

void loop() {
  delay(2000);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Celsius

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Hiba: nem sikerült adatot olvasni!");
    return;
  }

  Serial.print("Homerseklet: ");
  Serial.print(temperature);
  Serial.print(" *C | Paratartalom: ");
  Serial.print(humidity);
  Serial.println(" %");
}
