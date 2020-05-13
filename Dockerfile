# docker build -t gebbdoom .
# docker run --volume $PWD:/build gebbdoom

FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y \
    inkscape texlive texlive-font-utils texlive-latex-extra \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir /build
VOLUME [ "/build" ]
WORKDIR /build

CMD sed -i -e 's/\\usetkzobj/% \\usetkzobj/g' src/mystyle.sty; \
    ./make.sh
