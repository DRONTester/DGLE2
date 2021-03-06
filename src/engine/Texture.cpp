/**
\author		Korotkov Andrey aka DRON
\date		01.05.2012 (c)Korotkov Andrey

This file is a part of DGLE2 project and is distributed
under the terms of the GNU Lesser General Public License.
See "DGLE2.h" for more details.
*/

#include "Texture.h"
#include "ResourceManager.h"
#include "Render.h"
#include "Render2D.h"

const float c_fQuad[] = {
	-0.5f, -0.5f, -0.5f, 0.5f,
	 0.5f, -0.5f, 0.5f, 0.5f,
	 0.f, 1.f, 0.f, 0.f,
	 1.f, 1.f, 1.f, 0.f
	};

CTexture::CTexture(uint uiInstIdx, ICoreTexture *pCoreTex, uint uiWidth, uint uiHeight):
CInstancedObj(uiInstIdx),
_pCoreTexture(pCoreTex),
_uiWidth(uiWidth), _uiHeight(uiHeight),
_uiFrameWidth(0), _uiFrameHeight(0)
{
	memcpy(_quad, c_fQuad, sizeof(float)*8);
	_pCoreRenderer = Core()->pCoreRenderer();
	_pRender2D = Core()->pRender()->pRender2D();
}

CTexture::~CTexture()
{
	_pCoreTexture->Free();
}

HRESULT CALLBACK CTexture::SetFrameSize(uint uiFrameWidth, uint uiFrameHeight)
{
	_uiFrameWidth	= uiFrameWidth;
	_uiFrameHeight	= uiFrameHeight;
	return S_OK;
}

HRESULT CALLBACK CTexture::GetFrameSize(uint &uiFrameWidth, uint &uiFrameHeight)
{
	uiFrameWidth	= _uiFrameWidth;
	uiFrameHeight	= _uiFrameHeight;
	return S_OK;
}

HRESULT CALLBACK CTexture::GetCoreTexture(ICoreTexture *&prCoreTex)
{
	prCoreTex = _pCoreTexture;
	return S_OK;
}

HRESULT CALLBACK CTexture::Draw2DSimple(int iX, int iY, uint uiFrameIndex)
{
	if (_uiFrameWidth + _uiFrameHeight + uiFrameIndex == 0)
	{
		_pRender2D->DrawTexture((ITexture*)this, TPoint2((float)iX, (float)iY), TPoint2((float)_uiWidth, (float)_uiHeight), TRectF(0.f, 0.f, (float)_uiWidth, (float)_uiHeight), 0.f, EF_BLEND);
		return S_OK;
	}
	else
		return _pRender2D->DrawSpriteA((ITexture*)this, TPoint2((float)iX, (float)iY), TPoint2((float)_uiWidth, (float)_uiHeight), uiFrameIndex, 0.f, EF_BLEND);
}

HRESULT CALLBACK CTexture::Draw2D(int iX, int iY, uint uiWidth, uint uiHeight, float fAngle, uint uiFrameIndex)
{
	if (_uiFrameWidth + _uiFrameHeight + uiFrameIndex == 0)
	{
		_pRender2D->DrawTexture((ITexture*)this, TPoint2((float)iX, (float)iY), TPoint2((float)uiWidth, (float)uiHeight), TRectF(0.f, 0.f, (float)_uiWidth, (float)_uiHeight), fAngle, EF_BLEND);
		return S_OK;
	}
	else
		return _pRender2D->DrawSpriteA((ITexture*)this, TPoint2((float)iX, (float)iY), TPoint2((float)uiWidth, (float)uiHeight), uiFrameIndex, fAngle, EF_BLEND);
}

HRESULT CALLBACK CTexture::Draw3D(uint uiFrameIndex)
{
	_pCoreRenderer->BindTexture(_pCoreTexture);

	if (_uiFrameWidth + _uiFrameHeight + uiFrameIndex == 0)
		_pCoreRenderer->Draw(TDrawDataDesc((uint8 *)c_fQuad, 8*sizeof(float)), CRDM_TRIANGLE_STRIP, 4);
	else
	{
		float	tx = (uiFrameIndex * _uiFrameWidth % _uiWidth)/(float)_uiWidth,
				ty = (uiFrameIndex * _uiFrameWidth / _uiWidth * _uiFrameHeight)/(float)_uiHeight, 
				tw = _uiFrameWidth/(float)_uiWidth,
				th = _uiFrameHeight/(float)_uiHeight;

		_quad[8]  = tx;			_quad[9]  = ty + th;
		_quad[10] = tx;			_quad[11] = ty;
		_quad[12] = tx + tw;	_quad[13] = _quad[9];
		_quad[14] = _quad[12];	_quad[15] = ty;

		_pCoreRenderer->Draw(TDrawDataDesc((uint8 *)_quad, 8*sizeof(float)), CRDM_TRIANGLE_STRIP, 4);
	}

	return S_OK;
}

HRESULT CALLBACK CTexture::GetDimensions(uint &uiWidth, uint &uiHeight)
{
	uiWidth  = _uiWidth;
	uiHeight = _uiHeight;
	return S_OK;
}

HRESULT CALLBACK CTexture::Bind(uint uiMTextureLayer)
{
	Core()->pCoreRenderer()->BindTexture(_pCoreTexture);
	return S_OK;
}

HRESULT CALLBACK CTexture::Free()
{
	bool can_delete;

	Core()->pResMan()->RemoveResource(this, can_delete);

	if (can_delete)
	{
		delete this;
		return S_OK;
	}
	else
		return S_FALSE;
}

HRESULT CALLBACK CTexture::GetType(E_ENG_OBJ_TYPE &eObjType)
{
	eObjType = EOT_TEXTURE;
	return S_OK;
}

HRESULT CALLBACK CTexture::GetUnknownType(uint &uiObjUnknownType)
{
	uiObjUnknownType = -1;
	return S_FALSE;
}