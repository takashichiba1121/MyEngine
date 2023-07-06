#include"DirectionalLight.h"
#include<assert.h>
#include<d3d12.h>

void DirectionalLight::SetLightDir(const Vector3& lightDir)
{
	lightDir_ = lightDir;
	lightDir_.normalize();
}

void DirectionalLight::SetLightColor(const Vector3& lightColor)
{
	lightColor_ = lightColor;
}
