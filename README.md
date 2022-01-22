# shared-memory-example
Using shared memory to communicate between two executables or processes, for Windows, Linux and MacOS (posix). Can also be useful for remote visualization/debugging.

## build
```
mkdir build
cd build
cmake ..
```

## run
Run the shared_memory_server in one terminal window. Then run the shared_memory_client in another window. 
If all goes well, the server will receive a command and print a confirmation.
