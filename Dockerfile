# 使用官方的Ubuntu镜像作为基础镜像
FROM ubuntu:20.04

# 设置维护者信息
LABEL maintainer="me@w0x7ce.eu"

# 设置非交互模式，避免在构建过程中出现交互提示
ENV DEBIAN_FRONTEND=noninteractive

# 更新软件源并安装必要的依赖
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        git \
        cmake \
        gcc-arm-none-eabi \
        libnewlib-arm-none-eabi \
        build-essential \
        libstdc++-arm-none-eabi-newlib \
        python3 \
        python3-pip \
        python3-venv \
        python3-dev \
        openocd \
        wget \
        unzip && \
    rm -rf /var/lib/apt/lists/*

# 安装RP2040 SDK
RUN mkdir -p /rp2040 && \
    cd /rp2040 && \
    git clone -b master https://github.com/raspberrypi/pico-sdk.git && \
    cd pico-sdk && \
    git submodule update --init

# 设置环境变量
ENV PICO_SDK_PATH=/rp2040/pico-sdk

# 安装RP2040工具链
RUN cd /rp2040 && \
    git clone -b master https://github.com/raspberrypi/pico-examples.git

# 设置工作目录
WORKDIR /rp2040/pico-examples

# 编译示例代码
RUN mkdir build && \
    cd build && \
    cmake .. && \
    make

CMD ["/bin/bash"]
