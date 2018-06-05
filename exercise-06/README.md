Implement a clsact qdisc filter that can be attached to an interface
to drop all traffic. For testing, we use the loopback `lo` device.

To attach a filter, we have to first create a clsact qdisc:

```
tc qdisc add dev lo clsact
```

Then we can attach the filter:

```
tc filter add dev lo ingress prio 1 handle 1 bpf direct-action obj <PATH/TO/program.o> sec <SEC_NAME>
```

To remove the qdisc again, you can do:

```
tc qdisc del dev lo clsact
```

Hints:

* You can start with program.c from exercise 05.
* You have to find out how to signal the kernel to drop a packet from
  a clsact program.

Note: do not attach the filter to the default interface / route when
you rely on a SSH connection for access :)
