#include "LandGen.h"

LandGen* LandGen::Instance()
{
	static LandGen instance;
	return &instance;
}

LandGen::LandGen()
{
	Setup(0);
}

LandGen::~LandGen()
{}

/****************************
* Setup - Sets up the land generation variables
****************************/
void LandGen::Setup(const int& type)
{
	//sets the general settings for the land generation modules
	noise.SetOctaveCount(8);
	
	sharpMountain.SetFrequency(1);
	mountains.SetSourceModule(0,sharpMountain);
	mountains.SetScale(5);

	baseFlat.SetFrequency(2.0);
	plains.SetSourceModule(0,baseFlat);
	plains.SetScale(0.125);
	plains.SetBias(-0.75);

	noise.SetFrequency(.09);
	noise.SetPersistence(0.25);

	final.SetSourceModule(0,mountains);
	final.SetSourceModule(1,plains);
	final.SetControlModule(noise);

	final.SetBounds(0,100);
	final.SetEdgeFalloff(0.75);
}

/****************************
* genValue - Generates a value based on the passed variables
****************************/
float LandGen::genValue(const int& x, const int& z, const int& seed) const
{
	float val;
	val=(float)final.GetValue(x,z,seed);

	return val;
}