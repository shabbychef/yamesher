
# yamesher

Yet Another Mesher. Originally called _lopper_, 
a simple two dimensional mesh generator with corner lopping.

This is an implementation of the algorithm described in 
`Delaunay Refinement by Corner Lopping,' by 
Steven E. Pav and Noel J. Walkington, IMR 14, San Diego, 2005.
This algorithm uses Ruppert's idea of corner lopping to deal
with small input angles, and improves on the work of Boivin
and Olivier Gooch on meshing for curved boundaries.

This software isn't terribly robust yet.  In particular,
it was designed to convert xfig files of input into eps files
of meshed output which could go into a paper.  See the
Running notes for more limitations.

send feedback to:  Steven E. Pav <shabbychef@gmail.com>

## Installation

(Out of date!)

Prerequisites:

1. the [CGAL library](www.cgal.org) (3.1.0 and greater should work). 
available as `libcgal-dev` on ubuntu.
2. the TCLAP library. this small library for parsing
command line arguments should be included in this
software bundle.

Compiling:

1. edit the Makefile to have the proper location of your
CGAL Makefile, for example:
```
CGAL_MAKEFILE = /usr/share/etc/cgal/make/makefile_i686_Linux-2.4.20-64GB-SMP_g++-3.3.0
```
2. compile the software by typing `make`.
3. when that doesn't work, email me. (ugh.)

## Running

(Also out of date!)

The software can convert `.dat` files and xfig `.fig` files into
meshed `.pcmp` files, and generates `.eps` figures along the way.
The simplest way for me to run the software is with the
included makefile. It knows all the flags and how to call
the software. Test this with the included `.fig` and `.dat`
files.

```{bash}
make helloworld.pcmp
```

This should generate a helloworld.pcmp and a bunch of
figures, including

```
	as_input.ps
	curvature_bounded.ps
	finished_groom.ps
	input_conforming.ps
	quality_insured.ps
```

These give the state of the mesh at various points in the algorithm.
These figures have been tweaked to be human readable; with just a little
editing of thier headers, one can hide the input or the mesh edges.
Poke a little around in these files.  Edit the following lines

```
/sho1  {  true } bind def
/sho12 {  true } bind def
/sho2  {  true } bind def
/shocc { false } bind def
/thinl { false } bind def
```

to change the ps file. 

other information is contained in the ps file, for example the mesh
size, and basic geometry limits:

```
%num of vertices: 25
%num of    faces: 46
%num of    edges: 69
%%%%%%%%%%%%%%%%%%%%%%%%
%min angle: 0.107709 radians. (6.17126deg)
%max angle: 2.84067 radians. (162.759deg)
```

### FLAGS

Try
```{bash}
make help
```
to get a listing of the flags for the 

The flags for the software have the prefix `LTF_` in the makefile.
they are

```
	-L 	the iteration limit (maximum number of output vertices)
	-d	the degree limit--essentially the software has lazy lopping. if
				an input angle is less than 2pi/d it will be lopped.
	-g	the factor determining the size of the original corner protecting ball.
				this should be in (0,0.5).  the corner protecting ball will have
				radius gamma times the shortest input edge at the corner.
	-t  the upper limit on total variation.  input with greater
				curvature will automagically be cut down.
	-b	the factor beta from the paper. this is the factor by which
				corner lopped balls are reduced.
	-q	the quality bound, in the form 4 sin^2(kappa), where kappa 
				is the smallest acceptable angle.  Thus use -q 1 for 30 degrees,
				-q 0.5 for 20.07 degrees, etc.
	-C	a simple Chew-mode upper limit on circumradius.  Larger triangles
				will be split.
	-V	the verbosity.  set to 0 for no output from the algorithm.
	-u	a binary flag of whether a `provoker' should be used to split
				input segments.  See the paper for more details.
```

reasonable values for these are included in the makefile.

## Copyright

Copyright (c) 2004-2016 Steven E. Pav

Permission is hereby granted, free of charge, to any person 
obtaining a copy of this software and associated documentation 
files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, 
publish, distribute, sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be 
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS 
BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN 
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR 
IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
THE SOFTWARE.

------

_more to come..._

