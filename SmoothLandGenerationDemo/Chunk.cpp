#include "Chunk.h"
#include "Singletons.h"
#include <glm\gtx\transform.hpp>

//Global Externs
extern int CHUNK_SIZE;
extern int HEIGHT_CHUNK;
extern bool activeRender;
extern bool polyRender;
extern bool polyWireframe;
extern bool lowPolyRender;
extern int GLOBAL_SEED;

/****************************
* Constructor
****************************/
Chunk::Chunk():
	pos(0,0,0)
{

}
	
Chunk::Chunk(const glm::vec3& newpos):
	pos(newpos)
{
}

/****************************
* Destructor
****************************/
Chunk::~Chunk()
{
	glDeleteVertexArrays(10,terrainArray);
	glDeleteBuffers(10,terrainBuffer);
	glDeleteBuffers(10,terrainUV);
	glDeleteBuffers(10,terrainNormal);
}

/****************************
* TestRender
****************************/
void Chunk::Render()
{
	glm::mat4 modelMatrix(1.0f);
	modelMatrix=glm::translate(modelMatrix,pos);

	for(int column=0;column<10;++column)
	{
		glBindVertexArray(terrainArray[column]);
		glUniformMatrix4fv(ogl->modelMatrixID,1,GL_FALSE,&modelMatrix[0][0]);
		glUniformMatrix4fv(ogl->normalMatrixID,1,GL_FALSE,&normalMatrix[0][0]);
		glUniform1i(ogl->textureSamplerID,0);
		glDrawArrays(GL_TRIANGLE_STRIP,0,22);
	}
}

/****************************
* Generate - This function will generate a
				newly created chunk based on
				it's position and the world seed.
****************************/
void Chunk::Generate()
{
	//Generate height data
	for(int xPos(0);xPos<11;++xPos)
	{
		for(int zPos(0);zPos<11;++zPos)
		{
			heightData[xPos][zPos]=landGen->genValue(int(pos.x)+xPos,int(pos.z)+zPos,5214682);
		}
	}

	CompileOpenGLArrays();
}

void Chunk::CompileOpenGLArrays()
{
	glGenVertexArrays(10,terrainArray);
	glGenBuffers(10,terrainBuffer);
	glGenBuffers(10,terrainUV);
	glGenBuffers(10,terrainNormal);

	int uvCounter=0;
	float uv[44];

	//Generate UV data. UVs will be the same for each strip so only one data set is needed
	for(float uvIndex=0;uvIndex<11;++uvIndex)
	{
		uv[uvCounter]=uvIndex;
		++uvCounter;
		uv[uvCounter]=0;
		++uvCounter;

		uv[uvCounter]=uvIndex;
		++uvCounter;
		uv[uvCounter]=1;
		++uvCounter;
	}

	for(int column=0;column<10;++column)
	{
		int vertCounter=0;

		//Generate vertex data
		for(int xPos=0;xPos<11;++xPos)
		{
			vertices[column][vertCounter]=float(xPos);
			++vertCounter;
			vertices[column][vertCounter]=heightData[xPos][column];
			++vertCounter;
			vertices[column][vertCounter]=float(column);
			++vertCounter;

			vertices[column][vertCounter]=float(xPos);
			++vertCounter;
			vertices[column][vertCounter]=heightData[xPos][column+1];
			++vertCounter;
			vertices[column][vertCounter]=float(column+1);
			++vertCounter;
		}

		int triangleCounter=0;
		//Calculate the normals for each triangle
		for(int vertex=0;vertex<55;vertex+=6)
		{
			glm::vec3 v1(vertices[column][vertex],vertices[column][1+vertex],vertices[column][2+vertex]);
			glm::vec3 v2(vertices[column][3+vertex],vertices[column][4+vertex],vertices[column][5+vertex]);
			glm::vec3 v3(vertices[column][6+vertex],vertices[column][7+vertex],vertices[column][8+vertex]);
			glm::vec3 v4(vertices[column][9+vertex],vertices[column][10+vertex],vertices[column][11+vertex]);

			glm::vec3 normal1(v2-v1);
			glm::vec3 normal2(v3-v1);

			glm::vec3 tNormal=glm::cross(normal1,normal2);
			glm::normalize(tNormal);

			glm::vec3 normal3(v4-v3);
			glm::vec3 normal4(v4-v2);

			glm::vec3 tNormal2=glm::normalize(glm::cross(normal3,normal4));

			triangleNormals[column][triangleCounter]=tNormal;
			++triangleCounter;
			triangleNormals[column][triangleCounter]=tNormal2;
			++triangleCounter;
		}
		

		glBindVertexArray(terrainArray[column]);

		//Set the position buffer data
		glBindBuffer(GL_ARRAY_BUFFER, terrainBuffer[column]);
		glBufferData(GL_ARRAY_BUFFER, 66*sizeof(float), vertices[column], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		
		//Set the UV buffer data
		glBindBuffer(GL_ARRAY_BUFFER, terrainUV[column]);
		glBufferData(GL_ARRAY_BUFFER,44*sizeof(float),uv,GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	//Generate the normal matrix data
	normalMatrix=glm::transpose(glm::inverse(glm::mat4(1.0)));
}

void Chunk::generateNormals()
{
	glm::vec3 newNormal,newNormal2,newNormal3,newNormal4;

	//Column 0 specialcase
	newNormal=glm::normalize(triangleNormals[0][0]);
	newNormal2=glm::normalize(triangleNormals[1][0]+triangleNormals[0][0]+triangleNormals[0][1]);
	newNormal3=glm::normalize(triangleNormals[0][18]+triangleNormals[0][19]);
	newNormal4=glm::normalize(triangleNormals[1][18]+triangleNormals[1][19]+triangleNormals[0][19]);

	if(ZM){
		if(ZM->XM){
			newNormal=glm::normalize(triangleNormals[0][0]+
				ZM->getTriangleNormal(9,0)+ZM->getTriangleNormal(9,1)+
				ZM->XM->getTriangleNormal(9,19)+
				XM->getTriangleNormal(0,19)+XM->getTriangleNormal(0,18));
			newNormal2=glm::normalize(triangleNormals[1][0]+triangleNormals[0][0]+triangleNormals[0][1]+
				XM->getTriangleNormal(0,19));
		}
		else{
			if(XM){
				newNormal=glm::normalize(triangleNormals[0][0]+
					ZM->getTriangleNormal(9,0)+ZM->getTriangleNormal(9,1)+
					ZM->XM->getTriangleNormal(9,19)+
					XM->getTriangleNormal(0,19)+XM->getTriangleNormal(0,18));
				newNormal2=glm::normalize(triangleNormals[1][0]+triangleNormals[0][0]+triangleNormals[0][1]+
					XM->getTriangleNormal(0,19));
			}
			else{
				newNormal=glm::normalize(triangleNormals[0][0]+
					ZM->getTriangleNormal(9,0)+ZM->getTriangleNormal(9,1));
				newNormal2=glm::normalize(triangleNormals[1][0]+triangleNormals[0][0]+triangleNormals[0][1]);
			}
		}

		if(ZM->XP){
			newNormal3=glm::normalize(triangleNormals[0][18]+triangleNormals[0][19]+
				ZM->XP->getTriangleNormal(9,0)+ZM->XP->getTriangleNormal(9,1)+
				ZM->getTriangleNormal(9,19)+
				XP->getTriangleNormal(0,0));
			newNormal4=glm::normalize(triangleNormals[1][18]+triangleNormals[1][19]+triangleNormals[0][19]+
				XP->getTriangleNormal(0,0)+XP->getTriangleNormal(0,1));
		}
		else{
			if(XP){
				newNormal3=glm::normalize(triangleNormals[0][18]+triangleNormals[0][19]+
					ZM->XP->getTriangleNormal(9,0)+ZM->XP->getTriangleNormal(9,1)+
					ZM->getTriangleNormal(9,19)+
					XP->getTriangleNormal(0,0));
				newNormal4=glm::normalize(triangleNormals[1][18]+triangleNormals[1][19]+triangleNormals[0][19]+
					XP->getTriangleNormal(0,0)+XP->getTriangleNormal(0,1));
			}
			else{
				newNormal3=glm::normalize(triangleNormals[0][18]+triangleNormals[0][19]+
					ZM->getTriangleNormal(9,19));
				newNormal4=glm::normalize(triangleNormals[1][18]+triangleNormals[1][19]+triangleNormals[0][19]);
			}
		}
	}
	else{
		if(XM){
			newNormal=glm::normalize(triangleNormals[0][0]+
				XM->getTriangleNormal(0,19)+XM->getTriangleNormal(0,18));
			newNormal2=glm::normalize(triangleNormals[1][0]+triangleNormals[0][0]+triangleNormals[0][1]+
				XM->getTriangleNormal(0,19));
		}
		else{
			newNormal=glm::normalize(triangleNormals[0][0]);
			newNormal2=glm::normalize(triangleNormals[1][0]+triangleNormals[0][0]+triangleNormals[0][1]);
		}

		if(XP){
			newNormal3=glm::normalize(triangleNormals[0][18]+triangleNormals[0][19]+
				XP->getTriangleNormal(0,0));
			newNormal4=glm::normalize(triangleNormals[1][18]+triangleNormals[1][19]+triangleNormals[0][19]+
				XP->getTriangleNormal(0,0)+XP->getTriangleNormal(0,1));
		}
		else{
			newNormal3=glm::normalize(triangleNormals[0][18]+triangleNormals[0][19]);
			newNormal4=glm::normalize(triangleNormals[1][18]+triangleNormals[1][19]+triangleNormals[0][19]);
		}
	}

	normals[0][0]=newNormal.x;		normals[0][1]=newNormal.y;		normals[0][2]=newNormal.z;
	normals[0][3]=newNormal2.x;		normals[0][4]=newNormal2.y;		normals[0][5]=newNormal2.z;
	normals[0][60]=newNormal3.x;	normals[0][61]=newNormal3.y;	normals[0][62]=newNormal3.z;
	normals[0][63]=newNormal4.x;	normals[0][64]=newNormal4.y;	normals[0][65]=newNormal4.z;
	int triangleCounter=0;
	for(int normalCounter=6;normalCounter<60;normalCounter+=6)
	{
		if(ZM){
			newNormal=glm::normalize(triangleNormals[0][triangleCounter]+triangleNormals[0][1+triangleCounter]+triangleNormals[0][2+triangleCounter]+
				ZM->getTriangleNormal(9,triangleCounter+1)+ZM->getTriangleNormal(9,triangleCounter+2)+ZM->getTriangleNormal(9,triangleCounter+3));
		}
		else{
			newNormal=glm::normalize(triangleNormals[0][triangleCounter]+triangleNormals[0][1+triangleCounter]+triangleNormals[0][2+triangleCounter]);
		}
		normals[0][normalCounter]=newNormal.x;
		normals[0][1+normalCounter]=newNormal.y;
		normals[0][2+normalCounter]=newNormal.z;

		++triangleCounter;
			
		newNormal=glm::normalize(triangleNormals[0][triangleCounter]+triangleNormals[0][1+triangleCounter]+triangleNormals[0][2+triangleCounter]
										+triangleNormals[1][triangleCounter-1]+triangleNormals[1][triangleCounter]+triangleNormals[1][1+triangleCounter]);

		normals[0][3+normalCounter]=newNormal.x;
		normals[0][4+normalCounter]=newNormal.y;
		normals[0][5+normalCounter]=newNormal.z;

		++triangleCounter;
	}
		
	
	//Standard Columns
	for(int column=1;column<9;++column)
	{
		if(XM)
		{
			newNormal=glm::normalize(triangleNormals[column-1][0]+triangleNormals[column-1][1]+triangleNormals[column][0]+
				XM->getTriangleNormal(column,18)+XM->getTriangleNormal(column,19)+XM->getTriangleNormal(column-1,19));
			newNormal2=glm::normalize(triangleNormals[1+column][0]+triangleNormals[column][0]+triangleNormals[column][1]+
				XM->getTriangleNormal(column,19)+XM->getTriangleNormal(column+1,18)+XM->getTriangleNormal(column+1,19));
		}
		else
		{
			newNormal=glm::normalize(triangleNormals[column-1][0]+triangleNormals[column-1][1]+triangleNormals[column][0]);
			newNormal2=glm::normalize(triangleNormals[1+column][0]+triangleNormals[column][0]+triangleNormals[column][1]);
		}
		normals[column][0]=newNormal.x;		normals[column][1]=newNormal.y;		normals[column][2]=newNormal.z;
		normals[column][3]=newNormal2.x;	normals[column][4]=newNormal2.y;	normals[column][5]=newNormal2.z;

		int triangleCounter=0;
		for(int normalCounter=6;normalCounter<60;normalCounter+=6)
		{
			newNormal=glm::normalize(triangleNormals[column][triangleCounter]+triangleNormals[column][1+triangleCounter]+triangleNormals[column][2+triangleCounter]
											+triangleNormals[column-1][1+triangleCounter]+triangleNormals[column-1][2+triangleCounter]+triangleNormals[column-1][3+triangleCounter]);

			normals[column][normalCounter]=newNormal.x;
			normals[column][1+normalCounter]=newNormal.y;
			normals[column][2+normalCounter]=newNormal.z;

			++triangleCounter;

			newNormal=glm::normalize(triangleNormals[column][triangleCounter]+triangleNormals[column][1+triangleCounter]+triangleNormals[column][2+triangleCounter]
											+triangleNormals[column+1][triangleCounter-1]+triangleNormals[column+1][triangleCounter]+triangleNormals[column+1][1+triangleCounter]);

			normals[column][3+normalCounter]=newNormal.x;
			normals[column][4+normalCounter]=newNormal.y;
			normals[column][5+normalCounter]=newNormal.z;

			++triangleCounter;
		}
		
		if(XP){
			newNormal=glm::normalize(triangleNormals[column-1][19]+triangleNormals[column][18]+triangleNormals[column][19]+
				XP->getTriangleNormal(column,0)+XP->getTriangleNormal(column-1,0)+XP->getTriangleNormal(column-1,1));
			newNormal2=glm::normalize(triangleNormals[1+column][18]+triangleNormals[1+column][19]+triangleNormals[column][19]+
				XP->getTriangleNormal(column,0)+XP->getTriangleNormal(column,1)+XP->getTriangleNormal(column+1,0));
		}
		else{
			newNormal=glm::normalize(triangleNormals[column-1][19]+triangleNormals[column][18]+triangleNormals[column][19]);
			newNormal2=glm::normalize(triangleNormals[1+column][18]+triangleNormals[1+column][19]+triangleNormals[column][19]);
		}
		normals[column][60]=newNormal.x;	normals[column][61]=newNormal.y;	normals[column][62]=newNormal.z;
		normals[column][63]=newNormal2.x;	normals[column][64]=newNormal2.y;	normals[column][65]=newNormal2.z;
	}
	
	//Column 9 specialcase	
	if(ZP){
		if(ZP->XM){
			newNormal=glm::normalize(triangleNormals[8][0]+triangleNormals[8][1]+triangleNormals[9][0]+
				XM->getTriangleNormal(9,18)+XM->getTriangleNormal(9,19));
			newNormal2=glm::normalize(triangleNormals[9][0]+triangleNormals[9][1]+
				ZP->getTriangleNormal(0,1)+
				ZP->XM->getTriangleNormal(0,19)+ZP->XM->getTriangleNormal(0,18)+
				XM->getTriangleNormal(9,19));
		}
		else{
			if(XM){
				newNormal=glm::normalize(triangleNormals[8][0]+triangleNormals[8][1]+triangleNormals[9][0]+
					XM->getTriangleNormal(9,18)+XM->getTriangleNormal(9,19));
				newNormal2=glm::normalize(triangleNormals[9][0]+triangleNormals[9][1]+
					ZP->getTriangleNormal(0,1)+
					ZP->XM->getTriangleNormal(0,19)+ZP->XM->getTriangleNormal(0,18)+
					XM->getTriangleNormal(9,19));
			}
			else{
				newNormal=glm::normalize(triangleNormals[8][0]+triangleNormals[8][1]+triangleNormals[9][0]);
				newNormal2=glm::normalize(triangleNormals[9][0]+triangleNormals[9][1]+
					ZP->getTriangleNormal(0,1));
			}
		}

		if(ZP->XP){
			newNormal3=glm::normalize(triangleNormals[8][19]+triangleNormals[9][18]+triangleNormals[9][19]+
				XP->getTriangleNormal(9,0)+XP->getTriangleNormal(8,0)+XP->getTriangleNormal(8,1));

			newNormal4=glm::normalize(triangleNormals[9][19]+
				ZP->getTriangleNormal(0,19)+
				ZP->XP->getTriangleNormal(0,0)+ZP->XP->getTriangleNormal(0,1)+
				XP->getTriangleNormal(9,0)+XP->getTriangleNormal(9,1));
		}
		else{
			if(XP){
				newNormal3=glm::normalize(triangleNormals[8][19]+triangleNormals[9][18]+triangleNormals[9][19]+
					XP->getTriangleNormal(9,0)+XP->getTriangleNormal(8,0)+XP->getTriangleNormal(8,1));

				newNormal4=glm::normalize(triangleNormals[9][19]+
					ZP->getTriangleNormal(0,19)+
					XP->getTriangleNormal(9,0)+XP->getTriangleNormal(9,1));
			}
			else{
				newNormal3=glm::normalize(triangleNormals[8][19]+triangleNormals[9][18]+triangleNormals[9][19]);

				newNormal4=glm::normalize(triangleNormals[9][19]+ZP->getTriangleNormal(0,19));
			}
		}
	}
	else{
		if(XM){
			newNormal=glm::normalize(triangleNormals[8][0]+triangleNormals[8][1]+triangleNormals[9][0]+
				XM->getTriangleNormal(9,18)+XM->getTriangleNormal(9,19));
			newNormal2=glm::normalize(triangleNormals[9][0]+triangleNormals[9][1]+
				XM->getTriangleNormal(9,19));
		}
		else{
			newNormal=glm::normalize(triangleNormals[8][0]+triangleNormals[8][1]+triangleNormals[9][0]);
			newNormal2=glm::normalize(triangleNormals[9][0]+triangleNormals[9][1]);
		}

		if(XP){
			newNormal3=glm::normalize(triangleNormals[8][19]+triangleNormals[9][18]+triangleNormals[9][19]+
				XP->getTriangleNormal(9,0)+XP->getTriangleNormal(8,0)+XP->getTriangleNormal(8,1));

			newNormal4=glm::normalize(triangleNormals[9][19]+
				XP->getTriangleNormal(9,0)+XP->getTriangleNormal(9,1));
		}
		else{
			newNormal3=glm::normalize(triangleNormals[8][19]+triangleNormals[9][18]+triangleNormals[9][19]);
			newNormal4=glm::normalize(triangleNormals[9][19]);
		}
	}

	normals[9][0]=newNormal.x;		normals[9][1]=newNormal.y;		normals[9][2]=newNormal.z;
	normals[9][3]=newNormal2.x;		normals[9][4]=newNormal2.y;		normals[9][5]=newNormal2.z;
	normals[9][60]=newNormal3.x;	normals[9][61]=newNormal3.y;	normals[9][62]=newNormal3.z;
	normals[9][63]=newNormal4.x;	normals[9][64]=newNormal4.y;	normals[9][65]=newNormal4.z;

	triangleCounter=0;
	for(int normalCounter=6;normalCounter<60;normalCounter+=6)
	{
		newNormal=glm::normalize(triangleNormals[9][triangleCounter]+triangleNormals[9][1+triangleCounter]+triangleNormals[9][2+triangleCounter]
											+triangleNormals[8][1+triangleCounter]+triangleNormals[8][2+triangleCounter]);

		normals[9][normalCounter]=newNormal.x;
		normals[9][1+normalCounter]=newNormal.y;
		normals[9][2+normalCounter]=newNormal.z;

		++triangleCounter;

		if(ZP)
		{
			newNormal=glm::normalize(triangleNormals[9][triangleCounter]+triangleNormals[9][1+triangleCounter]+triangleNormals[9][2+triangleCounter]+
				ZP->getTriangleNormal(0,triangleCounter-1)+ZP->getTriangleNormal(0,triangleCounter)+ZP->getTriangleNormal(0,triangleCounter+1));
		}
		else
		{
			newNormal=glm::normalize(triangleNormals[9][triangleCounter]+triangleNormals[9][1+triangleCounter]+triangleNormals[9][2+triangleCounter]);
		}

		normals[9][3+normalCounter]=newNormal.x;
		normals[9][4+normalCounter]=newNormal.y;
		normals[9][5+normalCounter]=newNormal.z;

		++triangleCounter;
	}
	
	//Sends the normal data to GPU
	for(int column=0;column<10;++column)
	{
		glBindVertexArray(terrainArray[column]);

		//Set the normal buffer data
		glBindBuffer(GL_ARRAY_BUFFER, terrainNormal[column]);
		glBufferData(GL_ARRAY_BUFFER, 66*sizeof(float), normals[column], GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}
}

void Chunk::AssignNeighboringChunks(Chunk* zp, Chunk* zm, Chunk* xp, Chunk* xm)
{
	ZP=zp;
	ZM=zm;
	XP=xp;
	XM=xm;
}