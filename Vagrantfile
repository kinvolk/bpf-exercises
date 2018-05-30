# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|
  config.vm.box = "ubuntu/bionic64"
  config.vm.network "private_network", ip: "192.168.167.10"

  config.vm.provision "shell", path: "terraform/user-data.bash"
  config.vm.provision "shell", inline: "usermod -a -G docker vagrant"
end
