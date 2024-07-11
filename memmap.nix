{ runCommand, tools, duo-buildroot-sdk, chip, board }:

runCommand "memmap" {
  nativeBuildInputs = [tools];
} ''
  mkdir -p $out/include/
  XML=${duo-buildroot-sdk}/build/boards/cv180x/${chip}_${board}/partition/partition_sd.xml
  mmap_conv.py --type h $(type -p memmap.py) $out/cvi_board_memmap.h
  mkcvipart.py $XML $out/include/
  mk_imgHeader.py $XML $out/include/
''
