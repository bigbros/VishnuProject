function Init()

	vsnUtil("3d_light_vec", {x=1, y=1, z=1})			-- light vec
	vsnUtil("3d_light_col", {r=1,g=1,b=1})				-- light col
	vsnUtil("3d_ambient", {r=0.08,g=0.08,b=0.08})		-- ambient col
	
	camera = vsnTaskCreate("camera", {x=0, y=0, z=100})			-- camera position

	task = vsnTaskCreate("test", makeHalfSphere(50, 12, 24), true)

	return true
end

function Execute(deltaT)
end

function Leave()
	vsnTaskKill(task)
end


function makeHalfSphere(r, vsep, hsep)

	local t_vert = {}
	local t_idx = {}
	local i, j, x, y, z, rx, theta
	
	local makeVert = function(x, y, z, u, v)
		local len = math.sqrt(x*x+y*y+z*z)
		local tlen = math.sqrt(x*x+z*z)
		return {
			x, y, z,
			x/len, y/len, z/len,
			-z/tlen, 0, x/tlen,
			u, v,
			255, 255, 255, 255,
			0, 0, 0, 0,
			255, 0, 0, 0 }
	end

	table.insert(t_vert, makeVert(0, r, 0, 0, 1.0))

	for i=1,vsep do
		theta = (math.pi / (vsep * 2)) * (vsep - i)
		rx = r * math.cos(theta);
		y = r * math.sin(theta)
		for j = 0, hsep do
	 	 		theta = (math.pi * 2 / hsep) * j
			x = rx * math.cos(theta)
			z = -rx * math.sin(theta)
			table.insert(t_vert, makeVert(x, y, z, j/hsep, 1.0 - i/(2.0*vsep)))
		end
	end
	
	for i = vsep-1,1,-1 do 
		theta = (math.pi / (vsep * 2)) * (vsep - i)
		rx = r * math.cos(theta);
		y = -r * math.sin(theta)
		for j = 0, hsep do
	 	 	theta = (math.pi * 2 / hsep) * j
			x = rx * math.cos(theta)
			z = -rx * math.sin(theta)
			table.insert(t_vert, makeVert(x, y, z, j/hsep, i/(2.0*vsep)))
		end
	end
	
	table.insert(t_vert, makeVert(0, -r, 0, 0, 0))
	
	for i=0,hsep do
		table.insert(t_idx, 0)
		table.insert(t_idx, i+1)
		table.insert(t_idx,(i+1) + 1)
		table.insert(t_idx,(i+1) + 1)
	end

	for i=1,vsep*2-2 do
		for j=0,hsep do
			table.insert(t_idx, 1 + (i - 1) * (hsep + 1) + j)
			table.insert(t_idx, 1 + i * (hsep + 1) + j)
			table.insert(t_idx, 1 + (i - 1) * (hsep + 1) + (j + 1))
			table.insert(t_idx, 1 + i * (hsep + 1) + (j + 1))
		end
	end
	
	for i=0,hsep do
		table.insert(t_idx, 1+ (vsep*2-2)*(hsep+1)+i)
		table.insert(t_idx, 1+ (vsep*2-2)*(hsep+1)+i+1)
		table.insert(t_idx, 1+ (vsep*2-2)*(hsep+2))
		table.insert(t_idx, 1+ (vsep*2-2)*(hsep+2))
	end			


	return {
		bones = {
			{ pos = { x = 0, y = 0, z = 0 } }
		},
		mesh = {
			vertices = t_vert,
			indices = t_idx
		},
		material = {
			texture = "LuaScripts/test.png"
		}
	}
end


