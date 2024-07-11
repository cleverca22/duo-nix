{ runCommandCC }:

runCommandCC "fipinfo" {}
''
  mkdir -p $out/bin/
  $CC ${./fipinfo.c} -o $out/bin/fipinfo
''
