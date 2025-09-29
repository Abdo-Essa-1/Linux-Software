# FILE SYSTEMS
Now we have a piece of metal called hard disk. let's make it useful here :)

# fdiskl.c
fdiskl.c is a C program that reads and displays the partition table of a disk device by directly accessing its Master Boot Record (MBR) and, if present, Extended Boot Records (EBRs). It prints information about all primary and logical partitions, including their type, size, and boot status. This tool is useful for understanding how disk partitioning works at a low level in Linux systems.