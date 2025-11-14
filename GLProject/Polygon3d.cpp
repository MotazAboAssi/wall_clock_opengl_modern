#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <learnopengl/shader_m.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Polygon2d.cpp"

using namespace std;
using namespace glm;
void toString(vector<vec3> vertices)
{
	cout << "*********************\n";
	for (int i = 0; i < vertices.size(); i++)
	{
		cout << "(" << vertices.at(i).x << "," << vertices.at(i).y << "," << vertices.at(i).z << ")\n";
	}
	cout << "*********************\n";
}

float a = 0.5;
class Polygon3d
{

public:
	Polygon3d(vector<vec3> front_face, vec3 color)
	{
		vector_polygon = {};
		vector<vec3> back_face = {};
		for (int i = 0; i < front_face.size(); i++)
		{
			back_face.push_back(vec3(front_face.at(i).x, front_face.at(i).y, front_face.at(i).z + a));
		}

		vector_polygon.push_back(Polygon2d(front_face, color));
		vector_polygon.push_back(Polygon2d(back_face, color));

		// std::cout << "front :" <<  front_face.size() << std::endl;
		// toString(front_face);
		// std::cout << "back: "<< back_face.size() << std::endl;
		for (int i = 0; i < front_face.size() - 1; i++)
		{
			vector<vec3> v = {};
			v.push_back(front_face.at(i));
			v.push_back(back_face.at(i));
			v.push_back(back_face.at(i + 1));
			v.push_back(front_face.at(i + 1));
			// std::cout << "face "<< i << ": "<< back_face.size() << std::endl;
			// toString(v);
			vector_polygon.push_back(Polygon2d(v, vec3(0.5f+(i*0.01), 1.0f - (i * 0.01), 0.0f)));
		}
		vector<vec3> v = {};
		v.push_back(front_face.at(front_face.size() - 1));
		v.push_back(back_face.at(back_face.size() - 1));
		v.push_back(back_face.at(0));
		v.push_back(front_face.at(0));
		vector_polygon.push_back(Polygon2d(v, vec3(2, 0.0f, 1.0f)));
		// v.();
	}

	void transformation(mat4 t)
	{
		for (int i = 0; i < vector_polygon.size(); i++)
		{
			vector_polygon.at(i).transformation(t);
		}
	}
	void rotation(float angle, vec3 vec)
	{
		for (int i = 0; i < vector_polygon.size(); i++)
		{
			vector_polygon.at(i).transformation(rotate(vector_polygon.at(i).getMatrixModel(), angle, vec));
		}
	}
	void translation(vec3 vec)
	{
		for (int i = 0; i < vector_polygon.size(); i++)
		{
			vector_polygon.at(i).transformation(translate(vector_polygon.at(i).getMatrixModel(), vec));
		}
	}
	void scalation(vec3 vec)
	{
		for (int i = 0; i < vector_polygon.size(); i++)
		{
			vector_polygon.at(i).transformation(scale(vector_polygon.at(i).getMatrixModel(), vec));
		}
	}

	void draw(Shader &shader)
	{
		for (int i = 0; i < vector_polygon.size(); i++)
		{
			vector_polygon.at(i).draw(shader);
		}
	}

private:
	vector<Polygon2d> vector_polygon;
	// GLuint VAO;
	Polygon3d();
};
