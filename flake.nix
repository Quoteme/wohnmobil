{
  description = "My development shell for all my RV projects";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
    flake-utils = {
      inputs.nixpkgs.follows = "nixpkgs";
      url = "github:numtide/flake-utils";
    };
    esp32.url = "github:mirrexagon/nixpkgs-esp-dev";
  };

  outputs = { self, nixpkgs, flake-utils, esp32 }@inputs:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
          overlays = [ (import "${esp32}/overlay.nix") ];
        };
      in rec {
        defaultPackage = packages.wohnmobil;
        packages.wohnmobil = with pkgs;
          mkShell {
            buildInputs = [ esp-idf-full ];

            shellHook = ''
              exec ${fish}/bin/fish --init-command "source ${esp-idf-full}/export.fish"
            '';
          };
      });
}
