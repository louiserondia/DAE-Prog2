<a name="readme-top"></a>

<!-- GENERAL GAME INFO -->
<br />
<div align="center">

  <h2 align="center">Project Zombouise</h2>

  <p align="center">
    Project Zomboid is a hardcore, open-world isometric survival sandbox game developed by The Indie Stone, set in a zombie-infested, post-apocalyptic Kentucky. Players focus on scavenging, building bases, and managing needs (hunger, fatigue, fear) to survive as long as possible before their inevitable death.
    <br />
    <strong>Original game : Project Zomboid</strong>
    <a href="https://store.steampowered.com/app/108600/Project_Zomboid/"><strong>General info »</strong></a>
    ·
    <a href="https://youtu.be/J5es-BacqLI?si=wqa7DBI5vso4mlhe&t=142"><strong>Youtube video »<strong></a>
    <br />
    <br />
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
    <li>
      <a href="#my-version">My version</a>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
    </li>
    <li><a href="#how-to-play">How To Play</a></li>
    <li><a href="#class-structure">Class structure</a></li>
    <li><a href="#checklist">Checklist</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project


<img width="1335" height="902" alt="Capture d&#39;écran 2026-06-01 011627" src="https://github.com/user-attachments/assets/d56a0907-f975-4c20-918e-920c255eca3c" />
<img width="1335" height="883" alt="Capture d&#39;écran 2026-06-01 012825" src="https://github.com/user-attachments/assets/9e3f5c08-f493-4700-bc0f-a6df4b3af678" />
<img width="1330" height="893" alt="Capture d&#39;écran 2026-06-01 012920" src="https://github.com/user-attachments/assets/bf81aaf5-6b39-436d-b74c-6aea6f8cb3db" />
<img width="1311" height="871" alt="Capture d&#39;écran 2026-06-01 012211" src="https://github.com/user-attachments/assets/65594cf9-7727-4156-92ab-8a7964523bc1" />
<img width="1320" height="879" alt="Capture d&#39;écran 2026-06-01 012311" src="https://github.com/user-attachments/assets/46635d40-45f0-4c79-b265-ea5e136d20a3" />


Here's why:
* The game is freaking cool
* Different AIs and algorithms (path-finding, flocking)
* Raycasting
* Infinite sources of fun

<p align="right">(<a href="#readme-top">back to top</a>)</p>


## My version

This section gives a clear and detailed overview of which parts of the original game I planned to make.

### The minimum I will most certainly develop:
* An isometric view of the map in 2D (alpha release of the game)
* Map is read from a custom file
* Animations of movements
* 8 directions walk, run and crouch walk
* Zombies attacking using path-finding
* Camera Movements

### What I will probably make as well:
* Fight with the zombies
* Sound also attracts zombies

### What I plan to create if I have enough time left:
* Inventory 
* Health (e.g. hunger, thirst) system
* Complex raycasting, tiles are darkened when outside of FOV
* Flocking

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- GETTING STARTED -->
## Getting Started
Detailed instructions on how to run your game project are in this section.

### Prerequisites

* Visual Studio 2022

### How to run the project

Open the game solution (located at the root as "ProjectZombouise.sln") in Visual Studio 2022 and run the program!
You can also simply run the .exe file.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- HOW TO PLAY -->
## How to play

Use this space to show useful examples of how a game can be played. 
Additional screenshots and demos work well in this space. 

### Controls
* WASD keys to move
* shift to run
* right-click to look around
* space-bar to attack (push or stomp if the enemy zombie is on the ground)
* space-bar to restart

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CLASS STRUCTURE -->
## Class structure 

The game is split in two main branches : the gameplay part and the display part.

Renderer displays every object. Each entity owns a Renderable which is stored in a queue in the Renderer. 
The game uses isometric view so the queue is used to draw the different textures in the correct order.
TextureManager owns every texture, sound effect and even font that is allocated in the game then passes pointers to the requiring objects.
ZombieManager handles the herd of zombies for instance spawns a zombie every now and then, keeps track of a death counter and so on.

### Object composition 

I used aggregation to handle media management : TextureManager owns the textures, sound effects, etc and has control over their lifetime. 
Classes such as Renderable (of any type) use a pointer from the TextureManager, Player and Zombie do the same with the SoundEffect, Renderer also uses a pointer to text textures. 

### Inheritance 

I used inheritance in two major cases : my entities and my renderables.

A Renderable is anything that is rendered. 
Renderable is the parent class and from there, we have WallRenderable, FloorRenderable and WalkingRenderable.
WalkingRenderable is the parent of 2 subclasses : ZombieRenderable and PlayerRenderable, it encapsules all the rendering common to the render part of a walking entity.

On the gameplay aspect, we have a similar tree with WalkingEntity, parent of Zombie and Player.

### Polymorphism

My classes WalkingEntity and WalkingRenderable are abstract classes which Player and Zombie (and their Renderable part) inherit from. They must own methods such as UpdatePos, declared int the parent class as pure virtual and they use compile-time and run-time polymorphism for methods such as Attack or Draw.

<img width="880" height="1066" alt="image" src="https://github.com/user-attachments/assets/1db1dbb0-58e5-48fe-a7cb-d83f946437d0" />

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- CHECKLIST -->
## Checklist

- [x] Accept / set up github project
- [x] week 01 topics applied
    - [x] const keyword applied proactively (variables, functions,..)
    - [x] static keyword applied proactively (class variables, static functions,..)
    - [x] object composition (optional)
- [x] week 02 topics applied (classes basics)
- [x] week 03 topics applied (classes object lifecycle)
- [x] week 04 topics applied (inheritance and composition)
- [x] week 05 topics applied (polymorphism and inheritance)
- [x] week 06 topics applied (operator overloading and polymorphism)
- [x] week 07 topics applied (copy semantics and operator overloading)
- [x] week 08 topics applied (copy semantics and object slicing)
- [x] week 09 topics applied (optional) (move semantics)
- [x] week 10 topics applied (optional) (streams)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTACT -->
## Contact

Louise Rondia - louise.rondia@student.howest.be

Project Link: [https://github.com/HowestDAE/gd12-louiserondia](https://github.com/HowestDAE/gd12-louiserondia)

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

* point in a triangle by Sebastian Lague - (https://youtu.be/HYAgJN3x4GA?si=VZ0vn8herYIX3eXy)
* cpp reference for a lot of standard library features (https://en.cppreference.com)
* BFS pseudo-code on Wikipedia (https://en.wikipedia.org/wiki/Breadth-first_search)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

