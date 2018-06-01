FROM ubuntu:18.04

RUN apt-get update && \
  apt-get install -y \
  build-essential \
  clang \
  curl \
  git \
  iproute2 \
  iputils-ping \
  linux-headers-generic \
  llvm \
  make \
  netcat-openbsd \
  vim

RUN cd /tmp && \
  curl -fsSL https://dl.google.com/go/go1.10.2.linux-amd64.tar.gz -o go.tar.gz && \
  tar -C /usr/local -xzvf go.tar.gz

ENV PATH $PATH:/usr/local/go/bin

RUN mkdir /go
ENV GOPATH /go

RUN go get github.com/iovisor/gobpf/elf

RUN mkdir /bpf-exercises
WORKDIR /bpf-exercises
