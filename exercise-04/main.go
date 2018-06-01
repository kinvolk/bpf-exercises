package main

import (
	"fmt"
	"os"
	"os/signal"
	"strconv"
	"unsafe"

	"github.com/iovisor/gobpf/elf"
)

/*
#include <linux/types.h>

struct syscall_event_t {
	__u64 timestamp;
	__u64 cpu;
	__u32 pid;
	__u32 syscall_id;
};

*/
import "C"

type SyscallEvent struct {
	Timestamp uint64
	CPU       uint64
	PID       uint32
	SyscallID uint32
}

func syscallEventToGo(data *[]byte) (event SyscallEvent) {
	eventC := (*C.struct_syscall_event_t)(unsafe.Pointer(&(*data)[0]))

	event.Timestamp = uint64(eventC.timestamp)
	event.CPU = uint64(eventC.cpu)
	event.PID = uint32(eventC.pid)
	event.SyscallID = uint32(eventC.syscall_id)

	return
}

func main() {
	module := elf.NewModule("./program.o")
	err := module.Load(nil)
	// fmt.Printf("%s\n", string(module.Log()))
	if err != nil {
		fmt.Fprintf(os.Stderr, "Failed to load program: %v\n", err)
		os.Exit(1)
	}
	defer func() {
		if err := module.Close(); err != nil {
			fmt.Fprintf(os.Stderr, "Failed to close program: %v\n", err)
		}
	}()

	fmt.Println("Loaded BPF program")

	pidsToWatchMap := module.Map("pids_to_watch")
	if pidsToWatchMap == nil {
		fmt.Fprintf(os.Stderr, "Failed to load 'pids_to_watch' map\n")
		os.Exit(1)
	}

	if err := module.EnableTracepoint("tracepoint/raw_syscalls/sys_enter"); err != nil {
		fmt.Fprintf(os.Stderr, "Failed to enable tracepoint: %v\n", err)
		os.Exit(1)
	}

	fmt.Println("Enabled tracepoints")

	var one uint32 = 1
	for _, pidStr := range os.Args[1:] {
		pid, err := strconv.Atoi(pidStr)
		if err != nil {
			fmt.Fprintf(os.Stderr, "Failed to convert %q to int: %v\n", pidStr, err)
			os.Exit(1)
		}
		if err := module.UpdateElement(pidsToWatchMap, unsafe.Pointer(&pid), unsafe.Pointer(&one), 0); err != nil {
			fmt.Fprintf(os.Stderr, "Failed to add pid %d to watch list: %v\n", pid, err)
			os.Exit(1)
		}
		fmt.Printf("Watching pid %d ...\n", pid)
	}

	eventChan := make(chan []byte)
	lostChan := make(chan uint64)

	perfMap, err := elf.InitPerfMap(module, "syscall_event", eventChan, lostChan)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Failed to initialize 'syscall_event' perf map: %v\n", err)
		os.Exit(1)
	}

	sig := make(chan os.Signal, 1)
	signal.Notify(sig, os.Interrupt, os.Kill)

	fmt.Println("Abort with Ctrl+C")

	perfMap.PollStart()

L:
	for {
		select {
		case <-sig:
			perfMap.PollStop()
			break L
		case data, ok := <-eventChan:
			if !ok {
				continue
			}
			event := syscallEventToGo(&data)
			fmt.Printf("%v cpu#%d pid: %v syscall_id: %v\n", event.Timestamp, event.CPU, event.PID, event.SyscallID)
		case lost, ok := <-lostChan:
			if !ok {
				continue
			}
			fmt.Printf("ERR: lost %d events\n", lost)
		}

	}
}
