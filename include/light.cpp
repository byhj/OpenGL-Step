#include <limits.h>
#include <string.h>
#include <cstdio>
#include <GL/glew.h>
#include "light.h"

#define SNPRINTF _snprintf_s
#define INVALID_UNIFORM_LOCATION 0xffffffff
#define INVALID_OGL_VALUE 0xffffffff

#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }
#define GLM_SWIZZLE

Lighting::Lighting()
{   
}

bool Lighting::Init(GLuint program)
{

    eyeWorldPos_loc = glGetUniformLocation(program,  "gEyeWorldPos");
    matSpecularPower_loc = glGetUniformLocation(program, "gSpecularPower");
    numPointLight_loc = glGetUniformLocation(program, "gNumPointLights");
    dirLight_loc.Color = glGetUniformLocation(program, "gDirectionalLight.Base.Color");
    dirLight_loc.AmbientIntensity = glGetUniformLocation(program, "gDirectionalLight.Base.AmbientIntensity");
    dirLight_loc.Direction = glGetUniformLocation(program, "gDirectionalLight.Direction");
    dirLight_loc.DiffuseIntensity = glGetUniformLocation(program, "gDirectionalLight.Base.DiffuseIntensity");
    matSpecularIntensity_loc = glGetUniformLocation(program, "gMatSpecularIntensity");


    for (unsigned int i = 0 ; i < 2 ; i++) {
        char Name[128];
        memset(Name, 0, sizeof(Name));
		printf(Name, "gPointLights[%d].Base.Color", i);
        pointLight_loc[i].Color = glGetUniformLocation(program, Name);

        SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.AmbientIntensity", i);
        pointLight_loc[i].AmbientIntensity = glGetUniformLocation(program, Name);

        SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Position", i);
        pointLight_loc[i].Position = glGetUniformLocation(program, Name);

        SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.DiffuseIntensity", i);
        pointLight_loc[i].DiffuseIntensity = glGetUniformLocation(program, Name);

        SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Constant", i);
        pointLight_loc[i].Atten.Constant = glGetUniformLocation(program, Name);

        SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Linear", i);
        pointLight_loc[i].Atten.Linear = glGetUniformLocation(program, Name);

        SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Exp", i);
        pointLight_loc[i].Atten.Exp = glGetUniformLocation(program, Name);

        if (pointLight_loc[i].Color == INVALID_UNIFORM_LOCATION ||
            pointLight_loc[i].Position == INVALID_UNIFORM_LOCATION ||
            pointLight_loc[i].DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
            pointLight_loc[i].Atten.Constant == INVALID_UNIFORM_LOCATION ||
            pointLight_loc[i].Atten.Linear == INVALID_UNIFORM_LOCATION ||
            pointLight_loc[i].Atten.Exp == INVALID_UNIFORM_LOCATION) {
            return false;
        }
    }
    
    if (dirLight_loc.AmbientIntensity == INVALID_UNIFORM_LOCATION ||
        eyeWorldPos_loc == INVALID_UNIFORM_LOCATION ||
        dirLight_loc.Color == INVALID_UNIFORM_LOCATION ||
        dirLight_loc.DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
        dirLight_loc.Direction == INVALID_UNIFORM_LOCATION ||
        matSpecularIntensity_loc == INVALID_UNIFORM_LOCATION ||
        matSpecularPower_loc  == INVALID_UNIFORM_LOCATION ||
        numPointLight_loc == INVALID_UNIFORM_LOCATION) {
        return false;
    }

    return true;
}


void Lighting::SetDirectionalLight(const DirectionalLight& Light)
{
    glUniform3f(dirLight_loc.Color, Light.Color.x, Light.Color.y, Light.Color.z);
    glUniform1f(dirLight_loc.AmbientIntensity, Light.AmbientIntensity);
    glm::vec3 Direction = Light.Direction;
	glm::normalize(Direction);
    glUniform3f(dirLight_loc.Direction, Direction.x, Direction.y, Direction.z);
    glUniform1f(dirLight_loc.DiffuseIntensity, Light.DiffuseIntensity);
}

void Lighting::SetEyeWorldPos(const glm::vec3 &EyeWorldPos)
{
    glUniform3fv(eyeWorldPos_loc, 1, &EyeWorldPos[0]);
}

void Lighting::SetMatSpecularIntensity(float Intensity)
{
    glUniform1f(matSpecularIntensity_loc, Intensity);
}

void Lighting::SetMatSpecularPower(float Power)
{
    glUniform1f(matSpecularPower_loc, Power);
}

void Lighting::SetPointLights(unsigned int NumLights, const PointLight* pLights)
{
    glUniform1i(numPointLight_loc, NumLights);
    
    for (unsigned int i = 0 ; i < NumLights ; i++) {
		glUniform3f(pointLight_loc[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
        glUniform1f(pointLight_loc[i].AmbientIntensity, pLights[i].AmbientIntensity);
        glUniform1f(pointLight_loc[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
        glUniform3f(pointLight_loc[i].Position, pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);
        glUniform1f(pointLight_loc[i].Atten.Constant, pLights[i].Attenuation.Constant);
        glUniform1f(pointLight_loc[i].Atten.Linear, pLights[i].Attenuation.Linear);
        glUniform1f(pointLight_loc[i].Atten.Exp, pLights[i].Attenuation.Exp);
    }
}


