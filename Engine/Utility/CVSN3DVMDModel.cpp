#include <memory>
#include "CVSNEngineHeap.h"
#include "CVSN3DVMDModel.h"
#include "CVSNPNGTex.h"
#include "CVSNUtil.h"
#include "platform.h"

CVSN3DVMDModel::CVSN3DVMDModel(C3DDefaultShader * shader, const char * modelName, const char * path)
	: C3DModel(shader, modelName)
	, m_pathName(0)
{

	try {
		char * buf = (char *)vsnMemAlloc(strlen(path) + 1);
		strcpy(buf, path);
		m_pathName = buf;
		if (!loadModel(path)) throw std::bad_alloc();
	}
	catch (std::bad_alloc& ex) {
		vsnMemFree((void *)m_pathName);
		throw ex;
	}
}

CVSN3DVMDModel::~CVSN3DVMDModel()
{
	vsnMemFree((void *)m_pathName);
}

bool
CVSN3DVMDModel::loadModel(const char * path)
{
	CVSNStorage * storage = CVSNPlatform::getInstance().Module<CVSNStorage>(PF_STORAGE, "DATA");

	size_t size;
	void * data = storage->readData(path, &size);
	if (!data) return false;

	u8 * ptr = (u8 *)data;

	// ID check
	bool result = true;
	result = result && (memcmp(ptr, "VMDL", 4) == 0);
	if (result) {
		u32 offset_bones = readU32(data, 4);	// bones section
		u32 offset_vert = readU32(data, 8);		// vertices section
		u32 offset_idx = readU32(data, 12);		// indices section
		u32 offset_mat = readU32(data, 16);		// material section

		result = result && loadVertices(data, offset_vert);
		result = result && loadIndices(data, offset_idx);
		result = result && loadBones(data, offset_bones);
		result = result && loadMaterials(data, offset_mat);
	}
	if (result) setBuffer();
	storage->closeData(data);

	return result;
}

bool
CVSN3DVMDModel::loadVertices(void * data, u32 offset)
{
	u32 num_vert = readU32(data, offset);

	newVertices((int)num_vert);
	memcpy(m_vertices, (u8 *)data + offset + 4, sizeof(VERTEX) * num_vert);
	return true;
}

bool
CVSN3DVMDModel::loadIndices(void * data, u32 offset)
{
	u32 num_idx = readU32(data, offset);
	newIndices((int)num_idx);
	memcpy(m_indices, (u8 *)data + offset + 4, sizeof(INDEXTYPE) * num_idx);
	return true;
}

bool
CVSN3DVMDModel::loadBones(void * data, u32 offset)
{
	u32 num_bones = readU32(data, offset);
	newBones((int)num_bones);
	memcpy(m_bones, (u8 *)data + offset + 4, sizeof(C3DVec) * num_bones);
	memcpy(m_boneparent, (u8 *)data + offset + 4 + sizeof(C3DVec) * num_bones, num_bones);
	return true;
}

bool
CVSN3DVMDModel::loadMaterials(void * data, u32 offset)
{
	int numMaterial = readU32(data, offset);	// É}ÉeÉäÉAÉãêî
	u32 matoff = offset + sizeof(u32);
	offset += 4;
	const char * basePath = CVSNUtil::pathDir(m_pathName);

	for (int i = 0; i < numMaterial; i++) {
		C3DMaterial::MATERIAL tmp;
		memcpy(&tmp, (u8 *)data + matoff, sizeof(C3DMaterial::MATERIAL));
		C3DMaterial * material = new C3DMaterial();
		material->setMaterialParams(&tmp);
		matoff += sizeof(C3DMaterial::MATERIAL);
		const char * texture = (const char *)data + matoff;		matoff += strlen(texture) + 1;
		const char * normal = (const char *)data + matoff;		matoff += strlen(normal) + 1;
		const char * specular = (const char *)data + matoff;	matoff += strlen(specular) + 1;

		if (*texture) {
			const char * fname = CVSNUtil::jointPath(basePath, texture);
			CVSNPNGTex * pTex = new CVSNPNGTex("tex", fname);
			CVSNUtil::freePath(fname);
			material->setTexture(pTex);
		}
		if (*normal) {
			const char * fname = CVSNUtil::jointPath(basePath, normal);
			CVSNPNGTex * pTex = new CVSNPNGTex("tex", fname);
			CVSNUtil::freePath(fname);
			material->setNormal(pTex);
		}
		if (*specular) {
			const char * fname = CVSNUtil::jointPath(basePath, specular);
			CVSNPNGTex * pTex = new CVSNPNGTex("tex", fname);
			CVSNUtil::freePath(fname);
			material->setSpecular(pTex);
		}
		setMaterial(material);
	}
	CVSNUtil::freePath(basePath);
	return true;
}

u32
CVSN3DVMDModel::readU32(void * data, u32 offset)
{
	u32 tmp = 0;
	for (int i = 0; i < 4; i++) {
		tmp |= (int)*((u8 *)data + offset + i) << (i * 8);
	}
	return tmp;
}

float
CVSN3DVMDModel::readFloat(void * data, u32 offset)
{
	float tmp = 0.0f;
	u8 * dst = (u8 *)&tmp;
	for (int i = 0; i < sizeof(float); i++) {
		dst[i] = *((u8 *)data + offset + i);
	}
	return tmp;
}
