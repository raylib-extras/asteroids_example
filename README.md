# Asteroids Example
An example game of asteroids for raylib written in C++.

## building
Uses game-premake
https://github.com/raylib-extras/game-premake

### Windows MinGW
Run the premake-mingw.bat and then run make in the folder

### Windows Visual Studio (not VSC)
Run premake-VisualStudio.bat and then open the fasteroids.sln that is generated

### Linux
CD into the directory, run ./premake5 gmake2 and then run make

### MacOS
CD into the directory, run ./premake5.osx gmake2 and then run make

## Pre-Release
Pre-Release, still a work in progress.

## Gameplay features
* Shooting Asteroids
* Increasing Dificulty
* Powerups

## Code Goals
The goals of this example are to show common techniques for making 2d action games, in the form of a playable asteroids game.
The main features are :

* Time Based Movement
    * Moving items based on speed and thrust
    * Movementum
    * Computing a rotation and vector from a mouse position
    * Basic physics for collisions and bouncing
* Game world larger than the screen
    * Use of a Camera2d
    * World space vs Screenspace
    * Camera shake
* Entity Polymorphism for game objects
    * Entity Base class
    * Derived types for various gameplay objects
* Time Based Events
    * Triggering effects that expire over time
    * Expiring shots based on time
* Performant Entity Lists
    * Reusing entities
    * Processing entities together by type to maximize cache hits
* Audio
    * Sound Effects
    * Background Music
* Game States
    * Different Screen overlays for different game states (menu, playing, game over, etc...)
    * Resetting gamestate based on different levels of play
    * Player data state that is updated by powerups

## Design
The game is designed around two main systems, the world and the overlay.
A world contains all the game objects that make up the game, and the overylay will display one of many screens depending on the current game state (menu, gameplay, game over, etc..)

### World
The world contains lists of game object engities. Each enitty type is derived from a base entity class that has common data and logic that is applied to all entities, such as alive state, position, rotation, and basic movement update logic. Entities in are stored in world space and can extend outside the bounds of the vissible screen.

The derived entities are
* Asteroid
* Bullet
* Player
* PowerUp
* Explosion

Each of these entity types is stored in a sperate list that is part of the world. When an entity is no longer alive, it is not removed from the lists but is just kept in an inactive state and reused when a new entity of that type is needed. This is done to remove the overhead of erasing from a vector in the middle and forcing all following entities to be moved. This is a common way to make large lists of items be peformant in games.

#### Why not one big entity list?
Some may ask that since we have an entity base class, why not just have one big list of entity pointers and itterate them all, then just draw and update them all at once. We could do this, but it is often not the best way to do things.

1) It is very common to process all the entities of a specific type at one time, By having a big mixed list of entities you spend a lot of time itterating the big list looking for the entities you want.
2) It is much more efficent for the CPU to load data that is going to be processed together from memory that is together. By keeping the entities seperate by type, we get a lot fewer CPU Cache misses when processing things like updating or drawing all the asteroids or shots.
3) When we need to compare one type of object to another, such as checking if shots collide with asteroids, we can quicly access just the shot or just the asteroids without doing large searches.
4) Since we want to reuse entity slots, having a big list of mixed entities, makes that much harder. Seperate entitity lists allows us to minimize allocations and memory moves.

This system has some of the main benifts of a full Entity Component System (ECS) by keeping similar data together, but does not have the full complexity of an ECS.

### Entitiies
The basic entity stores common entity data and processes basic collision and moveent. Movement is handled using velocity and time. Each entity has a velocity that represents how many pixels per second in each axis the entity will move. This is turned into a per frame update by multiplying this movement vector by the frame time. Rotation is handled in a similar manner using an angular velocity (degrees per second).

Collision is handled using simple circle checks with each entity having a radius that is used for checks.

#### Asteroid
The asteroid entity uses it's radius to determine the size the sprite on screen and how much damage it does if it was to hit the player. When an asterpoid is hit by a bullet it will split into two new asterpids that are a random percentage of the parrent asteroid. Splitting an asteroid gives the player 1 point. If the new peices are smaller than some limit the peice will be destoryed itself and the radius will be added to the player's score. This gives more points for destorying small asteroids compared to splitting larger asteroids. Everytime an asteroid is destoyed an expliosion is spawned. Asteroids bounce off the walls of the world.

#### Bullet
The Bullet entity is spanwed by the player enity and is a fixed size. It follows a fixed path and does not rotate as it travels. They have a fixed lifetime and are marked inactive when they despawn. Bullets are checked aginst asteroids for hits, and trigger a split or destruction when they hit one.

#### PowerUps
The Powerup entity is randomly spawned when an asteroid is hit. There is a 10% chance that one will be spawned. They bounce off walls like asteroids, but can not collide with bullets. They are checked for collisions aginst the player. When they collide with the player they grant an upgrade bonus to the player by modifying state data in the player entity before they are marked inactive.

There are 3 powerup types

* Shot (Red)
    - Reduces the reload time between shots (more bullets)
* Shield (Yellow)
    - Adds 10% shield power back to the player (up to the maxium), and increases the ammount of sheild regeneration per second.
* Boost (Blue)
    - Adds 10% boost power back to the player (up to the maxium), incrases the base speed and increases the boost multiplyer of the player ship.

#### Explosions
Explosion entities are spawned when asteroids or the player is destoryed. They are a graphics only entity and do not affect gameplay. They do not move and are not checked for collisions. Each explision has a list of particle entities that are used to draw the various parts of the explosion. Explosions have a short fixed lifetime and fade out as they die.

#### Player
The world contains a single player entity that contains all the state data pertanining to the player. THe player does not use the normal update system from the base class because it checks for user input and applys a special rule for friction to make the game more playable. Input is polled from the mouse, keyboard, and gamepad (if available). 

The player entity has the following properties

* Base Thrust
    - When thrusting this is the speed per second per added to the velocity vector. Powerups can increase this.
* Boost Multiplyer
    - When boosting this is how much extra thrust is added. Powerups can increase this.
* Power
    - When boosting power is reduced. A boost can not be started if the boost power is less than 25%. A boost will end when boost power is at 0. WHen not boosting boost power will restore at a fixed rate.
* Shield
    - When the ship hits an asteroid it's radius is removed from the shield total. If the shield drops below 0 the ship is destoryed and the game session is over. The shield power restores slowly at the shield restore rate.
* Shield Restore Rate
    - How much shield power per second is added back. Powerups can increase this.
* Shot reload time
    - How much time before the ship can shoot again. Powerups can lower this time (allowing more bullets alive at a time)

The player has cusotm drawing logic that handles showing the thruster fire in regular and boost mode, as well as showing when the shield has been hit, and fades out over time.

## Sprites
All sprites are contained in the Sprites namespace that stores a list of sprite frames. A sprite frame is an index into a spritesheet and a source rectangle. Sprites are drawn by providing the sprite frame ID from a set of extern variables that the rest of the code can use. Sprite drawing by default centers the sprite frame on the position unless the DrawJustified function is used. Options are available to modify the center point to allow for various special effects.

## Audio
Audio is contained in the Sounds namespace and works in a similar way to how sprites are managed. There are 3 main music streams, the background music, and two thuster tracks. The thruster tracks are only played when the player is trusting and boosting.

Sound effects are played with PlaySoundMulti.

## Game System (overlays and gamestate)
The Game namespace tracks the current game state and draws the correct overlay for that state. Each state is reponsible for transitioning to the next state based on actions in the state.

### Menu
SHows the menu screen with instructions and waits for a click, then resets the world back to level 1 and resets the player and changes the game state to playing

### Playing HUD
Shows the main game hud and checks to see if the player has died or cleared the level. If the level is cleared, the gamestate is set to the NextLevel state.
If the player has died, they are sent to the game over state.

### Next Level
SHows a countdown timer for the next level. When the timer is done, the level is incremented and the world is reset and the player is respawned.

### Game Over
Shows a game over screen with the current score and waits for a click, On Click the world is reset to a default but the player is left dead, and the gamestate is set to the menu state.

## Other Notable Things

### Entity Buffer Reuse
There is common code in the various entity create functions that will look for an empty slot in the list of entities for that type before creating a new one. This is to prevent the removal of an entity from causing the rest of the entities to need to be moved. This code would be a great candidate for templates in C++, but they were not used in this example to keep it simple.

### Camera
The world is drawn using a Camera2d. The offset is set as the center of the screen, and the target is set to track the player. This allows the game to follow the player though the world that is larger than the screen.

When boosing the offet is jittered at a high frequency using sin and cos to provide a shake effect.

A second camera is used to draw the overlay, in order to provide a magnified shake effect for the UI.

### Culling
In order to not draw things that can't be seen the world system checks the distance to each assteroid an only draws ones that are within a specific radius of the screen center. This radius is computed using the screen size in world space. 

### Levels
Every time a new level starts the entities in the world are reset and the player is respanwed. The level number is used to add more asteroids to the level and increase the speed. 

### Additive Blending
Bullets, explosions and shields are drawn with an additive blend mode to make them appear more smooth.

## TODO
* Pause
* UFO
* Code Comments
* Menu for volume?
* Bins for major platforms

## Bugs