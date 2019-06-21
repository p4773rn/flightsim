# Flightsim
Just another basic flight simulator. Work was done by team of 3 people. Written in C++ using SFML and OpenGL. Terrain rendered using tessellation shaders. (TO DO: add some information about deffered rendering).

BUGS:
  -During writing parser for AC3D we somehow ended up with not quite correct normals.
  -Still need to correct instruments (It is something to do with quaternions and its conversion relative to the world)
  -Sadly, we still need more objects like rocks and trees (Terrain looks dead).
Controls:
  Z,X for thrust.
  V for view.
  Up, Down - Elevator deflection.
  Q, W - Rudder. R to reset.
  Left, Right - Flaps.
  +, -   - simulation speed.
  F1 - wireframe.
  F2 - ugly clouds (Sorry Alen; but they are still quite awesome given the time constraints)
  F3 - render targets.
  F4 - arrows.
  F5 - sun cycle.
  
For compilation it requires: SFML, GLM, GLEW.
![alt text](https://github.com/p4773rn/flightsim/blob/master/screen.png)

P.S: Project was abandonded after we get our mark for it (Maybe someday if I have time... I think it needs to be rewritten totally from scratch, but it was some cool experience with OpenGL).

A lot of thanks to this guys' blogs:
https://victorbush.com/2015/01/tessellated-terrain/
www.iquilezles.org
