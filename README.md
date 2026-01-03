# miniRT

![C](https://img.shields.io/badge/language-C-blue.svg)

### Overview

miniRT is a **minimal ray tracer** developed by [biertisch](https://github.com/biertisch) and [HL-PT](https://github.com/HL-PT). It renders 3D scenes by casting rays from a virtual camera and computing object intersections, lighting, and shading.

The programme is written in C using the **MiniLibX** graphics library. It supports multiple geometric primitives — **plane**, **sphere**, **cylinder**, **cone**, and **triangle** —, along with visual enhancements such as **checkerboard patterns** and **bump mapping**. Lighting is implemented using a **Phong reflection model**, combining ambient, diffuse, and specular components, with support for **colored and multiple light sources**.

The renderer features **interactive scene manipulation** via keyboard controls and a custom control panel, enabling real-time transformations of the camera, lights, and objects.

The project is part of the 42 curriculum, serving as an introduction to **computer graphics** and fundamental **ray-tracing principles**.

---
### Features

* **Ray tracing engine**
	* Ray-object intersections for planes, spheres, cylinders, cones, and triangles
	* Correct handling of object interiors
* **Lighting**
	* Phong reflection model: ambient, diffuse, and specular lighting
	* Hard shadows
	* Colored and multiple spot lights
* **Scene parsing**
	* `rt` scene description file
	* Strict validation based on scene metadata
	* Detailed error reporting with precise line-level diagnostics
* **Transformations**
	* Translation of camera, lights, and objects
	* Rotation of camera and objects
	* Dynamic resizing of object properties
* **Interactivity**
	* Keyboard controls for camera movement and rotation
	* Custom control panel for light and object transformations
* **Visual enhancements**
	* Checkerboard patterns
	* Bump map textures

---
### Installation

**Requirements**:
* Linux with X11
* GCC
* make
* MiniLibX (included)


```
git clone https://github.com/biertisch/miniRT.git
cd miniRT
make
```

---
### Usage

```
./miniRT [path/to/scene/description]
```

Example:
```
./miniRT RT/axis.rt
```


#### Configuration file
* **Camera**:
	* Format: name:char:C origin:vector:[x,y,z] orientation_vector:vector:[-1,1] field_of_view:[0-180]
	* Example: `C 0,0,-5 0,0,1 70`
* **Ambient light**:
	* Format: name:char:A lighting_ratio:double:[0.0,1.0] color:vector:[0-255,0-255,0-255]
	* Example: `A 0.1 255,255,255`
* **Localized light**:
	* Format: name:char:L position:vector:[x,y,z] brightness:double:[0.0,1.0] color:vector:[0-255,0-255,0-255]
	* Example: `L 2,3,-2 0.6 255,255,255`
* **Plane**:
	* Format: name:char:pl point:vector:[x,y,z] orientation_vector:vector:[-1,1] color:vector:[0-255,0-255,0-255]
	* Example: `pl 0,-2,0 0,1,0 200,0,0`
* **Sphere**:
	* Format: name:char:sp center:vector:[x,y,z] diameter:double color:vector:[0-255,0-255,0-255]
	* Example: `sp 0,0,0 2 0,0,200`
* **Cylinder**:
	* Format: name:char:cy center:vector:[x,y,z] orientation_vector:vector:[-1,1] diameter:double height:double color:vector:[0-255,0-255,0-255]
	* Example: `cy 2,1,2 0,1,0 2 2 100,80,255`
* **Cone**:
	* Format: name:char:co apex:vector:[x,y,z] orientation_vector:vector:[-1,1] diameter:double height:double color:vector:[0-255,0-255,0-255]
	* Example: `co -2,1,2 0,1,0 2 2 0,100,255`
* **Triangle**:
	* Format: name:char:tr vertex:vector:[x,y,z] vertex:vector:[x,y,z] vertex:vector:[x,y,z] color:vector:[0-255,0-255,0-255]
	* Example: `tr 0,10,0 2,10,0 0,4,0 0,150,0`
* **General rules**:
	* The scene must contain exactly one camera
	* The scene must contain exactly one ambient light
	* The scene must contain at least one spot light
	* All orientation vectors must be normalized (length = 1)


#### Controls
* `p` - print scene info
* `W`/`A`/`S`/`D` - move camera
* `↑`/`←`/`↓`/`→` - rotate camera
* `r` - rotate scene
* `h` - help
* `esc` - quit

---
### About 42

Founded in 2013, 42 is a global network of schools providing high-quality, scalable software engineering education. Its project-based, peer-to-peer curriculum emphasizes hands-on programming, algorithmic thinking, and systems-level problem solving.

---
### License & Contact

This repository is open for learning and reuse. Contributions, suggestions, and discussions are welcome — feel free to open an issue or reach out.
