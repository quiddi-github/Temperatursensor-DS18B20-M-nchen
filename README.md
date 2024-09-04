"# Temperatursensor-DS18B20-München bei Steffi" 

Stand: 4.9.2024\
\
 *  Als Temperatursensor wird ein DS18B20 verwendet.\
 *  Das Signal wird per W-Lan an ThingSpeak gesendet\
 *  Als Board wird ein "AZ-Delivery Board mit ESP32-WROOM-32" verwendet.\
 *  Der Sensor DS18B20 ist an GPIO 4 angeschlossen (gelb: Data, schwarz: Masse, rot: +3,3V). Zwischen +3,3V und Data ist ein 4,7k Widerstand zu schalten. Ein Anschlussbild ist als PDF-Dokument beigefügt.\
 *  Thingspeak-Bibliothek (ThingSpeak von MathWorks) installieren.\
 *  DallasTemperature von Miles Burton installieren.\
 *  Als Board ein ESP32 Dev Module in der Arduino IDE auswählen. Ein Screenshot ist auch beigefügt.\
 *  W-Lan Password und SSID im Code eingeben.\
 *  ThingSpeak API Daten im Code eingeben.\
 *  Der Code sendet alle 6 Minuten an ThingSpeak und alle 6 Sekunden über den seriellen Monitor (115200 Baud) die Daten.\
 *  ThingSpeak Channel ID: 2645720, arminmuller@freenet.de (Es muss noch nach arminmuller die Ziffern 345 angehängt werden, bei der Mail-Adresse.)\
 *  Alle 30 Minuten wird in Field 1 gespeichert, alle 2h in Field 2.