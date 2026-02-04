#include <SPI.h>
#include <Ethernet.h>

// Ethernet beállítások
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress fallbackIp(192, 168, 108, 189);
EthernetServer server(80);

// API adat változók
float lastTemp = 0.0;
float lastHum  = 0.0;
unsigned long startTime;

void setup() {
  Serial.begin(9600);
  startTime = millis();

  Serial.println("Ethernet HTTP API indul...");

  if (Ethernet.begin(mac) == 0) {
    Serial.println("DHCP hiba – fix IP hasznalata");
    Ethernet.begin(mac, fallbackIp);
  }

  delay(1000);
  Serial.print("Arduino IP: ");
  Serial.println(Ethernet.localIP());

  server.begin();

  // Random seed
  randomSeed(analogRead(0));
}

void loop() {

  // ===== API TESZT ADATOK (SZIMULÁCIÓ) =====
  lastTemp = random(180, 300) / 10.0;  // 18.0–30.0 °C
  lastHum  = random(300, 700) / 10.0;  // 30–70 %
  // =========================================

  EthernetClient client = server.available();

  if (client) {

    String request = "";

    while (client.available()) {
      char c = client.read();
      request += c;
      if (c == '\n') break;
    }

    Serial.print("Keres: ");
    Serial.println(request);

    unsigned long uptimeSec = (millis() - startTime) / 1000;

    // ===== API ROUTE =====
    if (request.indexOf("GET /dht11") >= 0) {

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

      Serial.println("API JSON elkuldve");

    } else {

      client.println("HTTP/1.1 404 Not Found");
      client.println("Connection: close");
      client.println();

      Serial.println("404 – ismeretlen endpoint");
    }

    delay(1);
    client.stop();
  }

  delay(500);
}
