# crowamor: Linux Driver Documentation

This document provides a comprehensive overview of the functionality and components of the crowarmor Linux Driver.

## Roadmap

* [Installation](#installation)
* [Organizational Folders](#organizational-folders)
* [Specifications](#specifications)

## Installation

To install crowarmor, follow the commands below. Ensure that you have GCC version 12 installed for proper compilation. Clone crowarmor dependencies, with Lunatik being a current requirement for extending and scripting in the kernel using LUA scripts.

```sh
make          # Compile crowarmor
make install  # Install crowarmor module
```

Additionally, the Makefile provides helpful targets for executing scripts:

```sh
Available targets:
  make all           : Compile the CrowArmor Driver
  make clean         : Delete driver compilation
  make infos         : Display CrowArmor Driver information
  make help          : Display this help message
  make install       : Install Driver in machine
  make uninstall     : Uninstall Driver from machine
  make tests_compile : Make Tests Driver
  make tests_clean   : Delete Tests Driver
  make qemu_start    : Start emulation using qemu # for debian-based
```

After you install the driver, make sure the driver resides in `/dev/crowarmor`.

## Organizational Folders

<pre>
.
├── <a href="../assets">assets</a>
├── <a href="../documentation">documentation</a>
├── <a href="../include">include</a>
├── <a href="../LICENSE">LICENSE</a>
├── <a href="../Makefile">Makefile</a>
├── <a href="../README.md">README.md</a>
├── <a href="../scripts">scripts</a>
├── <a href="../sources">sources</a>
└── <a href="../tests">tests</a>

7 directories, 3 files
</pre>

- [**sources**](../sources): This directory contains main modules and comprehensive source code files for the crowarmor project.

- [**modules**](../modules): Inside this directory, you'll find submodules and their corresponding dependencies, forming the building blocks of the crowarmor application.

- [**include**](../include): This directory houses essential header files utilized in the source code, providing structured and modular inclusion for efficient code organization.

- [**documentation**](../documentation): Explore this section for well-commented code snippets and an overarching overview, serving as a comprehensive guide to understand the crowarmor project.

- [**assets**](../assets): In this directory, discover a collection of images specifically related to the visual elements and graphics associated with the crowarmor application.

- [**assets**](../tests): Tests used for features

## Specifications

The crowarmor Linux Driver is specifically optimized for compilation using GCC version 12 to achieve optimal performance and compatibility. Please ensure that GCC version 12 is installed on your system before proceeding with the installation steps provided below. The last version was successfully compiled on kernel 6.5.6-76060506-generic.
