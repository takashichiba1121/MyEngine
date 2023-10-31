#include"EnemyBullet.h"
#include"EnemyManager.h"

void EnemyBullet::Initialize(Model* model, Vector3 velocity, Vector3 position)
{
	obj_ = std::make_unique<Object3d>();

	obj_->Initialize();
	
	velocity_ = velocity;

	obj_->SetModel(model);

	obj_->SetPosition(position);

	obj_->SetPolygonExplosion({ 0.0f,1.0f,6.28f,100.0f });
}

void EnemyBullet::Update()
{

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	Vector3 move = obj_->GetPosition();

	move +=velocity_;

	

	obj_->SetPosition(move);

	obj_->Update();
}

void EnemyBullet::Draw()
{
	obj_->Draw();
}

void EnemyBullet::OnCollision()
{
	isDead_ = true;

	for ( int i = 0; i < 50; i++ )
	{
		//消えるまでの時間
		const uint32_t rnd_life = 10;
		//最低限のライフ
		const uint32_t constlife = 30;
		uint32_t life = ( rand() / RAND_MAX * rnd_life ) + constlife;

		//XYZの広がる距離
		const float rnd_pos = 0.3f;
		Vector3 pos{};
		pos.x = ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2;
		pos.y = ( ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2 );
		pos.z = ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2;

		//pos.normalize();

		//追加
		EnemyManager::Instance()->GetParticle()->Add(life,obj_->GetPosition(),pos,{ 0,0,0 },1.0f,1.0f,{ 1,0.5,0,1 },{ 1,0.5,0,0 });
	}
}
