#version 460 core
layout(location = 0) in vec3 vertexPositions;
//layout(location = 1) in vec2 texCoord;
layout(location = 1) in vec3 vertexNormal;

out vec4 v_TexColor;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

uniform float GrassSlopeThreshold;
uniform float GrassBlendAmount;

vec3 GrassColor = vec3(0.1, 0.66, 0.1);
vec3 SoilColor = vec3(0.4, 0.3, 0.05);

void main()

{
	vec3 p = normalize(vec3(100, 200, 0) - vertexPositions);
	float diff = dot(p, vertexNormal);
	float slope = 1 - vertexNormal.y;
	float grassBlendHeight = GrassSlopeThreshold * (1 - GrassBlendAmount);
	float grassWeight = 1 - clamp((slope - grassBlendHeight) / (GrassSlopeThreshold - grassBlendHeight), 0.0, 1.0);
	vec3 color = (GrassColor * grassWeight + SoilColor * (1 - grassWeight)) * clamp(diff, 0.1, 1.0);
	v_TexColor = vec4(color, 1.0);

	gl_Position = projMatrix*viewMatrix*modelMatrix*vec4(vertexPositions, 1.0);
}