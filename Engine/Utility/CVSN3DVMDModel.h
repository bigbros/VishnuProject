#ifndef CVSN3DVMDModel_h
#define CVSN3DVMDModel_h

#include "C3DModel.h"

class CVSN3DVMDModel : public C3DModel
{
public:
	CVSN3DVMDModel(C3DDefaultShader * shader, const char * modelName, const char * model);
	virtual ~CVSN3DVMDModel();

	bool loadModel(const char * path);

	bool loadVertices(void * data, u32 offset);
	bool loadIndices(void * data, u32 offset);


	bool loadBones(void * data, u32 offset);
	bool loadMaterials(void * data, u32 offset);
private:
	u32 readU32(void * data, u32 offset);
	float readFloat(void * data, u32 offset);

	const char		*	m_pathName;
};

#endif // CVSN3DVMDModel_h
