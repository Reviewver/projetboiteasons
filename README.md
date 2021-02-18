# Projet Boite à Sons

# Présentation

Le but est de réaliser un audioguide qui déclenche un sons en fonction de la balise détécter.

# Configuration du projet 

## Installation de l'éditeur

Vous devez au préalable installer l'éditeur Arduino.

Pour installe l'IDE Arduino sous Debian GNU/Linux, ouvrer un terminale, loguer vous en root ::

  su

Puis installer le paquet Arduino avec l'utilitaire apt via la commande ::

  apt-get intall arduino
  
## Configuration de l'éditeur

Rendez-vous dans le menu : Fichier -> Préférence.

Ajouter dans URL de gestionnaire de cartes supplémentaires :

	https://dl.espressif.com/dl/package_esp32_index.json
	
Puis dans le menu : Outils -> Types de Carte -> Gestionnaire de carte 

Chercher esp32.

Puis installer le packet esp32 by Espressif Systems en dernière version.

Vous pouvez ensuite ouvrir le fichier Arduino.

## Compilation

Avant de compiler sélectionner : Outils -> Types de Carte -> ESP32 Arduino -> ESP32 Dev Module.

Puis ensuite cliquer sur "Téléverser".

Si vous avez l'erreur :

“Failed to connect to ESP32: Timed out waiting for packet header”

il faut maintenir enfoncé le bouton « BOOT/FLASH » de votre carte ESP32 tout en téléversant en même temps.

Vous pouvez voir les infos que la carte envoie sur le port série en sélectionnant dans le menu : Outils -> Moniteur Série. Puis sélectionner 115200 baud en vitesse et Pas de fin de Ligne.

Si vous avez une erreur affichée par Java à propos du port série c'est due à java-8-openjdk/jre il faut le remplacer par java-11-openjdk en version de Java par défaut. 
