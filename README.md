# Semester 2 of programming at DAE (Howest)
A few exercises and Project Zombouise


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

<img width="1320" height="879" alt="image" src="https://github.com/user-attachments/assets/2921428e-97b7-4c64-837c-86bbab17d26f" />
<img width="1311" height="871" alt="image" src="https://github.com/user-attachments/assets/f2621468-f9f4-4b38-9b26-65c1731db11f" />
<img width="1330" height="893" alt="image" src="https://github.com/user-attachments/assets/89f614aa-9469-41f0-b75a-b7fffb4b757d" />
<img width="1335" height="883" alt="image" src="https://github.com/user-attachments/assets/dd6ef520-639b-45fd-86a2-e5c3cb2586fb" />
<img width="1335" height="902" alt="image" src="https://github.com/user-attachments/assets/193f2e0a-b0d7-4af4-85f6-75dd5e7d52e3" />


<p align="right">(<a href="#readme-top">back to top</a>)</p>


## My version

* An isometric view of the map in 2D 
* Customisable map read from file
* Animations of movements
* 8 directions walk, run and crouch walk
* Camera Movements
* Fight with the zombies
* Sound also attracts zombies, speed will influence their radius of detection
* Health system
* FOV used to darken areas not visible by the player and zombies are not visible (unless very close)
* BFS algorithm used for the zombies to track the player

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
* space-bar to restart when dead

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

<img width="880" height="1066" alt="image" src="https://github.com/user-attachments/assets/f750916f-1fd3-40f3-a41e-2781393fe37c" />


<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

* point in a triangle by Sebastian Lague - (https://youtu.be/HYAgJN3x4GA?si=VZ0vn8herYIX3eXy)
* cpp reference for a lot of standard library features (https://en.cppreference.com)
* BFS pseudo-code on Wikipedia (https://en.wikipedia.org/wiki/Breadth-first_search)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

