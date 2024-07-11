{ runCommandCC, pkgsCross, duo-buildroot-sdk, memmap, bison, flex, chip, board }:

runCommandCC "uboot" {
  nativeBuildInputs = [
    pkgsCross.riscv64.stdenv.cc
    bison
    flex
  ];
  src = duo-buildroot-sdk;
  patches = [ ./u-boot.patch ];
} ''
  unpackPhase
  cd $sourceRoot
  patchPhase

  cp -v build/boards/cv180x/${chip}_${board}/u-boot/*defconfig u-boot-2021.10/configs
  cp -v build/boards/cv180x/${chip}_${board}/u-boot/cvitek_${chip}_${board}_defconfig u-boot-2021.10/.config
  cp -v build/boards/cv180x/${chip}_${board}/u-boot/cvi_board_init.c u-boot-2021.10/board/cvitek/
  find build/boards/cv180x/ -path "*dts_riscv/*.dts" -print0 | xargs -0 cp -t u-boot-2021.10/arch/riscv/dts

   cp build/boards/default/dts/cv180x_riscv/cv180x_base_riscv.dtsi u-boot-2021.10/arch/riscv/dts/
   cp build/boards/default/dts/cv180x/*dtsi u-boot-2021.10/arch/riscv/dts/

  cd u-boot-2021.10

  cp -v ${memmap}/cvi_board_memmap.h include/
  cp -v ${memmap}/include/cvipart.h include/
  cp -v ${memmap}/include/imgs.h include/

  export CROSS_COMPILE=riscv64-unknown-linux-gnu-
  export CVIBOARD=${board}
  export ARCH=riscv
  export MULTI_FIP=0
  export CHIP=${chip}

  make olddefconfig
  make all -j 8

  mkdir -p $out
  cp -v u-boot.{bin,elf} u-boot.cfg.configs $out/
  cp -v arch/riscv/dts/${chip}_${board}.dtb $out/
''
