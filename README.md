# shmem_poc
shared mem interop using boost

## docker build
From solution root run:

1. `docker build -f algo_svc/Dockerfile -t algo-svc .`
2. `docker build -f video_svc/Dockerfile -t vid-svc .`

## run containers
1. `docker run --rm -it --ipc=shareable --name algo1 algo-svc:latest`
2. `docker run --rm -it --ipc=container:algo1 --name vid1 vid-svc:latest`

### current reading:

https://theboostcpplibraries.com/boost.interprocess-synchronization

some source refs:

https://github.com/search?p=2&q=boost+shared+memory&type=Repositories

https://github.com/buduJ/boost-shared-memory-communication
https://github.com/ColinTogashi/BoostSharedMemoryTemplate
https://github.com/twelvebarblues/BoostSharedMemoryWrappers
https://github.com/mincongzhang/ShmStringHashMap



