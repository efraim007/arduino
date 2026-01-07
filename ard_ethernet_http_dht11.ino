#include <SPI.h>
#include <Ethernet.h>
#include <DHT.h>

#define DHTPIN 8
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// Ethernet
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
EthernetServer server(80);

float lastTemp = 0;
float lastHum = 0;
unsigned long startTime;

void setup() {

  Serial.begin(9600);
  dht.begin();
  startTime = millis();

  Serial.println("Ethernet inditasa /dht11 endpointtal...");

  if (Ethernet.begin(mac) == 0) {
    Serial.println("DHCP hiba – fix IP: 192.168.0.100");
    Ethernet.begin(mac, IPAddress(192,168,0,100));
  }

  delay(1000);

  Serial.print("Arduino IP: ");
  Serial.println(Ethernet.localIP());

  server.begin();
}

void loop() {

  // Szenzor olvasás
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (!isnan(h) && !isnan(t)) {
    lastTemp = t;
    lastHum = h;
  }

  EthernetClient client = server.available();

  if (client) {

    String request = "";

    // HTTP kérés első sorának beolvasása
    while (client.available()) {
      char c = client.read();
      request += c;

      // csak az első sor érdekel
      if (c == '\n') break;
    }

    Serial.print("Keres: ");
    Serial.println(request);

    unsigned long uptimeSec = (millis() - startTime) / 1000;

    // ---- URL ELLENORZES ----
    if (request.indexOf("GET /dht11") >= 0) {

      // JSON válasz
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: application/json");
      client.println("Connection: close");
      client.println();

      client.print("{\"temperature\":");
      client.print(lastTemp);
      client.print(",\"humidity\":");
      client.print(lastHum);

      client.print(",\"uptime\":");
      client.print(uptimeSec);

      client.println("}");

      Serial.println("/dht11 JSON elkuldve");

    } else {
      // ha mas URL -> 404
      client.println("HTTP/1.1 404 Not Found");
      client.println("Connection: close");
      client.println();

      Serial.println("404 – nem /dht11 volt");
    }

    delay(1);
    client.stop();
  }

  delay(200);
}