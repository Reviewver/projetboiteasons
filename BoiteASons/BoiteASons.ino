/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include "DFRobotDFPlayerMini.h"

// Initialisation de la connection série avec le DFPlayer
HardwareSerial mySoftwareSerial(1);

// Initialisation du Dfplayer
DFRobotDFPlayerMini myDFPlayer;

// Régler le volume du Dfplayer
int volume = 1;

// Définition de la structure Balise
struct balise
{
  // Numéro de la balise
  int number;

  // Adresse mac de la balise
  std::string mac;

  // Activer si la balise n'a pas été trouvée
  bool notFound;
};

// Nombre de balise dans le tableau
const int balise_max = 2;

// Puissance maxi réception de zone
const unsigned int maximum_reception_beacon = -60;

// Tableau des adresses mac considéré comme étant des balises
const struct balise baliseList[balise_max] = {{1,"d5:f4:0d:a3:66:e4"},{2,"d7:02:10:a7:5d:e0"}};

// Nombre de balise détecter maximum
const int balise_max_detect = 2;

// Tableau des balise détecter
struct balise detect_balise_buffer[balise_max_detect] = {};

int scanTime = 5; //In seconds
BLEScan* pBLEScan;

// Vérifie que c'est une balise et retourne le numéro de la balise si elle est détecter sinon retourne -1
struct balise check_if_balise_and_return_number(BLEAdvertisedDevice device)
{
    for(int i = 0 ; i < balise_max ; i++)
    {
      // Si l'address mac de la balise est présent dans la liste
      if(device.getAddress().toString() == baliseList[i].mac)
      {
        return baliseList[i];        
      }
    }
    struct balise balise;
    balise.notFound = true;

    return balise;
}

// Vérifie que la balise n'est pas déjà présente dans le tableau des détection
bool balise_is_detect_array(struct balise detect_balise)
{
  for(int i = 0 ; i < balise_max_detect ; i++)
  {
      if((detect_balise.number == detect_balise_buffer[i].number) && (detect_balise.mac == detect_balise_buffer[i].mac))
      {
        return true;
      }
  }

  return false;
}

// Ajoute la balise dans le tableau des balise détecter
bool add_balise_detect_buffer(struct balise detect_balise)
{
    for(int i = 0 ; i < balise_max_detect ; i++)
    {
      if(detect_balise_buffer[i].number==0)
      {
        detect_balise_buffer[i] = detect_balise;
        return true;
      }
    }
    return false;
}
bool check_if_balise_is_in_zone(BLEAdvertisedDevice advertisedDevice)
{
      if(advertisedDevice.getRSSI() > maximum_reception_beacon)
      {
        return 1;
      }
      return 0;
}

// Parcours le tableau
void display_detect_buffer()
{
    for(int i = 0 ; i < balise_max_detect ; i++)
    {
      Serial.printf(detect_balise_buffer[i].mac.c_str());
    }
}

// Calcul la taille du buffer
int return_size_detect_buffer()
{
  int size_buffer_detect = 0;
  for(int i = 0 ; i < balise_max_detect ; i++)
  {
    if(detect_balise_buffer[i].number!=0)
    {
      size_buffer_detect++;
    }
  }

  return size_buffer_detect;
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.printf("Périphérique : %s \n", advertisedDevice.toString().c_str());
      Serial.printf("Puissance : %d\n", advertisedDevice.getRSSI());
      struct balise baliseDetect = check_if_balise_and_return_number(advertisedDevice);
      if(!baliseDetect.notFound)
      {
        Serial.printf("Balise détécter numéro => %d", baliseDetect.number);
        if(!balise_is_detect_array(baliseDetect))
        {    
          Serial.printf("La balise n'est pas dans le tableau");
          if(add_balise_detect_buffer(baliseDetect))
          {
            Serial.printf("La balise à été ajoutée avec succès");
          }
          else
          {
            Serial.printf("Le buffer de détection est plein");
          }
        }

        Serial.printf("\nDétection : %d\n", maximum_reception_beacon);
        if(check_if_balise_is_in_zone(advertisedDevice))
        {
          Serial.printf("\nLa balise est dans la zone\n");

          // Si le Dfplayer est disponible
          if (myDFPlayer.available()) {
            uint8_t type = myDFPlayer.readType();
            
            if(type == DFPlayerPlayFinished){
                // Joue la première chansons
                myDFPlayer.play(1);
            }
          }
        }
      }
      
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


  // Initialisation de la connection sur les port 16 et 17
  mySoftwareSerial.begin(9600, SERIAL_8N1, 16, 17);  // speed, type, RX, TX

  // Vérification que le DFPlayer est connecté
  while(!myDFPlayer.begin(mySoftwareSerial))
  {
    Serial.println(F("Impossible de se connecter"));
  }
  Serial.println(F("Initialisation du DFPlayer...."));

  // Fixe le volume du dfplayer
  myDFPlayer.volume(volume);
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  // Lit les donnée depuis la carte SD
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

}

void loop() {
  // Lance le scanner
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.println("Recherche terminé !");
  Serial.printf("Taille buffer : %d", return_size_detect_buffer());
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(2000);
}
