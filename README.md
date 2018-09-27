# Amlogic-Repacker
MS-WINDOWS
BCB6 Project
//workaround MINIX NEO U9-H firmware (based on s912H Amlogic chip)
Ready:
1. Slice img firmware.img into separate files
//recognize uboot v1 or v2 struct's
2. PROPER check CRC32 of firmware.img  
3. Way for unpack dtb blob to dtc device tree structure (i.e. meson1.dtb -> 1.dts, 2.dts...)
