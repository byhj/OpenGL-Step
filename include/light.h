#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>


class BaseLight 
{
public:
	glm::vec3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;
	
	BaseLight(glm::vec3 c, float ai, float di)
		:Color(c), AmbientIntensity(ai), DiffuseIntensity(di){}
	BaseLight()
	{
		Color = glm::vec3(0.0, 0.0, 0.0);
		AmbientIntensity = 0.0f;
		DiffuseIntensity = 0.0f;
	}
	
};

class DirectionalLight: public BaseLight
{
public:
	glm::vec3 Direction;
	DirectionalLight(const glm::vec3 &d):Direction(d) {};
	DirectionalLight()
	{
		Direction = glm::vec3(0.0, 0.0, 0.0);
	}
	
};
	
class PointLight: public BaseLight
{
public:
	glm::vec3 Position;
	struct {
		float Constant;
		float Linear;
		float Exp;
	} Attenuation;
	PointLight()
	{
		Position = glm::vec3(0.0, 0.0, 0.0);
		Attenuation.Constant = 1.0f;
        Attenuation.Linear = 0.0f;
        Attenuation.Exp = 0.0f;
	}

};
class Lighting
{
public:
    static const unsigned int MAX_POINT_LIGHTS = 2;
    Lighting();
    bool Init(GLuint program);

    void SetDirectionalLight(const DirectionalLight& Light);
    void SetEyeWorldPos(const glm::vec3 &EyeWorldPos);
    void SetMatSpecularIntensity(float Intensity);
    void SetMatSpecularPower(float Power);
    void SetPointLights(unsigned int NumLights, const PointLight* pLights);

private:

    GLuint eyeWorldPos_loc;
    GLuint matSpecularIntensity_loc;
    GLuint matSpecularPower_loc;
    GLuint numPointLight_loc;

    struct {
        GLuint Color;
        GLuint AmbientIntensity;
        GLuint DiffuseIntensity;
        GLuint Direction;
    } dirLight_loc;

    struct {
        GLuint Color;
        GLuint AmbientIntensity;
        GLuint DiffuseIntensity;
        GLuint Position;
        struct
        {
            GLuint Constant;
            GLuint Linear;
            GLuint Exp;
        } Atten;
    } pointLight_loc[MAX_POINT_LIGHTS];
};

#endif
