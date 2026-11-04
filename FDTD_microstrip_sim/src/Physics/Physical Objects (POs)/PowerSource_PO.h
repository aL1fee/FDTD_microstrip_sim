#pragma once

#include "../PO Templates (POTs)/Cylinder_POT.h"

#include <vector>
#include <string>

class PowerSource_PO : public Cylinder_POT
{
private:
	float power;
	float frequency;
	float wavelength;
    float driveAmplitude3D = 1.0f;
    int polarization3D = 1; // global Y, across a typical trace/ground gap

public:
	PowerSource_PO(glm::vec3 o, glm::vec3 dir, float p, float r, float l,
		glm::vec3 col, float perm, float cond, Shader* sh) :
		Cylinder_POT(o, dir, r, l, col, perm, cond, sh)
	{
		power = p;
		powerSourceOutDesignator = true;
		frequency = POWER_SOURCE_INITIAL_FREQUENCY;
		wavelength = (float) SPEED_OF_LIGHT / 
			(frequency * (float) GHZ_TO_HZ) * 1000;
	}

	void updatePropertyMap() override;

	float* getDriveAmplitude3D() { return &driveAmplitude3D; }
    int* getPolarization3D() { return &polarization3D; }
    float* getFrequency() { return &frequency; }
	float* getPower() { return &power; }
	float* getWavelength() { return &wavelength; }

};