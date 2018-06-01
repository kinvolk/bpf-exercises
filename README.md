# Linux eBPF exercises

Small exercises to accompany an eBPF workshop.

## Requirements

A Linux system with a recent kernel (v4.15 or newer) and Docker installed.
You can either use the Vagrant box or ask us for a preconfigured VM in
the cloud (for example to avoid a corporate proxy).

Build the used Docker container with:

```
make container
```

## Exercises

Some exercises contain a README file with questions and / or tasks.

If not otherwise noted, an exercise should be build and executable
with the following commands:

```
$ make build-exercise-XX
$ make run-exercise-XX
```

### Exercise 01: a first eBPF progam

```
$ make build-exercise-01
$ make run-exercise-01
```

### Exercise 02: using ELF files

Build the exercise with:

```
$ make build-exercise-02
```

To run the this exercise, the kernel's debugfs needs to be mounted:

```
$ sudo mount -t debugfs debugfs /sys/kernel/debug/
```

In a first shell, run `cat` on the kernel's `trace_pipe`:

```
$ sudo cat /sys/kernel/debug/tracing/trace_pipe
```

In a second shell, run the Go program:

```
$ make run-exercise-02
```

### Exercise 03: using maps

```
$ make build-exercise-03
$ make run-exercise-03
```

### Exercise 04: using a perf map

Build the program:

```
$ make build-exercise-04
```

Run the container:

```
$ make run-exercise-04
```

Run the program manually and pass one or more process ids to watch:

```
$ go run main.go <pid>...
```

For example, open a new shell and use its pid: `echo $$`.

### Exercise 05: using a `sched_cls` program

Build the program:

```
$ make build-exercise-05
```

Run the container:

```
$ make run-exercise-05
```

Load the eBPF program and put it in the background with:

```
$ go run main.go &
```

Add a new tc qdisc to `eth0`:

```
$ tc qdisc add dev eth0 clsact
```

Add the eBPF program as a filter to the qdisc:

```
$ tc filter add dev eth0 ingress bpf object-pinned /sys/fs/bpf/bpf-sched-cls-test/demo
```

On the host, watch the `trace_pipe`:

```
$ cat /sys/kernel/debug/tracing/trace_pipe
```

In the container, produce some network traffic, e.g.

```
$ curl ipschwein.de
```

## Building exercises manually

By default, the exercises are build and run within a Docker container to spare
users installing all requirements on the host system.

To build an individual exercise outside of Docker, use the corresponding
Makefile in the exercise directory. For that, kernel headers, clang-6.0,
golang, etc. is required.

`LINUX_HEADERS` can be used to point to the kernel headers. For example,
on a Debian system this could look like:

```
$ cd exercise-01/
$ make LINUX_HEADERS="/lib/modules/$(uname -r)/build /lib/modules/$(uname -r)/source" build
$ sudo ./main
```

## Terraform plan for playground boxes

`terraform/` contains a Terraform plan to provision playground boxes on Hetzner
Cloud in order to provide attendees with a homogenous environment.

The provider for Hetzner cloud needs to be installed manually for the time
being:

https://github.com/hetznercloud/terraform-provider-hcloud

Create a file `terraform.tvfars` from `terraform.tfvars.example` and change
the example values to your settings.
