# IRG
Laboratory exercises for course Interactive Computer Graphics.

# Licence

Part of code is done by professors on FER. 
© 2012-2021 Marko Čupić, Željka Mihajlović and Hrvoje Nuić
http://creativecommons.org/licenses/by-nc-nd/3.0/hr/

# Content

* Lab1
  * Drawing lines on raster units
  * Drawing and filling a polygon
* Lab2
  * OpenGL program
  * 3D objects
  * Model, view and projection matrices  
* Lab3
  * Removing hidden polygons
  * Shading
    * Constant shading
    * Gouraud shading
    * Phong shading
* Lab4
  * Textures
  * Shadow mapping
 
# Work

## Bresenham

Implementation of Bresenham's algorithm. Lines are drawn in the center of screen and lines cutting is implemented using Cohen Sutherland's algorithm. With left mouse button you determine two points and with right mouse button lines are drawn only in the screen center.

![bresenham][https://github.com/as51340/IRG/images/Bresenham.png]

## Polygons

Create a polygon with left mouse button and fill it with right mouse button.

![polgons][https://github.com/as51340/IRG/images/Polygon.png]

## Triangles

Drawing filled triangles using OpenGL VAO, VBO and EBO.

## Wired model

Wired model of robot's head.
![WIRED][https://github.com/as51340/IRG/images/Wired.png]

## OpenGL camera

Implementation of model, view and projection matrices. User can move his eye vector using keys Q, W, E, A, S, D.

## Removing hidden polygons 

Backface culling. Three different algorithms used for shading. Constant, Gouraud, Phong. 
![hidden-polgons][https://github.com/as51340/IRG/images/RemovingBackPolygons.png]

## Bezier curve

Eye is moving on Bezier's curve. User can set polygon's points using left mouse button and moving with Q,W,E,A,S,D. 
## Texture mapping.

Usage of minimaps, interpolation using nearest neighbour...

## Shadow mapping

Scene is first drawn from light's perspective and with this we get depth's map. Then second time it's drawn from observer's position for obtaining correct shadow position. Bias of 0.005 is used for removing effect called shadown acne. 
![shadow][https://github.com/as51340/IRG/images/shadow.gif]


























