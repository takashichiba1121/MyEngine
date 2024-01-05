#pragma once
/*
* アプリケーションのあたり判定管理
*/
class AppCollider
{
#pragma region Singleton
private:
	AppCollider() {};

	~AppCollider() {};
public:
	AppCollider(const AppCollider& carManager) = delete;

	AppCollider& operator=(const AppCollider& carManager) = delete;

	static AppCollider* Instance();
#pragma endregion
};

