{
  description = "A gnss observations and products downloader";
  inputs = {
    flake-utils.url = "github:numtide/flake-utils";
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };

  outputs = { self, flake-utils, nixpkgs }:
    flake-utils.lib.eachSystem
      (with flake-utils.lib.system; [ x86_64-linux aarch64-linux ])
      (system:
        let
          version = pkgs.lib.substring 0 8 self.lastModifiedDate or self.lastModified or "19700101";
          meta = with pkgs.lib; {
            license = licenses.mit;
            maintainers = with maintainers; [ ocfox ];
          };
          pkgs = import nixpkgs { inherit system; };

          gampii-good = pkgs.stdenv.mkDerivation {
            pname = "gampii-good";
            meta = meta;
            version = version;

            src = ./.;

            buildInputs = with pkgs; [
              cmake
              yaml-cpp
            ];

          };

        in
        rec {
          packages = {
            gampii-good = gampii-good;
            default = gampii-good;
          };

          devShells = rec {
            default = gampii-dev;
            gampii-dev = pkgs.mkShell {
              inputsFrom = with packages; [ cmake make ];
            };
          };

          apps = {
            default = gampii-good;
            gampii-good = {
              type = "gampii-good";
              program = "${self.packages.${system}.gampii-good}/bin/run_GOOD";
            };
          };
        }
      );
}
