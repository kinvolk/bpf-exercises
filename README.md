# Linux eBPF exercises

Status: work in progress (commits will be rebased and squashed w/o notice)

## Requirements

A Linux system with a recent kernel (v4.15 or newer) and Docker installed.

Build the used Docker container with:

```
make container
```

## Exercises

Each exercise contains a README file with questions and / or tasks.

If not otherwise noted, an exercise should be build and executable
with the following commands:

```
$ make build-exercise-XX
$ make run-exercise-XX
```

### exercise-01: a first eBPF progam

```
$ make build-exercise-01
$ make run-exercise-01
```

### exercise-02: using ELF files

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

## Building exercises manually

By default, the exercises are build and run within a Docker container to spare
user installing all requirements on the host system.

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
