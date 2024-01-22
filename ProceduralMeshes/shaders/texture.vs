#version 400 

in vec3 inputPosition;
in vec2 inputTexCoord;
in float inputTime;

out vec2 texCoord;

#define M_PI 3.1415926535897932384626433832795

void main(void)
{
  float inputPeriod = 2;
  float inputAmplitude = 0.1;
  float inputSpeed = 0.1;
  vec3 inputOrigin = vec3(0.0f, 0.0f, 0.0f);

  vec3 p = inputPosition - inputOrigin;
  float d = length(p);
  float f = 2.0 * M_PI * inputPeriod * (d - inputSpeed * inputTime);

  vec3 newPosition = inputPosition + vec3(0.0, inputAmplitude * sin(f), 0.0);
  gl_Position = vec4(newPosition, 1.0f);

  texCoord = inputTexCoord;
}
