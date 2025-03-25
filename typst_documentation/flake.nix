{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    fonts = {
      url = "/home/jiricmi/repos/logic_probe/typst_documentation/template/template/res/fonts";
      flake = false;
    };
  };

  outputs = { self, nixpkgs, fonts, ... }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs {
        inherit system;
        config.allowUnfree = true;
      };
      
      # Create a package for your custom fonts
      customFonts = pkgs.stdenvNoCC.mkDerivation {
        name = "custom-fonts";
        src = fonts;
        installPhase = ''
          mkdir -p $out/share/fonts/truetype
          cp -R $src/* $out/share/fonts/truetype/
        '';
      };

    in
    {
      devShells.${system}.default = pkgs.mkShell {
        nativeBuildInputs = with pkgs; [
          typst
          tinymist
          customFonts  # Add the fonts to the shell
        ];

        # Set FONTCONFIG_FILE to make fonts discoverable
        shellHook = ''
          export FONTCONFIG_FILE=${pkgs.makeFontsConf {
            fontDirectories = [ 
              "${customFonts}/share/fonts/truetype"
              "${pkgs.dejavu_fonts}/share/fonts"
            ];
          }}
          unset SOURCE_DATE_EPOCH
        '';
      };
    };
}
