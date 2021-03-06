/**
\author		Andrey Korotkov aka DRON
\date		26.04.2012 (c)Andrey Korotkov

This file is a part of DGLE2 project and is distributed
under the terms of the GNU Lesser General Public License.
See "DGLE2.h" for more details.
*/

#ifndef _BASERENDERERGL_H
#define _BASERENDERERGL_H


#include "..\..\Common.h"

#ifndef NO_BUILTIN_RENDERER

#include <GL/glew.h>
#include <GL/wglew.h>

class CBaseRendererGL : public CInstancedObj
{
	HGLRC _hRC;
	TWinDrawHandle _hDC;

public:
	
	CBaseRendererGL(uint uiInstIdx);

	bool Prepare(TCRendererInitResult &stResults);
	bool Initialize(TCRendererInitResult &stResults);
	bool Finalize();
	bool AdjustMode(TEngWindow &stNewWin);
	bool MakeCurrent();
	void Present();

};

#endif

#endif //_BASERENDERERGL_H