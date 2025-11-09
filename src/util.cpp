#include "util.h"

#include <afxwin.h>
#include <vfw.h>
#include <windows.h>

#include "dialog.h"

int supportFileNameBufIndex;
char supportFileBasePath[256];
char supportFileNameBufs[5][256];
HPALETTE current_pal;
HPALETTE current_pal2;

static char _far kCDNotFound[] = "The Journeyman Project Turbo CD Not Found";
static char _far kJmanConfNotFound[] = "Journeyman Configuration File Not Found";
static char _far kPathConfNotFound[] = "Path Configuration Not Found";
static char _far kJmanIni[] = "JMAN.INI";

// FUNCTION: JMAN10 0x10081542
char *GetPathToSupportFile(const char *filename) {
	supportFileNameBufIndex++;
	if (supportFileNameBufIndex >= 4) {
		supportFileNameBufIndex = 0;
	}

	char *buf = supportFileNameBufs[supportFileNameBufIndex];
	if ((lstrlen(supportFileBasePath) + lstrlen(filename)) > 256) {
		return NULL;
	}

	lstrcpy(buf, supportFileBasePath);
	lstrcat(buf, filename);

	return buf;
}

// FUNCTION: JMAN10 0x100840e6
BOOL SetupPalette(CWnd *cf) {
	HDIB dib = OpenDIB(GetPathToSupportFile("SUPPORT\\FULLSCRN.BMP"));
	current_pal = CreateDibPalette(dib);
	GlobalUnlock(dib);
	GlobalFree(dib);

	CDC *cdc = cf->GetDC();
	current_pal2 = ::SelectPalette(cdc->m_hDC, current_pal, NULL);

	SetSystemPaletteUse(cdc->m_hDC, SYSPAL_NOSTATIC);
	UnrealizeObject(current_pal);
	cdc->RealizePalette();
	cf->ReleaseDC(cdc);
	SendMessage((HWND)-1, WM_SYSCOLORCHANGE, 0, 0);

	return TRUE;
}

// FUNCTION: JMAN10 0x10084386
HPALETTE GetCurrentPal(void) {
	return current_pal;
}

// STUB: JMAN10 0x10081212
BOOL CheckMinimumSupportFiles(const char *basePath) {
	return TRUE;
}

// STUB: JMAN10 0x10081122
BOOL Startup_DialogHiColorWarning() {
	// HWND window = GetDesktopWindow();
	// CDC *pDC = GetDC(window);
	/*
	CDC *pDC = GetDC(GetDesktopWindow());
	int n_caps = pDC->GetDeviceCaps(RASTERCAPS) & 256;
	int n_colors = pDC->GetDeviceCaps(NUMCOLORS);
	int n_something = pDC->GetDeviceCaps(NUMCOLORS);
	::ReleaseDC(pDC);
	*/
	return TRUE;

	HWND hwnd = GetDesktopWindow();
	HDC hdc = GetDC(hwnd);

	int n_caps = GetDeviceCaps(hdc, RASTERCAPS) & 256;
	int n_colors = GetDeviceCaps(hdc, NUMCOLORS);
	int n_something = GetDeviceCaps(hdc, NUMCOLORS);

	if (GetSystemMetrics(SM_CXSCREEN) > 640 &&
		GetSystemMetrics(SM_CYSCREEN) > 480) {

		GetPrivateProfileInt("Journeyman", "HiColorWarning", 0, kJmanIni);
		CDialog(RES_HICOLORWARNING).DoModal();
	} else {
		CDialog(RES_HICOLORERROR).DoModal();
	}

	return TRUE;
}

// FUNCTION: JMAN10 0x100813b6
BOOL Startup_BasePathWarning(void) {
	GetPrivateProfileString("Journeyman", "Base Path", "",
							supportFileBasePath, sizeof(supportFileBasePath), kJmanIni);

	// if no base path found in the INI file
	if (supportFileBasePath[0] == 0) {
		BasePathDialog dlg;

		// TODO: what is this?
		if (kJmanConfNotFound) {
			lstrcpy(dlg._header_text, kJmanConfNotFound);
		} else {
			lstrcpy(dlg._header_text, kPathConfNotFound);
		}

		// if user didn't click okay?
		if (dlg.DoModal() == 2) {
			return FALSE;
		}

		WritePrivateProfileString("Journeyman", "Base Path",
								  supportFileBasePath, kJmanIni);
	}

	if (!CheckMinimumSupportFiles(supportFileBasePath)) {
		do {
			BasePathDialog dlg;

			// TODO: what is this?
			if (kCDNotFound) {
				lstrcpy(dlg._header_text, kCDNotFound);
			} else {
				lstrcpy(dlg._header_text, kPathConfNotFound);
			}

			// if user didn't click okay?
			if (dlg.DoModal() == 2) {
				return FALSE;
			}

			WritePrivateProfileString("Journeyman", "Base Path",
									  supportFileBasePath, kJmanIni);
		} while (!CheckMinimumSupportFiles(supportFileBasePath));
	}

	return TRUE;
}

// set palette from bmp?
// FUNCTION: JMAN10 0x100841a0
BOOL HDC_FUN_1008_41a0(CWnd *cf, char *filename) {
	HDIB dib = OpenDIB(filename);
	HPALETTE pal = CreateDibPalette(dib);
	GlobalUnlock(dib);
	GlobalFree(dib);

	CDC *cdc = cf->GetDC();
	::SelectPalette(cdc->m_hDC, pal, NULL);

	SetSystemPaletteUse(cdc->m_hDC, SYSPAL_NOSTATIC);
	UnrealizeObject(current_pal);
	cdc->RealizePalette();
	cf->ReleaseDC(cdc);

	if (current_pal != NULL) {
		DeleteObject(current_pal);
		current_pal = NULL;
	}
	current_pal = pal;

	return TRUE;
}

// delete first palette?
// FUNCTION: JMAN10 0x1008424c
BOOL HDC_FUN_1008_424c(CWnd *cf) {
	HDC_FUN_1008_42fa(cf);
	DeleteObject(current_pal);
	current_pal = 0;
	return TRUE;
}

// delete second palette?
// FUNCTION: JMAN10 0x100842fa
BOOL HDC_FUN_1008_42fa(CWnd *cf) {
	if (current_pal2 == 0) {
		return FALSE;
	}

	CDC *cdc = cf->GetDC();
	::SelectPalette(cdc->m_hDC, current_pal2, NULL);

	SetSystemPaletteUse(cdc->m_hDC, SYSPAL_STATIC);
	UnrealizeObject(current_pal2);
	cdc->RealizePalette();
	cf->ReleaseDC(cdc);
	SendMessage((HWND)-1, WM_SYSCOLORCHANGE, 0, 0);

	return TRUE;
}

// FUNCTION: JMAN10 0x100844b0
void HDC_FUN_1008_44b0(HDC hdc, HBITMAP hBitmap, int x, int y) {
	HDC hMemDC = CreateCompatibleDC(hdc);
	SelectObject(hMemDC, (HGDIOBJ)hBitmap);
	SetMapMode(hMemDC, GetMapMode(hdc));

	BITMAP bmp;
	::GetObject(hBitmap, sizeof(BITMAP), &bmp);

	POINT pt1 = {bmp.bmWidth, bmp.bmHeight};
	::DPtoLP(hdc, &pt1, 1);

	POINT pt2 = {0, 0};
	::DPtoLP(hMemDC, &pt2, 1);

	BitBlt(hdc, x, y, pt1.x, pt1.y, hMemDC, pt2.x, pt2.y, SRCCOPY);
	DeleteDC(hMemDC);
}

// FUNCTION: JMAN10 0x1008453e
void HDC_FUN_1008_453e(HDC hdc, int dx, int dy, int width, int height,
					   HBITMAP hBitmap, int sx, int sy) {

	HDC hMemDC = CreateCompatibleDC(hdc);
	SelectObject(hMemDC, (HGDIOBJ)hBitmap);
	BitBlt(hdc, dx, dy, width, height, hMemDC, sx, sy, SRCCOPY);
	DeleteDC(hMemDC);
}

// dissolving between two images?
// Good except for XOR. macro to invert colors?
// FUNCTION: JMAN10 0x10084580
BOOL HDC_FUN_1008_4580(HDC hdc, int dx, int dy, int width, int height,
					   HDC hdc2, int sx, int sy, COLORREF color) {

	HBITMAP hbmp = CreateCompatibleBitmap(hdc, width, height);
	HDC hMemDC = CreateCompatibleDC(hdc);
	HGDIOBJ oldobj = SelectObject(hMemDC, (HGDIOBJ)hbmp);

	HBITMAP newbmp = CreateBitmap(width, height, 1, 1, NULL);

	HDC hMemDC2 = CreateCompatibleDC(hdc);
	HGDIOBJ oldobj2 = SelectObject(hMemDC2, (HGDIOBJ)newbmp);

	BitBlt(hMemDC, 0, 0, width, height, hdc, dx, dy, SRCCOPY);
	BitBlt(hMemDC2, 0, 0, width, height, hdc2, sx, sy, SRCCOPY);

	COLORREF bk = SetBkColor(hMemDC, color);
	// TODO: XOR wrong here.
	COLORREF fk = SetTextColor(hMemDC, color ^ -1);

	BitBlt(hMemDC, 0, 0, width, height, hdc2, sx, sy, SRCINVERT);
	BitBlt(hMemDC, 0, 0, width, height, hMemDC2, 0, 0, SRCAND);
	BitBlt(hMemDC, 0, 0, width, height, hdc2, sx, sy, SRCINVERT);

	SetTextColor(hMemDC, fk);
	SetBkColor(hMemDC, bk);

	BitBlt(hdc, dx, dy, width, height, hMemDC, 0, 0, SRCCOPY);

	SelectObject(hMemDC2, oldobj2);
	DeleteObject(newbmp);
	DeleteDC(hMemDC2);

	SelectObject(hMemDC, oldobj);
	DeleteObject(hbmp);
	DeleteDC(hMemDC);

	return TRUE;
}

// FUNCTION: JMAN10 0x100846f6
BOOL HDC_FUN_1008_46f6(HDC hdc, HBITMAP bmp_src,
					   int sx, int sy, int width, int height, HBITMAP bmp_dst,
					   int dx, int dy, BOOL dissolve, COLORREF color) {
	if (!hdc) {
		return FALSE;
	}

	if (!bmp_dst) {
		return FALSE;
	}

	if (!bmp_src) {
		return FALSE;
	}

	HDC hDC_src = CreateCompatibleDC(hdc);
	HGDIOBJ oldbmp0 = SelectObject(hDC_src, (HGDIOBJ)bmp_src);
	HDC hDC_dst = CreateCompatibleDC(hdc);
	HGDIOBJ oldbmp1 = SelectObject(hDC_dst, (HGDIOBJ)bmp_dst);

	if (!dissolve) {
		BitBlt(hDC_dst, dx, dy, width, height,
			   hDC_src, sx, sy, SRCCOPY);
	} else {
		HDC_FUN_1008_4580(hDC_dst, dx, dy, width, height,
						  hDC_src, sx, sy, color);
	}

	SelectObject(hDC_src, oldbmp0);
	DeleteDC(hDC_src);

	SelectObject(hDC_dst, oldbmp1);
	DeleteDC(hDC_dst);

	return TRUE;
}

// FUNCTION: JMAN10 0x100847a8
BOOL HDC_FUN_1008_47a8(HDC hdc, HWND hwnd, HBITMAP bmp, int x, int y, DWORD, int) {
	HDC hdc2 = CreateCompatibleDC(hdc);
	HGDIOBJ tObj = SelectObject(hdc2, bmp);
	CPoint pt = SetViewportOrg(hdc2, x, y);

	char buf[512];
	wsprintf(buf, "update hdc %u", hdc2);
	MCIWndSendString(hwnd, buf);

	SetViewportOrg(hdc2, pt.x, pt.y);
	SelectObject(hdc2, tObj);
	DeleteDC(hdc2);

	return TRUE;
}

// two unused parameters.
// FUNCTION: JMAN10 0x10084820
BOOL hdc_UpdateHDC(HWND hwnd, HDC hdc, int x, int y, DWORD, int) {
	CPoint pt = SetViewportOrg(hdc, x, y);

	char buf[512];
	wsprintf(buf, "update hdc %u", hdc);
	MCIWndSendString(hwnd, buf);
	SetViewportOrg(hdc, pt.x, pt.y);

	return TRUE;
}

// FUNCTION: JMAN10 0x10084876
BOOL HDC_FUN_1008_4876(HWND hwnd, HDC hdc, int dx, int dy, int width, int height,
					   HBITMAP argbmp, int sx, int sy, BYTE b0, DWORD duration) {

	HBITMAP hbmp = CreateCompatibleBitmap(hdc, width, height);
	HDC hdc28 = CreateCompatibleDC(hdc);
	HGDIOBJ selectObj2c = SelectObject(hdc28, (HGDIOBJ)hbmp);

	HBITMAP createBitmap24 = CreateBitmap(width, height, 1, 1, NULL);

	HDC hdc26 = CreateCompatibleDC(hdc);
	HGDIOBJ selectObj2e = SelectObject(hdc26, (HGDIOBJ)createBitmap24);

	BITMAP mybmp;
	::GetObject(createBitmap24, sizeof(BITMAP), &mybmp);

	DWORD n_bits = ((DWORD)mybmp.bmHeight * (DWORD)mybmp.bmWidthBytes) * 8;

	HGLOBAL gBitBuf = ::GlobalAlloc(GHND, n_bits + 1);
	BYTE *bitBuf = (BYTE *)::GlobalLock(gBitBuf);

	HGLOBAL gByteBuf = ::GlobalAlloc(GHND, (n_bits + 1) / 8);
	BYTE *byteBuf = (BYTE *)::GlobalLock(gByteBuf);

	for (DWORD mysiz = 0; mysiz < GlobalSize(gByteBuf); mysiz++) {
		byteBuf[mysiz] = 0;
	}

	srand(LOWORD(GetCurrentTime()));

	for (mysiz = 0; mysiz < n_bits; mysiz++) {
		bitBuf[mysiz] = rand();

		while (bitBuf[mysiz] >= b0) {
			if (bitBuf[mysiz] & 1) {
				bitBuf[mysiz] /= 3;
			} else {
				bitBuf[mysiz] >>= 1;
			}
		}
	}

	HDC hdc16 = CreateCompatibleDC(hdc);
	HGDIOBJ selectObj22 = SelectObject(hdc16, argbmp);

	for (BYTE bs = 3; bs < b0; bs++) {

		DWORD timein = GetCurrentTime();

		for (mysiz = 0; mysiz < n_bits; mysiz++) {
			if (bitBuf[mysiz] <= bs)
				byteBuf[(mysiz - (mysiz & 7)) >> 3] |= (1 << (mysiz & 7));
		}

		SetBitmapBits(createBitmap24, ::GlobalSize(gByteBuf), byteBuf);
		COLORREF fk = SetTextColor(hdc28, RGB(255, 255, 255));
		COLORREF bk = SetBkColor(hdc28, NULL);

		BitBlt(hdc28, 0, 0, width, height, hdc, dx, dy, SRCCOPY);
		BitBlt(hdc28, 0, 0, width, height, hdc16, sx, sy, SRCINVERT);

		BitBlt(hdc28, 0, 0, width, height, hdc26, 0, 0, SRCAND);

		BitBlt(hdc28, 0, 0, width, height, hdc16, sx, sy, SRCINVERT);
		BitBlt(hdc, dx, dy, width, height, hdc28, 0, 0, SRCCOPY);

		SetTextColor(hdc28, fk);
		SetBkColor(hdc28, bk);

		while (GetCurrentTime() < timein + (duration / b0))
			;
	}

	if (::GlobalUnlock(gBitBuf))
		::GlobalFree(gBitBuf);

	::GlobalUnlock(gByteBuf);
	::GlobalFree(gByteBuf);

	SelectObject(hdc26, selectObj2e);
	DeleteDC(hdc26);
	DeleteDC(hdc); //?

	DeleteObject(createBitmap24);
	SelectObject(hdc28, selectObj2c);
	DeleteDC(hdc28);

	DeleteObject(hbmp);
	SelectObject(hdc16, selectObj22);
	DeleteDC(hdc16);

	return TRUE;
}

// MS DIB functions

/* flags for _lseek */
#define SEEK_CUR 1
#define SEEK_END 2
#define SEEK_SET 0

#define BFT_ICON 0x4349   /* 'IC' */
#define BFT_BITMAP 0x4d42 /* 'BM' */
#define BFT_CURSOR 0x5450 /* 'PT' */

#define ISDIB(bft) ((bft) == BFT_BITMAP)

#define PALVERSION 0x300
#define MAXPALETTE 256

// FUNCTION: JMAN10 0x10084c72
HDIB OpenDIB(LPSTR szFile) {
	int fh;
	LPBITMAPINFOHEADER lpbi;
	DWORD dwLen;
	HDIB hdib;
	HANDLE h;
	OFSTRUCT of;

	if (HIWORD((DWORD)szFile) == 0) {
		fh = LOWORD((DWORD)szFile);
	} else {
		fh = OpenFile(szFile, &of, OF_READ);
	}

	if (fh == -1)
		return NULL;

	hdib = ReadDibBitmapInfo(fh);

	if (!hdib)
		goto error;

	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hdib);

	/* How much memory do we need to hold the DIB */

	dwLen = DibSize(lpbi);

	/* Can we get more memory? */

	h = GlobalReAlloc(hdib, dwLen, 0);

	if (!h) {
		GlobalFree(hdib);
		hdib = NULL;
	} else {
		hdib = h;
	}

	if (hdib) {
		lpbi = (LPBITMAPINFOHEADER)GlobalLock(hdib);

		/* read in the bits */
		_hread(fh, DibPtr(lpbi), lpbi->biSizeImage);
	}

error:
	if (HIWORD((DWORD)szFile) != 0)
		_lclose(fh);

	return hdib;
}

// FUNCTION: JMAN10 0x10084d46
HPALETTE CreateBIPalette(LPBITMAPINFOHEADER lpbi) {
	LOGPALETTE *pPal;
	HPALETTE hpal = NULL;
	WORD nNumColors;
	int i;
	RGBQUAD FAR *pRgb;

	if (!lpbi)
		return NULL;

	nNumColors = DibNumColors(lpbi);

	if (nNumColors) {
		pRgb = DibColors(lpbi);
		pPal = (LOGPALETTE *)LocalAlloc(LPTR, sizeof(LOGPALETTE) + nNumColors * sizeof(PALETTEENTRY));

		if (!pPal)
			goto exit;

		pPal->palNumEntries = nNumColors;
		pPal->palVersion = PALVERSION;

		for (i = 0; i < (int)nNumColors; i++) {
			pPal->palPalEntry[i].peRed = pRgb->rgbRed;
			pPal->palPalEntry[i].peGreen = pRgb->rgbGreen;
			pPal->palPalEntry[i].peBlue = pRgb->rgbBlue;
			pPal->palPalEntry[i].peFlags = (BYTE)0;
			pRgb++;
		}

		hpal = CreatePalette(pPal);
		LocalFree((HANDLE)pPal);
	}

exit:
	return hpal;
}

// FUNCTION: JMAN10 0x10084e1e
HDIB ReadDibBitmapInfo(int fh) {
	DWORD off;
	HDIB hbi = NULL;
	int size;
	int i;
	WORD nNumColors;

	RGBQUAD FAR *pRgb;
	BITMAPINFOHEADER bi;
	BITMAPCOREHEADER bc;
	LPBITMAPINFOHEADER lpbi;
	BITMAPFILEHEADER bf;

	if (fh == -1)
		return NULL;

	off = _llseek(fh, 0L, SEEK_CUR);

	if (sizeof(bf) != _lread(fh, (LPVOID)&bf, sizeof(bf)))
		return NULL;

	/*
	 *  do we have a RC HEADER?
	 */
	if (!ISDIB(bf.bfType)) {
		bf.bfOffBits = 0L;
		_llseek(fh, off, SEEK_SET);
	}

	if (sizeof(bi) != _lread(fh, (LPVOID)&bi, sizeof(bi)))
		return NULL;

	nNumColors = DibNumColors(&bi);

	/*
	 *  what type of bitmap info is this?
	 */
	switch (size = (int)bi.biSize) {
	default:
	case sizeof(BITMAPINFOHEADER):
		break;

	case sizeof(BITMAPCOREHEADER):
		bc = *(BITMAPCOREHEADER *)&bi;
		bi.biSize = sizeof(BITMAPINFOHEADER);
		bi.biWidth = (DWORD)bc.bcWidth;
		bi.biHeight = (DWORD)bc.bcHeight;
		bi.biPlanes = (WORD)bc.bcPlanes;
		bi.biBitCount = (WORD)bc.bcBitCount;
		bi.biCompression = BI_RGB;
		bi.biSizeImage = 0;
		bi.biXPelsPerMeter = 0;
		bi.biYPelsPerMeter = 0;
		bi.biClrUsed = nNumColors;
		bi.biClrImportant = nNumColors;

		_llseek(fh, (LONG)sizeof(BITMAPCOREHEADER) - sizeof(BITMAPINFOHEADER), SEEK_CUR);

		break;
	}

	/*
	 *  fill in some default values!
	 */
	if (bi.biSizeImage == 0)
		bi.biSizeImage = DibSizeImage(&bi);

	if (bi.biClrUsed == 0)
		bi.biClrUsed = DibNumColors(&bi);

	hbi = GlobalAlloc(GMEM_MOVEABLE, (LONG)bi.biSize + nNumColors * sizeof(RGBQUAD));

	if (!hbi)
		return NULL;

	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hbi);
	*lpbi = bi;

	if (lpbi->biSize > sizeof(bi)) {
		if (_lread(fh,
				   (LPBYTE)lpbi + sizeof(bi),
				   (UINT)lpbi->biSize - sizeof(bi)) != (lpbi->biSize - sizeof(bi))) {
			GlobalFree(hbi);
			return NULL;
		}
	}

	pRgb = DibColors(lpbi);

	if (nNumColors) {
		if (size == sizeof(BITMAPCOREHEADER)) {
			/*
			 * convert a old color table (3 byte entries) to a new
			 * color table (4 byte entries)
			 */
			_lread(fh, (LPSTR)pRgb, nNumColors * sizeof(RGBTRIPLE));

			for (i = nNumColors - 1; i >= 0; i--) {
				RGBQUAD rgb;

				rgb.rgbRed = ((RGBTRIPLE FAR *)pRgb)[i].rgbtRed;
				rgb.rgbBlue = ((RGBTRIPLE FAR *)pRgb)[i].rgbtBlue;
				rgb.rgbGreen = ((RGBTRIPLE FAR *)pRgb)[i].rgbtGreen;
				rgb.rgbReserved = (BYTE)0;

				pRgb[i] = rgb;
			}
		} else {
			_lread(fh, (LPSTR)pRgb, nNumColors * sizeof(RGBQUAD));
		}
	}

	if (bf.bfOffBits != 0L)
		_llseek(fh, off + bf.bfOffBits, SEEK_SET);

	return hbi;
}

// FUNCTION: JMAN10 0x100850a6
HBITMAP BitmapFromDib(HDIB hdib, HPALETTE hpal, WORD wUsage) {
	LPBITMAPINFOHEADER lpbi;
	HPALETTE hpalT;
	HDC hdc;
	HBITMAP hbm;

	if (!hdib)
		return NULL;

	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hdib);

	if (!lpbi)
		return NULL;

	hdc = GetDC(NULL);

	if (hpal) {
		hpalT = SelectPalette(hdc, hpal, FALSE);
		RealizePalette(hdc);
	}

	hbm = CreateDIBitmap(hdc, (LPBITMAPINFOHEADER)lpbi, (LONG)CBM_INIT,
						 DibPtr(lpbi), (LPBITMAPINFO)lpbi, wUsage);

	if (hpal && hpalT)
		SelectPalette(hdc, hpalT, FALSE);

	ReleaseDC(NULL, hdc);

	return hbm;
}
