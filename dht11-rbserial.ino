#include "DHT.h"

// ---- BEÁLLÍTÁSOK ----

// DHT11 DATA láb -> Arduino D2
#define DHTPIN 2

// Szenzor típusa
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);       // Soros kapcsolat Pi felé (USB-n)
  Serial.println("DHT11 start...");
  dht.begin();
}

void loop() {
  delay(2000);  // 2 mp-enként mérünk

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Celsius

  // Ha hiba történt a mérésnél:
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("ERROR");
    return;
  }

  // Embernek olvasható formátum:
  Serial.print("Homerseklet: ");
  Serial.print(temperature);
  Serial.print(" *C | Paratartalom: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Gépnek (Raspberry Pi Python scriptnek) könnyen feldolgozható formátum:
  Serial.print("T:");
  Serial.print(temperature, 1);
  Serial.print(";H:");
  Serial.println(humidity, 1);
}
