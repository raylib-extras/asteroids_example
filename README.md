# Asteroids Example
An example game of asteroids for raylib written in C++.

## building
Uses game-premake
https://github.com/raylib-extras/game-premake

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
    * Basic physics for collisions and bouncing
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


## TODO
* UFO

* Code Comments

* Menu for volume?
* Bins for major platforms

## Bugs