# Projet Boite à Sons

# Présentation

Le but est de réaliser un audioguide qui déclenche un sons en fonction de la balise détécter.

# Configuration du projet 

## Installation de l'éditeur

Vous devez au préalable installer l'éditeur Arduino.

Pour installe l'IDE Arduino sous Debian GNU/Linux, ouvrer un terminale, loguer vous en root ::

  $ su

Puis installer le paquet Arduino avec l'utilitaire apt via la commande ::

  # apt-get intall arduino
  
## Configuration de l'éditeur

Rendez-vous dans le menu : Fichier -> Préférence.

Ajouter dans URL de gestionnaire de cartes supplémentaires :

	https://dl.espressif.com/dl/package_esp32_index.json
	
Puis dans le menu : Outils -> Types de Carte -> Gestionnaire de carte 

Chercher esp32.

Puis installer le packet esp32 by Espressif Systems en dernière version.

Vous pouvez ensuite ouvrir le fichier Arduino.
