FROM ubuntu:latest

RUN apt update -y && apt install -y build-essential libosmpbf-dev cmake protobuf-compiler

COPY . /app

WORKDIR /app

RUN ./compile-all.sh

CMD ./getcsvdata isle-of-man-latest.osm.pbf stops.txt