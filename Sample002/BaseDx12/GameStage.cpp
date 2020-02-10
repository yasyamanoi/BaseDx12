#include "stdafx.h"
#include "Scene.h"
#include "GameStage.h"
namespace basedx12 {


    GameStage::GameStage() :
        Stage(),
        m_device()
    {
    }

    void GameStage::OnInit()
    {
        m_device.OnInit();
    }


    // Update frame-based values.
    void GameStage::OnUpdate()
    {
        m_device.OnUpdate();
    }

    // Render the scene.
    void GameStage::OnRender()
    {
        m_device.OnRender();
    }

    void GameStage::OnDestroy()
    {
        m_device.OnDestroy();
    }


}
//end basedx12
