/*
    NOTE FROM DEVELOPER:
    Because i previously made this game on company provided
    framework which had coordinate system that started in top-left 
    corner - some changes to rendering code had to be made:
    That is why here im getting a NEGATIVE of Y position and in my 
    texture loading im NOT using stbi_set_flip_vertically_on_load.
*/


#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 texCoord;
uniform float offsetX;
uniform float offsetY;

void main()
{
    gl_Position = vec4(aPos.x + offsetX, -aPos.y + offsetY, aPos.z, 1.0);
    ourColor = aColor;
    texCoord = aTexCoord;
}