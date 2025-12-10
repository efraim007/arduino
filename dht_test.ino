#include <DHT.h>

#define DHTPIN 2        // DATA láb pin száma
#define DHTTYPE DHT11   // Szenzor típusa

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("DHT11 Teszt kezdés...");
  dht.begin();
}

void loop() {
  delay(2000);  // 2 mp várakozás
  
  float paratartalom = dht.readHumidity();
  float homerseklet = dht.readTemperature();
  
  if (isnan(paratartalom) || isnan(homerseklet)) {
    Serial.println("Hiba! Ellenőrizd a bekötést!");
    return;
  }
  
  Serial.print("Páratartalom: ");
  Serial.print(paratartalom);
  Serial.print(" %\tHőmérséklet: ");
  Serial.print(homerseklet);
  Serial.println(" °C");
}
