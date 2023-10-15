
return
{
	vertices =
	{
        { x =  0.2, y =  0.2, z = 0.0, r = 0.5, g = 0.0, b = 0.25, a = 1.0,},  -- 0
		{ x =  0.2, y = -0.2, z = 0.0, r = 0.0, g = 0.25, b = 0.0, a = 1.0,},  -- 1
		{ x = -0.2, y = -0.2, z = 0.0, r = 0.25, g = 0.0, b = 0.5, a = 1.0,},  -- 2
		{ x = -0.2, y =  0.2, z = 0.0, r = 0.75, g = 0.75, b = 0.75, a = 1.0,},  -- 3
	},

	indices =
	{
		-- Using right-handed order
		{ u = 0, v = 2, w = 1, },	-- triangle 0
		{ u = 0, v = 3, w = 2, },	-- triangle 1
	},

    indexOfFirstIndexToUse = 0,
    offsetToAddToEachIndex = 0,
}