VERSION 0.6

bionic:
	FROM ubuntu:18.04
	ENV DEBIAN_FRONTEND noninteractive
	ENV DEBCONF_NONINTERACTIVE_SEEN true
	WORKDIR /code

focal:
	FROM ubuntu:20.04
	ENV DEBIAN_FRONTEND noninteractive
	ENV DEBCONF_NONINTERACTIVE_SEEN true
	WORKDIR /code

image:
	ARG TARGETARCH
	ARG DISTRIBUTION=bionic
	ARG PREMAKE=5.0.0-alpha16

	FROM +$DISTRIBUTION
	RUN apt-get update && apt-get install -y --no-install-recommends ca-certificates curl build-essential clang fakeroot chrpath dh-exec
	RUN curl -L -o premake.deb https://github.com/emergent-design/premake-pkg/releases/download/v$PREMAKE/premake_$PREMAKE-0ubuntu1_$TARGETARCH.deb \
		&& dpkg -i premake.deb
	RUN apt-get install -y --no-install-recommends libmicrohttpd-dev


deps:
	FROM +image
	ARG GITHUB_EMERGENT=github.com/emergent-design
	COPY --platform=linux/amd64 $GITHUB_EMERGENT/libentity:v1.1.8+package/libentity-dev.deb .
	COPY --platform=linux/amd64 $GITHUB_EMERGENT/libemergent:v0.0.37+package/libemergent-dev.deb .
	RUN dpkg -i libentity-dev.deb libemergent-dev.deb

build:
	FROM +deps
	COPY --dir include packages src premake5.lua strip .
	RUN premake5 gmake && make -j$(nproc) libniven

package:
	FROM +build
	ARG DISTRIBUTION=bionic

	# RUN cd packages && ./build ${DISTRIBUTION}
	RUN cd packages && dpkg-buildpackage -b -uc -us
	SAVE ARTIFACT packages/libniven-dev_*.deb libniven-dev.deb
	SAVE ARTIFACT packages/libniven0_*.deb libniven0.deb
	SAVE ARTIFACT libniven*.deb AS LOCAL build/$DISTRIBUTION/

all:
	BUILD --platform=linux/amd64 --platform=linux/arm64 +package --DISTRIBUTION=bionic
	BUILD --platform=linux/amd64 --platform=linux/arm64 +package --DISTRIBUTION=focal
