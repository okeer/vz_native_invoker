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
  -m [ --mode ] arg     Select mode: clone resize refresh


Clone VM mode options (PrlVm_CloneEx):
  -n [ --name ] arg     a name for the new VM
  -t [ --template ] arg a template from which VM will be cloned


Resize VM HDD options (PrlVmDev_ResizeImage):
  -n [ --name ] arg     a name for the VM to resize
  -s [ --size ] arg     a new size for HDD


Refresh VM options (PrlVm_RefreshConfig):
  -n [ --name ] arg     a name for the VM to refresh config
```

Example:

```
# ./vz_native_invoker --mode refresh -n centos1
Action refresh completed with success
#
```

# How to build on CentOS

## Prerequisites

* prlsdk library;
* boost (program_options module is used).

## Steps

1. Install boost lib:

`# yum install boost-devel.x86_64`

2. Install Virtuozzo SDK (Virtuozzo 7, for PCS its parallels-virtualization-sdk):

`# yum install libprlsdk.x86_64`

3. Clone repo:

`# git clone https://github.com/okeer/vz_native_invoker.git`

4. Build the tool:

`# cd vz_native_invoker && make`
