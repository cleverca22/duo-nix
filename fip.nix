{ runCommand, tools, chip_conf, duo-buildroot-sdk, uboot, opensbi, fsbl }:

let
  uboot_path = "${uboot}/u-boot.bin";
  uboot_path' = ./buildroot-unpacked/u-boot.bin;
  bl2_path' = "${fsbl}/bl2.bin";
  bl2_path = ./buildroot-unpacked/bl2.bin;
  opensbi_path' = ./buildroot-unpacked/opensbi.bin;
  opensbi_path = "${opensbi}/fw_dynamic.bin";
in
runCommand "fip-simple" {
  nativeBuildInputs = [ tools ];
} ''
  mkdir $out
  NOR_INFO=$(printf '%72s' | tr ' ' 'FF')
  NAND_INFO=00000000

  touch empty.bin

  fiptool.py -v genfip $out/fip.bin \
    --DDR_PARAM=${duo-buildroot-sdk}/fsbl/test/cv181x/ddr_param.bin \
    --MONITOR_RUNADDR=0x80000000 --MONITOR=${opensbi_path} \
    --BLCP_2ND_RUNADDR=0x83f40000 --BLCP_2ND=${./buildroot-unpacked/blcp_2nd.bin} \
    --CHIP_CONF=${chip_conf} --NOR_INFO=$NOR_INFO --NAND_INFO=$NAND_INFO \
    --LOADER_2ND=${uboot_path} --compress=lzma \
    --BLCP=empty.bin --BLCP_IMG_RUNADDR=0x05200200 \
    --BL2=${bl2_path}
''
