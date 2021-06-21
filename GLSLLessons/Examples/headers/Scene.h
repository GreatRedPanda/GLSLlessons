#pragma once
#include "OpenGLHeaders.h"
#include "RenderersHandler.h"
//#include <functional>
class Scene
{

public: 
	//std::function<void()> LoopFunc;
	std::vector<RenderersHandler *> sceneRenderersHandlers = std::vector<RenderersHandler *>();
	 void  loop(GLFWwindow* window, Camera& mainCamera, RenderLoop timingCalc);
	 
};

