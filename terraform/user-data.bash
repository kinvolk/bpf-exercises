#!/bin/bash

set -euo pipefail
set -x

id -u ubuntu &>/dev/null || adduser --disabled-password ubuntu
usermod -a -G sudo ubuntu
echo "%sudo ALL=(ALL) NOPASSWD:ALL" >>/etc/sudoers.d/10-sudo-group-nopasswd

apt-get update
apt-get upgrade -y

apt-get install -y \
  apt-transport-https \
  ca-certificates \
  curl \
  git \
  make \
  python3-venv \
  screen \
  software-properties-common \
  tmux

curl -fsSL https://download.docker.com/linux/ubuntu/gpg | apt-key add -

add-apt-repository \
  "deb [arch=amd64] https://download.docker.com/linux/ubuntu \
  $(lsb_release -cs) \
  edge"

apt-get update

apt-get install -y docker-ce

usermod -a -G docker ubuntu
