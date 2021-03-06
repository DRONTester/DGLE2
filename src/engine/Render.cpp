/**
\author		Korotkov Andrey aka DRON
\date		29.04.2012 (c)Korotkov Andrey

This file is a part of DGLE2 project and is distributed
under the terms of the GNU Lesser General Public License.
See "DGLE2.h" for more details.
*/

#include "Render.h"
#include "Render2D.h"

CRender::CRender(uint uiInstIdx):
CInstancedObj(uiInstIdx),
_fFovAngle(60.f), _fZNear(0.25f), _fZFar(1000.f)
{
	_pCoreRenderer = Core()->pCoreRenderer();

	bool b_supported;

	std::string flist = "Core Renderer supported features\n\n\t";

	_pCoreRenderer->IsFeatureSupported(CRSF_BUILTIN_FSCREEN_MODE, b_supported);
	flist += std::string("Hardware FScreen: ") + (b_supported ? "Yes" : "No") + "\n\t";

	_pCoreRenderer->IsFeatureSupported(CRSF_BUILTIN_STATE_FILTER, b_supported);
	flist += std::string("Builtin State Filter: ") + (b_supported ? "Yes" : "No") + "\n\t";

	_pCoreRenderer->IsFeatureSupported(CRSF_MULTISAMPLING, b_supported);
	flist += std::string("Multisampling: ") + (b_supported ? "Yes" : "No") + "\n\t";
	
	_pCoreRenderer->IsFeatureSupported(CRDF_VSYNC, b_supported);
	flist += std::string("VSync: ") + (b_supported ? "Yes" : "No") + "\n\t";

	_pCoreRenderer->IsFeatureSupported(CRDF_PROGRAMMABLE_PIPELINE, b_supported);
	flist += std::string("Programmable pipeline: ") + (b_supported ? "Yes" : "No") + "\n\t";

	_pCoreRenderer->IsFeatureSupported(CRSF_BGRA_DATA_FORMAT, b_supported);
	flist += std::string("BGRA textures: ") + (b_supported ? "Yes" : "No") + "\n\t";

	_pCoreRenderer->IsFeatureSupported(CRSF_TEXTURE_COMPRESSION, b_supported);
	flist += std::string("Texture compression: ") + (b_supported ? "Yes" : "No") + "\n\t";

	_pCoreRenderer->IsFeatureSupported(CRSF_NON_POWER_OF_TWO_TEXTURES, b_supported);
	flist += std::string("Non power of two textures: ") + (b_supported ? "Yes" : "No") + "\n\t";

	_pCoreRenderer->IsFeatureSupported(CRSF_DEPTH_TEXTURES, b_supported);
	flist += std::string("Depth textures: ") + (b_supported ? "Yes" : "No") + "\n\t";

	_pCoreRenderer->IsFeatureSupported(CRSF_TEXTURE_ANISOTROPY, b_supported);
	flist += std::string("Texture anisotropy: ") + (b_supported ? "Yes" : "No") + "\n\t";

	_pCoreRenderer->IsFeatureSupported(CRSF_TEXTURE_MIPMAP_GENERATION, b_supported);
	flist += std::string("Hardware mipmap generation: ") + (b_supported ? "Yes" : "No") + "\n\t";

	_pCoreRenderer->IsFeatureSupported(CRDF_TEXTURE_MIRRORED_REPEAT, b_supported);
	flist += std::string("Texture mirrored repeat: ") + (b_supported ? "Yes" : "No") + "\n\t";

	_pCoreRenderer->IsFeatureSupported(CRDF_TEXTURE_MIRROR_CLAMP, b_supported);
	flist += std::string("Texture mirror clamp: ") + (b_supported ? "Yes" : "No") + "\n\t";

	_pCoreRenderer->IsFeatureSupported(CRDF_GEOMETRY_BUFFER, b_supported);
	flist += std::string("Geometry buffer: ") + (b_supported ? "Yes" : "No") + "\n\t";

	LOG(flist, LT_INFO);

	std::string mlist = "Core Renderer device metrics\n\n\t";

	_pCoreRenderer->GetDeviceMetric(CRMT_MAX_TEXTURE_RESOLUTION, _iMaxTexResolution);
	mlist += "Maximum texture resolution: " + IntToStr(_iMaxTexResolution) + "X" + IntToStr(_iMaxTexResolution) + "\n\t";

	_pCoreRenderer->IsFeatureSupported(CRSF_TEXTURE_ANISOTROPY, b_supported);
	if (b_supported)
	{
		_pCoreRenderer->GetDeviceMetric(CRMT_MAX_ANISOTROPY_LEVEL, _iMaxAnisotropy);
		mlist += "Maximum anisotropy level: " + IntToStr(_iMaxAnisotropy) + "X" + "\n\t";
	}

	_pCoreRenderer->GetDeviceMetric(CRMT_MAX_LIGHTS_PER_PASS, _iMaxLight);

	_pCoreRenderer->GetDeviceMetric(CRMT_MAX_TEXTURE_LAYERS, _iMaxTexUnits);
	mlist += "Maximum multitexturing layer: " + IntToStr(_iMaxTexUnits) + "\n\t";

	LOG(mlist, LT_INFO);

	SetClearColor(TColor4(0x64646400));

	_pRender2D = new CRender2D(InstIdx());

	LOG("Render subsystem initialized.", LT_INFO);
}

CRender::~CRender()
{
	delete _pRender2D;

	LOG("Render subsystem finalized.", LT_INFO);
}

void CRender::BeginRender()
{
	_pCoreRenderer->Clear();
	_pCoreRenderer->SetMatrix(MatrixIdentity());
}

void CRender::EndRender()
{
	_pCoreRenderer->Present();
}

void CRender::_SetPerspectiveMatrix(uint width, uint height)
{
	float aspect = (float)width/(float)height,
	top = _fZNear * tanf(_fFovAngle * (float)M_PI / 360.f),
	bottom = -top,
	left = bottom * aspect,
	right = top * aspect;

	_pCoreRenderer->SetMatrix(TMatrix(
		(2*_fZNear)/(right - left), 0.f, (right + left)/(right - left), 0.f,
		0.f, (2*_fZNear)/(top - bottom), (top + bottom)/(top - bottom), 0.f,
		0.f, 0.f, -((_fZFar + _fZNear)/(_fZFar - _fZNear)), -1.f,
		0.f, 0.f, -((2*_fZFar*_fZNear)/(_fZFar - _fZNear)), 0.f
		), MT_PROJECTION);
}

void CRender::OnResize(uint uiWidth, uint uiHeight)
{
	_pCoreRenderer->SetViewport(0, 0, uiWidth, uiHeight);						

	_SetPerspectiveMatrix(uiWidth, uiHeight);

	_pCoreRenderer->SetMatrix(MatrixIdentity());
}

HRESULT CALLBACK CRender::SetClearColor(const TColor4 &stColor)
{
	_pCoreRenderer->SetClearColor(stColor);
	return S_OK;
}

HRESULT CALLBACK CRender::Unbind(E_ENG_OBJ_TYPE eType)
{
	switch (eType)
	{
	case EOT_UNKNOWN:
		Unbind(EOT_MATERIAL);
		Unbind(EOT_TEXTURE);
		Unbind(EOT_MESH);
		break;
	case EOT_TEXTURE:
		for (int i = _iMaxTexUnits - 1; i >= 0; i--)
			_pCoreRenderer->BindTexture(NULL, i);
		break;
	case EOT_MESH:
	case EOT_MODEL:
		_pCoreRenderer->DrawBuffer(NULL);
		break;
	case EOT_MATERIAL:
		//ToDo
		break;
	default:
		return E_INVALIDARG;
	}

	return S_OK;
}

HRESULT CALLBACK CRender::EnableScissor(const TRectF &stArea)
{
	TRasterizerStateDesc desc;
	_pCoreRenderer->GetRasterizerState(desc);
	desc.bScissorEnable = true;
	_pCoreRenderer->SetRasterizerState(desc);
	_pCoreRenderer->SetScissor((uint)stArea.x, (uint)stArea.y, (uint)stArea.width, (uint)stArea.height);
	return S_OK;
}

HRESULT CALLBACK CRender::DisableScissor()
{
	TRasterizerStateDesc desc;
	_pCoreRenderer->GetRasterizerState(desc);
	desc.bScissorEnable = false;
	_pCoreRenderer->SetRasterizerState(desc);
	return S_OK;
}

HRESULT CALLBACK CRender::SetRenderTarget(ITexture* pTargetTex)
{
	//ToDo
	return S_OK;
}

HRESULT CALLBACK CRender::ScreenshotBMP(const char* pFileName)
{
	//ToDo
	return S_OK;
}

HRESULT CALLBACK CRender::CreatePostProcess(IPostprocess *&pPP)
{
	//ToDo
	return S_OK;
}

HRESULT CALLBACK CRender::GetRender2D(IRender2D *&prRender2D)
{
	prRender2D = _pRender2D;
	return S_OK;
}

HRESULT CALLBACK CRender::GetRender3D(IRender3D *&prRender3D)
{
	return S_OK;
}

HRESULT CALLBACK CRender::GetType(E_ENGINE_SUB_SYSTEM &eSubSystemType)
{
	eSubSystemType = ESS_RENDER;
	return S_OK;
}