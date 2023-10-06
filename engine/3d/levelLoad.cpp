 #include "levelLoad.h"
#include<fstream>

LevelData* LevelLoad::Load(const std::string fullpath)
{
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
			if (object.contains("Tag_name")) {
				//ファイル名
				objectData.tagName = object["Tag_name"];
			}
			nlohmann::json& transform = object["transform"];

			if (object.contains("colider")) {
				//当たり判定
				nlohmann::json& collision = object["colider"];

				//当たり判定のサイズ
				objectData.size.x = (float)collision["size"][1];
				objectData.size.y = (float)collision["size"][2];
				objectData.size.z = (float)collision["size"][0];
				//当たり判定の中心
				objectData.center.x = (float)collision["center"][1];
				objectData.center.y = (float)collision["center"][2];
				objectData.center.z = (float)collision["center"][0];
			}
			//平行移動
			objectData.trans.x = (float)transform["translation"][1];
			objectData.trans.y = (float)transform["translation"][2];
			objectData.trans.z = -(float)transform["translation"][0];
			//回転角
			objectData.rot.x = -(float)transform["rotation"][1];
			objectData.rot.y = -(float)transform["rotation"][2];
			objectData.rot.z = (float)transform["rotation"][0];
			//スケール
			objectData.scale.x = (float)transform["scaling"][1];
			objectData.scale.y = (float)transform["scaling"][2];
			objectData.scale.z = (float)transform["scaling"][0];

			objectData.center.x += objectData.trans.x;
			objectData.center.y += objectData.trans.y;
			objectData.center.z += objectData.trans.z;
		}
	}	
	return levelData;
}
