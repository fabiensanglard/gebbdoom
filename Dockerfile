# docker build -t choas/gebb .
# docker run --volume $PWD:/build choas/gebb

FROM ubuntu:18.04

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y \
    inkscape texlive texlive-font-utils texlive-latex-extra \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir /build
VOLUME [ "/build" ]
WORKDIR /build

CMD ./make.sh
