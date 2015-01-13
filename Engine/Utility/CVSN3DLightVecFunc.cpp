#include "CVSN3DLightVecFunc.h"
#include "C3DDrawEnv.h"
#include "CVSNKVObj.h"

CVSN3DLightVecFunc::CVSN3DLightVecFunc() : IVSNScriptUtilFunc("3d_light_vec") {}
CVSN3DLightVecFunc::~CVSN3DLightVecFunc() {}

bool
CVSN3DLightVecFunc::utilFunc(CVSNScriptArgs& args, CVSNScriptArgs& ret)
{
	std::auto_ptr<CVSNKVObj> light_vec(args.getTable(1));
	C3DVec lightvec((*light_vec)["x"], (*light_vec)["y"], (*light_vec)["z"], 1.0f);
	CGLEnv::getInstance().DrawEnv<C3DDrawEnv>(CGLEnv::C3D)->setLightVec(lightvec);
	return true;
}



CVSN3DLightColFunc::CVSN3DLightColFunc() : IVSNScriptUtilFunc("3d_light_col") {}
CVSN3DLightColFunc::~CVSN3DLightColFunc() {}

bool
CVSN3DLightColFunc::utilFunc(CVSNScriptArgs& args, CVSNScriptArgs& ret)
{
	std::auto_ptr<CVSNKVObj> light_col(args.getTable(1));
	float alpha = (light_col->ContainsKey("a")) ? (float)(*light_col)["a"] : 1.0f;
	CGLEnv::getInstance().DrawEnv<C3DDrawEnv>(CGLEnv::C3D)->setLightCol((*light_col)["r"], (*light_col)["g"], (*light_col)["b"], alpha);
	return true;
}



CVSN3DAmbientFunc::CVSN3DAmbientFunc() : IVSNScriptUtilFunc("3D_ambient") {}
CVSN3DAmbientFunc::~CVSN3DAmbientFunc() {}

bool
CVSN3DAmbientFunc::utilFunc(CVSNScriptArgs& args, CVSNScriptArgs& ret)
{
	std::auto_ptr<CVSNKVObj> ambient(args.getTable(1));
	float alpha = (ambient->ContainsKey("a")) ? (float)(*ambient)["a"] : 1.0f;
	CGLEnv::getInstance().DrawEnv<C3DDrawEnv>(CGLEnv::C3D)->setAmbient((*ambient)["r"], (*ambient)["g"], (*ambient)["b"], alpha);
	return true;
}
