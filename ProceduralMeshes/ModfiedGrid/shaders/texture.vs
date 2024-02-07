#version 400 

in vec3 inputPosition;
in vec2 inputTexCoord;
in float inputTime;
in vec3  inputNormal;
in vec3  inputTangent;
in vec3  inputBinormal;

out vec2 texCoord;
out vec3 normal;
out vec3 tangent;
out vec3 binormal;

#define M_PI 3.1415926535897932384626433832795

void main(void)
{
  float inputPeriod = 1;
  float inputAmplitude = 0.2;
  float inputSpeed = 0.1;
  vec3 inputOrigin = vec3(0.5f, 0.0f, 0.0f);

  vec3 p = inputPosition - inputOrigin;
  float d = length(p);
  float f = 2.0 * M_PI * inputPeriod * (d - inputSpeed * inputTime);

  vec3 newPosition = inputPosition + vec3(0.0, inputAmplitude * sin(f), 0.0);
  gl_Position = vec4(newPosition, 1.0f);

  texCoord = inputTexCoord;
}
