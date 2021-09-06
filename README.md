![Forwarder Logo](/oscforward.png)

The forwarder channel for the Open Shop Channel.

---

_This forwarder is based on code provided by GBAtemp user [FIX94](https://gbatemp.net/members/fix94.232444/). FIX94 posted [a thread](https://gbatemp.net/threads/forwarder-with-meta-xml-support.273165/) in which they provided the source code for their Configurable USB Loader forwarder channel. Thank you, FIX94!_

---

## Purpose

This application attempts to load and execute `/openshopchannel/downloader/boot.dol` from the SD card on the user's Wii system. Because this application only loads and executes another application, it is known as a forwarder.

If the application fails to set up SD card access, open the target file, or read the target file, it will launch the Wii Shop Channel to the page `/error?error=FAT_INIT_FAILED`, `/error?error=DOL_OPEN_FAILED`, or `/error?error=DOL_BUFF_FAILED`, respectively.

## Intended Packaging

This application is meant to be installed as a [hidden channel](https://wiibrew.org/wiki/Title_database#00010008:_.22Hidden.22_channels), and should be packaged as a [WAD](https://wiibrew.org/wiki/WAD_files). If installed correctly, the channel will take up about 1 megabyte of the system NAND, and will not occupy a channel slot in the System Menu.

An example of a correctly packed WAD is included in the releases section, as well as the unpacked WAD tree with the correct [title metadata](https://wiibrew.org/wiki/Title_metadata) and [ticket](https://wiibrew.org/wiki/Ticket). The example WAD was created with [CustomizeMii](https://sites.google.com/site/completesg/useful-tools/customizemii), and is based off of [SifJar's "Base 1"](https://sites.google.com/site/wiibannerguide/base-wads-2) base WAD.
