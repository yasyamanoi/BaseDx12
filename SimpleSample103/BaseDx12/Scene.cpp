#include "stdafx.h"
#include "Scene.h"

namespace basedx12 {


	IMPLEMENT_DX12SHADER(VSPTSprite, App::GetShadersPath() + L"VSPTSprite.cso")
	IMPLEMENT_DX12SHADER(PSPTSprite, App::GetShadersPath() + L"PSPTSprite.cso")

	IMPLEMENT_DX12SHADER(VSPNTStatic, App::GetShadersPath() + L"BcVSPNTStaticPL.cso")
	IMPLEMENT_DX12SHADER(PSPNTStatic, App::GetShadersPath() + L"BcPSPNTPL.cso")



	void MoveSquare::OnInit() {
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		auto aspectRatio = baseDevice->GetAspectRatio();
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PipeLineDesc;

		m_ptConstPipelineState
			= PipelineState::CreateDefault3D<VertexPositionTexture, VSPTSprite, PSPTSprite>(baseDevice->GetRootSignature(), PipeLineDesc);
		//���b�V��
		{
			float HelfSize = 0.5f;
			//���_�z��
			vector<VertexPositionTexture> vertices = {
				{ VertexPositionTexture(Float3(-HelfSize, HelfSize, 0),	Float2(0.0f, 0.0f)) },
				{ VertexPositionTexture(Float3(HelfSize, HelfSize, 0),	Float2(1.0f, 0.0f)) },
				{ VertexPositionTexture(Float3(-HelfSize, -HelfSize, 0),	Float2(0.0f, 1.0f)) },
				{ VertexPositionTexture(Float3(HelfSize, -HelfSize, 0),	Float2(1.0f, 1.0f)) },
			};
			//�C���f�b�N�X�z��
			vector<uint32_t> indices = { 0, 1, 2, 1, 3, 2 };
			//�l�p�`���b�V���̍쐬
			m_ptSquareMesh = BaseMesh::CreateBaseMesh<VertexPositionTexture>(vertices, indices);
		}
		//�e�N�X�`��
		{
			auto TexFile = App::GetRelativeAssetsPath() + L"sky.jpg";
			//�e�N�X�`���̍쐬
			//�V�F�[�_���\�[�X�n���h�����쐬
			m_srvIndex = baseDevice->GetCbvSrvUavNextIndex();
			CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(
			baseDevice->GetCbvSrvUavDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
			m_srvIndex,
			baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
			);
			//�摜�t�@�C�������ƂɃe�N�X�`�����쐬
			m_SkyTexture = BaseTexture::CreateBaseTexture(TexFile, srvHandle);
		}
		//�R���X�^���g�o�b�t�@�n���h�����쐬
		m_constBuffIndex = baseDevice->GetCbvSrvUavNextIndex();
		CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(
			baseDevice->GetCbvSrvUavDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
			m_constBuffIndex,
			baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		m_ConstantBuffer = ConstantBuffer::CreateDirect(Handle, m_constantBufferData);
		m_scale = Float3(256.0f, 256.0f, 1.0f);
		m_pos = Float3(0, -200.0, 0.0f);

	}
	void MoveSquare::OnUpdate() {
		m_pos.x += m_posSpan;
		if (abs(m_pos.x) >= 400.0f) {
			m_posSpan *= -1.0f;
		}
		Mat4x4 worldMatrix;
		worldMatrix.affineTransformation(
			m_scale,
			Float3(0.0f),
			m_qt,
			m_pos
		);
		float w = static_cast<float>(App::GetGameWidth());
		float h = static_cast<float>(App::GetGameHeight());
		Mat4x4 proj;
		proj.orthographicLH(w, h, -1.0, 1.0f);
		worldMatrix *= proj;


		m_constantBufferData.World = worldMatrix;
		m_ConstantBuffer->Copy(m_constantBufferData);
	}
	void MoveSquare::OnDraw() {
		m_ConstantBuffer->Copy(m_constantBufferData);
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();

		//GraphicsRootDescriptorTable�փZ�b�g�B
		//Srv
		CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
			baseDevice->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_srvIndex,
			baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		//srv�BRootSignature���ID��0��
		commandList->SetGraphicsRootDescriptorTable(0, SrvHandle);
		//Sampler
		CD3DX12_GPU_DESCRIPTOR_HANDLE samplerHandle(
			baseDevice->GetSamplerDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		//sampler�BRootSignature���ID��1��
		commandList->SetGraphicsRootDescriptorTable(1, samplerHandle);
		//Cbv
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			baseDevice->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_constBuffIndex,
			baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		//csv�BRootSignature���ID��2��
		commandList->SetGraphicsRootDescriptorTable(2, CbvHandle);
		commandList->SetPipelineState(m_ptConstPipelineState.Get());
		m_SkyTexture->UpdateSRAndCreateSRV(commandList);
		commandList->IASetVertexBuffers(0, 1, &m_ptSquareMesh->GetVertexBufferView());
		commandList->IASetIndexBuffer(&m_ptSquareMesh->GetIndexBufferView());
		commandList->DrawIndexedInstanced(m_ptSquareMesh->GetNumIndices(), 1, 0, 0, 0);
	}

	void MoveBox::OnInit() {
		m_camera = Camera::CreateCamera<Camera>(Float3(0, 3.0f, -5.0f), Float3(0, 0, 0));
		m_lightSet = LightSet::CreateDefaultLights();
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		auto aspectRatio = baseDevice->GetAspectRatio();
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PipeLineDesc;
		//�p�C�v���C�X�e�[�g
		m_pipelineState
			= PipelineState::CreateDefault3D<VertexPositionNormalTexture, VSPNTStatic, PSPNTStatic>(baseDevice->GetRootSignature(), PipeLineDesc);
		//���b�V��
		{
			vector<VertexPositionNormalTexture> vertices;
			vector<uint32_t> indices;
			MeshUtill::CreateCube(1.0f, vertices, indices);
			m_mesh = BaseMesh::CreateBaseMesh<VertexPositionNormalTexture>(vertices, indices);
		}
		//�R���X�^���g�o�b�t�@
		m_constBuffIndex = baseDevice->GetCbvSrvUavNextIndex();
		//�R���X�^���g�o�b�t�@�n���h�����쐬
		CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(
			baseDevice->GetCbvSrvUavDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
			m_constBuffIndex,
			baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
		);
		m_constantBuffer = ConstantBuffer::CreateDirect(Handle, m_constantBufferData);



		auto SamplerDescriptorHandle = baseDevice->GetSamplerDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
		Sampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);
		//�e�N�X�`��
		//�e�N�X�`��
		{
			auto TexFile = App::GetRelativeAssetsPath() + L"sky.jpg";
			//�e�N�X�`���̍쐬
			//�V�F�[�_���\�[�X�n���h�����쐬
			m_srvIndex = baseDevice->GetCbvSrvUavNextIndex();
			CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(
				baseDevice->GetCbvSrvUavDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
				m_srvIndex,
				baseDevice->GetCbvSrvUavDescriptorHandleIncrementSize()
			);
			//�摜�t�@�C�������ƂɃe�N�X�`�����쐬
			m_SkyTexture = BaseTexture::CreateBaseTexture(TexFile, srvHandle);
		}
		//�e�p�����[�^

	}

	void MoveBox::SetConstants() {
		if (!m_mesh) {
			throw BaseException(
				L"���b�V�����ݒ肳��Ă܂���",
				L"if (!m_mesh)",
				L"MoveBox::SetConstants()"
				);
		}
		if (!m_SkyTexture) {
			throw BaseException(
				L"�e�N�X�`�����ݒ肳��Ă܂���",
				L"if (!m_SkyTexture)",
				L"MoveBox::SetConstants()"
				);
		}
		//�s��̒�`
		//������
		m_constantBufferData = {};
		m_constantBufferData.activeFlg.x = 3;
		m_constantBufferData.activeFlg.y = 1;
		Mat4x4 worldMatrix;
		worldMatrix.affineTransformation(
			m_scale,
			Float3(0.0f),
			m_qt,
			m_pos
			);
		auto view = m_camera->GetViewMatrix();
		auto proj = m_camera->GetProjectionMatrix();
		auto worldView = worldMatrix * view;
		m_constantBufferData.worldViewProj = XMMatrixTranspose(XMMatrixMultiply(worldView, proj));
		m_constantBufferData.fogVector = g_XMZero;
		m_constantBufferData.fogColor = g_XMZero;
		//���C�g�̌���
		for (int i = 0; i < m_lightSet->GetNumLights(); i++) {
			m_constantBufferData.lightDirection[i] = m_lightSet->GetLight(i).m_directional;
			m_constantBufferData.lightDiffuseColor[i] = m_lightSet->GetLight(i).m_diffuseColor;
			m_constantBufferData.lightSpecularColor[i] = m_lightSet->GetLight(i).m_specularColor;
		}
		//���[���h�s��
		m_constantBufferData.world = XMMatrixTranspose(worldMatrix);

		XMMATRIX worldInverse = XMMatrixInverse(nullptr, worldMatrix);
		m_constantBufferData.worldInverseTranspose[0] = worldInverse.r[0];
		m_constantBufferData.worldInverseTranspose[1] = worldInverse.r[1];
		m_constantBufferData.worldInverseTranspose[2] = worldInverse.r[2];

		XMMATRIX viewInverse = XMMatrixInverse(nullptr, view);
		m_constantBufferData.eyePosition = viewInverse.r[3];

		XMVECTOR diffuse = Float3(1.0f);
		XMVECTOR alphaVector = XMVectorReplicate(1.0f);
		XMVECTOR emissiveColor = Float3(0.0f);
		XMVECTOR ambientLightColor = m_lightSet->GetAmbient();
		// Merge emissive and ambient light contributions.
		m_constantBufferData.emissiveColor = (emissiveColor + ambientLightColor * diffuse) * alphaVector;
		m_constantBufferData.specularColorAndPower = Float4(0.2f);

		// xyz = diffuse * alpha, w = alpha.
		m_constantBufferData.diffuseColor = XMVectorSelect(alphaVector, diffuse * alphaVector, g_XMSelect1110);
	}


	void MoveBox::OnUpdate() {
		Quat qtspan(Float3(0, 1, 1), -0.02f);
		m_qt *= qtspan;
		m_qt.normalize();
		m_pos.x += m_posSpan;
		if (abs(m_pos.x) >= 5.0f) {
			m_posSpan *= -1.0f;
		}
	}

	void MoveBox::OnDraw() {
		SetConstants();
		m_constantBuffer->Copy(m_constantBufferData);

		auto Device = App::GetBaseDevice();
		auto commandList = Device->GetCommandList();
		//Srv
		CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
			Device->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_srvIndex,
			Device->GetCbvSrvUavDescriptorHandleIncrementSize()
			);
		commandList->SetGraphicsRootDescriptorTable(0, SrvHandle);
		//Sampler
		CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle(
			Device->GetSamplerDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
			);
		commandList->SetGraphicsRootDescriptorTable(1, SamplerHandle);
		//Cbv
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			Device->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			m_constBuffIndex,
			Device->GetCbvSrvUavDescriptorHandleIncrementSize()
			);
		commandList->SetGraphicsRootDescriptorTable(2, CbvHandle);
		commandList->SetPipelineState(m_pipelineState.Get());

		m_SkyTexture->UpdateSRAndCreateSRV(commandList);
		commandList->IASetVertexBuffers(0, 1, &m_mesh->GetVertexBufferView());
		commandList->IASetIndexBuffer(&m_mesh->GetIndexBufferView());
		commandList->DrawIndexedInstanced(m_mesh->GetNumIndices(), 1, 0, 0, 0);

	}




	void Scene::OnInit() {
		//�t���[������3
		ResetActiveBaseDevice<GameDevice>(3);
	}
	void Scene::OnInitAssets() {
		auto baseDevice = App::GetBaseDevice();
		//�T���v���[�͋��L
		{
			auto samplerDescriptorHandle = baseDevice->GetSamplerDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
			Sampler::CreateSampler(SamplerState::LinearClamp, samplerDescriptorHandle);
		}
		// ���ꂼ��̃I�u�W�F�N�g�̏�����
		{
			m_MoveSquare.OnInit();
			m_MoveBox.OnInit();
		}
	}

	void Scene::OnUpdate() {
		m_MoveSquare.OnUpdate();
		m_MoveBox.OnUpdate();
	}
	void Scene::OnDraw() {
		//m_ConstantBuffer->Copy(m_constantBufferData);
		auto baseDevice = App::GetBaseDevice();
		auto commandList = baseDevice->GetCommandList();
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_MoveBox.OnDraw();
		m_MoveSquare.OnDraw();
	}
	void Scene::OnDestroy() {
	}




}
//end badedx12
