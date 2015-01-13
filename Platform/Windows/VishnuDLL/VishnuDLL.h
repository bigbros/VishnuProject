// �ȉ��� ifdef �u���b�N�� DLL ����̃G�N�X�|�[�g��e�Ղɂ���}�N�����쐬���邽�߂� 
// ��ʓI�ȕ��@�ł��B���� DLL ���̂��ׂẴt�@�C���́A�R�}���h ���C���Œ�`���ꂽ VISHNUDLL_EXPORTS
// �V���{�����g�p���ăR���p�C������܂��B���̃V���{���́A���� DLL ���g�p����v���W�F�N�g�ł͒�`�ł��܂���B
// �\�[�X�t�@�C�������̃t�@�C�����܂�ł��鑼�̃v���W�F�N�g�́A 
// VISHNUDLL_API �֐��� DLL ����C���|�[�g���ꂽ�ƌ��Ȃ��̂ɑ΂��A���� DLL �́A���̃}�N���Œ�`���ꂽ
// �V���{�����G�N�X�|�[�g���ꂽ�ƌ��Ȃ��܂��B
#ifdef VISHNUDLL_EXPORTS
#define VISHNUDLL_API __declspec(dllexport)
#else
#define VISHNUDLL_API __declspec(dllimport)
#endif

extern "C" {
	VISHNUDLL_API void * gles2CreateWindow(HWND parent_hWnd);
	VISHNUDLL_API void gles2DestroyWindow(void * ptr);
	VISHNUDLL_API void gles2Resize(void * ptr, int width, int height);
	VISHNUDLL_API void gles2Update(void * ptr);
	VISHNUDLL_API void gles2Flip(void * ptr);


	VISHNUDLL_API void * vsnCreate(const char * path, int width, int height);
	VISHNUDLL_API void vsnDestroy(void * ptr);
	VISHNUDLL_API void vsnUpdate(void * ptr);
	VISHNUDLL_API void vsnOnSizeChanged(void * ptr, int width, int height);
	VISHNUDLL_API void vsnOnPointing(void * ptr, int id, int mode, int x, int y, float force);
	VISHNUDLL_API void vsnOnPause(void * ptr);
	VISHNUDLL_API void vsnOnResume(void * ptr);
	VISHNUDLL_API void vsnOnDetach(void * ptr);
	VISHNUDLL_API void vsnOnSurface(void * ptr);
	VISHNUDLL_API void vsnSetLogFunc(void(*func)(const char *));
}
