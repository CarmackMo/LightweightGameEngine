// Includes
//=========

#include <Engine/Asserts/Asserts.h>
#include <Engine/Graphics/cEffect.h>
#include <Engine/Graphics/Graphics.h>
#include <Engine/Graphics/sContext.h>
#include <Engine/Logging/Logging.h>
#include <Engine/ScopeGuard/cScopeGuard.h>


void eae6320::Graphics::cEffect::Bind()
{
	{
		EAE6320_ASSERT(m_programId != 0);
		glUseProgram(m_programId);
		EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
	}
	// Render state
	{
		m_renderState->Bind();
	}
}


eae6320::cResult eae6320::Graphics::cEffect::Initialize(const std::string& i_vertexShaderPath, const std::string& i_fragmentShaderPath)
{
	// Wait for the rendering thread release the rendering context
	// Then claim the rendering context and signal the rendering thread
	{
		if (Graphics::WaitUntilContextReleaseByRenderingThread(5000) == Results::Success)
		{
			if (Graphics::ResetThatContextIsClaimedByApplicationThread() == Results::Failure)
			{
				EAE6320_ASSERTF(false, "Couldn't signal that application thread is trying to claim rendering context");
				Logging::OutputError("Couldn't signal that application thread is trying to claim rendering context");
				return Results::Failure;
			}

			// TODO
			auto id1 = GetCurrentThreadId();
			auto id2 = sContext::g_context.ownerThreadId;

			if (sContext::g_context.EnableContext(GetCurrentThreadId()) == FALSE)
			{
				EAE6320_ASSERTF(false, "Claim rendering context for application thread failed");
				Logging::OutputError("Claim rendering context for application thread failed");
				return Results::Failure;
			}
		}
		else
		{
			EAE6320_ASSERTF(false, "Failed to wait for rendering thread releases rendering context")
			Logging::OutputError("Failed to wait for rendering thread releases rendering context");
			return Results::Failure;
		}
	}


	auto result = Results::Success;
	result = InitializeShader(i_vertexShaderPath, i_fragmentShaderPath);

	// If initialization fails, clean up the OpenGL program that is already
	// been registered.
	cScopeGuard scopeGuard_program([&result, this]
		{
			if (!result)
			{
				if (m_programId != 0)
				{
					glDeleteProgram(m_programId);
					const auto errorCode = glGetError();
					if (errorCode != GL_NO_ERROR)
					{
						if (result)
						{
							result = Results::Failure;
						}

						EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
						Logging::OutputError("OpenGL failed to delete the program: %s",
							reinterpret_cast<const char*>(gluErrorString(errorCode)));
					}
					m_programId = 0;
				}
			}
		}
	);

	// Initialize program
	{
		EAE6320_ASSERT(m_programId == 0);
		m_programId = glCreateProgram();
		const auto errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to create a program: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			return result;
		}
		else if (m_programId == 0)
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERT(false);
			eae6320::Logging::OutputError("OpenGL failed to create a program");
			return result;
		}
	}

	// Attach the shaders to the program
	{
		// Vertex
		{
			EAE6320_ASSERT((m_vertexShader != nullptr) && (m_vertexShader->m_shaderId != 0));
			glAttachShader(m_programId, m_vertexShader->m_shaderId);
			const auto errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				eae6320::Logging::OutputError("OpenGL failed to attach the vertex shader to the program: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				return result;
			}
		}
		// Fragment
		{
			EAE6320_ASSERT((m_fragmentShader != nullptr) && (m_fragmentShader->m_shaderId != 0));
			glAttachShader(m_programId, m_fragmentShader->m_shaderId);
			const auto errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				eae6320::Logging::OutputError("OpenGL failed to attach the fragment shader to the program: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				return result;
			}
		}
	}

	// Link the program
	{
		glLinkProgram(m_programId);
		const auto errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			// Get link info
			// (this won't be used unless linking fails
			// but it can be useful to look at when debugging)
			std::string linkInfo;
			{
				GLint infoSize;
				glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &infoSize);
				const auto errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					if (infoSize > 0)
					{
						auto* const info = new (std::nothrow) GLchar[infoSize];
						if (info)
						{
							eae6320::cScopeGuard scopeGuard_info([info]
								{
									delete[] info;
								});
							constexpr GLsizei* const dontReturnLength = nullptr;
							glGetProgramInfoLog(m_programId, static_cast<GLsizei>(infoSize), dontReturnLength, info);
							const auto errorCode = glGetError();
							if (errorCode == GL_NO_ERROR)
							{
								linkInfo = info;
							}
							else
							{
								result = eae6320::Results::Failure;
								EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
								eae6320::Logging::OutputError("OpenGL failed to get link info of the program: %s",
									reinterpret_cast<const char*>(gluErrorString(errorCode)));
								return result;
							}
						}
						else
						{
							result = eae6320::Results::OutOfMemory;
							EAE6320_ASSERTF(false, "Couldn't allocate memory for the program link info");
							eae6320::Logging::OutputError("Failed to allocate memory for the program link info");
							return result;
						}
					}
				}
				else
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					eae6320::Logging::OutputError("OpenGL failed to get the length of the program link info: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
					return result;
				}
			}
			// Check to see if there were link errors
			GLint didLinkingSucceed;
			{
				glGetProgramiv(m_programId, GL_LINK_STATUS, &didLinkingSucceed);
				const auto errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					if (didLinkingSucceed == GL_FALSE)
					{
						result = eae6320::Results::Failure;
						EAE6320_ASSERTF(false, linkInfo.c_str());
						eae6320::Logging::OutputError("The program failed to link: %s",
							linkInfo.c_str());
						return result;
					}
				}
				else
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					eae6320::Logging::OutputError("OpenGL failed to find out if linking of the program succeeded: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
					return result;
				}
			}
		}
		else
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to link the program: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			return result;
		}
	}


	// Release rendering context and signal the rendering thread 
	{
		if (sContext::g_context.DisableContext() == FALSE)
		{
			EAE6320_ASSERTF(false, "Release rendering context from application failed");
			Logging::OutputError("Release rendering context from application failed");
		}

		if (Graphics::SignalThatContextIsReleasedByApplicationThread() == Results::Failure)
		{
			EAE6320_ASSERTF(false, "Couldn't signal that application thread releases rendering context");
			Logging::OutputError("Couldn't signal that application thread releases rendering context");
		}
	}

	return result;
}


eae6320::cResult eae6320::Graphics::cEffect::CleanUp()
{

	// Wait for the rendering thread release the rendering context
	// Then claim the rendering context and signal the rendering thread
	{
		if (Graphics::WaitUntilContextReleaseByRenderingThread(100) == Results::Success)
		{
			if (Graphics::ResetThatContextIsClaimedByApplicationThread() == Results::Failure)
			{
				EAE6320_ASSERTF(false, "Couldn't signal that application thread is trying to claim rendering context");
				Logging::OutputError("Couldn't signal that application thread is trying to claim rendering context");
				return Results::Failure;
			}

			// TODO
			auto id1 = GetCurrentThreadId();
			auto id2 = sContext::g_context.ownerThreadId;
			auto staticDC = GetDC(Graphics::sContext::g_context.windowBeingRenderedTo);
			auto currentDC = wglGetCurrentDC();

			if (sContext::g_context.EnableContext(GetCurrentThreadId()) == FALSE)
			{
				EAE6320_ASSERTF(false, "Claim rendering context for application thread failed");
				Logging::OutputError("Claim rendering context for application thread failed");
				return Results::Failure;
			}
		}
		else
		{
			EAE6320_ASSERTF(false, "Failed to wait for rendering thread releases rendering context");
			Logging::OutputError("Failed to wait for rendering thread releases rendering context");
			return Results::Failure;
		}
	}


	auto result = eae6320::Results::Success;

	// Clean up OpenGL program
	if (m_programId != 0)
	{
		glDeleteProgram(m_programId);
		const auto errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			if (result)
			{
				result = eae6320::Results::Failure;
			}

			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to delete the program: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
		}
		m_programId = 0;
	}

	// Clean up shaders
	{
		result = CleanUpShader();
	}

	// Release rendering context and signal the rendering thread 
	{
		if (sContext::g_context.DisableContext() == FALSE)
		{
			EAE6320_ASSERTF(false, "Release rendering context from application failed");
			Logging::OutputError("Release rendering context from application failed");
		}

		if (Graphics::SignalThatContextIsReleasedByApplicationThread() == Results::Failure)
		{
			EAE6320_ASSERTF(false, "Couldn't signal that application thread releases rendering context");
			Logging::OutputError("Couldn't signal that application thread releases rendering context");
		}
	}

	return result;
}
