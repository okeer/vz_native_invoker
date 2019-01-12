# VZ native API test tool

## Description

This tool is designed to play around with Virtuozzo/OpenVZ C++ API.

## Usage

```
# ./vz_native_invoker

This tool invokes Virtuozzo native C++ API.

Usage:

./vz_native_invoker --mode <specific mode> [<mode specific options/arguments>]

Options:
  -h [ --help ]         Show this message
  -m [ --mode ] arg     Select mode: clone resize


Clone VM mode options (PrlVm_CloneEx):
  -n [ --name ] arg     a name for the new VM
  -t [ --template ] arg a template from which VM will be cloned


Resize VM HDD options (PrlVmDev_ResizeImage):
  -n [ --name ] arg     a name for the VM to resize
  -s [ --size ] arg     a new size for HDD
```

# How to build on CentOS

## Prerequisites

* prlsdk library;
* boost (program_options module is used).

## Steps

1. Install boost lib:

`# yum install boost-devel.x86_64`

2. Install Virtuozzo SDK:

`# yum install libprlsdk.x86_64`

3. Create a symlink to the library - it does not exist by default, however SDK relies on it:

`# ln -s /usr/lib64/libprl_sdk.so.7 /usr/lib64/libprl_sdk.so`

4. Clone repo:

`# git clone https://github.com/okeer/vz_native_invoker.git`

5. Build the tool:

`# cd vz_native_invoker`

`# make`
