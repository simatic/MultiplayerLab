# MultiplayerLab
Labs to illustrate a Multiplayer course at Télécom SudParis and Institut Polytechnique de Paris

# Classes documentation

## Application
The main application, used to launch the game. Manages framerate and client tickrate.

## Button
A Component used specifically as a button which can be pressed.

## Car
An Entity representing all the cars.
It uses either local key bindings or network given inputs to compute the next position and speed it should have in the Update or ServerUpdate methods.
It also locally manages dead reckoning, and saves timestamped inputs when on the server.

## ClientData
A simple class used by the server to save its clients' information.

## Collider
A utility file used to more easily manage collisions.

## Component
A mother class for GUI ccomponents, such as buttons or label.

## Container
A container class for aforementioned Components.

## Entity
A mother class for all game objects, be it cars, bullets, whatever.
It has a sprite which can be drawn, has all basic physical coordinates and information, an ID for client/server interactions.
It also has collider methods, especially OnCollision, which is overriden by every child class to perfectly manage collisions between entity types.

## GameClient
The class used by the local Multiplayer Game State to manage all network actions.
It also has a clock and a clock offset from the server, which is for now unused and might be wrong.

## GameServer
Unlike GameClient, GameServer is a class which manages everything the server has to : networking, but also physics, updates, collisions, etc, thanks to its ServerWorld.
It has a clock which is never reset and should be used as a sync tool for every client.
Each tick, it sends a ping to each client to compute its delay, and it also sends every cars' informations, such as position, velocity, etc.

## GameState
A State, used for local play.

## Keybinding
The class used to stock the bindings between keyboard's keys and the corresponding in game actions.

## Label
A Component used to simply write a string on screen.

## MenuState
A State, used for the main menu.

## MultiplayerGameState
A State, used for networked play : it can be used to either join or host a networked game.
If used to host, it will launch a new thread in which a game server will be running, and then connects to that server.
If used to join, it will only join a game with the given IP address.
It uses a GameClient to manage network actions, and a World to manage local physics, such as dead reckoning, and graphics.

## NetworkCommon
A utility file used for basic network actions, such as packing a custom object in a SFML packet.

## Particles
A class used by Entities to spawn, update, and draw particles.

## PickUp
An Entity, which can be picked up by cars driving over it.

## Player
A class used mainly to compute the part of the screen which will be displayed on screen, based on the player's car position.

## PlayerGUI
A Component used to display the player overlay : the map, the speedometer, and the selected action.

## Projectile
An Entity, representing either a bullet or an autoguided missile.

## ResourceHolder
A template class used to hold resources such as fonts, textures, etc.

## ResourceIdentifier
A simple file used to define specific resource holder often used.

## ServerWorld
The world class used by the server. It works as the basic world, but has additional methods to manage network.
Importantly, it holds a stack of update frames, containing all the information needed to rollback to a given date.

## SettingsState
A State used for binding keys to in game actions.

## State
A mother class for all states the game can be in.
It holds a context which stocks various resource holders and keybindings, along with the window used for display.

## StateIdentifiers
Simply holds the enum containing every ID representing each state the game can be in.

## StateStack
A stack containing all the current states the game is in (there can be multiple states at the same time, for example a pause state over a game state).
It is the class which calls all the states update, tick and draw methods.

## TitleState
A State used as the title screen.

## Utility
A utility file, used mainly for conevrsions and vectorial computations.

## World
The class used by local multiplayer game state or local game state to update all entities, handle events, collisions, etc.


# To Do

- Implement a way to correctly time stamp events passed from the server to the clients and vice versa.
- From this time stamp system, add a synchronization check on the server side, in order to rollback if needed.
- From the time stamp system, correctly update cars on the client side, in order to have correct dead reckoning.






































