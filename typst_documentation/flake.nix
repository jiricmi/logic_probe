{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    fonts = {
      url = "path:./template/template/res/fonts";
      flake = false;
    };
  };

  outputs =
    {
      self,
      nixpkgs,
      fonts,
    }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs {
        inherit system;
        config.allowUnfree = true;
      };

      # Vytvoření balíčku se všemi fonty
      customFonts = pkgs.runCommand "custom-fonts" { } ''
        mkdir -p $out/share/fonts/truetype
        cp ${fonts}/*.ttf $out/share/fonts/truetype/
      '';

    in
    {
      devShells.${system}.default = pkgs.mkShell {
        nativeBuildInputs = with pkgs; [
          typst
          tinymist
        ];

        packages = [ customFonts ];

        # Konfigurace pro aplikace používající fonty
        FONTCONFIG_FILE = pkgs.makeFontsConf {
          fontDirectories = [
            "${customFonts}/share/fonts/truetype"
          ];
        };
        shellHook = ''
          unset SOURCE_DATE_EPOCH
        '';
      };
    };
}
