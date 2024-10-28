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
    clang-tools
    glibc_multi
  ];
  OPENOCD_PATH="${pkgs.openocd}";
}
