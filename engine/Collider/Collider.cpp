#include"Collider.h"
#include"object3d.h"
#include<array>


bool Collider::CubeAndCube(Cube A,Cube B,Type type)
{
	if ( type==Type::Collsion )
	{
		Vector3 aabb1Center = A.Pos;
		Vector3 aabb1Size = A.scale;
		Vector3 aabb2Center = B.Pos;
		Vector3 aabb2Size = B.scale;
		std::array<Vector3,2> min_;
		min_[ 0 ] = { aabb1Center.x - aabb1Size.x, aabb1Center.y - aabb1Size.y ,aabb1Center.z - aabb1Size.z };
		min_[ 1 ] = { aabb2Center.x - aabb2Size.x, aabb2Center.y - aabb2Size.y ,aabb2Center.z - aabb2Size.z };
		std::array<Vector3,2> max_;
		max_[ 0 ] = { aabb1Center.x + aabb1Size.x, aabb1Center.y + aabb1Size.y ,aabb1Center.z + aabb1Size.z };
		max_[ 1 ] = { aabb2Center.x + aabb2Size.x, aabb2Center.y + aabb2Size.y ,aabb2Center.z + aabb2Size.z };

		if ( min_[ 0 ].x < max_[ 1 ].x && max_[ 0 ].x > min_[ 1 ].x &&
			min_[ 0 ].y < max_[ 1 ].y && max_[ 0 ].y > min_[ 1 ].y &&
			min_[ 0 ].z < max_[ 1 ].z && max_[ 0 ].z > min_[ 1 ].z )
		{
			return true;
		}
		else
		{
			return false;

		}
	}
	if ( type==Type::Inside )
	{
		Vector3 aabb1Center = A.Pos;
		Vector3 aabb1Size = A.scale;
		Vector3 aabb2Center = B.Pos;
		Vector3 aabb2Size = B.scale;
		std::array<Vector3,2> min_;
		min_[ 0 ] = { aabb1Center.x - aabb1Size.x, aabb1Center.y - aabb1Size.y ,aabb1Center.z - aabb1Size.z };
		min_[ 1 ] = { aabb2Center.x - aabb2Size.x, aabb2Center.y - aabb2Size.y ,aabb2Center.z - aabb2Size.z };
		std::array<Vector3,2> max_;
		max_[ 0 ] = { aabb1Center.x + aabb1Size.x, aabb1Center.y + aabb1Size.y ,aabb1Center.z + aabb1Size.z };
		max_[ 1 ] = { aabb2Center.x + aabb2Size.x, aabb2Center.y + aabb2Size.y ,aabb2Center.z + aabb2Size.z };

		if ( min_[ 0 ].x < min_[ 1 ].x && max_[ 0 ].x > max_[ 1 ].x &&
			min_[ 0 ].y < min_[ 1 ].y && max_[ 0 ].y > max_[ 1 ].y &&
			min_[ 0 ].z < min_[ 1 ].z && max_[ 0 ].z > max_[ 1 ].z )
		{
			return true;
		}
		else
		{
			return false;

		}
	}
	return false;
}

bool Collider::QuadAndQuad(Cube A,Cube B,Type type)
{
	if ( type == Type::Collsion )
	{
		Vector2 aabb1Center = { A.Pos.x,A.Pos.z };
		Vector2 aabb1Size = { A.scale.x,A.scale.z };
		Vector2 aabb2Center = { B.Pos.x,B.Pos.z };
		Vector2 aabb2Size = { B.scale.x,B.scale.z };
		std::array<Vector2,2> min_;
		min_[ 0 ] = { aabb1Center.x - aabb1Size.x, aabb1Center.y - aabb1Size.y };
		min_[ 1 ] = { aabb2Center.x - aabb2Size.x, aabb2Center.y - aabb2Size.y };
		std::array<Vector2,2> max_;
		max_[ 0 ] = { aabb1Center.x + aabb1Size.x, aabb1Center.y + aabb1Size.y };
		max_[ 1 ] = { aabb2Center.x + aabb2Size.x, aabb2Center.y + aabb2Size.y };

		if ( min_[ 0 ].x < max_[ 1 ].x && max_[ 0 ].x > min_[ 1 ].x &&
			min_[ 0 ].y < max_[ 1 ].y && max_[ 0 ].y > min_[ 1 ].y )
		{
			return true;
		}
		else
		{
			return false;

		}
	}
	if ( type==Type::Inside )
	{
		Vector2 aabb1Center = { A.Pos.x,A.Pos.z };
		Vector2 aabb1Size = { A.scale.x,A.scale.z };
		Vector2 aabb2Center = { B.Pos.x,B.Pos.z };
		Vector2 aabb2Size = { B.scale.x,B.scale.z };
		std::array<Vector2,2> min_;
		min_[ 0 ] = { aabb1Center.x - aabb1Size.x, aabb1Center.y - aabb1Size.y };
		min_[ 1 ] = { aabb2Center.x - aabb2Size.x, aabb2Center.y - aabb2Size.y };
		std::array<Vector2,2> max_;
		max_[ 0 ] = { aabb1Center.x + aabb1Size.x, aabb1Center.y + aabb1Size.y };
		max_[ 1 ] = { aabb2Center.x + aabb2Size.x, aabb2Center.y + aabb2Size.y };

		if ( min_[ 0 ].x < min_[ 1 ].x && max_[ 0 ].x > max_[ 1 ].x &&
			min_[ 0 ].y < min_[ 1 ].y && max_[ 0 ].y > max_[ 1 ].y)
		{
			return true;
		}
		else
		{
			return false;

		}
	}
	return false;
}

bool Collider::SphereAndSphere(Sphere A,Sphere B,Type type)
{
	if ( type == Type::Collsion )
	{

		double AR = pow(( B.Pos.x - A.Pos.x ),2) + pow(( B.Pos.y - A.Pos.y ),2) + pow(( B.Pos.z - A.Pos.z ),2);
		double BR = pow(( A.scale.x + B.scale.x ),2);

		if ( AR <= BR )
		{
			return true;
		}
	}
	if ( type == Type::Inside )
	{

		double AR = pow(( B.Pos.x + A.Pos.x ),2) + pow(( B.Pos.y + A.Pos.y ),2) + pow(( B.Pos.z + A.Pos.z ),2);
		double BR = pow(( A.scale.x + B.scale.x ),2);

		if ( AR <= BR )
		{
			return true;
		}
	}
	return false;
}
