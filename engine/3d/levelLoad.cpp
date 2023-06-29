#include "levelLoad.h"
#include<fstream>

LevelData* LevelLoad::Load()
{
	const std::string fullpath = "Resources/level.json";
	//�t�@�C���X�g���[��
	std::ifstream file;
	//�t�@�C�����J��
	file.open(fullpath);
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) {
		assert(0);
	}
	//JSON�����񂩂�𓀂����f�[�^
	nlohmann::json deserialized;

	//��
	file >> deserialized;

	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"�𕶎���Ƃ��Ď擾
	std::string name =
		deserialized["name"].get<std::string>();
	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(name.compare("scene") == 0);
	
	LevelData* levelData = new LevelData();

	//"objects"�̑S�I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		//��ʂ��擾
		std::string type = object["type"].get < std::string>();

		//MESH
		if (type.compare("MESH") == 0)
		{
			//�v�f�ǉ�
			levelData->objects.emplace_back(LevelData::ObjectData{});
			//���ǉ������v�f�̎Q�Ƃ𓾂�
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name")) {
				//�t�@�C����
				objectData.fileName = object["file_name"];
			}
			nlohmann::json& transform = object["transform"];
			//���s�ړ�
			objectData.trans.m128_f32[0] = (float)transform["translation"][1];
			objectData.trans.m128_f32[1] = (float)transform["translation"][2];
			objectData.trans.m128_f32[2] = -(float)transform["translation"][0];
			objectData.trans.m128_f32[3] = 1.0f;
			//��]�p
			objectData.rot.m128_f32[0] = -(float)transform["rotation"][1];
			objectData.rot.m128_f32[1] = -(float)transform["rotation"][2];
			objectData.rot.m128_f32[2] = (float)transform["rotation"][0];
			objectData.rot.m128_f32[3] = 0.0f;
			//��]�p
			objectData.scale.m128_f32[0] = (float)transform["scaling"][1];
			objectData.scale.m128_f32[1] = (float)transform["scaling"][2];
			objectData.scale.m128_f32[2] = (float)transform["scaling"][0];
			objectData.scale.m128_f32[3] = 0.0f;
		}
	}	
	return levelData;
}
