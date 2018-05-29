Note how we set `SEC("version")`:

```
__u32 _version SEC("version") = 0xFFFFFFFE;
```

This is a gobpf specifc magic constant: if set, gobpf's elf loader will
determine the current kernel version and set it for you when loading the
kprobe.
For kprobes the version code must match, otherwise the program will be
rejected. The [version check is not used for safety, but for enforcing
'non-ABI-ness'](https://lwn.net/Articles/636976/).
