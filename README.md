mkinitcpio-multiflash
=====================
This is the initramfs script that will be executed on a node to flash the SD card and to rename the node with a prefix and a serial number. 

Required Packages for Building
------------------------------
* base-devel
* rsync
* mkinitcpio
* mkinitcpio-nfs-utils

Building
--------
Run `updpkgsums` to generate the checksum followed by `makepkg -s` to build the package.

Installing
----------
Install the package via `sudo pacman -U mkinitcpio-multiflash*.pkg.tar.xz`

Usage
-----
Run `mkinitcpio -k $(uname -r) -g /boot/initramfs.img` to generate a new initramfs for example after a Linux kernel update.

Append `multilash=<nfs_path>,<server_port>,<timeout_in_seconds>,<hostname_prefix>` to the Linux kernel command line to enable the multiflash script.
It is expected that on the NFS server runs the package `broadcastipd` with given `<nfs_path>` and `<server_port>`. See also `whereisip`.

### whereisip

`whereisip` can be used to discover the IP address of a server.
This is accomplished on the server by the means of sending UDP broadcast packets to the entire network.
Clients that wish to get the IP address of the server simply execute whereisip that listens to the UDP packets.

### Usage

`whereisip <dgram_message> <dgram_port> <dgram_timeout_sec>`

The protocol is quite simple. `whereisip` matches the incoming datagram package against a given port and message.
It returns the IP address if a match has been found or else an error if no match was found or when the program timed out.

On a server the UDP packages can be simulated with `netcat`, for example: `echo "My_msg" | nc.traditional -ub -w 1 255.255.255.255 23456`.

Authors
-------
Daniele Gadler & Julian Sanin
