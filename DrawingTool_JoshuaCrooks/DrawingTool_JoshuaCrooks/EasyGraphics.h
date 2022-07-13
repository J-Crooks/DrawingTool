#pragma once
#include <assert.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

// make sure you add "Msimg32.lib" to your additional library dependencies
// once you have added Msimage32.lib, uncomment the following #define statement
//#define USE_EG_TRANSPARENT_BITMAPS

/*
  WARNING: Only edit this file if you cannot achieve what you need via inheriting from EasyGraphics or using it as its own object.
           If you edit this file and I release an updated version, you'll not be able to easily update your version
*/

/* This class provides a way of creating a simple windows-based output display
* and is based on the ConsoleUtils.cpp and ConsoleUtils.h file you used in FoP
*/

class EasyGraphics : public std::ostream, std::streambuf
{
public:
  EasyGraphics();
  virtual ~EasyGraphics();

  // Create a window
  // cols = number of text columns to set the window to (think console)
  // createmodel: true is a modal window that takes over the calling thread until the window is closed
  //              false creates a window that runs in it's own thread and immediately returns control back to the calling thread - use WaitForClose to know when the window closes
  void create(HINSTANCE hInstance, const int width, const int height, const int cols, const bool createmodal);
  
  // if not created as modal, call this method to pause execution on the main thread until the window is closed
  void waitForClose();
  
  // methods from ConsoleUtils
  void clrscr(int colour);                      //clear the screen, and reset cursor to (0, 0) (or a direct BGR value)
  void gotoxy(int x, int y);                    //move the text cursor to position (x, y)
  void selectBackColour(int colour);            //select a background/fill colour from the colour constants list (or a direct BGR value)
  void selectTextColour(int colour);            //select a text colour from the colour constants list (or a direct BGR value)
  int screenWidth();                            //get the width of the screen (number of logical columns - this is not pixels - use getDrawableSize for pixel resolution)
  int screenHeight();                           //get the height of the screen (number of logical rows - this is not pixels)
  
  int getPixelsPerColumn();                     //returns the number of pixels per logical column
  int getPixelsPerRow();                        //returns the number of pixels per logical row
  void getDrawableSize(int& width, int& height);//returns the width and height of the drawable part of the window in pixels

  // this determines whether the window should update/redraw as soon as a draw command is issued
  // when immediate is set to true, all draw commands are automatically flushed to the screen immediately
  // when immediate is set to false, you should call the EasyGraphics::onDraw method to flush and update/redraw the screen after you have finished your draw commands
  void setImmediateDrawMode(const bool immediate);

  // Creates a timer that will create an onTimer event with the returned timerid (check the returned id is the same as request)
  int setTimer(int timerid, int elapsedms);
  void killTimer(int timerid);

  // Enhanced methods
  void setPenColour(int colour, int penwidth);  //set the colour to be used for line drawing along with pen width (there is a platform-dependent limit on the thickness)
  void drawLine(int x1, int y1, int x2, int y2);//draws a line between {x1, y1} and {x2, y2}
  void drawRectangle(int x, int y, int width, int height, bool fill); // fills with back colour, outlines with pen
  void drawCircle(int x, int y, int radius, bool fill); // Draws a circle centred on {x, y} with a given radius; fills with back colour, outlines with pen
  void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, bool fill); // Draws a triangle between the three points; fills with back colour, outlines with pen
  void drawBitmap(const wchar_t* filename, int x, int y, int width, int height, int transparent=-1); // draws a bitmap file (.bmp) at {x, y} to the specified width and height - maintaining the aspect ratio is the responsibility of the caller - use the transparent parameter to turn a colour transparent (use 0xFFFFFFFF for no transparent colour) - see the comments at the top of EasyGraphics.h for library requirements
  void drawText(const wchar_t* text, int x, int y); // draws text using the default windows font and with a transparent background.  The top, left of the text are the (x, y) parameters.  Use the setHDEFFont method to change this font.

  // captures the ostream stuff
  virtual int overflow(int c);  
  
  // istream stuff
  char get();
  void clear();
  void ignore();
  void getline(std::string& text);
  template<typename T> EasyGraphics& operator>>(T& val);

  // Colour constants - these are now direct BGR values (RGB is stored in reverse in windows) for greater flexibility in setting your own colours
  static const int clBlack=0x00000000;
  static const int clDarkRed=0x00000080;
  static const int clDarkGreen=0x00008000;
  static const int clDarkBlue=0x00800000;
  static const int clDarkCyan=0x00808000;
  static const int clDarkMagenta=0x00800080;
  static const int clDarkYellow=0x00008080;
  static const int clDarkGrey=0x00808080;
  static const int clGrey=0x00C0C0C0;
  static const int clRed=0x000000FF;
  static const int clGreen=0x0000FF00;
  static const int clBlue=0x00FF0000;
  static const int clCyan=0x00C0C000;
  static const int clMagenta=0x00C000C0;
  static const int clYellow=0x0000FFFF;
  static const int clWhite=0x00FFFFFF;

  static const char* EASY_GRAPHICS_VERSION_STRING;

protected:
  /* if you override
   * onCreate()
   * onSize()
   * onDestroy()
   * You MUST also call the EasyGraphics versions as they perform key operations */

  virtual void onCreate();                                        // Remember to call EasyGraphics's onCreate() if overriden in a derived class.  See WM_CREATE in MSDN documentation
  virtual void onSize(UINT nType, int cx, int cy);                // Remember to call EasyGraphics's onSize(...) if overriden in a derived class.  See WM_SIZE in MSDN documentation
  virtual void onDraw();                                          // See WM_PAINT in MSDN documentation
  virtual void onTimer(UINT nIDEvent);                            // See WM_TIMER in MSDN documentation
  virtual void onLButtonDown(UINT nFlags, int x, int y);          // See WM_LBUTTONDOWN in MSDN documentation
  virtual void onLButtonUp(UINT nFlags, int x, int y);            // See WM_LBUTTONUP in MSDN documentation
  virtual void onRButtonDown(UINT nFlags, int x, int y);          // See WM_RBUTTONDOWN in MSDN documentation
  virtual void onRButtonUp(UINT nFlags, int x, int y);            // See WM_RBUTTONUP in MSDN documentation
  virtual void onMouseMove(UINT nFlags, int x, int y);            // See WM_MOUSEMOVE in MSDN documentation
  virtual void onKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);  // See WM_KEYDOWN in MSDN documentation
  virtual void onKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);    // See WM_KEYUP in MSDN documentation
  virtual void onChar(UINT nChar, UINT nRepCnt, UINT nFlags);     // See WM_CHAR in MSDN documentation
  virtual void onDestroy();                                       // Remember to call EasyGraphics's onDestroy() if overriden in a derived class.  See WM_DESTROY in MSDN documentation

  // Calculates the corresponding cell that the mouse has been clicked (based on the character grid overlay)
  void calcCell(const int mousex, const int mousey, int& cellx, int& celly);

  // Returns the HDC for the backbuffer - to fully override the Draw method and use the GDI library, use this as the off-screen
  // image to draw everything to and then call the EasyGraphics OnDraw method to copy it to the window - this avoids flickering
  HDC getBackBuffer();

  // Returns the HWND for the window - useful if you wish to use any of the Win32 GDI functions that require the HWND paramter, for which there are many
  HWND getHWND();

  // Sets the font used when drawing text using the drawText method
  void setHDEFFont(HFONT font);

private:
  HWND hwnd;              // handle to the window
  HDC hdcback, hdcstamp;  // hdcback is used for the backbuffer / hdcstamp is used for bitmap draws
  HFONT hfont;            // current font
  HFONT hdeffont;         // default font used by windows
  HBITMAP hbmpback;       // off-screen image used for the backbuffer
  HANDLE winthread;       // handle to the window thread when non-modal
  HANDLE ready;           // used when non-modal to indicate when the window is ready to be used (null otherwise)
  HPEN hpen;              // current pen (colour and width) for drawing shapes
  HBRUSH hbrush;          // current brush for filled and background colour
  int textcolour;         // colour of the drawn text
  bool immediatemode;     // true when draws should happen immediately.  when false, onDraw must be called to update the screen

  POINT curpos;           // current position of the next character press/display
  int noofcols, noofrows; // number of text-based rows and columns
  float scales[2];        // scalex and scaley - maps between pixels and text-based rows and columns

  // This is used to cache drawing "tools" such as bitmaps, pens and brushes
  template<typename T, typename S> class ToolCache
  {
  public:
    ToolCache(const int cachesize);
    ~ToolCache() { delete[] items; }
    const T* getTool(S find);
  private:
    int cachesize;
    struct CACHE_PAIR { T item; DWORD lastaccessed; } *items; // a pointer to an array of CACHE_PAIRs
  };

  // Used with ToolCache to cache bitmaps
  class BitmapItem 
  { 
  public:
    BitmapItem() : filename(NULL), hbmp(NULL) {}
    ~BitmapItem() { free(filename); ::DeleteObject(hbmp); }
    bool operator==(const wchar_t* filename) { return this->filename && filename && !_wcsicmp(this->filename, filename); }
    const BitmapItem& operator=(const wchar_t* filename);
    HBITMAP getHBITMAP() const { return hbmp; }
  private:
    wchar_t* filename; HBITMAP hbmp; 
  };

  // Used with ToolCache to cache pens
  class PenItem
  { 
  public:
    PenItem() : colour(0), thickness(0), hpen(NULL) {}
    ~PenItem() { ::DeleteObject(hpen); }
    bool operator==(const LOGPEN& rhs) { return colour==rhs.lopnColor && thickness==rhs.lopnWidth.x; }
    const PenItem& operator=(const LOGPEN& rhs);
    HPEN getHPEN() const { return hpen; }
  private:
    DWORD colour; int thickness; HPEN hpen; 
  };

  // Used with ToolCache to cache brushes
  class BrushItem
  { 
  public:
    BrushItem() : colour(0), hbrush(NULL) {}
    ~BrushItem() { ::DeleteObject(hbrush); }
    bool operator==(const int rhs) { return colour==(DWORD) rhs && hbrush; }  // need to check that hBrush is valid
    const BrushItem& operator=(const int rhs);
    HBRUSH getHBRUSH() const { return hbrush; }
  private:
    DWORD colour; HBRUSH hbrush;
  };

  // Three cache objects to hold drawing tools for faster creation and use
  ToolCache<BitmapItem, const wchar_t*> bitmapcache;
  ToolCache<PenItem, const LOGPEN&> pencache;
  ToolCache<BrushItem, const int> brushcache;

  // stream stuff used to manager key presses and stream ">>" calls
  HANDLE keyevent;
  std::stringstream keylogger;
  std::vector<char> keyinput;

  // used to block the calling thread and wait for keyboard input
  void waitForInput();

  // static members used to control multiple windows
  static int noofwindows;
  static EasyGraphics** windows;
  static void registerWindow(EasyGraphics* window, HWND hwnd);
  static void deregisterWindow(EasyGraphics* window);
  static EasyGraphics* findWindow(HWND hwnd);

  // Let's give our class some friends to make it liked
  friend DWORD WINAPI WindowThread(LPVOID lpParameter);
  friend LRESULT CALLBACK EasyGraphicsWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

/* Object Methods*/
inline int EasyGraphics::screenWidth() { return noofcols; }
inline int EasyGraphics::screenHeight() { return noofrows; }
inline int EasyGraphics::getPixelsPerColumn() { return (int) scales[0]; }
inline int EasyGraphics::getPixelsPerRow() { return (int) scales[1]; }

inline void EasyGraphics::getDrawableSize(int& width, int& height)
{
  RECT rect;
  ::GetClientRect(hwnd, &rect);
  width=rect.right-rect.left; // left should be zero, but do the correct math just in case
  height=rect.bottom-rect.top; // top should be zero, but do the correct math just in case
}

inline void EasyGraphics::setPenColour(int colour, int penwidth)
{
  LOGPEN pen={PS_SOLID,{penwidth, 0}, (DWORD) colour};
  hpen=pencache.getTool(pen)->getHPEN();
  ::SelectObject(hdcback, hpen);
}

inline void EasyGraphics::selectBackColour(int colour) { hbrush=brushcache.getTool(colour)->getHBRUSH(); ::SelectObject(hdcback, hbrush); }
inline void EasyGraphics::selectTextColour(int colour) { textcolour=colour; ::SetTextColor(hdcback, textcolour); }
inline void EasyGraphics::gotoxy(int x, int y) { assert(x>=0 && x<noofcols && y>=0 && y<noofrows); curpos.x=x; curpos.y=y; }
inline void EasyGraphics::drawLine(int x1, int y1, int x2, int y2)
{
  ::MoveToEx(hdcback, x1, y1, NULL);
  ::LineTo(hdcback, x2, y2);
  
  if (immediatemode)
    EasyGraphics::onDraw();
}
inline void EasyGraphics::setImmediateDrawMode(const bool immediate) { immediatemode=immediate; }
inline HDC EasyGraphics::getBackBuffer() { return hdcback; }
inline HWND EasyGraphics::getHWND() { return hwnd; }

inline int EasyGraphics::setTimer(int timerid, int elapsedms) { return (int) ::SetTimer(hwnd, timerid, elapsedms, NULL); }
inline void EasyGraphics::killTimer(int timerid) { ::KillTimer(hwnd, timerid); }

inline void EasyGraphics::waitForInput()
{
  if ((keylogger.tellp()-keylogger.tellg())==0)
  {
    WaitForSingleObject(keyevent, INFINITE);
    ResetEvent(keyevent);
  }
}

inline void EasyGraphics::setHDEFFont(HFONT font)
{
  if (font && font!=hdeffont)
  {
    ::DeleteObject(hdeffont);
    hdeffont=font;
  }
}


template<typename T> EasyGraphics& EasyGraphics::operator>>(T& val)
{
  waitForInput();
  keylogger >> val;
  return *this;
}

inline void EasyGraphics::getline(std::string& val)
{
  waitForInput();
  std::getline(keylogger, val);
}

inline char EasyGraphics::get() { return keylogger.get(); }
inline void EasyGraphics::clear() { keylogger.clear(); }
inline void EasyGraphics::ignore() { keylogger.ignore(); }


inline void EasyGraphics::calcCell(const int mousex, const int mousey, int& cellx, int& celly)
{
  cellx=(int) (mousex/scales[0]);
  celly=(int) (mousey/scales[1]);
}


/* Static class methods */
inline void EasyGraphics::registerWindow(EasyGraphics* window, HWND hwnd)
{
  windows=(EasyGraphics**) realloc(windows, sizeof(EasyGraphics*)*(noofwindows+1));
  windows[noofwindows++]=window;
  window->hwnd=hwnd;
}

// This method is a little big to inline, but at least it keeps all static members in one place... meh, live with it
inline void EasyGraphics::deregisterWindow(EasyGraphics* window)
{
  for (int i=0; i<noofwindows; i++)
  {
    if (windows[i]==window)
    {
      windows[i]=windows[--noofwindows];
      if (!noofwindows)
      {
        free(windows);
        windows=NULL;
      }
      break;
    }
  }
}

inline EasyGraphics* EasyGraphics::findWindow(HWND hwnd)
{
  for (int i=0; i<noofwindows; i++)
    if (windows[i]->hwnd==hwnd)
      return windows[i];
  return NULL;
}


// Member functions for ToolCache
template<typename T, typename S> EasyGraphics::ToolCache<T, S>::ToolCache(const int cachesize) : cachesize(cachesize) 
{ 
  items=new CACHE_PAIR[cachesize]; 
  for (int i=0; i<cachesize; i++)
    items[i].lastaccessed=0; 
}

template<typename T, typename S> const T* EasyGraphics::ToolCache<T, S>::getTool(S find)
{
  int lru=0;  // least recently used (incase we need to create it and trash an old one)
  for (int i=0; i<cachesize; i++)
  {
    if (items[i].item==find)
    {
      items[i].lastaccessed=::GetTickCount();
      return &items[i].item;
    }
    else if (items[i].lastaccessed<items[lru].lastaccessed)
      lru=i;
  }

  items[lru].item=find;
  items[lru].lastaccessed=::GetTickCount();
  return &items[lru].item;
}
