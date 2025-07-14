#include "ModelManager.h"
#include <filesystem>
ModelManager* ModelManager::instance = nullptr;

ModelManager* ModelManager::GetInstans()
{
	if (instance == nullptr) {

		instance = new ModelManager;
	}
	return instance;
}

void ModelManager::Finalize()
{

	delete instance;
	instance = nullptr;

}

void ModelManager::Initialize(DirectXCommon* dxcommon, SrvManager* srvmnager)
{
	srvmnager_ = srvmnager;
	modelCommon = std::make_unique<ModelCommon>();
	modelCommon->Initialize(dxcommon, srvmnager_);


}

void ModelManager::LoadModel(const std::string& filePath)
{
	//読み込み済みモデルを検索
	if (models.contains(filePath)) {
		//読み込み済みなら早期return
		return;
	}
	
	std::filesystem::path fullPath = "Resources/" + filePath;	
	std::string directorypath = fullPath.parent_path().string();  // → "Resources/Title"
	std::string filename = fullPath.filename().string();          // → "UI_Title_Stsrt.obj"

	//モデルの生成とファイル読み込み、初期化
	std::unique_ptr<Model> model = std::make_unique<Model>();
	model->Initialize(modelCommon.get(), directorypath, filename);

	//モデルをmapコンテナに格納する
	models.insert(std::make_pair(filePath, std::move(model)));
}

Model* ModelManager::FindModel(const std::string& filePath)
{
	//読み込みモデルを戻り値としてreturn
	if (models.contains(filePath)) {
		return models.at(filePath).get();
	}

	//ファイル名一致なし
	return nullptr;
}

void ModelManager::LoadAllModels(const std::vector<std::string>& modelNames) {
	for (const auto& name : modelNames) {
		std::string fileName = name;
		if (fileName.length() < 4 || fileName.substr(fileName.length() - 4) != ".obj") {
			fileName += ".obj";
		}
		LoadModel(fileName);
	}
}