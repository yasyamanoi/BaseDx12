#include "stdafx.h"
#include "Project.h"

namespace basedx12 {

    void GameStage::OnInit() {
        AddGameObject<MoveTriangle>();
       AddGameObject<MoveTriangle2>();
        AddGameObject<MoveSquare>();
    }

}
//end basedx12
