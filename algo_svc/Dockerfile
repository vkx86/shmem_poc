FROM ubuntu:20.04

#RUN apt-get update

# webrtc-streamer deps
#RUN DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends libssl-dev libasound2 libgtk-3-0 libxtst6 libpulse0 avahi-utils

RUN apt-get clean && rm -rf /var/lib/apt/lists/

#copy from CMake build
WORKDIR /app
COPY /cmake-build-debug/algo_svc/algo_svc .

ENTRYPOINT [ "./algo_svc" ]
#CMD [ "-a" ]