//
// GUIslice Library Examples
// - Calvin Hass
// - http://www.impulseadventure.com/elec/guislice-gui.html
// - Example 07 (LINUX):
//     Sliders with dynamic color control and position callback
//
#include "GUIslice.h"
#include "GUIslice_ex.h"
#include "GUIslice_drv.h"


// Defines for resources
#define FONT_DROID_SANS "/usr/share/fonts/truetype/droid/DroidSans.ttf"

// Enumerations for pages, elements, fonts, images
enum {E_PG_MAIN};
enum {E_ELEM_BOX,E_ELEM_BTN_QUIT,E_ELEM_COLOR,
      E_SLIDER_R,E_SLIDER_G,E_SLIDER_B};
enum {E_FONT_BTN,E_FONT_TXT,E_FONT_HEAD,E_FONT_TITLE};

bool      m_bQuit = false;

// Free-running counter for display
unsigned  m_nCount = 0;

// Instantiate the GUI
#define MAX_PAGE            1
#define MAX_FONT            5
#define MAX_ELEM_PG_MAIN    17

gslc_tsGui                  m_gui;
gslc_tsDriver               m_drv;
gslc_tsFont                 m_asFont[MAX_FONT];
gslc_tsPage                 m_asPage[MAX_PAGE];
gslc_tsElem                 m_asPageElem[MAX_ELEM_PG_MAIN];
gslc_tsElemRef              m_asPageElemRef[MAX_ELEM_PG_MAIN];

gslc_tsXSlider              m_sXSlider_R,m_sXSlider_G,m_sXSlider_B;


// Current RGB value for color box
// - Globals defined here for convenience so that callback
//   can update R,G,B components independently
uint16_t   m_nPosR = 255;
uint16_t   m_nPosG = 128;
uint16_t   m_nPosB = 0;

// Configure environment variables suitable for display
// - These may need modification to match your system
//   environment and display type
// - Defaults for GSLC_DEV_FB and GSLC_DEV_TOUCH are in GUIslice_config.h
// - Note that the environment variable settings can
//   also be set directly within the shell via export
//   (or init script).
//   - eg. export TSLIB_FBDEVICE=/dev/fb1
void UserInitEnv()
{
#if defined(DRV_DISP_SDL1) || defined(DRV_DISP_SDL2)
  setenv((char*)"FRAMEBUFFER",GSLC_DEV_FB,1);
  setenv((char*)"SDL_FBDEV",GSLC_DEV_FB,1);
  setenv((char*)"SDL_VIDEODRIVER",(char*)"fbcon",1);
#endif  
  
#if defined(DRV_TOUCH_TSLIB)
  setenv((char*)"TSLIB_FBDEVICE",GSLC_DEV_FB,1);
  setenv((char*)"TSLIB_TSDEVICE",GSLC_DEV_TOUCH,1); 
  setenv((char*)"TSLIB_CALIBFILE",(char*)"/etc/pointercal",1);
  setenv((char*)"TSLIB_CONFFILE",(char*)"/etc/ts.conf",1);
  setenv((char*)"TSLIB_PLUGINDIR",(char*)"/usr/local/lib/ts",1);
#endif
}

// Define debug message function
static int16_t DebugOut(char ch) { fputc(ch,stderr); return 0; }

// Quit button callback
bool CbBtnQuit(void* pvGui,void *pvElem,gslc_teTouch eTouch,int16_t nX,int16_t nY)
{
  if (eTouch == GSLC_TOUCH_UP_IN) {
    m_bQuit = true;
  }
  return true;
}

// Callback function for when a slider's position has been updated
// - After a slider position has been changed, update the color box
// - Note that all three sliders use the same callback for
//   convenience. From the element's ID we can determine which
//   slider was updated.
bool CbSlidePos(void* pvGui,void* pvElem,int16_t nPos)
{
  gslc_tsGui*     pGui    = (gslc_tsGui*)(pvGui);
  gslc_tsElem*    pElem   = (gslc_tsElem*)(pvElem);  
  //gslc_tsXSlider* pSlider = (gslc_tsXSlider*)(pElem->pXData);  
  
  // Fetch the new RGB component from the slider
  switch (pElem->nId) {
    case E_SLIDER_R:
      m_nPosR = gslc_ElemXSliderGetPos(pElem);
      break;
    case E_SLIDER_G:
      m_nPosG = gslc_ElemXSliderGetPos(pElem);      
      break;
    case E_SLIDER_B:
      m_nPosB = gslc_ElemXSliderGetPos(pElem);      
      break;
    default:
      break;
  }
  
  // Calculate the new RGB value
  gslc_tsColor colRGB = (gslc_tsColor){m_nPosR,m_nPosG,m_nPosB};
  
  // Update the color box
  gslc_tsElem* pElemColor = gslc_PageFindElemById(pGui,E_PG_MAIN,E_ELEM_COLOR);
  gslc_ElemSetCol(pElemColor,GSLC_COL_WHITE,colRGB,GSLC_COL_WHITE); 
  
  return true;
}

// Create page elements
bool InitOverlays()
{
  gslc_tsElem*  pElem = NULL;

  gslc_PageAdd(&m_gui,E_PG_MAIN,m_asPageElem,MAX_ELEM_PG_MAIN,m_asPageElemRef,MAX_ELEM_PG_MAIN);
  
  // Background flat color
  gslc_SetBkgndColor(&m_gui,GSLC_COL_GRAY_DK2);

  // Create Title with offset shadow
  pElem = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){2,2,320,50},
    "Home Automation",0,E_FONT_TITLE);
  gslc_ElemSetTxtCol(pElem,(gslc_tsColor){32,32,60});
  gslc_ElemSetTxtAlign(pElem,GSLC_ALIGN_MID_MID);
  gslc_ElemSetFillEn(pElem,false);
  pElem = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){0,0,320,50},
    "Home Automation",0,E_FONT_TITLE);
  gslc_ElemSetTxtCol(pElem,(gslc_tsColor){128,128,240});
  gslc_ElemSetTxtAlign(pElem,GSLC_ALIGN_MID_MID);
  gslc_ElemSetFillEn(pElem,false);
    
  // Create background box
  pElem = gslc_ElemCreateBox(&m_gui,E_ELEM_BOX,E_PG_MAIN,(gslc_tsRect){10,50,300,180});
  gslc_ElemSetCol(pElem,GSLC_COL_WHITE,GSLC_COL_BLACK,GSLC_COL_BLACK);

  // Create dividers
  pElem = gslc_ElemCreateBox(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){20,100,280,1});
  gslc_ElemSetCol(pElem,GSLC_COL_GRAY_DK3,GSLC_COL_BLACK,GSLC_COL_BLACK);  
  pElem = gslc_ElemCreateBox(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){235,60,1,35});
  gslc_ElemSetCol(pElem,GSLC_COL_GRAY_DK3,GSLC_COL_BLACK,GSLC_COL_BLACK);  

  // Create color box
  pElem = gslc_ElemCreateBox(&m_gui,E_ELEM_COLOR,E_PG_MAIN,(gslc_tsRect){20,90+30,130,100});
  gslc_tsColor colRGB = (gslc_tsColor){m_nPosR,m_nPosG,m_nPosB};
  gslc_ElemSetCol(pElem,GSLC_COL_WHITE,colRGB,GSLC_COL_WHITE); 

  
  // Create Quit button with text label
  pElem = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN_QUIT,E_PG_MAIN,
    (gslc_tsRect){250,60,50,30},"SAVE",0,E_FONT_BTN,&CbBtnQuit);
  gslc_ElemSetCol(pElem,GSLC_COL_BLUE_DK2,GSLC_COL_BLUE_DK4,GSLC_COL_BLUE_DK1);    
  gslc_ElemSetTxtCol(pElem,GSLC_COL_WHITE);

  // Create dummy selector
  pElem = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){20,65,100,20},
    "Selected Room:",0,E_FONT_HEAD);
  gslc_ElemSetTxtCol(pElem,GSLC_COL_GRAY_LT2);                        
  
  

  pElem = gslc_ElemCreateBtnTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,
   (gslc_tsRect){140,65,80,20},"Kitchen...",0,E_FONT_BTN,NULL); 
  gslc_ElemSetCol(pElem,GSLC_COL_GRAY_DK2,GSLC_COL_GRAY_DK3,GSLC_COL_BLUE_DK1);    
  gslc_ElemSetTxtCol(pElem,GSLC_COL_WHITE);
  
  // Create sliders
  // - Define element arrangement
  int16_t   nCtrlY    = 115;
  int16_t   nCtrlGap  = 30;
  uint16_t  nSlideW   = 80;
  uint16_t  nSlideH   = 20;
  int16_t   nLabelX   = 160;
  uint16_t  nLabelW   = 30;
  uint16_t  nLabelH   = 20;
  int16_t   nSlideX   = nLabelX + nLabelW + 20;
  
  pElem = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){nLabelX,nCtrlY,120,20},
    "Set LED RGB:",0,E_FONT_HEAD);
  gslc_ElemSetTxtCol(pElem,GSLC_COL_WHITE);
  nCtrlY += 25;
  

  // Create three sliders (R,G,B) and assign callback function
  // that is invoked upon change. The common callback will update
  // the color box.
  pElem = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){nLabelX,nCtrlY,nLabelW,nLabelH},
    "Red:",0,E_FONT_TXT);
  gslc_ElemSetTxtCol(pElem,GSLC_COL_GRAY_LT3);  

  pElem = gslc_ElemXSliderCreate(&m_gui,E_SLIDER_R,E_PG_MAIN,&m_sXSlider_R,
          (gslc_tsRect){nSlideX,nCtrlY,nSlideW,nSlideH},0,255,m_nPosR,5,false);
  gslc_ElemSetCol(pElem,GSLC_COL_RED,GSLC_COL_BLACK,GSLC_COL_BLACK);          
  gslc_ElemXSliderSetStyle(pElem,true,GSLC_COL_RED_DK4,10,5,GSLC_COL_GRAY_DK2);
  gslc_ElemXSliderSetPosFunc(pElem,&CbSlidePos);  
  nCtrlY += nCtrlGap;
  
  pElem = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){nLabelX,nCtrlY,nLabelW,nLabelH},
    "Green:",0,E_FONT_TXT);
  gslc_ElemSetTxtCol(pElem,GSLC_COL_GRAY_LT3);  
  
  pElem = gslc_ElemXSliderCreate(&m_gui,E_SLIDER_G,E_PG_MAIN,&m_sXSlider_G,
          (gslc_tsRect){nSlideX,nCtrlY,nSlideW,nSlideH},0,255,m_nPosG,5,false);
  gslc_ElemSetCol(pElem,GSLC_COL_GREEN,GSLC_COL_BLACK,GSLC_COL_BLACK);
  gslc_ElemXSliderSetStyle(pElem,true,GSLC_COL_GREEN_DK4,10,5,GSLC_COL_GRAY_DK2);
  gslc_ElemXSliderSetPosFunc(pElem,&CbSlidePos);    
  nCtrlY += nCtrlGap;
  
  
  pElem = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){nLabelX,nCtrlY,nLabelW,nLabelH},
    "Blue:",0,E_FONT_TXT);
  gslc_ElemSetTxtCol(pElem,GSLC_COL_GRAY_LT3);  
  
  pElem = gslc_ElemXSliderCreate(&m_gui,E_SLIDER_B,E_PG_MAIN,&m_sXSlider_B,
          (gslc_tsRect){nSlideX,nCtrlY,nSlideW,nSlideH},0,255,m_nPosB,5,false);
  gslc_ElemSetCol(pElem,GSLC_COL_BLUE,GSLC_COL_BLACK,GSLC_COL_BLACK);          
  gslc_ElemXSliderSetStyle(pElem,true,GSLC_COL_BLUE_DK4,10,5,GSLC_COL_GRAY_DK2);
  gslc_ElemXSliderSetPosFunc(pElem,&CbSlidePos);    
  nCtrlY += nCtrlGap;

  
  pElem = gslc_ElemCreateTxt(&m_gui,GSLC_ID_AUTO,E_PG_MAIN,(gslc_tsRect){250,230,60,10},
    "GUIslice Example",0,E_FONT_TXT);
  gslc_ElemSetTxtCol(pElem,GSLC_COL_BLACK); 
  gslc_ElemSetTxtAlign(pElem,GSLC_ALIGN_MID_RIGHT); 
  gslc_ElemSetFillEn(pElem,false); 

  return true;
}


int main( int argc, char* args[] )
{
  bool                bOk = true;

  // -----------------------------------
  // Initialize
  gslc_InitDebug(&DebugOut);
  UserInitEnv();
  if (!gslc_Init(&m_gui,&m_drv,m_asPage,MAX_PAGE,m_asFont,MAX_FONT)) { exit(1); }  
  
  // Load Fonts
  // - Normally we would select a number of different fonts
  bOk = gslc_FontAdd(&m_gui,E_FONT_BTN,FONT_DROID_SANS,14);
  if (!bOk) { fprintf(stderr,"ERROR: FontAdd failed\n"); exit(1); }
  bOk = gslc_FontAdd(&m_gui,E_FONT_TXT,FONT_DROID_SANS,10);
  if (!bOk) { fprintf(stderr,"ERROR: FontAdd failed\n"); exit(1); }
  bOk = gslc_FontAdd(&m_gui,E_FONT_HEAD,FONT_DROID_SANS,14);
  if (!bOk) { fprintf(stderr,"ERROR: FontAdd failed\n"); exit(1); }
  bOk = gslc_FontAdd(&m_gui,E_FONT_TITLE,FONT_DROID_SANS,36);
  if (!bOk) { fprintf(stderr,"ERROR: FontAdd failed\n"); exit(1); }

  // -----------------------------------
  // Start display
  InitOverlays();

  // Start up display on main page
  gslc_SetPageCur(&m_gui,E_PG_MAIN);
 
  // -----------------------------------
  // Main event loop

  m_bQuit = false;
  while (!m_bQuit) {

    // General counter
    m_nCount++;

    // Periodically call GUIslice update function
    gslc_Update(&m_gui);
    
  } // bQuit


  // -----------------------------------
  // Close down display

  gslc_Quit(&m_gui);

  return 0;
}
