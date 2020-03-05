#include "stdafx.h"

namespace basedx12 {

	IMPLEMENT_DX12SHADER(VSPNTStatic, App::GetShadersPath() + L"BcVSPNTStaticPL.cso")
	IMPLEMENT_DX12SHADER(PSPNTStatic, App::GetShadersPath() + L"BcPSPNTPL.cso")

	//--------------------------------------------------------------------------------------
	///	PNT���_Static�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PNTStaticDraw::PNTStaticDraw(const shared_ptr<GameObject>& gameObjectPtr) :
		Draw3D(gameObjectPtr)
	{}
	PNTStaticDraw::~PNTStaticDraw() {}

	void PNTStaticDraw::SetConstants() {
		if (!m_mesh) {
			throw BaseException(
				L"���b�V�����ݒ肳��Ă܂���",
				L"if (!m_mesh)",
				L"PNTStaticDraw::SetConstants()"
			);
		}
		if (!m_texture) {
			throw BaseException(
				L"�e�N�X�`�����ݒ肳��Ă܂���",
				L"if (!m_texture)",
				L"PNTStaticDraw::SetConstants()"
			);
		}
		//�J�������擾
		auto cameraKey = GetCameraKey();
		auto camera = GetGameObject()->GetStage()->GetCamera(cameraKey);
		//���C�g���擾
		auto lightSetKey = GetLightSetKey();
		auto lightSet = GetGameObject()->GetStage()->GetLightSet(lightSetKey);
		//�s��̒�`
		auto ptrTrans = GetGameObject()->GetComponent<Transform>();
		//������
		m_constantBufferData = {};
		m_constantBufferData.activeFlg.x = 3;
		m_constantBufferData.activeFlg.y = 1;

		auto world = ptrTrans->GetWorldMatrix();
		auto view = camera->GetViewMatrix();
		auto proj = camera->GetProjectionMatrix();
		auto worldView = world * view;
		m_constantBufferData.worldViewProj = XMMatrixTranspose(XMMatrixMultiply(worldView, proj));
		m_constantBufferData.fogVector = g_XMZero;
		m_constantBufferData.fogColor = g_XMZero;
		//���C�g�̌���
		for (int i = 0; i < lightSet->GetNumLights(); i++) {
			m_constantBufferData.lightDirection[i] = lightSet->GetLight(i).m_directional;
			m_constantBufferData.lightDiffuseColor[i] = lightSet->GetLight(i).m_diffuseColor;
			m_constantBufferData.lightSpecularColor[i] = lightSet->GetLight(i).m_specularColor;
		}
		//���[���h�s��
		m_constantBufferData.world = XMMatrixTranspose(world);

		XMMATRIX worldInverse = XMMatrixInverse(nullptr, world);
		m_constantBufferData.worldInverseTranspose[0] = worldInverse.r[0];
		m_constantBufferData.worldInverseTranspose[1] = worldInverse.r[1];
		m_constantBufferData.worldInverseTranspose[2] = worldInverse.r[2];

		XMMATRIX viewInverse = XMMatrixInverse(nullptr, view);
		m_constantBufferData.eyePosition = viewInverse.r[3];

		XMVECTOR diffuse = GetDiffuse();
		XMVECTOR alphaVector = XMVectorReplicate(GetAlpha());
		XMVECTOR emissiveColor = GetEmissive();
		XMVECTOR ambientLightColor = lightSet->GetAmbient();
		// Merge emissive and ambient light contributions.
		m_constantBufferData.emissiveColor = (emissiveColor + ambientLightColor * diffuse) * alphaVector;
		m_constantBufferData.specularColorAndPower = GetSpecularColorAndPower();

		// xyz = diffuse * alpha, w = alpha.
		m_constantBufferData.diffuseColor = XMVectorSelect(alphaVector, diffuse * alphaVector, g_XMSelect1110);
	}


	void PNTStaticDraw::OnInit() {
		auto Device = App::GetDx12Device();
		auto commandList = Device->GetCommandList();
		auto aspectRatio = Device->GetAspectRatio();
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PipeLineDesc;
		//�p�C�v���C�X�e�[�g
		m_pipelineState
			= PipelineState::CreateDefault3D<VertexPositionNormalTexture, VSPNTStatic, PSPNTStatic>(Device->GetRootSignature(), PipeLineDesc);
		//�R���X�^���g�o�b�t�@
		//�R���X�^���g�o�b�t�@�n���h�����쐬
		m_constBuffIndex = Device->GetConstBuffNextIndex();
		CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(
			Device->GetCbvSrvUavDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
			m_constBuffIndex,
			Device->GetCbvSrvDescriptorHandleIncrementSize()
		);
		m_constantBuffer = ConstantBuffer::CreateDirect(Handle, m_constantBufferData);

		auto SamplerDescriptorHandle = Device->GetSamplerDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
		Sampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);

	}

	void PNTStaticDraw::SetTextureFile(const wstring& textureName) {
		m_textureFileName = textureName;
		auto Device = App::GetDx12Device();
		//�e�N�X�`���̍쐬
		//�V�F�[�_���\�[�X�n���h�����쐬
		CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(
			Device->GetCbvSrvUavDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		//�摜�t�@�C�������ƂɃe�N�X�`�����쐬
		m_texture = Dx12Texture::CreateDx12Texture(m_textureFileName, srvHandle);
	}

	void PNTStaticDraw::OnDraw() {

		SetConstants();
		m_constantBuffer->Copy(m_constantBufferData);

		auto Device = App::GetDx12Device();
		auto commandList = Device->GetCommandList();
		//Srv
		CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
			Device->GetCbvSrvUavDescriptorHeap()->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
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
			Device->GetCbvSrvDescriptorHandleIncrementSize()
		);
		commandList->SetGraphicsRootDescriptorTable(2, CbvHandle);
		commandList->SetPipelineState(m_pipelineState.Get());

		m_texture->UpdateSRAndCreateSRV(commandList);
		commandList->IASetVertexBuffers(0, 1, &m_mesh->GetVertexBufferView());
		commandList->IASetIndexBuffer(&m_mesh->GetIndexBufferView());
		commandList->DrawIndexedInstanced(m_mesh->GetNumIndices(), 1, 0, 0, 0);

	}


}
//end basedx12
