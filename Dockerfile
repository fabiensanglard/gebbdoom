# Use the official Ubuntu base image
FROM ubuntu:latest

# Set the working directory inside the container
WORKDIR /workspace

# Update and install required packages
RUN apt-get update && \
    apt-get install -y \
    inkscape \
    texlive \
    texlive-font-utils \
    texlive-fonts-recommended \
    texlive-latex-extra \
    golang-go && \
    # Clean up the apt cache to reduce image size
    rm -rf /var/lib/apt/lists/*

WORKDIR /book
VOLUME ["/book"]

# Set default command
CMD ["bash"]
