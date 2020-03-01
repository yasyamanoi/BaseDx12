#include "stdafx.h"
#include "Project.h"

namespace basedx12 {

    void MoveTriangle::OnInit() {
        auto Device = App::GetDx12Device();
        auto aspectRatio = Device->GetAspectRatio();
        auto compPtr = AddComponent<PCSpriteDraw>();
        vector<VertexPositionColor> vertex =
        {
            { Float3(0.0f, 0.25f * aspectRatio, 0.0f), Float4(1.0f, 0.0f, 0.0f, 1.0f) },
            { Float3(0.5f, -0.25f * aspectRatio, 0.0f), Float4(0.0f, 1.0f, 0.0f, 1.0f) },
            { Float3(-0.5f, -0.25f * aspectRatio, 0.0f), Float4(0.0f, 0.0f, 1.0f, 1.0f) }
        };
        //三角形メッシュ作成
        auto mesh = Dx12Mesh::CreateDx12Mesh<VertexPositionColor>(vertex);
        compPtr->SetMesh(mesh);
        auto ptrTrans = GetComponent<Transform>();
        ptrTrans->SetScale(Float3(256.0f, 256.0f, 1.0f));
        Float3 pos(0, -200.0, 0.002f);
        ptrTrans->SetPosition(pos);
    }
    void MoveTriangle::OnUpdate() {
        auto ptrTrans = GetComponent<Transform>();
        auto pos = ptrTrans->GetPosition();
        pos.x += m_posSpan;
        if (abs(pos.x) >= 400.0f) {
            m_posSpan *= -1.0f;
        }
        ptrTrans->SetPosition(pos);
    }

    void MoveTriangle2::OnInit() {
        auto Device = App::GetDx12Device();
        auto aspectRatio = Device->GetAspectRatio();
        auto compPtr = AddComponent<PCSpriteDraw>();
        vector<VertexPositionColor> vertex =
        {
            { Float3(0.0f, 0.25f * aspectRatio, 0.0f), Float4(0.0f, 0.0f, 1.0f, 1.0f) },
            { Float3(0.5f, -0.25f * aspectRatio, 0.0f), Float4(0.0f, 1.0f, 0.0f, 1.0f) },
            { Float3(-0.5f, -0.25f * aspectRatio, 0.0f), Float4(1.0f, 0.0f, 1.0f, 0.0f) }
        };
        //三角形メッシュ作成
        auto mesh = Dx12Mesh::CreateDx12Mesh<VertexPositionColor>(vertex);
        compPtr->SetMesh(mesh);
        auto ptrTrans = GetComponent<Transform>();
        ptrTrans->SetScale(Float3(256.0f, 256.0f, 1.0f));
        Float3 pos(0, -200.0, 0.001f);
        ptrTrans->SetPosition(pos);
    }
    void MoveTriangle2::OnUpdate() {
        auto ptrTrans = GetComponent<Transform>();
        auto pos = ptrTrans->GetPosition();
        pos.x += m_posSpan;
        if (abs(pos.x) >= 400.0f) {
            m_posSpan *= -1.0f;
        }
        ptrTrans->SetPosition(pos);
    }



    void MoveSquare::OnInit() {
        auto Device = App::GetDx12Device();
        auto compPtr = AddComponent<PTSpriteDraw>();
        //メッシュ
        {
            float HelfSize = 0.5f;
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
        auto ptrTrans = GetComponent<Transform>();
        ptrTrans->SetScale(Float3(128.0f, 128.0f, 1.0f));
        Float3 pos(0, -200.0, 0.0f);
        ptrTrans->SetPosition(pos);
    }
    void MoveSquare::OnUpdate() {
        auto ptrTrans = GetComponent<Transform>();
        auto pos = ptrTrans->GetPosition();
        pos.x += m_posSpan;
        if (abs(pos.x) >= 400.0f) {
            m_posSpan *= -1.0f;
        }
        ptrTrans->SetPosition(pos);
    }


    void MoveBox::OnInit() {
        auto Device = App::GetDx12Device();
        auto compPtr = AddComponent<PNTStaticDraw>();
        //メッシュ
        vector<VertexPositionNormalTexture> vertices;
        vector<uint32_t> indices;
        MeshUtill::CreateCube(1.0f, vertices, indices);
        auto mesh = Dx12Mesh::CreateDx12Mesh<VertexPositionNormalTexture>(vertices, indices);
        compPtr->SetMesh(mesh);
        //テクスチャ
        auto TexFile = App::GetRelativeAssetsPath() + L"sky.jpg";
        compPtr->SetTextureFile(TexFile);
        compPtr->SetCameraKey(L"MainCamera");
        compPtr->SetLightSetKey(L"MainLightSet");

        auto ptrTrans = GetComponent<Transform>();
        ptrTrans->SetScale(Float3(1.0f));
        ptrTrans->SetPosition(Float3(0.0f));
    }
    void MoveBox::OnUpdate() {
        auto ptrTrans = GetComponent<Transform>();
        auto qt = ptrTrans->GetQuaternion();
        Quat qtspan(Float3(0, 1, 1), -0.02f);
        qt *= qtspan;
        qt.normalize();
        ptrTrans->SetQuaternion(qt);
        auto pos = ptrTrans->GetPosition();
        pos.x += m_posSpan;
        if (abs(pos.x) >= 5.0f) {
            m_posSpan *= -1.0f;
        }
        ptrTrans->SetPosition(pos);
    }


}
//end basedx12
