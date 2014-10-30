#ifndef ChunkManager_H
#define ChunkManager_H

#include "Chunk.h"
#include "Texture.h"

class ChunkManager
{
private:
	ChunkManager();
	~ChunkManager();
	int RenderDistance;
public:
	static ChunkManager* Instance();

	Chunk  **p_chunks;
	void RenderDistanceChange(const int& newDist);
	void Render();
	void Startup();

	Texture landTexture;
	void SetupNeighborChunkData();
};

#endif