FROM ubuntu:18.04

RUN apt-get update && \
  apt-get install -y \
  build-essential \
  clang \
  git \
  linux-headers-generic \
  make \
  vim

RUN mkdir /bpf-exercises

WORKDIR /bpf-exercises
