# **BackChannel**

Maintainer: Jonathan Williams

## Description

A program to generate gamma-ray spectra for a specific channel, using a linear combination of gamma-ray spectra for specific gates.

## How to Use

The program takes a parameter file (a sample is provided) as the only command line argument.  The parameter file tells the program which particle channel to sort, what the detection efficiencies for different particles are, what gate spectra are available to use to construct the final spectrum (and what their particles gates are), and finally the filename to write the output spectrum to.

### Parameter File Format

```
SORT_CH 0 2
```
Specifies the channel to be sorted, in number of particles.  In this example, the sorted data will have 0 of the first particle type (protons) and 2 of the 2nd particle type (alphas).  The order of the particle types doesn't matter as long as it is consistent throughout the parameter file, and more particle types can be added.

```
EFF 0.4 0.3
```
Specifies the absolute detection efficiencies for the different particle types.  In this example, protons are detected with 40% efficiency and alphas with 30% efficiency.

```
GATE_SP 0 1 0p1a.mca
```
Specifies a particle-gated spectrum (in .mca format) to use when constructing the sorted data.  The particle gate is specified along with the filename.  In this example, the particle gate is 0 protons and 1 alpha.  Multiple particle-gated spectra can (and should) be specified.  

The gated data should contain gates which are more restrictive than the desired channel to be sorted.  For example, if the 2 proton 0 alpha channel is to be sorted, there should be data corresponding to the 2 proton 0 alpha gate as well as more restrictive gates such as 3 proton 0 alpha, 2 proton 1 alpha, etc. 

```
OUT_SP 0p2ach.mca
```
Specifies the filename to write the sorted output spectrum to.


## How to Install

Use 'make' to compile.  To run the program from anywhere, move the resulting `backchannel` executable to any directory under your `$PATH` environment variable.

Tested using gcc and GNU make on Ubuntu 14.04.  The code is self-contained and should work on more or less any Linux distro.
