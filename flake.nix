{
  inputs = {
    duo-buildroot-sdk.url = "github:milkv-duo/duo-buildroot-sdk";
    duo-buildroot-sdk.flake = false;
  };
  outputs = { self, nixpkgs, duo-buildroot-sdk }:
  {
    packages.x86_64-linux = let
      ov = _self: super: {
        inherit self duo-buildroot-sdk;
      };
      pkgs = (nixpkgs.legacyPackages.x86_64-linux.extend ov).extend (import ./overlay.nix);
    in {
      inherit (pkgs) tools chip_conf fip-simple memmap fsbl opensbi uboot fipinfo;
    };
  };
}
