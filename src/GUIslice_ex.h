#ifndef _GUISLICE_EX_H_
#define _GUISLICE_EX_H_

// =======================================================================
// GUIslice library (extensions)
// - Calvin Hass
// - http://www.impulseadventure.com/elec/guislice-gui.html
// =======================================================================
//
// The MIT License
//
// Copyright 2017 Calvin Hass
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =======================================================================


// Extended element definitions
// - This file extends the core GUIslice functionality with
//   additional widget types

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Extended element enumerations
  
/// Extended Element types
typedef enum {
    // Extended elements:
    GSLC_TYPEX_GAUGE        ///< Guage / progressbar extended element
            = GSLC_TYPE_BASE_EXTEND,  // Continue from gslc_teTypeBase enum range
    GSLC_TYPEX_CHECKBOX,    ///< Checkbox extended element
    GSLC_TYPEX_SLIDER,      ///< Slider extended element
    GSLC_TYPEX_SELNUM,      ///< SelNum extended element
} gslc_teTypeExtend;
  
  
// Extended element data structures
// - These data structures are maintained in the gslc_tsElem
//   structure via the pXData pointer

  
  
// ============================================================================
// Extended Element: Gauge
// ============================================================================

/// Extended data for Gauge element
typedef struct {
  int16_t         nGaugeMin;    ///< Minimum control value
  int16_t         nGaugeMax;    ///< Maximum control value
  int16_t         nGaugeVal;    ///< Current control value
  gslc_tsColor    colGauge;     ///< Color of gauge fill bar
  bool            bGaugeVert;   ///< Vertical if true, else Horizontal
} gslc_tsXGauge;


///
/// Create a Gauge Element
/// - Draws a horizontal or vertical box with a filled region
///   corresponding to the proportion that nVal represents
///   between nMin and nMax.
///
/// \param[in]  pGui:        Pointer to GUI
/// \param[in]  nElemId:     Element ID to assign (0..16383 or GSLC_ID_AUTO to autogen)
/// \param[in]  nPage:       Page ID to attach element to
/// \param[in]  pXData:      Ptr to extended element data structure
/// \param[in]  rElem:       Rectangle coordinates defining gauge size
/// \param[in]  nMin:        Minimum value of gauge for nVal comparison
/// \param[in]  nMax:        Maximum value of gauge for nVal comparison
/// \param[in]  nVal:        Starting value of gauge
/// \param[in]  colGauge:    Color to fill the gauge with
/// \param[in]  bVert:       Flag to indicate vertical vs horizontal action
///                          (true = vertical, false = horizontal)
///
/// \return Pointer to Element or NULL if failure
///
gslc_tsElem* gslc_ElemXGaugeCreate(gslc_tsGui* pGui,int16_t nElemId,int16_t nPage,
  gslc_tsXGauge* pXData,gslc_tsRect rElem,int16_t nMin,int16_t nMax,int16_t nVal,gslc_tsColor colGauge,bool bVert);

///
/// Update a Gauge element's current value
/// - Note that min & max values are assigned in create()
///
/// \param[in]  pElem:       Pointer to Element
/// \param[in]  nVal:        New value to show in gauge
///
/// \return none
///
void gslc_ElemXGaugeUpdate(gslc_tsElem* pElem,int16_t nVal);

///
/// Draw a gauge element on the screen
/// - Called from gslc_ElemDraw()
///
/// \param[in]  pvGui:       Void ptr to GUI (typecast to gslc_tsGui*)
/// \param[in]  pvElem:      Void ptr to Element (typecast to gslc_tsElem*)
///
/// \return true if success, false otherwise
///
bool gslc_ElemXGaugeDraw(void* pvGui,void* pvElem);



// ============================================================================
// Extended Element: Checkbox
// ============================================================================

/// Checkbox drawing style
typedef enum {
  GSLCX_CHECKBOX_STYLE_BOX,                 ///< Inner box
  GSLCX_CHECKBOX_STYLE_X,                   ///< Crossed
  GSLCX_CHECKBOX_STYLE_ROUND,               ///< Circular
} gslc_teXCheckboxStyle;

/// Extended data for Checkbox element
typedef struct {
  gslc_tsGui*                 pGui;         ///< Ptr to GUI (for radio group control)
  bool                        bRadio;       ///< Radio-button operation if true
  gslc_teXCheckboxStyle       nStyle;       ///< Drawing style for element
  bool                        bChecked;     ///< Indicates if it is selected (checked)
  gslc_tsColor                colCheck;     ///< Color of checked inner fill
} gslc_tsXCheckbox;


///
/// Create a Checkbox Element
///
/// \param[in]  pGui:        Pointer to GUI
/// \param[in]  nElemId:     Element ID to assign (0..16383 or GSLC_ID_AUTO to autogen)
/// \param[in]  nPage:       Page ID to attach element to
/// \param[in]  pXData:      Ptr to extended element data structure
/// \param[in]  rElem:       Rectangle coordinates defining checkbox size
/// \param[in]  bRadio:      Radio-button functionality if true
/// \param[in]  nStyle:      Drawing style for checkbox / radio button
/// \param[in]  colCheck:    Color for inner fill when checked
/// \param[in]  bChecked:    Default state
///
/// \return Element pointer or NULL if failure
///
gslc_tsElem* gslc_ElemXCheckboxCreate(gslc_tsGui* pGui,int16_t nElemId,int16_t nPage,
  gslc_tsXCheckbox* pXData,gslc_tsRect rElem,bool bRadio,
  gslc_teXCheckboxStyle nStyle,gslc_tsColor colCheck,bool bChecked);


///
/// Get a Checkbox element's current state
///
/// \param[in]  pElem:       Pointer to Element
///
/// \return Current state
///
bool gslc_ElemXCheckboxGetState(gslc_tsElem* pElem);


///
/// Set a Checkbox element's current state
///
/// \param[in]  pElem:       Pointer to Element
/// \param[in]  bChecked:    New state
///
/// \return none
///
void gslc_ElemXCheckboxSetState(gslc_tsElem* pElem,bool bChecked);

///
/// Find the checkbox within a group that has been checked
///
/// \param[in]  pGui:        Pointer to GUI
/// \param[in]  nGroupId:    Group ID to search
///
/// \return Element Ptr or NULL if none checked
///
gslc_tsElem* gslc_ElemXCheckboxFindChecked(gslc_tsGui* pGui,int16_t nGroupId);

///
/// Toggle a Checkbox element's current state
///
/// \param[in]  pElem:       Pointer to Element
///
/// \return none
///
void gslc_ElemXCheckboxToggleState(gslc_tsElem* pElem);


///
/// Draw a Checkbox element on the screen
/// - Called from gslc_ElemDraw()
///
/// \param[in]  pvGui:       Void ptr to GUI (typecast to gslc_tsGui*)
/// \param[in]  pvElem:      Void ptr to Element (typecast to gslc_tsElem*)
///
/// \return true if success, false otherwise
///
bool gslc_ElemXCheckboxDraw(void* pvGui,void* pvElem);

///
/// Handle touch events to Checkbox element
/// - Called from gslc_ElemSendEventTouch()
///
/// \param[in]  pvGui:       Void ptr to GUI (typecast to gslc_tsGui*)
/// \param[in]  pvElem:      Void ptr to Element (typecast to gslc_tsElem*)
/// \param[in]  eTouch:      Touch event type
/// \param[in]  nRelX:       Touch X coord relative to element
/// \param[in]  nRelY:       Touch Y coord relative to element
///
/// \return true if success, false otherwise
///
bool gslc_ElemXCheckboxTouch(void* pvGui,void* pvElem,gslc_teTouch eTouch,int16_t nRelX,int16_t nRelY);


// ============================================================================
// Extended Element: Slider
// ============================================================================

/// Callback function for slider feedback
typedef bool (*GSLC_CB_XSLIDER_POS)(void* pvGui,void* pvElem,int16_t nPos);

/// Extended data for Slider element
typedef struct {
  // Config
  bool            bVert;          ///< Orientation: true if vertical, else horizontal
  int16_t         nThumbSz;       ///< Size of the thumb control
  int16_t         nPosMin;        ///< Minimum position value of the slider
  int16_t         nPosMax;        ///< Maximum position value of the slider
  // Style config
  uint16_t        nTickDiv;       ///< Style: number of tickmark divisions (0 for none)
  int16_t         nTickLen;       ///< Style: length of tickmarks
  gslc_tsColor    colTick;        ///< Style: color of ticks
  bool            bTrim;          ///< Style: show a trim color
  gslc_tsColor    colTrim;        ///< Style: color of trim
  // State
  int16_t         nPos;           ///< Current position value of the slider
  // Callbacks
  GSLC_CB_XSLIDER_POS pfuncXPos;  ///< Callback func ptr for position update
} gslc_tsXSlider;


///
/// Create a Slider Element
///
/// \param[in]  pGui:        Pointer to GUI
/// \param[in]  nElemId:     Element ID to assign (0..16383 or GSLC_ID_AUTO to autogen)
/// \param[in]  nPage:       Page ID to attach element to
/// \param[in]  pXData:      Ptr to extended element data structure
/// \param[in]  rElem:       Rectangle coordinates defining checkbox size
/// \param[in]  nPosMin:     Minimum position value
/// \param[in]  nPosMax:     Maximum position value
/// \param[in]  nPos:        Starting position value
/// \param[in]  nThumbSz:    Size of the thumb control
/// \param[in]  bVert:       Orientation (true for vertical)
///
/// \return Element pointer or NULL if failure
///
gslc_tsElem* gslc_ElemXSliderCreate(gslc_tsGui* pGui,int16_t nElemId,int16_t nPage,
  gslc_tsXSlider* pXData,gslc_tsRect rElem,int16_t nPosMin,int16_t nPosMax,int16_t nPos,
  uint16_t nThumbSz,bool bVert);


///
/// Set a Slider element's current position
///
/// \param[in]  pElem:       Pointer to Element
/// \param[in]  bTrim:       Show a colored trim?
/// \param[in]  colTrim:     Color of trim
/// \param[in]  nTickDiv:    Number of tick divisions to show (0 for none)
/// \param[in]  nTickLen:    Length of tickmarks
/// \param[in]  colTick:     Color of ticks
///
/// \return none
///
void gslc_ElemXSliderSetStyle(gslc_tsElem* pElem,
        bool bTrim,gslc_tsColor colTrim,uint16_t nTickDiv,
        int16_t nTickLen,gslc_tsColor colTick);


///
/// Get a Slider element's current position
///
/// \param[in]  pElem:       Pointer to Element
///
/// \return Current slider position
///
int gslc_ElemXSliderGetPos(gslc_tsElem* pElem);


///
/// Set a Slider element's current position
/// 
/// \param[in]  pGui:        Pointer to GUI
/// \param[in]  pElem:       Pointer to Element
/// \param[in]  nPos:        New position value
///
/// \return none
///
void gslc_ElemXSliderSetPos(gslc_tsGui* pGui,gslc_tsElem* pElem,int16_t nPos);

///
/// Assign the position callback function for a slider
///
/// \param[in]  pElem:       Pointer to element
/// \param[in]  funcCb:      Function pointer to position routine (or NULL for none)
///
/// \return none
///
void gslc_ElemXSliderSetPosFunc(gslc_tsElem* pElem,GSLC_CB_XSLIDER_POS funcCb);

///
/// Draw a Slider element on the screen
/// - Called from gslc_ElemDraw()
///
/// \param[in]  pvGui:       Void ptr to GUI (typecast to gslc_tsGui*)
/// \param[in]  pvElem:      Void ptr to Element (typecast to gslc_tsElem*)
///
/// \return true if success, false otherwise
///
bool gslc_ElemXSliderDraw(void* pvGui,void* pvElem);

///
/// Handle touch events to Slider element
/// - Called from gslc_ElemSendEventTouch()
///
/// \param[in]  pvGui:       Void ptr to GUI (typecast to gslc_tsGui*)
/// \param[in]  pvElem:      Void ptr to Element (typecast to gslc_tsElem*)
/// \param[in]  eTouch:      Touch event type
/// \param[in]  nRelX:       Touch X coord relative to element
/// \param[in]  nRelY:       Touch Y coord relative to element
///
/// \return true if success, false otherwise
///
bool gslc_ElemXSliderTouch(void* pvGui,void* pvElem,gslc_teTouch eTouch,int16_t nRelX,int16_t nRelY);


// ============================================================================
// Extended Element: SelNum (Number Selector)
// - Demonstration of a compound element consisting of
//   a counter text field along with an increment and
//   decrement button.
// ============================================================================

#define SELNUM_STR_LEN  6

/// Extended data for SelNum element
typedef struct {

  // Core functionality for SelNum
  int16_t             nCounter;       ///< Counter for demo purposes
   
  // Internal sub-element members
  gslc_tsCollect      sCollect;       ///< Collection management for sub-elements
  gslc_tsElemRef      asElemRef[4];   ///< Storage for sub-element references
  gslc_tsElem         asElem[4];      ///< Storage for sub-elements

  #if (GSLC_LOCAL_STR == 0)
  // If elements don't provide their own internal string buffer, then
  // we need to provide storage here in the extended data of the compound
  // element. For now, simply provide a fixed-length memory buffer.
  char                acElemTxt[4][SELNUM_STR_LEN]; ///< Storage for strings
  #endif
} gslc_tsXSelNum;




///
/// Create a SelNum Element
///
/// \param[in]  pGui:        Pointer to GUI
/// \param[in]  nElemId:     Element ID to assign (0..16383 or GSLC_ID_AUTO to autogen)
/// \param[in]  nPage:       Page ID to attach element to
/// \param[in]  pXData:      Ptr to extended element data structure
/// \param[in]  rElem:       Rectangle coordinates defining element size
/// \param[in]  nFontId:     Font ID to use for drawing the element
///
/// \return Pointer to Element or NULL if failure
///
gslc_tsElem* gslc_ElemXSelNumCreate(gslc_tsGui* pGui,int16_t nElemId,int16_t nPage,
  gslc_tsXSelNum* pXData,gslc_tsRect rElem,int8_t nFontId);


///
/// Draw a SelNum element on the screen
/// - Called during redraw
///
/// \param[in]  pvGui:       Void ptr to GUI (typecast to gslc_tsGui*)
/// \param[in]  pvElem:      Void ptr to Element (typecast to gslc_tsElem*)
///
/// \return true if success, false otherwise
///
bool gslc_ElemXSelNumDraw(void* pvGui,void* pvElem);


///
/// Get the current counter associated with SelNum
///
/// \param[in]  pGui:        Ptr to GUI
/// \param[in]  pSelNum:     Ptr to Element
///
/// \return Current counter value
///
int gslc_ElemXSelNumGetCounter(gslc_tsGui* pGui,gslc_tsXSelNum* pSelNum);


///
/// Set the current counter associated with SelNum
///
/// \param[in]  pSelNum:     Ptr to Element
/// \param[in]  nCount:      New counter value
///
/// \return none
///
void gslc_ElemXSelNumSetCounter(gslc_tsXSelNum* pSelNum,int16_t nCount);


///
/// Handle a click event within the SelNum
/// - This is called internally by the SelNum touch handler
///
/// \param[in]  pvGui:       Void ptr to GUI (typecast to gslc_tsGui*)
/// \param[in]  pvElem:      Void ptr to Element (typecast to gslc_tsElem*)
/// \param[in]  eTouch:      Touch event type
/// \param[in]  nX:          Touch X coord
/// \param[in]  nY:          Touch Y coord
///
/// \return none
///
bool gslc_ElemXSelNumClick(void* pvGui,void *pvElem,gslc_teTouch eTouch,int16_t nX,int16_t nY);

///
/// Handle touch (up,down,move) events to SelNum element
/// - Called from gslc_ElemSendEventTouch()
///
/// \param[in]  pvGui:       Void ptr to GUI (typecast to gslc_tsGui*)
/// \param[in]  pvElem:      Void ptr to Element (typecast to gslc_tsElem*)
/// \param[in]  eTouch:      Touch event type
/// \param[in]  nRelX:       Touch X coord relative to element
/// \param[in]  nRelY:       Touch Y coord relative to element
///
/// \return true if success, false otherwise
///
bool gslc_ElemXSelNumTouch(void* pvGui,void* pvElem,gslc_teTouch eTouch,int16_t nRelX,int16_t nRelY);


// ============================================================================



#ifdef __cplusplus
}
#endif // __cplusplus
#endif // _GUISLICE_EX_H_

