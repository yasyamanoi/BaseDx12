#pragma once
#include "stdafx.h"

namespace basedx12 {

	//--------------------------------------------------------------------------------------
	///	���b�V���N���X
	//--------------------------------------------------------------------------------------
	class Dx12Mesh {
		ComPtr<ID3D12Resource> m_vertexBuffer;
		ComPtr<ID3D12Resource> m_indexBuffer;
		ComPtr<ID3D12Resource> m_vertexBufferUploadHeap;
		ComPtr<ID3D12Resource> m_indexBufferUploadHeap;

		D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
		D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
		UINT m_numVertices;
		UINT m_numIndices;
		Dx12Mesh() {}
	public:
		~Dx12Mesh() {}
		ComPtr<ID3D12Resource> GetVertexBuffer() const {
			return m_vertexBuffer;
		}
		ComPtr<ID3D12Resource> GetVertexBufferUploadHeap() const {
			return m_vertexBufferUploadHeap;
		}

		ComPtr<ID3D12Resource> GetIndexBuffer() const {
			return m_vertexBuffer;
		}
		ComPtr<ID3D12Resource> GetIndexBufferUploadHeap() const {
			return m_indexBufferUploadHeap;
		}

		UINT GetNumVertices()const {
			return m_numVertices;
		}
		UINT GetNumIndices()const {
			return m_numIndices;
		}
		const D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView() const {
			return m_vertexBufferView;
		}
		const D3D12_INDEX_BUFFER_VIEW& GetIndexBufferView() const {
			return m_indexBufferView;
		}
		//���_�̂�
		template<typename T>
		static shared_ptr<Dx12Mesh> CreateDx12Mesh(const ComPtr<ID3D12GraphicsCommandList>& commandList, const vector<T>& vertices) {
			//�f�o�C�X�̎擾
			auto Dev = App::GetID3D12Device();
			shared_ptr<Dx12Mesh> Ptr = shared_ptr<Dx12Mesh>(new Dx12Mesh());
			UINT vertexBufferSize = (UINT)(sizeof(T) * vertices.size());
			//���_�o�b�t�@�̍쐬
			{
				ThrowIfFailed(Dev->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
					D3D12_RESOURCE_STATE_COPY_DEST,
					nullptr,
					IID_PPV_ARGS(&Ptr->m_vertexBuffer)),
					L"���_�o�b�t�@�쐬�Ɏ��s���܂����B",
					L"if(FAILED(Dev->CreateCommittedResource())",
					L"Dx12Mesh::CreateDx12Mesh()"
				);

				ThrowIfFailed(Dev->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&Ptr->m_vertexBufferUploadHeap)),
					L"���_�o�b�t�@�쐬�Ɏ��s���܂����B",
					L"if(FAILED(Dev->CreateCommittedResource())",
					L"Dx12Mesh::CreateDx12Mesh()"
				);

				//���_�o�b�t�@�̍X�V
				D3D12_SUBRESOURCE_DATA vertexData = {};
				vertexData.pData = &vertices[0];
				vertexData.RowPitch = vertexBufferSize;
				vertexData.SlicePitch = vertexData.RowPitch;

				UpdateSubresources<1>(
					commandList.Get(),
					Ptr->m_vertexBuffer.Get(), 
					Ptr->m_vertexBufferUploadHeap.Get(), 
					0, 
					0, 
					1, 
					&vertexData
					);
				commandList->ResourceBarrier(
					1, 
					&CD3DX12_RESOURCE_BARRIER::Transition(
						Ptr->m_vertexBuffer.Get(),
						D3D12_RESOURCE_STATE_COPY_DEST, 
						D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER
					)
				);

				Ptr->m_vertexBufferView.BufferLocation = Ptr->m_vertexBuffer->GetGPUVirtualAddress();
				Ptr->m_vertexBufferView.StrideInBytes = static_cast<UINT>(sizeof(T));
				Ptr->m_vertexBufferView.SizeInBytes = vertexBufferSize;
			}
			//���_���̐ݒ�
			Ptr->m_numVertices = static_cast<UINT>(vertices.size());
			return Ptr;
		}

		//���_�ƃC���f�b�N�X
		template<typename T>
		static shared_ptr<Dx12Mesh> CreateDx12Mesh(const ComPtr<ID3D12GraphicsCommandList>& commandList,const vector<T>& vertices, const vector<uint32_t>& indices) {
			//�f�o�C�X�̎擾
			auto Dev = App::GetID3D12Device();
			shared_ptr<Dx12Mesh> Ptr = shared_ptr<Dx12Mesh>(new Dx12Mesh());
			UINT vertexBufferSize = (UINT)(sizeof(T) * vertices.size());
			//���_�o�b�t�@�̍쐬
			{
				ThrowIfFailed(Dev->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
					D3D12_RESOURCE_STATE_COPY_DEST,
					nullptr,
					IID_PPV_ARGS(&Ptr->m_vertexBuffer)),
					L"���_�o�b�t�@�쐬�Ɏ��s���܂����B",
					L"if(FAILED(Dev->CreateCommittedResource())",
					L"Dx12Mesh::CreateDx12Mesh()"
				);

				ThrowIfFailed(Dev->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&Ptr->m_vertexBufferUploadHeap)),
					L"���_�o�b�t�@�쐬�Ɏ��s���܂����B",
					L"if(FAILED(Dev->CreateCommittedResource())",
					L"Dx12Mesh::CreateDx12Mesh()"
				);

				//���_�o�b�t�@�̍X�V
				D3D12_SUBRESOURCE_DATA vertexData = {};
				vertexData.pData = &vertices[0];
				vertexData.RowPitch = vertexBufferSize;
				vertexData.SlicePitch = vertexData.RowPitch;

				UpdateSubresources<1>(
					commandList.Get(),
					Ptr->m_vertexBuffer.Get(),
					Ptr->m_vertexBufferUploadHeap.Get(),
					0,
					0,
					1,
					&vertexData
					);
				commandList->ResourceBarrier(
					1,
					&CD3DX12_RESOURCE_BARRIER::Transition(
						Ptr->m_vertexBuffer.Get(),
						D3D12_RESOURCE_STATE_COPY_DEST,
						D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER
					)
				);

				Ptr->m_vertexBufferView.BufferLocation = Ptr->m_vertexBuffer->GetGPUVirtualAddress();
				Ptr->m_vertexBufferView.StrideInBytes = static_cast<UINT>(sizeof(T));
				Ptr->m_vertexBufferView.SizeInBytes = vertexBufferSize;
			}
			//���_���̐ݒ�
			Ptr->m_numVertices = static_cast<UINT>(vertices.size());
			//�C���f�b�N�X�o�b�t�@�̍쐬
			UINT indexBufferSize = static_cast<UINT>(sizeof(uint32_t) * indices.size());
			{
				ThrowIfFailed(Dev->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize),
					D3D12_RESOURCE_STATE_COPY_DEST,
					nullptr,
					IID_PPV_ARGS(&Ptr->m_indexBuffer)),
					L"�C���f�b�N�X�o�b�t�@�쐬�Ɏ��s���܂����B",
					L"if(FAILED(Dev->CreateCommittedResource())",
					L"Dx12Mesh::CreateDx12Mesh()"
				);

				ThrowIfFailed(Dev->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&Ptr->m_indexBufferUploadHeap)),
					L"�C���f�b�N�X�A�b�v���[�h�o�b�t�@�쐬�Ɏ��s���܂����B",
					L"if(FAILED(Dev->CreateCommittedResource())",
					L"Dx12Mesh::CreateDx12Mesh()"
				);

				// Copy data to the intermediate upload heap and then schedule a copy 
				// from the upload heap to the index buffer.
				D3D12_SUBRESOURCE_DATA indexData = {};
				indexData.pData = (void*)&indices[0];
				indexData.RowPitch = indexBufferSize;
				indexData.SlicePitch = indexData.RowPitch;

				UpdateSubresources<1>(commandList.Get(), Ptr->m_indexBuffer.Get(), Ptr->m_indexBufferUploadHeap.Get(), 0, 0, 1, &indexData);
				commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(Ptr->m_indexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER));


				// Describe the index buffer view.
				Ptr->m_indexBufferView.BufferLocation = Ptr->m_indexBuffer->GetGPUVirtualAddress();
				Ptr->m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
				Ptr->m_indexBufferView.SizeInBytes = indexBufferSize;

			}
			//�C���f�b�N�X���̐ݒ�
			Ptr->m_numIndices = static_cast<UINT>(indices.size());
			return Ptr;
		}

	};


}
//end basedx12
