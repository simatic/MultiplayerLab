# 2M3 Multiplayer Lab

## Introduction

The goal of this lab is to make you experiment the different problems of multiplayer games, and some solutions to these problems.

For each step of the lab:

-   Experience the problem.
-   Experience the proposed solution by coding it according to the instructions.

## Preliminary installation

Go to https://github.com/simatic/MultiplayerLab and download the latest release of the Lab, in the "Releases" section.

Extract the archive. Then follow the installation instructions depending on your operating system.

### Windows with Visual Studio 2019 and MSVC2019

Open up Visual Studio 2019 and select `Continue without code` on the welcoming page.
Then go to `File > Open > CMake...` and navigate to the extracted archive. Select the `CMakeLists.txt` file and click on `Open`.

### Linux with GCC
TODO

## Structure of the program

This Lab is based on a 1v1 shooting game against two vessels. The goal is to make each of the two clients communicate with each other through a server.

The project is divided into 3 sections : Client, Server and Common.

- The `Client` folder contains all the code of the Client side. That is, the code of the game you would execute if you had purshased a multiplayer game.
- The `Server` folder contains the code of the Server side, which is what would be executed on cloud servers in real-life implementations.
- `Common` contains code that are used both by Client and Server for convenience. However, the app behaves as if Server and Client where totally separted and connected only via network, just like in real-life.

The engine used for this game is composed of three elements :
- `Components` are basic C++ structs that hold data. For instance, the `Transfrom` component holds a `sf::Vector2f` to store the position and a `float` for the rotation :
```
struct Transform {
	sf::Vector2f position;
	float rotation;
}
```
- `Entities` are the equivalent of GameObjects in Unity. They have a unique ID and a set of components, depending on their function.
- `Systems` handle the logic of the game and act on entities' components. For example, the `MovementSystem` fetch the speed of the `Kinematics` component and update the `Transform` component accordingly. Of course, this system runs only on entities that **have** the `Kinematics` and `Transform` components. The `GameManager` performs smart checks to ensure this property.


`Systems` are what you will be modifing the most, so as to send data from the client to the server and vice-versa. They work a bit like in Unity with `start()` and `update()` methods.

## First steps with 2M3 structure

To understand the game engine a bit further, our first task will be to implement the `MovementSystem` for entities to update their position according to their speed.

Open the `MovementSystem` at `Common/Systems/MovementSystem.cpp`.

This is what you should see :
```
MovementSystem::MovementSystem(GameManager* const gameManager) :
	LogicSystem<Transform, Kinematics>(gameManager)
{}

void MovementSystem::update(const sf::Time& dt) {
	// TODO: Update entities' positions
}
``` 

The first method is the constructor if the system. As you can see, the system needs a reference to the `gameManager`. You can also see that it is a `LogicSystem` that only acts on entities that have a `Transform` and a `Kinematics` component.

The second method is what will be called at each frame. It is important to know that each system has a list of the entites it can perform operations on. This list is stored under the `entites` member variable.

What you need to do is :
- For each entity in `entities`, update entity's position with `position += speed * dt`.
- Check the `Transform` and `Kinematics` components under `Common/Components` to see what they contain.
- You can get the entity's components with `entity->getComponent<...>();`.
- Once you have a component, you can access its data with `->`, e.g. `transform->position`.

Once the `update()` method is completed, compile and run the program. You should see the vessels move if you press the `WASD` keys. You may need to wait a littel bit until the server disconnects because the server is authoritative and will maintain the initial positions. It will disconnect after a few seconds.

## Replicating movements

The tow vessels are nox movable! Great! But this is still a single-player game, and the enemy isn't moving at all.
Most multiplayer games use a client-server architecture, where each client (the players) is connected to a central server that receives all the inputs from the clients, update the world status accordingly and send back information to the client for them to render the resulting scene.

In our game, Client A and Client B send their inputs to the Server. Then, the Server computes the new positions of the vessels. Finally, the Server sends the positions of both vessels to each Client so that they can render the scene.

The goal is now to set up this communication.

### First step : send inputs from client to server

We will first focus on sending inputs from the client to the server.
Open `Source/Client/Systems/NetworkSendInputs.cpp`

As with the `MovementSystem`, you can see that this system needs a `Transform`component and a `PlayableCarInputs` component. This component holds inputs for the vessel that is played by the user. Remember that a system has a list of entites named `entities` which contains the entities on which the system can be applied. Each client has only one playable vessel, so the size of `entities` should be 1.

What you need to do is:
- Get the playable vessel from the `entities` list.
- Extract ist inputs from the `PlayableCarInputs` component.
- With the `createAndSend` method of the `networkModule`, send an `InputPacket` that contains these inputs. Check the constructor of `InputPacket` to see in which order the inputs need to be passed to the packet. Note that `PacketSequenceIndex` will automatically added to the constructor by the method `createAndSend`.

### Second step : receive client's inputs from server side

Now, we need to get the inputs the client sent us.
Recieving is a bit more complex. The server, as well as the clients, has a background thread that listens for incoming packets. Once packets are recieved, they are stored into a buffer until they are used.

Open `Source/Server/Systems/NetworkPlayerInputs`.
- In order to manage client inputs, we first need to get the `InputPacket`s from the buffer. This is done with the `extractPacketsOfType<...>` method of the `serverModule`.
- The server now needs to know for which vessel does each packet apply to. The server has a corresponding table between the client's ID and the playable vessel of this client. You can get it with `serverModule->getServer().getClientEntityByID(...);`. You can get the ID of the sender by calling `getSender` on the packet.
- Finally, put the inputs of the `InputPacket` in the `CarInput` of the fetched entity with `inputPacket->get[Up/Down...]`.

Play with the server controls and observe what happens when you increase packet loss or latency.

