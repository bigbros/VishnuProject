#include <png.h>
#include <string.h>
#include <memory>

#include "CVSNEngineHeap.h"
#include "CVSNPNGTex.h"
#include "platform.h"

extern "C" {
	static void png_read(png_structp png, png_bytep data, size_t size)
	{
		u8 ** p = (u8 **)png_get_io_ptr(png);
		memcpy(data, *p, size);
		*p += size;
	}
}
CVSNPNGTex::CVSNPNGTex(const char * name, const char * pngName)
	: CGLTex(name)
	, m_pngName(0)
{
	try {
		char * buf = (char *)vsnMemAlloc(strlen(pngName) + 1);
		strcpy(buf, pngName);
		m_pngName = buf;
		if (!loadPng(pngName)) throw std::bad_alloc();
	}
	catch (std::bad_alloc& ex) {
		vsnMemFree((void *)m_pngName);
		throw ex;
	}
}

CVSNPNGTex::~CVSNPNGTex()
{
	vsnMemFree((void *)m_pngName);
}

bool
CVSNPNGTex::recovery()
{
	return loadPng(m_pngName);
}


bool
CVSNPNGTex::loadPng(const char * pngName)
{
	CVSNStorage * storage = CVSNPlatform::getInstance().Module<CVSNStorage>(PF_STORAGE, "DATA");

	size_t size;
	void * data = storage->readData(pngName, &size);
	if (!data) return false;

	void * pngData = data;
	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	if (!png) return false;
	png_infop info = png_create_info_struct(png);
	if (!info) {
		png_destroy_read_struct(&png, 0, 0);
		return false;
	}

	png_set_read_fn(png, 0, png_read);
	png_init_io(png, (png_FILE_p)&pngData);
	png_read_info(png, info);

	png_uint_32 width, height;
	int			depth, type;

	png_get_IHDR(png, info, &width, &height, &depth, &type, 0, 0, 0);
	GLint format;
	int gldepth;
	switch (type) {
	case PNG_COLOR_TYPE_RGB: {
		gldepth = 3;
		format = GL_RGB;
		break;
	}
	case PNG_COLOR_TYPE_PALETTE: {
		png_set_palette_to_rgb(png);
		png_read_update_info(png, info);
		png_get_IHDR(png, info, &width, &height, &depth, &type, 0, 0, 0);
		format = (type == PNG_COLOR_TYPE_RGB) ? GL_RGB : GL_RGBA;
		gldepth = (type == PNG_COLOR_TYPE_RGB) ? 3 : 4;
		break;
	}
	case PNG_COLOR_TYPE_GRAY: {
		png_set_gray_to_rgb(png);
		png_read_update_info(png, info);
		format = GL_RGB;
		gldepth = 3;
		break;
	}
	case PNG_COLOR_TYPE_GRAY_ALPHA: {
		png_set_gray_to_rgb(png);
		png_read_update_info(png, info);
		format = GL_RGBA;
		gldepth = 4;
		break;
	}
	default: {
		gldepth = 4;
		format = GL_RGBA;
		break;
	}
	}

	newImage(width, height, gldepth, format);

	// GLÇÃâÊëúÇÕç∂â∫å¥ì_Ç»ÇÃÇ≈ÅAóÃàÊÇÃç∂â∫Ç…ãlÇﬂÇƒäiî[Ç∑ÇÈÅB
	for (u32 y = 0; y < height; y++) {
		png_read_row(png, m_bitmap + m_stride * (height - 1 - y), 0);
	}
	png_destroy_read_struct(&png, &info, 0);
	storage->closeData(data);

	setBuffer();


	return true;
}