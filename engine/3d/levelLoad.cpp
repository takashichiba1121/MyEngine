#include "levelLoad.h"
#include<fstream>

LevelData* LevelLoad::Load(const std::string fullpath)
{
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

			if (object.contains("colider")) {
				//�����蔻��
				nlohmann::json& collision = object["colider"];

				//�����蔻��̃T�C�Y
				objectData.size.x = (float)collision["size"][1];
				objectData.size.y = (float)collision["size"][2];
				objectData.size.z = (float)collision["size"][0];
				//�����蔻��̒��S
				objectData.center.x = (float)collision["center"][1];
				objectData.center.y = (float)collision["center"][2];
				objectData.center.z = (float)collision["center"][0];
			}
			//���s�ړ�
			objectData.trans.x = (float)transform["translation"][1];
			objectData.trans.y = (float)transform["translation"][2];
			objectData.trans.z = -(float)transform["translation"][0];
			//��]�p
			objectData.rot.x = -(float)transform["rotation"][1];
			objectData.rot.y = -(float)transform["rotation"][2];
			objectData.rot.z = (float)transform["rotation"][0];
			//�X�P�[��
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
