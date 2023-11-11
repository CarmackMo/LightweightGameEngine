// Includes
//=========

#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Graphics/cLine.h>
#include <Engine/Graphics/sContext.h>


// TODO: current implementation still use OpenGL as rendering backend


eae6320::cResult eae6320::Graphics::cLine::Initialize(
	VertexFormats::sVertex_line i_vertexData[],
	const uint32_t i_vertexCount,
	uint16_t i_indexData[],
	const uint32_t i_indexCount)
{
	auto result = eae6320::Results::Success;

	//// Vertex Format
	//{
	//	if (!(result = cVertexFormat::Load(eVertexType::Mesh, m_vertexFormat,
	//		"data/Shaders/Vertex/vertexInputLayout_mesh.shader")))
	//	{
	//		EAE6320_ASSERTF(false, "Can't initialize geometry without vertex format");
	//		return result;
	//	}
	//}
	//// Vertex Buffer
	//{
	//	uint32_t vertexBufferSize = sizeof(VertexFormats::sVertex_line) * i_vertexCount;

	//	auto* const direct3dDevice = sContext::g_context.direct3dDevice;
	//	EAE6320_ASSERT(direct3dDevice);
	//	EAE6320_ASSERT(vertexBufferSize <= std::numeric_limits<decltype(D3D11_BUFFER_DESC::ByteWidth)>::max());

	//	// Create a lambda expression that create and initialize BUFFER_DESC struct, and call it, and assign
	//	// it to the "bufferDescription"
	//	const auto bufferDescription = [vertexBufferSize]
	//	{
	//		D3D11_BUFFER_DESC bufferDescription{};

	//		bufferDescription.ByteWidth = static_cast<unsigned int>(vertexBufferSize);
	//		bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;	// In our class the buffer will never change after it's been created
	//		bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//		bufferDescription.CPUAccessFlags = 0;	// No CPU access is necessary
	//		bufferDescription.MiscFlags = 0;
	//		bufferDescription.StructureByteStride = 0;	// Not used

	//		return bufferDescription;
	//	}();

	//	const auto initialData = [i_vertexData]
	//	{
	//		D3D11_SUBRESOURCE_DATA initialData{};

	//		initialData.pSysMem = i_vertexData;
	//		// (The other data members are ignored for non-texture buffers)

	//		return initialData;
	//	}();

	//	const auto result_create = direct3dDevice->CreateBuffer(&bufferDescription, &initialData, &m_vertexBuffer);
	//	if (FAILED(result_create))
	//	{
	//		result = eae6320::Results::Failure;
	//		EAE6320_ASSERTF(false, "3D object vertex buffer creation failed (HRESULT %#010x)", result_create);
	//		eae6320::Logging::OutputError("Direct3D failed to create a 3D object vertex buffer (HRESULT %#010x)", result_create);
	//		return result;
	//	}

	//}

	//// Index Buffer
	//{
	//	uint32_t indexBufferSize = sizeof(uint16_t) * i_indexCount;

	//	auto* const direct3dDevice = sContext::g_context.direct3dDevice;
	//	EAE6320_ASSERT(direct3dDevice);
	//	EAE6320_ASSERT(indexBufferSize <= std::numeric_limits<decltype(D3D11_BUFFER_DESC::ByteWidth)>::max());

	//	const auto bufferDescription = [indexBufferSize]
	//	{
	//		D3D11_BUFFER_DESC bufferDescription{};

	//		bufferDescription.ByteWidth = static_cast<unsigned int>(indexBufferSize);
	//		bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;	// In our class the buffer will never change after it's been created
	//		bufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//		bufferDescription.CPUAccessFlags = 0;	// No CPU access is necessary
	//		bufferDescription.MiscFlags = 0;
	//		bufferDescription.StructureByteStride = 0;	// Not used

	//		return bufferDescription;
	//	}();

	//	const auto initialData = [i_indexData]
	//	{
	//		D3D11_SUBRESOURCE_DATA initialData{};

	//		initialData.pSysMem = i_indexData;
	//		// (The other data members are ignored for non-texture buffers)

	//		return initialData;
	//	}();

	//	const auto result_create = direct3dDevice->CreateBuffer(&bufferDescription, &initialData, &m_indexBuffer);
	//	if (FAILED(result_create))
	//	{
	//		result = eae6320::Results::Failure;
	//		EAE6320_ASSERTF(false, "3D object index buffer creation failed (HRESULT %#010x)", result_create);
	//		eae6320::Logging::OutputError("Direct3D failed to create a 3D object index buffer (HRESULT %#010x)", result_create);
	//		return result;
	//	}
	//}



	//// TODO: Debug
	//{
	//	ID3D11Debug* debugDev = NULL;
	//	sContext::g_context.direct3dDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debugDev));
	//	debugDev->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

	//	auto temp = 1;
	//}


	return result;
}


eae6320::cResult eae6320::Graphics::cLine::CleanUp()
{
	//if (m_vertexBuffer)
	//{
	//	m_vertexBuffer->Release();
	//	m_vertexBuffer = nullptr;
	//}
	//if (m_indexBuffer)
	//{
	//	m_indexBuffer->Release();
	//	m_indexBuffer = nullptr;
	//}
	//if (m_vertexFormat)
	//{
	//	m_vertexFormat->DecrementReferenceCount();
	//	m_vertexFormat = nullptr;
	//}

	return Results::Success;
}


void eae6320::Graphics::cLine::Draw()
{
	//auto* const direct3dImmediateContext = sContext::g_context.direct3dImmediateContext;
	//EAE6320_ASSERT(direct3dImmediateContext);

	//// Bind a specific vertex buffer to the device as a data source
	//{
	//	EAE6320_ASSERT(m_vertexBuffer != nullptr);

	//	constexpr unsigned int startingSlot = 0;
	//	constexpr unsigned int vertexBufferCount = 1;
	//	// The "stride" defines how large a single vertex is in the stream of data
	//	constexpr unsigned int bufferStride = sizeof(VertexFormats::sVertex_line);
	//	// It's possible to start streaming data in the middle of a vertex buffer
	//	constexpr unsigned int bufferOffset = 0;

	//	direct3dImmediateContext->IASetVertexBuffers(startingSlot, vertexBufferCount, &m_vertexBuffer, &bufferStride, &bufferOffset);
	//}

	//// Bind a specific index buffer to the device
	//{
	//	EAE6320_ASSERT(m_indexBuffer);
	//	constexpr DXGI_FORMAT indexFormat = DXGI_FORMAT_R16_UINT;
	//	// The indices start at the beginning of the buffer
	//	constexpr unsigned int offset = 0;
	//	direct3dImmediateContext->IASetIndexBuffer(m_indexBuffer, indexFormat, offset);
	//}

	//// Specify what kind of data the vertex buffer holds
	//{
	//	// Bind the vertex format (which defines how to interpret a single vertex)
	//	{
	//		EAE6320_ASSERT(m_vertexFormat != nullptr);
	//		m_vertexFormat->Bind();
	//	}
	//	// Set the topology (which defines how to interpret multiple vertices as a single "primitive";
	//	// the vertex buffer was defined as a triangle list
	//	// (meaning that every primitive is a triangle and will be defined by three vertices)
	//	direct3dImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	//}

	//// Render triangles from the currently-bound vertex buffer
	//{
	//	// new draw call to draw indexed, non-instanced primitives.
	//	// It's possible to start rendering primitives in the middle of the stream
	//	// 
	//	//direct3dImmediateContext->DrawIndexed(static_cast<unsigned int>(m_indexCountToRender), 0, 0);

	//	direct3dImmediateContext->Draw(static_cast<unsigned int>(m_indexCountToRender), 0);
	//}
}