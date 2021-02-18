/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

int scanTime = 5; //In seconds
BLEScan* pBLEScan;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.printf("Périphérique : %s \n", advertisedDevice.toString().c_str());
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Recherche...");

  // Initialisation du Bluetooth
  BLEDevice::init("");
  // Création du scanner
  pBLEScan = BLEDevice::getScan();
  // Paramètre la fonction de callback si un périphérique bluetooth est détecter
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  // Active le mode scan qui utilise plus de puissance mais récupère plus rapidement
  pBLEScan->setActiveScan(true);
  
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
}

// Filtrer 
void filtreBalise(BLEScanResults foundDevices)
{
    Serial.print("Nombre de périphérique trouvés : ");
    Serial.println(foundDevices.getCount());
    BLEAdvertisedDevice device = foundDevices.getDevice(0);
    Serial.printf(device.toString().c_str());
    if(device.toString() == "RDL51822")
    {
      Serial.println("Balise");
    }
}
void loop() {
  // Lance le scanner
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.println("Recherche terminé !");
  filtreBalise(foundDevices);
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(2000);
}
