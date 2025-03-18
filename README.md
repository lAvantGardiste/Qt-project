# Encodage de message dans un parachute

Ce projet est une application Qt permettant d'encoder des messages dans un motif de parachute, inspiré par le système utilisé pour le rover Perseverance de la NASA.

## Fonctionnalités

- Encodage de messages ASCII en motifs binaires
- Visualisation du parachute avec les motifs encodés
- Interface graphique interactive avec contrôles de personnalisation
- Vue binaire et vue parachute
- Support multilingue
- Sauvegarde et chargement des paramètres

## Prérequis

- Qt 6.x
- CMake 3.16 ou supérieur
- Compilateur C++ supportant C++17

## Installation

1. Cloner le dépôt
2. Créer un dossier build :
   ```bash
   mkdir build
   cd build
   ```
3. Configurer avec CMake :
   ```bash
   cmake ..
   ```
4. Compiler :
   ```bash
   cmake --build .
   ```

## Structure du projet

- `src/` : Code source
  - `model/` : Classes de données et logique métier
  - `view/` : Composants d'interface utilisateur
- `resources/` : Ressources (traductions, images, etc.)

## Licence

Ce projet est créé dans le cadre d'un TP universitaire à l'ENSICAEN. 