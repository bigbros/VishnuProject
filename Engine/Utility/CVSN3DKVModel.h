#ifndef CVSN3DKVModel_h
#define CVSN3DKVModel_h

#include "CVSNKVObj.h"
#include "C3DModel.h"

class CVSN3DKVModel : public C3DModel
{
public:
	CVSN3DKVModel(const char * modelName, CVSNKVObj * kvObj);
	virtual ~CVSN3DKVModel();

	bool loadModel(CVSNKVObj& kvObj);

	bool loadVertices(CVSNKVObj& vertices);
	bool loadIndices(CVSNKVObj& indices);


	bool loadBones(CVSNKVObj& bones);
};

#endif // CVSN3DKVModel_h
