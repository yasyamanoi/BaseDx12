#pragma once
#include "stdafx.h"

namespace basedx12 {

	//--------------------------------------------------------------------------------------
	///	メッシュクラス
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
		//頂点のみ
		template<typename T>
		static shared_ptr<Dx12Mesh> CreateDx12Mesh(const ComPtr<ID3D12GraphicsCommandList>& commandList, const vector<T>& vertices) {
			//デバイスの取得
			auto Dev = App::GetID3D12Device();
			shared_ptr<Dx12Mesh> Ptr = shared_ptr<Dx12Mesh>(new Dx12Mesh());
			UINT vertexBufferSize = (UINT)(sizeof(T) * vertices.size());
			//頂点バッファの作成
			{
				ThrowIfFailed(Dev->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
					D3D12_RESOURCE_STATE_COPY_DEST,
					nullptr,
					IID_PPV_ARGS(&Ptr->m_vertexBuffer)),
					L"頂点バッファ作成に失敗しました。",
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
					L"頂点バッファ作成に失敗しました。",
					L"if(FAILED(Dev->CreateCommittedResource())",
					L"Dx12Mesh::CreateDx12Mesh()"
				);

				//頂点バッファの更新
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
			//頂点数の設定
			Ptr->m_numVertices = static_cast<UINT>(vertices.size());
			return Ptr;
		}

		//頂点とインデックス
		template<typename T>
		static shared_ptr<Dx12Mesh> CreateDx12Mesh(const ComPtr<ID3D12GraphicsCommandList>& commandList,const vector<T>& vertices, const vector<uint32_t>& indices) {
			//デバイスの取得
			auto Dev = App::GetID3D12Device();
			shared_ptr<Dx12Mesh> Ptr = shared_ptr<Dx12Mesh>(new Dx12Mesh());
			UINT vertexBufferSize = (UINT)(sizeof(T) * vertices.size());
			//頂点バッファの作成
			{
				ThrowIfFailed(Dev->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
					D3D12_RESOURCE_STATE_COPY_DEST,
					nullptr,
					IID_PPV_ARGS(&Ptr->m_vertexBuffer)),
					L"頂点バッファ作成に失敗しました。",
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
					L"頂点バッファ作成に失敗しました。",
					L"if(FAILED(Dev->CreateCommittedResource())",
					L"Dx12Mesh::CreateDx12Mesh()"
				);

				//頂点バッファの更新
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
			//頂点数の設定
			Ptr->m_numVertices = static_cast<UINT>(vertices.size());
			//インデックスバッファの作成
			UINT indexBufferSize = static_cast<UINT>(sizeof(uint32_t) * indices.size());
			{
				ThrowIfFailed(Dev->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize),
					D3D12_RESOURCE_STATE_COPY_DEST,
					nullptr,
					IID_PPV_ARGS(&Ptr->m_indexBuffer)),
					L"インデックスバッファ作成に失敗しました。",
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
					L"インデックスアップロードバッファ作成に失敗しました。",
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
			//インデックス数の設定
			Ptr->m_numIndices = static_cast<UINT>(indices.size());
			return Ptr;
		}

	};


}
//end basedx12
