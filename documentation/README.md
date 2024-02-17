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
make depends  # Compile dependencies
make install  # Install crowarmor module
```

Additionally, the Makefile provides helpful targets for executing scripts:

```sh
Available targets:
  make all           : Compile the crowarmor Driver
  make clean         : Delete driver compilation
  make infos         : Display crowarmor Driver information
  make help          : Display this help message
  make install       : Install Driver in machine
  make uninstall     : Uninstall Driver from machine
  make depends       : Compile the dependencies
```

After you install the driver, make sure the driver resides in `/dev/crowarmor`.

## Organizational Folders

<pre>
.
├── <a href="../assets">assets</a>             <!-- Images -->
│   └── <a href="../assets/crowarmor.png">crowarmor.png</a>
├── <a href="../documentation">documentation</a>      <!-- crowarmor documentation -->
│   └── <a href="../documentation/README.md">README.md</a>
├── <a href="../include">include</a>            <!-- Headers utilized in sources -->
│   ├── <a href="../include/control_registers">control_registers</a>
│   ├── <a href="../include/drm">drm</a>
│   ├── <a href="../include/err">err</a>
│   ├── <a href="../include/kpobres">kpobres</a>
│   └── <a href="../include/signals">signals</a>
├── <a href="../license">LICENSE</a>
├── <a href="../makefile">Makefile</a>
├── <a href="../modules">modules</a>            <!-- Dependencies -->
│   └── <a href="../modules/lunatik">lunatik</a>
├── <a href="../readme">README.md</a>
└── <a href="../sources">sources</a>            <!-- crowarmor codes and inclusive modules -->
    ├── <a href="../sources/chrdev">chrdev</a>
    ├── <a href="../sources/control_registers">control_registers</a>
    ├── <a href="../sources/drm">drm</a>
    ├── <a href="../sources/hook_syscall">hook_syscall</a>
    ├── <a href="../sources/inspector">inspector</a>
    ├── <a href="../sources/kpobres">kpobres</a>
    ├── <a href="../sources/signals">signals</a>
    ├── <a href="../sources/crowarmor">crowarmor</a>
    └── <a href="../sources/tracing">tracing</a>
</pre>

- [**sources**](../sources): This directory contains main modules and comprehensive source code files for the crowarmor project.

- [**modules**](../modules): Inside this directory, you'll find submodules and their corresponding dependencies, forming the building blocks of the crowarmor application.

- [**include**](../include): This directory houses essential header files utilized in the source code, providing structured and modular inclusion for efficient code organization.

- [**documentation**](../documentation): Explore this section for well-commented code snippets and an overarching overview, serving as a comprehensive guide to understand the crowarmor project.

- [**assets**](../assets): In this directory, discover a collection of images specifically related to the visual elements and graphics associated with the crowarmor application.

## Specifications

The crowarmor Linux Driver is specifically optimized for compilation using GCC version 12 to achieve optimal performance and compatibility. Please ensure that GCC version 12 is installed on your system before proceeding with the installation steps provided below. The last version was successfully compiled on kernel 6.5.6-76060506-generic.
