{ runCommand, tools }:

runCommand "chip_conf.bin" {
  nativeBuildInputs = [ tools ];
}
''
  chip_conf.py $out
''
