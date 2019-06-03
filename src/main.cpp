
//#define GLFW

#ifdef GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#else
#include <QApplication>
#include "GUI/mainwindow.h"
#endif


int main(int argc, char *argv[])
{
#ifdef GLFW

#else
	QApplication a(argc, argv);

	MainWindow w;
	w.show();

	return a.exec();
#endif
}