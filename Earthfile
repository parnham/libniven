VERSION 0.8

# jammy:
# 	FROM ubuntu:22.04

noble:
	FROM ubuntu:24.04

trixie:
	FROM debian:trixie


image:
	ARG TARGETARCH
	ARG DISTRIBUTION=noble
	# ARG PREMAKE=5.0.0-alpha16

	FROM +$DISTRIBUTION
	ENV DEBIAN_FRONTEND noninteractive
	ENV DEBCONF_NONINTERACTIVE_SEEN true
	WORKDIR /code

	RUN apt-get -q update && apt-get install -y --no-install-recommends ca-certificates curl build-essential cmake clang fakeroot chrpath dh-exec

	RUN curl -so /usr/share/keyrings/emergent.gpg https://apt.emergent-design.co.uk/emergent.gpg \
		&& echo "deb [signed-by=/usr/share/keyrings/emergent.gpg] https://apt.emergent-design.co.uk/public $DISTRIBUTION main" > /etc/apt/sources.list.d/emergent.list

	RUN apt-get -q update && apt-get install -y --no-install-recommends libmicrohttpd-dev libgcrypt20-dev libemergent-dev libentity-dev


build:
	FROM +image
	COPY --dir include packages src CMakeLists.txt .
	# RUN premake5 gmake && make -j$(nproc) libniven
	RUN cmake -B build \
		&& make -j8 -C build

package:
	FROM +build
	ARG DISTRIBUTION=noble

	RUN cd packages && dpkg-buildpackage -b -uc -us
	SAVE ARTIFACT --keep-ts libniven*.*deb AS LOCAL build/$DISTRIBUTION/

all-dists:
	BUILD +package --DISTRIBUTION=noble --DISTRIBUTION=trixie

# to be deprecated in favour of using arm64 and amd64 specific runners
all:
	# BUILD --platform=linux/amd64 --platform=linux/arm64 +package --DISTRIBUTION=bionic
	# BUILD --platform=linux/amd64 --platform=linux/arm64 +package --DISTRIBUTION=focal
	# BUILD --platform=linux/amd64 --platform=linux/arm64 +package --DISTRIBUTION=jammy
	BUILD --platform=linux/amd64 --platform=linux/arm64 +package --DISTRIBUTION=noble
	BUILD --platform=linux/amd64 --platform=linux/arm64 +package --DISTRIBUTION=trixie


check:
	BUILD +build

niven-all:
	BUILD +all
