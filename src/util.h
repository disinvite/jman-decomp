#ifndef UTIL_H
#define UTIL_H

#include <afxwin.h>
#include <windows.h>

extern int supportFileNameBufIndex;
extern char supportFileBasePath[256];
extern char supportFileNameBufs[5][256];

char *GetPathToSupportFile(const char *filename);
BOOL SetupPalette(CWnd *cf);
HPALETTE GetCurrentPal(void);
BOOL Startup_DialogHiColorWarning(void);
BOOL Startup_BasePathWarning(void);

// drawing functions?
BOOL HDC_FUN_1008_41a0(CWnd *cf, char *filename);
BOOL HDC_FUN_1008_424c(CWnd *cf);
BOOL HDC_FUN_1008_42fa(CWnd *cf);
void HDC_FUN_1008_44b0(HDC hdc, HBITMAP hBitmap, int x, int y);
void HDC_FUN_1008_453e(HDC hdc, int dx, int dy, int width, int height,
					   HBITMAP hBitmap, int sx, int sy);
BOOL HDC_FUN_1008_46f6(HDC hdc, HBITMAP bmp_src,
					   int sx, int sy, int width, int height, HBITMAP bmp_dst,
					   int dx, int dy, BOOL dissolve, COLORREF color);
BOOL HDC_FUN_1008_47a8(HDC hdc, HWND hwnd, HBITMAP bmp, int x, int y, DWORD, int);
BOOL hdc_UpdateHDC(HWND hwnd, HDC hdc, int x, int y, DWORD, int);
BOOL HDC_FUN_1008_4876(HWND hwnd, HDC hdc, int dx, int dy, int width, int height,
					   HBITMAP argbmp, int sx, int sy, BYTE b0, DWORD duration);

// DIB functions

typedef HANDLE HDIB;

HDIB ReadDibBitmapInfo(int fh);
HDIB OpenDIB(LPSTR szFile);
HBITMAP BitmapFromDib(HDIB hdib, HPALETTE hpal, WORD wUsage);
HPALETTE CreateBIPalette(LPBITMAPINFOHEADER lpbi);

// DIB macros

#define DrawDib(hdc, x, y, hdib, hpal, wUsage) StretchDib(hdc, x, y, -1, -1, hdib, hpal, 0, 0, -1, -1, SRCCOPY, wUsage)
#define CreateDibPalette(hdib) CreateBIPalette((LPBITMAPINFOHEADER)GlobalLock(hdib))

#define ALIGNULONG(i) ((i + 3) / 4 * 4)                  /* ULONG aligned ! */
#define WIDTHBYTES(i) ((unsigned)((i + 31) & (~31)) / 8) /* ULONG aligned ! */

#define DIBWIDTHBYTES(bi) DibWidthBytes(&bi)

#define DibWidthBytesN(lpbi, n) (UINT) WIDTHBYTES((UINT)(lpbi)->biWidth *(UINT)(n))
#define DibWidthBytes(lpbi) DibWidthBytesN(lpbi, (lpbi)->biBitCount)

#define DibSizeImage(lpbi) ((lpbi)->biSizeImage == 0                                                 \
								? ((DWORD)(UINT)DibWidthBytes(lpbi) * (DWORD)(UINT)(lpbi)->biHeight) \
								: (lpbi)->biSizeImage)

#define DibSize(lpbi) ((lpbi)->biSize + (lpbi)->biSizeImage + (int)(lpbi)->biClrUsed * sizeof(RGBQUAD))

#define DibPtr(lpbi) (LPVOID)(DibColors(lpbi) + (UINT)(lpbi)->biClrUsed)
#define DibColors(lpbi) ((LPRGBQUAD)((LPBYTE)(lpbi) + (int)(lpbi)->biSize))

#define DibNumColors(lpbi) ((lpbi)->biClrUsed == 0 && (lpbi)->biBitCount <= 8 \
								? (int)(1 << (int)(lpbi)->biBitCount)         \
								: (int)(lpbi)->biClrUsed)

#define DibXYN(lpbi, pb, x, y, n) (LPVOID)( \
	(PBYTE)(pb) +                           \
	(UINT)((UINT)(x) * (UINT)(n) / 8u) +    \
	((DWORD)DibWidthBytesN(lpbi, n) * (DWORD)(UINT)(y)))

#define DibXY(lpbi, x, y) DibXYN(lpbi, DibPtr(lpbi), x, y, (lpbi)->biBitCount)

#define FixBitmapInfo(lpbi)                       \
	if ((lpbi)->biSizeImage == 0)                 \
		(lpbi)->biSizeImage = DibSizeImage(lpbi); \
	if ((lpbi)->biClrUsed == 0)                   \
		(lpbi)->biClrUsed = DibNumColors(lpbi);

#endif
