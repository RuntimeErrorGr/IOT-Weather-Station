#include <DHTesp.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SFE_BMP180.h>
#include <Wire.h>
#include "index.h"

#define LED 2                     //LED
#define DHTpin 14                 //pinul D5 NodeMCU este GPIO14

SFE_BMP180 bmp180;

#define ALTITUDE 1655.0           //Altitudine (m)

DHTesp dht;

//SSID si parola router
const char * ssid = "DNA-Berceni, cartier cu valoare";
const char * password = "anchetamcoruptii";

ESP8266WebServer server(80);        //Server port 80

void handleRoot() {
  String s = MAIN_page;             //Citeste continutul HTML
  server.send(200, "text/html", s); //Trimite pagina web
}

float humidity, temperature;

void handleADC() {
  char status;
  double T, P, p0, a;
  double Tdeg, Tfar, phg, pmb;

  status = bmp180.startTemperature();
  if (status != 0) {
    delay(status);                    // Asteapta pentru masuratori
    status = bmp180.getTemperature(T);
    if (status != 0) {
      Serial.print("temperatura: ");  // Afiseaza masuratori
      Serial.print(T, 2);
      Tdeg = T;
      Serial.print(" deg C, ");
      

      status = bmp180.startPressure(3);
      if (status != 0) {
        delay(status);                              // Asteapta pentru masuratori
        status = bmp180.getPressure(P, T);
        if (status != 0) {
          Serial.print("presiunea atmosferica: ");  // Afiseaza masuratori
          Serial.print(P, 2);
          pmb = P;
          Serial.print(" mbar, ");
          phg = P * 0.750061683;
          Serial.print(P * 0.750061683, 2);
          Serial.println(" mmHg");

          p0 = bmp180.sealevel(P, ALTITUDE);
          Serial.print("Presiune relativa (fata de nivelul marii): ");
          Serial.print(p0, 2);
          Serial.print(" mbar, ");
          Serial.print(p0 * 0.750061683, 2);
          Serial.println(" mmHg");

          a = (bmp180.altitude(P, p0))/100;
          Serial.print("altitudine: ");
          Serial.print(a, 0);
          Serial.print("m, ");
        } else Serial.println("error retrieving pressure measurement\n");
      } else Serial.println("error starting pressure measurement\n");
    } else Serial.println("error retrieving temperature measurement\n");
  } else Serial.println("error starting temperature measurement\n");

  int rain = (1024 - analogRead(A0)) / 10;
  String message = "Cer senin";
  if (rain < 2) {
    if (pmb < 1010 && pmb >= 1005) {
      message = "Cer partial innorat";
    } else if (pmb < 1005) {
      message = "Cer innorat";
    }
  }
  if (rain >= 5 && rain <= 40) {
    message = "Ploaie usoara";
  } else if (rain > 40 && rain <= 50) {
    message = "Ploaie";
  } else if (rain > 50) {
    message = "Furtuna";
  }
  //Create JSON data
  String data = "{\"Rain\":\"" + String(rain) + "\",\"Altitude\":\"" + String(a) + "\",\"Message\":\"" + message + "\",\"Pressuremb\":\"" + String(pmb) + "\",\"Pressurehg\":\"" + String(phg) + "\", \"Temperature\":\"" + String(temperature) + "\", \"Humidity\":\"" + String(humidity) + "\"}";

  digitalWrite(LED, !digitalRead(LED)); //aprinde LED pt cereri
  server.send(200, "text/plane", data); //trimite ADC, temperatura, umiditate, altitudine, mesaj in JSON catre client

  delay(dht.getMinimumSamplingPeriod());

  humidity = dht.getHumidity();
  temperature = dht.getTemperature();

  Serial.print("Precipitatii:");
  Serial.print(rain);
  Serial.println(" %\n");
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  // dht22 Sensor
  dht.setup(DHTpin, DHTesp::DHT22); //for DHT22 Connect DHT sensor to GPIO 17
  pinMode(LED, OUTPUT);

  //BMP180 Sensor
  if (bmp180.begin())
    Serial.println("BMP180 init success");
  else {
    Serial.println("BMP180 init fail\n\n");
    while(1); // Pause forever.
  }

  WiFi.begin(ssid, password); 
  Serial.println("");

  // Asteapta pt conexiune la wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //Daca conexiunea a reusit, afiseaza datele retelei la care ne-am conectat
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());     //adresa IP locala

  server.on("/", handleRoot);         //afiseaza pagina
  server.on("/readADC", handleADC); 

  server.begin();                     //Start server
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();            //manevreaza cererile clientului
}
