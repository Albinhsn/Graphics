# A trip down the graphics pipeline
Essentially write a software renderer

We call functions via the some class API that sends command to the command processor

The command processor then "processes" and executes the command
The command in question needs to have some state attached to it 
Which means it can be different commands essentially

Vertex Shader walks through each primitive and does some transformation
Sends those primitives to the rasterizer who then rasters them and returns a list of points
To which the fragment shader then takes and calculates each color for by some computation

All additional data is moved via the abstract shader class?



## ToDo
- [x] Be able to open a window (using glfw) and write to the framebuffer
- [x] Wavefront parser
  - [x] test and render some object
- [x] TGA 
  - [x] test to render a texture
  - [ ] cleanup and test other files
  - [ ] TGA encoder
- [x] Implement Bresenham’s line algorithm
- [x] Implement basic triangle rasterization (painter’s algorithm)
- [ ] Implement a z-buffer 
- [ ] Vertex/Fragment shader classes setup
- [ ] Command processor
- [ ] Texture sampler
- [ ] Draw lines
- [ ] Draw splines
- [ ] Ray tracing/Post processing


## First steps
* Create an API that just writes to a framebuffer and renders it

### Obligatory parts
* Rasterizer
  * z buffer
* Vertex/Fragment shader
  * Able to be written by user?
    * Should be some generic class that could be extended
      * Takes as argument some data container 
* Command processor
* Compute Shader
  * This is just giving back access to the data and able to write directly to the framebuffer
* Texture sampler

* Wavefront decoder
* TGA decoder/encoder
### Potential parts
* Tesselation stage
* Geometry shader
* Actually write something that decodes a subset of HLSL/GLSL
* IA
  * No reason to make this


