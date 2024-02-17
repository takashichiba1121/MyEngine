#include "PlayerBulletManager.h"
#include"TextureManager.h"
#include"imgui.h"

PlayerBulletManager* PlayerBulletManager::Instance()
{
	static PlayerBulletManager instance;

	return &instance;
}

void PlayerBulletManager::Initialize()
{
	particle_ = std::make_unique<ParticleManager>();

	particle_->Initialize();

	particle_->SetTextureHandle(TextureManager::Instance()->LoadTexture("Resources/effect3.png"));
}

void PlayerBulletManager::Update()
{
	bullets_.remove_if([ ] (std::unique_ptr<PlayerBullet>& bullet)
 {
	 return bullet->IsDead();
		});

	for ( std::unique_ptr<PlayerBullet>& bullet : bullets_ )
	{
		bullet->Update();
	}

	particle_->Update();

#ifdef _DEBUG

	ImGui::Begin("playerBullet");

	ImGui::Text("particleSize%d",particle_->GetParticlesListSize());

	ImGui::End();

#endif
}

void PlayerBulletManager::Draw()
{
	for ( std::unique_ptr<PlayerBullet>& bullet : bullets_ )
	{
		bullet->Draw();
	}
}

void PlayerBulletManager::ParticleDraw()
{
	particle_->Draw();
}

void PlayerBulletManager::AddBullet(std::unique_ptr<PlayerBullet> newBullet)
{
	bullets_.push_back(std::move(newBullet));
}

void PlayerBulletManager::Fin()
{

	bullets_.clear();

	particle_ = nullptr;
}

void PlayerBulletManager::Clear()
{
	bullets_.clear();

	particle_->Clear();
}
