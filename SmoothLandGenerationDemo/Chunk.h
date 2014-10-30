#ifndef Chunk_H
#define Chunk_H

#include <gl\glew.h>
#include <gl\GL.h>
#include <glm\glm.hpp>


class Chunk
{
public:
	Chunk();
	Chunk(const glm::vec3& pos);
	~Chunk();

	void SetPos(const glm::vec3& newPos) {pos=newPos;};
	void Render();
	void Generate();
	glm::vec3 getTriangleNormal(const int& index1, const int&index2){return triangleNormals[index1][index2];};
	void AssignNeighboringChunks(Chunk* zp, Chunk* zm, Chunk* xp, Chunk* xm);
	void generateNormals();
	Chunk *ZP,*ZM,*XP,*XM;
private:
	glm::vec3 pos;
	float vertices[10][66];
	float normals[10][66];
	glm::vec3 triangleNormals[10][20];
	
	float heightData[11][11];

	void CompileOpenGLArrays();
	GLuint terrainArray[10];
	GLuint terrainBuffer[10];
	GLuint terrainUV[10];
	GLuint terrainNormal[10];
	glm::mat4 normalMatrix;
};

#endif