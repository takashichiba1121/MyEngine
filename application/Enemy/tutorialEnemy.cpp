#include "tutorialEnemy.h"

void TutorialEnemy::Update()
{
	if ( isDaed_&&isRevival_==false)
	{
		ExplosionFrame_++;

		float a = ExplosionFrame_ / ExplosionMaxFrame_;

		obj_->SetDestruction(a);

		obj_->Setalpha(static_cast< float >( ( ExplosionMaxFrame_ - ExplosionFrame_ ) / ExplosionMaxFrame_ ));
		if ( ExplosionFrame_ >= ExplosionMaxFrame_ )
		{
			isRevival_ = true;
		}
	}
	else if(isRevival_)
	{
		ExplosionFrame_--;

		float a = ExplosionFrame_ / ExplosionMaxFrame_;

		obj_->SetDestruction(a);

		obj_->Setalpha(static_cast< float >( ( ExplosionMaxFrame_ - ExplosionFrame_ ) / ExplosionMaxFrame_ ));
		if ( ExplosionFrame_ <=0 )
		{
			isRevival_ = false;
			isDaed_ = false;
		}
	}

	obj_->Update();
}
