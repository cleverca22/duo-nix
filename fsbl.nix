{ runCommand, pkgsCross, duo-buildroot-sdk, memmap, python3, opensbi, uboot }:

runCommand "fsbl" {
  nativeBuildInputs = [
    pkgsCross.riscv64.stdenv.cc
    python3
  ];
  src = duo-buildroot-sdk;
  patches = [ ./fsbl.patch ];
} ''
  unpackPhase
  cd $sourceRoot
  patchPhase
  patchShebangs fsbl/plat/cv181x/chip_conf.py
  patchShebangs fsbl/plat/cv181x/fiptool.py

  mkdir -pv fsbl/build
  cp ${memmap}/cvi_board_memmap.h fsbl/build/
  mkdir -p opensbi/build/platform/generic/firmware
  cp ${opensbi}/fw_dynamic.bin opensbi/build/platform/generic/firmware/fw_dynamic.bin

  export LOADER_2ND_PATH=${uboot}/u-boot.bin

  export NIX_HARDENING_ENABLE=
  CHIP_ARCH=CV180X make -C fsbl/ O=build CROSS_COMPILE=riscv64-unknown-linux-gnu- ARCH=riscv BOOT_CPU=riscv V=1

  ls -ltrh fsbl/build/
  mkdir -p $out
  cp fsbl/build/fip.bin $out/
''
