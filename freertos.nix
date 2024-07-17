{ runCommand, pkgsCross, duo-buildroot-sdk, cmake, ninja }:

runCommand "freertos" {
  nativeBuildInputs = [
    pkgsCross.riscv64-embedded.stdenv.cc
    cmake
    ninja
  ];
  src = duo-buildroot-sdk;
  patches = [ ./freertos.patch ];
} ''
  unpackPhase
  cd $sourceRoot

  patchPhase

  export BUILD_PATH=$(realpath build)
  cp -v build/boards/cv180x/cv1800b_milkv_duo_sd/cv1800b_milkv_duo_sd_defconfig build/.config

  cd freertos/cvitek

  patchShebangs ./build_cv180x.sh

  export DDR_64MB_SIZE=y

  ./build_cv180x.sh

  ls -ltrh install/bin
''
