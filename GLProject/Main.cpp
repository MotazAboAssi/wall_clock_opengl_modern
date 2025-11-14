// GLProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <learnopengl/shader_m.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Polygon2d.cpp"
#include "Polygon3d.cpp"

using namespace glm;
using namespace std;

vector<Polygon2d> convert3d(vector<vec3> front_face);
void render_vector_polygon(vector<Polygon2d> polygons, Shader &ourShader);
void drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides, vector<vec3> &arr);
void processInput(GLFWwindow *window);

int width = 800;
int height = 600;

vec3 cameraPos = vec3(0.0f, 0.0f, 2.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

GLfloat speedrotation = 1;

int main()
{
	glfwInit();
	GLFWwindow *window = glfwCreateWindow(width, height, "Lecture 3", NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glEnable(GL_DEPTH_TEST);

	Shader ourShader("./shaders/vs/L3.vs", "./shaders/fs/L3.fs");

	glfwSwapInterval(1);

	// ******* create object *******
	vector<vec3> vertices_cylinder = {};
	drawCircle(0, 0, 0, 0.5, 30, vertices_cylinder);
	Polygon3d cylinder = Polygon3d(vertices_cylinder, vec3(1.0f, 0.0f, 1.0f));
	//
	GLfloat a_line = 0.05f;
	vector<vec3> vertices_line = {};
	vertices_line.push_back(vec3(a_line - 0.040, a_line * 5, 0.0f));
	vertices_line.push_back(vec3(-a_line + 0.040, a_line * 5, 0.0f));
	vertices_line.push_back(vec3(-a_line + 0.040, -a_line * 5, 0.0f));
	vertices_line.push_back(vec3(a_line - 0.040, -a_line * 5, 0.0f));
	Polygon3d line = Polygon3d(vertices_line, vec3(0.0f, 1.0f, 1.0f));
	//
	GLfloat a_cube = 0.5f;
	vector<vec3> vertices_cube = {};
	vertices_cube.push_back(vec3(a_cube, a_cube, 0.0f));
	vertices_cube.push_back(vec3(-a_cube, a_cube, 0.0f));
	vertices_cube.push_back(vec3(-a_cube, -a_cube, 0.0f));
	vertices_cube.push_back(vec3(a_cube, -a_cube, 0.0f));
	Polygon3d cube = Polygon3d(vertices_cube, vec3(1.0f, 1.0f, 0.0f));
	//
	vector<vec3> vertices_clock_hand = {};
	vertices_clock_hand.push_back(vec3(0.005f, 0.0f, 0.0f));
	vertices_clock_hand.push_back(vec3(0.005f, a_cube, 0.0f));
	vertices_clock_hand.push_back(vec3(0.0f, a_cube + 0.05f, 0.0f));
	vertices_clock_hand.push_back(vec3(-0.005f, a_cube, 0.0f));
	vertices_clock_hand.push_back(vec3(-0.005f, 0.0f, 0.0f));
	Polygon3d clock_hand = Polygon3d(vertices_clock_hand, vec3(1.0f, 0.0f, 0.0f));
	//
	vector<vec3> vertices_clock_hand_screw = {};
	drawCircle(0, 0, 0, 0.5, 30, vertices_clock_hand_screw);
	Polygon3d clock_hand_screw = Polygon3d(vertices_clock_hand_screw, vec3(0.0f, 0.0f, 1.0f));

	// **************

	ourShader.use();

	mat4 projection = mat4(1.0f);
	projection = perspective(radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
	ourShader.setMat4("projection", projection);

	while (!glfwWindowShouldClose(window))
	{
		GLfloat seconds_clock = -(float)glfwGetTime() / (9.6f * 2);
		GLfloat seconds_cylinder = -(float)glfwGetTime() / 9.6f * 30.0f;
		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mat4 view = mat4(1.0f);
		view = translate(view, vec3(0.0f, 0.0f, -4.0f));
		view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		ourShader.setMat4("view", view);

		mat4 transformation = mat4(1.0f);

		// ******* call object with draw *******
		cube.transformation(transformation);
		cube.scalation(vec3(0.35,0.35,0.5));
		cube.translation(vec3(0, 0, -0.25));
		cube.draw(ourShader);
		//
		cylinder.transformation(transformation);
		cylinder.rotation(-(float)(cos(seconds_cylinder) / 3), vec3(0.0f, 0.0f, 1.0f));
		cylinder.translation(vec3(0, -a_line * 11.0f, -0.01));
		cylinder.scalation(vec3(0.16, 0.16, 0.08));
		cylinder.draw(ourShader);
		//
		cylinder.transformation(transformation);
		cylinder.rotation(-(float)(cos(seconds_cylinder) / 3), vec3(0.0f, 0.0f, 1.0f));
		cylinder.translation(vec3(0, -0.15, 0.01));
		cylinder.rotation(M_PI*0.5, vec3(1.0f, 0.0f, 0.0f));
		
		cylinder.scalation(vec3(0.01, 0.01, 1));
		cylinder.draw(ourShader);
		// line.transformation(transformation);
		// line.rotation(-(float)(cos(seconds_cylinder) / 3), vec3(0.0f, 0.0f, 1.0f));
		// line.translation(vec3(0, -a_line * 8.5f, 0));
		// line.scalation(vec3(1, 1, 0.05));
		// line.draw(ourShader);
		// clocks_handle
		// **************
		// second
		clock_hand.transformation(transformation);
		clock_hand.rotation(seconds_clock, vec3(0.0f, 0.0f, 1.0f));
		clock_hand.translation(vec3(0.0f, 0.0f, 0.13f));
		clock_hand.scalation(vec3(1, 0.3, 0.005));
		clock_hand.draw(ourShader);
		// minute
		clock_hand.transformation(transformation);
		clock_hand.rotation(seconds_clock / 60, vec3(0.0f, 0.0f, 1.0f));
		clock_hand.translation(vec3(0.0f, 0.0f, 0.135f));
		clock_hand.scalation(vec3(1, 0.25, 0.005));
		clock_hand.draw(ourShader);
		// hour
		clock_hand.transformation(transformation);
		clock_hand.rotation(seconds_clock / 3600, vec3(0.0f, 0.0f, 1.0f));
		clock_hand.translation(vec3(0.0f, 0.0f, 0.14f));
		clock_hand.scalation(vec3(1, 0.20, 0.005));
		clock_hand.draw(ourShader);
		//
		cylinder.transformation(transformation);
		cylinder.translation(vec3(0.0f, 0.0f, 0.12f));
		cylinder.scalation(vec3(0.04, 0.04, 0.05));
		cylinder.draw(ourShader);

		// **************
		//

		// *******************************
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
vector<Polygon2d> convert3d(vector<vec3> front_face)
{
	vector<Polygon2d> vector_polygon = {};
	vector<vec3> back_face = {};
	for (int i = 0; i < front_face.size(); i++)
	{
		back_face.push_back(vec3(front_face.at(i).x, front_face.at(i).y, front_face.at(i).z + 0.1));
	}

	vector_polygon.push_back(Polygon2d(front_face, vec3(0.0f, 1.0f, 1.0f)));
	vector_polygon.push_back(Polygon2d(back_face, vec3(0.0f, 1.0f, 1.0f)));

	for (int i = 0; i < front_face.size() - 1; i++)
	{
		vector<vec3> v = {};
		v.push_back(front_face.at(i));
		v.push_back(back_face.at(i));
		v.push_back(back_face.at(i + 1));
		v.push_back(front_face.at(i + 1));
		vector_polygon.push_back(Polygon2d(v, vec3(1.0f, 1.0f, 1.0f)));
	}

	return vector_polygon;
}

void render_vector_polygon(vector<Polygon2d> polygons, Shader &ourShader)
{
	for (int i = 0; i < polygons.size(); i++)
	{
		polygons.at(i).draw(ourShader);
	}
}

void drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides, vector<vec3> &arr)
{
	int numberOfVertices = numberOfSides + 2;

	GLfloat twicePi = 2.0f * M_PI;

	GLfloat circleVerticesX[numberOfVertices];
	GLfloat circleVerticesY[numberOfVertices];
	GLfloat circleVerticesZ[numberOfVertices];

	circleVerticesX[0] = x;
	circleVerticesY[0] = y;
	circleVerticesZ[0] = z;

	for (int i = 1; i < numberOfVertices; i++)
	{
		circleVerticesX[i] = x + (radius * cos(i * twicePi / numberOfSides));
		circleVerticesY[i] = y + (radius * sin(i * twicePi / numberOfSides));
		circleVerticesZ[i] = z;
	}

	GLfloat allCircleVertices[(numberOfVertices) * 3];

	for (int i = 0; i < numberOfVertices; i++)
	{
		arr.push_back(vec3(circleVerticesX[i], circleVerticesY[i], circleVerticesZ[i]));
	}
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		cameraPos = vec3(0.0f, 0.0f, 4.0f);
		cameraFront = vec3(0.0f, 0.0f, -1.0f);
		cameraUp = vec3(0.0f, 1.0f, 0.0f);
	}
	const float cameraSpeed = 0.02f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		cameraPos += cross(cameraFront, normalize(cross(cameraFront, cameraUp))) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		cameraPos -= cross(cameraFront, normalize(cross(cameraFront, cameraUp))) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		mat4 rotationMatrix = rotate(mat4(1.0f), 0.01f, vec3(0.0f, 1.0f, 0.0f));
		vec4 rotatedVectorHomogeneous = rotationMatrix * vec4(cameraFront, 1.0f);
		cameraFront = vec3(rotatedVectorHomogeneous);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		mat4 rotationMatrix = rotate(mat4(1.0f), 0.01f, vec3(0.0f, -1.0f, 0.0f));
		vec4 rotatedVectorHomogeneous = rotationMatrix * vec4(cameraFront, 1.0f);
		cameraFront = vec3(rotatedVectorHomogeneous);
	}
}