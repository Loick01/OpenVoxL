# OpenVoxL (WIP)

**This repository is an improved version of one of my previous projects developed during my computer science studies, when I was first learning 3D programming with OpenGL. This updated version aims to provide cleaner and more maintainable code, as well as additional features.**

### How to compile

```bash
# Run these commands from the root directory
mkdir build
cd build
cmake ..
make
./openvoxl # Run
```

### Requirements

+ C++17
+ CMake >= 3.10
+ OpenGL >= 4.3
+ GLAD, GLFW, GLM and ImGui are already included in [external](external)

### Controls

+ **W/A/S/D :** Move the camera forward/backward/left/right
+ **Spacebar/Left control :** Move the camera up/down
+ **Mouse :** Look around
+ **E :** Switch between MouseFree and KeyFree camera states
+ **Q :** Show/Hide the HUD
+ **F3 :** Show/Hide the Debug window
+ **Escape :** Close the window

### Demo videos

<table align="center">
  <tr>
    <td align="center" width="250">
      <a href="https://www.youtube.com/watch?v=aOq84dPkSeA">
        <img
          src="https://img.youtube.com/vi/aOq84dPkSeA/maxresdefault.jpg"
          width="240"
          height="135"
          style="object-fit: cover;"
        >
      </a>
      <br>
      <strong>Skybox, Camera and Face Culling</strong>
    </td>
    <td align="center" width="250">
      <a href="https://www.youtube.com/watch?v=mNM1NbJVsNA">
        <img
          src="https://img.youtube.com/vi/mNM1NbJVsNA/maxresdefault.jpg"
          width="240"
          height="135"
          style="object-fit: cover;"
        >
      </a>
      <br>
      <strong>Flat, Full, Wave and Cheese Chunks</strong>
    </td>
    <td align="center" width="250">
      <a href="https://www.youtube.com/watch?v=uEHOm_V1WbA">
        <img
          src="https://img.youtube.com/vi/uEHOm_V1WbA/maxresdefault.jpg"
          width="240"
          height="135"
          style="object-fit: cover;"
        >
      </a>
      <br>
      <strong>Terrain Heightmap Generation</strong>
    </td>
  </tr>
</table>

<!--
### Debug window
-->
