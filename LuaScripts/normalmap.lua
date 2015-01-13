function Init()

	local model = makeNormalMapCube(100)
	
	vsnUtil("3d_light_vec", {x=0, y=0, z=1})			-- light vec
	vsnUtil("3d_light_col", {r=1,g=1,b=1})				-- light col
	vsnUtil("3d_ambient", {r=0.08,g=0.08,b=0.08})		-- ambient col
	
	local skinning = false
	local camerarot = true

	camera = vsnTaskCreate("camera", {x=0, y=0, z=100}, camerarot)			-- camera position

	task1 = vsnTaskCreate("test", model, true)

	return true;
end

function Execute(deltaT)
end

function Leave()
	vsnTaskKill(task1)
	vsnTaskKill(camera)
end

function OnPause()
	DebugLog("Script:OnPause()\n");
end

function OnResume()
	DebugLog("Script:OnResume()\n");
end


function makeNormalMapCube(siz)

	local makeVert = function(x, y, z, nx, ny, nz, u, v)
		local len = math.sqrt(nx*nx+ny*ny+nz*nz);
		return {
			x, y, z,
			nx/len, ny/len, nz/len,
			nz/len, 0, nx/len,
			u, v,
			255, 255, 255, 255,
			0, 0, 0, 0,
			255, 0, 0, 0}
	end 

	local t_vert = {}
	local t_idx = {}
	
	table.insert(t_vert, makeVert(-siz/2, siz/2, 0,		0, 0, 1, 0, 1));
	table.insert(t_vert, makeVert(siz/2, siz/2, 0,		0, 0, 1, 1, 1));
	table.insert(t_vert, makeVert(-siz/2, -siz/2, 0,	0, 0, 1, 0, 0));
	table.insert(t_vert, makeVert(siz/2, -siz/2, 0, 	0, 0, 1, 1, 0));
	
	table.insert(t_idx, 0);
	table.insert(t_idx, 1);
	table.insert(t_idx, 2);
	table.insert(t_idx, 3);
	
	return {
		bones = {
			{ pos = {x = 0, y = 0, z = 0} },
		},
		mesh = {
			vertices = t_vert,
			indices = t_idx
		},
		material = {
			texture = "LuaScripts/mars.png",
			normal = "LuaScripts/baketest.png",
			shiness = 4.0
		}
	}
end
