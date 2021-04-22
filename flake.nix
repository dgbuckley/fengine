{
  description = "Recipe Tools";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, utils }:
    utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
        };
      in rec {
        devShell = pkgs.mkShell {
          name = "recipe-util_dev";
          nativeBuildInputs = with pkgs; [
            ninja
            meson
            gcc
            ccls
          ];
        };
      });
}
