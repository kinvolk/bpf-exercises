package main

import (
	"fmt"
	"os"
	"os/signal"
	"syscall"

	"github.com/iovisor/gobpf/elf"
)

func main() {
	module := elf.NewModule("./program.o")
	if err := module.Load(nil); err != nil {
		fmt.Fprintf(os.Stderr, "Failed to load program: %v\n", err)
		os.Exit(1)
	}
	defer func() {
		if err := module.Close(); err != nil {
			fmt.Fprintf(os.Stderr, "Failed to close program: %v", err)
		}
	}()

	program := module.SchedProgram("sched_cls/demo")
	if program == nil {
		fmt.Fprintf(os.Stderr, "sched_cls/demo program not found\n")
		os.Exit(1)
	}

	pinPath := "/sys/fs/bpf/bpf-sched-cls-test/demo"
	if err := elf.PinObject(program.Fd(), pinPath); err != nil {
		fmt.Fprintf(os.Stderr, "Failed to pin program to %q: %v\n", pinPath, err)
		os.Exit(1)
	}
	defer func() {
		if err := syscall.Unlink(pinPath); err != nil {
			fmt.Fprintf(os.Stderr, "Failed to unpin program from %q: %v\n", pinPath, err)
		}
	}()

	sig := make(chan os.Signal, 1)
	signal.Notify(sig, os.Interrupt, os.Kill)

	<-sig
}
