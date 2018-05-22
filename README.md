# Linux eBPF exercises

Status: work in progress (commits will be rebased and squashed w/o notice)

## Requirements

A Linux system with a recent kernel (v4.15 or newer) and Docker installed.

## Exercises

Each exercise contains a README file with questions and / or tasks.

All exercises should be buildable from the root directory with

```
$ make exercise-XX
```

### exercise-01: a first eBPF progam

```
$ make exercise-01
$ sudo ./exercise-01/run
```

## Building exercises manually


By default, the exercises are build within a Docker container to spare
user installing all requirements on the host system.

To build an individual exercise outside of Docker, use the corresponding
Makefile in the exercise directory. For that, kernel headers, clang-6.0,
golang, etc. is required.

`LINUX_HEADERS` can be used to point to the kernel headers. For example,
on a Debian system this could look like:

```
$ cd exercise-01/
$ make LINUX_HEADERS="/lib/modules/$(uname -r)/build /lib/modules/$(uname -r)/source"
$ sudo ./run
```

## Terraform plan for playground boxes

`terraform/` contains a Terraform plan to provision playground boxes on Hetzner
Cloud in order to provide attendees with a homogenous environment.

The provider for Hetzner cloud needs to be installed manually for the time
being:

https://github.com/hetznercloud/terraform-provider-hcloud

Create a file `terraform.tvfars` from `terraform.tfvars.example` and change
the example values to your settings.
