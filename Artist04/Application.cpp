#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <dwrite.h>
#pragma comment(lib, "dwrite")
#include "GdiText.h"
#include <iostream>
#include <string>

IMAGE_DOS_HEADER __ImageBase;
#define THIS_HINSTCOMPONENT ((HINSTANCE)&__ImageBase)
#define local_persist static
#define internal static
#define global static

constexpr auto CLASSNAME = L"CArtistWindow04";
using namespace std;

global IDWriteFactory* pWriteFactory = nullptr;
global IDWriteGdiInterop* pGdiInterop = nullptr;
global IDWriteBitmapRenderTarget* pWriteBitmapRender = nullptr;
global IDWriteRenderingParams* pWriteRenderParams = nullptr;
global IDWriteTextFormat* pWriteTextFormat = nullptr;
global IDWriteTextLayout* pWriteTextLayout = nullptr;
global IDWriteTypography* pWriteTypography = nullptr;

internal void 
Win32ReleaseDWrite()
{
	if (pWriteFactory)
		pWriteFactory->Release();
	if (pGdiInterop)
		pGdiInterop->Release();
	if (pWriteBitmapRender)
		pWriteBitmapRender->Release();
	if (pWriteRenderParams)
		pWriteRenderParams->Release();
	if (pWriteTextFormat)
		pWriteTextFormat->Release();
	if (pWriteTextLayout)
		pWriteTextLayout->Release();
	if (pWriteTypography)
		pWriteTypography->Release();

}
internal void
Win32CreateDIB(int Width, int Height)
{
	BITMAPINFO bi{};
	bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
	bi.bmiHeader.biWidth = Width;
	bi.bmiHeader.biHeight = -Height;
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biPlanes = 1;

	HDC hdc = CreateCompatibleDC(nullptr);
	// CreateDIBSection(hdc, &bi, DIB_RGB_COLORS, )
}
internal void 
Win32CreateRenderTarget(HWND hwnd)
{
	// TODO(david): move out as it is dependent on device
	HRESULT hr = S_OK;
	UINT Dpi = GetDpiForWindow(hwnd);
	RECT ClientRect{};
	int Width = (ClientRect.right - ClientRect.left) * Dpi / 96;
	int Height = (ClientRect.bottom - ClientRect.top) * Dpi / 96;

	if (SUCCEEDED(hr))
	{
		hr = pGdiInterop->CreateBitmapRenderTarget(nullptr,
			Width,
			Height,
			&pWriteBitmapRender
		);
	}

	
}
internal LRESULT CALLBACK
Win32WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		
	}
	return 0;
	case WM_SIZE:
	{
		HRESULT hr = S_OK;
		auto Dpi = GetDpiForWindow(hwnd);
		RECT ClientRect{};
		GetClientRect(hwnd, &ClientRect);
		int Width = (ClientRect.right - ClientRect.left) * Dpi / 96;
		int Height = (ClientRect.bottom - ClientRect.top) * Dpi / 96;
		pWriteBitmapRender->Resize(Width, Height);
		if (SUCCEEDED(hr))
		{
			if (pWriteTextLayout)
			{
				pWriteTextLayout->Release();
			}

			std::wstring text{ 
				L"package main\n"
				L"import (\n"
				L"\t\"fmt\"\n"
				L")\n"
			};
			hr = pWriteFactory->CreateTextLayout(
				text.c_str(),
				text.size(),
				pWriteTextFormat,
				Width,
				Height,
				&pWriteTextLayout
			);
		}
		GdiText aTextRenderer(pWriteBitmapRender, pWriteRenderParams);
		
		PatBlt(pWriteBitmapRender->GetMemoryDC(), 0, 0, Width, Height, BLACKNESS);

		if (SUCCEEDED(hr))
		{
			hr = pWriteTextLayout->Draw(nullptr, &aTextRenderer, 6.0f, 5.0f);
		}
	}
	return 0;
	case WM_PRINTCLIENT:
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
	
		UINT Dpi = GetDpiForWindow(hwnd);
		int Width = (ps.rcPaint.right - ps.rcPaint.left) * Dpi / 96;
		int Height = (ps.rcPaint.bottom - ps.rcPaint.top) * Dpi / 96;
		HDC memdc = CreateCompatibleDC(0);
		HBITMAP memBm = CreateCompatibleBitmap(memdc, 0,0);
		SelectObject(memdc, memBm);
		BitBlt(memdc, 0, 0, Width, Height, pWriteBitmapRender->GetMemoryDC(), 0, 0, SRCCOPY);



		BitBlt(hdc, 0, 0, Width, Height, memdc, 0, 0, SRCCOPY);
		EndPaint(hwnd, &ps);
	}
	return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return DefWindowProcW(hwnd, msg, wparam, lparam);
}

internal bool
Win32Register()
{
	WNDCLASSEX wc{};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = THIS_HINSTCOMPONENT;
	wc.lpszClassName = CLASSNAME;
	wc.lpfnWndProc = &Win32WndProc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.hbrBackground = (HBRUSH)GetStockObject(COLOR_APPWORKSPACE + 1);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpszMenuName = nullptr;

	return RegisterClassEx(&wc);
}

internal HWND Win32CreateWindow(std::wstring const& Title)
{
	HWND hwnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		CLASSNAME,
		Title.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr,
		nullptr,
		THIS_HINSTCOMPONENT,
		nullptr);
	return hwnd;
}
internal void 
Win32CreateDWrite(HWND hwnd)
{
	HRESULT hr = S_OK;


	if (SUCCEEDED(hr))
	{
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED, 
			__uuidof(IDWriteFactory),
			(IUnknown**)&pWriteFactory);
	}
	if (SUCCEEDED(hr))
	{
		hr = pWriteFactory->GetGdiInterop(&pGdiInterop);
	}

	if (SUCCEEDED(hr))
	{
		hr = pWriteFactory->CreateRenderingParams(&pWriteRenderParams);
	}
	if (SUCCEEDED(hr))
	{
		hr = pWriteFactory->CreateTypography(&pWriteTypography);
	}

	if (SUCCEEDED(hr))
	{
		hr = pWriteTypography->AddFontFeature(DWRITE_FONT_FEATURE{ .nameTag = DWRITE_FONT_FEATURE_TAG_SLASHED_ZERO, .parameter = 1 });
		
	}
	if (SUCCEEDED(hr))
	{
		hr = pWriteTypography->AddFontFeature(DWRITE_FONT_FEATURE{ .nameTag = DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_2, .parameter = 1 });
	}
	if (SUCCEEDED(hr))
	{

		hr = pWriteFactory->CreateTextFormat(
			L"Anvylon",
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			72.5f,
			L"",
			&pWriteTextFormat
		);
	}
	
}
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{


	if (!Win32Register())
	{
		MessageBoxW(nullptr, L"Failed to register class", L"Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	HWND hwnd = Win32CreateWindow(L"Artist 04");

	Win32CreateDWrite(hwnd);
	Win32CreateRenderTarget(hwnd);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	MSG msg;
	BOOL bRet;
	while (bRet = GetMessageW(&msg, nullptr, 0, 0) > 0)
	{
		if (bRet == -1)
		{
			// TODO(david): Log
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	Win32ReleaseDWrite();
	return (int)msg.wParam;
}