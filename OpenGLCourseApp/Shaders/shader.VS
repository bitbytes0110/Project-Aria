#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

out vec4 vCol;
out vec2 TexCoord;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4( pos, 1.0);
	vCol= vec4( clamp( pos, 0.0f, 1.0f), 1.0f);

	TexCoord = tex;

	Normal = mat3(transpose(inverse(model))) * norm;
	
	//	Note: Lighting changes with respect to the orientation of the object
	//	Position doesn't matter for lighting as the normals won't change when an object it moved
	//	But Rotation and scaling would change the lighting!
	//	Hence we have to multiply model matrix (4x4)along with it
	//	Fortunately the scaling and rotation transforms are done in the top left 3x3. 
	//	So to make it compatible with the normals(vec3) we can convert the (4x4) model matrix into a (3x3) matrix!

	//	Also while non uniform scaling the normals deteriorate, to overcome it is to invert the scaling process and then transpose it 

}