#include "tutorialEnemy.h"

void TutorialEnemy::Update()
{
	if ( isDaed_)
	{
		ExplosionFrame++;

		float a = ExplosionFrame / ExplosionMaxFrame;

		obj_->SetDestruction(a);

		obj_->Setalpha(static_cast< float >( ( ExplosionMaxFrame - ExplosionFrame ) / ExplosionMaxFrame ));
		if ( ExplosionFrame >= ExplosionMaxFrame )
		{
			isRevival = true;
			isDaed_ = false;
		}
	}
	else if(isRevival)
	{
		ExplosionFrame--;

		float a = ExplosionFrame / ExplosionMaxFrame;

		obj_->SetDestruction(a);

		obj_->Setalpha(static_cast< float >( ( ExplosionMaxFrame - ExplosionFrame ) / ExplosionMaxFrame ));
		if ( ExplosionFrame <=0 )
		{
			isRevival = false;
		}
	}

	obj_->Update();
}
