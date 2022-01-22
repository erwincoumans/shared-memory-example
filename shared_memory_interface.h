#ifndef SHARED_MEMORY_INTERFACE_H
#define SHARED_MEMORY_INTERFACE_H

#define SHARED_MEMORY_VERSION_NUMBER 20220122
#define SHARED_MEMORY_KEY 32768 // 0xfce2
#ifdef __APPLE__
#define SHARED_MEMORY_MAX_STREAM_CHUNK_SIZE (1024 * 1024)
#else
#define SHARED_MEMORY_MAX_STREAM_CHUNK_SIZE (8 * 1024 * 1024)
#endif



namespace visdebug
{

enum EnumSharedMemoryClientCommand
{
	CMD_INVALID = 0,
	CMD_DEBUG_AUDIO,
	CMD_TERMINATE_SERVER,
};

enum EnumSharedMemoryServerStatus
{
	STAT_INVALID = 0,
	STAT_DEBUG_AUDIO_COMPLETED,
	STAT_DEBUG_AUDIO_FAILED,
	STAT_TERMINATE_SERVER_COMPLETED,
	STAT_TERMINATE_SERVER_FAILED,
};

struct DebugAudioArgs
{
	int audio_size;
};

struct SharedMemoryCommand
{
	int type;
	union {
		struct DebugAudioArgs debugArguments;
	};
};

struct SharedMemoryStatus
{
	int status_code;
};

struct SharedMemoryBlock
{
	int version_number_;
	
	int num_client_commands_;
	int num_processed_client_commands_;
	SharedMemoryCommand client_command_;

	int num_server_status_;
	int num_processed_server_status_;
	SharedMemoryStatus server_status_;
		
	
	char data_stream_[SHARED_MEMORY_MAX_STREAM_CHUNK_SIZE];
};


class SharedMemoryInterface
{
public:
	virtual ~SharedMemoryInterface()
	{
	}

	virtual void* allocate(int key, int size, bool allowCreation) = 0;
	virtual void release(int key, int size) = 0;
};

};

#endif
