#include "levelLoad.h"
#include<fstream>

LevelData* LevelLoad::Load()
{
	const std::string fullpath = "Resources/level.json";
	//ファイルストリーム
	std::ifstream file;
	//ファイルを開く
	file.open(fullpath);
	//ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}
	//JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	//解凍
	file >> deserialized;

	//正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"を文字列として取得
	std::string name =
		deserialized["name"].get<std::string>();
	//正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);
	
	LevelData* levelData = new LevelData();

	//"objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get < std::string>();

		//MESH
		if (type.compare("MESH") == 0)
		{
			//要素追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			//今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name")) {
				//ファイル名
				objectData.fileName = object["file_name"];
			}
			nlohmann::json& transform = object["transform"];
			//平行移動
			objectData.trans.m128_f32[0] = (float)transform["translation"][1];
			objectData.trans.m128_f32[1] = (float)transform["translation"][2];
			objectData.trans.m128_f32[2] = -(float)transform["translation"][0];
			objectData.trans.m128_f32[3] = 1.0f;
			//回転角
			objectData.rot.m128_f32[0] = -(float)transform["rotation"][1];
			objectData.rot.m128_f32[1] = -(float)transform["rotation"][2];
			objectData.rot.m128_f32[2] = (float)transform["rotation"][0];
			objectData.rot.m128_f32[3] = 0.0f;
			//回転角
			objectData.scale.m128_f32[0] = (float)transform["scaling"][1];
			objectData.scale.m128_f32[1] = (float)transform["scaling"][2];
			objectData.scale.m128_f32[2] = (float)transform["scaling"][0];
			objectData.scale.m128_f32[3] = 0.0f;
		}
	}	
	return levelData;
}
