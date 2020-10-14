#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include <vector>
#include "SutherlandHodgeman.h"

VertexArray inVertexArray;
VertexArray outVertexArray;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void clear() {

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearDepth(1.0);//指定深度缓冲区中每个像素需要的值
	glClear(GL_DEPTH_BUFFER_BIT);//清除深度缓冲区
	glLoadIdentity();
}
void drawPolygon(float xl, float xr, float yb, float yt) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_POLYGON);
	glColor3f(0.0, 1.0, 0.0);    // Green
	glVertex3f(xl, yb, 0.0);
	glVertex3f(xr, yb, 0.0);
	glVertex3f(xr, yt, 0.0);
	glVertex3f(xl, yt, 0.0);
	//printf("polygon");
	glEnd();
}

int main() {
	//sutherHodgeman多边形裁剪, 按下方向上键裁剪
	Vertex v1, v2, v3, rect1, rect2;	//三角形，v1,v3是底角，v2顶角
	v1.x = -1;
	v1.y = v3.y = -0.9;
	v2.y = v3.x = 1;
	v2.x = 0;
	rect1.x = -0.5;
	rect1.y = -0.8;
	rect2.x = 0.5;
	rect2.y = 0.5;
	VertexArray clipBoundary;
	clipBoundary.push_back(rect1);
	clipBoundary.push_back(rect2);

	VertexArray temp = { v1,v2,v3 };
	inVertexArray = temp;

	int inLength = 3;
	int outLength = 0;
	outVertexArray = SutherlandHodgeClip(inVertexArray, outVertexArray, clipBoundary, inLength, outLength);

	/*for (auto i : outVertexArray) {
		std::cout << i.x << " " << i.y << std::endl;
	}*/

	GLFWwindow* window;
	if (!glfwInit())
		return -1;
	window = glfwCreateWindow(480, 320, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		//display(originLine, xl, xr, yb, yt);
		clear();
		drawPolygon(rect1.x, rect2.x, rect1.y, rect2.y);
		drawVertexArray(inVertexArray);
		//glFlush();
		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_UP) {
		inVertexArray.clear();
		inVertexArray = outVertexArray;
	}
}