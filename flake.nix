{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs =
    { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs {
        inherit system;
        config.allowUnfree = true;
      };
      crossPkgs = pkgs.pkgsCross.arm-embedded;
    in
    {
      devShells.${system}.default = pkgs.mkShell {
        nativeBuildInputs = with pkgs; [
          stlink
          crossPkgs.buildPackages.gdb
          gcc-arm-embedded
          openocd
          cmake
          putty
          stm32cubemx
          clang-tools
          glibc_multi
          stm32flash
        ];

        shellHook = ''
          export OPENOCD_PATH="${pkgs.openocd}"
        '';
      };
    };
}
