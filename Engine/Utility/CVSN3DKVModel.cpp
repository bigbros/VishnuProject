#include <memory>
#include "CVSN3DKVModel.h"
#include "CVSNPNGTex.h"
#include "platform.h"

CVSN3DKVModel::CVSN3DKVModel(C3DDefaultShader * shader, const char * modelName, CVSNKVObj * kvObj)
	: C3DModel(shader, modelName)
{
	if (!loadModel(*kvObj)) throw std::bad_alloc();
}

CVSN3DKVModel::~CVSN3DKVModel() {}

bool
CVSN3DKVModel::loadModel(CVSNKVObj& kvObj)
{
	newVertices(kvObj["mesh"]["vertices"].length());
	newIndices(kvObj["mesh"]["indices"].length());
	newBones(kvObj["bones"].length());

	loadVertices(kvObj["mesh"]["vertices"]);
	loadIndices(kvObj["mesh"]["indices"]);
	loadBones(kvObj["bones"]);

	setBuffer();

	if (kvObj.ContainsKey("material")) {
		CVSNKVObj& kvMat = kvObj["material"];
		for (int i = 0; i < kvMat.length(); i++) {
			//C3DMaterial::MATERIAL tmp;
			C3DMaterial * material = new C3DMaterial();

			if (kvMat.ContainsKey("texture")) {
				LOG("READ TEXTURE\n");
				const char * name = kvMat["texture"];
				CVSNPNGTex * pTex = new CVSNPNGTex("tex", name);
				material->setTexture(pTex);
			}
			if (kvMat.ContainsKey("normal")) {
				LOG("READ NORMAL\n");
				const char * name = kvMat["normal"];
				CVSNPNGTex * pTex = new CVSNPNGTex("norm", name);
				material->setNormal(pTex);
			}
			if (kvMat.ContainsKey("specular")) {
				LOG("READ SPECULAR\n");
				const char * name = kvMat["specular"];
				CVSNPNGTex * pTex = new CVSNPNGTex("spcl", name);
				material->setSpecular(pTex);
			}
			/*
			if (kvMat.ContainsKey("shiness")) {
				LOG("SET SPECULAR\n");
				const float shiness = kvMat["shiness"];
				material->setShininess(shiness);
			}
			*/
			setMaterial(material);
		}
	}
	return true;
}

bool
CVSN3DKVModel::loadVertices(CVSNKVObj& vertices)
{
	for (int i = 0; i < vertices.length(); i++) {
		CVSNKVObj& vert = vertices[i];

		m_vertices[i].vert.x = vert[0];
		m_vertices[i].vert.y = vert[1];
		m_vertices[i].vert.z = vert[2];

		m_vertices[i].norm.x = vert[3];
		m_vertices[i].norm.y = vert[4];
		m_vertices[i].norm.z = vert[5];

		m_vertices[i].tang.x = vert[6];
		m_vertices[i].tang.y = vert[7];
		m_vertices[i].tang.z = vert[8];

		m_vertices[i].uv.u = vert[9];
		m_vertices[i].uv.v = vert[10];

		m_vertices[i].rgba[0] = (int)vert[11];
		m_vertices[i].rgba[1] = (int)vert[12];
		m_vertices[i].rgba[2] = (int)vert[13];
		m_vertices[i].rgba[3] = (int)vert[14];

		m_vertices[i].bone[0] = (int)vert[15];
		m_vertices[i].bone[1] = (int)vert[16];
		m_vertices[i].bone[2] = (int)vert[17];
		m_vertices[i].bone[3] = (int)vert[18];

		m_vertices[i].wght[0] = (int)vert[19];
		m_vertices[i].wght[1] = (int)vert[20];
		m_vertices[i].wght[2] = (int)vert[21];
		m_vertices[i].wght[3] = (int)vert[22];
	}
	return true;
}

bool
CVSN3DKVModel::loadIndices(CVSNKVObj& indices)
{
	for (int i = 0; i < indices.length(); i++) {
		m_indices[i] = (int)indices[i];
	}
	return true;
}

bool
CVSN3DKVModel::loadBones(CVSNKVObj& bones)
{
	for (int i = 0; i < bones.length(); i++) {
		CVSNKVObj& item = bones[i];
		CVSNKVObj& pos = item["pos"];
		m_bones[i].x = pos["x"];
		m_bones[i].y = pos["y"];
		m_bones[i].z = pos["z"];
		m_boneparent[i] = (item.ContainsKey("parent")) ? (int)item["parent"] : 0;
	}
	return true;
}
