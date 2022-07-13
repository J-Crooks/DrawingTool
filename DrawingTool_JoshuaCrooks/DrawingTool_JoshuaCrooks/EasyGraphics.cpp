#include "EasyGraphics.h"
#include <assert.h>

// see header file about "Msimg32.lib"

/*
  WARNING: Only edit this file if you cannot achieve what you need via inheriting from EasyGraphics or using it as its own object.
           If you edit this file and I release an updated version, you'll not be able to easily update your version
*/

const char* EasyGraphics::EASY_GRAPHICS_VERSION_STRING="[EasyGraphics Version 1.2]";

// (static) class variables
int EasyGraphics::noofwindows=0;
EasyGraphics** EasyGraphics::windows=NULL;

// Window Thread
DWORD WINAPI WindowThread(LPVOID lpParameter);
struct CREATEINFO
{
  EasyGraphics* window;
  int width, height;
  HINSTANCE hInstance;
  HANDLE ready;
  bool ismodal;
};

// Window callback function
LRESULT CALLBACK EasyGraphicsWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

/* Start of the object methods */

EasyGraphics::EasyGraphics() : std::ostream(this), noofrows(20), noofcols(20), textcolour(clWhite), hdcback(NULL), immediatemode(true), bitmapcache(16), pencache(16), brushcache(16)
{
  keyevent=::CreateEvent(NULL, TRUE, FALSE, NULL);

  NONCLIENTMETRICS metrics;
  memset(&metrics, 0, sizeof(NONCLIENTMETRICS));
  metrics.cbSize=sizeof(NONCLIENTMETRICS)-4;// cannot include "iPaddedBorderWidth" sizeof(metrics.iPaddedBorderWidth);
  ::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, metrics.cbSize, &metrics, 0);
  if (metrics.lfMessageFont.lfHeight) // check this function worked OK
    hdeffont=::CreateFontIndirect(&metrics.lfMessageFont);
  else // else revert back to the default method
    hdeffont=(HFONT) ::GetStockObject(DEFAULT_GUI_FONT);
}

EasyGraphics::~EasyGraphics() 
{
  ::DeleteObject(hbmpback);
  ::DeleteDC(hdcback);
  ::DeleteDC(hdcstamp);
  ::DeleteObject(hfont);
}

void EasyGraphics::create(HINSTANCE hInstance, const int width, const int height, const int cols, const bool createmodal)
{
  if (cols>0)
    noofcols=cols;
  noofrows=0;

  // Create this on the heap because if we create a thread, it will go out of scope if on the heap
  CREATEINFO* info=(CREATEINFO*) malloc(sizeof(CREATEINFO));
  info->window=this;
  info->width=width;
  info->height=height;
  info->hInstance=hInstance;
  info->ready=::CreateEvent(NULL, TRUE, FALSE, NULL);

  if (createmodal)
  {
    winthread=NULL;
    WindowThread(info);
  }
  else
  {
    ready=info->ready;
    winthread=CreateThread(NULL, 0, WindowThread, info, 0, NULL);
    WaitForSingleObject(info->ready, INFINITE); // wait until the window has been created and set up
  }
}

void EasyGraphics::waitForClose()
{
  WaitForSingleObject(winthread, INFINITE);
  Sleep(200); // just gives a little extra time for the window to fully draw etc
}


DWORD WINAPI WindowThread(LPVOID lpParameter)
{
  CREATEINFO* info=(CREATEINFO*) lpParameter;

  WNDCLASSEX classname;
  classname.cbSize=sizeof(WNDCLASSEX);
  classname.style=CS_HREDRAW|CS_VREDRAW;
  classname.lpfnWndProc=(WNDPROC) EasyGraphicsWndProc;   // This is the name of the event-based callback method
  classname.cbClsExtra=0;
  classname.cbWndExtra=0;
  classname.hInstance=info->hInstance;
  classname.hIcon=LoadIcon(NULL, IDI_WINLOGO);
  classname.hCursor=LoadCursor(NULL, IDC_ARROW);
  classname.hbrBackground=::GetSysColorBrush(COLOR_3DFACE);
  classname.lpszMenuName=NULL;
  classname.lpszClassName=L"EasyGraphics";
  classname.hIconSm=LoadIcon(NULL, IDI_WINLOGO);
  RegisterClassEx(&classname);

  int offx=(::GetSystemMetrics(SM_CXSCREEN)-info->width)/2;
  int offy=(::GetSystemMetrics(SM_CYSCREEN)-info->height)/2;

  // Create the window using the definition provided above
  CreateWindowEx(NULL, L"EasyGraphics", L"EasyGraphics", WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX|WS_VISIBLE, offx, offy, info->width, info->height, NULL, NULL, info->hInstance, info->window);

  // Set the event-based message system up
  MSG msg;
  PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
  while (msg.message!=WM_QUIT)  // keep looping until we get the quit message
  {
    if (GetMessage(&msg, NULL, 0, 0)) // cause this thread to wait until there is a message to process
    {
      // These two lines of code take the MSG structure, mess with it, and correctly dispatch it to the WndProc defined during the window creation
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  CloseHandle(info->ready);

  EasyGraphics::deregisterWindow(info->window);
  free(info);

  return 0;
}

LRESULT CALLBACK EasyGraphicsWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  if (message==WM_CREATE)
  {
    EasyGraphics* window=((EasyGraphics*) ((CREATESTRUCT*) lParam)->lpCreateParams);
    EasyGraphics::registerWindow(window, hWnd);
    window->onCreate();
    return 0;
  }
  else
  {
    EasyGraphics* window=EasyGraphics::findWindow(hWnd);
    if (window)
    {
      switch (message)
      {
      case WM_PAINT:
        {
          PAINTSTRUCT paint;
          BeginPaint(window->hwnd, &paint);
          window->onDraw();
          EndPaint(window->hwnd, &paint);
        }
        break;
      case WM_DESTROY:
        window->onDestroy();
        PostQuitMessage(0);
        break;
      case WM_CLOSE:
        DestroyWindow(hWnd);
        PostQuitMessage(0);
        break;
      case WM_SIZE: window->onSize((DWORD) wParam, (UINT) (lParam & 0xFFFF), (UINT) (lParam >> 16)); break;
      case WM_TIMER: window->onTimer((UINT) wParam); break;
      case WM_LBUTTONDOWN: window->onLButtonDown((UINT)wParam, (int)(lParam & 0xFFFF), (int)((lParam >> 16) & 0xFFFF)); break;
      case WM_LBUTTONUP: window->onLButtonUp((UINT)wParam, (int)(lParam & 0xFFFF), (int)((lParam >> 16) & 0xFFFF)); break;
      case WM_RBUTTONDOWN: window->onRButtonDown((UINT) wParam, (int) (lParam & 0xFFFF), (int) ((lParam >> 16) & 0xFFFF)); break;
      case WM_RBUTTONUP: window->onRButtonUp((UINT) wParam, (int) (lParam & 0xFFFF), (int) ((lParam >> 16) & 0xFFFF)); break;
      case WM_MOUSEMOVE: window->onMouseMove((UINT)wParam, (int)(lParam & 0xFFFF), (int)((lParam >> 16) & 0xFFFF)); break;
      case WM_KEYDOWN: window->onKeyDown((UINT) wParam, (UINT) (lParam & 0xFFFF), (UINT) (lParam >> 16)); break;
      case WM_KEYUP: window->onKeyUp((UINT) wParam, (UINT) (lParam & 0xFFFF), (UINT) (lParam >> 16)); break;
      case WM_CHAR: window->onChar((UINT) wParam, (UINT) (lParam & 0xFFFF), (UINT) (lParam >> 16)); break;
      }
    }
  }
  return DefWindowProc(hWnd, message, wParam, lParam);
}

void EasyGraphics::onCreate()
{
  hbmpback=NULL;
  HDC hdcsrc=::GetDC(NULL);
  hdcback=::CreateCompatibleDC(hdcsrc);
  hdcstamp=::CreateCompatibleDC(hdcsrc);
  ::ReleaseDC(NULL, hdcsrc);

  setPenColour(clWhite, 1);
  selectBackColour(clBlack);

  hfont=NULL;
}

void EasyGraphics::onDestroy()
{
  SetEvent(keyevent);
  CloseHandle(keyevent);
  keyevent=NULL;
}

void EasyGraphics::onSize(UINT nType, int cx, int cy)
{
  // Create an off-screen image buffer to avoid flicker (double-buffering)
  HDC hdcsrc=::GetDC(NULL);

  ::DeleteObject(hfont);
  LOGFONT logfont;
  logfont.lfHeight=(2*cx)/noofcols; // the font is roughly twice as high as it is wide
  logfont.lfWidth=0;
  logfont.lfEscapement=0;
  logfont.lfOrientation=0;
  logfont.lfWeight=FW_REGULAR;
  logfont.lfItalic=0;
  logfont.lfUnderline=0;
  logfont.lfStrikeOut=0;
  logfont.lfCharSet=DEFAULT_CHARSET;
  logfont.lfOutPrecision= OUT_DEFAULT_PRECIS;
  logfont.lfClipPrecision= CLIP_DEFAULT_PRECIS;
  logfont.lfQuality=DEFAULT_QUALITY;
  logfont.lfPitchAndFamily=DEFAULT_PITCH;
  MultiByteToWideChar(CP_ACP, 0, "Consolas", -1, logfont.lfFaceName, 32);
  
  TEXTMETRIC metrics;
  hfont=::CreateFontIndirect(&logfont);
  ::GetTextMetrics(hdcback, &metrics);
  noofrows=cy/metrics.tmHeight;

  scales[0]=cx/(float) noofcols;
  scales[1]=cy/(float) noofrows;

  HBITMAP hbmpback;
  BITMAPINFO backbmpinfo;
  backbmpinfo.bmiHeader.biSize=sizeof(BITMAPINFO);
  backbmpinfo.bmiHeader.biWidth=cx;
  backbmpinfo.bmiHeader.biHeight=cy;
  backbmpinfo.bmiHeader.biPlanes=1;
  backbmpinfo.bmiHeader.biBitCount=32;
  backbmpinfo.bmiHeader.biCompression=BI_RGB;
  backbmpinfo.bmiHeader.biSizeImage=backbmpinfo.bmiHeader.biWidth*backbmpinfo.bmiHeader.biHeight*backbmpinfo.bmiHeader.biBitCount/8;
  backbmpinfo.bmiHeader.biXPelsPerMeter=0;
  backbmpinfo.bmiHeader.biYPelsPerMeter=0;
  backbmpinfo.bmiHeader.biClrUsed=0;
  backbmpinfo.bmiHeader.biClrImportant=0;

  hbmpback=::CreateDIBSection(hdcsrc, &backbmpinfo, DIB_RGB_COLORS, NULL, NULL, NULL);

  const bool resize=(this->hbmpback!=NULL);
  if (resize) // resize the current image (although this has been disabled in the window creation setup now)
  {
    BITMAP bitmap;
    GetObject(this->hbmpback, sizeof(BITMAP), &bitmap);
    float scale=cx/(float) bitmap.bmWidth;
    HDC hdctmp=::CreateCompatibleDC(hdcsrc);
    ::SelectObject(hdctmp, hbmpback);
    RECT rect={0, 0, cx, cy};
    ::FillRect(hdctmp, &rect, hbrush);
    ::SetStretchBltMode(hdctmp, HALFTONE);
    ::SetBrushOrgEx(hdctmp, 0, 0, NULL);
    ::StretchBlt(hdctmp, 0, 0, (int) (bitmap.bmWidth*scale), (int) (bitmap.bmHeight*scale), hdcback, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
    ::DeleteDC(hdctmp);

    ::DeleteObject(this->hbmpback);
  }
  this->hbmpback=hbmpback;
  ::SelectObject(hdcback, hbmpback);
  ::SetTextAlign(hdcback, TA_LEFT|TA_TOP);
  ::SetBkMode(hdcback, TRANSPARENT);

  ::SetTextColor(hdcback, textcolour);
  ::SelectObject(hdcback, hpen);
  ::SelectObject(hdcback, hbrush);

  if (!resize)
  {
    clrscr(clBlack);
    (*this) << EASY_GRAPHICS_VERSION_STRING << "\n";
    SetEvent(ready);
  }


  ::ReleaseDC(NULL, hdcsrc);
}

void EasyGraphics::clrscr(int colour)
{
  curpos.x=0;
  curpos.y=0;

  BITMAP bitmap;
  GetObject(hbmpback, sizeof(BITMAP), &bitmap);

  RECT rect={0, 0, bitmap.bmWidth, bitmap.bmHeight};
  ::FillRect(hdcback, &rect, brushcache.getTool(colour)->getHBRUSH());
/*
  Displays the console grid
  ::SelectObject(hdcback, ::GetStockObject(WHITE_PEN));
  for (int i=0; i<=noofrows; i++)
  {
    const int y=(int) (i*scales[1]);
    ::MoveToEx(hdcback, 0, y, NULL);
    ::LineTo(hdcback, bitmap.bmWidth, y);
  }

  for (int i=0; i<=noofcols; i++)
  {
    const int x=(int) (i*scales[0]);
    ::MoveToEx(hdcback, x, 0, NULL);
    ::LineTo(hdcback, x, bitmap.bmHeight);
  }

  ::SetTextColor(hdcback, 0x00FFFFFF);
  ::TextOut(hdcback, 0, 0, L"Microsoft Windows [Version", 26);
  */
  
  if (immediatemode)
    EasyGraphics::onDraw();
}

void EasyGraphics::drawRectangle(int x1, int y1, int width, int height, bool fill)
{
  if (!fill)
    ::SelectObject(hdcback, ::GetStockObject(NULL_BRUSH));
  ::Rectangle(hdcback, x1, y1, x1+width, y1+height);

  if (!fill)
    ::SelectObject(hdcback, hbrush);
  
  if (immediatemode)
    EasyGraphics::onDraw();
}

void EasyGraphics::drawCircle(int x, int y, int radius, bool fill)
{
  if (!fill)
    ::SelectObject(hdcback, ::GetStockObject(NULL_BRUSH));
  ::Ellipse(hdcback, x-radius, y-radius, x+radius, y+radius);
  
  if (!fill)
    ::SelectObject(hdcback, hbrush);

  if (immediatemode)
    EasyGraphics::onDraw();
}

void EasyGraphics::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, bool fill)
{
  POINT points[4];
  points[0]={x1, y1};
  points[1]={x2, y2};
  points[2]={x3, y3};
  points[3]={x1, y1};

  if (!fill)
    ::SelectObject(hdcback, ::GetStockObject(NULL_BRUSH));
  ::Polygon(hdcback, points, 4);

  if (!fill)
    ::SelectObject(hdcback, hbrush);

  if (immediatemode)
    EasyGraphics::onDraw();
}

void EasyGraphics::drawBitmap(const wchar_t* filename, int x, int y, int width, int height, int transparent)
{
  const BitmapItem* cache=bitmapcache.getTool(filename);

  BITMAP bmpinfo;
  GetObject(cache->getHBITMAP(), sizeof(BITMAP), &bmpinfo);
  ::SelectObject(hdcstamp, cache->getHBITMAP());

  ::SetStretchBltMode(hdcback, HALFTONE);
  ::SetBrushOrgEx(hdcback, 0, 0, NULL);
#ifdef USE_EG_TRANSPARENT_BITMAPS // this provides a double check that the library is included in the project build and adds support to use EasyGraphics without transparent bitmaps and without the need to adding in the library resource
  if (transparent==-1)
    ::StretchBlt(hdcback, x, y, width, height, hdcstamp, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, SRCCOPY);
  else
    ::TransparentBlt(hdcback, x, y, width, height, hdcstamp, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, transparent);
#else
  ::StretchBlt(hdcback, x, y, width, height, hdcstamp, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, SRCCOPY);
#endif


  if (immediatemode)
    EasyGraphics::onDraw();
}

void EasyGraphics::drawText(const wchar_t* text, int x, int y)
{
  if (text)
  {
    ::SelectObject(hdcback, hdeffont);

    ::TextOut(hdcback, x, y, text, (int) wcslen(text));

    ::SelectObject(hdcback, hfont);
  }

  if (immediatemode)
    EasyGraphics::onDraw();
}

void EasyGraphics::onDraw()
{
  RECT rect;
  ::GetClientRect(hwnd, &rect);

  HDC hdcdst=::GetDC(hwnd);
  ::BitBlt(hdcdst, 0, 0, rect.right, rect.bottom, hdcback, 0, 0, SRCCOPY);
  ::ReleaseDC(hwnd, hdcdst);
}

void EasyGraphics::onChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  if (nChar==0x8) // backspace
  {
    if (curpos.x)
    {
      --curpos.x;
      int t=curpos.x;
      overflow(' ');
      curpos.x=t;
    }

    if (keyinput.size())
      keyinput.erase(keyinput.end()-1);
  }
  else if (nChar=='\r' || nChar=='\n')
  {
    for (unsigned int i=0; i<keyinput.size(); i++)
      keylogger << (char) keyinput.at(i);
    keylogger << (char) ' ';

    overflow('\n');
    keyinput.clear();

    SetEvent(keyevent);
  }
  else
  {
    if (nChar>=0x20 && nChar<=0xFF)
    {
      keyinput.push_back((char) nChar);
      overflow(nChar);
    }
  }

  if (!immediatemode)
    EasyGraphics::onDraw();
}

int EasyGraphics::overflow(int c)
{
  if (c=='\n' || c=='\r')
  {
    curpos.x=0;
    ++curpos.y;
  }
  else if (c=='\t')
  {
    int rem=curpos.x%10;
    curpos.x+=(10-rem);
    if (curpos.x>noofcols)
    {
      curpos.x=0;
      ++curpos.y;
    }
  }
  else
  {
    const float x=(curpos.x*scales[0]);
    const float y=(curpos.y*scales[1]);

    const int ix=(int) x;
    const int iy=(int) y;

    RECT rect={ix, iy, (int) (x+scales[0]), (int) (y+scales[1])};
    ::FillRect(hdcback, &rect, hbrush);

    wchar_t text[1]={(wchar_t) c};

    //::SetTextColor(hdcback, textcolour);
    ::TextOut(hdcback, ix, iy, text, 1);

    ++curpos.x;
    if (curpos.x==noofcols)
    {
      curpos.x=0;
      ++curpos.y;
    }
  }

  if (curpos.y+1>=screenHeight())
  {
    BITMAP bitmap;
    ::GetObject(hbmpback, sizeof(BITMAP), &bitmap);
    ::BitBlt(hdcback, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcback, 0, (int) scales[1], SRCCOPY);

    RECT rect={0, bitmap.bmHeight-(int) scales[1], bitmap.bmWidth, bitmap.bmHeight};
    ::FillRect(hdcback, &rect, hbrush);
    --curpos.y;
  }

  if (immediatemode)
    EasyGraphics::onDraw();
  return 0;
}

void EasyGraphics::onTimer(UINT nIDEvent) {}
void EasyGraphics::onLButtonDown(UINT nFlags, int x, int y) {}
void EasyGraphics::onLButtonUp(UINT nFlags, int x, int y) {}
void EasyGraphics::onRButtonDown(UINT nFlags, int x, int y) {}
void EasyGraphics::onRButtonUp(UINT nFlags, int x, int y) {}
void EasyGraphics::onMouseMove(UINT nFlags, int x, int y) {}
void EasyGraphics::onKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {}
void EasyGraphics::onKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {}


// Member functions for BitmapItem
const EasyGraphics::BitmapItem& EasyGraphics::BitmapItem::operator=(const wchar_t* filename)
{
  if (filename)
  {
    const size_t len=wcslen(filename)+1;
    this->filename=(wchar_t*) realloc(this->filename, 2*len);
    wcscpy_s(this->filename, len, filename);
    ::DeleteObject(hbmp);
    hbmp=(HBITMAP) ::LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION|LR_LOADFROMFILE);
  }
  return *this;
}

// Member functions for PenItem
const EasyGraphics::PenItem& EasyGraphics::PenItem::operator=(const LOGPEN& rhs)
{
  ::DeleteObject(hpen);
  this->colour=rhs.lopnColor;
  this->thickness=rhs.lopnWidth.x;
  hpen=::CreatePen(PS_SOLID, thickness, colour);

  return *this;
}

// Member functions for BrushItem
const EasyGraphics::BrushItem& EasyGraphics::BrushItem::operator=(const int rhs)
{
  ::DeleteObject(hbrush);
  colour=(DWORD) rhs;
  LOGBRUSH lbrush={BS_SOLID, (DWORD) colour, NULL};
  hbrush=::CreateBrushIndirect(&lbrush);

  return *this;
}

