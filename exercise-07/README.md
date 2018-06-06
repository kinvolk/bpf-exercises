For this exercise we use [bcc](https://github.com/iovisor/bcc), the BPF
compiler collection.

Install bcc on your VM with the following script:

```
echo "deb [trusted=yes] https://repo.iovisor.org/apt/artful artful-nightly main" | sudo tee /etc/apt/sources.list.d/iovisor.list
sudo apt-get update
sudo apt-get install -y bcc-tools libbcc-examples linux-headers-$(uname -r) python-pyroute2
```

Make a copy of the `xdp_drop_count.py` example:

```
cp /usr/share/bcc/examples/networking/xdp/xdp_drop_count.py .
```

Test the tool and take a look at the code to get an idea how it works.

```
sudo ./xdp_drop_count.py lo
```

Try to update the program to `XDP_PASS` packets by default and only
drop ICMP packets.
