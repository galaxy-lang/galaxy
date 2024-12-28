FROM debian:latest
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    python3 \
    binutils \
    build-essential \
    cmake \
    make \
    libtinfo-dev \
    zlib1g-dev \
    libomp-dev \
    wget \
    git \
    gh \
    nano \
    vim \
    neovim && \
    apt-get clean

RUN useradd -m meow && \
    echo "meow:galaxy" | chpasswd && \
    adduser meow sudo

ADD https://github.com/llvm/llvm-project/releases/download/llvmorg-19.1.6/llvm-project-19.1.6.src.tar.xz /tmp/llvm-project.tar.xz

RUN mkdir -p /tmp/llvm-project
RUN tar xvf /tmp/llvm-project.tar.xz -C /tmp/llvm-project --strip-components=1
RUN mkdir -p /tmp/llvm-project/build
RUN cd /tmp/llvm-project/build
RUN cmake /tmp/llvm-project/llvm \
        -DCMAKE_BUILD_TYPE=Release \
        -DLLVM_ENABLE_PROJECTS="clang;openmp" \
        -DLLVM_ENABLE_RTTI=ON \
        -DCLANG_INCLUDE_TESTS=OFF
RUN make -j$(nproc)
RUN make install
RUN git clone https://github.com/galaxy-lang/galaxy /home/meow/galaxy
RUN cmake /home/meow/galaxy && \
        cd /home/meow/galaxy && \
        make

WORKDIR /home/meow/galaxy

USER meow

CMD ["/bin/bash"]
