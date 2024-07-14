#version 450 core



layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;


layout(std140, binding = 4) buffer Pos
{
  vec3 Positions[];
  vec2 Uvs[];  
};

void main()
{



}
