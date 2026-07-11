# Fanzine DS

This repository is based on LNH-team's [DSpico Bootloader](https://github.com/LNH-team/dspico-bootloader). It's meant to be used as a reader for fanzines that can be placed inside a DSpico cartridge.

- Author of the fanzine used: [Nanaki](https://nanakigamedev.carrd.co/)
- Author of the fanzine reader: [Geri](https://links.gerardgascon.com/)

> [!NOTE]
> The rest of the README is copied from the original repository.

## Setup & configuration
We recommend using WSL (Windows Subsystem for Linux), or MSYS2 to compile this repository.
The steps provided will assume you already have one of those environments set up.

1. Install [BlocksDS](https://blocksds.skylyrac.net/docs/setup/options/).
2. To be able to use this bootloader, compile [Pico Loader](https://github.com/LNH-team/pico-loader) and place the files on your SD card.
3. Place the rom to boot on the root of your SD card as `_picoboot.nds`.

## Compiling
1. Compile the bootloader by running `make`
2. Patch `BOOTLOADER.nds` with the [DSpico DLDI driver](https://github.com/LNH-team/dspico-dldi) using dlditool:
    - `dlditool DSpico.dldi BOOTLOADER.nds`
3. Prepare `BOOTLOADER.nds` for use on a cartridge using [DSRomEncryptor](https://github.com/Gericom/DSRomEncryptor):
    - `DSRomEncryptor BOOTLOADER.nds BOOTLOADER.nds`
    - Note that this requires the ntr and twl blowfish keys (see also the readme of DSRomEncryptor) and [.NET 9.0](`https://learn.microsoft.com/en-us/dotnet/core/install/linux-ubuntu-install?tabs=dotnet9&pivots=os-linux-ubuntu-2404`)
4. Rename `BOOTLOADER.nds` to `default.nds` and place it in the `roms` folder of the [DSpico Firmware](https://github.com/LNH-team/dspico-firmware)
5. Compile the DSpico Firmware and flash it to the DSpico

## License
This software is licensed under the zlib license.

Additional licenses may apply to the project.

For details, see the `license` directory, as well as `LICENSE.txt`.

## Contributors
- [@Gericom](https://github.com/Gericom)
- [@XLuma](https://github.com/XLuma)
- [@Dartz150](https://github.com/Dartz150)
- [@lifehackerhansol](https://github.com/lifehackerhansol)
