#ifndef CVSNTracking_h
#define CVSNTracking_h


#include "CVSNPlatform.h"
#include "Types3D.h"

class CVSNTracking : public IVSNModule
{
public:
	CVSNTracking();
	virtual ~CVSNTracking();
	
	// 3軸センサーによる回転と並行移動を返す。
	// 回転はquaternionに変換されて返される。センサーによる軸回転の順番が異なる可能性があるため。
	// 平行移動はベクトルで、単位は[m]で返す。
	virtual bool getStatus(C3DQuat& rot, C3DVec& trans) = 0;

	// 3軸センサーの位置/回転等をリセットする。
	// 具体的に何が行われるかはデバイスによる。
	virtual bool reposition() = 0;
};

#endif // CVSNTracking_h
