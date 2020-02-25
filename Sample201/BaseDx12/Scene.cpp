#include "stdafx.h"
#include "Scene.h"

namespace basedx12 {

    void FixedTriangle::OnInit() {
        auto Device = App::GetDx12Device();
        auto commandList = Device->GetCommandList();
        auto aspectRatio = Device->GetAspectRatio();

        D3D12_GRAPHICS_PIPELINE_STATE_DESC PipeLineDesc;
        //コンスタントバッファ無し
        m_pcPipelineState
            = PipelineState::CreateDefault2D<VertexPositionColor, VSPCSprite, PSPCSprite>(Device->GetRootSignature(), PipeLineDesc);
        vector<VertexPositionColor> vertex =
        {
            { Float3(0.0f, 0.25f * aspectRatio, 0.0f), Float4(1.0f, 0.0f, 0.0f, 1.0f) },
            { Float3(0.25f, -0.25f * aspectRatio, 0.0f), Float4(0.0f, 1.0f, 0.0f, 1.0f) },
            { Float3(-0.25f, -0.25f * aspectRatio, 0.0f), Float4(0.0f, 0.0f, 1.0f, 1.0f) }
        };
        //三角形メッシュ作成
        m_pcTriangleMesh = Dx12Mesh::CreateDx12Mesh<VertexPositionColor>(vertex);
    }

    void FixedTriangle::OnDraw() {
        auto Device = App::GetDx12Device();
        auto commandList = Device->GetCommandList();
        commandList->SetPipelineState(m_pcPipelineState.Get());
        commandList->IASetVertexBuffers(0, 1, &m_pcTriangleMesh->GetVertexBufferView());
        commandList->DrawInstanced(3, 1, 0, 0);
    }

    void MoveTriangle::OnInit() {
        auto Device = App::GetDx12Device();
        auto aspectRatio = Device->GetAspectRatio();
        auto compPtr = AddComponent<PCSpriteDraw>();
        vector<VertexPositionColor> vertex =
        {
            { Float3(0.0f, 0.25f * aspectRatio, 0.0f), Float4(1.0f, 0.0f, 0.0f, 1.0f) },
            { Float3(0.25f, -0.25f * aspectRatio, 0.0f), Float4(0.0f, 1.0f, 0.0f, 1.0f) },
            { Float3(-0.25f, -0.25f * aspectRatio, 0.0f), Float4(0.0f, 0.0f, 1.0f, 1.0f) }
        };
        //三角形メッシュ作成
        auto mesh = Dx12Mesh::CreateDx12Mesh<VertexPositionColor>(vertex);
        compPtr->SetMesh(mesh);
    }
    void MoveTriangle::OnUpdate() {
        auto compPtr = GetComponent<PCSpriteDraw>();
        compPtr->Move(0.005f);
    }

    void MoveTriangle2::OnInit() {
        auto Device = App::GetDx12Device();
        auto aspectRatio = Device->GetAspectRatio();
        auto compPtr = AddComponent<PCSpriteDraw>();
        vector<VertexPositionColor> vertex =
        {
            { Float3(0.0f, 0.25f * aspectRatio, 0.0f), Float4(1.0f, 0.0f, 0.0f, 1.0f) },
            { Float3(0.25f, -0.25f * aspectRatio, 0.0f), Float4(0.0f, 1.0f, 0.0f, 1.0f) },
            { Float3(-0.25f, -0.25f * aspectRatio, 0.0f), Float4(0.0f, 0.0f, 1.0f, 1.0f) }
        };
        //三角形メッシュ作成
        auto mesh = Dx12Mesh::CreateDx12Mesh<VertexPositionColor>(vertex);
        compPtr->SetMesh(mesh);
    }
    void MoveTriangle2::OnUpdate() {
        auto compPtr = GetComponent<PCSpriteDraw>();
        compPtr->Move(0.0075f);
    }



    void MoveSquare::OnInit() {
        auto Device = App::GetDx12Device();
        auto compPtr = AddComponent<PTSpriteDraw>();
        //メッシュ
        {
            float HelfSize = 0.2f;
            //頂点配列
            vector<VertexPositionTexture> vertices = {
                { VertexPositionTexture(Float3(-HelfSize, HelfSize, 0),	Float2(0.0f, 0.0f)) },
                { VertexPositionTexture(Float3(HelfSize, HelfSize, 0),	Float2(1.0f, 0.0f)) },
                { VertexPositionTexture(Float3(-HelfSize, -HelfSize, 0),	Float2(0.0f, 1.0f)) },
                { VertexPositionTexture(Float3(HelfSize, -HelfSize, 0),	Float2(1.0f, 1.0f)) },
            };
            //インデックス配列
            vector<uint32_t> indices = { 0, 1, 2, 1, 3, 2 };
            //四角形メッシュの作成
            auto mesh = Dx12Mesh::CreateDx12Mesh<VertexPositionTexture>(vertices, indices);
            compPtr->SetMesh(mesh);
        }
        //テクスチャ
        {
            auto TexFile = App::GetRelativeAssetsPath() + L"sky.jpg";
            compPtr->SetTextureFile(TexFile);
        }
    }
    void MoveSquare::OnUpdate() {
        auto compPtr = GetComponent<PTSpriteDraw>();
        compPtr->Move(0.01f);

    }
    void GameStage::OnInit() {
        AddGameObject<FixedTriangle>();
        AddGameObject<MoveTriangle>();
        AddGameObject<MoveTriangle2>();
        AddGameObject<MoveSquare>();
    }

    void Scene::OnInit() {
        ResetActiveDx12Device<Default2DDivece>(3);
    }
    void Scene::OnInitAssets() {
        ResetActiveStage<GameStage>();
    }

}
//end badedx12


