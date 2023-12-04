--[[
	This file lists every asset that must be built by the AssetBuildSystem
]]

return
{
	shaders =
	{
		{ path = "Shaders/Fragment/moziheng.shader", arguments = { "fragment" } },
	
		{ path = "Shaders/Vertex/standard.shader", arguments = { "vertex" } },
		{ path = "Shaders/Fragment/standard.shader", arguments = { "fragment" } },

		{ path = "Shaders/Vertex/vertexInputLayout_mesh.shader", arguments = { "vertex" } },
	},

	meshes =
	{
		{ path = "Meshes/mesh_rectangle.lua" },
		{ path = "Meshes/mesh_triangle.lua" },
		{ path = "Meshes/mesh_plane.lua" },
		{ path = "Meshes/mesh_cube.lua" },
		{ path = "Meshes/mesh_keqing.lua" },
		{ path = "Meshes/mesh_keqing_skin.lua" },
		{ path = "Meshes/mesh_ganyu.lua" },
	},

	audios = 
	{
		{ path = "Audios/audio_Main.wav" },
		{ path = "Audios/audio_Shoot.wav" },
	},
}
