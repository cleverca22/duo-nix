{ runCommand, tools, chip_conf }:

runCommand "fip-simple" {
  nativeBuildInputs = [ tools ];
} ''
  mkdir $out
  NOR_INFO=$(shell printf '%72s' | tr ' ' 'FF')
  NAND_INFO=00000000
  fiptool.py -v genfip $out/fip.bin --CHIP_CONF=${chip_conf} --NOR_INFO=$NOR_INFO --NAND_INFO=$NAND_INFO --BL2=${bl2}/bl2.bin
''
