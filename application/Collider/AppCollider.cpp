#include"AppCollider.h"

AppCollider* AppCollider::Instance()
{
	static  AppCollider instance;

	return &instance;
}
