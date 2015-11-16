function Init()

	vsnUtil("3d_light_vec", {x=1, y=1, z=1})			-- light vec
	vsnUtil("3d_light_col", {r=1,g=1,b=1})				-- light col
	vsnUtil("3d_ambient", {r=0.08,g=0.08,b=0.08})		-- ambient col

--	celestialSphere = vsnTaskCreate("sysSkyDome", "LuaScripts/SkyDome.png", 340.0)

	local skinning = false
	local camerarot = false

	camera = vsnTaskCreate("camera",
							{angle=math.pi/2, near=0.05, far=350.0},
							{x=0, y=0, z=10.0}, camerarot)			-- camera position

--	tracking = vsnTaskCreate("vrcamera")
	
	-- local hand = makeSphere(0.02, 3, 12)
	-- leap = vsnTaskCreate("leapmotion", hand)
	taskModel = vsnTaskCreate("model", {x=0, y=0, z=0}, "LuaScripts/schooldesk.vmd", true)
--[[
	local model = makeBoneCylinder(0.2, 1.44, 12, 24)
	task1 = vsnTaskCreate("test2", {x=0, y=0, z=0}, model, skinning)
	task2 = vsnTaskCreate("test2", {x=0.3, y=-1.5, z=0}, model, skinning)
	task3 = vsnTaskCreate("test2", {x=-0.3, y=-1.5, z=0}, model, skinning)
	task4 = vsnTaskCreate("test2", {x=0.5, y=0, z=0}, model, skinning)
	task5 = vsnTaskCreate("test2", {x=-0.5, y=0, z=0}, model, skinning)
]]
	hand = nil
	model = nil
--[[	
	sprite = vsnTaskCreate("test2d",
					{x=-300, y=-150, width=200, height=100, r=1, g=1, b=1, a=1.0},
					{"LuaScripts/test.png", "LuaScripts/mars.png"}, true)
]]
	return true;
end

function Execute(deltaT)
end

function Leave()
	vsnTaskKill(taskModel)
--[[
	vsnTaskKill(task1)
	vsnTaskKill(task2)
	vsnTaskKill(task3)
	vsnTaskKill(task4)
	vsnTaskKill(task5)
]]
	vsnTaskKill(celestialSphere)
	vsnTaskKill(tracking)
	vsnTaskKill(leap)
end

function OnPause()
	DebugLog("Script:OnPause()\n");
end

function OnResume()
	DebugLog("Script:OnResume()\n");
end

function makeSphere(r, vsep, hsep)
	local makeVert = function(x, y, z, u, v)
		local len = math.sqrt(x*x + y*y + z*z)
		return {
			x, y, z,
			x / len, y / len, z / len,
			-z / len, 0, x / len,
			u, v,
			255,255, 255, 255,
			0, 0, 0, 0,
			255, 0, 0, 0 }
	end

	local t_vert = {}
	local t_idx = {}
	local vnum = vsep * 2 + 1
	local hnum = hsep
	local vc, hc
	local x, y, z, u, v, rh, theta
	
	--
	-- vertex buffer
	--
	for vc = 0, vsep * 2 do
		theta = math.pi / (vsep*2) * vc
		rh = r * math.sin(theta)
		y = r * math.cos(theta)
		v = 1.0 - (vc/vsep)
		for hc = 0, hsep do
			theta = math.pi * 2 / hsep * hc
			x = rh * math.cos(theta)
			z = rh * math.sin(theta);
			u = hc / hsep
			table.insert(t_vert, makeVert(x, y, z, u, v))
		end
	end
	
	--
	-- index buffer
	--
	for vc = 0, vsep * 2 - 1 do
		for hc = 0, hsep do
			table.insert(t_idx, vc * (hsep + 1) + hc)
			table.insert(t_idx, (vc+1)*(hsep+1) + hc)
		end
	end

	return {
		bones = {
			{ pos = {x = 0, y = 0, z = 0} }
		},
		mesh = {
			vertices = t_vert,
			indices = t_idx
		},
		material = {
--			texture = "LuaScripts/mars.png",
--			normal = "LuaScripts/wood_normal.png",
			shiness = 0.125
		}
	}
end


function makeBoneCylinder(r, h, vsep, hsep)

	local makeVert = function(x, y, z, u, v, b1w, b2w, b3w)
		local len = math.sqrt(x*x+z*z);
		return {
			x, y, z,
			x/len, 0, z/len,
			-z/len, 0, x/len,
			u, v,
			255, 255, 255, 255,
			0, 1, 2, 0,
			math.floor(255*b1w), math.floor(255*b2w), math.floor(255*b3w), 0}
	end 

	local t_vert = {}
	local t_idx = {}
	local i, j, x, y, z, theta
	local w1,w2,w3;

	for i = 0, vsep do
		y = i * h / vsep
		if y < h/2 then
			w2 = y * 2 / h
			w1 = 1.0 - w2
			w3 = 0.0
		else
			w1 = 0.0
			w3 = (y-h/2)*2 / h
			w2 = 1.0 - w3
		end
		for j = 0, hsep do
			theta = math.pi * 2 * j / hsep
			x = r * math.cos(theta)
			z = -r * math.sin(theta)
			
			table.insert(t_vert, makeVert(x, y, z,
					j/hsep, i/vsep, w1, w2, w3))
		end
	end

	for i= 0, vsep - 1 do
		for j = 0, hsep do
			table.insert(t_idx, i * (hsep + 1) + j)
			table.insert(t_idx, (i+1)*(hsep+1) + j)
			table.insert(t_idx, i * (hsep + 1) + j + 1)
			table.insert(t_idx, (i+1)*(hsep+1) + j + 1)
		end
	end
	
	return {
		bones = {
			{ pos = {x = 0, y = 0, z = 0} },
			{ pos = {x = 0, y = h / 3, z = 0}, parent = 0 },
			{ pos = {x = 0, y = h * 2 / 3, z = 0}, parent = 1 }
		},
		mesh = {
			vertices = t_vert,
			indices = t_idx
		},
		material = {
			texture = "LuaScripts/mars.png",
			normal = "LuaScripts/wood_normal.png",
			specular = "LuaScripts/specularmap.png",
			shiness = 0.2
		}
	}
end
