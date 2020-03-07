# cip - The C/C++ Package Installer

* * *


cip is the package installer for C/C++. You can use cip to install packages from the [C or CPP Package Index](https://github.com/springkim/cip/releases/tag/archive)and other indexes.


### Installation

[Download windows installer](https://github.com/springkim/cip/releases/download/release/install_cip.bat)

[Download linux installer](https://github.com/springkim/cip/releases/download/release/install_cip.sh)

Download and run the above file to complete the installation.

### Usage

```
cip <command> [options]

Commands:
  install                       Install packages.
  download                      Download packages.
  uninstall                     Uninstall packages.
  freeze                        Output installed packages in requirements format.
  archive                       List installable packages.
  help                          Show help for commands.
Options:
  -h, --help                    Show help.
  -V, --version                 Show version and exit.
  -q, --quiet                   Give less output. Option is additive, and can be used up to 3 times (corresponding to WARNING, ERROR, and CRITICAL logging levels).
```

CIP requires administrator privileges. For Windows, you need run a cmd with administrator privileges, and for Linux, you need run as sudo.

### Example

The example below installs the default option openv and openblas for the latest version.
If you do not specify a compiler, install it in the most recent version of Visual Studio.
If you do not have Visual Studio, install it in MinGW.

```
cip install opencv openblas
```

The example below installs an opencv for a particular version and option.

```
cip install opencv==3.4.9+cu101 -c vs2017
```