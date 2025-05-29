#pragma once
#include "IParticleBehavior.h"
class PlayerDashParticle : public IParticleBehavior
{
	Particle Create(std::mt19937& rng, const Vector3& pos, bool isLeft) override;

	void Update(Particle& particle, float dt, Material* matelialData) override;

};

