//Stand 04.09.2024
//
// Code für AZ-Delivery Board mit ESP32-WROOM-32
//
// Code für München bei Steffi
//
// Arduino IDE v1
// Folgenden Link zur Boardverwaltung hinzufügen:
// https://dl.espressif.com/dl/package_esp32_index.json
// zu finden in der Arduino IDE v1 in Datei/ Voreinstellungen/ Einstellungen/ Zusätzliche Boardverwalter-URLs:
// In der Arduino IDE v1 über Werkzeuge/ Board/ Boardverwalter das Paket "esp32" von Espressif Systems installieren
// Als Board ESP32 Arduino/ ESP32 Dev Module auswählen
//
//
// Hardware
// Temperatursensor DS18B20 ist an Pin GPIO4 angeschlossen.
// Betriebsspannung des Sensors ist 3,3V und zwischen Data und +3,3V muss ein 4,7k Widerstand angeschlossen sein.
//
// ! ! ! Zum Schreiben des Programmes bitte den "Boot" Taster beim Übertragen drücken ! ! !
// Arduino wurde durch einen AZ-Delivery Board mit ESP32-WROOM-32 ersetzt.





#define LED    3                               // LED auf D1 am Arduino, Blinkkontrolle
#include <ThingSpeak.h>                         // ThingSpeak Bibliothek einbinden
#include "WiFi.h"
//#include <ESP8266WiFi.h>                        // W-Lan Bibliothek einbinden
#include <OneWire.h>                            // BUS-Bibliothek für Sensor einbinden
#include <DallasTemperature.h>                  // Dallas Sensor Bibliothek einbinden

#define ONE_WIRE_BUS 4                          // Temperatursensor an GPIO4 (war alt bzw. Pin D2)
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

unsigned long myChannelNumber = 2645720;         //ThingSpeak Channel ID
const char * myWriteAPIKey = "KOIHJDSEGSL95NX3";

const char* ssid = "Vodafone-ACCC";                    // W-Lan Passwörter
const char* password = "Hier PW eintragen";

WiFiClient  client;

WiFiServer server(80);

unsigned long AktuelleZeit=0;                   //aktuelle Zeit-Variable um delay nicht benutzen zu müssen
unsigned long LetzterLauf=0;
unsigned long LetzterSendelauf=0;
unsigned long LetzterLaufLED=0;
unsigned long LetzterReboot=0;
bool LetzterSendelaufbool = false;
bool Timer2h = false;

void(* resetFunc) (void) = 0; //0-Funktion Pointer für Reboot

void setup()  
 {  
  Serial.begin(115200);                         // Seriellen Port initialisieren 
  delay(10);
  pinMode(LED, OUTPUT);                         // Setzen des PINs als Ausgang für LED

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  int count = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    count++;
    if (count > 50){resetFunc();};
  }
  Serial.println("");
  Serial.println("WiFi connected");  

  // Print the IP address
  Serial.println(WiFi.localIP());

ThingSpeak.begin(client);
LetzterReboot = millis();
}  
  
void loop() {
  AktuelleZeit = millis();
  bool arbeiten = false;


// Anfang: Alle 6 Sekunden die Temperatur über den seriellen Monitpr ausgeben
if (AktuelleZeit > (LetzterLauf+6000)) {
  LetzterLauf = AktuelleZeit;
  arbeiten = true;
};
if (arbeiten == true) {
sensors.requestTemperatures();
 Serial.println("Temperatur ist: " + String(sensors.getTempCByIndex(0)) + "°C" + "     " + String(AktuelleZeit));
};
// Ende: Alle 6 Sekunden die Temperatur über den seriellen Monitpr ausgeben

// Anfang: Alle 6 Minuten die Temperatur an ThingSpeak senden
if (!LetzterSendelaufbool) {
sensors.requestTemperatures();
ThingSpeak.writeField(myChannelNumber, 1, sensors.getTempCByIndex(0) , myWriteAPIKey);
LetzterSendelaufbool = true;
LetzterSendelauf = AktuelleZeit;
 Serial.println("Temperatur gesendet an TS   " + String(AktuelleZeit));
 if (AktuelleZeit > (LetzterReboot + 7200000)) // alle 2h einen Reboot machen
{
LetzterSendelaufbool = false;
Timer2h = false;
resetFunc(); // Reboot
};
}

if ((!Timer2h) && (AktuelleZeit > (LetzterSendelauf + 60000))){
  //delay(5000);
ThingSpeak.writeField(myChannelNumber, 2, sensors.getTempCByIndex(0) , myWriteAPIKey); // alle 2h in Field 2 einen Datenpunkt speichern
 Serial.println("Temperatur 2 gesendet an TS   " + String(AktuelleZeit));
Timer2h = true;
}

if (AktuelleZeit > (LetzterSendelauf + 1800000)) {
  LetzterSendelaufbool = false;
}
// Ende: Alle 6 Minuten die Temperatur an ThingSpeak senden


// Anfang: LED-Blinken ohne delay()-Befehl
if (AktuelleZeit < (LetzterLaufLED+1000) ) {
  digitalWrite(LED, LOW);
};
if (AktuelleZeit > (LetzterLaufLED+1000)) {
  digitalWrite(LED, HIGH);
};
if (AktuelleZeit > (LetzterLaufLED+2000)) {
  LetzterLaufLED = AktuelleZeit;
};
// Ende: LED-Blinken ohne delay()-Befehl


}

// arminmuller@freenet.de (Es muss noch nach arminmuller die Ziffern 345 angehängt werden, bei der Mail-Adresse.)
// ThingSpeak45!
