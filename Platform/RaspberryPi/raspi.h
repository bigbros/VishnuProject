#ifndef raspi_h
#define raspi_h

#ifdef __cplusplus
extern "C" {
#endif

	void * vsnCreate(const char * path, int width, int height, int is_vr);
	void vsnDestroy(void * ptr);
	void vsnUpdate(void * ptr);
	void vsnOnSizeChanged(void * ptr, int width, int height);
	void vsnOnPointing(void * ptr, int id, int mode, int x, int y, float force);
	void vsnOnPause(void * ptr);
	void vsnOnResume(void * ptr);
	void vsnOnDetach(void * ptr);
	void vsnOnSurface(void * ptr);

#ifdef __cplusplus
}
#endif

#endif // raspi_h
