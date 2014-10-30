#ifndef LandGen_H
#define LandGen_H

//Disable max warning level for outside library
#pragma warning(push,3)
#include <libnoise\noise.h>
#pragma warning(pop)

using namespace noise;

class LandGen
{
public:
	static LandGen* Instance();

	void Setup(const int& type);
	float genValue(const int& x,const int& z, const int& seed) const;
private:
	LandGen();
	~LandGen();
	LandGen(const LandGen&);
	LandGen operator=(const LandGen&);

	module::Perlin noise;
	module::RidgedMulti softMountain;
	module::RidgedMulti sharpMountain;
	module::Billow baseFlat;
	module::ScaleBias plains;
	module::ScaleBias mountains;
	module::Select final;
};

#endif