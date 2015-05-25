#ifndef CVSNHeapMap_h
#define CVSNHeapMap_h

#include "CVSNEngineHeap.h"

class CVSNHeapMap
{
private:
	CVSNHeapMap();
	virtual ~CVSNHeapMap();
public:
	struct {
		float begin;
		float end;
	};
	static CVSNHeapMap& getInstance();



};

#endif // CVSNHeapMap_h
