# 2M3 Multiplayer Lab (FR)
**TP compatible avec la version 1.1_Beta_Lab**

## Introduction

L'objectif de ce TP est d'observer différentes problématiques rencontrées dans le développement de jeux multijoueur et d'y apporter des solutions.

## Installation

Allez sur https://github.com/simatic/MultiplayerLab et téléchargez la dernière version du TP dans la section "Releases".

Extrayez l'archive dans le dossier de votre choix et suivez les instructions d'installation en fonction de votre système.

### Windows avec Visual Studio 2019 et MSVC2019

Ouvrez Visual Studio 2019 et séléctionnez `Continuer sans code` dans la fenêtre d'accueil.

Allez ensuite dans `Fichier > Ouvrir > CMake`et naviguez dans le dossier du TP. Sélectionnez le fichier `MultiplayerLab/2M3/CMakeLists.txt` et cliquez sur `Ouvrir`.

## Structure du programme

Ce TP est basé sur un jeu de tir en 1v1. Vous contrôlez une voiture et le bit est de tuer votre adversaire.

Ce projet est divisé en trois sous-dossiers :
- Le dossier `Client`contient tout le code côté client, c'est-à-dire le code qui serait exécuté sur votre machine personnelle si vous achetiez un jeu multijoueur.
- Le dossier `Server`contient le code côté serveur, c'est-à-dire ce qui serait exécuté sur des machines distantes dans le cas d'applications réelles.
- Le dossier `Common`contient du code utilisé à la fois par le serveur et le client. Notez bien que l'application se comporte comme si le serveur et le client étaient complètement séparés et connectés uniquement via le réseau, mais du code comme le moteur de jeu est commun au deux et se trouve donc dans ce dossier.

Le moteur utilisé pour ce jeu est composé de trois éléments :
- Les `Components`sont des structures C++ qui contiennent des données. Par exemple, le composant `Transform`contient un `sf::Vector2f`pour stocker la position et un `float` pour la rotation.
- Les `Entities`sont l'équivalent des GameObject dans Unity. Elles ont un ID unique et une liste de composants, selon leur fonction.
- Les `Systems` gèrent la logique du jeu et agissent sur les composants des entités. Par exemple, le `MovementSystem` cherche la vitesse dans le composant `Kinematics`et met à jour le composant `Transform`. De fait, ce système ne peut fonctionner **que** sur les entités qui **ont** les composants `Transform`et `Kinematics`. Le `GameManager` réalise des vérifications intelligents pour s'assurer de cette propriété.

Les systèmes sont les classes que vous allez manipuler le plus, pour envoyer des données du client vers le serveur et vice-versa. Ils fonctionnent de la même manière que les scripts `MonoBehaviour`de Unity, avec une méthode `start()` (éventuellement) et `update()`.

## Premier lancement

Compilez puis lancez l'application. Cela peut durer un moment mais devrait être plus rapide par la suite, puisque le compilateur ne régénérera que les fichiers modifiés.

Vous devriez avoir la vue suivante :
![Application au premier lancement](../Images/First_Launch.png)

A droite, encadré de rouge, c'est la vue du client (joueur) 1. A gauche, en vert, celle du client (joueur) 2.

Vous ne pouvez pas encore les faire bouger. Ce sera l'objet de la partie suivante. En bas, vous pouvez observer les graphes de communication. 

Le graphe de gauche indique les paquets entrant (des clients vers le serveur) et celui de droite des paquets sortants (du serveur vers les clients). 

Enfin, vous pouvez modifier la simulation réseau en ajoutant des pertes de paquet et de la latence en 4 points de communication :
- Du client 1 vers le serveur
- Du client 2 vers le serveur
- Du serveur vers le client 1
- Du serveur vers le client 2

## Premiers pas avec le moteur de jeu

Pour bien comprendre le fonctionnement du moteur de jeu, nous vous proposons d'implémenter le `MovementSystem` qui met à jour les position des entités selon leur vitesse.

Ouvrez le `MovementSystem`dans `Source/Common/Systems/MovementSystem.cpp`. Vous devriez voir ceci :

```
MovementSystem::MovementSystem(GameManager* const gameManager) :
	LogicSystem<Transform, Kinematics>(gameManager)
{}

void MovementSystem::update(const sf::Time& dt) {
	// TODO: Update entities' positions
}
```

La première méthode est le constructeur du système. Comme vous pouvez le voir, il demande un pointeur vers le `GameManager`. Notez également que c'est un système logic `LogicSystem` qui agit uniquement sur les entités qui ont les composants `Transform`et `Kinematics`.

La deuxième méthode est la méthode `update`, appelée à chaque frame. Chaque système a la liste des entités sur lesquelles il peut faire des opérations. Cette liste est stockée dans la variable membre `entities`.

To-Do :
- Pour chaque entité dans `entities`, mettez à jour la position de l'entité avec la formule `position += velocity * dt`.
- Allez voir les déclarations des composants `Transform`et `Kinematics` dans `Include/Common/Components`pour voir ce qu'ils contiennent.
- Vous pouvez obtenir les composants des entités avec `entity->getComponent<...>();`.

**Rappels C++**
La méthode `getComponent<...>` utilise les templates. Il s'agit de mettre le **type** du composant que vous souhaitez obtenir à l'intérieur des chevrons, c'est-à-dire ici `Transform` ou `Kinematics`.

Une fois la méthode `update()` complétée, compilez et lancez le programme. Attendez quelques instants que le serveur se déconnecte pour éviter qu'il ne force les position des joueurs à zéro. Vous pouvez aussi augmenter la perte de paquets au maximum. Vous devriez maintenant être en mesure de faire bouger les véhicules avec les touches `ZQSD` et `haut/bas/gauche/droite`.

## Répliquer les mouvements

### Observation

Pour le moment, les mouvement des véhicules ne sont pas répliqués sur le client en face. Vous pouvez observer, en lançant l'application, que le graphe des paquets sortant est vide. C'est parce que, pour le moment, nous n'avons pas dit au serveur d'envoyer la position et la vitesse des entités au client.

### Théorie

La plupart des jeux multijoueur utilisent une architecture client-serveur, où chaque joueur (client) est connecté à un ou plusieurs serveurs qui reçoivent les *inputs* des joueurs, mettent à jour l'état du monde et renvoient des informations aux client pour qu'ils puissent faire le rendu de la scène.

Dans notre jeu, les clients A et B envoient leurs *inputs* au serveur. Ensuite, le serveur calcule la nouvelle position des véhicules. Enfin, il envoie les position des deux véhicules à chaque client pour qu'ils puissent effectuer le rendu.

L'envoi des inputs depuis le client et leur réception par le serveur est déjà implémentée, mais il faut désormais envoyer les nouvelles positions et vitesses au véhicule et les recevoir côté client.

### Première étape : envoi des positions et vitesse du serveur vers les clients

Dans un premier temps, nous allons nous concentrer sur l'envoi des positions/rotations et vitesses du serveur vers les clients.
Ouvrez le fichier `Source/Server/Systems/NetworkUpdatePositionsAndVelocities.cpp`.

Comme pour le `MovementSystem`, vous pouvez remarquer que ce système a besoin d'un composant `Transform` et d'un composant ` Kinematics`. On rappelle qu'un système a une liste d'entités `entities` qui contient toutes les entités sur lesquelles le système peut être appelé.

To-Do :
-Pour chaque entité, il nous faut récupérer les composants `Transform` et `Kinematics` afin d'envoyer leurs position/rotation et vitesse.
- Une fois ceci fait, il faut envoyer **à chaque client** un paquet contenant la position/rotation (composant `Transform` complet) et un paquet contenant la vélocité. Pour récupérer la liste des clients connectés au serveur, il suffit de demander au `networkModule` une référence vers le serveur, avec `getServer()`, puis de demander au serveur ainsi obtenu la liste des clients avec `getClients()`.
- Itérez sur cette liste de client, et pour chacun d'eux :
	- Créez un paquet `SetTransformPacket` avec le composant `Transform` de l'entité.
	- Créez un paquet `SetVelocityPacket` avec la vélocité de l'entité.
	- Envoyez ces paquets.
	
Pour créer des paquets, utilisez la méthode `create<...>(...)` du `networkModule`, puis envoyez-les au client en appelant la méthode `send(...)` sur le client.

La méthode `create<...>(...)` prend en argument de template `<...>` le **type du paquet** à créer et en arguments normaux `(...)` les **données à écrire** dans le paquet. N'oubliez pas qu'il faut envoyer dans le paquet l'**ID de l'entité** à laquelle correspond les données, sinon comment le client ferait-il à la réception pour savoir quelle entité il doit mettre à jour ? Allez-voir les constructeurs de `SetTransformPacket` et `SetVelocityPacket` pour voir quels types de données sont requises pour leur création et dans quel ordre. Notez que le `PacketSequenceIndex` est automatiquement ajouté par le `networkModule`, il faut donc faire comme s'il n'existait pas quand vous appelez la méthode `create<...>(...)`.

**Aide**
Par exemple pour le paquet `SetTransformPacket`: 
```
networkModule->create<SetTransformPacket>(entity->getID(), transform);
```

Compilez et lancez l'application. Observez les graphes et remarquez que le graphe des paquets sortants montre désormais l'envoi des `SetTransformPacket`  et `SetVelocityPacket` du serveur vers les clients.

### Deuxième étape : recevoir la vitesse côté client

Nous devons maintenant traiter les données que nous a envoyé le serveur.
Les clients, tout comme le serveur, fait tourner un *thread* en arrière-plan qui reçoit les paquets. Dès qu'un paquet est réceptionné, il est stocké dans une mémoire tampon (*buffer*) jusqu'à ce qu'il soit utilisé.

Ouvrez `Source/Client/Systems/NetworkSetSpeed`.

To-Do :
- Nous devons tout d'abord récupérer les `SetVelocityPacket` du *buffer*. Pour ce faire, utilisez la méthode `extractPacketsOfType<...>` du `networkModule`. Cette méthode retourne une version réduite du *buffer* avec uniquement les paquets du type qui nous intéressent.
- Tant qu'il reste des paquets dans le *buffer* ainsi récupéré :
	- Extrayez le premier paquet avec `fetchPacket()`. Cela vous retourne le premier paquet reçu en ordre FIFO et le supprime du *buffer*.
	- Le client a besoin de savoir à quelle entité s'applique la vélocité transmise via le `SetVelocityPacket`. Vous pouvez l'obtenir avec `getEntityID()` sur le paquet.
	- Trouvez dans le `gameManager` l'entité avec cette ID, en appelant `getEntityWithID` sur le `gameManager`.
	- Mettez les données du `SetVelocityPacket` dans la `velocity` du composant `Kinematics` de l'entité récupérée. Vous pouvez obtenir les données du paquet avec `getSpeedX()`, `getSpeedY()` (vitesse selon X et Y).

Pour le traitement des paquets `SetTransformPacket` , nous l'avons implémenté pour vous dans `NetworkSetTransform.cpp`.

Compilez et lancez l'application. Vous devriez être en mesure de voir les véhicules bouger chez les deux clients en même temps.

Jouez avec les paramètres du serveur et observez ce qu'il se passe quand vous augmentez la latence ou la perte de paquets : 
- Augmentez par exemple le taux de perte de paquet du serveur vers le client 1 et bougez la voiture du client 2 avec les flèches. Les mouvements devraient être saccadés sur le client 1.
- Augmentez la latence pour la communication serveur -> client 1. Bougez la voiture du client 2 avec les flèches. Les mouvements devraient être répliqués sur le client 1 avec un délai non négligeable.





