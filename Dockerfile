FROM ubuntu:latest

RUN apt-get update -y && apt-get install -y \
    build-essential \
    libosmpbf-dev \
    cmake \
    vim \
    tmux \
    protobuf-compiler

COPY . /app

WORKDIR /app

RUN ./compile-all.sh

CMD ./getcsvdata isle-of-man-latest.osm.pbf stops.txt