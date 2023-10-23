// Includes
//=========

#include "../sContext.h"

#include "Includes.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>

// Helper Declarations
//====================

namespace
{
	eae6320::cResult CreateDevice( const unsigned int i_resolutionWidth, const unsigned int i_resolutionHeight );
}

// Interface
//==========

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::Graphics::sContext::Initialize( const sInitializationParameters& i_initializationParameters )
{
	auto result = Results::Success;

	windowBeingRenderedTo = i_initializationParameters.mainWindow;

	// Create an interface to a Direct3D device
	if ( !( result = CreateDevice( i_initializationParameters.resolutionWidth, i_initializationParameters.resolutionHeight ) ) )
	{
		EAE6320_ASSERTF( false, "Can't initialize context without device" );
		return result;
	}

	return result;
}

eae6320::cResult eae6320::Graphics::sContext::CleanUp()
{
	auto result = Results::Success;

	if ( direct3dImmediateContext )
	{
		direct3dImmediateContext->Release();
		direct3dImmediateContext = nullptr;
	}
	if ( direct3dDevice )
	{
		direct3dDevice->Release();
		direct3dDevice = nullptr;
	}
	if ( swapChain )
	{
		swapChain->Release();
		swapChain = nullptr;
	}

	windowBeingRenderedTo = NULL;

	return result;
}

// Helper Definitions
//===================

namespace
{
	eae6320::cResult CreateDevice( const unsigned int i_resolutionWidth, const unsigned int i_resolutionHeight )
	{
		auto& g_context = eae6320::Graphics::sContext::g_context;

		IDXGIAdapter* const useDefaultAdapter = nullptr;
		constexpr D3D_DRIVER_TYPE useHardwareRendering = D3D_DRIVER_TYPE_HARDWARE;
		constexpr HMODULE dontUseSoftwareRendering = NULL;
		constexpr unsigned int flags = D3D11_CREATE_DEVICE_SINGLETHREADED
#ifdef EAE6320_GRAPHICS_ISDEVICEDEBUGINFOENABLED
			| D3D11_CREATE_DEVICE_DEBUG
#endif
			;
		constexpr D3D_FEATURE_LEVEL* const useDefaultFeatureLevels = nullptr;
		constexpr unsigned int requestedFeatureLevelCount = 0;
		constexpr unsigned int sdkVersion = D3D11_SDK_VERSION;
		const auto swapChainDescription = [i_resolutionWidth, i_resolutionHeight, &g_context]
		{
			DXGI_SWAP_CHAIN_DESC swapChainDescription{};
			{
				DXGI_MODE_DESC& bufferDescription = swapChainDescription.BufferDesc;

				bufferDescription.Width = i_resolutionWidth;
				bufferDescription.Height = i_resolutionHeight;
				{
					DXGI_RATIONAL& refreshRate = bufferDescription.RefreshRate;

					refreshRate.Numerator = 0;	// Refresh as fast as possible
					refreshRate.Denominator = 1;
				}
				bufferDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				bufferDescription.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
				bufferDescription.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			}
			{
				DXGI_SAMPLE_DESC& multiSamplingDescription = swapChainDescription.SampleDesc;

				multiSamplingDescription.Count = 1;
				multiSamplingDescription.Quality = 0;	// Anti-aliasing is disabled
			}
			swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDescription.BufferCount = 1;
			swapChainDescription.OutputWindow = g_context.windowBeingRenderedTo;
			swapChainDescription.Windowed = TRUE;
			swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapChainDescription.Flags = 0;

			return swapChainDescription;
		}();
		D3D_FEATURE_LEVEL highestSupportedFeatureLevel;
		const auto result_create = D3D11CreateDeviceAndSwapChain( useDefaultAdapter, useHardwareRendering, dontUseSoftwareRendering,
			flags, useDefaultFeatureLevels, requestedFeatureLevelCount, sdkVersion, &swapChainDescription,
			&g_context.swapChain, &g_context.direct3dDevice, &highestSupportedFeatureLevel, &g_context.direct3dImmediateContext );
		if ( SUCCEEDED( result_create ) )
		{
			return eae6320::Results::Success;
		}
		else
		{
			EAE6320_ASSERT( false );
			eae6320::Logging::OutputError( "Direct3D failed to create a Direct3D11 device with HRESULT %#010x", result_create );
			return eae6320::Results::Failure;
		}
	}
}
