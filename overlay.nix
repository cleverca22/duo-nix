self: super: {
  chip = "cv1800b";
  board = "milkv_duo_sd";
  tools = self.callPackage ./tools.nix {};
  chip_conf = self.callPackage ./chipconf.nix {};
  fip-simple = self.callPackage ./fip-simple.nix {};
  memmap = self.callPackage ./memmap.nix {};
  fsbl = self.callPackage ./fsbl.nix {};
  opensbi = self.callPackage ./opensbi.nix {};
  uboot = self.callPackage ./uboot.nix {};
  fipinfo = self.callPackage ./fipinfo.nix {};
  freertos = self.callPackage ./freertos.nix {};
  fip = self.callPackage ./fip.nix {};
}
