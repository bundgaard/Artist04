#pragma once
#include <dwrite.h>
#include <d2d1.h>

class GdiText : public IDWriteTextRenderer
{
	IDWriteBitmapRenderTarget* m_rt;
	IDWriteRenderingParams* m_renderParams;
public:
	GdiText(IDWriteBitmapRenderTarget* rt, IDWriteRenderingParams *rp) : m_rt(rt), m_renderParams(rp)
	{

	}
	// IDWritePixelSnapping methods
	IFACEMETHOD(IsPixelSnappingDisabled)(
		void* clientDrawingContext,
		OUT BOOL* isDisabled
		)
	{
		*isDisabled = FALSE;
		return S_OK;
	}
	IFACEMETHOD(GetCurrentTransform)(
		void* clientDrawingContext,
		OUT DWRITE_MATRIX* transform
		) {

		m_rt->GetCurrentTransform(transform);

		return S_OK;
	}
	IFACEMETHOD(GetPixelsPerDip)(
		void* clientDrawingContext,
		OUT FLOAT* pixelsPerDip
		) {
		return S_OK;
	}
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject)
	{
		*ppvObject = NULL;
		return E_NOTIMPL;
	};

	virtual ULONG STDMETHODCALLTYPE AddRef(void)
	{
		return 0;
	};

	virtual ULONG STDMETHODCALLTYPE Release(void)
	{
		return 0;
	};

	IFACEMETHOD(DrawGlyphRun)(
		void* clientDrawingContext,
		FLOAT baselineOriginX,
		FLOAT baselineOriginY,
		DWRITE_MEASURING_MODE measuringMode,
		DWRITE_GLYPH_RUN const* glyphRun,
		DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
		IUnknown* clientDrawingEffect
		)
	{

		return m_rt->DrawGlyphRun(
			baselineOriginX, 
			baselineOriginY, 
			measuringMode, 
			glyphRun, 
			m_renderParams,
			RGB(255,0,0));
	}

	IFACEMETHOD(DrawUnderline)(
		_In_opt_ void* clientDrawingContext,
		FLOAT baselineOriginX,
		FLOAT baselineOriginY,
		_In_ DWRITE_UNDERLINE const* underline,
		_In_opt_ IUnknown* clientDrawingEffect
		) {
		return E_NOTIMPL;
	};

	IFACEMETHOD(DrawStrikethrough)(
		_In_opt_ void* clientDrawingContext,
		FLOAT baselineOriginX,
		FLOAT baselineOriginY,
		_In_ DWRITE_STRIKETHROUGH const* strikethrough,
		_In_opt_ IUnknown* clientDrawingEffect
		) {
		return E_NOTIMPL;
	};

	IFACEMETHOD(DrawInlineObject)(
		_In_opt_ void* clientDrawingContext,
		FLOAT originX,
		FLOAT originY,
		_In_ IDWriteInlineObject* inlineObject,
		BOOL isSideways,
		BOOL isRightToLeft,
		_In_opt_ IUnknown* clientDrawingEffect
		)
	{
		return E_NOTIMPL;
	}
	;
};

