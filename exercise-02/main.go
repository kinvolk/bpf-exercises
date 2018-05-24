package main

import (
	"fmt"
	"os"
	"time"

	"github.com/iovisor/gobpf/elf"
)

func main() {
	module := elf.NewModule("./program.o")
	err := module.Load(nil)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Failed to load program from elf: %v\n", err)
		os.Exit(1)
	}
	defer func() {
		if err := module.Close(); err != nil {
			fmt.Fprintf(os.Stderr, "Failed to close module: %v\n", err)
		}
	}()
	err = module.EnableTracepoint("tracepoint/raw_syscalls/sys_enter")
	if err != nil {
		fmt.Fprintf(os.Stderr, "Failed to enable tracepoint: %v\n", err)
		os.Exit(1)
	}
	time.Sleep(10 * time.Second)
}
