#include <glm.hpp>
#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <cstdlib>
#include "fluid.h"
#include "settings.h"

//From texture.cpp
GLuint create_empty_texture();
GLuint create_water_texture();

/*
	There are 4 presets for different simulations
	1: only water
	2: only lava
	3: water and lava
	4: 3 different fluids
*/
#define PRESET 1

inline float random(const float bottom, const float top) {
	return (float)rand() / (float)RAND_MAX * (top - bottom) + bottom;
}

inline void draw_fullscreen_rectangle() {
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(0.0, 1.0); glVertex2f(0.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex2f(SCREEN_WIDTH, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex2f(0.0, SCREEN_HEIGHT);
	glTexCoord2f(1.0, 0.0); glVertex2f(SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnd();
}


void loop() {
	//glfwInit();
	//GLFWwindow* window = glfwCreateWindow(720, 720, "LearnOpenGL", NULL, NULL);

	//glfwMakeContextCurrent(window);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//GLenum err = glewInit();


	//GLuint framebuffer = create_empty_texture();
	//GLuint water_texture = create_water_texture();
	//std::list<fluid_c> fluid;
	//unsigned int framecount = 0;
	//bool draw_next_frame = true;
	//double time = glfwGetTime();
	//for(int i=0; i<50; i++)
	//	fluid.push_back(
	//		fluid_c(5, 250, random(6, 12), random(-8, -5), //Spawn position and speed
	//			0.995, 0.85, 0.12, //Attributes
	//			0.1, 0.8, 0.9)); //Color
	//while (!glfwWindowShouldClose(window)) {
	//	framecount++;

	//	//Here we create some new fluid according to preset





	//	double mousex, mousey;
	//	//Handle mouse; drain/repel fluid if mouse buttons are pressed
	//	glfwGetCursorPos(window, &mousex, &mousey);

	//	for (std::list<fluid_c>::iterator i = fluid.begin(); i != fluid.end(); i++) {
	//		if (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT)==GLFW_PRESS) i->drain(mousex, mousey);
	//		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) i->repel(mousex, mousey);
	//		i->act(fluid);
	//	}

	//	//If the FPS gets low enough some frames won't be drawn
	//	//if (draw_next_frame) {
	//		glClear(GL_COLOR_BUFFER_BIT);

	//		//Draw fuild here
	//		glBindTexture(GL_TEXTURE_2D, water_texture);
	//		glBlendFunc(GL_ONE, GL_ONE); //Additive rendering
	//		for (std::list<fluid_c>::const_iterator i = fluid.begin(); i != fluid.end(); i++) i->draw();

	//		//Copy the screen into a texture
	//		glBindTexture(GL_TEXTURE_2D, framebuffer);
	//		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	//		//Draw a transparent black rectangle over the whole screen
	//		//Instead of just clearing the screen we now leave the original (non-metaball) image slightly visible to produce glow
	//		glDisable(GL_TEXTURE_2D);
	//		glColor4f(0.0, 0.0, 0.0, 0.7);
	//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//		draw_fullscreen_rectangle();
	//		glEnable(GL_TEXTURE_2D);

	//		//Redraw the copied screen and produce metaballs
	//		//Metaballs are produced simply by discarding pixels that don't have enough alpha
	//		glEnable(GL_ALPHA_TEST);
	//		glAlphaFunc(GL_GREATER, 0.85); //Discard treshold
	//		glColor4f(1.0, 1.0, 1.0, 1.0);
	//		glBlendFunc(GL_SRC_ALPHA, GL_ZERO); //Discard current screen pixels of pixels that are redrawn
	//		draw_fullscreen_rectangle();
	//		glDisable(GL_ALPHA_TEST);

	//		glfwSwapBuffers(window);
	//		glfwPollEvents();
	//	//}
	//
	//	//FPS handling
	//	draw_next_frame = true;
	//	time += 1.0 / 60.0; //FPS = 60
	//	const double sleep = time - glfwGetTime();
	//	//
	//	//if (sleep > 0.0) glfw(sleep);
	//	//else if (framecount % 4) draw_next_frame = false;
	//}
	//glDeleteTextures(1, &framebuffer);
	//glDeleteTextures(1, &water_texture);
}