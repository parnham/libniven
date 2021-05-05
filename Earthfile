bionic:
	FROM ubuntu:18.04
	# configure apt to be noninteractive
	ENV DEBIAN_FRONTEND noninteractive
	ENV DEBCONF_NONINTERACTIVE_SEEN true
	WORKDIR /code

focal:
	FROM ubuntu:20.04
	# configure apt to be noninteractive
	ENV DEBIAN_FRONTEND noninteractive
	ENV DEBCONF_NONINTERACTIVE_SEEN true
	WORKDIR /code

image:
	ARG DISTRIBUTION=bionic
	FROM +$DISTRIBUTION
	RUN apt-get update && apt-get install -y --no-install-recommends ca-certificates curl build-essential clang fakeroot chrpath
	RUN curl -L https://github.com/premake/premake-core/releases/download/v5.0.0-alpha15/premake-5.0.0-alpha15-linux.tar.gz | tar -xz -C /usr/bin/
	RUN apt-get install -y --no-install-recommends libmicrohttpd-dev

deps:
	FROM +image
	ARG GITHUB_EMERGENT=github.com/emergent-design
	COPY $GITHUB_EMERGENT/libentity:v1.1.8+package/libentity-dev.deb .
	COPY $GITHUB_EMERGENT/libemergent:v0.0.31+package/libemergent-dev.deb .
	RUN dpkg -i libentity-dev.deb libemergent-dev.deb

build:
	FROM +deps
	COPY --dir include packages src premake5.lua strip .
	RUN premake5 gmake && make -j$(nproc) libniven

package:
	FROM +build
	ARG DISTRIBUTION=bionic
	RUN cd packages && ./build $DISTRIBUTION
	SAVE ARTIFACT packages/libniven-dev_*.deb libniven-dev.deb
	SAVE ARTIFACT packages/libniven0_*.deb libniven0.deb
	SAVE ARTIFACT packages/libniven*.deb AS LOCAL build/

all:
	BUILD --build-arg DISTRIBUTION=bionic +package
	BUILD --build-arg DISTRIBUTION=focal +package
