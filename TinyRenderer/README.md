# TinyRenderer


## Lesson 1
### Bresenham's line drawing algorithm
Just a basic implementation of bresenham's line drawing algorithm.

Essentially given two points that creates a line in 2D you want to figure out which pixels that needs to be painted.

This is done by essentially walking across one axis, let's say x for our this case and determining whether or not to increase or decrease our y value.

So how would be choose to do this? Well an approximation to this is to use the slope aka the rise of run between the two points and then rounding upwards at each step.

This becomes kinda ugly because the approximation isn't that good, a better approximation (which is what's being done in the algorithm) is rounding up if we're > .5 and down if < .5.

So essentially we just traverse from our start x to end x and add our slope after each step and painting the pixel that we'd round our y value to.

This works but is kinda slow considering we have to take into account floating point values and a bunch of rounding. 

Optimizing this is done via multiplying it with two so we can check at integer value rather the floating point
i.e the loop from start x to end x would look like
```
for(i16 x = x0; x <= x1; x++)
{
    setPixel(image, x,y, color);
    derror += d;
    if(derror > dx)
    {   
        y++;
        derror -= 2 * dx;
    }
}
```

We then have to do some shuffling around of stuff depending on which values are greater and which slopes is the steepest

## Lesson 2
Implemented triangle rasterization. 

Given three vertices how do we compute if a point is inside the triangle or not?

Well we do this by looping over every possible point (inside the bounding box created by the x and y values of the vertices in screen space) and checking whether it's inside the triangle or not

This is done by using the barycentric coordinates of the point, since the barycentric coordinates all sum to 1, if one of the coordinates is below 0 it's not inside the triangle.

So we can calculate outside the loop the entire area of the triangle (since it's dependent on the vertices to begin with).

Then calculate the barycentric coordinates, divide it by the total area of the triangle and tada, we know if it's inside or not.

Pikumas rasterization video is good for learning about this (and more).

Since calculating the total area (by taking cross product of the three points) also gives us our normal vector we can implement basic illumination by checking if the normalized normal dotted with the light direction is > 0.

And then painting the vertex multiplied with the light intensity

## Lesson 3

Here we use the barycentric coordinates from lesson 2 and implement the light per pixel rather then per triangle.

We also used the barycentric coordinates to map the texture using uv coordinates to the image as well 
## Lesson 4

In this lesson i implemented perspective projection as well. 

What this means is that our screen has what's called the canonical view volume which is a cube like model of what we render on the screen. 

But in reality a camera doesn't show just a cube but rather what looks like a pyramid shape. So we need to make a transformation matrix to make a perspective transformation before we can make our orthographic projection

What this just means is that we need to take a 4x4 matrix that looks like 


[
1   0   0   0
0   1   0   0
0   0   1   0 
0   0  -1/c 1
]
to multiply with.

c here is the distance from the origin

(Skipping over the usage of homogeneous coordinates and the transformation back to 3D here)

## Lesson 5
Here we create the lookat function which creates a transformation matrix from (usually?) model view to camera space

This is done using the eye location, the center location and an up vector. 

With the final matrix end up looking like 
z = (eye-center).norm()
x = (up X z).norm()
y = (z X x).norm()
    [           [
    x0 x1 x2     0 0 -eye0   
A = y0 y1 y2  *  0 0 -eye1 
    z0 z1 z2     0 0 -eye2
    ]           ]

Then we also create the viewport matrix which is a transformation matrix from the canonical view volume to the position on the screen

## Lesson 6
Here we use two different normal maps, one with values in rgb that is mapped to xyz coordinates of the normal and one with coordinates in tangent space.

This mapping is done so we can map the normal for lighting on each indivudal pixel.

You sample the image at the uv coordinate position, do some transformation (in tagent space case) to get it the the correct normal.

## Lesson 7
Shadow mapping was done by essentially doing two passes of rendering, one from the position of the light source in which you gathered the distance (z) from the lightsource to the object.

This was done so that we can then use that to know how much light hit the object and to add some global illumination on the other pixels
## Lesson 8
