
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
RUNNING notes for more limitations.

send feedback to:  Steven E. Pav <shabbychef@gmail.com>

## more to come

