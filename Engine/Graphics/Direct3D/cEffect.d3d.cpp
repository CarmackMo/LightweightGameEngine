// Includes
//=========

#include <Engine/Asserts/Asserts.h>
#include "../cEffect.h"
#include "../sContext.h"


void eae6320::Graphics::cEffect::Bind()
{
	auto* const direct3dImmediateContext = sContext::g_context.direct3dImmediateContext;

	// Bind the shading data
	{
		constexpr ID3D11ClassInstance* const* noInterfaces = nullptr;
		constexpr unsigned int interfaceCount = 0;
		// Vertex shader
		{
			EAE6320_ASSERT((m_vertexShader != nullptr) && (m_vertexShader->m_shaderObject.vertex != nullptr));
			direct3dImmediateContext->VSSetShader(m_vertexShader->m_shaderObject.vertex, noInterfaces, interfaceCount);
		}
		// Fragment shader
		{
			EAE6320_ASSERT((m_fragmentShader != nullptr) && (m_fragmentShader->m_shaderObject.vertex != nullptr));
			direct3dImmediateContext->PSSetShader(m_fragmentShader->m_shaderObject.fragment, noInterfaces, interfaceCount);
		}
	}
	// Render state
	{
		m_renderState->Bind();
	}
}


eae6320::cResult eae6320::Graphics::cEffect::Initialize(const std::string& i_vertexShaderPath, const std::string& i_fragmentShaderPath)
{
	return InitializeShader(i_vertexShaderPath, i_fragmentShaderPath);
}


eae6320::cResult eae6320::Graphics::cEffect::CleanUp()
{
	return CleanUpShader();
}