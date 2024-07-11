{ runCommand, pkgsCross, duo-buildroot-sdk, uboot, chip, board }:

runCommand "opensbi" {
  nativeBuildInputs = [
    pkgsCross.riscv64.stdenv.cc
  ];
  src = duo-buildroot-sdk;
  PLATFORM_RISCV_ISA = "rv64ima_zifencei_zicsr";
  hardeningDisable = [ "all" ];
  patches = [ ./opensbi.patch ];
} ''
  unpackPhase
  cd $sourceRoot
  patchPhase

  make -C opensbi PLATFORM=generic FW_PAYLOAD_PATH=${uboot}/u-boot.bin FW_FDT_PATH=${uboot}/${chip}_${board}.dtb

  mkdir -p $out
  cp opensbi/build/platform/generic/firmware/fw_dynamic.bin $out/
''
