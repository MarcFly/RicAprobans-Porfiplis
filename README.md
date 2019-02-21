﻿# DrunkEngine

## About the Project
DrunkEngine is the the result of developing a 3D Game Engine for our third year subject Game Engines. This is not a fully fledged game engine but holds core subsystems for building one.

## Info
This release includes an editor where you can import a .FBX file, transform all its contents and make your own scenes (you are able to save the scene and load it from the editor). It also includes an animation subsistem to load fbx with animations.

Github Page - https://github.com/MarcFly/DrunkEngine

Project Website - https://marcfly.github.io/DrunkEngine/

## Authors
The people currently working in this engine are Andreu Sacasas and Marc Torres, currently 3rd Year students coursing Design and Game Development at CITM, a center abscribed to the UPC.


## How to use it
DrunkEngine will automatically load an .FBX file with a street and some houses with textures and a basic animation to showcase the animation system.
To test the animation blending you can press 1 on the keyboard to throw a punch or hold 2 to make a walking animation.

It will export the files to our own format under a HIDDEN directory ./Library/

To move around the space, you can orbit around something with Alt+LeftClick or move in first person using RightClick+WASD. If you wander off to far, don't worry, press F to focus again to the current model.

In the menu bar, an array of options are available, take some time to look into it. We specially recommend using the Inspector and the Scene Viewer. Besides clicking them on the menu you can also open them with Ctrl+I and Ctrl+V respectively (you can check and change other shortcuts on the "input" section in the Options menu).
All keybindings are rebindable, make use of your perfect layout.

To add new fbx into the scene, just select the object that will be the parent for it on the Scene Viewer, go to Menu->Import... and select the new asset.
If you want to save the current scene or to load a new one, just go to Menu->Save/Load. 

In order to check out how time is flowing in-engine vs in-game, a simple collapsable header is under the Options Menu->Time Viewer.
The first 2 values represent engine delta time and time since startup, the 2 below represent current game delta time and time since game startup.

To change game speed you can change between modes:
 - Play - Normal Playback speed
 - Slow - Will slow gameplay by X times
 - Fast - Will speed up gameplay by X times
 - Reverse - Reverses play speed (negative delta time)
 - RSlow - Reverses play speed and slows reverse time by X amount
 - RFast - Reverses play speed and speeds reverse time by X amount

Currently game time is not used in any shape or form, but there is a structure prepared for future use of it, separating editor and game runtime code.

When saving a scene, it will be created automatically under an Assets folder inside the Executable directory.

If you make a mess loading things, you can clear up the editor at Options->Delete Scene.

## Each member's Work

![](https://i.imgur.com/2GgGvr3.png)  
### Andreu Sacasas

<details><summary>Input options.</summary>  
<p>    
   
TEXT  

<a href="https://gyazo.com/9d995af0f8ce2298e3e03b468e906ea8"><img src="https://i.gyazo.com/9d995af0f8ce2298e3e03b468e906ea8.gif" alt="Image from Gyazo" width="300"/></a>  

</p>  
</details>  
<details><summary>Frustum implementation (Editor camera + component Cameras & FOV).</summary>  
<p>  
  
TEXT  

Editor Camera:  
<a href="https://gyazo.com/9d3a69bc0125d8a719edb56b7979ebd9"><img src="https://i.gyazo.com/9d3a69bc0125d8a719edb56b7979ebd9.gif" alt="Image from Gyazo" width="800"/></a>  

Component:  
<a href="https://gyazo.com/dc4f9ff165e5b1fddb1bf70b76f76960"><img src="https://i.gyazo.com/dc4f9ff165e5b1fddb1bf70b76f76960.gif" alt="Image from Gyazo" width="800"/></a>  

</p>  
</details>   
<details><summary>Static objects System.</summary>  
<p>  
 
TEXT  

</p>  
</details> 
<details><summary>KD-Tree + implemented it on frustum culling / mouse picking optimizations.</summary>  
<p>    
   
TEXT  

Image  

</p>  
</details> 
<details><summary>Implementation of ImGuizmo.</summary>  
<p>    
   
TEXT  

Image  

</p>   
</details> 
<details><summary>World / Local transformations to GameObjs.</summary>  
<p>    
   
TEXT  

Image  

</p>   
</details> 
<details><summary>Event System.</summary>  
<p>  
   
TEXT  

Image  

</p>   
</details> 
<details><summary>Scene Viewer (Objs Tree + Selection).</summary>  
<p>    
   
TEXT  

Image  

</p>  
</details> 
<details><summary>Implementation of the skeletal animation subsystem.</summary>  
<p>    
   
TEXT  

Image  

</p>  
</details> 

![](https://i.imgur.com/1uM1OJA.jpg)  
### Marc Torres

- Mouse Pick.
- Recource Management.
- Save/Load Scene.
- Import Assets.
- GameObject Components in the Inspector.
- Export assets to our own format (into a hidden "Library" folder in Game).
- Reference counting for Resource Management
- Engine Time Management for Game and Editor mode.
- File management for the animation system.
- Activate / Deactivate components.

## SubSystems

### Base Sub-Systems  
- Event System: To have a better control throughout the engine subsystems.  
- Scene: Gameobject hierarchy management and all of its components.  
- Input: Takes care of the user I/O using SDL.  
- Renderer: Initializes OpenGL to use it with SDL and ImGui.  
- Window: Application control through SDL.  
- Time: Module that controls in-game and editor time.  
- Import: Translates external files to engine format for efficiency purposes.  
- Resource: Manages memory usage with asets.  
- UI: Uses ImGui to manage all the editor controls.  
- KDTree: To optimize the engine.  

### Skeletal Animation Sub-System
As part of the last assignment, a basic skeletal animation sub-system has been implemented. Currently it can handle single mesh skeletons with different animations. Animations can be created from a single timeline import, through the inspector and later revised with the animation panel or ingame.  

![](https://media.giphy.com/media/9x5b92dJqSBvtiqYBQ/giphy.gif)
![](https://media.giphy.com/media/A7VJ37OYWXSTzRGHP1/giphy.gif)

For a detailed explanation on our susbsystem and getting into our engine, refer to the following video:  
[![](https://upload.wikimedia.org/wikipedia/commons/thumb/4/4c/YouTube_icon.png/800px-YouTube_icon.png)](https://www.youtube.com/watch?v=bNXV1bHX8Es&feature=youtu.be)

[**Link to Repo**][1]  
[**Latest Release**][2] 

## MIT License

Copyright (c) 2018 Marc Torres Jimenez & Andreu Sacasas Vel�zquez

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

[1]:https://github.com/MarcFly/DrunkEngine
[2]:https://github.com/MarcFly/DrunkEngine/releases
