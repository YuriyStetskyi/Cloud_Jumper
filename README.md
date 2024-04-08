# Cloud_Jumper

2D platformer game in OpenGL created by YuriyStetskyi

----------------------------------------------------------------------

# About

This project's framework was built with OpenGL v3.3 core, GLAD, GLFW for 
rendering in the window purposes , stb_image for loading sprites, 
and irrKlang v1.5 for audio. Shaders were written in GLSL.

Jump up on top of the clouds increasing your score and collect
stars to gain a powerful shield ability. Be wary of platfroms 
that break after you step on them and enemies that can destroy
you if you are not careful. You can shoot them or jump on top
of them to prevent your character from a certain demise.

----------------------------------------------------------------------

# How to run

To play the game run the Cloud_Jumper.exe file in x64/Debug or x64/Release.

or

Start Cloud_Jumper.sln and build + run the game from visual studio

or alternatively

You can visit my itch.io page and download the game from there:
https://yuriystetskyi.itch.io/Cloud_Jumper

----------------------------------------------------------------------

# Controls:

<- , -> , A , D - left and right movement
Left Mouse Button - shoot projectile that destroys enemies
Right Mouse Button - at cost of 20 stars deploy a shield that
makes player invulnerable for 5 seconds (excluding falling down)
Escape - leaves the game

You can change the games resolution by typing command in console: 
game -window WidthxHeight 

Or change window mode by typing:
game -window fullscreen
game -window windowed

### Project was built in approximately 2,5 weeks without prior OpenGL and rendering pipeline knowledge