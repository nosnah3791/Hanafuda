#include "StdAfx.h"
#include "XGraphics.h"

XGraphics::XGraphics(void)
: m_hWnd(NULL)
, m_uiWidth(0)
, m_uiHeight(0)
, m_gOnScreen(NULL)
, m_offScreenBmp(NULL)
, m_gOffScreen(NULL)
{
}

XGraphics::~XGraphics(void)
{
}

XGraphics::XGraphics(HWND hWnd)
{
	this->SetProperty(hWnd);
}

XGraphics::XGraphics(HWND hWnd, int width, int height)
{
}

Status XGraphics::Update(HDC hdc)
{
	//180回転＋反転
	this->m_offScreenBmp->RotateFlip(Rotate180FlipX);

	// オフスクリーンからウィンドウへの転送
	Gdiplus::BitmapData bitmapData;
	this->m_offScreenBmp->LockBits(NULL, Gdiplus::ImageLockModeRead, PixelFormat24bppRGB, &bitmapData);

	BITMAPINFOHEADER bmpInfoHeader;
	memset(&bmpInfoHeader, 0, sizeof(BITMAPINFOHEADER));
	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfoHeader.biWidth = bitmapData.Width;
	bmpInfoHeader.biHeight = bitmapData.Height;
	bmpInfoHeader.biPlanes = 1;
	bmpInfoHeader.biBitCount = 24;

	StretchDIBits(hdc,
		0, 0, bmpInfoHeader.biWidth, bmpInfoHeader.biHeight,
		0, 0, bmpInfoHeader.biWidth, bmpInfoHeader.biHeight,
		bitmapData.Scan0, (BITMAPINFO*)&bmpInfoHeader,
		DIB_RGB_COLORS, SRCCOPY);

	this->m_offScreenBmp->UnlockBits(&bitmapData);
	
	return (Gdiplus::Status)NULL;
}


void XGraphics::SetProperty(HWND hWnd)
{
	this->m_hWnd = hWnd;
	struct tagRECT lpRect;
	GetWindowRect(this->m_hWnd,&lpRect);
	this->m_uiWidth = lpRect.right - lpRect.left;
	this->m_uiHeight = lpRect.bottom - lpRect.top;
	
	//オフスクリーン
	this->m_offScreenBmp = new Bitmap(this->m_uiWidth,this->m_uiHeight);
	this->m_gOffScreen = Graphics::FromImage(this->m_offScreenBmp);

	//オンスクリーン
	this->m_gOnScreen = Graphics::FromHWND(this->m_hWnd);
}

//アルファブレンドする
byte XGraphics::BlendAlpha(UINT dstByte, UINT srcByte, UINT alpha)
{
	UINT  blendedSrcByte = ((255-alpha)*dstByte+alpha*(srcByte+dstByte-srcByte*dstByte/255))/255;
	//UINT  blendedSrcByte = srcByte+dstByte-srcByte*dstByte/255;
	
	if(blendedSrcByte>255)
	{
		return (byte)255;
	}
	else if(blendedSrcByte<0)
	{
		return (byte)0;
	}
	return (byte)blendedSrcByte;
}

//二つの画像をブレンドする
void XGraphics::BlendImages(INT x, INT y, int blendFunc,Bitmap* srcBitmap, Bitmap* dstBitmap)
{

	if(!dstBitmap) dstBitmap = this->m_offScreenBmp;

	//ブレンド方法の分類
	switch(blendFunc){
		case BLEND_ALPHA://アルファ
			this->m_funcBlend = &XGraphics::BlendAlpha;
		break;
	}


    int srcWidth  = srcBitmap->GetWidth();
    int srcHeight = srcBitmap->GetHeight();
    
    BitmapData dstBD;
    BitmapData srcBD;

    Rect dstRect(x, y, srcWidth, srcHeight);
    Rect srcRect(0, 0, srcWidth, srcHeight);

    dstBitmap->LockBits(&dstRect, NULL, PixelFormat32bppARGB, &dstBD);
    srcBitmap->LockBits(&srcRect, ImageLockModeRead, PixelFormat32bppARGB, &srcBD);

    int dstStride = dstBD.Stride;
    int srcStride = srcBD.Stride;

    UINT* dstPixel = (UINT*)dstBD.Scan0;
    UINT* srcPixel = (UINT*)srcBD.Scan0;

    byte* pDst = (byte*)dstPixel;
    byte* pSrc = (byte*)srcPixel;
    
    byte dstByte;
    byte srcByte;
    byte alpha;
    
    for(int y=0; y<srcHeight; y++)
    {
        for(int x=0; x<srcWidth; x++)
        {
            // アルファ値
            alpha = pSrc[4*x+3+y*srcStride];
            // RGB
            for(int p=0; p<3; p++)
            {
                dstByte = pDst[4*x+p+y*dstStride];
                srcByte = pSrc[4*x+p+y*srcStride];
				pDst[4*x+p+y*dstStride] = (this->*m_funcBlend)(dstByte, srcByte, alpha);
            }
        }
    }

    dstBitmap->UnlockBits(&dstBD);
    srcBitmap->UnlockBits(&srcBD);
            
    return;
}

