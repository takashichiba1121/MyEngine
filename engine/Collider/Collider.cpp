#include"Collider.h"
#include"object3d.h"
#include<array>


bool Collider::CubeAndCube(Cube A, Cube B)
{
	Vector3 aabb1Center = A.Pos;
	Vector3 aabb1Size = A.scale;
	Vector3 aabb2Center = B.Pos;
	Vector3 aabb2Size = B.scale;
	std::array<Vector3, 2> min_;
	min_[0] = { aabb1Center.x - aabb1Size.x, aabb1Center.y - aabb1Size.y ,aabb1Center.z - aabb1Size.z };
	min_[1] = { aabb2Center.x - aabb2Size.x, aabb2Center.y - aabb2Size.y ,aabb2Center.z - aabb2Size.z };
	std::array<Vector3, 2> max_;
	max_[0] = { aabb1Center.x + aabb1Size.x, aabb1Center.y + aabb1Size.y ,aabb1Center.z + aabb1Size.z };
	max_[1] = { aabb2Center.x + aabb2Size.x, aabb2Center.y + aabb2Size.y ,aabb2Center.z + aabb2Size.z };

	if (min_[0].x < max_[1].x && max_[0].x > min_[1].x &&
		min_[0].y < max_[1].y && max_[0].y > min_[1].y &&
		min_[0].z < max_[1].z && max_[0].z > min_[1].z)
	{
		return true;
	}
	else
	{
		return false;

	}
}
