/////////////////////////////////////////////////////////////////////////////
// Name:        treelistctrl.h
// Purpose:     wxDbTreeListCtrl class
// Author:      Robert Roebling
// Maintainer:  Otto Wyss
// Modified by: Anatoly Zamyatin
// Created:     01/02/97
// RCS-ID:      $Id: treelistctrl.h,v 1.36 2010/04/19 17:49:41 pgriddev Exp $
// Copyright:   (c) 2004 Robert Roebling, Julian Smart, Alberto Griggio,
//              Vadim Zeitlin, Otto Wyss
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////


#ifndef DBTREELISTCTRL_H
#define DBTREELISTCTRL_H

#if defined(__GNUG__) && !defined(__APPLE__)
    #pragma interface "dbtreelistctrl.h"
#endif

#include <wx/treectrl.h>
#include <wx/control.h>
#include <wx/pen.h>
#include <wx/listctrl.h> // for wxListEvent


class WXDLLEXPORT wxDbTreeListItem;
class WXDLLEXPORT wxDbTreeListHeaderWindow;
class WXDLLEXPORT wxDbTreeListMainWindow;
class WXDLLEXPORT wxDbEditDateCtrl;//Zamyatin
class WXDLLEXPORT wxDbEditCtrl;//Zamyatin
class WXDLLEXPORT wxDbEditListCtrl;//Zamyatin
class WXDLLEXPORT wxDbEditTextCtrl;//Zamyatin

#define wxTR_COLUMN_LINES 0x1000 // put border around items
#define wxTR_VIRTUAL      0x4000 // The application provides items text on demand.

// Using this typedef removes an ambiguity when calling Remove()
#ifdef __WXMSW__
#if !wxCHECK_VERSION(2, 5, 0)
typedef long wxTreeItemIdValue;
#else
typedef void *wxTreeItemIdValue;
#endif
#endif

//-----------------------------------------------------------------------------
// wxTreeListColumnAttrs
//-----------------------------------------------------------------------------

enum {
    DB_DEFAULT_COL_WIDTH = 100
};

class WXDLLEXPORT wxDbTreeListColumnInfo: public wxObject {

public:
    wxDbTreeListColumnInfo (const wxString &text = wxEmptyString,
                          int width = DB_DEFAULT_COL_WIDTH,
                          int flag = wxALIGN_LEFT,
                          int image = -1,
                          bool shown = true,
                          bool edit = false) {
        m_text = text;
        m_width = width;
        m_flag = flag;
        m_image = image;
        m_selected_image = -1;
        m_shown = shown;
        m_edit = edit;
    }

    wxDbTreeListColumnInfo (const wxDbTreeListColumnInfo& other) {
        m_text = other.m_text;
        m_width = other.m_width;
        m_flag = other.m_flag;
        m_image = other.m_image;
        m_selected_image = other.m_selected_image;
        m_shown = other.m_shown;
        m_edit = other.m_edit;
    }

    ~wxDbTreeListColumnInfo() {}

    // get/set
    wxString GetText() const { return m_text; }
    wxDbTreeListColumnInfo& SetText (const wxString& text) { m_text = text; return *this; }

    int GetWidth() const { return m_width; }
    wxDbTreeListColumnInfo& SetWidth (int width) { m_width = width; return *this; }

    int GetAlignment() const { return m_flag; }
    wxDbTreeListColumnInfo& SetAlignment (int flag) { m_flag = flag; return *this; }

    int GetImage() const { return m_image; }
    wxDbTreeListColumnInfo& SetImage (int image) { m_image = image; return *this; }

    int GetSelectedImage() const { return m_selected_image; }
    wxDbTreeListColumnInfo& SetSelectedImage (int image) { m_selected_image = image; return *this; }

    bool IsEditable() const { return m_edit; }
    wxDbTreeListColumnInfo& SetEditable (bool edit)
        { m_edit = edit; return *this; }

    bool IsShown() const { return m_shown; }
    wxDbTreeListColumnInfo& SetShown(bool shown) { m_shown = shown; return *this; }

private:
    wxString m_text;
    int m_width;
    int m_flag;
    int m_image;
    int m_selected_image;
    bool m_shown;
    bool m_edit;
};

//----------------------------------------------------------------------------
// wxDbTreeListCtrl - the multicolumn tree control
//----------------------------------------------------------------------------

// modes for navigation
const int wxDB_TL_MODE_NAV_FULLTREE = 0x0000; // default
const int wxDB_TL_MODE_NAV_EXPANDED = 0x0001;
const int wxDB_TL_MODE_NAV_VISIBLE  = 0x0002;
const int wxDB_TL_MODE_NAV_LEVEL    = 0x0004;

// modes for FindItem
const int wxDB_TL_MODE_FIND_EXACT   = 0x0000; // default
const int wxDB_TL_MODE_FIND_PARTIAL = 0x0010;
const int wxDB_TL_MODE_FIND_NOCASE  = 0x0020;

// additional flag for HitTest
const int wxDB_TREE_HITTEST_ONITEMCOLUMN = 0x2000;
extern WXDLLEXPORT const wxChar* wxDbTreeListCtrlNameStr;


class WXDLLEXPORT wxDbTreeListCtrl : public wxControl
{
friend class wxDbTreeListHeaderWindow;
friend class wxDbTreeListMainWindow;
friend class wxDbTreeListItem;
public:
    // creation
    // --------
    wxDbTreeListCtrl()
        : m_header_win(0), m_main_win(0), m_headerHeight(0)
    {}

    wxDbTreeListCtrl(wxWindow *parent, wxWindowID id = -1,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = wxTR_DEFAULT_STYLE,
               const wxValidator &validator = wxDefaultValidator,
               const wxString& name = wxDbTreeListCtrlNameStr )
        : m_header_win(0), m_main_win(0), m_headerHeight(0)
    {
        Create(parent, id, pos, size, style, validator, name);
    }

    virtual ~wxDbTreeListCtrl() {}

    bool Create(wxWindow *parent, wxWindowID id = -1,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxTR_DEFAULT_STYLE,
                const wxValidator &validator = wxDefaultValidator,
                const wxString& name = wxDbTreeListCtrlNameStr );

    void Refresh(bool erase=TRUE, const wxRect* rect=NULL);
    void SetFocus();
    // accessors
    // ---------

    // get the total number of items in the control
    size_t GetCount() const;

    // indent is the number of pixels the children are indented relative to
    // the parents position. SetIndent() also redraws the control
    // immediately.
    unsigned int GetIndent() const;
    void SetIndent(unsigned int indent);

    // line spacing is the space above and below the text on each line
    unsigned int GetLineSpacing() const;
    void SetLineSpacing(unsigned int spacing);

    // image list: these functions allow to associate an image list with
    // the control and retrieve it. Note that when assigned with
    // SetImageList, the control does _not_ delete
    // the associated image list when it's deleted in order to allow image
    // lists to be shared between different controls. If you use
    // AssignImageList, the control _does_ delete the image list.
    //
    // The normal image list is for the icons which correspond to the
    // normal tree item state (whether it is selected or not).
    // Additionally, the application might choose to show a state icon
    // which corresponds to an app-defined item state (for example,
    // checked/unchecked) which are taken from the state image list.
    wxImageList *GetImageList() const;
    wxImageList *GetStateImageList() const;
    wxImageList *GetButtonsImageList() const;

    void SetImageList(wxImageList *imageList);
    void SetStateImageList(wxImageList *imageList);
    void SetButtonsImageList(wxImageList *imageList);
    void AssignImageList(wxImageList *imageList);
    void AssignStateImageList(wxImageList *imageList);
    void AssignButtonsImageList(wxImageList *imageList);

    void SetToolTip(const wxString& tip);
    void SetToolTip (wxToolTip *tip);
    void SetItemToolTip(const wxTreeItemId& item, const wxString &tip);

    // Functions to work with columns

    // adds a column
    void AddColumn (const wxString& text,
                    int width = DB_DEFAULT_COL_WIDTH,
                    int flag = wxALIGN_LEFT,
                    int image = -1,
                    bool shown = true,
                    bool edit = false) {
        AddColumn (wxDbTreeListColumnInfo (text, width, flag, image, shown, edit));
    }
    void AddColumn (const wxDbTreeListColumnInfo& colInfo);

    // inserts a column before the given one
    void InsertColumn (int before,
                       const wxString& text,
                       int width = DB_DEFAULT_COL_WIDTH,
                       int flag = wxALIGN_LEFT,
                       int image = -1,
                       bool shown = true,
                       bool edit = false) {
        InsertColumn (before,
                      wxDbTreeListColumnInfo (text, width, flag, image, shown, edit));
    }
    void InsertColumn (int before, const wxDbTreeListColumnInfo& colInfo);

    // deletes the given column - does not delete the corresponding column
    void RemoveColumn (int column);

    // returns the number of columns in the ctrl
    int GetColumnCount() const;

    // tells which column is the "main" one, i.e. the "threaded" one
    void SetMainColumn (int column);
    int GetMainColumn() const;

    void SetColumn (int column, const wxDbTreeListColumnInfo& colInfo);
    wxDbTreeListColumnInfo& GetColumn (int column);
    const wxDbTreeListColumnInfo& GetColumn (int column) const;

    void SetColumnText (int column, const wxString& text);
    wxString GetColumnText (int column) const;

    void SetColumnWidth (int column, int width);
    int GetColumnWidth (int column) const;

    void SetColumnAlignment (int column, int flag);
    int GetColumnAlignment (int column) const;

    void SetColumnImage (int column, int image);
    int GetColumnImage (int column) const;

    void SetColumnShown (int column, bool shown = true);
    bool IsColumnShown (int column) const;

    void SetColumnEditable (int column, bool edit = true);
    bool IsColumnEditable (int column) const;

    // Functions to work with items.

    // accessors
    // ---------

    // retrieve item's label (of the main column)
    wxString GetItemText (const wxTreeItemId& item) const
        { return GetItemText (item, GetMainColumn()); }
    // retrieves item's label of the given column
    wxString GetItemText (const wxTreeItemId& item, int column) const;

    // get one of the images associated with the item (normal by default)
    int GetItemImage (const wxTreeItemId& item,
                      wxTreeItemIcon which = wxTreeItemIcon_Normal) const
    { return GetItemImage (item, GetMainColumn(), which); }
    int GetItemImage (const wxTreeItemId& item, int column,
                      wxTreeItemIcon which = wxTreeItemIcon_Normal) const;

    // get the data associated with the item
    wxTreeItemData *GetItemData (const wxTreeItemId& item) const;

    bool GetItemBold (const wxTreeItemId& item) const;
    wxColour GetItemTextColour (const wxTreeItemId& item) const;
    wxColour GetItemBackgroundColour (const wxTreeItemId& item) const;
    wxFont GetItemFont (const wxTreeItemId& item) const;

    // modifiers

    // set item's label
    void SetItemText (const wxTreeItemId& item, const wxString& text)
        { SetItemText (item, GetMainColumn(), text); }
    void SetItemText (const wxTreeItemId& item, int column, const wxString& text);

    // get one of the images associated with the item (normal by default)
    void SetItemImage (const wxTreeItemId& item, int image,
                       wxTreeItemIcon which = wxTreeItemIcon_Normal)
        { SetItemImage (item, GetMainColumn(), image, which); }
    // the which parameter is ignored for all columns but the main one
    void SetItemImage (const wxTreeItemId& item, int column, int image,
                       wxTreeItemIcon which = wxTreeItemIcon_Normal);

    // associate some data with the item
    void SetItemData (const wxTreeItemId& item, wxTreeItemData *data);

    // force appearance of [+] button near the item. This is useful to
    // allow the user to expand the items which don't have any children now
    // - but instead add them only when needed, thus minimizing memory
    // usage and loading time.
    void SetItemHasChildren(const wxTreeItemId& item, bool has = true);

    // the item will be shown in bold
    void SetItemBold (const wxTreeItemId& item, bool bold = true);

    // set the item's text colour
    void SetItemTextColour (const wxTreeItemId& item, const wxColour& colour);

    // set the item's background colour
    void SetItemBackgroundColour (const wxTreeItemId& item, const wxColour& colour);

    // set the item's font (should be of the same height for all items)
    void SetItemFont (const wxTreeItemId& item, const wxFont& font);

    // set the window font
    virtual bool SetFont ( const wxFont &font );

    // set the styles.
    void SetWindowStyle (const long styles);
    long GetWindowStyle() const;
    long GetWindowStyleFlag () const { return GetWindowStyle(); }

    // item status inquiries
    // ---------------------

    // is the item visible (it might be outside the view or not expanded)?
    bool IsVisible (const wxTreeItemId& item, bool fullRow = false, bool within = true) const;
    // does the item has any children?
    bool HasChildren (const wxTreeItemId& item) const;
    // is the item expanded (only makes sense if HasChildren())?
    bool IsExpanded (const wxTreeItemId& item) const;
    // is this item currently selected (the same as has focus)?
    bool IsSelected (const wxTreeItemId& item) const;
    // is item text in bold font?
    bool IsBold (const wxTreeItemId& item) const;
    // does the layout include space for a button?

    // number of children
    // ------------------

    // if 'recursively' is FALSE, only immediate children count, otherwise
    // the returned number is the number of all items in this branch
    size_t GetChildrenCount (const wxTreeItemId& item, bool recursively = true);

    // navigation
    // ----------

    // wxTreeItemId.IsOk() will return FALSE if there is no such item

    // get the root tree item
    wxTreeItemId GetRootItem() const;

    // get the item currently selected (may return NULL if no selection)
    wxTreeItemId GetSelection() const;

    // get the items currently selected, return the number of such item
    size_t GetSelections (wxArrayTreeItemIds&) const;

    // get the parent of this item (may return NULL if root)
    wxTreeItemId GetItemParent (const wxTreeItemId& item) const;

    // for this enumeration function you must pass in a "cookie" parameter
    // which is opaque for the application but is necessary for the library
    // to make these functions reentrant (i.e. allow more than one
    // enumeration on one and the same object simultaneously). Of course,
    // the "cookie" passed to GetFirstChild() and GetNextChild() should be
    // the same!

    // get child of this item
#if !wxCHECK_VERSION(2, 5, 0)
    wxTreeItemId GetFirstChild(const wxTreeItemId& item, long& cookie) const;
    wxTreeItemId GetNextChild(const wxTreeItemId& item, long& cookie) const;
    wxTreeItemId GetPrevChild(const wxTreeItemId& item, long& cookie) const;
    wxTreeItemId GetLastChild(const wxTreeItemId& item, long& cookie) const;
#else
    wxTreeItemId GetFirstChild(const wxTreeItemId& item, wxTreeItemIdValue& cookie) const;
    wxTreeItemId GetNextChild(const wxTreeItemId& item, wxTreeItemIdValue& cookie) const;
    wxTreeItemId GetPrevChild(const wxTreeItemId& item, wxTreeItemIdValue& cookie) const;
    wxTreeItemId GetLastChild(const wxTreeItemId& item, wxTreeItemIdValue& cookie) const;
#endif

    // get sibling of this item
    wxTreeItemId GetNextSibling(const wxTreeItemId& item) const;
    wxTreeItemId GetPrevSibling(const wxTreeItemId& item) const;

    // get item in the full tree (currently only for internal use)
    wxTreeItemId GetNext(const wxTreeItemId& item) const;
    wxTreeItemId GetPrev(const wxTreeItemId& item) const;

    // get expanded item, see IsExpanded()
    wxTreeItemId GetFirstExpandedItem() const;
    wxTreeItemId GetNextExpanded(const wxTreeItemId& item) const;
    wxTreeItemId GetPrevExpanded(const wxTreeItemId& item) const;

    // get visible item, see IsVisible()
    wxTreeItemId GetFirstVisibleItem(                      bool fullRow = false) const;
    wxTreeItemId GetFirstVisible(                          bool fullRow = false, bool within = true) const;
    wxTreeItemId GetNextVisible (const wxTreeItemId& item, bool fullRow = false, bool within = true) const;
    wxTreeItemId GetPrevVisible (const wxTreeItemId& item, bool fullRow = false, bool within = true) const;
    wxTreeItemId GetLastVisible (                          bool fullRow = false, bool within = true) const;

    // operations
    // ----------

    // add the root node to the tree
    wxTreeItemId AddRoot (const wxString& text,
                          int image = -1, int selectedImage = -1,
                          wxTreeItemData *data = NULL);

    // insert a new item in as the first child of the parent
    wxTreeItemId PrependItem (const wxTreeItemId& parent,
                              const wxString& text,
                              int image = -1, int selectedImage = -1,
                              wxTreeItemData *data = NULL);

    // insert a new item after a given one
    wxTreeItemId InsertItem (const wxTreeItemId& parent,
                             const wxTreeItemId& idPrevious,
                             const wxString& text,
                             int image = -1, int selectedImage = -1,
                             wxTreeItemData *data = NULL);

    // insert a new item before the one with the given index
    wxTreeItemId InsertItem (const wxTreeItemId& parent,
                             size_t index,
                             const wxString& text,
                             int image = -1, int selectedImage = -1,
                             wxTreeItemData *data = NULL);

    // insert a new item in as the last child of the parent
    wxTreeItemId AppendItem (const wxTreeItemId& parent,
                             const wxString& text,
                             int image = -1, int selectedImage = -1,
                             wxTreeItemData *data = NULL);

    // delete this item (except root) + children and associated data if any
    void Delete (const wxTreeItemId& item);
    // delete all children (but don't delete the item itself)
    void DeleteChildren (const wxTreeItemId& item);
    // delete the root and all its children from the tree
    void DeleteRoot();

    // expand this item
    void Expand (const wxTreeItemId& item);
    // expand this item and all subitems recursively
    void ExpandAll (const wxTreeItemId& item);
    // collapse the item without removing its children
    void Collapse (const wxTreeItemId& item);
    // collapse the item and remove all children
    void CollapseAndReset(const wxTreeItemId& item); //? TODO ???
    // toggles the current state
    void Toggle (const wxTreeItemId& item);

    // remove the selection from currently selected item (if any)
    void Unselect();
    void UnselectAll();
    // select this item - return true if selection was allowed (no veto)
    bool SelectItem (const wxTreeItemId& item,
                     const wxTreeItemId& last = (wxTreeItemId*)NULL,
                     bool unselect_others = true);
    // select all items in the expanded tree
    void SelectAll();
    // make sure this item is visible (expanding the parent item and/or
    // scrolling to this item if necessary)
    void EnsureVisible (const wxTreeItemId& item);
    // scroll to this item (but don't expand its parent)
    void ScrollTo (const wxTreeItemId& item);

    // The first function is more portable (because easier to implement
    // on other platforms), but the second one returns some extra info.
    wxTreeItemId HitTest (const wxPoint& point)
        { int flags; int column; return HitTest (point, flags, column); }
    wxTreeItemId HitTest (const wxPoint& point, int& flags)
        { int column; return HitTest (point, flags, column); }
    wxTreeItemId HitTest (const wxPoint& point, int& flags, int& column);

    // get the bounding rectangle of the item (or of its label only)
    bool GetBoundingRect (const wxTreeItemId& item, wxRect& rect,
                          bool textOnly = false) const;

    // Start editing the item label: this (temporarily) replaces the item
    // with a one line edit control. The item will be selected if it hadn't
    // been before.
    void EditLabel (const wxTreeItemId& item)
        { EditLabel (item, GetMainColumn()); }
    // edit item's label of the given column
    void EditLabel (const wxTreeItemId& item, int column);

    // virtual mode
    virtual wxString OnGetItemText( wxTreeItemData* item, long column ) const;

    // sorting
    // this function is called to compare 2 items and should return -1, 0
    // or +1 if the first item is less than, equal to or greater than the
    // second one. The base class version performs alphabetic comparaison
    // of item labels (GetText)
    virtual int OnCompareItems (const wxTreeItemId& item1, const wxTreeItemId& item2);
    // sort the children of this item using OnCompareItems
    // NB: this function is not reentrant and not MT-safe (FIXME)!
    void SortChildren(const wxTreeItemId& item);

    // searching
    wxTreeItemId FindItem (const wxTreeItemId& item, const wxString& str, int mode = 0);

    // overridden base class virtuals
    virtual bool SetBackgroundColour (const wxColour& colour);
    virtual bool SetForegroundColour (const wxColour& colour);

    // drop over item
    void SetDragItem (const wxTreeItemId& item = (wxTreeItemId*)NULL);


    virtual wxSize DoGetBestSize() const;

protected:
    // header window, responsible for column visualization and manipulation
    wxDbTreeListHeaderWindow* GetHeaderWindow() const
        { return m_header_win; }
    wxDbTreeListHeaderWindow* m_header_win;  // future cleanup: make private or remove GetHeaderWindow()

    // main window, the "true" tree ctrl
    wxDbTreeListMainWindow* GetMainWindow() const
        { return m_main_win; }
    wxDbTreeListMainWindow* m_main_win;  // future cleanup: make private or remove GetMainWindow()

    int GetHeaderHeight() const { return m_headerHeight; }

    void CalculateAndSetHeaderHeight();
    void DoHeaderLayout();
    void OnSize(wxSizeEvent& event);
	//Zamyatin
	//Редактируемая строка 
	wxTreeItemId  eId;
	//Редактируемая колонка 
	int  eCol;
	//Zamyatin
private:
    int m_headerHeight;


    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxDbTreeListCtrl)
};


/////////////////////////////////////////////////////////////////////////////
// Name:        treelistctrl.cpp
// Purpose:     multi column tree control implementation
// Author:      Robert Roebling
// Maintainer:  $Author: pgriddev $
// Created:     01/02/97
// RCS-ID:      $Id: treelistctrl.cpp,v 1.115 2010/06/26 16:37:41 pgriddev Exp $
// Copyright:   (c) 2004-2008 Robert Roebling, Julian Smart, Alberto Griggio,
//              Vadim Zeitlin, Otto Wyss, Ronan Chartois
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

// ===========================================================================
// declarations
// ===========================================================================

// ---------------------------------------------------------------------------
// headers
// ---------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(__APPLE__)
  #pragma implementation "dbtreelistctrl.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif


#include <wx/app.h>
#include <wx/treebase.h>
#include <wx/timer.h>
#include <wx/textctrl.h>
#include <wx/imaglist.h>
#include <wx/settings.h>
#include <wx/dcclient.h>
#include <wx/dcscreen.h>
#include <wx/scrolwin.h>
#include <wx/dcmemory.h>
#if wxCHECK_VERSION(2, 7, 0)
#include <wx/renderer.h>
#endif
#include <wx/apptrait.h>
#include <wx/dcbuffer.h>
#include <wx/tooltip.h>

#ifdef __WXMAC__
#include "wx/mac/private.h"
#endif

#include "dbtreelistctrl.h"

#include <wx/log.h>  // only required for debugging purpose


// ---------------------------------------------------------------------------
// array types
// ---------------------------------------------------------------------------

class  wxDbTreeListItem;


#include <wx/dynarray.h>
WX_DECLARE_OBJARRAY(wxDbTreeListColumnInfo, wxDbArrayTreeListColumnInfo);
//WX_DEFINE_ARRAY(wxDbTreeListItem *, wxArrayTreeListItems);
#if !wxCHECK_VERSION(2, 5, 0)
2WX_DEFINE_ARRAY(wxDbTreeListItem *, wxArrayTreeListItems);
#else
WX_DEFINE_ARRAY_PTR(wxDbTreeListItem *, wxArrayTreeListItems);
#endif




// --------------------------------------------------------------------------
// constants
// --------------------------------------------------------------------------

static const int NO_IMAGE = -1;

static const int LINEHEIGHT = 10;
static const int LINEATROOT = 5;
static const int MARGIN = 2;
static const int MININDENT = 16;
static const int BTNWIDTH = 9;
static const int BTNHEIGHT = 9;
static const int EXTRA_WIDTH = 4;
static const int EXTRA_HEIGHT = 4;
static const int HEADER_OFFSET_X = 0;  // changed from 1 to 0 on 2009.03.10 for Windows (other OS untested)
static const int HEADER_OFFSET_Y = 1;

static const int DRAG_TIMER_TICKS = 250; // minimum drag wait time in ms
static const int FIND_TIMER_TICKS = 500; // minimum find wait time in ms
static const int RENAME_TIMER_TICKS = 250; // minimum rename wait time in ms

extern const wxChar* wxDbTreeListCtrlNameStr; //= _T("treelistctrl");

static wxDbTreeListColumnInfo wxInvalidTreeListColumnInfo;


// ---------------------------------------------------------------------------
// private classes
// ---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  wxDbTreeListHeaderWindow (internal)
//-----------------------------------------------------------------------------

class  wxDbTreeListHeaderWindow : public wxWindow
{
protected:
    wxDbTreeListMainWindow *m_owner;
    const wxCursor *m_currentCursor;
    const wxCursor *m_resizeCursor;
    bool m_isDragging;

    // column being resized
    int m_column;

    // divider line position in logical (unscrolled) coords
    int m_currentX;

    // minimal position beyond which the divider line can't be dragged in
    // logical coords
    int m_minX;

    wxDbArrayTreeListColumnInfo m_columns;

    // total width of the columns
    int m_total_col_width;

#if wxCHECK_VERSION_FULL(2, 7, 0, 1)
    // which col header is currently highlighted with mouse-over
    int m_hotTrackCol;
    int XToCol(int x);
    void RefreshColLabel(int col);
#endif

public:
    wxDbTreeListHeaderWindow();

    wxDbTreeListHeaderWindow( wxWindow *win,
                            wxWindowID id,
                            wxDbTreeListMainWindow *owner,
                            const wxPoint &pos = wxDefaultPosition,
                            const wxSize &size = wxDefaultSize,
                            long style = 0,
                            const wxString &name = _T("wxDbTreeListCtrlcolumntitles") );

    virtual ~wxDbTreeListHeaderWindow();

    void DoDrawRect( wxDC *dc, int x, int y, int w, int h );
    void DrawCurrent();
    void AdjustDC(wxDC& dc);

    void OnPaint( wxPaintEvent &event );
    void OnEraseBackground(wxEraseEvent& WXUNUSED(event)) { ;; } // reduce flicker
    void OnMouse( wxMouseEvent &event );
    void OnSetFocus( wxFocusEvent &event );

    // total width of all columns
    int GetWidth() const { return m_total_col_width; }

    // column manipulation
    int GetColumnCount() const { return (int)m_columns.GetCount(); }

    void AddColumn (const wxDbTreeListColumnInfo& colInfo);

    void InsertColumn (int before, const wxDbTreeListColumnInfo& colInfo);

    void RemoveColumn (int column);

    // column information manipulation
    const wxDbTreeListColumnInfo& GetColumn (int column) const{
        wxCHECK_MSG ((column >= 0) && (column < GetColumnCount()),
                     wxInvalidTreeListColumnInfo, _T("Invalid column"));
        return m_columns[column];
    }
    wxDbTreeListColumnInfo& GetColumn (int column) {
        wxCHECK_MSG ((column >= 0) && (column < GetColumnCount()),
                     wxInvalidTreeListColumnInfo, _T("Invalid column"));
        return m_columns[column];
    }
    void SetColumn (int column, const wxDbTreeListColumnInfo& info);

    wxString GetColumnText (int column) const {
        wxCHECK_MSG ((column >= 0) && (column < GetColumnCount()),
                     wxEmptyString, _T("Invalid column"));
        return m_columns[column].GetText();
    }
    void SetColumnText (int column, const wxString& text) {
        wxCHECK_RET ((column >= 0) && (column < GetColumnCount()),
                     _T("Invalid column"));
        m_columns[column].SetText (text);
    }

    int GetColumnAlignment (int column) const {
        wxCHECK_MSG ((column >= 0) && (column < GetColumnCount()),
                     wxALIGN_LEFT, _T("Invalid column"));
        return m_columns[column].GetAlignment();
    }
    void SetColumnAlignment (int column, int flag) {
        wxCHECK_RET ((column >= 0) && (column < GetColumnCount()),
                     _T("Invalid column"));
        m_columns[column].SetAlignment (flag);
    }

    int GetColumnWidth (int column) const {
        wxCHECK_MSG ((column >= 0) && (column < GetColumnCount()),
                     -1, _T("Invalid column"));
        return m_columns[column].GetWidth();
    }
    void SetColumnWidth (int column, int width);

    bool IsColumnEditable (int column) const {
        wxCHECK_MSG ((column >= 0) && (column < GetColumnCount()),
                     false, _T("Invalid column"));
        return m_columns[column].IsEditable();
    }

    bool IsColumnShown (int column) const {
        wxCHECK_MSG ((column >= 0) && (column < GetColumnCount()),
                     true, _T("Invalid column"));
        return m_columns[column].IsShown();
    }

    // needs refresh
    bool m_dirty;

private:
    // common part of all ctors
    void Init();

    void SendListEvent(wxEventType type, wxPoint pos);

    DECLARE_DYNAMIC_CLASS(wxDbTreeListHeaderWindow)
    DECLARE_EVENT_TABLE()
};


//-----------------------------------------------------------------------------
//  wxDbTreeListMainWindow (internal)
//-----------------------------------------------------------------------------

class wxDbEditTextControl;


// this is the "true" control
class  wxDbTreeListMainWindow: public wxScrolledWindow
{
public:
    // creation
    // --------
    wxDbTreeListMainWindow() { Init(); }

    wxDbTreeListMainWindow (wxDbTreeListCtrl *parent, wxWindowID id = -1,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = wxTR_DEFAULT_STYLE,
               const wxValidator &validator = wxDefaultValidator,
               const wxString& name = _T("wxDbTreeListMainWindow"))
    {
        Init();
        Create (parent, id, pos, size, style, validator, name);
    }

    virtual ~wxDbTreeListMainWindow();

    bool Create(wxDbTreeListCtrl *parent, wxWindowID id = -1,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxTR_DEFAULT_STYLE,
                const wxValidator &validator = wxDefaultValidator,
                const wxString& name = _T("wxDbTreeListCtrl"));

    // accessors
    // ---------

    // return true if this is a virtual list control
    bool IsVirtual() const { return HasFlag(wxTR_VIRTUAL); }

    // get the total number of items in the control
    size_t GetCount() const;

    // indent is the number of pixels the children are indented relative to
    // the parents position. SetIndent() also redraws the control
    // immediately.
    unsigned int GetIndent() const { return m_indent; }
    void SetIndent(unsigned int indent);

    // see wxDbTreeListCtrl for the meaning
    unsigned int GetLineSpacing() const { return m_linespacing; }
    void SetLineSpacing(unsigned int spacing);

    // image list: these functions allow to associate an image list with
    // the control and retrieve it. Note that when assigned with
    // SetImageList, the control does _not_ delete
    // the associated image list when it's deleted in order to allow image
    // lists to be shared between different controls. If you use
    // AssignImageList, the control _does_ delete the image list.

    // The normal image list is for the icons which correspond to the
    // normal tree item state (whether it is selected or not).
    // Additionally, the application might choose to show a state icon
    // which corresponds to an app-defined item state (for example,
    // checked/unchecked) which are taken from the state image list.
    wxImageList *GetImageList() const { return m_imageListNormal; }
    wxImageList *GetStateImageList() const { return m_imageListState; }
    wxImageList *GetButtonsImageList() const { return m_imageListButtons; }

    void SetImageList(wxImageList *imageList);
    void SetStateImageList(wxImageList *imageList);
    void SetButtonsImageList(wxImageList *imageList);
    void AssignImageList(wxImageList *imageList);
    void AssignStateImageList(wxImageList *imageList);
    void AssignButtonsImageList(wxImageList *imageList);

    void SetToolTip(const wxString& tip);
    void SetToolTip(wxToolTip *tip);
    void SetItemToolTip(const wxTreeItemId& item, const wxString &tip);

    // Functions to work with tree ctrl items.

    // accessors
    // ---------

    // retrieve item's label
    wxString GetItemText (const wxTreeItemId& item) const
    { return GetItemText (item, GetMainColumn()); }
    wxString GetItemText (const wxTreeItemId& item, int column) const;
    wxString GetItemText (wxTreeItemData* item, int column) const;

    // get one of the images associated with the item (normal by default)
    int GetItemImage (const wxTreeItemId& item,
                      wxTreeItemIcon which = wxTreeItemIcon_Normal) const
    { return GetItemImage (item, GetMainColumn(), which); }
    int GetItemImage (const wxTreeItemId& item, int column,
                      wxTreeItemIcon which = wxTreeItemIcon_Normal) const;

    // get the data associated with the item
    wxTreeItemData *GetItemData(const wxTreeItemId& item) const;

    bool GetItemBold(const wxTreeItemId& item) const;
    wxColour GetItemTextColour(const wxTreeItemId& item) const;
    wxColour GetItemBackgroundColour(const wxTreeItemId& item) const;
    wxFont GetItemFont(const wxTreeItemId& item) const;

    // modifiers
    // ---------

    // set item's label
    void SetItemText (const wxTreeItemId& item, const wxString& text)
    { SetItemText (item, GetMainColumn(), text); }
    void SetItemText (const wxTreeItemId& item, int column, const wxString& text);

    // get one of the images associated with the item (normal by default)
    void SetItemImage (const wxTreeItemId& item, int image,
                       wxTreeItemIcon which = wxTreeItemIcon_Normal)
    { SetItemImage (item, GetMainColumn(), image, which); }
    void SetItemImage (const wxTreeItemId& item, int column, int image,
                       wxTreeItemIcon which = wxTreeItemIcon_Normal);

    // associate some data with the item
    void SetItemData(const wxTreeItemId& item, wxTreeItemData *data);

    // force appearance of [+] button near the item. This is useful to
    // allow the user to expand the items which don't have any children now
    // - but instead add them only when needed, thus minimizing memory
    // usage and loading time.
    void SetItemHasChildren(const wxTreeItemId& item, bool has = true);

    // the item will be shown in bold
    void SetItemBold(const wxTreeItemId& item, bool bold = true);

    // set the item's text colour
    void SetItemTextColour(const wxTreeItemId& item, const wxColour& colour);

    // set the item's background colour
    void SetItemBackgroundColour(const wxTreeItemId& item, const wxColour& colour);

    // set the item's font (should be of the same height for all items)
    void SetItemFont(const wxTreeItemId& item, const wxFont& font);

    // set the window font
    virtual bool SetFont( const wxFont &font );

    // set the styles.  No need to specify a GetWindowStyle here since
    // the base wxWindow member function will do it for us
    void SetWindowStyle(const long styles);

    // item status inquiries
    // ---------------------

    // is the item visible (it might be outside the view or not expanded)?
    bool IsVisible(const wxTreeItemId& item, bool fullRow, bool within = true) const;
    // does the item has any children?
    bool HasChildren(const wxTreeItemId& item) const;
    // is the item expanded (only makes sense if HasChildren())?
    bool IsExpanded(const wxTreeItemId& item) const;
    // is this item currently selected (the same as has focus)?
    bool IsSelected(const wxTreeItemId& item) const;
    // is item text in bold font?
    bool IsBold(const wxTreeItemId& item) const;
        // does the layout include space for a button?

    // number of children
    // ------------------

    // if 'recursively' is false, only immediate children count, otherwise
    // the returned number is the number of all items in this branch
    size_t GetChildrenCount(const wxTreeItemId& item, bool recursively = true);

    // navigation
    // ----------

    // wxTreeItemId.IsOk() will return false if there is no such item

    // get the root tree item
    wxTreeItemId GetRootItem() const { return m_rootItem; }  // implict cast from wxDbTreeListItem *

    // get the item currently selected, only if a single item is selected
    wxTreeItemId GetSelection() const { return m_selectItem; }

    // get all the items currently selected, return count of items
    size_t GetSelections(wxArrayTreeItemIds&) const;

    // get the parent of this item (may return NULL if root)
    wxTreeItemId GetItemParent(const wxTreeItemId& item) const;

    // for this enumeration function you must pass in a "cookie" parameter
    // which is opaque for the application but is necessary for the library
    // to make these functions reentrant (i.e. allow more than one
    // enumeration on one and the same object simultaneously). Of course,
    // the "cookie" passed to GetFirstChild() and GetNextChild() should be
    // the same!

    // get child of this item
#if !wxCHECK_VERSION(2, 5, 0)
    wxTreeItemId GetFirstChild(const wxTreeItemId& item, long& cookie) const;
    wxTreeItemId GetNextChild(const wxTreeItemId& item, long& cookie) const;
    wxTreeItemId GetPrevChild(const wxTreeItemId& item, long& cookie) const;
    wxTreeItemId GetLastChild(const wxTreeItemId& item, long& cookie) const;
#else
    wxTreeItemId GetFirstChild(const wxTreeItemId& item, wxTreeItemIdValue& cookie) const;
    wxTreeItemId GetNextChild(const wxTreeItemId& item, wxTreeItemIdValue& cookie) const;
    wxTreeItemId GetPrevChild(const wxTreeItemId& item, wxTreeItemIdValue& cookie) const;
    wxTreeItemId GetLastChild(const wxTreeItemId& item, wxTreeItemIdValue& cookie) const;
#endif

    // get sibling of this item
    wxTreeItemId GetNextSibling(const wxTreeItemId& item) const;
    wxTreeItemId GetPrevSibling(const wxTreeItemId& item) const;

    // get item in the full tree (currently only for internal use)
    wxTreeItemId GetNext(const wxTreeItemId& item, bool fulltree = true) const;
    wxTreeItemId GetPrev(const wxTreeItemId& item, bool fulltree = true) const;

    // get expanded item, see IsExpanded()
    wxTreeItemId GetFirstExpandedItem() const;
    wxTreeItemId GetNextExpanded(const wxTreeItemId& item) const;
    wxTreeItemId GetPrevExpanded(const wxTreeItemId& item) const;

    // get visible item, see IsVisible()
    wxTreeItemId GetFirstVisible(                          bool fullRow, bool within) const;
    wxTreeItemId GetNextVisible (const wxTreeItemId& item, bool fullRow, bool within) const;
    wxTreeItemId GetPrevVisible (const wxTreeItemId& item, bool fullRow, bool within) const;
    wxTreeItemId GetLastVisible (                          bool fullRow, bool within) const;

    // operations
    // ----------

    // add the root node to the tree
    wxTreeItemId AddRoot (const wxString& text,
                          int image = -1, int selectedImage = -1,
                          wxTreeItemData *data = NULL);

    // insert a new item in as the first child of the parent
    wxTreeItemId PrependItem(const wxTreeItemId& parent,
                             const wxString& text,
                             int image = -1, int selectedImage = -1,
                             wxTreeItemData *data = NULL);

    // insert a new item after a given one
    wxTreeItemId InsertItem(const wxTreeItemId& parent,
                            const wxTreeItemId& idPrevious,
                            const wxString& text,
                            int image = -1, int selectedImage = -1,
                            wxTreeItemData *data = NULL);

    // insert a new item before the one with the given index
    wxTreeItemId InsertItem(const wxTreeItemId& parent,
                            size_t index,
                            const wxString& text,
                            int image = -1, int selectedImage = -1,
                            wxTreeItemData *data = NULL);

    // insert a new item in as the last child of the parent
    wxTreeItemId AppendItem(const wxTreeItemId& parent,
                            const wxString& text,
                            int image = -1, int selectedImage = -1,
                            wxTreeItemData *data = NULL);

    // delete this item and associated data if any
    void Delete(const wxTreeItemId& item);
    // delete all children (but don't delete the item itself)
    // NB: this won't send wxEVT_COMMAND_TREE_ITEM_DELETED events
    void DeleteChildren(const wxTreeItemId& item);
    // delete the root and all its children from the tree
    // NB: this won't send wxEVT_COMMAND_TREE_ITEM_DELETED events
    void DeleteRoot();

    // expand this item
    void Expand(const wxTreeItemId& item);
    // expand this item and all subitems recursively
    void ExpandAll(const wxTreeItemId& item);
    // collapse the item without removing its children
    void Collapse(const wxTreeItemId& item);
    // collapse the item and remove all children
    void CollapseAndReset(const wxTreeItemId& item);
    // toggles the current state
    void Toggle(const wxTreeItemId& item);

    // remove the selection from currently selected item (if any)
    void Unselect();
    void UnselectAll();
    // select this item
    bool SelectItem(const wxTreeItemId& item, const wxTreeItemId& prev = (wxTreeItemId*)NULL,
                    bool unselect_others = true);
    void SelectAll();
    // make sure this item is visible (expanding the parent item and/or
    // scrolling to this item if necessary)
    void EnsureVisible(const wxTreeItemId& item);
    // scroll to this item (but don't expand its parent)
    void ScrollTo(const wxTreeItemId& item);
    void AdjustMyScrollbars();

    // The first function is more portable (because easier to implement
    // on other platforms), but the second one returns some extra info.
    wxTreeItemId HitTest (const wxPoint& point)
        { int flags; int column; return HitTest (point, flags, column); }
    wxTreeItemId HitTest (const wxPoint& point, int& flags)
        { int column; return HitTest (point, flags, column); }
    wxTreeItemId HitTest (const wxPoint& point, int& flags, int& column);


    // get the bounding rectangle of the item (or of its label only)
    bool GetBoundingRect(const wxTreeItemId& item,
                         wxRect& rect,
                         bool textOnly = false) const;

    // Start editing the item label: this (temporarily) replaces the item
    // with a one line edit control. The item will be selected if it hadn't
    // been before.
    void EditLabel (const wxTreeItemId& item, int column);

    // sorting
    // this function is called to compare 2 items and should return -1, 0
    // or +1 if the first item is less than, equal to or greater than the
    // second one. The base class version performs alphabetic comparaison
    // of item labels (GetText)
    virtual int OnCompareItems(const wxTreeItemId& item1,
                               const wxTreeItemId& item2);
    // sort the children of this item using OnCompareItems
    //
    // NB: this function is not reentrant and not MT-safe (FIXME)!
    void SortChildren(const wxTreeItemId& item);

    // searching
    wxTreeItemId FindItem (const wxTreeItemId& item, const wxString& str, int mode = 0);

    // implementation only from now on

    // overridden base class virtuals
    virtual bool SetBackgroundColour(const wxColour& colour);
    virtual bool SetForegroundColour(const wxColour& colour);

    // drop over item
    void SetDragItem (const wxTreeItemId& item = (wxTreeItemId*)NULL);

    // callbacks
    void OnPaint( wxPaintEvent &event );
    void OnEraseBackground(wxEraseEvent& WXUNUSED(event)) { ;; } // to reduce flicker
    void OnSetFocus( wxFocusEvent &event );
    void OnKillFocus( wxFocusEvent &event );
    void OnChar( wxKeyEvent &event );
    void OnMouse( wxMouseEvent &event );
    void OnIdle( wxIdleEvent &event );
    void OnScroll(wxScrollWinEvent& event);
    void OnCaptureLost(wxMouseCaptureLostEvent & WXUNUSED(event)) { ;; }
	void OnKeyDown(wxKeyEvent &event);//Zamyatin 
    // implementation helpers
    int GetColumnCount() const
    { return m_owner->GetHeaderWindow()->GetColumnCount(); }

    void SetMainColumn (int column)
    { if ((column >= 0) && (column < GetColumnCount())) m_main_column = column; }

    int GetMainColumn() const { return m_main_column; }

    int GetBestColumnWidth (int column, wxTreeItemId parent = wxTreeItemId());
    int GetItemWidth (int column, wxDbTreeListItem *item);
    wxFont GetItemFont (wxDbTreeListItem *item);

    void SetFocus();

protected:
    wxDbTreeListCtrl* m_owner;

    int m_main_column;

    friend class wxDbTreeListItem;
    friend class wxDbTreeListRenameTimer;
    friend class wxDbEditTextControl;
	friend class wxDbEditDateCtrl;//Zamyatin
	friend class wxDbEditCtrl;//Zamyatin
	friend class wxDbTreeGrid;//Zamyatin
	friend class wxDbEditListCtrl;//Zamyatin
	friend class wxDbEditTextCtrl;//Zamyatin

    wxFont               m_normalFont;
    wxFont               m_boldFont;

    wxDbTreeListItem       *m_rootItem; // root item
    wxDbTreeListItem       *m_curItem; // current item, either selected or marked
    wxDbTreeListItem       *m_shiftItem; // item, where the shift key was pressed
    wxDbTreeListItem       *m_selectItem; // current selected item, not with wxTR_MULTIPLE

    int                  m_curColumn;

    int                  m_btnWidth, m_btnWidth2;
    int                  m_btnHeight, m_btnHeight2;
    int                  m_imgWidth, m_imgWidth2;
    int                  m_imgHeight, m_imgHeight2;
    unsigned short       m_indent;
    int                  m_lineHeight;
    unsigned short       m_linespacing;
    wxPen                m_dottedPen;
    wxBrush             *m_hilightBrush,
                        *m_hilightUnfocusedBrush;
    bool                 m_hasFocus;
public:
    bool                 m_dirty;

protected:
    bool                 m_ownsImageListNormal,
                         m_ownsImageListState,
                         m_ownsImageListButtons;
    bool                 m_lastOnSame;  // last click on the same item as prev
    bool                 m_left_down_selection;

    wxImageList         *m_imageListNormal,
                        *m_imageListState,
                        *m_imageListButtons;

    bool                 m_isDragStarted;  // set at the very beginning of dragging
    bool                 m_isDragging; // set once a drag begin event was fired
    wxPoint              m_dragStartPos;  // set whenever m_isDragStarted is set to true
    wxDbTreeListItem      *m_dragItem;
    int                  m_dragCol;

    wxDbTreeListItem       *m_editItem; // item, which is currently edited
    wxTimer             *m_editTimer;
    bool                 m_editAccept;  // currently unused, OnRenameAccept() argument makes it redundant
    wxString             m_editRes;
    int                  m_editCol;
    wxDbEditCtrl			*m_editControl;//Zamyatin

    // char navigation
    wxTimer             *m_findTimer;
    wxString             m_findStr;

    bool                 m_isItemToolTip;  // true if individual item tooltips were set (disable global tooltip)
    wxString             m_toolTip;  // global tooltip
    wxDbTreeListItem      *m_toolTipItem;  // item whose tip is currently shown (NULL==global, -1==not displayed)

    // the common part of all ctors
    void Init();

    // misc helpers
    wxTreeItemId DoInsertItem(const wxTreeItemId& parent,
                              size_t previous,
                              const wxString& text,
                              int image, int selectedImage,
                              wxTreeItemData *data);
    void DoDeleteItem (wxDbTreeListItem *item);
    void SetCurrentItem(wxDbTreeListItem *item);
    bool HasButtons(void) const
        { return (m_imageListButtons) || HasFlag (wxTR_TWIST_BUTTONS|wxTR_HAS_BUTTONS); }

    void CalculateLineHeight();
    int  GetLineHeight(wxDbTreeListItem *item) const;
    void PaintLevel( wxDbTreeListItem *item, wxDC& dc, int level, int &y,
                     int x_maincol);
    void PaintItem( wxDbTreeListItem *item, wxDC& dc);

    void CalculateLevel( wxDbTreeListItem *item, wxDC &dc, int level, int &y,
                         int x_maincol);
    void CalculatePositions();
    void CalculateSize( wxDbTreeListItem *item, wxDC &dc );

    void RefreshSubtree (wxDbTreeListItem *item);
    void RefreshLine (wxDbTreeListItem *item);
    // redraw all selected items
    void RefreshSelected();
    // RefreshSelected() recursive helper
    void RefreshSelectedUnder (wxDbTreeListItem *item);

    void OnRenameTimer();
    void OnRenameAccept(bool isCancelled);

    void FillArray(wxDbTreeListItem*, wxArrayTreeItemIds&) const;
    bool TagAllChildrenUntilLast (wxDbTreeListItem *crt_item, wxDbTreeListItem *last_item);
    bool TagNextChildren (wxDbTreeListItem *crt_item, wxDbTreeListItem *last_item);
    void UnselectAllChildren (wxDbTreeListItem *item );
    bool SendEvent(wxEventType event_type, wxDbTreeListItem *item = NULL, wxTreeEvent *event = NULL);  // returns true if processed

private:
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxDbTreeListMainWindow)
};


// timer used for enabling in-place edit
class  wxDbTreeListRenameTimer: public wxTimer
{
public:
    wxDbTreeListRenameTimer( wxDbTreeListMainWindow *owner );

    void Notify();

private:
    wxDbTreeListMainWindow   *m_owner;
};



// a tree item (NOTE: this class is storage only, does not generate events)
class  wxDbTreeListItem
{
public:
    // ctors & dtor
    wxDbTreeListItem() { m_data = NULL; m_toolTip = NULL; }
    wxDbTreeListItem( wxDbTreeListMainWindow *owner,
                    wxDbTreeListItem *parent,
                    const wxArrayString& text,
                    int image,
                    int selImage,
                    wxTreeItemData *data );

    ~wxDbTreeListItem();

    // trivial accessors
    wxArrayTreeListItems& GetChildren() { return m_children; }

    const wxString GetText() const
    {
        return GetText(0);
    }
    const wxString GetText (int column) const
    {
        if(m_text.GetCount() > 0)
        {
            if( IsVirtual() )   return m_owner->GetItemText( m_data, column );
            else                return m_text[column];
        }
        return wxEmptyString;
    }

    int GetImage (wxTreeItemIcon which = wxTreeItemIcon_Normal) const
        { return m_images[which]; }
    int GetImage (int column, wxTreeItemIcon which=wxTreeItemIcon_Normal) const
    {
        if(column == m_owner->GetMainColumn()) return m_images[which];
        if(column < (int)m_col_images.GetCount()) return m_col_images[column];
        return NO_IMAGE;
    }

    wxTreeItemData *GetData() const { return m_data; }

    const wxString * GetToolTip() const  {  return m_toolTip;  }

    // returns the current image for the item (depending on its
    // selected/expanded/whatever state)
    int GetCurrentImage() const;

    void SetText (const wxString &text );
    void SetText (int column, const wxString& text)
    {
        if (column < (int)m_text.GetCount()) {
            m_text[column] = text;
        }else if (column < m_owner->GetColumnCount()) {
            int howmany = m_owner->GetColumnCount();
            for (int i = (int)m_text.GetCount(); i < howmany; ++i) m_text.Add (wxEmptyString);
            m_text[column] = text;
        }
    }
    void SetImage (int image, wxTreeItemIcon which) { m_images[which] = image; }
    void SetImage (int column, int image, wxTreeItemIcon which)
    {
        if (column == m_owner->GetMainColumn()) {
            m_images[which] = image;
        }else if (column < (int)m_col_images.GetCount()) {
            m_col_images[column] = image;
        }else if (column < m_owner->GetColumnCount()) {
            int howmany = m_owner->GetColumnCount();
            for (int i = (int)m_col_images.GetCount(); i < howmany; ++i) m_col_images.Add (NO_IMAGE);
            m_col_images[column] = image;
        }
    }

    void SetData(wxTreeItemData *data) { m_data = data; }

    void SetToolTip(const wxString &tip) {
        if (m_toolTip) {
            delete m_toolTip; m_toolTip = NULL;
        }
        if (tip.length() > 0) {
            m_toolTip = new wxString(tip);
        }
    }

    void SetHasPlus(bool has = true) { m_hasPlus = has; }

    void SetBold(bool bold) { m_isBold = bold; }

    int GetX() const { return m_x; }
    int GetY() const { return m_y; }

    void SetX (int x) { m_x = x; }
    void SetY (int y) { m_y = y; }

    int  GetHeight() const { return m_height; }
    int  GetWidth()  const { return m_width; }

    void SetHeight (int height) { m_height = height; }
    void SetWidth (int width) { m_width = width; }

    int GetTextX() const { return m_text_x; }
    void SetTextX (int text_x) { m_text_x = text_x; }

    wxDbTreeListItem *GetItemParent() const { return m_parent; }

    // operations
    // deletes all children
    void DeleteChildren();

    // get count of all children (and grand children if 'recursively')
    size_t GetChildrenCount(bool recursively = true) const;

    void Insert(wxDbTreeListItem *child, size_t index)
    { m_children.Insert(child, index); }

    void GetSize( int &x, int &y, const wxDbTreeListMainWindow* );

    // return the item at given position (or NULL if no item), onButton is
    // true if the point belongs to the item's button, otherwise it lies
    // on the button's label
    wxDbTreeListItem *HitTest (const wxPoint& point,
                             const wxDbTreeListMainWindow *,
                             int &flags, int& column, int level);

    void Expand() { m_isCollapsed = false; }
    void Collapse() { m_isCollapsed = true; }

    void SetHilight( bool set = true ) { m_hasHilight = set; }

    // status inquiries
    bool HasChildren() const { return !m_children.IsEmpty(); }
    bool IsSelected()  const { return m_hasHilight != 0; }
    bool IsExpanded()  const { return !m_isCollapsed; }
    bool HasPlus()     const { return m_hasPlus || HasChildren(); }
    bool IsBold()      const { return m_isBold != 0; }
    bool IsVirtual()   const { return m_owner->IsVirtual(); }

    // attributes
    // get them - may be NULL
    wxTreeItemAttr *GetAttributes() const { return m_attr; }
    // get them ensuring that the pointer is not NULL
    wxTreeItemAttr& Attr()
    {
        if ( !m_attr )
        {
            m_attr = new wxTreeItemAttr;
            m_ownsAttr = true;
        }
        return *m_attr;
    }
    // set them
    void SetAttributes(wxTreeItemAttr *attr)
    {
        if ( m_ownsAttr ) delete m_attr;
        m_attr = attr;
        m_ownsAttr = false;
    }
    // set them and delete when done
    void AssignAttributes(wxTreeItemAttr *attr)
    {
        SetAttributes(attr);
        m_ownsAttr = true;
    }

private:
    wxDbTreeListMainWindow  *m_owner;        // control the item belongs to

    // since there can be very many of these, we save size by chosing
    // the smallest representation for the elements and by ordering
    // the members to avoid padding.
    wxArrayString        m_text;    // labels to be rendered for item

    wxTreeItemData      *m_data;         // user-provided data

    wxString            *m_toolTip;

    wxArrayTreeListItems m_children; // list of children
    wxDbTreeListItem      *m_parent;       // parent of this item

    wxTreeItemAttr      *m_attr;         // attributes???

    // tree ctrl images for the normal, selected, expanded and
    // expanded+selected states
    short               m_images[wxTreeItemIcon_Max];
    wxArrayShort        m_col_images; // images for the various columns (!= main)

    // main column item positions
    wxCoord             m_x;            // (virtual) offset from left (vertical line)
    wxCoord             m_y;            // (virtual) offset from top
    wxCoord             m_text_x;       // item offset from left
    short               m_width;        // width of this item
    unsigned char       m_height;       // height of this item

    // use bitfields to save size
    int                 m_isCollapsed :1;
    int                 m_hasHilight  :1; // same as focused
    int                 m_hasPlus     :1; // used for item which doesn't have
                                          // children but has a [+] button
    int                 m_isBold      :1; // render the label in bold font
    int                 m_ownsAttr    :1; // delete attribute when done
};




#endif // TREELISTCTRL_H

