#ifndef CameraController_H
#define CameraController_H

#include <glm\glm.hpp>

class CameraController
{
private:
	glm::mat4 viewMatrix;
	glm::mat4 perspectiveMatrix;

	glm::vec3 position;
	float pitch,heading,fov,aspectRatio;
	int counter;

	void remakeViewMatrix();
	void remakePerspectiveMatrix();
public:
	CameraController();
	CameraController(const CameraController&){};
	CameraController operator=(const CameraController&){};
	~CameraController(){};

	glm::mat4 getViewMatrix(){return viewMatrix;};
	glm::mat4 getPerspectiveMatrix(){return perspectiveMatrix;};

	void addTranslationCamera(const glm::vec3& direction,const float& time);
	void addRotationToCamera(const float& heading,const float& pitch);
	void Update(const float& time);
};

#endif