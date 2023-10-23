
return
{
	vertices =
	{
        { x = 0.0, y = 0.0, z = 0.0, r = 1.0, g = 0.0, b = 0.0, a = 1.0,},  -- 0
		{ x = 1.0, y = 0.0, z = 0.0, r = 0.0, g = 1.0, b = 0.0, a = 1.0,},  -- 1
		{ x = 1.0, y = 1.0, z = 0.0, r = 0.0, g = 0.0, b = 1.0, a = 1.0,},  -- 2
		{ x = 0.0, y = 1.0, z = 0.0, r = 1.0, g = 1.0, b = 1.0, a = 1.0,},  -- 3
	},

	indices =
	{
		-- Using right-handed order
		{ u = 0, v = 1, w = 2, },	-- triangle 0
	},

    indexOfFirstIndexToUse = 0,
    offsetToAddToEachIndex = 0,
}