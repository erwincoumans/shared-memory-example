
#include "win32_shared_memory.h"
#include "command_line_args.h"
#include <iostream>

using namespace visdebug;



int main(int argc, char* argv[])
{
	//CommandLineArgs args(argc, argv);
	//bool isServer = args.CheckCmdLineFlag("server");
	
	Win32SharedMemory shmem;
	//only the server initializes the shared memory, not the client!
	bool allowCreation = false;
	int key = SHARED_MEMORY_KEY;
	int size = sizeof(SharedMemoryBlock);
	SharedMemoryBlock* shared_mem_block = static_cast<SharedMemoryBlock*>(shmem.allocate(key, size, allowCreation));
	if (shared_mem_block)
	{
		if (shared_mem_block->version_number_ == SHARED_MEMORY_VERSION_NUMBER)
		{
			std::cout << "client" << std::endl;
			if (shared_mem_block->num_client_commands_ > shared_mem_block->num_processed_client_commands_)
			{
				std::cout << "server is busy processing outstanding commands" << std::endl;
			} else
			{
				//send a server terminate command
				//shared_mem_block->client_commands_[0].type = CMD_TERMINATE_SERVER;
				//shared_mem_block->num_client_commands_++;
				
				shared_mem_block->client_command_.type = CMD_DEBUG_AUDIO;
				int audio_size = 1024;

				shared_mem_block->client_command_.debugArguments.audio_size = audio_size;

				//send the actual audio data
				for (int i=0;i<audio_size;i++)
				{
					shared_mem_block->data_stream_[i] = 0;
				}
				shared_mem_block->num_client_commands_++;
				
				//wait for completion?
				while (shared_mem_block->num_client_commands_ > shared_mem_block->num_processed_client_commands_)
				{
					//todo: yield/sleep while waiting
				}
				if (shared_mem_block->num_server_status_ > shared_mem_block->num_processed_server_status_)
				{
					switch (shared_mem_block->server_status_.status_code)
					{
						case STAT_DEBUG_AUDIO_COMPLETED:
						{
							std::cout << "STAT_DEBUG_AUDIO_COMPLETED" << std::endl;
							break;
						}
						case STAT_DEBUG_AUDIO_FAILED:
						{
							std::cout << "STAT_DEBUG_AUDIO_FAILED" << std::endl;
							break;
						}

						case STAT_TERMINATE_SERVER_COMPLETED:
						{
							std::cout << "STAT_TERMINATE_SERVER_COMPLETED" << std::endl;
							break;
						};
						case STAT_TERMINATE_SERVER_FAILED:
						{
							std::cout << "STAT_TERMINATE_SERVER_FAILED" << std::endl;
							break;
						}
						default:
						{
						}
					}
					
				}
			}
		} else
		{
			std::cout << "invalid server version, expected " << SHARED_MEMORY_VERSION_NUMBER << " and got " << shared_mem_block->version_number_ << std::endl;
		}
	} else
	{
		std::cout << "Cannot connect to shared memory, is the server running?" << std::endl;
	}
	
	shmem.release(key, size);
}