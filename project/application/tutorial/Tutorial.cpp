#include "Tutorial.h"
#include "SceneManager.h"

//初期化
void Tutorial::Initialize() {
    //チュートリアルテキスト
    const std::array<TutorialTextData, 13> tutorialData = { {
        { "tutorial/tutorial1.obj",  {0.5f, 0.5f, 0.5f}, {17.3f, 12.57f, 0.0f}, {8.5f, 21.44f, 1.0f} },
        { "tutorial/tutorial2.obj",  {0.5f, 0.5f, 0.5f}, {17.3f, 12.57f, 0.0f}, {15.3f, 21.4f, 0.75f} },
        { "tutorial/tutorial3.obj",  {0.5f, 0.5f, 0.5f}, {17.3f, 12.56f, 0.0f}, {8.5f, 21.45f, 1.0f} },
        { "tutorial/tutorial4.obj",  {0.5f, 0.5f, 0.5f}, {17.3f, 12.56f, 0.0f}, {15.46f, 21.4f, 0.69f} },
        { "tutorial/tutorial5.obj",  {0.75f, 0.5f, 0.5f}, {17.3f, 12.57f, 0.0f}, {12.24f, 21.45f, 1.0f} },
        { "tutorial/tutorial6.obj",  {0.5f, 0.5f, 0.5f}, {17.3f, 12.56f, 0.0f}, {8.5f, 21.4f, 1.0f} },
        { "tutorial/tutorial7.obj",  {0.5f, 0.5f, 0.5f}, {17.3f, 12.56f, 0.0f}, {15.46f, 21.4f, 1.0f} },
        { "tutorial/tutorial8.obj",  {0.5f, 0.5f, 0.5f}, {17.3f, 12.56f, 0.0f}, {12.46f, 21.4f, 1.0f} },
        { "tutorial/tutorial9.obj",  {0.5f, 0.5f, 0.5f}, {17.3f, 12.56f, 0.0f}, {12.46f, 21.4f, 1.0f} },
        { "tutorial/tutorial10.obj", {1.0f, 0.5f, 0.5f}, {17.3f, 12.56f, 0.0f}, {12.46f, 21.4f, 1.0f} },
        { "tutorial/tutorial11.obj", {1.0f, 0.5f, 0.5f}, {17.3f, 12.56f, 0.0f}, {12.46f, 21.4f, 1.0f} },
        { "tutorial/tutorial12.obj", {1.0f, 0.5f, 0.5f}, {17.3f, 12.56f, 0.0f}, {12.46f, 21.4f, 1.0f} },
        { "tutorial/tutorial13.obj", {1.0f, 0.5f, 0.5f}, {17.3f, 12.56f, 0.0f}, {12.46f, 19.5f, 1.0f} },
    } };

    for (size_t i = 0; i < tutorialTexts.size(); ++i) {
        tutorialTexts[i] = std::make_unique<Object3D>();
        tutorialTexts[i]->Initialize(Object3DCommon::GetInstance());
        tutorialTexts[i]->SetModel(tutorialData[i].modelPath);
        tutorialTexts[i]->SetScale(tutorialData[i].scale);
        tutorialTexts[i]->SetRotate(tutorialData[i].rotate);
        tutorialTexts[i]->SetTranslate(tutorialData[i].translate);
        tutorialTexts[i]->SetLighting(false);
        tutorialTexts[i]->SetIsTutorialActive(false);
    }

    //ブロックスプライト
    const std::array<SpriteData, 3> spriteList = { {
        { "Resources/GamePlay/ncopy.png",        {275, 121}, {45, 45} },
        { "Resources/GamePlay/jumpsprite.png",   {276, 214}, {45, 45} },
        { "Resources/GamePlay/timersprite.png",  {275, 121}, {45, 45} },
    } };

    for (size_t i = 0; i < blockSprites.size(); ++i) {
        blockSprites[i] = std::make_unique<Sprite>();
        blockSprites[i]->Initialize(SpriteCommon::GetInstance(), spriteList[i].texturePath);
        blockSprites[i]->SetPosition(spriteList[i].position);
        blockSprites[i]->SetSize(spriteList[i].size);
    }
}

//更新
void Tutorial::Update() {

    //現在のステージ番号を取得
    int stageIndex = SceneManager::GetInstance()->GetStageIndex();
    
    //map1
    if (stageIndex == 0) {
        if (!tutorial1_2) {
            tutorialTexts[0]->SetIsTutorialActive(true);
            tutorialTexts[1]->SetIsTutorialActive(true);
            tutorial1_2 = true;
        }

        if (photoCamera->HasStarted() && !tutorial3_4) {
            tutorialTexts[0]->SetIsTutorialActive(false);
            tutorialTexts[1]->SetIsTutorialActive(false);
            tutorialTexts[2]->SetIsTutorialActive(true);
            tutorialTexts[3]->SetIsTutorialActive(true);
            tutorial3_4 = true;
        }

        if (photoCamera->HasMoved() && !tutorial5) {
            tutorialTexts[2]->SetIsTutorialActive(false);
            tutorialTexts[3]->SetIsTutorialActive(false);
            tutorialTexts[4]->SetIsTutorialActive(true);
            tutorial5 = true;
        }

        if (photoCamera->isFirstCopied && !tutorial6_7) {
            tutorialTexts[4]->SetIsTutorialActive(false);
            tutorialTexts[5]->SetIsTutorialActive(true);
            tutorialTexts[6]->SetIsTutorialActive(true);
            tutorial6_7 = true;
        }

        if (photoCamera->isFirstPasted && !tutorial8) {
            tutorialTexts[5]->SetIsTutorialActive(false);
            tutorialTexts[6]->SetIsTutorialActive(false);
            tutorialTexts[7]->SetIsTutorialActive(true);
            tutorial8 = true;
        }

        if (tutorial8) {
            elapsedTime += deltaTime;
            if (elapsedTime >= afterseconds && !tutorial10) {
                tutorialTexts[7]->SetIsTutorialActive(false);
                tutorialTexts[9]->SetIsTutorialActive(true);
                secondspassed = true;
                tutorial10 = true;
            }
        }
    }

    //map2
    if (stageIndex == 1 && !tutorial9) {
        tutorialTexts[8]->SetIsTutorialActive(true);
        tutorial9 = true;
    }

    //map3
    if (stageIndex == 2 && !tutorial11) {
        tutorialTexts[10]->SetIsTutorialActive(true);
        blockSprites[0]->Update();//ncopy
        tutorial11 = true;
    }

    //map9
    if (stageIndex == 8 && !tutorial12) {
        tutorialTexts[11]->SetIsTutorialActive(true);
        blockSprites[2]->Update();//timer
        tutorial12 = true;
    }

    //map7
    if (stageIndex == 6 && !tutorial13) {
        tutorialTexts[12]->SetIsTutorialActive(true);
        blockSprites[1]->Update();//jump
        tutorial13 = true;
    }


    for (const auto& text : tutorialTexts) {
        if (text && text->GetIsTutorialActive()) {
            text->Update();
        }
    }

}

//テキスト描画
void Tutorial::TextDraw() {
    for (const auto& text : tutorialTexts) {
        if (text && text->GetIsTutorialActive()) {
            text->Draw();
        }
    }
}

//スプライト描画
void Tutorial::SpriteDraw() {
    int stageIndex = SceneManager::GetInstance()->GetStageIndex();

    //map3
    if (stageIndex == 2 && blockSprites[0]) {
        blockSprites[0]->Draw();//ncopy
    }

    //map7
    if (stageIndex == 6 && blockSprites[1]) {
        blockSprites[1]->Draw();//jump
    }

    //map9
    if (stageIndex == 8 && blockSprites[2]) {
        blockSprites[2]->Draw();//timer
    }
}

void Tutorial::SetPhotoCamera(PhotoCamera* camera) {
    photoCamera = camera;
}