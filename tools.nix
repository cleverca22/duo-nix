{ runCommand, duo-buildroot-sdk, python3, chip, board }:

runCommand "tools" {
  buildInputs = [ python3 ];
}
''
  mkdir -pv $out/bin
  cp ${duo-buildroot-sdk}/fsbl/plat/cv181x/{fiptool.py,chip_conf.py} $out/bin
  cp ${duo-buildroot-sdk}/build/scripts/mmap_conv.py $out/bin
  cp ${duo-buildroot-sdk}/build/boards/cv180x/${chip}_${board}/memmap.py $out/bin
  cp ${duo-buildroot-sdk}/build/tools/common/image_tool/{mkcvipart.py,XmlParser.py} $out/bin/
  cp ${duo-buildroot-sdk}/build/tools/common/image_tool/mk_imgHeader.py $out/bin
  chmod +x $out/bin/*.py
  patchShebangs $out/bin
''
