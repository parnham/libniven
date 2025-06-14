VERSION 0.6

bionic:
	FROM ubuntu:18.04

# focal:
# 	FROM ubuntu:20.04

jammy:
	FROM ubuntu:22.04

noble:
	FROM ubuntu:24.04

# rolling:
# 	FROM debian:testing

image:
	ARG TARGETARCH
	ARG DISTRIBUTION=bionic
	ARG PREMAKE=5.0.0-alpha16

	FROM +$DISTRIBUTION
	ENV DEBIAN_FRONTEND noninteractive
	ENV DEBCONF_NONINTERACTIVE_SEEN true
	WORKDIR /code
	RUN apt-get update && apt-get install -y --no-install-recommends ca-certificates curl build-essential clang fakeroot chrpath dh-exec
	RUN curl -Ls -o premake.deb https://github.com/emergent-design/premake-pkg/releases/download/v$PREMAKE/premake_$PREMAKE-0ubuntu1_$TARGETARCH.deb \
		&& dpkg -i premake.deb
	RUN apt-get install -y --no-install-recommends libmicrohttpd-dev libgcrypt20-dev


deps:
	ARG EMERGENT=0.1.10
	ARG ENTITY=1.2.1

	FROM +image
	RUN curl -Ls -o libemergent-dev.deb https://github.com/emergent-design/libemergent/releases/download/v$EMERGENT/libemergent-dev_${EMERGENT}_all.deb \
		&& curl -Ls -o libentity-dev.deb https://github.com/emergent-design/libentity/releases/download/v$ENTITY/libentity-dev_${ENTITY}_all.deb \
		&& dpkg -i libemergent-dev.deb libentity-dev.deb

build:
	FROM +deps
	COPY --dir include packages src premake5.lua .
	RUN premake5 gmake && make -j$(nproc) libniven

package:
	FROM +build
	ARG DISTRIBUTION=bionic

	RUN cd packages && dpkg-buildpackage -b -uc -us
	# SAVE ARTIFACT libniven-dev_*.deb libniven-dev.deb
	# SAVE ARTIFACT libniven0_*.deb libniven0.deb
	SAVE ARTIFACT libniven*.*deb AS LOCAL build/$DISTRIBUTION/

all:
	BUILD --platform=linux/amd64 --platform=linux/arm64 +package --DISTRIBUTION=bionic
	# BUILD --platform=linux/amd64 --platform=linux/arm64 +package --DISTRIBUTION=focal
	BUILD --platform=linux/amd64 --platform=linux/arm64 +package --DISTRIBUTION=jammy
	BUILD --platform=linux/amd64 --platform=linux/arm64 +package --DISTRIBUTION=noble


check:
	BUILD +build

niven-all:
	BUILD +all
