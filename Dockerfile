# podman build -t choas/gebbdoom .
# podman run --volume $PWD:/build choas/gebbdoom

FROM golang:1.22

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y \
    inkscape texlive texlive-font-utils texlive-fonts-recommended texlive-latex-extra golang-go && rm -rf /var/lib/apt/lists/*

RUN mkdir /build
VOLUME [ "/build" ]
WORKDIR /build

CMD ./make.sh
