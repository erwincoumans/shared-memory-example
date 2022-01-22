
#include "win32_shared_memory.h"
#include "command_line_args.h"
#include <iostream>

using namespace visdebug;


bool keep_running = true;

#ifdef _WIN32
#include <Windows.h>

BOOL WINAPI consoleHandler(DWORD signal) {

    if (signal == CTRL_C_EVENT)
	{
		keep_running = false;
        printf("Ctrl-C handled\n"); // do cleanup
	}

    return TRUE;
}
#endif


int main(int argc, char* argv[])
{
	 if (!SetConsoleCtrlHandler(consoleHandler, TRUE)) {
        printf("\nERROR: Could not set control handler"); 
        return 1;
    }

	CommandLineArgs args(argc, argv);
	
	Win32SharedMemory shmem;
	//only the server initializes the shared memory!
	bool allowCreation = true;
	int key = SHARED_MEMORY_KEY;
	int size = sizeof(SharedMemoryBlock);
	SharedMemoryBlock* shared_mem_block = static_cast<SharedMemoryBlock*>(shmem.allocate(key, size, allowCreation));
	if (shared_mem_block)
	{
		std::cout << "server" << std::endl;
		//make sure there isn't already a shared memory server running
		if (shared_mem_block->version_number_ == SHARED_MEMORY_VERSION_NUMBER)
		{
			std::cout << "shared memory server is already running, terminating" << std::endl;
		} else
		{
			shared_mem_block->num_client_commands_ = 0;
			shared_mem_block->num_processed_client_commands_ = 0;
			shared_mem_block->num_server_status_ = 0;
			shared_mem_block->num_processed_server_status_ = 0;
			shared_mem_block->version_number_ = SHARED_MEMORY_VERSION_NUMBER;
			
			while (keep_running)
			{
				//check if there is any unprocessed commands
				if (shared_mem_block->num_client_commands_ > shared_mem_block->num_processed_client_commands_)
				{
					//process command
					const SharedMemoryCommand& client_cmd = shared_mem_block->client_command_;
					switch (client_cmd.type)
					{
						case CMD_DEBUG_AUDIO:
						{
							int audio_size = client_cmd.debugArguments.audio_size;

							std::cout << "received audio data: " << audio_size << " bytes" << std::endl;

							shared_mem_block->server_status_.status_code = STAT_DEBUG_AUDIO_COMPLETED;
							shared_mem_block->num_server_status_++;
							break;
						}
						case CMD_TERMINATE_SERVER:
						{
							std::cout << "terminate server command received" << std::endl;
							shared_mem_block->server_status_.status_code = STAT_TERMINATE_SERVER_COMPLETED;
							shared_mem_block->num_server_status_++;
							keep_running = false;
							break;
						};
						default:
						{
							std::cout << "Unknown command" << std::endl;
						}
					};

					shared_mem_block->num_processed_client_commands_++;
				}
			}
		}

	} else
	{
		std::cout << "Cannot create shared memory" << std::endl;
	}
	
	shmem.release(key, size);
}