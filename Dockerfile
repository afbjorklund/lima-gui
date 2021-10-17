# docker build -t qt5-build .

FROM docker.io/ubuntu:20.04

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y qt5-default qt5-qmake g++ make && rm -rf /var/lib/apt/lists/*

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y pkg-config libqtermwidget5-0-dev libutf8proc-dev && rm -rf /var/lib/apt/lists/*

# docker run -v $PWD:/build qt5-build

VOLUME /build

WORKDIR /build

CMD qmake && make
