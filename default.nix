{pkgs ? import <nixpkgs> {config = { allowUnfree = true; }; } }:
pkgs.mkShell {
  nativeBuildInputs = with pkgs; [
    stlink
    pkgsCross.arm-embedded.buildPackages.gdb
    gcc-arm-embedded
    openocd
    cmake
    putty
    stm32cubemx
    minicom
  ];
  OPENOCD_PATH="${pkgs.openocd}";
}
