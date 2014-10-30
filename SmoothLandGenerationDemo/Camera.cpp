#include "Camera.h"
#include <glm\gtx\transform.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <iostream>

using namespace glm;

CameraController::CameraController():
position(0,20,-20),pitch(-30),heading(0),fov(90),aspectRatio(16/9),counter(0)
{
	remakeViewMatrix();
	remakePerspectiveMatrix();
}

void CameraController::remakeViewMatrix()
{
	vec3 xAxis(cos(glm::radians(heading)),0,-sin(glm::radians(heading)));
	vec3 yAxis(sin(glm::radians(heading))*sin(glm::radians(pitch)),cos(glm::radians(pitch)),cos(glm::radians(heading))*sin(glm::radians(pitch)));
	vec3 zAxis(sin(glm::radians(heading))*cos(glm::radians(pitch)),-sin(glm::radians(pitch)),cos(glm::radians(pitch))*cos(glm::radians(heading)));

	viewMatrix=mat4(
		vec4(cos(glm::radians(heading)),sin(glm::radians(heading))*sin(glm::radians(pitch)),sin(glm::radians(heading))*cos(glm::radians(pitch)),0),
		vec4(0,cos(glm::radians(pitch)),-sin(glm::radians(pitch)),0),
		vec4(-sin(glm::radians(heading)),cos(glm::radians(heading))*sin(glm::radians(pitch)),cos(glm::radians(pitch))*cos(glm::radians(heading)),0),
		vec4(-dot(xAxis,position),-dot(yAxis,position),-dot(zAxis,position),1));
}

void CameraController::remakePerspectiveMatrix()
{
	perspectiveMatrix=perspective(fov,aspectRatio,0.001f,10000.0f);
}

void CameraController::addRotationToCamera(const float& h, const float& p)
{
	pitch-=p;
	if(pitch<-90)
		pitch=-90;
	else if(pitch>90)
		pitch=90;

	heading-=h;

	remakeViewMatrix();
}

void CameraController::addTranslationCamera(const vec3& dir, const float& time)
{
	vec3 direction=dir*time;
	vec3 newDirection(
		direction.x*cos(radians(heading))-direction.z*sin(radians(heading-180)),
		direction.y,
		direction.z*cos(radians(heading))-direction.x*-sin(radians(heading-180)));	

	if(counter==1000)
	{
		//std::cout << "Direction: " << dir.x << " "  << dir.y << " " << dir.z << "    Time: " << time << "\nNew Direction: " << newDirection.x << " " << newDirection.y << " " << newDirection.z <<"\n";
		counter=0;
	}
	else
		counter++;

	position+=newDirection;
	remakeViewMatrix();
}