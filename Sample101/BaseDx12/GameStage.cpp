#include "stdafx.h"
#include "Scene.h"
#include "GameStage.h"

namespace basedx12 {

    GameStage::GameStage() :
        Stage(),
        device()
    {
    }

    void GameStage::OnInit()
    {
        device.OnInit();
    }


    // Update frame-based values.
    void GameStage::OnUpdate()
    {
        device.OnUpdate();
    }

    // Render the scene.
    void GameStage::OnRender()
    {
        device.OnRender();
    }

    void GameStage::OnDestroy()
    {
        device.OnDestroy();
    }

    void GameStage::OnKeyDown(UINT8 key)
    {
        device.OnKeyDown(key);
    }

    void GameStage::OnKeyUp(UINT8 key)
    {
        device.OnKeyUp(key);
    }

}
//end basedx12
