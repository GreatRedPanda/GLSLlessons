#include "Scene.h"

void Scene::loop(GLFWwindow* window, Camera& mainCamera, RenderLoop timingCalc)
{

	while (!glfwWindowShouldClose(window))
	{

		timingCalc(window);
		for(auto &r: sceneRenderersHandlers)
		{
			r->renderLoop( mainCamera);
		
		}
		glfwSwapBuffers(window);
		glfwPollEvents();

	}
}
