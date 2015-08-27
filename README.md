mkinitcpio-multiflash
=====================
This is the initramfs script that will be executed on a node to flash the SD card and to rename the node with a prefix and a serial number. 

Required Packages for Building
------------------------------
* base-devel
* mkinitcpio
* mkinitcpio-nfs-utils

Building
--------
Make sure that you have installed the `base-devel` package and run `makepkg -s` to build the package.

Installing
----------
Install the package via `sudo pacman -U mkinitcpio-multiflash*.pkg.tar.xz`

Usage
-----
Run `mkinitcpio -k $(ls /lib/modules | grep ARCH) -g /boot/initramfs.img` to generate a new initramfs for example after a Linux kernel update.

Append `multiflash=<nfs_file_path>,<server_port>,<timeout_in_seconds>,<hostname_prefix>` to the Linux kernel command line to enable the multiflash script.
It is expected that on the NFS server runs the package [broadcastipd](https://github.com/unibz-bobo/broadcastipd) with given `<nfs_file_path>` and `<server_port>`. See also `whereisip`.

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
