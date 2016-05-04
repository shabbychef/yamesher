
/*
 
  This file is part of yamesher.
  
  yamesher is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  yamesher is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public License
  along with yamesher.  If not, see <http://www.gnu.org/licenses/>.

  Created: 2016.05.02
  Copyright: Steven E. Pav, 2016
  Author: Steven E. Pav <steven@corecast.io>
  Comments: Steven E. Pav
*/

#ifndef __DEF_DEBUGGING__
#define __DEF_DEBUGGING__

#include "config.h"

#include <cmath>
#include <CGAL/basic.h>

//verbosity levels

//whether to gripe info regarding the progress of the algorithm
#define GRIPE_PROGRESS 1
//whether to gripe info regarding the progress of the algorithm
#define GRIPE_ITERATES 2
#define GROSS_ITERATES 10000

//debugging verbosity

//whether to gripe when entering or exiting a function
#define GRIPE_GATES 3
//whether to gripe when entering or exiting a major loop
#define GRIPE_LOOPS 4
//whether to gripe info regarding points, segs, balls being split, etc
#define GRIPE_INFO 3
//whether to gripe info regarding which way control goes
#define GRIPE_FORKS 4
//whether to gripe constructor and destructor calls
#define GRIPE_CDTORS 4

//paranoia levels

//whether to check really bizarre possibilities
#define CHECK_BIZARRE 6
//whether to check for problems with inexact arithmetic.
#define CHECK_MATH 1


//convert objects to string to gripe them

#define PTOSTR(p)  "(x: " << p.x() << " y: " << p.y() << ")\n" 
#define VHTOSTR(vh)  (*vh) << "\n"

#define GRIPEP(p) GRIPE( "(x: " << p.x() << " y: " << p.y() << ")\n" )
#define GRIPEV(v) GRIPEP( v.point() )
#define GRIPEVH(vh) GRIPE( (*vh) << "\n" )

#ifdef PARANOID
#define GRIPE_P(X) std::cerr << X
#define GRIPE_PX(Y,Z,X) if (Y > Z) { GRIPE_P(X); } else { }
#define GRIPE_PXP(Y,Z,p) GRIPE_PX(Y,Z, PTOSTR(p))
#define GRIPE_PXVH(Y,Z,vh) GRIPE_PX(Y,Z, VHTOSTR(vh))
#else
#define GRIPE_P(X) 			/* X */ 
#define GRIPE_PX(Y,Z,X) /* X */
#define GRIPE_PXP(Y,Z,p) /* X */
#define GRIPE_PXVH(Y,Z,vh) /* X */
#endif

#define GRIPE_IX(Y,Z,X) if (Y > Z) { std::cerr << X; }

#define LUVERBOSITY lopping::VERBOSITY

#define VOICE(Z,X) GRIPE_IX(LUVERBOSITY,Z,X)
#define CARP(Z,X)   GRIPE_PX(LUVERBOSITY,Z,X)
#define CARPP(Z,p)  GRIPE_PXP(LUVERBOSITY,Z,p)
#define CARPVH(Z,vh) GRIPE_PXVH(LUVERBOSITY,Z,vh)

#endif /* __DEF_DEBUGGING__ */

//for vim modeline: (do not edit)
// vim:ts=2:sw=2:tw=79:fdm=marker:fmr=FOLDUP,UNFOLD:cms=//%s:tags=.c_tags;:syn=cpp:ft=cpp:mps+=<\:>:ai:si:cin:nu:fo=croql:cino=p0t0c5(0:
