# scop

This project is based on 42 project Scop but I pushed it to be like HumanGL project. To explain it, the goal is to load an object and render it with OpenGL. Normally the object is stored in a `.obj` but to handle animations, I decided to switch to `.glb./.gtlf`. 

## How to use it
You just need to compile the project and use it with `.glb/.gtlf` parameters. You will be able to see them, and combine them via ImGui window.

## Rendering
I also improved rendering, by adding light management. In the Scene, you will find 4 point light, one directional light coming from the top, and you also have a flashlight.

## Future
This project will still evolve in the future. I will probably continue to update and improve rendering based on `.glb/.gltf` data. It will also be the starting point for a the model loading part of my future engine project.
