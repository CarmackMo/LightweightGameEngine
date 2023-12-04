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

		{ path = "Meshes/mesh_bullet_enemy.lua" },
		{ path = "Meshes/mesh_bullet_player.lua" },
		{ path = "Meshes/mesh_enemy_alien.lua" },
		{ path = "Meshes/mesh_enemy_rock.lua" },
	},

	audios = 
	{
		{ path = "Audios/audio_Main.wav" },
		{ path = "Audios/audio_Shoot.wav" },
		{ path = "Audios/audio_Boom_Enemy.wav" },
		{ path = "Audios/audio_Hit_Enemy.wav" },
	},
}
