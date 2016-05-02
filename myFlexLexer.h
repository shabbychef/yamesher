//START HEADER
//START COPYRIGHT
/*
 *  Corner Lopping Mesher Software
 *  Copyright (C) 2004 Steven E. Pav
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  Send comments and/or bug reports to:
 *                 spav@ucsd.edu 
 */
//END COPYRIGHT
//START CVSLOG
/* CVS info:
 * $Id: work_queue.h,v 1.6 2004/11/02 23:51:20 spav Exp $
 * $Log: work_queue.h,v $
 *
 */
//END CVSLOG
//END HEADER

#ifndef MYFLEXLEXER_H
#define MYFLEXLEXER_H

#include "readinput.tab.hpp"
#include <FlexLexer.h>

namespace lopping {
	class myFlexLexer : public FlexLexer
	{
		public:
			int yylex( YYSTYPE *yylval );
	};
}

#endif //MYFLEXLEXER_H
