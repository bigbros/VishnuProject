$#include "raspi.h"
#include "CVSNRasPi.h"

extern "C" {

	void * vsnCreate(const char * path, int width, int height, int is_vr) {
		CVSNRasPi * vsn = new CVSNRasPi(path, width, height, is_vr);
		return vsn;
	}

	void vsnDestroy(void * ptr)
	{
		CVSNRasPi * vsn = static_cast<CVSNRasPi *>(ptr);
		delete vsn;
	}

	void vsnUpdate(void * ptr)
	{
		CVSNRasPi * vsn = static_cast<CVSNRasPi *>(ptr);
		vsn->Update();
	}

	void vsnOnSizeChanged(void * ptr, int width, int height)
	{
		CVSNRasPi * vsn = static_cast<CVSNRasPi *>(ptr);
		vsn->OnSizeChanged(width, height);
	}

	void vsnOnPointing(void * ptr, int id, int mode, int x, int y, float force)
	{
		CVSNRasPi * vsn = static_cast<CVSNRasPi *>(ptr);
		vsn->OnPointing(id, mode, x, y, force);
	}

	void vsnOnPause(void * ptr)
	{
		CVSNRasPi * vsn = static_cast<CVSNRasPi *>(ptr);
		vsn->OnPause();
	}

	void vsnOnResume(void * ptr)
	{
		CVSNRasPi * vsn = static_cast<CVSNRasPi *>(ptr);
		vsn->OnResume();
	}

	void vsnOnDetach(void * ptr)
	{
		CVSNRasPi * vsn = static_cast<CVSNRasPi *>(ptr);
		vsn->OnDetach();
	}

	void vsnOnSurface(void * ptr)
	{
		CVSNRasPi * vsn = static_cast<CVSNRasPi *>(ptr);
		vsn->OnSurface();
	}

}
