# Amlogic-Repacker
MS-WINDOWS
BCB6 Project
//workaround MINIX NEO U9-H firmware (based on s912H Amlogic chip)

What ready:
1. Slice img firmware.img into separate files
//recognize uboot v1 or v2 struct's
2. PROPER calculate and check CRC32 of firmware.img (use semi-fast silce8 algo for calc crc32) 
3. Way for unpack dtb blob to dtc device tree structure (i.e. meson1.dtb -> 1.dts, 2.dts...)
4. Interface (ini file, etc)
