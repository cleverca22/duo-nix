{ runCommandCC, xz, enableDebugging }:

runCommandCC "fipinfo" {
  buildInputs = [ (enableDebugging xz) ];
}
''
  mkdir -p $out/bin/
  $CC ${./fipinfo.c} -o $out/bin/fipinfo -llzma -g
''
