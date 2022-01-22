#ifndef WIN32_SHARED_MEMORY_H
#define WIN32_SHARED_MEMORY_H

#include "shared_memory_interface.h"
namespace visdebug
{

class Win32SharedMemory : public SharedMemoryInterface
{
	struct Win32SharedMemoryInteralData* m_internalData;

public:
	Win32SharedMemory();
	virtual ~Win32SharedMemory();

	virtual void* allocate(int key, int size, bool allowCreation);
	virtual void release(int key, int size);
};

};

#endif  //WIN32_SHARED_MEMORY_H
