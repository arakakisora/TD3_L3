#include "PlayerDashParticle.h"
#include "ParticleMnager.h"
#include <numbers>

Particle PlayerDashParticle::Create(std::mt19937& randomEngine, const Vector3& translate, bool isLeft)
{
	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
	std::uniform_real_distribution<float> distTime(0.3f, 0.6f);
	std::uniform_real_distribution<float> distOffset(-0.1f, 0.1f);
	std::uniform_real_distribution<float> distColor(0.4f, 0.6f); // くすんだ色

	Particle particle;

	Vector3 randomTranslate{
		distOffset(randomEngine),
		0.05f,
		distOffset(randomEngine)
	};

	particle.transform.scale = { 0.3f, 0.3f, 0.3f };
	particle.transform.translate = translate + randomTranslate;

	// ← 方向に応じて飛ばす方向を調整
	float xVelocity = isLeft ? -0.3f : 0.3f;
	particle.Velocity = { xVelocity + distOffset(randomEngine), 0.1f, distOffset(randomEngine) };

	particle.color = { distColor(randomEngine), distColor(randomEngine) * 0.9f, distColor(randomEngine) * 0.7f, 1.0f };

	particle.lifetime = distTime(randomEngine);
	particle.currentTime = 0;
	return particle;

}

void PlayerDashParticle::Update(Particle& particle, float dt, Material* matelialData)
{

	// 位置更新
	particle.transform.translate += particle.Velocity * dt;
	particle.currentTime += dt;

	//// フェードアウト
	//float alpha = 1.0f - (particle.currentTime / particle.lifetime);
	//particle.color.w = std::max(alpha, 0.0f);
}
