#include "ChunkManager.h"
#include "Singletons.h"

ChunkManager* ChunkManager::Instance()
{
	static ChunkManager instance;
	return &instance;
}

/****************************
* Constructor
****************************/
ChunkManager::ChunkManager():
	RenderDistance(20),
	p_chunks(0)
{
	p_chunks=new Chunk*[RenderDistance];

	for(int i=0;i<RenderDistance;i++)
	{
		p_chunks[i]=new Chunk[RenderDistance];
	}

	int center=RenderDistance/2;

	for(int xPos=0;xPos<RenderDistance;++xPos)
	{
		for(int zPos=0;zPos<RenderDistance;++zPos)
		{
			p_chunks[xPos][zPos].SetPos(glm::vec3(10*xPos-(10*center),0,10*zPos-(10*center)));
			p_chunks[xPos][zPos].Generate();
		}
	}

	SetupNeighborChunkData();

	for(int xPos=0;xPos<RenderDistance;++xPos)
	{
		for(int zPos=0;zPos<RenderDistance;++zPos)
		{
			p_chunks[xPos][zPos].generateNormals();
		}
	}

	landTexture.LoadTexture("Dirt.tga");
}

/****************************
* Update
****************************/
ChunkManager::~ChunkManager()
{
	//Unload the chunks
	for(int i=0;i<RenderDistance;++i)
	{
		delete [] p_chunks[i];
	}

	delete [] p_chunks;
}

/****************************
* RenderDistanceChange -This function is called when the user changes the render distance within the configuration. This will rebuild the chunk array to the
						appropriate size. This can be expensive because of the dynamic memory clearing and re-assigning.
****************************/
void ChunkManager::RenderDistanceChange(const int& newDist)
{
	//Unload the chunks
	for(int i=0;i<RenderDistance;++i)
	{
		delete [] p_chunks[i];
	}

	delete [] p_chunks;

	p_chunks=0;

	//sets the new render distance
	RenderDistance=newDist;

	//uses the ChunkManager constructor to rebuild the dynamic chunk array with the new render distance
	ChunkManager();
}

/****************************
* Render - Cycles through the chunks and renders them to the scene
****************************/
void ChunkManager::Render()
{
	//Load the texture for land as the active texture
	landTexture.UseTexture();
	
	glUniformMatrix4fv(ogl->cameraID,1,GL_FALSE,&ogl->camera.getViewMatrix()[0][0]);
	glUniformMatrix4fv(ogl->projectionID,1,GL_FALSE,&ogl->camera.getPerspectiveMatrix()[0][0]);

	//Go through all chunks
	for(int i=0;i<RenderDistance;i++)
	{
		for(int j=0;j<RenderDistance;j++)
		{
			p_chunks[i][j].Render();
		}
	}
}

/****************************
* SetupNeighborChunkData - Sets up each chunk's neighbor pointers so chunks will have access to their neighbors for checking data
****************************/
void ChunkManager::SetupNeighborChunkData()
{
	if(RenderDistance>1)
	{
		//Set the corner chunks
		p_chunks[0][0].AssignNeighboringChunks(&p_chunks[0][1],0,&p_chunks[1][0],0);
		p_chunks[0][RenderDistance-1].AssignNeighboringChunks(0,&p_chunks[0][RenderDistance-2],&p_chunks[1][RenderDistance-1],0);
		p_chunks[RenderDistance-1][0].AssignNeighboringChunks(&p_chunks[RenderDistance-1][1],0,0,&p_chunks[RenderDistance-2][0]);
		p_chunks[RenderDistance-1][RenderDistance-1].AssignNeighboringChunks(0,&p_chunks[RenderDistance-1][RenderDistance-2],0,&p_chunks[RenderDistance-2][RenderDistance-1]);

		//Set the side chunks
		for(int xPos=1;xPos<RenderDistance-1;++xPos)
		{
			p_chunks[xPos][0].AssignNeighboringChunks(&p_chunks[xPos][1],0,&p_chunks[xPos+1][0],&p_chunks[xPos-1][0]);
			p_chunks[xPos][RenderDistance-1].AssignNeighboringChunks(0,&p_chunks[xPos][RenderDistance-2],&p_chunks[xPos+1][RenderDistance-1],&p_chunks[xPos-1][RenderDistance-1]);
			p_chunks[0][xPos].AssignNeighboringChunks(&p_chunks[0][xPos+1],&p_chunks[0][xPos-1],&p_chunks[1][xPos],0);
			p_chunks[RenderDistance-1][xPos].AssignNeighboringChunks(&p_chunks[RenderDistance-1][xPos+1],&p_chunks[RenderDistance-1][xPos-1],0,&p_chunks[RenderDistance-2][xPos]);
		}

		//Set the middle chunks
		for(int xPos=1;xPos<RenderDistance-1;++xPos)
		{
			for(int zPos=1;zPos<RenderDistance-1;++zPos)
			{
				p_chunks[xPos][zPos].AssignNeighboringChunks(&p_chunks[xPos][zPos+1],&p_chunks[xPos][zPos-1],&p_chunks[xPos+1][zPos],&p_chunks[xPos-1][zPos]);
			}
		}
	}
	else
	{
		//Only 1 chunk so no neighbors
		p_chunks[0][0].AssignNeighboringChunks(0,0,0,0);
	}
}