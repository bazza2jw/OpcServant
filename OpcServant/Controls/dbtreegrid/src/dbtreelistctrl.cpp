
#include "../include/dbtreelistctrl.h"
#include "../include/editctrl.h"

#include "../include/dbdispatcher.h"//Zamyatin
#include "../include/dbtreegrid.h"

#include <wx/dynarray.h>
#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(wxDbArrayTreeListColumnInfo);
const wxChar* wxDbTreeListCtrlNameStr = _T("treelistctrl");

// ===========================================================================
// implementation
// ===========================================================================

// ---------------------------------------------------------------------------
// wxDbTreeListRenameTimer (internal)
// ---------------------------------------------------------------------------

wxDbTreeListRenameTimer::wxDbTreeListRenameTimer( wxDbTreeListMainWindow *owner )
{
    m_owner = owner;
}

void wxDbTreeListRenameTimer::Notify()
{
    m_owner->OnRenameTimer();
}

//-----------------------------------------------------------------------------
// wxDbEditTextControl (internal)
//-----------------------------------------------------------------------------

BEGIN_EVENT_TABLE (wxDbEditTextControl,wxTextCtrl)
    EVT_CHAR           (wxDbEditTextControl::OnChar)
    EVT_KEY_UP         (wxDbEditTextControl::OnKeyUp)
    EVT_KILL_FOCUS     (wxDbEditTextControl::OnKillFocus)
END_EVENT_TABLE()

wxDbEditTextControl::wxDbEditTextControl (wxWindow *parent,
                                const wxWindowID id,
                                bool *accept,
                                wxString *res,
                                wxDbTreeListMainWindow *owner,
                                const wxString &value,
                                const wxPoint &pos,
                                const wxSize &size,
                                int style,
                                const wxValidator& validator,
                                const wxString &name)
    : wxTextCtrl (parent, id, value, pos, size, style | wxSIMPLE_BORDER, validator, name)
{
    m_res = res;
    m_accept = accept;
    m_owner = owner;
    (*m_accept) = false;
    (*m_res) = wxEmptyString;
    m_startValue = value;
    m_finished = false;
}

wxDbEditTextControl::~wxDbEditTextControl() {
    EndEdit(true); // cancelled
}

void wxDbEditTextControl::EndEdit(bool isCancelled) {
    if (m_finished) return;
    m_finished = true;

    if (m_owner) {
        (*m_accept) = ! isCancelled;
        (*m_res) = isCancelled ? m_startValue : GetValue();
        m_owner->OnRenameAccept(*m_res == m_startValue);
        m_owner->m_editControl = NULL;
        m_owner->m_editItem = NULL;
        m_owner->SetFocus(); // This doesn't work. TODO.
        m_owner = NULL;
    }

    Destroy();
}

bool wxDbEditTextControl::Destroy() {
    Hide();
    wxTheApp->GetTraits()->ScheduleForDestroy(this);
    return true;
}

void wxDbEditTextControl::OnChar( wxKeyEvent &event )
{
    if (m_finished)
    {
        event.Skip();
        return;
    }
    if (event.GetKeyCode() == WXK_RETURN)
    {
        EndEdit(false);  // not cancelled
        return;
    }
    if (event.GetKeyCode() == WXK_ESCAPE)
    {
        EndEdit(true);  // cancelled
        return;
    }
    event.Skip();
}

void wxDbEditTextControl::OnKeyUp( wxKeyEvent &event )
{
    if (m_finished)
    {
        event.Skip();
        return;
    }

    // auto-grow the textctrl:
    wxSize parentSize = m_owner->GetSize();
    wxPoint myPos = GetPosition();
    wxSize mySize = GetSize();
    int sx, sy;
    GetTextExtent(GetValue() + _T("M"), &sx, &sy);
    if (myPos.x + sx > parentSize.x) sx = parentSize.x - myPos.x;
    if (mySize.x > sx) sx = mySize.x;
    SetSize(sx, -1);

    event.Skip();
}

void wxDbEditTextControl::OnKillFocus( wxFocusEvent &event )
{
    if (m_finished)
    {
        event.Skip();
        return;
    }

    EndEdit(false);  // not cancelled
}

//-----------------------------------------------------------------------------
//  wxDbTreeListHeaderWindow
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxDbTreeListHeaderWindow,wxWindow);

BEGIN_EVENT_TABLE(wxDbTreeListHeaderWindow,wxWindow)
    EVT_PAINT         (wxDbTreeListHeaderWindow::OnPaint)
    EVT_ERASE_BACKGROUND(wxDbTreeListHeaderWindow::OnEraseBackground) // reduce flicker
    EVT_MOUSE_EVENTS  (wxDbTreeListHeaderWindow::OnMouse)
    EVT_SET_FOCUS     (wxDbTreeListHeaderWindow::OnSetFocus)
END_EVENT_TABLE()


void wxDbTreeListHeaderWindow::Init()
{
    m_currentCursor = (wxCursor *) NULL;
    m_isDragging = false;
    m_dirty = false;
    m_total_col_width = 0;
#if wxCHECK_VERSION_FULL(2, 7, 0, 1)
    m_hotTrackCol = -1;
#endif

    // prevent any background repaint in order to reducing flicker
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}

wxDbTreeListHeaderWindow::wxDbTreeListHeaderWindow()
{
    Init();

    m_owner = (wxDbTreeListMainWindow *) NULL;
    m_resizeCursor = (wxCursor *) NULL;
}

wxDbTreeListHeaderWindow::wxDbTreeListHeaderWindow( wxWindow *win,
                                                wxWindowID id,
                                                wxDbTreeListMainWindow *owner,
                                                const wxPoint& pos,
                                                const wxSize& size,
                                                long style,
                                                const wxString &name )
    : wxWindow( win, id, pos, size, style, name )
{
    Init();

    m_owner = owner;
    m_resizeCursor = new wxCursor(wxCURSOR_SIZEWE);

#if !wxCHECK_VERSION(2, 5, 0)
    SetBackgroundColour (wxSystemSettings::GetSystemColour (wxSYS_COLOUR_BTNFACE));
#else
    SetBackgroundColour (wxSystemSettings::GetColour (wxSYS_COLOUR_BTNFACE));
#endif
}

wxDbTreeListHeaderWindow::~wxDbTreeListHeaderWindow()
{
    delete m_resizeCursor;
}

void wxDbTreeListHeaderWindow::DoDrawRect( wxDC *dc, int x, int y, int w, int h )
{
#if !wxCHECK_VERSION(2, 5, 0)
    wxPen pen (wxSystemSettings::GetSystemColour (wxSYS_COLOUR_BTNSHADOW ), 1, wxSOLID);
#else
    wxPen pen (wxSystemSettings::GetColour (wxSYS_COLOUR_BTNSHADOW ), 1, wxSOLID);
#endif

    const int m_corner = 1;

    dc->SetBrush( *wxTRANSPARENT_BRUSH );
#if defined( __WXMAC__  )
    dc->SetPen (pen);
#else // !GTK, !Mac
    dc->SetPen( *wxBLACK_PEN );
#endif
    dc->DrawLine( x+w-m_corner+1, y, x+w, y+h );  // right (outer)
    dc->DrawRectangle( x, y+h, w+1, 1 );          // bottom (outer)

#if defined( __WXMAC__  )
    pen = wxPen( wxColour( 0x88 , 0x88 , 0x88 ), 1, wxSOLID ); // Line colour
//    pen = wxPen( wxColour( 0x10 , 0x10 , 0x10 ), 1, wxSOLID );
#endif
    dc->SetPen( pen );
    dc->DrawLine( x+w-m_corner, y, x+w-1, y+h );  // right (inner)
    dc->DrawRectangle( x+1, y+h-1, w-2, 1 );      // bottom (inner)

    dc->SetPen( *wxWHITE_PEN );
    dc->DrawRectangle( x, y, w-m_corner+1, 1 );   // top (outer)
    dc->DrawRectangle( x, y, 1, h );              // left (outer)
    dc->DrawLine( x, y+h-1, x+1, y+h-1 );
    dc->DrawLine( x+w-1, y, x+w-1, y+1 );
}

// shift the DC origin to match the position of the main window horz
// scrollbar: this allows us to always use logical coords
void wxDbTreeListHeaderWindow::AdjustDC(wxDC& dc)
{
    int xpix;
    m_owner->GetScrollPixelsPerUnit( &xpix, NULL );
    int x;
    m_owner->GetViewStart( &x, NULL );

    // account for the horz scrollbar offset
    dc.SetDeviceOrigin( -x * xpix, 0 );
}

void wxDbTreeListHeaderWindow::OnPaint( wxPaintEvent &WXUNUSED(event) )
{
    wxAutoBufferedPaintDC dc( this );
    AdjustDC( dc );

    int x = HEADER_OFFSET_X;

    // width and height of the entire header window
    int w, h;
    GetClientSize( &w, &h );
    m_owner->CalcUnscrolledPosition(w, 0, &w, NULL);
    dc.SetBackgroundMode(wxTRANSPARENT);

#if wxCHECK_VERSION_FULL(2, 7, 0, 1)
    int numColumns = GetColumnCount();
    for ( int i = 0; i < numColumns && x < w; i++ )
    {
        if (!IsColumnShown (i)) continue; // do next column if not shown

        wxHeaderButtonParams params;

        // TODO: columnInfo should have label colours...
        params.m_labelColour = wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT );
        params.m_labelFont = GetFont();

        wxDbTreeListColumnInfo& column = GetColumn(i);
        int wCol = column.GetWidth();
        int flags = 0;
        wxRect rect(x, 0, wCol, h);
        x += wCol;

        if ( i == m_hotTrackCol)
            flags |= wxCONTROL_CURRENT;

        params.m_labelText = column.GetText();
        params.m_labelAlignment = column.GetAlignment();

        int image = column.GetImage();
        wxImageList* imageList = m_owner->GetImageList();
        if ((image != -1) && imageList)
            params.m_labelBitmap = imageList->GetBitmap(image);

        wxRendererNative::Get().DrawHeaderButton(this, dc, rect, flags, wxHDR_SORT_ICON_NONE, &params);
    }

    if (x < w) {
        wxRect rect(x, 0, w-x, h);
        wxRendererNative::Get().DrawHeaderButton(this, dc, rect);
    }

#else  // not 2.7.0.1+

    dc.SetFont( GetFont() );

    // do *not* use the listctrl colour for headers - one day we will have a
    // function to set it separately
    //dc.SetTextForeground( *wxBLACK );
#if !wxCHECK_VERSION(2, 5, 0)
    dc.SetTextForeground (wxSystemSettings::GetSystemColour( wxSYS_COLOUR_WINDOWTEXT ));
#else
    dc.SetTextForeground (wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT ));
#endif

    int numColumns = GetColumnCount();
    for ( int i = 0; i < numColumns && x < w; i++ )
    {
        if (!IsColumnShown (i)) continue; // do next column if not shown

        wxDbTreeListColumnInfo& column = GetColumn(i);
        int wCol = column.GetWidth();

        // the width of the rect to draw: make it smaller to fit entirely
        // inside the column rect
        int cw = wCol - 2;

#if !wxCHECK_VERSION(2, 7, 0)
        dc.SetPen( *wxWHITE_PEN );
        DoDrawRect( &dc, x, HEADER_OFFSET_Y, cw, h-2 );
#else
        wxRect rect(x, HEADER_OFFSET_Y, cw, h-2);
        wxRendererNative::GetDefault().DrawHeaderButton (this, dc, rect);
#endif

        // if we have an image, draw it on the right of the label
        int image = column.GetImage(); //item.m_image;
        int ix = -2, iy = 0;
        wxImageList* imageList = m_owner->GetImageList();
        if ((image != -1) && imageList) {
            imageList->GetSize (image, ix, iy);
        }

        // extra margins around the text label
        int text_width = 0;
        int text_x = x;
        int image_offset = cw - ix - 1;

        switch(column.GetAlignment()) {
        case wxALIGN_LEFT:
            text_x += EXTRA_WIDTH;
            cw -= ix + 2;
            break;
        case wxALIGN_RIGHT:
            dc.GetTextExtent (column.GetText(), &text_width, NULL);
            text_x += cw - text_width - EXTRA_WIDTH - MARGIN;
            image_offset = 0;
            break;
        case wxALIGN_CENTER:
            dc.GetTextExtent(column.GetText(), &text_width, NULL);
            text_x += (cw - text_width)/2 + ix + 2;
            image_offset = (cw - text_width - ix - 2)/2 - MARGIN;
            break;
        }

        // draw the image
        if ((image != -1) && imageList) {
            imageList->Draw (image, dc, x + image_offset/*cw - ix - 1*/,
                             HEADER_OFFSET_Y + (h - 4 - iy)/2,
                             wxIMAGELIST_DRAW_TRANSPARENT);
        }

        // draw the text clipping it so that it doesn't overwrite the column boundary
        wxDCClipper clipper(dc, x, HEADER_OFFSET_Y, cw, h - 4 );
        dc.DrawText (column.GetText(), text_x, HEADER_OFFSET_Y + EXTRA_HEIGHT );

        // next column
        x += wCol;
    }

    int more_w = m_owner->GetSize().x - x - HEADER_OFFSET_X;
    if (more_w > 0) {
#if !wxCHECK_VERSION(2, 7, 0)
        DoDrawRect (&dc, x, HEADER_OFFSET_Y, more_w, h-2 );
#else
        wxRect rect (x, HEADER_OFFSET_Y, more_w, h-2);
        wxRendererNative::GetDefault().DrawHeaderButton (this, dc, rect);
#endif
    }

#endif // 2.7.0.1
}

void wxDbTreeListHeaderWindow::DrawCurrent()
{
    int x1 = m_currentX;
    int y1 = 0;
    ClientToScreen (&x1, &y1);

    int x2 = m_currentX-1;
#ifdef __WXMSW__
    ++x2; // but why ????
#endif
    int y2 = 0;
    m_owner->GetClientSize( NULL, &y2 );
    m_owner->ClientToScreen( &x2, &y2 );

    wxScreenDC dc;
    dc.SetLogicalFunction (wxINVERT);
    dc.SetPen (wxPen (*wxBLACK, 2, wxSOLID));
    dc.SetBrush (*wxTRANSPARENT_BRUSH);

    AdjustDC(dc);
    dc.DrawLine (x1, y1, x2, y2);
    dc.SetLogicalFunction (wxCOPY);
    dc.SetPen (wxNullPen);
    dc.SetBrush (wxNullBrush);
}

#if wxCHECK_VERSION_FULL(2, 7, 0, 1)
int wxDbTreeListHeaderWindow::XToCol(int x)
{
    int colLeft = 0;
    int numColumns = GetColumnCount();
    for ( int col = 0; col < numColumns; col++ )
    {
        if (!IsColumnShown(col)) continue;
        wxDbTreeListColumnInfo& column = GetColumn(col);

        if ( x < (colLeft + column.GetWidth()) )
             return col;

        colLeft += column.GetWidth();
    }
    return -1;
}

void wxDbTreeListHeaderWindow::RefreshColLabel(int col)
{
    if ( col > GetColumnCount() )
        return;

    int x = 0;
    int width = 0;
    int idx = 0;
    do {
        if (!IsColumnShown(idx)) continue;
        wxDbTreeListColumnInfo& column = GetColumn(idx);
        x += width;
        width = column.GetWidth();
    } while (++idx <= col);

    m_owner->CalcScrolledPosition(x, 0, &x, NULL);
    RefreshRect(wxRect(x, 0, width, GetSize().GetHeight()));
}
#endif

void wxDbTreeListHeaderWindow::OnMouse (wxMouseEvent &event) {

    // we want to work with logical coords
    int x;
    m_owner->CalcUnscrolledPosition(event.GetX(), 0, &x, NULL);

#if wxCHECK_VERSION_FULL(2, 7, 0, 1)
    if ( event.Moving() )
    {
        int col = XToCol(x);
        if ( col != m_hotTrackCol )
        {
            // Refresh the col header so it will be painted with hot tracking
            // (if supported by the native renderer.)
            RefreshColLabel(col);

            // Also refresh the old hot header
            if ( m_hotTrackCol >= 0 )
                RefreshColLabel(m_hotTrackCol);

            m_hotTrackCol = col;
        }
    }

    if ( event.Leaving() && m_hotTrackCol >= 0 )
    {
        // Leaving the window so clear any hot tracking indicator that may be present
        RefreshColLabel(m_hotTrackCol);
        m_hotTrackCol = -1;
    }
#endif

    if (m_isDragging) {

        SendListEvent (wxEVT_COMMAND_LIST_COL_DRAGGING, event.GetPosition());

        // we don't draw the line beyond our window, but we allow dragging it
        // there
        int w = 0;
        GetClientSize( &w, NULL );
        m_owner->CalcUnscrolledPosition(w, 0, &w, NULL);
        w -= 6;

        // erase the line if it was drawn
        if (m_currentX < w) DrawCurrent();

        if (event.ButtonUp()) {
            m_isDragging = false;
            if (HasCapture()) ReleaseMouse();
            m_dirty = true;
            SetColumnWidth (m_column, m_currentX - m_minX);
            Refresh();
            SendListEvent (wxEVT_COMMAND_LIST_COL_END_DRAG, event.GetPosition());
        }else{
            m_currentX = wxMax (m_minX + 7, x);

            // draw in the new location
            if (m_currentX < w) DrawCurrent();
        }

    }else{ // not dragging

        m_minX = 0;
        bool hit_border = false;

        // end of the current column
        int xpos = 0;

        // find the column where this event occured
        int countCol = GetColumnCount();
        for (int column = 0; column < countCol; column++) {
            if (!IsColumnShown (column)) continue; // do next if not shown

            xpos += GetColumnWidth (column);
            m_column = column;
            if (abs (x-xpos) < 3) {
                // near the column border
                hit_border = true;
                break;
            }

            if (x < xpos) {
                // inside the column
                break;
            }

            m_minX = xpos;
        }

        if (event.LeftDown() || event.RightUp()) {
            if (hit_border && event.LeftDown()) {
                m_isDragging = true;
                CaptureMouse();
                m_currentX = x;
                DrawCurrent();
                SendListEvent (wxEVT_COMMAND_LIST_COL_BEGIN_DRAG, event.GetPosition());
            }else{ // click on a column
                wxEventType evt = event.LeftDown()? wxEVT_COMMAND_LIST_COL_CLICK:
                                                    wxEVT_COMMAND_LIST_COL_RIGHT_CLICK;
                SendListEvent (evt, event.GetPosition());
            }
        }else if (event.LeftDClick() && hit_border) {
            SetColumnWidth (m_column, m_owner->GetBestColumnWidth (m_column));
            Refresh();

        }else if (event.Moving()) {
            bool setCursor;
            if (hit_border) {
                setCursor = m_currentCursor == wxSTANDARD_CURSOR;
                m_currentCursor = m_resizeCursor;
            }else{
                setCursor = m_currentCursor != wxSTANDARD_CURSOR;
                m_currentCursor = wxSTANDARD_CURSOR;
            }
            if (setCursor) SetCursor (*m_currentCursor);
        }

    }
}

void wxDbTreeListHeaderWindow::OnSetFocus (wxFocusEvent &WXUNUSED(event)) {

    m_owner->SetFocus();
}

void wxDbTreeListHeaderWindow::SendListEvent (wxEventType type, wxPoint pos) {
    wxWindow *parent = GetParent();
    wxListEvent le (type, parent->GetId());
    le.SetEventObject (parent);
    le.m_pointDrag = pos;

    // the position should be relative to the parent window, not
    // this one for compatibility with MSW and common sense: the
    // user code doesn't know anything at all about this header
    // window, so why should it get positions relative to it?
    le.m_pointDrag.y -= GetSize().y;
    le.m_col = m_column;
    parent->GetEventHandler()->ProcessEvent (le);
}

void wxDbTreeListHeaderWindow::AddColumn (const wxDbTreeListColumnInfo& colInfo) {
    m_columns.Add (colInfo);
    m_total_col_width += colInfo.GetWidth();
    m_owner->AdjustMyScrollbars();
    m_owner->m_dirty = true;
}

void wxDbTreeListHeaderWindow::SetColumnWidth (int column, int width) {
    wxCHECK_RET ((column >= 0) && (column < GetColumnCount()), _T("Invalid column"));
    m_total_col_width -= m_columns[column].GetWidth();
    m_columns[column].SetWidth(width);
    m_total_col_width += width;
    m_owner->AdjustMyScrollbars();
    m_owner->m_dirty = true;
}

void wxDbTreeListHeaderWindow::InsertColumn (int before, const wxDbTreeListColumnInfo& colInfo) {
    wxCHECK_RET ((before >= 0) && (before < GetColumnCount()), _T("Invalid column"));
    m_columns.Insert (colInfo, before);
    m_total_col_width += colInfo.GetWidth();
    m_owner->AdjustMyScrollbars();
    m_owner->m_dirty = true;
}

void wxDbTreeListHeaderWindow::RemoveColumn (int column) {
    wxCHECK_RET ((column >= 0) && (column < GetColumnCount()), _T("Invalid column"));
    m_total_col_width -= m_columns[column].GetWidth();
    m_columns.RemoveAt (column);
    m_owner->AdjustMyScrollbars();
    m_owner->m_dirty = true;
}

void wxDbTreeListHeaderWindow::SetColumn (int column, const wxDbTreeListColumnInfo& info) {
    wxCHECK_RET ((column >= 0) && (column < GetColumnCount()), _T("Invalid column"));
    int w = m_columns[column].GetWidth();
    m_columns[column] = info;
    if (w != info.GetWidth()) {
        m_total_col_width += info.GetWidth() - w;
        m_owner->AdjustMyScrollbars();
    }
    m_owner->m_dirty = true;
}

// ---------------------------------------------------------------------------
// wxDbTreeListItem
// ---------------------------------------------------------------------------

wxDbTreeListItem::wxDbTreeListItem (wxDbTreeListMainWindow *owner,
                                wxDbTreeListItem *parent,
                                const wxArrayString& text,
                                int image, int selImage,
                                wxTreeItemData *data)
              : m_text (text) {

    m_images[wxTreeItemIcon_Normal] = image;
    m_images[wxTreeItemIcon_Selected] = selImage;
    m_images[wxTreeItemIcon_Expanded] = NO_IMAGE;
    m_images[wxTreeItemIcon_SelectedExpanded] = NO_IMAGE;

    m_data = data;
    m_toolTip = NULL;
    m_x = 0;
    m_y = 0;
    m_text_x = 0;

    m_isCollapsed = true;
    m_hasHilight = false;
    m_hasPlus = false;
    m_isBold = false;

    m_owner = owner;
    m_parent = parent;

    m_attr = (wxTreeItemAttr *)NULL;
    m_ownsAttr = false;

    // We don't know the height here yet.
    m_width = 0;
    m_height = 0;
}

wxDbTreeListItem::~wxDbTreeListItem() {
    delete m_data;
    if (m_toolTip) delete m_toolTip;
    if (m_ownsAttr) delete m_attr;

    wxASSERT_MSG( m_children.IsEmpty(), _T("please call DeleteChildren() before destructor"));
}

void wxDbTreeListItem::DeleteChildren () {
    m_children.Empty();
}

void wxDbTreeListItem::SetText (const wxString &text) {
    if (m_text.GetCount() > 0) {
        m_text[0] = text;
    }else{
        m_text.Add (text);
    }
}

size_t wxDbTreeListItem::GetChildrenCount (bool recursively) const {
    size_t count = m_children.Count();
    if (!recursively) return count;

    size_t total = count;
    for (size_t n = 0; n < count; ++n) {
        total += m_children[n]->GetChildrenCount();
    }
    return total;
}

void wxDbTreeListItem::GetSize (int &x, int &y, const wxDbTreeListMainWindow *theButton) {
    int bottomY = m_y + theButton->GetLineHeight (this);
    if (y < bottomY) y = bottomY;
    int width = m_x +  m_width;
    if ( x < width ) x = width;

    if (IsExpanded()) {
        size_t count = m_children.Count();
        for (size_t n = 0; n < count; ++n ) {
            m_children[n]->GetSize (x, y, theButton);
        }
    }
}

wxDbTreeListItem *wxDbTreeListItem::HitTest (const wxPoint& point,
                                         const wxDbTreeListMainWindow *theCtrl,
                                         int &flags, int& column, int level) {

    // reset any previous hit infos
    flags = 0;
    column = -1;

    // for a hidden root node, don't evaluate it, but do evaluate children
    if (!theCtrl->HasFlag(wxTR_HIDE_ROOT) || (level > 0)) {

        wxDbTreeListHeaderWindow* header_win = theCtrl->m_owner->GetHeaderWindow();

        // check for right of all columns (outside)
        if (point.x > header_win->GetWidth()) return (wxDbTreeListItem*) NULL;
        // else find column
        for (int x = 0, j = 0; j < theCtrl->GetColumnCount(); ++j) {
            if (!header_win->IsColumnShown(j)) continue;
            int w = header_win->GetColumnWidth (j);
            if (point.x >= x && point.x < x+w) {
                column = j;
                break;
            }
            x += w;
        }

        // evaluate if y-pos is okay
        int h = theCtrl->GetLineHeight (this);
        if ((point.y >= m_y) && (point.y <= m_y + h)) {

            // check for above/below middle
            int y_mid = m_y + h/2;
            if (point.y < y_mid) {
                flags |= wxTREE_HITTEST_ONITEMUPPERPART;
            }else{
                flags |= wxTREE_HITTEST_ONITEMLOWERPART;
            }

            // check for button hit
            if (HasPlus() && theCtrl->HasButtons()) {
                int bntX = m_x - theCtrl->m_btnWidth2;
                int bntY = y_mid - theCtrl->m_btnHeight2;
                if ((point.x >= bntX) && (point.x <= (bntX + theCtrl->m_btnWidth)) &&
                    (point.y >= bntY) && (point.y <= (bntY + theCtrl->m_btnHeight))) {
                    flags |= wxTREE_HITTEST_ONITEMBUTTON;
                    return this;
                }
            }

            // check for image hit
            if (theCtrl->m_imgWidth > 0) {
                int imgX = m_text_x - theCtrl->m_imgWidth - MARGIN;
                int imgY = y_mid - theCtrl->m_imgHeight2;
                if ((point.x >= imgX) && (point.x <= (imgX + theCtrl->m_imgWidth)) &&
                    (point.y >= imgY) && (point.y <= (imgY + theCtrl->m_imgHeight))) {
                    flags |= wxTREE_HITTEST_ONITEMICON;
                    return this;
                }
            }

            // check for label hit
            if ((point.x >= m_text_x) && (point.x <= (m_text_x + m_width))) {
                flags |= wxTREE_HITTEST_ONITEMLABEL;
                return this;
            }

            // check for indent hit after button and image hit
            if (point.x < m_x) {
                flags |= wxTREE_HITTEST_ONITEMINDENT;
// Ronan, 2008.07.17: removed, not consistent               column = -1; // considered not belonging to main column
                return this;
            }

            // check for right of label
            int end = 0;
            for (int i = 0; i <= theCtrl->GetMainColumn(); ++i) end += header_win->GetColumnWidth (i);
            if ((point.x > (m_text_x + m_width)) && (point.x <= end)) {
                flags |= wxTREE_HITTEST_ONITEMRIGHT;
// Ronan, 2008.07.17: removed, not consistent                column = -1; // considered not belonging to main column
                return this;
            }

            // else check for each column except main
            if (column >= 0 && column != theCtrl->GetMainColumn()) {
                flags |= wxDB_TREE_HITTEST_ONITEMCOLUMN;
                return this;
            }

            // no special flag or column found
            return this;

        }

        // if children not expanded, return no item
        if (!IsExpanded()) return (wxDbTreeListItem*) NULL;
    }

    // in any case evaluate children
    wxDbTreeListItem *child;
    size_t count = m_children.Count();
    for (size_t n = 0; n < count; n++) {
        child = m_children[n]->HitTest (point, theCtrl, flags, column, level+1);
        if (child) return child;
    }

    // not found
    return (wxDbTreeListItem*) NULL;
}

int wxDbTreeListItem::GetCurrentImage() const {
    int image = NO_IMAGE;
    if (IsExpanded()) {
        if (IsSelected()) {
            image = GetImage (wxTreeItemIcon_SelectedExpanded);
        }else{
            image = GetImage (wxTreeItemIcon_Expanded);
        }
    }else{ // not expanded
        if (IsSelected()) {
            image = GetImage (wxTreeItemIcon_Selected);
        }else{
            image = GetImage (wxTreeItemIcon_Normal);
        }
    }

    // maybe it doesn't have the specific image, try the default one instead
    if (image == NO_IMAGE) image = GetImage();

    return image;
}

// ---------------------------------------------------------------------------
// wxDbTreeListMainWindow implementation
// ---------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxDbTreeListMainWindow, wxScrolledWindow)

BEGIN_EVENT_TABLE(wxDbTreeListMainWindow, wxScrolledWindow)
    EVT_PAINT          (wxDbTreeListMainWindow::OnPaint)
    EVT_ERASE_BACKGROUND(wxDbTreeListMainWindow::OnEraseBackground) // to reduce flicker
    EVT_MOUSE_EVENTS   (wxDbTreeListMainWindow::OnMouse)
	
    EVT_KEY_DOWN       (wxDbTreeListMainWindow::OnKeyDown) //Zamyatin чтобы нормально первая клавиша передавалась в редактируемый объект 
    EVT_CHAR           (wxDbTreeListMainWindow::OnChar) 
    EVT_SET_FOCUS      (wxDbTreeListMainWindow::OnSetFocus)
    EVT_KILL_FOCUS     (wxDbTreeListMainWindow::OnKillFocus)
    EVT_IDLE           (wxDbTreeListMainWindow::OnIdle)
    EVT_SCROLLWIN      (wxDbTreeListMainWindow::OnScroll)
    EVT_MOUSE_CAPTURE_LOST(wxDbTreeListMainWindow::OnCaptureLost)
END_EVENT_TABLE()


// ---------------------------------------------------------------------------
// construction/destruction
// ---------------------------------------------------------------------------




void wxDbTreeListMainWindow::Init() {

    m_rootItem = (wxDbTreeListItem*)NULL;
    m_curItem = (wxDbTreeListItem*)NULL;
    m_shiftItem = (wxDbTreeListItem*)NULL;
    m_editItem = (wxDbTreeListItem*)NULL;
    m_selectItem = (wxDbTreeListItem*)NULL;

    m_curColumn = -1; // no current column

    m_hasFocus = false;
    m_dirty = false;

    m_lineHeight = LINEHEIGHT;
    m_indent = MININDENT; // min. indent
    m_linespacing = 4;

#if !wxCHECK_VERSION(2, 5, 0)
    m_hilightBrush = new wxBrush (wxSystemSettings::GetSystemColour (wxSYS_COLOUR_HIGHLIGHT), wxSOLID);
    m_hilightUnfocusedBrush = new wxBrush (wxSystemSettings::GetSystemColour (wxSYS_COLOUR_BTNSHADOW), wxSOLID);
#else
    m_hilightBrush = new wxBrush (wxSystemSettings::GetColour (wxSYS_COLOUR_HIGHLIGHT), wxSOLID);
    m_hilightUnfocusedBrush = new wxBrush (wxSystemSettings::GetColour (wxSYS_COLOUR_BTNSHADOW), wxSOLID);
#endif

    m_imageListNormal = (wxImageList *) NULL;
    m_imageListButtons = (wxImageList *) NULL;
    m_imageListState = (wxImageList *) NULL;
    m_ownsImageListNormal = m_ownsImageListButtons =
    m_ownsImageListState = false;

    m_imgWidth = 0, m_imgWidth2 = 0;
    m_imgHeight = 0, m_imgHeight2 = 0;
    m_btnWidth = 0, m_btnWidth2 = 0;
    m_btnHeight = 0, m_btnHeight2 = 0;

    m_isDragStarted = m_isDragging = false;
    m_dragItem = NULL;
    m_dragCol = -1;

    m_editTimer = new wxDbTreeListRenameTimer (this);
    m_editControl = NULL;

    m_lastOnSame = false;
    m_left_down_selection = false;

    m_findTimer = new wxTimer (this, -1);

#if defined( __WXMAC__ ) && defined(__WXMAC_CARBON__)
    m_normalFont.MacCreateThemeFont (kThemeViewsFont);
#else
    m_normalFont = wxSystemSettings::GetFont (wxSYS_DEFAULT_GUI_FONT);
#endif
    m_boldFont = wxFont( m_normalFont.GetPointSize(),
                         m_normalFont.GetFamily(),
                         m_normalFont.GetStyle(),
                         wxBOLD,
                         m_normalFont.GetUnderlined(),
                         m_normalFont.GetFaceName(),
                         m_normalFont.GetEncoding());

    m_toolTip.clear();
    m_toolTipItem = (wxDbTreeListItem *)-1;  // no tooltip displayed
    m_isItemToolTip = false;  // so far no item-specific tooltip
}

bool wxDbTreeListMainWindow::Create (wxDbTreeListCtrl *parent,
                                   wxWindowID id,
                                   const wxPoint& pos,
                                   const wxSize& size,
                                   long style,
                                   const wxValidator &validator,
                                   const wxString& name) {

#ifdef __WXMAC__
    if (style & wxTR_HAS_BUTTONS) style |= wxTR_MAC_BUTTONS;
    if (style & wxTR_HAS_BUTTONS) style &= ~wxTR_HAS_BUTTONS;
    style &= ~wxTR_LINES_AT_ROOT;
    style |= wxTR_NO_LINES;

    int major,minor;
    wxGetOsVersion( &major, &minor );
    if (major < 10) style |= wxTR_ROW_LINES;
#endif

    wxScrolledWindow::Create (parent, id, pos, size, style|wxHSCROLL|wxVSCROLL, name);

#if wxUSE_VALIDATORS
    SetValidator(validator);
#endif

#if !wxCHECK_VERSION(2, 5, 0)
    SetBackgroundColour (wxSystemSettings::GetSystemColour (wxSYS_COLOUR_LISTBOX));
#else
    SetBackgroundColour (wxSystemSettings::GetColour (wxSYS_COLOUR_LISTBOX));
#endif
    // prevent any background repaint in order to reducing flicker
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

#ifdef __WXMSW__
    {
        int i, j;
        wxBitmap bmp(8, 8);
        wxMemoryDC bdc;
        bdc.SelectObject(bmp);
        bdc.SetPen(*wxGREY_PEN);
        bdc.DrawRectangle(-1, -1, 10, 10);
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                if (!((i + j) & 1)) {
                    bdc.DrawPoint(i, j);
                }
            }
        }

        m_dottedPen = wxPen(bmp, 1);
    }
#else
//?    m_dottedPen = wxPen( *wxGREY_PEN, 1, wxDOT );  // too slow under XFree86
    m_dottedPen = wxPen( _T("grey"), 0, 0 ); // Bitmap based pen is not supported by GTK!
#endif

    m_owner = parent;
    m_main_column = 0;

    return true;
}

wxDbTreeListMainWindow::~wxDbTreeListMainWindow() {
    delete m_hilightBrush;
    delete m_hilightUnfocusedBrush;

    delete m_editTimer;
    delete m_findTimer;
    if (m_ownsImageListNormal) delete m_imageListNormal;
    if (m_ownsImageListState) delete m_imageListState;
    if (m_ownsImageListButtons) delete m_imageListButtons;

    if (m_editControl) {
        m_editControl->SetOwner(NULL);    // prevent control from calling us during delete
        delete m_editControl;
    }

    DeleteRoot();
}


//-----------------------------------------------------------------------------
// accessors
//-----------------------------------------------------------------------------

size_t wxDbTreeListMainWindow::GetCount() const {
    return m_rootItem == NULL? 0: m_rootItem->GetChildrenCount();
}

void wxDbTreeListMainWindow::SetIndent (unsigned int indent) {
    m_indent = wxMax ((unsigned)MININDENT, indent);
    m_dirty = true;
}

void wxDbTreeListMainWindow::SetLineSpacing (unsigned int spacing) {
    m_linespacing = spacing;
    m_dirty = true;
    CalculateLineHeight();
}

size_t wxDbTreeListMainWindow::GetChildrenCount (const wxTreeItemId& item,
                                               bool recursively) {
    wxCHECK_MSG (item.IsOk(), 0u, _T("invalid tree item"));
    return ((wxDbTreeListItem*)item.m_pItem)->GetChildrenCount (recursively);
}

void wxDbTreeListMainWindow::SetWindowStyle (const long styles) {
    // change to selection mode, reset selection
    if ((styles ^ m_windowStyle) & wxTR_MULTIPLE) { UnselectAll(); }
    // right now, just sets the styles.  Eventually, we may
    // want to update the inherited styles, but right now
    // none of the parents has updatable styles
    m_windowStyle = styles;
    m_dirty = true;
}

void wxDbTreeListMainWindow::SetToolTip(const wxString& tip) {
    m_isItemToolTip = false;
    m_toolTip = tip;
    m_toolTipItem = (wxDbTreeListItem *)-1;  // no tooltip displayed (force refresh)
}
void wxDbTreeListMainWindow::SetToolTip(wxToolTip *tip) {
    m_isItemToolTip = false;
    m_toolTip = (tip == NULL) ? wxString() : tip->GetTip();
    m_toolTipItem = (wxDbTreeListItem *)-1;  // no tooltip displayed (force refresh)
}

void wxDbTreeListMainWindow::SetItemToolTip(const wxTreeItemId& item, const wxString &tip) {
    wxCHECK_RET (item.IsOk(), _T("invalid tree item"));
    m_isItemToolTip = true;
    ((wxDbTreeListItem*) item.m_pItem)->SetToolTip(tip);
    m_toolTipItem = (wxDbTreeListItem *)-1;  // no tooltip displayed (force refresh)
}


//-----------------------------------------------------------------------------
// functions to work with tree items
//-----------------------------------------------------------------------------

int wxDbTreeListMainWindow::GetItemImage (const wxTreeItemId& item, int column,
                                        wxTreeItemIcon which) const {
    wxCHECK_MSG (item.IsOk(), -1, _T("invalid tree item"));
    return ((wxDbTreeListItem*) item.m_pItem)->GetImage (column, which);
}

wxTreeItemData *wxDbTreeListMainWindow::GetItemData (const wxTreeItemId& item) const {
    wxCHECK_MSG (item.IsOk(), NULL, _T("invalid tree item"));
    return ((wxDbTreeListItem*) item.m_pItem)->GetData();
}

bool wxDbTreeListMainWindow::GetItemBold (const wxTreeItemId& item) const {
    wxCHECK_MSG(item.IsOk(), false, _T("invalid tree item"));
    return ((wxDbTreeListItem *)item.m_pItem)->IsBold();
}

wxColour wxDbTreeListMainWindow::GetItemTextColour (const wxTreeItemId& item) const {
    wxCHECK_MSG (item.IsOk(), wxNullColour, _T("invalid tree item"));
    wxDbTreeListItem *pItem = (wxDbTreeListItem*) item.m_pItem;
    return pItem->Attr().GetTextColour();
}

wxColour wxDbTreeListMainWindow::GetItemBackgroundColour (const wxTreeItemId& item) const {
    wxCHECK_MSG (item.IsOk(), wxNullColour, _T("invalid tree item"));
    wxDbTreeListItem *pItem = (wxDbTreeListItem*) item.m_pItem;
    return pItem->Attr().GetBackgroundColour();
}

wxFont wxDbTreeListMainWindow::GetItemFont (const wxTreeItemId& item) const {
    wxCHECK_MSG (item.IsOk(), wxNullFont, _T("invalid tree item"));
    wxDbTreeListItem *pItem = (wxDbTreeListItem*) item.m_pItem;
    return pItem->Attr().GetFont();
}

void wxDbTreeListMainWindow::SetItemImage (const wxTreeItemId& item, int column,
                                         int image, wxTreeItemIcon which) {
    wxCHECK_RET (item.IsOk(), _T("invalid tree item"));
    wxDbTreeListItem *pItem = (wxDbTreeListItem*) item.m_pItem;
    pItem->SetImage (column, image, which);
    wxClientDC dc (this);
    CalculateSize (pItem, dc);
    RefreshLine (pItem);
}

void wxDbTreeListMainWindow::SetItemData (const wxTreeItemId& item,
                                        wxTreeItemData *data) {
    wxCHECK_RET (item.IsOk(), _T("invalid tree item"));
    ((wxDbTreeListItem*) item.m_pItem)->SetData(data);
}

void wxDbTreeListMainWindow::SetItemHasChildren (const wxTreeItemId& item,
                                               bool has) {
    wxCHECK_RET (item.IsOk(), _T("invalid tree item"));
    wxDbTreeListItem *pItem = (wxDbTreeListItem*) item.m_pItem;
    pItem->SetHasPlus (has);
    RefreshLine (pItem);
}

void wxDbTreeListMainWindow::SetItemBold (const wxTreeItemId& item, bool bold) {
    wxCHECK_RET (item.IsOk(), _T("invalid tree item"));
    wxDbTreeListItem *pItem = (wxDbTreeListItem*) item.m_pItem;
    if (pItem->IsBold() != bold) { // avoid redrawing if no real change
        pItem->SetBold (bold);
        RefreshLine (pItem);
    }
}

void wxDbTreeListMainWindow::SetItemTextColour (const wxTreeItemId& item,
                                              const wxColour& colour) {
    wxCHECK_RET (item.IsOk(), _T("invalid tree item"));
    wxDbTreeListItem *pItem = (wxDbTreeListItem*) item.m_pItem;
    pItem->Attr().SetTextColour (colour);
    RefreshLine (pItem);
}

void wxDbTreeListMainWindow::SetItemBackgroundColour (const wxTreeItemId& item,
                                                    const wxColour& colour) {
    wxCHECK_RET (item.IsOk(), _T("invalid tree item"));
    wxDbTreeListItem *pItem = (wxDbTreeListItem*) item.m_pItem;
    pItem->Attr().SetBackgroundColour (colour);
    RefreshLine (pItem);
}

void wxDbTreeListMainWindow::SetItemFont (const wxTreeItemId& item,
                                        const wxFont& font) {
    wxCHECK_RET (item.IsOk(), _T("invalid tree item"));
    wxDbTreeListItem *pItem = (wxDbTreeListItem*) item.m_pItem;
    pItem->Attr().SetFont (font);
    RefreshLine (pItem);
}

bool wxDbTreeListMainWindow::SetFont (const wxFont &font) {
    wxScrolledWindow::SetFont (font);
    m_normalFont = font;
    m_boldFont = wxFont (m_normalFont.GetPointSize(),
                         m_normalFont.GetFamily(),
                         m_normalFont.GetStyle(),
                         wxBOLD,
                         m_normalFont.GetUnderlined(),
                         m_normalFont.GetFaceName());
    CalculateLineHeight();
    return true;
}


// ----------------------------------------------------------------------------
// item status inquiries
// ----------------------------------------------------------------------------

bool wxDbTreeListMainWindow::IsVisible (const wxTreeItemId& item, bool fullRow, bool within) const {
    wxCHECK_MSG (item.IsOk(), false, _T("invalid tree item"));

    // An item is only visible if it's not a descendant of a collapsed item
    wxDbTreeListItem *pItem = (wxDbTreeListItem*) item.m_pItem;
    wxDbTreeListItem* parent = pItem->GetItemParent();
    while (parent) {
        if (parent == m_rootItem && HasFlag(wxTR_HIDE_ROOT)) break;
        if (!parent->IsExpanded()) return false;
        parent = parent->GetItemParent();
    }

    // and the item is only visible if it is currently (fully) within the view
    if (within) {
        wxSize clientSize = GetClientSize();
        wxRect rect;
        if ((!GetBoundingRect (item, rect)) ||
            ((!fullRow && rect.GetWidth() == 0) || rect.GetHeight() == 0) ||
            (rect.GetTop() < 0 || rect.GetBottom() >= clientSize.y) ||
            (!fullRow && (rect.GetLeft() < 0 || rect.GetRight() >= clientSize.x))) return false;
    }

    return true;
}

bool wxDbTreeListMainWindow::HasChildren (const wxTreeItemId& item) const {
    wxCHECK_MSG (item.IsOk(), false, _T("invalid tree item"));

    // consider that the item does have children if it has the "+" button: it
    // might not have them (if it had never been expanded yet) but then it
    // could have them as well and it's better to err on this side rather than
    // disabling some operations which are restricted to the items with
    // children for an item which does have them
    return ((wxDbTreeListItem*) item.m_pItem)->HasPlus();
}

bool wxDbTreeListMainWindow::IsExpanded (const wxTreeItemId& item) const {
    wxCHECK_MSG (item.IsOk(), false, _T("invalid tree item"));
    return ((wxDbTreeListItem*) item.m_pItem)->IsExpanded();
}

bool wxDbTreeListMainWindow::IsSelected (const wxTreeItemId& item) const {
    wxCHECK_MSG (item.IsOk(), false, _T("invalid tree item"));
    return ((wxDbTreeListItem*) item.m_pItem)->IsSelected();
}

bool wxDbTreeListMainWindow::IsBold (const wxTreeItemId& item) const {
    wxCHECK_MSG (item.IsOk(), false, _T("invalid tree item"));
    return ((wxDbTreeListItem*) item.m_pItem)->IsBold();
}

// ----------------------------------------------------------------------------
// navigation
// ----------------------------------------------------------------------------

wxTreeItemId wxDbTreeListMainWindow::GetItemParent (const wxTreeItemId& item) const {
    wxCHECK_MSG (item.IsOk(), wxTreeItemId(), _T("invalid tree item"));
    return ((wxDbTreeListItem*) item.m_pItem)->GetItemParent();
}

#if !wxCHECK_VERSION(2, 5, 0)
wxTreeItemId wxDbTreeListMainWindow::GetFirstChild (const wxTreeItemId& item,
                                                  long& cookie) const {
#else
wxTreeItemId wxDbTreeListMainWindow::GetFirstChild (const wxTreeItemId& item,
                                                  wxTreeItemIdValue& cookie) const {
#endif
    wxCHECK_MSG (item.IsOk(), wxTreeItemId(), _T("invalid tree item"));
    wxArrayTreeListItems& children = ((wxDbTreeListItem*) item.m_pItem)->GetChildren();
    cookie = 0;
    return (!children.IsEmpty())? wxTreeItemId(children.Item(0)): wxTreeItemId();
}

#if !wxCHECK_VERSION(2, 5, 0)
wxTreeItemId wxDbTreeListMainWindow::GetNextChild (const wxTreeItemId& item,
                                                 long& cookie) const {
#else
wxTreeItemId wxDbTreeListMainWindow::GetNextChild (const wxTreeItemId& item,
                                                 wxTreeItemIdValue& cookie) const {
#endif
    wxCHECK_MSG (item.IsOk(), wxTreeItemId(), _T("invalid tree item"));
    wxArrayTreeListItems& children = ((wxDbTreeListItem*) item.m_pItem)->GetChildren();
    // it's ok to cast cookie to long, we never have indices which overflow "void*"
    long *pIndex = ((long*)&cookie);
    return ((*pIndex)+1 < (long)children.Count())? wxTreeItemId(children.Item(++(*pIndex))): wxTreeItemId();
}

#if !wxCHECK_VERSION(2, 5, 0)
wxTreeItemId wxDbTreeListMainWindow::GetPrevChild (const wxTreeItemId& item,
                                                 long& cookie) const {
#else
wxTreeItemId wxDbTreeListMainWindow::GetPrevChild (const wxTreeItemId& item,
                                                 wxTreeItemIdValue& cookie) const {
#endif
    wxCHECK_MSG (item.IsOk(), wxTreeItemId(), _T("invalid tree item"));
    wxArrayTreeListItems& children = ((wxDbTreeListItem*) item.m_pItem)->GetChildren();
    // it's ok to cast cookie to long, we never have indices which overflow "void*"
    long *pIndex = (long*)&cookie;
    return ((*pIndex)-1 >= 0)? wxTreeItemId(children.Item(--(*pIndex))): wxTreeItemId();
}

#if !wxCHECK_VERSION(2, 5, 0)
wxTreeItemId wxDbTreeListMainWindow::GetLastChild (const wxTreeItemId& item,
                                                 long& cookie) const {
#else
wxTreeItemId wxDbTreeListMainWindow::GetLastChild (const wxTreeItemId& item,
                                                 wxTreeItemIdValue& cookie) const {
#endif
    wxCHECK_MSG (item.IsOk(), wxTreeItemId(), _T("invalid tree item"));
    wxArrayTreeListItems& children = ((wxDbTreeListItem*) item.m_pItem)->GetChildren();
    // it's ok to cast cookie to long, we never have indices which overflow "void*"
    long *pIndex = ((long*)&cookie);
    (*pIndex) = (long)children.Count();
    return (!children.IsEmpty())? wxTreeItemId(children.Last()): wxTreeItemId();
}

wxTreeItemId wxDbTreeListMainWindow::GetNextSibling (const wxTreeItemId& item) const {
    wxCHECK_MSG (item.IsOk(), wxTreeItemId(), _T("invalid tree item"));

    // get parent
    wxDbTreeListItem *i = (wxDbTreeListItem*) item.m_pItem;
    wxDbTreeListItem *parent = i->GetItemParent();
    if (!parent) return wxTreeItemId(); // root item doesn't have any siblings

    // get index
    wxArrayTreeListItems& siblings = parent->GetChildren();
    size_t index = siblings.Index (i);
    wxASSERT (index != (size_t)wxNOT_FOUND); // I'm not a child of my parent?
    return (index < siblings.Count()-1)? wxTreeItemId(siblings[index+1]): wxTreeItemId();
}

wxTreeItemId wxDbTreeListMainWindow::GetPrevSibling (const wxTreeItemId& item) const {
    wxCHECK_MSG (item.IsOk(), wxTreeItemId(), _T("invalid tree item"));

    // get parent
    wxDbTreeListItem *i = (wxDbTreeListItem*) item.m_pItem;
    wxDbTreeListItem *parent = i->GetItemParent();
    if (!parent) return wxTreeItemId(); // root item doesn't have any siblings

    // get index
    wxArrayTreeListItems& siblings = parent->GetChildren();
    size_t index = siblings.Index(i);
    wxASSERT (index != (size_t)wxNOT_FOUND); // I'm not a child of my parent?
    return (index >= 1)? wxTreeItemId(siblings[index-1]): wxTreeItemId();
}

// Only for internal use right now, but should probably be public
wxTreeItemId wxDbTreeListMainWindow::GetNext (const wxTreeItemId& item, bool fulltree) const {
    wxCHECK_MSG (item.IsOk(), wxTreeItemId(), _T("invalid tree item"));

    // if there are any children, return first child
    if (fulltree || ((wxDbTreeListItem*)item.m_pItem)->IsExpanded()) {
        wxArrayTreeListItems& children = ((wxDbTreeListItem*)item.m_pItem)->GetChildren();
        if (children.GetCount() > 0) return children.Item (0);
    }

    // get sibling of this item or of the ancestors instead
    wxTreeItemId next;
    wxTreeItemId parent = item;
    do {
        next = GetNextSibling (parent);
        parent = GetItemParent (parent);
    } while (!next.IsOk() && parent.IsOk());
    return next;
}

// Only for internal use right now, but should probably be public
wxTreeItemId wxDbTreeListMainWindow::GetPrev (const wxTreeItemId& item, bool fulltree) const {
    wxCHECK_MSG (item.IsOk(), wxTreeItemId(), _T("invalid tree item"));

    // if there are no previous sibling get parent
    wxTreeItemId prev = GetPrevSibling (item);
    if (! prev.IsOk()) return GetItemParent (item);

    // while previous sibling has children, return last
    while (fulltree || ((wxDbTreeListItem*)prev.m_pItem)->IsExpanded()) {
        wxArrayTreeListItems& children = ((wxDbTreeListItem*)prev.m_pItem)->GetChildren();
        if (children.GetCount() == 0) break;
        prev = children.Item (children.GetCount() - 1);
    }

    return prev;
}

wxTreeItemId wxDbTreeListMainWindow::GetFirstExpandedItem() const {
    return GetNextExpanded (GetRootItem());
}

wxTreeItemId wxDbTreeListMainWindow::GetNextExpanded (const wxTreeItemId& item) const {
    wxCHECK_MSG (item.IsOk(), wxTreeItemId(), _T("invalid tree item"));
    return GetNext (item, false);
}

wxTreeItemId wxDbTreeListMainWindow::GetPrevExpanded (const wxTreeItemId& item) const {
    wxCHECK_MSG (item.IsOk(), wxTreeItemId(), _T("invalid tree item"));
    return GetPrev (item, false);
}

wxTreeItemId wxDbTreeListMainWindow::GetFirstVisible(bool fullRow, bool within) const {
    if (HasFlag(wxTR_HIDE_ROOT) || ! IsVisible(GetRootItem(), fullRow, within)) {
        return GetNextVisible (GetRootItem(), fullRow, within);
    } else {
        return GetRootItem();
    }
}

wxTreeItemId wxDbTreeListMainWindow::GetNextVisible (const wxTreeItemId& item, bool fullRow, bool within) const {
    wxCHECK_MSG (item.IsOk(), wxTreeItemId(), _T("invalid tree item"));
    wxTreeItemId id = GetNext (item, false);
    while (id.IsOk()) {
        if (IsVisible (id, fullRow, within)) return id;
        id = GetNext (id, false);
    }
    return wxTreeItemId();
}

wxTreeItemId wxDbTreeListMainWindow::GetLastVisible ( bool fullRow, bool within) const {
    wxCHECK_MSG (GetRootItem().IsOk(), wxTreeItemId(), _T("invalid tree item"));
    wxTreeItemId id = GetRootItem();
    wxTreeItemId res = id;
    while ((id = GetNext (id, false)).IsOk()) {
        if (IsVisible (id, fullRow, within)) res = id;
    }
    return res;
}

wxTreeItemId wxDbTreeListMainWindow::GetPrevVisible (const wxTreeItemId& item, bool fullRow, bool within) const {
    wxCHECK_MSG (item.IsOk(), wxTreeItemId(), _T("invalid tree item"));
    wxTreeItemId id = GetPrev (item, true);
    while (id.IsOk()) {
        if (IsVisible (id, fullRow, within)) return id;
        id = GetPrev(id, true);
    }
    return wxTreeItemId();
}

// ----------------------------------------------------------------------------
// operations
// ----------------------------------------------------------------------------

// ----------------------------  ADD OPERATION  -------------------------------

wxTreeItemId wxDbTreeListMainWindow::DoInsertItem (const wxTreeItemId& parentId,
                                                 size_t previous,
                                                 const wxString& text,
                                                 int image, int selImage,
                                                 wxTreeItemData *data) {
    wxDbTreeListItem *parent = (wxDbTreeListItem*)parentId.m_pItem;
    wxCHECK_MSG (parent, wxTreeItemId(), _T("item must have a parent, at least root!") );
    m_dirty = true; // do this first so stuff below doesn't cause flicker

    wxArrayString arr;
    arr.Alloc (GetColumnCount());
    for (int i = 0; i < (int)GetColumnCount(); ++i) arr.Add (wxEmptyString);
    arr[m_main_column] = text;
    wxDbTreeListItem *item = new wxDbTreeListItem (this, parent, arr, image, selImage, data);
    if (data != NULL) {
#if !wxCHECK_VERSION(2, 5, 0)
        data->SetId ((long)item);
#else
        data->SetId (item);
#endif
    }
    parent->Insert (item, previous);

    return item;
}

wxTreeItemId wxDbTreeListMainWindow::AddRoot (const wxString& text,
                                            int image, int selImage,
                                            wxTreeItemData *data) {
    wxCHECK_MSG(!m_rootItem, wxTreeItemId(), _T("tree can have only one root"));
    wxCHECK_MSG(GetColumnCount(), wxTreeItemId(), _T("Add column(s) before adding the root item"));
    m_dirty = true; // do this first so stuff below doesn't cause flicker

    wxArrayString arr;
    arr.Alloc (GetColumnCount());
    for (int i = 0; i < (int)GetColumnCount(); ++i) arr.Add (wxEmptyString);
    arr[m_main_column] = text;
    m_rootItem = new wxDbTreeListItem (this, (wxDbTreeListItem *)NULL, arr, image, selImage, data);
    if (data != NULL) {
#if !wxCHECK_VERSION(2, 5, 0)
        data->SetId((long)m_rootItem);
#else
        data->SetId(m_rootItem);
#endif
    }
    if (HasFlag(wxTR_HIDE_ROOT)) {
        // if we will hide the root, make sure children are visible
        m_rootItem->SetHasPlus();
        m_rootItem->Expand();
#if !wxCHECK_VERSION(2, 5, 0)
        long cookie = 0;
#else
        wxTreeItemIdValue cookie = 0;
#endif
        // TODO: suspect that deleting and recreating a root can leave a number of members dangling
        //  (here m_curItem should actually be set via SetCurrentItem() )
        m_curItem = (wxDbTreeListItem*)GetFirstChild (m_rootItem, cookie).m_pItem;
    }
    return m_rootItem;
}

wxTreeItemId wxDbTreeListMainWindow::PrependItem (const wxTreeItemId& parent,
                                                const wxString& text,
                                                int image, int selImage,
                                                wxTreeItemData *data) {
    return DoInsertItem (parent, 0u, text, image, selImage, data);
}

wxTreeItemId wxDbTreeListMainWindow::InsertItem (const wxTreeItemId& parentId,
                                               const wxTreeItemId& idPrevious,
                                               const wxString& text,
                                               int image, int selImage,
                                               wxTreeItemData *data) {
    wxDbTreeListItem *parent = (wxDbTreeListItem*)parentId.m_pItem;
    wxCHECK_MSG (parent, wxTreeItemId(), _T("item must have a parent, at least root!") );

    int index = parent->GetChildren().Index((wxDbTreeListItem*) idPrevious.m_pItem);
    wxASSERT_MSG( index != wxNOT_FOUND,
                  _T("previous item in wxDbTreeListMainWindow::InsertItem() is not a sibling") );

    return DoInsertItem (parentId, ++index, text, image, selImage, data);
}

wxTreeItemId wxDbTreeListMainWindow::InsertItem (const wxTreeItemId& parentId,
                                               size_t before,
                                               const wxString& text,
                                               int image, int selImage,
                                               wxTreeItemData *data) {
    wxDbTreeListItem *parent = (wxDbTreeListItem*)parentId.m_pItem;
    wxCHECK_MSG (parent, wxTreeItemId(), _T("item must have a parent, at least root!") );

    return DoInsertItem (parentId, before, text, image, selImage, data);
}

wxTreeItemId wxDbTreeListMainWindow::AppendItem (const wxTreeItemId& parentId,
                                               const wxString& text,
                                               int image, int selImage,
                                               wxTreeItemData *data) {
    wxDbTreeListItem *parent = (wxDbTreeListItem*) parentId.m_pItem;
    wxCHECK_MSG (parent, wxTreeItemId(), _T("item must have a parent, at least root!") );

    return DoInsertItem (parent, parent->GetChildren().Count(), text, image, selImage, data);
}


// --------------------------  DELETE OPERATION  ------------------------------

void wxDbTreeListMainWindow::Delete (const wxTreeItemId& itemId) {
    if (! itemId.IsOk()) return;
    wxDbTreeListItem *item = (wxDbTreeListItem*) itemId.m_pItem;
    wxDbTreeListItem *parent = item->GetItemParent();
    wxCHECK_RET (item != m_rootItem, _T("invalid item, root may not be deleted this way!"));

    // recursive delete
    DoDeleteItem(item);

    // update parent --CAUTION: must come after delete itself, so that item's
    //  siblings may be found
    if (parent) {
        parent->GetChildren().Remove (item);  // remove by value
    }
}


void wxDbTreeListMainWindow::DeleteRoot() {
    if (! m_rootItem) return;

    SetCurrentItem((wxDbTreeListItem*)NULL);
    m_selectItem = (wxDbTreeListItem*)NULL;
    m_shiftItem = (wxDbTreeListItem*)NULL;

    DeleteChildren (m_rootItem);
    SendEvent(wxEVT_COMMAND_TREE_DELETE_ITEM, m_rootItem);
    delete m_rootItem; m_rootItem = NULL;
}


void wxDbTreeListMainWindow::DeleteChildren (const wxTreeItemId& itemId) {
    if (! itemId.IsOk()) return;
    wxDbTreeListItem *item = (wxDbTreeListItem*) itemId.m_pItem;

    // recursive delete on all children, starting from the right to prevent
    //  multiple selection changes (see m_curItem handling in DoDeleteItem() )
    wxArrayTreeListItems& children = item->GetChildren();
    for (size_t n = children.GetCount(); n>0; n--) {
        DoDeleteItem(children[n-1]);
        // immediately remove child from array, otherwise it might get selected
        // as current item (see m_curItem handling in DoDeleteItem() )
        children.RemoveAt(n-1);
    }
}


void wxDbTreeListMainWindow::DoDeleteItem(wxDbTreeListItem *item) {
    wxCHECK_RET (item, _T("invalid item for delete!"));

    m_dirty = true; // do this first so stuff below doesn't cause flicker

    // cancel any editing
    if (m_editControl) {
        m_editControl->EndEdit(true);  // cancelled
    }

    // cancel any dragging
    if (item == m_dragItem) {
        // stop dragging
        m_isDragStarted = m_isDragging = false;
        if (HasCapture()) ReleaseMouse();
    }

    // don't stay with invalid m_curItem: take next sibling or reset to NULL
    // NOTE: this might be slighty inefficient when deleting a whole tree
    //  but has the advantage that all deletion side-effects are handled here
    if (item == m_curItem) {
        SetCurrentItem(item->GetItemParent());
        if (m_curItem) {
            wxArrayTreeListItems& siblings = m_curItem->GetChildren();
            size_t index = siblings.Index (item);
            wxASSERT (index != (size_t)wxNOT_FOUND); // I'm not a child of my parent?
            SetCurrentItem(index < siblings.Count()-1 ? siblings[index+1]: (wxDbTreeListItem*)NULL);
        }
    }
    // don't stay with invalid m_shiftItem: reset it to NULL
    if (item == m_shiftItem) m_shiftItem = (wxDbTreeListItem*)NULL;
    // don't stay with invalid m_selectItem: default to current item
    if (item == m_selectItem) {
        m_selectItem = m_curItem;
        SelectItem(m_selectItem, (wxTreeItemId*)NULL, true);  // unselect others
    }

    // recurse children, starting from the right to prevent multiple selection
    //  changes (see m_curItem handling above)
    wxArrayTreeListItems& children = item->GetChildren();
    for (size_t n = children.GetCount(); n>0; n--) {
        DoDeleteItem(children[n-1]);
        // immediately remove child from array, otherwise it might get selected
        // as current item (see m_curItem handling above)
        children.RemoveAt(n-1);
    }

    // delete item itself
    SendEvent(wxEVT_COMMAND_TREE_DELETE_ITEM, item);
    delete item;
}


// ----------------------------------------------------------------------------

void wxDbTreeListMainWindow::SetCurrentItem(wxDbTreeListItem *item) {
wxDbTreeListItem *old_item;

    old_item = m_curItem; m_curItem = item;

    // change of item, redraw previous
    if (old_item != NULL && old_item != item) {
        RefreshLine(old_item);
    }

}

// ----------------------------------------------------------------------------

void wxDbTreeListMainWindow::Expand (const wxTreeItemId& itemId) {
    wxDbTreeListItem *item = (wxDbTreeListItem*) itemId.m_pItem;
    wxCHECK_RET (item, _T("invalid item in wxDbTreeListMainWindow::Expand") );

    if (!item->HasPlus() || item->IsExpanded()) return;

    // send event to user code
    wxTreeEvent event(wxEVT_COMMAND_TREE_ITEM_EXPANDING, 0);
    event.SetInt(m_curColumn);
    if (SendEvent(0, item, &event) && !event.IsAllowed()) return; // expand canceled

    item->Expand();
    m_dirty = true;

    // send event to user code
    event.SetEventType (wxEVT_COMMAND_TREE_ITEM_EXPANDED);
    SendEvent(0, NULL, &event);
}

void wxDbTreeListMainWindow::ExpandAll (const wxTreeItemId& itemId) {
    wxCHECK_RET (itemId.IsOk(), _T("invalid tree item"));

    Expand (itemId);
    if (!IsExpanded (itemId)) return;
#if !wxCHECK_VERSION(2, 5, 0)
    long cookie;
#else
    wxTreeItemIdValue cookie;
#endif
    wxTreeItemId child = GetFirstChild (itemId, cookie);
    while (child.IsOk()) {
        ExpandAll (child);
        child = GetNextChild (itemId, cookie);
    }
}

void wxDbTreeListMainWindow::Collapse (const wxTreeItemId& itemId) {
    wxDbTreeListItem *item = (wxDbTreeListItem*) itemId.m_pItem;
    wxCHECK_RET (item, _T("invalid item in wxDbTreeListMainWindow::Collapse") );

    if (!item->HasPlus() || !item->IsExpanded()) return;

    // send event to user code
    wxTreeEvent event (wxEVT_COMMAND_TREE_ITEM_COLLAPSING, 0 );
    event.SetInt(m_curColumn);
    if (SendEvent(0, item, &event) && !event.IsAllowed()) return; // collapse canceled

    item->Collapse();
    m_dirty = true;

    // send event to user code
    event.SetEventType (wxEVT_COMMAND_TREE_ITEM_COLLAPSED);
    SendEvent(0, NULL, &event);
}

void wxDbTreeListMainWindow::CollapseAndReset (const wxTreeItemId& item) {
    wxCHECK_RET (item.IsOk(), _T("invalid tree item"));

    Collapse (item);
    DeleteChildren (item);
}

void wxDbTreeListMainWindow::Toggle (const wxTreeItemId& itemId) {
    wxCHECK_RET (itemId.IsOk(), _T("invalid tree item"));

    if (IsExpanded (itemId)) {
        Collapse (itemId);
    }else{
        Expand (itemId);
    }
}

void wxDbTreeListMainWindow::Unselect() {
    if (m_selectItem) {
        m_selectItem->SetHilight (false);
        RefreshLine (m_selectItem);
        m_selectItem = (wxDbTreeListItem*)NULL;
    }
}

void wxDbTreeListMainWindow::UnselectAllChildren (wxDbTreeListItem *item) {
    wxCHECK_RET (item, _T("invalid tree item"));

    if (item->IsSelected()) {
        item->SetHilight (false);
        RefreshLine (item);
        if (item == m_selectItem) m_selectItem = (wxDbTreeListItem*)NULL;
        if (item != m_curItem) m_lastOnSame = false;  // selection change, so reset edit marker
    }
    if (item->HasChildren()) {
        wxArrayTreeListItems& children = item->GetChildren();
        size_t count = children.Count();
        for (size_t n = 0; n < count; ++n) {
            UnselectAllChildren (children[n]);
        }
    }
}

void wxDbTreeListMainWindow::UnselectAll() {
    UnselectAllChildren ((wxDbTreeListItem*)GetRootItem().m_pItem);
}

// Recursive function !
// To stop we must have crt_item<last_item
// Algorithm :
// Tag all next children, when no more children,
// Move to parent (not to tag)
// Keep going... if we found last_item, we stop.
bool wxDbTreeListMainWindow::TagNextChildren (wxDbTreeListItem *crt_item,
                                            wxDbTreeListItem *last_item) {
    wxDbTreeListItem *parent = crt_item->GetItemParent();

    if (!parent) {// This is root item
        return TagAllChildrenUntilLast (crt_item, last_item);
    }

    wxArrayTreeListItems& children = parent->GetChildren();
    int index = children.Index(crt_item);
    wxASSERT (index != wxNOT_FOUND); // I'm not a child of my parent?

    if ((parent->HasChildren() && parent->IsExpanded()) ||
        ((parent == (wxDbTreeListItem*)GetRootItem().m_pItem) && HasFlag(wxTR_HIDE_ROOT))) {
        size_t count = children.Count();
        for (size_t n = (index+1); n < count; ++n) {
            if (TagAllChildrenUntilLast (children[n], last_item)) return true;
        }
    }

    return TagNextChildren (parent, last_item);
}

bool wxDbTreeListMainWindow::TagAllChildrenUntilLast (wxDbTreeListItem *crt_item,
                                                    wxDbTreeListItem *last_item) {
    crt_item->SetHilight (true);
    RefreshLine(crt_item);

    if (crt_item==last_item) return true;

    if (crt_item->HasChildren() && crt_item->IsExpanded()) {
        wxArrayTreeListItems& children = crt_item->GetChildren();
        size_t count = children.Count();
        for (size_t n = 0; n < count; ++n) {
            if (TagAllChildrenUntilLast (children[n], last_item)) return true;
        }
    }

    return false;
}

bool wxDbTreeListMainWindow::SelectItem (const wxTreeItemId& itemId,
                                       const wxTreeItemId& lastId,
                                       bool unselect_others) {

    wxDbTreeListItem *item = itemId.IsOk() ? (wxDbTreeListItem*) itemId.m_pItem : NULL;

    // send selecting event to the user code
    wxTreeEvent event( wxEVT_COMMAND_TREE_SEL_CHANGING, 0);
    event.SetInt(m_curColumn);
#if !wxCHECK_VERSION(2, 5, 0)
    event.SetOldItem ((long)m_curItem);
#else
    event.SetOldItem (m_curItem);
#endif
    if (SendEvent(0, item, &event) && !event.IsAllowed()) return false;  // veto on selection change

    // unselect all if unselect other items
    bool bUnselectedAll = false; // see that UnselectAll is done only once
    if (unselect_others) {
        if (HasFlag(wxTR_MULTIPLE)) {
            UnselectAll(); bUnselectedAll = true;
        }else{
            Unselect(); // to speed up thing
        }
    }

    // select item range
    if (lastId.IsOk() && itemId.IsOk() && (itemId != lastId)) {

        if (! bUnselectedAll) UnselectAll();
        wxDbTreeListItem *last = (wxDbTreeListItem*) lastId.m_pItem;

        // ensure that the position of the item it calculated in any case
        if (m_dirty) CalculatePositions();

        // select item range according Y-position
        if (last->GetY() < item->GetY()) {
            if (!TagAllChildrenUntilLast (last, item)) {
                TagNextChildren (last, item);
            }
        }else{
            if (!TagAllChildrenUntilLast (item, last)) {
                TagNextChildren (item, last);
            }
        }

    // or select single item
    }else if (itemId.IsOk()) {

        // select item according its old selection
        item->SetHilight (!item->IsSelected());
        RefreshLine (item);
        if (unselect_others) {
            m_selectItem = (item->IsSelected())? item: (wxDbTreeListItem*)NULL;
        }

    // or select nothing
    } else {
        if (! bUnselectedAll) UnselectAll();
    }

    // send event to user code
    event.SetEventType(wxEVT_COMMAND_TREE_SEL_CHANGED);
    SendEvent(0, NULL, &event);

    return true;
}

void wxDbTreeListMainWindow::SelectAll() {
    wxTreeItemId root = GetRootItem();
    wxCHECK_RET (HasFlag(wxTR_MULTIPLE), _T("invalid tree style"));
    wxCHECK_RET (root.IsOk(), _T("no tree"));

    // send event to user code
    wxTreeEvent event (wxEVT_COMMAND_TREE_SEL_CHANGING, 0);
#if !wxCHECK_VERSION(2, 5, 0)
    event.SetOldItem ((long)m_curItem);
#else
    event.SetOldItem (m_curItem);
#endif
    event.SetInt (-1); // no colum clicked
    if (SendEvent(0, m_rootItem, &event) && !event.IsAllowed()) return;  // selection change vetoed

#if !wxCHECK_VERSION(2, 5, 0)
    long cookie = 0;
#else
    wxTreeItemIdValue cookie = 0;
#endif
    wxDbTreeListItem *first = (wxDbTreeListItem *)GetFirstChild (root, cookie).m_pItem;
    wxDbTreeListItem *last = (wxDbTreeListItem *)GetLastChild (root, cookie).m_pItem;
    if (!TagAllChildrenUntilLast (first, last)) {
        TagNextChildren (first, last);
    }

    // send event to user code
    event.SetEventType (wxEVT_COMMAND_TREE_SEL_CHANGED);
    SendEvent(0, NULL, &event);
}

void wxDbTreeListMainWindow::FillArray (wxDbTreeListItem *item,
                                      wxArrayTreeItemIds &array) const {
    if (item->IsSelected()) array.Add (wxTreeItemId(item));

    if (item->HasChildren()) {
        wxArrayTreeListItems& children = item->GetChildren();
        size_t count = children.GetCount();
        for (size_t n = 0; n < count; ++n) FillArray (children[n], array);
    }
}

size_t wxDbTreeListMainWindow::GetSelections (wxArrayTreeItemIds &array) const {
    array.Empty();
    wxTreeItemId idRoot = GetRootItem();
    if (idRoot.IsOk()) FillArray ((wxDbTreeListItem*) idRoot.m_pItem, array);
    return array.Count();
}

void wxDbTreeListMainWindow::EnsureVisible (const wxTreeItemId& item) {
    if (!item.IsOk()) return; // do nothing if no item

    // first expand all parent branches
    wxDbTreeListItem *gitem = (wxDbTreeListItem*) item.m_pItem;
    wxDbTreeListItem *parent = gitem->GetItemParent();
    while (parent) {
        Expand (parent);
        parent = parent->GetItemParent();
    }

    ScrollTo (item);
    RefreshLine (gitem);
}

void wxDbTreeListMainWindow::ScrollTo (const wxTreeItemId &item) {
    if (!item.IsOk()) return; // do nothing if no item

    // ensure that the position of the item it calculated in any case
    if (m_dirty) CalculatePositions();

    wxDbTreeListItem *gitem = (wxDbTreeListItem*) item.m_pItem;

    // now scroll to the item
    int item_y = gitem->GetY();

    int xUnit, yUnit;
    GetScrollPixelsPerUnit (&xUnit, &yUnit);
    int start_x = 0;
    int start_y = 0;
    GetViewStart (&start_x, &start_y);
    start_y *= yUnit;

    int client_h = 0;
    int client_w = 0;
    GetClientSize (&client_w, &client_h);

    int x = 0;
    int y = 0;
    m_rootItem->GetSize (x, y, this);
    x = m_owner->GetHeaderWindow()->GetWidth();
    y += yUnit + 2; // one more scrollbar unit + 2 pixels
    int x_pos = GetScrollPos( wxHORIZONTAL );

    if (item_y < start_y+3) {
        // going down, item should appear at top
        SetScrollbars (xUnit, yUnit, xUnit ? x/xUnit : 0, yUnit ? y/yUnit : 0, x_pos, yUnit ? item_y/yUnit : 0);
    }else if (item_y+GetLineHeight(gitem) > start_y+client_h) {
        // going up, item should appear at bottom
        item_y += yUnit + 2;
        SetScrollbars (xUnit, yUnit, xUnit ? x/xUnit : 0, yUnit ? y/yUnit : 0, x_pos, yUnit ? (item_y+GetLineHeight(gitem)-client_h)/yUnit : 0 );
    }
}

// FIXME: tree sorting functions are not reentrant and not MT-safe!
static wxDbTreeListMainWindow *s_treeBeingSorted = NULL;

static int LINKAGEMODE tree_ctrl_compare_func(wxDbTreeListItem **item1,
                                  wxDbTreeListItem **item2)
{
    wxCHECK_MSG (s_treeBeingSorted, 0, _T("bug in wxDbTreeListMainWindow::SortChildren()") );

    return s_treeBeingSorted->OnCompareItems(*item1, *item2);
}

int wxDbTreeListMainWindow::OnCompareItems(const wxTreeItemId& item1,
                               const wxTreeItemId& item2)
{
    return m_owner->OnCompareItems (item1, item2);
}

void wxDbTreeListMainWindow::SortChildren (const wxTreeItemId& itemId) {
    wxCHECK_RET (itemId.IsOk(), _T("invalid tree item"));

    wxDbTreeListItem *item = (wxDbTreeListItem*) itemId.m_pItem;

    wxCHECK_RET (!s_treeBeingSorted,
                 _T("wxDbTreeListMainWindow::SortChildren is not reentrant") );

    wxArrayTreeListItems& children = item->GetChildren();
    if ( children.Count() > 1 ) {
        m_dirty = true;
        s_treeBeingSorted = this;
        children.Sort(tree_ctrl_compare_func);
        s_treeBeingSorted = NULL;
    }
}

wxTreeItemId wxDbTreeListMainWindow::FindItem (const wxTreeItemId& item, const wxString& str, int mode) {
    wxString itemText;
    // determine start item
    wxTreeItemId next = item;
    if (next.IsOk()) {
        if (mode & wxDB_TL_MODE_NAV_LEVEL) {
            next = GetNextSibling (next);
        }else if (mode & wxDB_TL_MODE_NAV_VISIBLE) { //
            next = GetNextVisible (next, false, true);
        }else if (mode & wxDB_TL_MODE_NAV_EXPANDED) {
            next = GetNextExpanded (next);
        }else{ // (mode & wxDB_TL_MODE_NAV_FULLTREE) default
            next = GetNext (next, true);
        }
    }

#if !wxCHECK_VERSION(2, 5, 0)
    long cookie = 0;
#else
    wxTreeItemIdValue cookie = 0;
#endif
    if (!next.IsOk()) {
        next = GetRootItem();
        if (next.IsOk() && HasFlag(wxTR_HIDE_ROOT)) {
            next = GetFirstChild (GetRootItem(), cookie);
        }
    }
    if (!next.IsOk()) return (wxTreeItemId*)NULL;

    // start checking the next items
    while (next.IsOk() && (next != item)) {
        if (mode & wxDB_TL_MODE_FIND_PARTIAL) {
            itemText = GetItemText (next).Mid (0, str.Length());
        }else{
            itemText = GetItemText (next);
        }
        if (mode & wxDB_TL_MODE_FIND_NOCASE) {
            if (itemText.CmpNoCase (str) == 0) return next;
        }else{
            if (itemText.Cmp (str) == 0) return next;
        }
        if (mode & wxDB_TL_MODE_NAV_LEVEL) {
            next = GetNextSibling (next);
        }else if (mode & wxDB_TL_MODE_NAV_VISIBLE) { //
            next = GetNextVisible (next, false, true);
        }else if (mode & wxDB_TL_MODE_NAV_EXPANDED) {
            next = GetNextExpanded (next);
        }else{ // (mode & wxDB_TL_MODE_NAV_FULLTREE) default
            next = GetNext (next, true);
        }
        if (!next.IsOk() && item.IsOk()) {
            next = (wxDbTreeListItem*)GetRootItem().m_pItem;
            if (HasFlag(wxTR_HIDE_ROOT)) {
                next = (wxDbTreeListItem*)GetNextChild (GetRootItem().m_pItem, cookie).m_pItem;
            }
        }
    }
    return (wxTreeItemId*)NULL;
}

void wxDbTreeListMainWindow::SetDragItem (const wxTreeItemId& item) {
    wxDbTreeListItem *prevItem = m_dragItem;
    m_dragItem = (wxDbTreeListItem*) item.m_pItem;
    if (prevItem) RefreshLine (prevItem);
    if (m_dragItem) RefreshLine (m_dragItem);
}

void wxDbTreeListMainWindow::CalculateLineHeight() {
    wxClientDC dc (this);
    dc.SetFont (m_normalFont);
    m_lineHeight = (int)(dc.GetCharHeight() + m_linespacing);

    if (m_imageListNormal) {
        // Calculate a m_lineHeight value from the normal Image sizes.
        // May be toggle off. Then wxDbTreeListMainWindow will spread when
        // necessary (which might look ugly).
        int n = m_imageListNormal->GetImageCount();
        for (int i = 0; i < n ; i++) {
            int width = 0, height = 0;
            m_imageListNormal->GetSize(i, width, height);
            if (height > m_lineHeight) m_lineHeight = height + m_linespacing;
        }
    }

    if (m_imageListButtons) {
        // Calculate a m_lineHeight value from the Button image sizes.
        // May be toggle off. Then wxDbTreeListMainWindow will spread when
        // necessary (which might look ugly).
        int n = m_imageListButtons->GetImageCount();
        for (int i = 0; i < n ; i++) {
            int width = 0, height = 0;
            m_imageListButtons->GetSize(i, width, height);
            if (height > m_lineHeight) m_lineHeight = height + m_linespacing;
        }
    }

    if (m_lineHeight < 30) { // add 10% space if greater than 30 pixels
        m_lineHeight += 2; // minimal 2 pixel space
    }else{
        m_lineHeight += m_lineHeight / 10; // otherwise 10% space
    }
}

void wxDbTreeListMainWindow::SetImageList (wxImageList *imageList) {
    if (m_ownsImageListNormal) delete m_imageListNormal;
    m_imageListNormal = imageList;
    m_ownsImageListNormal = false;
    m_dirty = true;
    CalculateLineHeight();
}

void wxDbTreeListMainWindow::SetStateImageList (wxImageList *imageList) {
    if (m_ownsImageListState) delete m_imageListState;
    m_imageListState = imageList;
    m_ownsImageListState = false;
}

void wxDbTreeListMainWindow::SetButtonsImageList (wxImageList *imageList) {
    if (m_ownsImageListButtons) delete m_imageListButtons;
    m_imageListButtons = imageList;
    m_ownsImageListButtons = false;
    m_dirty = true;
    CalculateLineHeight();
}

void wxDbTreeListMainWindow::AssignImageList (wxImageList *imageList) {
    SetImageList(imageList);
    m_ownsImageListNormal = true;
}

void wxDbTreeListMainWindow::AssignStateImageList (wxImageList *imageList) {
    SetStateImageList(imageList);
    m_ownsImageListState = true;
}

void wxDbTreeListMainWindow::AssignButtonsImageList (wxImageList *imageList) {
    SetButtonsImageList(imageList);
    m_ownsImageListButtons = true;
}

// ----------------------------------------------------------------------------
// helpers
// ----------------------------------------------------------------------------

void wxDbTreeListMainWindow::AdjustMyScrollbars() {
    if (m_rootItem) {
        int xUnit, yUnit;
        GetScrollPixelsPerUnit (&xUnit, &yUnit);
        if (xUnit == 0) xUnit = GetCharWidth();
        if (yUnit == 0) yUnit = m_lineHeight;
        int x = 0, y = 0;
        m_rootItem->GetSize (x, y, this);
        y += yUnit + 2; // one more scrollbar unit + 2 pixels
        int x_pos = GetScrollPos (wxHORIZONTAL);
        int y_pos = GetScrollPos (wxVERTICAL);
        x = m_owner->GetHeaderWindow()->GetWidth() + 2;
        if (x < GetClientSize().GetWidth()) x_pos = 0;
        SetScrollbars (xUnit, yUnit, x/xUnit, y/yUnit, x_pos, y_pos);
    }else{
        SetScrollbars (0, 0, 0, 0);
    }
}

int wxDbTreeListMainWindow::GetLineHeight (wxDbTreeListItem *item) const {
    if (GetWindowStyleFlag() & wxTR_HAS_VARIABLE_ROW_HEIGHT) {
        return item->GetHeight();
    }else{
        return m_lineHeight;
    }
}

void wxDbTreeListMainWindow::PaintItem (wxDbTreeListItem *item, wxDC& dc) {

    wxTreeItemAttr *attr = item->GetAttributes();

    dc.SetFont (GetItemFont (item));

    wxColour colText;
    if (attr && attr->HasTextColour()) {
        colText = attr->GetTextColour();
    }else{
        colText = GetForegroundColour();
    }
#if !wxCHECK_VERSION(2, 5, 0)
    wxColour colTextHilight = wxSystemSettings::GetSystemColour (wxSYS_COLOUR_HIGHLIGHTTEXT);
#else
    wxColour colTextHilight = wxSystemSettings::GetColour (wxSYS_COLOUR_HIGHLIGHTTEXT);
#endif

    int total_w = m_owner->GetHeaderWindow()->GetWidth();
    int total_h = GetLineHeight(item);
    int off_h = HasFlag(wxTR_ROW_LINES) ? 1 : 0;
    int off_w = HasFlag(wxTR_COLUMN_LINES) ? 1 : 0;
    wxDCClipper clipper (dc, 0, item->GetY(), total_w, total_h); // only within line

    int text_w = 0, text_h = 0;
    dc.GetTextExtent( item->GetText(GetMainColumn()).size() > 0
            ? item->GetText(GetMainColumn())
            : _T(" "),  // dummy text to avoid zero height and no highlight width
        &text_w, &text_h );

    // determine background and show it
    wxColour colBg;
    if (attr && attr->HasBackgroundColour()) {
        colBg = attr->GetBackgroundColour();
    }else{
        colBg = m_backgroundColour;
    }
    dc.SetBrush (wxBrush (colBg, wxSOLID));
    dc.SetPen (*wxTRANSPARENT_PEN);
    if (HasFlag (wxTR_FULL_ROW_HIGHLIGHT)) {
        if (item->IsSelected()) {
            if (! m_isDragging && m_hasFocus) {
                dc.SetBrush (*m_hilightBrush);
#ifndef __WXMAC__ // don't draw rect outline if we already have the background color
                dc.SetPen (*wxBLACK_PEN);
#endif // !__WXMAC__
            }else{
                dc.SetBrush (*m_hilightUnfocusedBrush);
#ifndef __WXMAC__ // don't draw rect outline if we already have the background color
                dc.SetPen (*wxTRANSPARENT_PEN);
#endif // !__WXMAC__
            }
            dc.SetTextForeground (colTextHilight);
        }else if (item == m_curItem) {
            dc.SetPen (m_hasFocus? *wxBLACK_PEN: *wxTRANSPARENT_PEN);
        }else{
            dc.SetTextForeground (colText);
        }
        dc.DrawRectangle (0, item->GetY() + off_h, total_w, total_h - off_h);
    }else{
        dc.SetTextForeground (colText);
    }

    int text_extraH = (total_h > text_h) ? (total_h - text_h)/2 : 0;
    int img_extraH = (total_h > m_imgHeight)? (total_h-m_imgHeight)/2: 0;
    int x_colstart = 0;
    for (int i = 0; i < GetColumnCount(); ++i ) {
        if (!m_owner->GetHeaderWindow()->IsColumnShown(i)) continue;

        int col_w = m_owner->GetHeaderWindow()->GetColumnWidth(i);
        wxDCClipper clipper (dc, x_colstart, item->GetY(), col_w, total_h); // only within column
		if (!m_owner->GetHeaderWindow()->IsColumnShown(i)) continue;
		//Zamyatin Выделяем активную ячейку
		 if (HasFlag (wxTR_FULL_ROW_HIGHLIGHT)) {
			if (item->IsSelected() && m_owner->eCol==i) {

				wxBrush br= *m_hilightBrush ;
				wxColour clr=br.GetColour();
				clr.Set(clr.Red()*4/2,clr.Green()*4/2,clr.Blue()*4/2);
				br.SetColour(clr);
				dc.SetBrush(br);
				dc.DrawRectangle ( x_colstart, item->GetY(), col_w, total_h);
			}
		 }
		//End Zamyatin
        int x = 0;
        int image = NO_IMAGE;
        int image_w = 0;
        if(i == GetMainColumn()) {
            x = item->GetX() + MARGIN;
            if (HasButtons()) {
                x += (m_btnWidth-m_btnWidth2) + LINEATROOT;
            }else{
                x -= m_indent/2;
            }
            if (m_imageListNormal) image = item->GetCurrentImage();
        }else{
            x = x_colstart + MARGIN;
            image = item->GetImage(i);
        }
        if (image != NO_IMAGE) image_w = m_imgWidth + MARGIN;

        // honor text alignment
        wxString text = item->GetText(i);
        int w = 0;
        switch ( m_owner->GetHeaderWindow()->GetColumn(i).GetAlignment() ) {
        case wxALIGN_LEFT:
            // nothing to do, already left aligned
            break;
        case wxALIGN_RIGHT:
            dc.GetTextExtent (text, &text_w, NULL);
            w = col_w - (image_w + text_w + off_w + MARGIN);
            x += (w > 0)? w: 0;
            break;
        case wxALIGN_CENTER:
            dc.GetTextExtent(text, &text_w, NULL);
            w = (col_w - (image_w + text_w + off_w + MARGIN))/2;
            x += (w > 0)? w: 0;
            break;
        }
        int text_x = x + image_w;
        if (i == GetMainColumn()) item->SetTextX (text_x);

        if (!HasFlag (wxTR_FULL_ROW_HIGHLIGHT)) {
            if (i == GetMainColumn()) {
                if (item->IsSelected()) {
                    if (!m_isDragging && m_hasFocus) {
                        dc.SetBrush (*m_hilightBrush);
#ifndef __WXMAC__ // don't draw rect outline if we already have the background color
                        dc.SetPen (*wxBLACK_PEN);
#endif // !__WXMAC__
                    }else{
                        dc.SetBrush (*m_hilightUnfocusedBrush);
#ifndef __WXMAC__ // don't draw rect outline if we already have the background color
                      dc.SetPen (*wxTRANSPARENT_PEN);
#endif // !__WXMAC__
                    }
                    dc.SetTextForeground (colTextHilight);
                }else if (item == m_curItem) {
                    dc.SetPen (m_hasFocus? *wxBLACK_PEN: *wxTRANSPARENT_PEN);
                }else{
                    dc.SetTextForeground (colText);
                }
                dc.DrawRectangle (text_x, item->GetY() + off_h, text_w, total_h - off_h);
            }else{
                dc.SetTextForeground (colText);
            }
        }

        if (HasFlag(wxTR_COLUMN_LINES)) { // vertical lines between columns
#if !wxCHECK_VERSION(2, 5, 0)
            wxPen pen (wxSystemSettings::GetSystemColour (wxSYS_COLOUR_3DLIGHT ), 1, wxSOLID);
#else
//            wxPen pen (wxSystemSettings::GetColour (wxSYS_COLOUR_3DLIGHT ), 1, wxSOLID);
            wxPen pen (wxColour(64,64,64), 1, wxSOLID); // by s --- fixed grid color
#endif
            dc.SetPen ((GetBackgroundColour() == *wxWHITE)? pen: *wxWHITE_PEN);
            dc.DrawLine (x_colstart+col_w-1, item->GetY(), x_colstart+col_w-1, item->GetY()+total_h);
        }

        dc.SetBackgroundMode (wxTRANSPARENT);

        if (image != NO_IMAGE) {
            int y = item->GetY() + img_extraH;
            m_imageListNormal->Draw (image, dc, x, y, wxIMAGELIST_DRAW_TRANSPARENT );
        }
        int text_y = item->GetY() + text_extraH;
        dc.DrawText (text, (wxCoord)text_x, (wxCoord)text_y);

        x_colstart += col_w;
    }

    // restore normal font
    dc.SetFont( m_normalFont );
}

// Now y stands for the top of the item, whereas it used to stand for middle !
void wxDbTreeListMainWindow::PaintLevel (wxDbTreeListItem *item, wxDC &dc,
                                       int level, int &y, int x_maincol) {

    // Handle hide root (only level 0)
    if (HasFlag(wxTR_HIDE_ROOT) && (level == 0)) {
        wxArrayTreeListItems& children = item->GetChildren();
        for (size_t n = 0; n < children.Count(); n++) {
            PaintLevel (children[n], dc, 1, y, x_maincol);
        }
        // end after expanding root
        return;
    }

    // calculate position of vertical lines
    int x = x_maincol + MARGIN; // start of column
    if (HasFlag(wxTR_LINES_AT_ROOT)) x += LINEATROOT; // space for lines at root
    if (HasButtons()) {
        x += (m_btnWidth-m_btnWidth2); // half button space
    }else{
        x += (m_indent-m_indent/2);
    }
    if (HasFlag(wxTR_HIDE_ROOT)) {
        x += m_indent * (level-1); // indent but not level 1
    }else{
        x += m_indent * level; // indent according to level
    }

    // set position of vertical line
    item->SetX (x);
    item->SetY (y);

    int h = GetLineHeight (item);
    int y_top = y;
    int y_mid = y_top + (h/2);
    y += h;

    int exposed_x = dc.LogicalToDeviceX(0);
    int exposed_y = dc.LogicalToDeviceY(y_top);

    if (IsExposed(exposed_x, exposed_y, 10000, h)) { // 10000 = very much

        if (HasFlag(wxTR_ROW_LINES)) { // horizontal lines between rows
            //dc.DestroyClippingRegion();
            int total_width = m_owner->GetHeaderWindow()->GetWidth();
            // if the background colour is white, choose a
            // contrasting color for the lines
#if !wxCHECK_VERSION(2, 5, 0)
//            wxPen pen (wxSystemSettings::GetSystemColour (wxSYS_COLOUR_3DLIGHT ), 1, wxSOLID);
            wxPen pen (wxColour(64,64,64), 1, wxSOLID); // by s --- fixed grid color
#else
//            wxPen pen (wxSystemSettings::GetColour (wxSYS_COLOUR_3DLIGHT ), 1, wxSOLID);
            wxPen pen (wxColour(64,64,64), 1, wxSOLID); // by s --- fixed grid color
#endif
            dc.SetPen ((GetBackgroundColour() == *wxWHITE)? pen: *wxWHITE_PEN);
            dc.DrawLine (0, y_top, total_width, y_top);
            dc.DrawLine (0, y_top+h, total_width, y_top+h);
        }

        // draw item
        PaintItem (item, dc);

        // restore DC objects
        dc.SetBrush(*wxWHITE_BRUSH);
        dc.SetPen(m_dottedPen);

        // clip to the column width
        int clip_width = m_owner->GetHeaderWindow()->
                            GetColumn(m_main_column).GetWidth();
        wxDCClipper clipper(dc, x_maincol, y_top, clip_width, 10000);

        if (!HasFlag(wxTR_NO_LINES)) { // connection lines

            // draw the horizontal line here
            dc.SetPen(m_dottedPen);
            int x2 = x - m_indent;
            if (x2 < (x_maincol + MARGIN)) x2 = x_maincol + MARGIN;
            int x3 = x + (m_btnWidth-m_btnWidth2);
            if (HasButtons()) {
                if (item->HasPlus()) {
                    dc.DrawLine (x2, y_mid, x - m_btnWidth2, y_mid);
                    dc.DrawLine (x3, y_mid, x3 + LINEATROOT, y_mid);
                }else{
                    dc.DrawLine (x2, y_mid, x3 + LINEATROOT, y_mid);
                }
            }else{
                dc.DrawLine (x2, y_mid, x - m_indent/2, y_mid);
            }
        }

        if (item->HasPlus() && HasButtons()) { // should the item show a button?

            if (m_imageListButtons) {

                // draw the image button here
                int image = wxTreeItemIcon_Normal;
                if (item->IsExpanded()) image = wxTreeItemIcon_Expanded;
                if (item->IsSelected()) image += wxTreeItemIcon_Selected - wxTreeItemIcon_Normal;
                int xx = x - m_btnWidth2 + MARGIN;
                int yy = y_mid - m_btnHeight2;
                dc.SetClippingRegion(xx, yy, m_btnWidth, m_btnHeight);
                m_imageListButtons->Draw (image, dc, xx, yy, wxIMAGELIST_DRAW_TRANSPARENT);
                dc.DestroyClippingRegion();

            }else if (HasFlag (wxTR_TWIST_BUTTONS)) {

                // draw the twisty button here
                dc.SetPen(*wxBLACK_PEN);
                dc.SetBrush(*m_hilightBrush);
                wxPoint button[3];
                if (item->IsExpanded()) {
                    button[0].x = x - (m_btnWidth2+1);
                    button[0].y = y_mid - (m_btnHeight/3);
                    button[1].x = x + (m_btnWidth2+1);
                    button[1].y = button[0].y;
                    button[2].x = x;
                    button[2].y = button[0].y + (m_btnHeight2+1);
                }else{
                    button[0].x = x - (m_btnWidth/3);
                    button[0].y = y_mid - (m_btnHeight2+1);
                    button[1].x = button[0].x;
                    button[1].y = y_mid + (m_btnHeight2+1);
                    button[2].x = button[0].x + (m_btnWidth2+1);
                    button[2].y = y_mid;
                }
                dc.DrawPolygon(3, button);

            }else{ // if (HasFlag(wxTR_HAS_BUTTONS))

                // draw the plus sign here
#if !wxCHECK_VERSION(2, 7, 0)
                dc.SetPen(*wxGREY_PEN);
                dc.SetBrush(*wxWHITE_BRUSH);
                dc.DrawRectangle (x-m_btnWidth2, y_mid-m_btnHeight2, m_btnWidth, m_btnHeight);
                dc.SetPen(*wxBLACK_PEN);
                dc.DrawLine (x-(m_btnWidth2-2), y_mid, x+(m_btnWidth2-1), y_mid);
                if (!item->IsExpanded()) { // change "-" to "+"
                    dc.DrawLine (x, y_mid-(m_btnHeight2-2), x, y_mid+(m_btnHeight2-1));
                }
#else
                wxRect rect (x-m_btnWidth2, y_mid-m_btnHeight2, m_btnWidth, m_btnHeight);
                int flag = item->IsExpanded()? wxCONTROL_EXPANDED: 0;
                wxRendererNative::GetDefault().DrawTreeItemButton (this, dc, rect, flag);
#endif

            }

        }

    }

    // restore DC objects
    dc.SetBrush(*wxWHITE_BRUSH);
    dc.SetPen(m_dottedPen);
    dc.SetTextForeground(*wxBLACK);

    if (item->IsExpanded())
    {
        wxArrayTreeListItems& children = item->GetChildren();

        // clip to the column width
        int clip_width = m_owner->GetHeaderWindow()->
                            GetColumn(m_main_column).GetWidth();

        // process lower levels
        int oldY;
        if (m_imgWidth > 0) {
            oldY = y_mid + m_imgHeight2;
        }else{
            oldY = y_mid + h/2;
        }
        int y2;
        for (size_t n = 0; n < children.Count(); ++n) {

            y2 = y + h/2;
            PaintLevel (children[n], dc, level+1, y, x_maincol);

            // draw vertical line
            wxDCClipper clipper(dc, x_maincol, y_top, clip_width, 10000);
            if (!HasFlag (wxTR_NO_LINES)) {
                x = item->GetX();
                dc.DrawLine (x, oldY, x, y2);
                oldY = y2;
            }
        }
    }
}


// ----------------------------------------------------------------------------
// wxWindows callbacks
// ----------------------------------------------------------------------------

void wxDbTreeListMainWindow::OnPaint (wxPaintEvent &WXUNUSED(event)) {
	//Zamyatin
   //Проверка если не все записи считаны, проверяем попадает ли не считанная запись на видимую область, если да , то считываем из БД

	wxDbTreeGrid *g=(wxDbTreeGrid*)m_owner;
	if( g->EmptyId.IsOk() && g->IsVisible(g->EmptyId))
	{
		g->GetAllRow();
	}

	//Zamyatin

    // init device context, clear background (BEFORE changing DC origin...)
    wxAutoBufferedPaintDC dc (this);
    wxBrush brush(GetBackgroundColour(), wxSOLID);
    dc.SetBackground(brush);
    dc.Clear();
    DoPrepareDC (dc);

    if (!m_rootItem || (GetColumnCount() <= 0)) return;

    // calculate button size
    if (m_imageListButtons) {
        m_imageListButtons->GetSize (0, m_btnWidth, m_btnHeight);
    }else if (HasButtons()) {
        m_btnWidth = BTNWIDTH;
        m_btnHeight = BTNHEIGHT;
    }
    m_btnWidth2 = m_btnWidth/2;
    m_btnHeight2 = m_btnHeight/2;

    // calculate image size
    if (m_imageListNormal) {
        m_imageListNormal->GetSize (0, m_imgWidth, m_imgHeight);
    }
    m_imgWidth2 = m_imgWidth/2;
    m_imgHeight2 = m_imgHeight/2;

    // calculate indent size
    if (m_imageListButtons) {
        m_indent = wxMax (MININDENT, m_btnWidth + MARGIN);
    }else if (HasButtons()) {
        m_indent = wxMax (MININDENT, m_btnWidth + LINEATROOT);
    }

    // set default values
    dc.SetFont( m_normalFont );
    dc.SetPen( m_dottedPen );

    // calculate column start and paint
    int x_maincol = 0;
    int i = 0;
    for (i = 0; i < (int)GetMainColumn(); ++i) {
        if (!m_owner->GetHeaderWindow()->IsColumnShown(i)) continue;
        x_maincol += m_owner->GetHeaderWindow()->GetColumnWidth (i);
    }
    int y = 0;
    PaintLevel (m_rootItem, dc, 0, y, x_maincol);



}

void wxDbTreeListMainWindow::OnSetFocus (wxFocusEvent &event) {

    m_hasFocus = true;
	//Zamyatin посылаем событие диспетчеру чтоб знал активный грид
	if(((wxDbTreeGrid*)m_owner)->dispatcher){

		wxDbGridEvent ev(dbEVT_GRID_ACTIVE_HANDLE);
		ev.grid=(wxDbTreeGrid*)m_owner;
		((wxDbTreeGrid*)m_owner)->SetMenuItems();
		((wxDbTreeGrid*)m_owner)->dispatcher->ProcessEvent(ev);
	/*	if(((wxDbTreeGrid*)m_owner)->eId.IsOk())
		{
			((wxDbTreeGrid*)m_owner)->eCol=0;
			m_curColumn=0;
			m_curItem=(wxDbTreeListItem*)((wxDbTreeGrid*)m_owner)->eId.m_pItem;
		}
		*/
		if(m_curColumn>=0 && m_curColumn< (int)((wxDbTreeGrid*)m_owner)->fieldsDescription.Count() &&
			!(((wxDbTreeGrid*)m_owner)->fieldsDescription[m_curColumn].GetProperty()&DB_FIELD_VISIBLE) &&
			((wxDbTreeGrid*)m_owner)->eId.IsOk() 
		)
		{
		
			
				for(int i=0; i< (int)((wxDbTreeGrid*)m_owner)->fieldsDescription.Count(); i++)
				{
					if(((wxDbTreeGrid*)m_owner)->fieldsDescription[i].GetProperty()&DB_FIELD_VISIBLE){ ((wxDbTreeGrid*)m_owner)->eCol=i; m_curColumn=i;break;}
				};
			
		}
	
	}
	//Zamyatin
	
    RefreshSelected();
	if (m_curItem) RefreshLine (m_curItem);

    event.Skip();
}

void wxDbTreeListMainWindow::OnKillFocus( wxFocusEvent &event )
{
    m_hasFocus = false;
    RefreshSelected();
    if (m_curItem) RefreshLine (m_curItem);
    event.Skip();
}

//Zamyatin
void wxDbTreeListMainWindow::OnKeyDown(wxKeyEvent &event)
{
		m_owner->ProcessEvent(event);
};
//Zamyatin
void wxDbTreeListMainWindow::OnChar (wxKeyEvent &event) {
//zAMYATIN Вызываем обработчик из главного объекта
	

		m_owner->ProcessEvent(event);
		if(!event.GetSkipped()){
			
			if(event.GetKeyCode() ==WXK_LEFT || event.GetKeyCode() == WXK_RIGHT ||  event.GetKeyCode() == WXK_TAB)
			m_owner->EnsureVisible(m_owner->eId); m_curColumn=m_owner->eCol; RefreshSelected(); 
			
			return;
		}
		event.Skip(false);
	
	
//end Zamyatin

    // send event to user code
    wxTreeEvent nevent (wxEVT_COMMAND_TREE_KEY_DOWN, 0 );
    nevent.SetInt(m_curColumn);
    nevent.SetKeyEvent (event);
    if (SendEvent(0, NULL, &nevent)) return; // char event handled in user code

    // if no item current, select root
    bool curItemSet = false;
    if (!m_curItem) {
        if (! GetRootItem().IsOk()) return;
        SetCurrentItem((wxDbTreeListItem*)GetRootItem().m_pItem);
        if (HasFlag(wxTR_HIDE_ROOT)) {
#if !wxCHECK_VERSION(2, 5, 0)
            long cookie = 0;
#else
            wxTreeItemIdValue cookie = 0;
#endif
            SetCurrentItem((wxDbTreeListItem*)GetFirstChild (m_curItem, cookie).m_pItem);
        }
        SelectItem(m_curItem, (wxTreeItemId*)NULL, true);  // unselect others
        curItemSet = true;
    }

    // remember item at shift down
    if (HasFlag(wxTR_MULTIPLE) && event.ShiftDown()) {
        if (!m_shiftItem) m_shiftItem = m_curItem;
    }else{
        m_shiftItem = (wxDbTreeListItem*)NULL;
    }

    if (curItemSet) return;  // if no item was current until now, do nothing more

    // process all cases
    wxTreeItemId newItem = (wxTreeItemId*)NULL;
    switch (event.GetKeyCode()) {

        // '+': Expand subtree
        case '+':
        case WXK_ADD: {
            if (m_curItem->HasPlus() && !IsExpanded (m_curItem)) Expand (m_curItem);
        }break;

        // '-': collapse subtree
        case '-':
        case WXK_SUBTRACT: {
            if (m_curItem->HasPlus() && IsExpanded (m_curItem)) Collapse (m_curItem);
        }break;

        // '*': expand/collapse all subtrees // TODO: Mak it more useful
        case '*':
        case WXK_MULTIPLY: {
            if (m_curItem->HasPlus() && !IsExpanded (m_curItem)) {
                ExpandAll (m_curItem);
            }else if (m_curItem->HasPlus()) {
                Collapse (m_curItem); // TODO: CollapseAll
            }
        }break;

        // ' ': toggle current item
        case ' ': {
            SelectItem (m_curItem, (wxDbTreeListItem*)NULL, false);
        }break;

        // <RETURN>: activate current item
        case WXK_RETURN: {
            if (! SendEvent(wxEVT_COMMAND_TREE_ITEM_ACTIVATED, m_curItem)) {

                // if the user code didn't process the activate event,
                // handle it ourselves by toggling the item when it is
                // double clicked
                if (m_curItem && m_curItem->HasPlus()) Toggle(m_curItem);
            }
        }break;

        // <BKSP>: go to the parent without collapsing
        case WXK_BACK: {
            newItem = GetItemParent (m_curItem);
            if ((newItem == GetRootItem()) && HasFlag(wxTR_HIDE_ROOT)) {
                newItem = GetPrevSibling (m_curItem); // get sibling instead of root
            }
        }break;

        // <HOME>: go to first visible
        case WXK_HOME: {
            newItem = GetFirstVisible(false, false);
        }break;

        // <PAGE-UP>: go to the top of the page, or if we already are then one page back
        case WXK_PAGEUP: {
        int flags = 0;
        int col = 0;
        wxPoint abs_p = CalcUnscrolledPosition (wxPoint(1,1));
        // PAGE-UP: first go the the first visible row
            newItem = m_rootItem->HitTest(abs_p, this, flags, col, 0);
            newItem = GetFirstVisible(false, true);
        // if we are already there then scroll back one page
            if (newItem == m_curItem) {
                abs_p.y -= GetClientSize().GetHeight() - m_curItem->GetHeight();
                if (abs_p.y < 0) abs_p.y = 0;
                newItem = m_rootItem->HitTest(abs_p, this, flags, col, 0);
            }
            // newItem should never be NULL
        } break;

        // <UP>: go to the previous sibling or for the last of its children, to the parent
        case WXK_UP: {
            newItem = GetPrevSibling (m_curItem);
            if (newItem) {
#if !wxCHECK_VERSION(2, 5, 0)
                long cookie = 0;
#else
                wxTreeItemIdValue cookie = 0;
#endif
                while (IsExpanded (newItem) && HasChildren (newItem)) {
                    newItem = GetLastChild (newItem, cookie);
                }
            }else {
                newItem = GetItemParent (m_curItem);
                if ((newItem == GetRootItem()) && HasFlag(wxTR_HIDE_ROOT)) {
                    newItem = (wxTreeItemId*)NULL; // don't go to root if it is hidden
                }
            }
        }break;


        // <LEFT>: if expanded collapse subtree, else go to the parent
        case WXK_LEFT: {
            if (IsExpanded (m_curItem)) {
                Collapse (m_curItem);
            }else{
                newItem = GetItemParent (m_curItem);
                if ((newItem == GetRootItem()) && HasFlag(wxTR_HIDE_ROOT)) {
                    newItem = GetPrevSibling (m_curItem); // go to sibling if it is hidden
                }
            }
        }break;

        // <RIGHT>: if possible expand subtree, else go go to the first child
        case WXK_RIGHT: {
            if (m_curItem->HasPlus() && !IsExpanded (m_curItem)) {
                Expand (m_curItem);
            }else{
                if (IsExpanded (m_curItem) && HasChildren (m_curItem)) {
#if !wxCHECK_VERSION(2, 5, 0)
                    long cookie = 0;
#else
                    wxTreeItemIdValue cookie = 0;
#endif
                    newItem = GetFirstChild (m_curItem, cookie);
                }
            }
        }break;

        // <DOWN>: if expanded go to the first child, else to the next sibling, ect
        case WXK_DOWN: {
			//wxLogMessage("wxDbTreeListMainWindow::OnChar DOWN");//Zamyatin
            if (IsExpanded (m_curItem) && HasChildren (m_curItem)) {
#if !wxCHECK_VERSION(2, 5, 0)
                long cookie = 0;
#else
                wxTreeItemIdValue cookie = 0;
#endif
                newItem = GetFirstChild( m_curItem, cookie );
            }
            if (!newItem) {
                wxTreeItemId parent = m_curItem;
                do {
                    newItem = GetNextSibling (parent);
                    parent = GetItemParent (parent);
                } while (!newItem && parent);
            }
        }break;

        // <PAGE-DOWN>: go to the bottom of the page, or if we already are then one page further
        case WXK_PAGEDOWN: {
        int flags = 0;
        int col = 0;
        wxPoint abs_p = CalcUnscrolledPosition (wxPoint(1,GetClientSize().GetHeight() - m_curItem->GetHeight()));
        // PAGE-UP: first go the the first visible row
            newItem = m_rootItem->HitTest(abs_p, this, flags, col, 0);
            newItem = GetLastVisible(false, true);
        // if we are already there then scroll down one page
            if (newItem == m_curItem) {
                abs_p.y += GetClientSize().GetHeight() - m_curItem->GetHeight();
//                if (abs_p.y >= GetVirtualSize().GetHeight()) abs_p.y = GetVirtualSize().GetHeight() - 1;
                newItem = m_rootItem->HitTest(abs_p, this, flags, col, 0);
            }
        // if we reached the empty area below the rows, return last item instead
            if (! newItem) newItem = GetLastVisible(false, false);
        } break;

        // <END>: go to last item of the root
        case WXK_END: {
            newItem = GetLastVisible (false, false);
        }break;

        // any char: go to the next matching string
        default:
            if (event.GetKeyCode() >= (int)' ') {
                if (!m_findTimer->IsRunning()) m_findStr.Clear();
                m_findStr.Append (event.GetKeyCode());
                m_findTimer->Start (FIND_TIMER_TICKS, wxTIMER_ONE_SHOT);
                wxTreeItemId prev = m_curItem? (wxTreeItemId*)m_curItem: (wxTreeItemId*)NULL;
                while (true) {
                    newItem = FindItem (prev, m_findStr, wxDB_TL_MODE_NAV_EXPANDED |
                                                         wxDB_TL_MODE_FIND_PARTIAL |
                                                         wxDB_TL_MODE_FIND_NOCASE);
                    if (newItem || (m_findStr.Length() <= 1)) break;
                    m_findStr.RemoveLast();
                };
            }
            event.Skip();

    }

    // select and show the new item
    if (newItem) {
        if (!event.ControlDown()) {
            bool unselect_others = !((event.ShiftDown() || event.ControlDown()) &&
                                      HasFlag(wxTR_MULTIPLE));
            SelectItem (newItem, m_shiftItem, unselect_others);
        }
        EnsureVisible (newItem);
        wxDbTreeListItem *oldItem = m_curItem;
        SetCurrentItem((wxDbTreeListItem*)newItem.m_pItem); // make the new item the current item
        RefreshLine (oldItem);
    }

}

wxTreeItemId wxDbTreeListMainWindow::HitTest (const wxPoint& point, int& flags, int& column) {

    int w, h;
    GetSize(&w, &h);
    flags=0;
    column = -1;
    if (point.x<0) flags |= wxTREE_HITTEST_TOLEFT;
    if (point.x>w) flags |= wxTREE_HITTEST_TORIGHT;
    if (point.y<0) flags |= wxTREE_HITTEST_ABOVE;
    if (point.y>h) flags |= wxTREE_HITTEST_BELOW;
    if (flags) return wxTreeItemId();

    if (!m_rootItem) {
        flags = wxTREE_HITTEST_NOWHERE;
        column = -1;
        return wxTreeItemId();
    }

    wxDbTreeListItem *hit = m_rootItem->HitTest (CalcUnscrolledPosition(point),
                                               this, flags, column, 0);
    if (!hit) {
        flags = wxTREE_HITTEST_NOWHERE;
        column = -1;
        return wxTreeItemId();
    }
    return hit;
}

// get the bounding rectangle of the item (or of its label only)
bool wxDbTreeListMainWindow::GetBoundingRect (const wxTreeItemId& itemId, wxRect& rect,
                                            bool WXUNUSED(textOnly)) const {
    wxCHECK_MSG (itemId.IsOk(), false, _T("invalid item in wxDbTreeListMainWindow::GetBoundingRect") );

    wxDbTreeListItem *item = (wxDbTreeListItem*) itemId.m_pItem;

    int xUnit, yUnit;
    GetScrollPixelsPerUnit (&xUnit, &yUnit);
    int startX, startY;
    GetViewStart(& startX, & startY);

    rect.x = item->GetX() - startX * xUnit;
    rect.y = item->GetY() - startY * yUnit;
    rect.width = item->GetWidth();
    rect.height = GetLineHeight (item);

    return true;
}

/* **** */

void wxDbTreeListMainWindow::EditLabel (const wxTreeItemId& item, int column) {

// validate
    if (!item.IsOk()) return;
    if (!((column >= 0) && (column < GetColumnCount()))) return;

// cancel any editing
    if (m_editControl) {
        m_editControl->EndEdit(true);  // cancelled
    }

// prepare edit (position)
    m_editItem = (wxDbTreeListItem*) item.m_pItem;

    wxTreeEvent te( wxEVT_COMMAND_TREE_BEGIN_LABEL_EDIT, 0 );
    te.SetInt (column);
    SendEvent(0, m_editItem, &te); if (!te.IsAllowed()) return;

    // ensure that the position of the item it calculated in any case
    if (m_dirty) CalculatePositions();

    wxDbTreeListHeaderWindow* header_win = m_owner->GetHeaderWindow();

    // position & size are rather unpredictable (tsssk, tssssk) so were
    //  set by trial & error (on Win 2003 pre-XP style)
    int x = 0;
    int w = +4;  // +4 is necessary, don't know why (simple border erronously counted somewhere ?)
    int y = m_editItem->GetY() + 1;  // this is cell, not text
    int h = m_editItem->GetHeight() - 1;  // consequence from above
    long style = 0;
    if (column == GetMainColumn()) {
        x += m_editItem->GetTextX() - 2;  // wrong by 2, don't know why
        w += m_editItem->GetWidth();
    } else {
        for (int i = 0; i < column; ++i) x += header_win->GetColumnWidth (i); // start of column
        w += header_win->GetColumnWidth (column);  // currently non-main column width not pre-computed
    }
    switch (header_win->GetColumnAlignment (column)) {
        case wxALIGN_LEFT:   {style = wxTE_LEFT;   x -= 1; break;}
        case wxALIGN_CENTER: {style = wxTE_CENTER; x -= 1; break;}
        case wxALIGN_RIGHT:  {style = wxTE_RIGHT;  x += 0; break;}  // yes, strange but that's the way it is
    }
    // wxTextCtrl simple border style requires 2 extra pixels before and after
    //  (measured by changing to style wxNO_BORDER in wxDbEditTextControl::wxDbEditTextControl() )
    y -= 2; x -= 2;
    w += 4; h += 4;

    wxClientDC dc (this);
    PrepareDC (dc);

    x = dc.LogicalToDeviceX (x);
    y = dc.LogicalToDeviceY (y);

// now do edit (change state, show control)
    m_editCol = column;  // only used in OnRenameAccept()
    m_editControl = new wxDbEditTextControl (this, -1, &m_editAccept, &m_editRes,
                                               this, m_editItem->GetText (column),
                                               wxPoint (x, y), wxSize (w, h), style);
    										   
    										   
    m_editControl->SetFocus();
}

void wxDbTreeListMainWindow::OnRenameTimer() {
    EditLabel (m_curItem, m_curColumn);
}

void wxDbTreeListMainWindow::OnRenameAccept(bool isCancelled) {

    // TODO if the validator fails this causes a crash
    wxTreeEvent le( wxEVT_COMMAND_TREE_END_LABEL_EDIT, 0 );
    le.SetLabel( m_editRes );
    le.SetEditCanceled(isCancelled);
    le.SetInt(m_editCol);
    SendEvent(0, m_editItem, &le); if (! isCancelled  && le.IsAllowed())
    {
        SetItemText (m_editItem, le.GetInt(), le.GetLabel());
    }
}

void wxDbTreeListMainWindow::OnMouse (wxMouseEvent &event) {
bool mayDrag = true;
bool maySelect = true;  // may change selection
bool mayClick = true;  // may process DOWN clicks to expand, send click events
bool mayDoubleClick = true;  // implies mayClick
bool bSkip = true;

if ( m_editControl)  {event.Skip(); return;}//Zamyatin чтоб не переходило на другую запись если введены неправильные данные
    // send event to user code
    if (m_owner->GetEventHandler()->ProcessEvent(event)) return; // handled (and not skipped) in user code
    if (!m_rootItem) return;


// ---------- DETERMINE EVENT ----------
/*
wxLogMessage("OnMouse: LMR down=<%d, %d, %d> up=<%d, %d, %d> LDblClick=<%d> dragging=<%d>",
    event.LeftDown(), event.MiddleDown(), event.RightDown(),
    event.LeftUp(), event.MiddleUp(), event.RightUp(),
    event.LeftDClick(), event.Dragging());
*/
    wxPoint p = wxPoint (event.GetX(), event.GetY());
    int flags = 0;
    wxDbTreeListItem *item = m_rootItem->HitTest (CalcUnscrolledPosition (p),
                                                this, flags, m_curColumn, 0);
    bool bCrosshair = (item && item->HasPlus() && (flags & wxTREE_HITTEST_ONITEMBUTTON));
    // we were dragging
    if (m_isDragging) {
        maySelect = mayDoubleClick = false;
    }
    // we are starting or continuing to drag
    if (event.Dragging()) {
        maySelect = mayDoubleClick = mayClick = false;
    }
    // crosshair area is special
    if (bCrosshair) {
        // left click does not select
        if (event.LeftDown()) maySelect = false;
        // double click is ignored
        mayDoubleClick = false;
    }
    // double click only if simple click
    if (mayDoubleClick) mayDoubleClick = mayClick;
    // selection conditions --remember also that selection exludes editing
    if (maySelect) maySelect = mayClick;  // yes, select/unselect requires a click
    if (maySelect) {

        // multiple selection mode complicates things, sometimes we
        //  select on button-up instead of down:
        if (HasFlag(wxTR_MULTIPLE)) {

            // CONTROL/SHIFT key used, don't care about anything else, will
            //  toggle on key down
            if (event.ControlDown() || event.ShiftDown()) {
                maySelect = maySelect && (event.LeftDown() || event.RightDown());
                m_lastOnSame = false;  // prevent editing when keys are used

            // already selected item: to allow drag or contextual menu for multiple
            //  items, we only select/unselect on click-up --and only on LEFT
            // click, right is reserved for contextual menu
            } else if ((item != NULL && item->IsSelected())) {
                maySelect = maySelect && event.LeftUp();

            // non-selected items: select on click-down like simple select (so
            //  that a right-click contextual menu may be chained)
            } else {
                maySelect = maySelect && (event.LeftDown() || event.RightDown());
            }

        // single-select is simply on left or right click-down
        } else {
            maySelect = maySelect && (event.LeftDown() || event.RightDown());
        }
    }


// ----------  GENERAL ACTIONS  ----------

    // set focus if window clicked
    if (event.LeftDown() || event.MiddleDown() || event.RightDown()) SetFocus();

    // tooltip change ?
    if (item != m_toolTipItem) {

        // not over an item, use global tip
        if (item == NULL) {
            m_toolTipItem = NULL;
            wxScrolledWindow::SetToolTip(m_toolTip);

        // over an item
        } else {
            const wxString *tip = item->GetToolTip();

            // is there an item-specific tip ?
            if (tip) {
                m_toolTipItem = item;
                wxScrolledWindow::SetToolTip(*tip);

            // no item tip, but we are in item-specific mode (SetItemToolTip()
            //  was called after SetToolTip() )
            } else if (m_isItemToolTip) {
                m_toolTipItem = item;
                wxScrolledWindow::SetToolTip(wxString());

            // no item tip, display global tip instead; item change ignored
            } else if (m_toolTipItem != NULL) {
                m_toolTipItem = NULL;
                wxScrolledWindow::SetToolTip(m_toolTip);
            }
        }
    }


// ----------  HANDLE SIMPLE-CLICKS  (selection change, contextual menu) ----------
    if (mayClick) {

        // 2nd left-click on an item might trigger edit
        if (event.LeftDown()) m_lastOnSame = (item == m_curItem);

        // left-click on haircross is expand (and no select)
        if (bCrosshair && event.LeftDown()) {

            bSkip = false;

            // note that we only toggle the item for a single click, double
            // click on the button doesn't do anything
            Toggle (item);
        }

        if (maySelect) {
            bSkip = false;

            // set / remember item at shift down before current item gets changed
            if (event.LeftDown() && HasFlag(wxTR_MULTIPLE) && event.ShiftDown())  {
                if (!m_shiftItem) m_shiftItem = m_curItem;
            }else{
                m_shiftItem = (wxDbTreeListItem*)NULL;
            }

            // how is selection altered
            // keep or discard already selected ?
            bool unselect_others = ! (HasFlag(wxTR_MULTIPLE) && (
                event.ShiftDown()
             || event.ControlDown()
            ));

            // check is selection change is not vetoed
            if (SelectItem(item, m_shiftItem, unselect_others)) {
                // make the new item the current item
                EnsureVisible (item);
                SetCurrentItem(item);
            }
        }

        // generate click & menu events
        if (event.MiddleDown()) {
            bSkip = false;
            SendEvent(wxEVT_COMMAND_TREE_ITEM_MIDDLE_CLICK, item);
        }
        if (event.RightDown()) {
            bSkip = false;
            SendEvent(wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK, item);
        }
        if (event.RightUp()) {
            wxTreeEvent nevent(wxEVT_COMMAND_TREE_ITEM_MENU, 0);
            nevent.SetPoint(p);
            nevent.SetInt(m_curColumn);
            SendEvent(0, item, &nevent);
        }

        // if 2nd left click finishes on same item, will edit it
        if (m_lastOnSame && event.LeftUp()) {
            if ((item == m_curItem) && (m_curColumn != -1) &&
                (m_owner->GetHeaderWindow()->IsColumnEditable (m_curColumn)) &&
                (flags & (wxTREE_HITTEST_ONITEMLABEL | wxDB_TREE_HITTEST_ONITEMCOLUMN))
            ){
                m_editTimer->Start (RENAME_TIMER_TICKS, wxTIMER_ONE_SHOT);
                bSkip = false;
            }
            m_lastOnSame = false;
        }
    }


// ----------  HANDLE DOUBLE-CLICKS  ----------
    if (mayDoubleClick && event.LeftDClick()) {

        bSkip = false;

        // double clicking should not start editing the item label
        m_editTimer->Stop();
        m_lastOnSame = false;

        // selection reset to that single item which was double-clicked
        if (SelectItem(item, (wxTreeItemId*)NULL, true)) {  // unselect others --return false if vetoed

            // selection change not vetoed, send activate event
            if (! SendEvent(wxEVT_COMMAND_TREE_ITEM_ACTIVATED, item)) {

                // if the user code didn't process the activate event,
                // handle it ourselves by toggling the item when it is
                // double clicked
                if (item && item->HasPlus()) Toggle(item);
            }
        }
    }


// ----------  HANDLE DRAGGING  ----------
// NOTE: drag itself makes no change to selection
    if (mayDrag) {  // actually this is always true

        // CASE 1: we were dragging => continue, end, abort
        if (m_isDragging) {

            // CASE 1.1: click aborts drag:
            if (event.LeftDown() || event.MiddleDown() || event.RightDown()) {

                bSkip = false;

                // stop dragging
                m_isDragStarted = m_isDragging = false;
                if (HasCapture()) ReleaseMouse();
                RefreshSelected();

            // CASE 1.2: still dragging
            } else if (event.Dragging()) {

                ;; // nothing to do

            // CASE 1.3: dragging now ends normally
            } else {

                bSkip = false;

                // stop dragging
                m_isDragStarted = m_isDragging = false;
                if (HasCapture()) ReleaseMouse();
                RefreshSelected();

                // send drag end event
                wxTreeEvent event(wxEVT_COMMAND_TREE_END_DRAG, 0);
                event.SetPoint(p);
                event.SetInt(m_curColumn);
                SendEvent(0, item, &event);
            }

        // CASE 2: not were not dragging => continue, start
        } else if (event.Dragging()) {

            // We will really start dragging if we've moved beyond a few pixels
            if (m_isDragStarted) {
                const int tolerance = 3;
                int dx = abs(p.x - m_dragStartPos.x);
                int dy = abs(p.y - m_dragStartPos.y);
                if (dx <= tolerance && dy <= tolerance)
                    return;
            // determine drag start
            } else {
                m_dragStartPos = p;
                m_dragCol = m_curColumn;
                m_dragItem = item;
                m_isDragStarted = true;
                return;
            }

            bSkip = false;

            // we are now dragging
            m_isDragging = true;
            RefreshSelected();
            CaptureMouse(); // TODO: usefulness unclear

            wxTreeEvent nevent(event.LeftIsDown()
                                  ? wxEVT_COMMAND_TREE_BEGIN_DRAG
                                  : wxEVT_COMMAND_TREE_BEGIN_RDRAG, 0);
            nevent.SetPoint(p);
            nevent.SetInt(m_dragCol);
            nevent.Veto();
            SendEvent(0, m_dragItem, &nevent);
        }
    }


    if (bSkip) event.Skip();
}


void wxDbTreeListMainWindow::OnIdle (wxIdleEvent &WXUNUSED(event)) {
    /* after all changes have been done to the tree control,
     * we actually redraw the tree when everything is over */

    if (!m_dirty) return;

    m_dirty = false;

    CalculatePositions();
    Refresh();
    AdjustMyScrollbars();
}

void wxDbTreeListMainWindow::OnScroll (wxScrollWinEvent& event) {
    // FIXME
#if defined(__WXGTK__) && !defined(__WXUNIVERSAL__)
    wxScrolledWindow::OnScroll(event);
#else
    HandleOnScroll( event );
#endif

    if(event.GetOrientation() == wxHORIZONTAL) {
        m_owner->GetHeaderWindow()->Refresh();
        m_owner->GetHeaderWindow()->Update();
    }
}

void wxDbTreeListMainWindow::CalculateSize (wxDbTreeListItem *item, wxDC &dc) {
    wxCoord text_w = 0;
    wxCoord text_h = 0;

    dc.SetFont (GetItemFont (item));
    dc.GetTextExtent (item->GetText(m_main_column).size() > 0
            ? item->GetText (m_main_column)
            : _T(" "),  // blank to avoid zero height and no highlight width
        &text_w, &text_h);
    // restore normal font
    dc.SetFont (m_normalFont);

    int max_h = (m_imgHeight > text_h) ? m_imgHeight : text_h;
    if (max_h < 30) { // add 10% space if greater than 30 pixels
        max_h += 2; // minimal 2 pixel space
    }else{
        max_h += max_h / 10; // otherwise 10% space
    }

    item->SetHeight (max_h);
    if (max_h > m_lineHeight) m_lineHeight = max_h;
    item->SetWidth(m_imgWidth + text_w+2);
}

// -----------------------------------------------------------------------------
void wxDbTreeListMainWindow::CalculateLevel (wxDbTreeListItem *item, wxDC &dc,
                                           int level, int &y, int x_colstart) {

    // calculate position of vertical lines
    int x = x_colstart + MARGIN; // start of column
    if (HasFlag(wxTR_LINES_AT_ROOT)) x += LINEATROOT; // space for lines at root
    if (HasButtons()) {
        x += (m_btnWidth-m_btnWidth2); // half button space
    }else{
        x += (m_indent-m_indent/2);
    }
    if (HasFlag(wxTR_HIDE_ROOT)) {
        x += m_indent * (level-1); // indent but not level 1
    }else{
        x += m_indent * level; // indent according to level
    }

    // a hidden root is not evaluated, but its children are always
    if (HasFlag(wxTR_HIDE_ROOT) && (level == 0)) goto Recurse;

    CalculateSize( item, dc );

    // set its position
    item->SetX (x);
    item->SetY (y);
    y += GetLineHeight(item);

    // we don't need to calculate collapsed branches
    if ( !item->IsExpanded() ) return;

Recurse:
    wxArrayTreeListItems& children = item->GetChildren();
    long n, count = (long)children.Count();
    ++level;
    for (n = 0; n < count; ++n) {
        CalculateLevel( children[n], dc, level, y, x_colstart );  // recurse
    }
}

void wxDbTreeListMainWindow::CalculatePositions() {
    if ( !m_rootItem ) return;

    wxClientDC dc(this);
    PrepareDC( dc );

    dc.SetFont( m_normalFont );

    dc.SetPen( m_dottedPen );
    //if(GetImageList() == NULL)
    // m_lineHeight = (int)(dc.GetCharHeight() + 4);

    int y = 2;
    int x_colstart = 0;
    for (int i = 0; i < (int)GetMainColumn(); ++i) {
        if (!m_owner->GetHeaderWindow()->IsColumnShown(i)) continue;
        x_colstart += m_owner->GetHeaderWindow()->GetColumnWidth(i);
    }
    CalculateLevel( m_rootItem, dc, 0, y, x_colstart ); // start recursion
}

void wxDbTreeListMainWindow::RefreshSubtree (wxDbTreeListItem *item) {
    if (m_dirty) return;

    wxClientDC dc(this);
    PrepareDC(dc);

    int cw = 0;
    int ch = 0;
    GetVirtualSize( &cw, &ch );

    wxRect rect;
    rect.x = dc.LogicalToDeviceX( 0 );
    rect.width = cw;
    rect.y = dc.LogicalToDeviceY( item->GetY() - 2 );
    rect.height = ch;

    Refresh (true, &rect );
    AdjustMyScrollbars();
}

void wxDbTreeListMainWindow::RefreshLine (wxDbTreeListItem *item) {
    if (m_dirty) return;

    wxClientDC dc(this);
    PrepareDC( dc );

    int cw = 0;
    int ch = 0;
    GetVirtualSize( &cw, &ch );

    wxRect rect;
    rect.x = dc.LogicalToDeviceX( 0 );
    rect.y = dc.LogicalToDeviceY( item->GetY() );
    rect.width = cw;
    rect.height = GetLineHeight(item); //dc.GetCharHeight() + 6;

    Refresh (true, &rect);
}

void wxDbTreeListMainWindow::RefreshSelected() {
    // TODO: this is awfully inefficient, we should keep the list of all
    //       selected items internally, should be much faster
    if (m_rootItem) {
        RefreshSelectedUnder (m_rootItem);
    }
}

void wxDbTreeListMainWindow::RefreshSelectedUnder (wxDbTreeListItem *item) {
    if (item->IsSelected()) {
        RefreshLine (item);
    }

    const wxArrayTreeListItems& children = item->GetChildren();
    long count = (long)children.GetCount();
    for (long n = 0; n < count; n++ ) {
        RefreshSelectedUnder (children[n]);
    }
}

// ----------------------------------------------------------------------------
// changing colours: we need to refresh the tree control
// ----------------------------------------------------------------------------

bool wxDbTreeListMainWindow::SetBackgroundColour (const wxColour& colour) {
    if (!wxWindow::SetBackgroundColour(colour)) return false;

    Refresh();
    return true;
}

bool wxDbTreeListMainWindow::SetForegroundColour (const wxColour& colour) {
    if (!wxWindow::SetForegroundColour(colour)) return false;

    Refresh();
    return true;
}

void wxDbTreeListMainWindow::SetItemText (const wxTreeItemId& itemId, int column,
                                        const wxString& text) {
    wxCHECK_RET (itemId.IsOk(), _T("invalid tree item"));

    wxClientDC dc (this);
    wxDbTreeListItem *item = (wxDbTreeListItem*) itemId.m_pItem;
    item->SetText (column, text);
    CalculateSize (item, dc);
    RefreshLine (item);
}

wxString wxDbTreeListMainWindow::GetItemText (const wxTreeItemId& itemId,
                                            int column) const {
    wxCHECK_MSG (itemId.IsOk(), _T(""), _T("invalid tree item") );

    if( IsVirtual() )   return m_owner->OnGetItemText(((wxDbTreeListItem*) itemId.m_pItem)->GetData(),column);
    else                return ((wxDbTreeListItem*) itemId.m_pItem)->GetText (column);
}

wxString wxDbTreeListMainWindow::GetItemText (wxTreeItemData* item,
int column) const {
   wxASSERT_MSG( IsVirtual(), _T("can be used only with virtual control") );
   return m_owner->OnGetItemText(item,column);
}

void wxDbTreeListMainWindow::SetFocus() {
    wxWindow::SetFocus();
}

wxFont wxDbTreeListMainWindow::GetItemFont (wxDbTreeListItem *item) {
    wxTreeItemAttr *attr = item->GetAttributes();

    if (attr && attr->HasFont()) {
        return attr->GetFont();
    }else if (item->IsBold()) {
        return m_boldFont;
    }else{
        return m_normalFont;
   }
}

int wxDbTreeListMainWindow::GetItemWidth (int column, wxDbTreeListItem *item) {
    if (!item) return 0;

    // determine item width
    int w = 0, h = 0;
    wxFont font = GetItemFont (item);
    GetTextExtent (item->GetText (column), &w, &h, NULL, NULL, font.Ok()? &font: NULL);
    w += 2*MARGIN;

    // calculate width
    int width = w + 2*MARGIN;
    if (column == GetMainColumn()) {
        width += MARGIN;
        if (HasFlag(wxTR_LINES_AT_ROOT)) width += LINEATROOT;
        if (HasButtons()) width += m_btnWidth + LINEATROOT;
        if (item->GetCurrentImage() != NO_IMAGE) width += m_imgWidth;

        // count indent level
        int level = 0;
        wxDbTreeListItem *parent = item->GetItemParent();
        wxDbTreeListItem *root = (wxDbTreeListItem*)GetRootItem().m_pItem;
        while (parent && (!HasFlag(wxTR_HIDE_ROOT) || (parent != root))) {
            level++;
            parent = parent->GetItemParent();
        }
        if (level) width += level * GetIndent();
    }

    return width;
}

int wxDbTreeListMainWindow::GetBestColumnWidth (int column, wxTreeItemId parent) {
    int maxWidth, h;
    GetClientSize (&maxWidth, &h);
    int width = 0;

    // get root if on item
    if (!parent.IsOk()) parent = GetRootItem();

    // add root width
    if (!HasFlag(wxTR_HIDE_ROOT)) {
        int w = GetItemWidth (column, (wxDbTreeListItem*)parent.m_pItem);
        if (width < w) width = w;
        if (width > maxWidth) return maxWidth;
    }

    wxTreeItemIdValue cookie = 0;
    wxTreeItemId item = GetFirstChild (parent, cookie);
    while (item.IsOk()) {
        int w = GetItemWidth (column, (wxDbTreeListItem*)item.m_pItem);
        if (width < w) width = w;
        if (width > maxWidth) return maxWidth;

        // check the children of this item
        if (((wxDbTreeListItem*)item.m_pItem)->IsExpanded()) {
            int w = GetBestColumnWidth (column, item);
            if (width < w) width = w;
            if (width > maxWidth) return maxWidth;
        }

        // next sibling
        item = GetNextChild (parent, cookie);
    }

    return width;
}


bool wxDbTreeListMainWindow::SendEvent(wxEventType event_type, wxDbTreeListItem *item, wxTreeEvent *event) {
wxTreeEvent nevent (event_type, 0);

    if (event == NULL) {
        event = &nevent;
        event->SetInt (m_curColumn); // the mouse colum
    }

    event->SetEventObject (m_owner);
    event->SetId(m_owner->GetId());
    if (item) {
#if !wxCHECK_VERSION(2, 5, 0)
        event->SetItem ((long)item);
#else
        event->SetItem (item);
#endif
    }

    return m_owner->GetEventHandler()->ProcessEvent (*event);
}


//-----------------------------------------------------------------------------
//  wxDbTreeListCtrl
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxDbTreeListCtrl, wxControl);

BEGIN_EVENT_TABLE(wxDbTreeListCtrl, wxControl)
    EVT_SIZE(wxDbTreeListCtrl::OnSize)
END_EVENT_TABLE();

bool wxDbTreeListCtrl::Create(wxWindow *parent, wxWindowID id,
                            const wxPoint& pos,
                            const wxSize& size,
                            long style, const wxValidator &validator,
                            const wxString& name)
{
    long main_style = style & ~(wxSIMPLE_BORDER|wxSUNKEN_BORDER|wxDOUBLE_BORDER|
                                wxRAISED_BORDER|wxSTATIC_BORDER);
         main_style |= wxWANTS_CHARS ;
    long ctrl_style = style & ~(wxVSCROLL|wxHSCROLL);

    if (!wxControl::Create(parent, id, pos, size, ctrl_style, validator, name)) {
       return false;
    }
    m_main_win = new wxDbTreeListMainWindow (this, -1, wxPoint(0, 0), size,
                                           main_style, validator);
    m_header_win = new wxDbTreeListHeaderWindow (this, -1, m_main_win,
                                               wxPoint(0, 0), wxDefaultSize,
                                               wxTAB_TRAVERSAL);
    CalculateAndSetHeaderHeight();
    return true;
}

void wxDbTreeListCtrl::CalculateAndSetHeaderHeight()
{
    if (m_header_win) {

        // we use 'g' to get the descent, too
        int h;
#if wxCHECK_VERSION_FULL(2, 7, 0, 1)
#ifdef __WXMSW__
        h = (int)(wxRendererNative::Get().GetHeaderButtonHeight(m_header_win) * 0.8) + 2;
#else
        h = wxRendererNative::Get().GetHeaderButtonHeight(m_header_win);
#endif
#else
        int w, d;
        m_header_win->GetTextExtent(_T("Hg"), &w, &h, &d);
        h += d + 2 * HEADER_OFFSET_Y + EXTRA_HEIGHT;
#endif

        // only update if changed
        if (h != m_headerHeight) {
            m_headerHeight = h;
            DoHeaderLayout();
        }
    }
}

void wxDbTreeListCtrl::DoHeaderLayout()
{
    int w, h;
    GetClientSize(&w, &h);
    if (m_header_win) {
        m_header_win->SetSize (0, 0, w, m_headerHeight);
        m_header_win->Refresh();
    }
    if (m_main_win) {
        m_main_win->SetSize (0, m_headerHeight, w, h - m_headerHeight);
    }
}

void wxDbTreeListCtrl::OnSize(wxSizeEvent& WXUNUSED(event))
{
    DoHeaderLayout();
}

size_t wxDbTreeListCtrl::GetCount() const { return m_main_win->GetCount(); }

unsigned int wxDbTreeListCtrl::GetIndent() const
{ return m_main_win->GetIndent(); }

void wxDbTreeListCtrl::SetIndent(unsigned int indent)
{ m_main_win->SetIndent(indent); }

unsigned int wxDbTreeListCtrl::GetLineSpacing() const
{ return m_main_win->GetLineSpacing(); }

void wxDbTreeListCtrl::SetLineSpacing(unsigned int spacing)
{ m_main_win->SetLineSpacing(spacing); }

wxImageList* wxDbTreeListCtrl::GetImageList() const
{ return m_main_win->GetImageList(); }

wxImageList* wxDbTreeListCtrl::GetStateImageList() const
{ return m_main_win->GetStateImageList(); }

wxImageList* wxDbTreeListCtrl::GetButtonsImageList() const
{ return m_main_win->GetButtonsImageList(); }

void wxDbTreeListCtrl::SetImageList(wxImageList* imageList)
{ m_main_win->SetImageList(imageList); }

void wxDbTreeListCtrl::SetStateImageList(wxImageList* imageList)
{ m_main_win->SetStateImageList(imageList); }

void wxDbTreeListCtrl::SetButtonsImageList(wxImageList* imageList)
{ m_main_win->SetButtonsImageList(imageList); }

void wxDbTreeListCtrl::AssignImageList(wxImageList* imageList)
{ m_main_win->AssignImageList(imageList); }

void wxDbTreeListCtrl::AssignStateImageList(wxImageList* imageList)
{ m_main_win->AssignStateImageList(imageList); }

void wxDbTreeListCtrl::AssignButtonsImageList(wxImageList* imageList)
{ m_main_win->AssignButtonsImageList(imageList); }

wxString wxDbTreeListCtrl::GetItemText(const wxTreeItemId& item, int column) const
{ return m_main_win->GetItemText (item, column); }

int wxDbTreeListCtrl::GetItemImage(const wxTreeItemId& item, int column,
                                 wxTreeItemIcon which) const
{ return m_main_win->GetItemImage(item, column, which); }

wxTreeItemData* wxDbTreeListCtrl::GetItemData(const wxTreeItemId& item) const
{ return m_main_win->GetItemData(item); }

bool wxDbTreeListCtrl::GetItemBold(const wxTreeItemId& item) const
{ return m_main_win->GetItemBold(item); }

wxColour wxDbTreeListCtrl::GetItemTextColour(const wxTreeItemId& item) const
{ return m_main_win->GetItemTextColour(item); }

wxColour wxDbTreeListCtrl::GetItemBackgroundColour(const wxTreeItemId& item)
    const
{ return m_main_win->GetItemBackgroundColour(item); }

wxFont wxDbTreeListCtrl::GetItemFont(const wxTreeItemId& item) const
{ return m_main_win->GetItemFont(item); }


void wxDbTreeListCtrl::SetItemText(const wxTreeItemId& item, int column,
                                 const wxString& text)
{ m_main_win->SetItemText (item, column, text); }

void wxDbTreeListCtrl::SetItemImage(const wxTreeItemId& item,
                                  int column,
                                  int image,
                                  wxTreeItemIcon which)
{ m_main_win->SetItemImage(item, column, image, which); }

void wxDbTreeListCtrl::SetItemData(const wxTreeItemId& item,
                                 wxTreeItemData* data)
{ m_main_win->SetItemData(item, data); }

void wxDbTreeListCtrl::SetItemHasChildren(const wxTreeItemId& item, bool has)
{ m_main_win->SetItemHasChildren(item, has); }

void wxDbTreeListCtrl::SetItemBold(const wxTreeItemId& item, bool bold)
{ m_main_win->SetItemBold(item, bold); }

void wxDbTreeListCtrl::SetItemTextColour(const wxTreeItemId& item,
                                       const wxColour& colour)
{ m_main_win->SetItemTextColour(item, colour); }

void wxDbTreeListCtrl::SetItemBackgroundColour(const wxTreeItemId& item,
                                             const wxColour& colour)
{ m_main_win->SetItemBackgroundColour(item, colour); }

void wxDbTreeListCtrl::SetItemFont(const wxTreeItemId& item,
                                 const wxFont& font)
{ m_main_win->SetItemFont(item, font); }

bool wxDbTreeListCtrl::SetFont(const wxFont& font)
{
    if (m_header_win) {
        m_header_win->SetFont(font);
        CalculateAndSetHeaderHeight();
        m_header_win->Refresh();
    }
    if (m_main_win) {
        return m_main_win->SetFont(font);
    }else{
        return false;
    }
}

void wxDbTreeListCtrl::SetWindowStyle(const long style)
{
    if(m_main_win)
        m_main_win->SetWindowStyle(style);
    m_windowStyle = style;
    // TODO: provide something like wxTL_NO_HEADERS to hide m_header_win
}

long wxDbTreeListCtrl::GetWindowStyle() const
{
    long style = m_windowStyle;
    if(m_main_win)
        style |= m_main_win->GetWindowStyle();
    return style;
}

bool wxDbTreeListCtrl::IsVisible(const wxTreeItemId& item, bool fullRow, bool within) const
{ return m_main_win->IsVisible(item, fullRow, within); }

bool wxDbTreeListCtrl::HasChildren(const wxTreeItemId& item) const
{ return m_main_win->HasChildren(item); }

bool wxDbTreeListCtrl::IsExpanded(const wxTreeItemId& item) const
{ return m_main_win->IsExpanded(item); }

bool wxDbTreeListCtrl::IsSelected(const wxTreeItemId& item) const
{ return m_main_win->IsSelected(item); }

bool wxDbTreeListCtrl::IsBold(const wxTreeItemId& item) const
{ return m_main_win->IsBold(item); }

size_t wxDbTreeListCtrl::GetChildrenCount(const wxTreeItemId& item, bool rec)
{ return m_main_win->GetChildrenCount(item, rec); }

wxTreeItemId wxDbTreeListCtrl::GetRootItem() const
{ return m_main_win->GetRootItem(); }

wxTreeItemId wxDbTreeListCtrl::GetSelection() const
{ return m_main_win->GetSelection(); }

size_t wxDbTreeListCtrl::GetSelections(wxArrayTreeItemIds& arr) const
{ return m_main_win->GetSelections(arr); }

wxTreeItemId wxDbTreeListCtrl::GetItemParent(const wxTreeItemId& item) const
{ return m_main_win->GetItemParent(item); }

#if !wxCHECK_VERSION(2, 5, 0)
wxTreeItemId wxDbTreeListCtrl::GetFirstChild (const wxTreeItemId& item,
                                            long& cookie) const
#else
wxTreeItemId wxDbTreeListCtrl::GetFirstChild (const wxTreeItemId& item,
                                            wxTreeItemIdValue& cookie) const
#endif
{ return m_main_win->GetFirstChild(item, cookie); }

#if !wxCHECK_VERSION(2, 5, 0)
wxTreeItemId wxDbTreeListCtrl::GetNextChild (const wxTreeItemId& item,
                                           long& cookie) const
#else
wxTreeItemId wxDbTreeListCtrl::GetNextChild (const wxTreeItemId& item,
                                           wxTreeItemIdValue& cookie) const
#endif
{ return m_main_win->GetNextChild(item, cookie); }

#if !wxCHECK_VERSION(2, 5, 0)
wxTreeItemId wxDbTreeListCtrl::GetPrevChild (const wxTreeItemId& item,
                                           long& cookie) const
#else
wxTreeItemId wxDbTreeListCtrl::GetPrevChild (const wxTreeItemId& item,
                                           wxTreeItemIdValue& cookie) const
#endif
{ return m_main_win->GetPrevChild(item, cookie); }

#if !wxCHECK_VERSION(2, 5, 0)
wxTreeItemId wxDbTreeListCtrl::GetLastChild (const wxTreeItemId& item,
                                           long& cookie) const
#else
wxTreeItemId wxDbTreeListCtrl::GetLastChild (const wxTreeItemId& item,
                                           wxTreeItemIdValue& cookie) const
#endif
{ return m_main_win->GetLastChild(item, cookie); }


wxTreeItemId wxDbTreeListCtrl::GetNextSibling(const wxTreeItemId& item) const
{ return m_main_win->GetNextSibling(item); }

wxTreeItemId wxDbTreeListCtrl::GetPrevSibling(const wxTreeItemId& item) const
{ return m_main_win->GetPrevSibling(item); }

wxTreeItemId wxDbTreeListCtrl::GetNext(const wxTreeItemId& item) const
{ return m_main_win->GetNext(item, true); }

wxTreeItemId wxDbTreeListCtrl::GetPrev(const wxTreeItemId& item) const
{ return m_main_win->GetPrev(item, true); }

wxTreeItemId wxDbTreeListCtrl::GetFirstExpandedItem() const
{ return m_main_win->GetFirstExpandedItem(); }

wxTreeItemId wxDbTreeListCtrl::GetNextExpanded(const wxTreeItemId& item) const
{ return m_main_win->GetNextExpanded(item); }

wxTreeItemId wxDbTreeListCtrl::GetPrevExpanded(const wxTreeItemId& item) const
{ return m_main_win->GetPrevExpanded(item); }

wxTreeItemId wxDbTreeListCtrl::GetFirstVisibleItem(bool fullRow) const
{ return GetFirstVisible(fullRow); }
wxTreeItemId wxDbTreeListCtrl::GetFirstVisible(bool fullRow, bool within) const
{ return m_main_win->GetFirstVisible(fullRow, within); }

wxTreeItemId wxDbTreeListCtrl::GetLastVisible(bool fullRow, bool within) const
{ return m_main_win->GetLastVisible(fullRow, within); }

wxTreeItemId wxDbTreeListCtrl::GetNextVisible(const wxTreeItemId& item, bool fullRow, bool within) const
{ return m_main_win->GetNextVisible(item, fullRow, within); }

wxTreeItemId wxDbTreeListCtrl::GetPrevVisible(const wxTreeItemId& item, bool fullRow, bool within) const
{ return m_main_win->GetPrevVisible(item, fullRow, within); }

wxTreeItemId wxDbTreeListCtrl::AddRoot (const wxString& text, int image,
                                      int selectedImage, wxTreeItemData* data)
{ return m_main_win->AddRoot (text, image, selectedImage, data); }

wxTreeItemId wxDbTreeListCtrl::PrependItem(const wxTreeItemId& parent,
                                         const wxString& text, int image,
                                         int selectedImage,
                                         wxTreeItemData* data)
{ return m_main_win->PrependItem(parent, text, image, selectedImage, data); }

wxTreeItemId wxDbTreeListCtrl::InsertItem(const wxTreeItemId& parent,
                                        const wxTreeItemId& previous,
                                        const wxString& text, int image,
                                        int selectedImage,
                                        wxTreeItemData* data)
{
    return m_main_win->InsertItem(parent, previous, text, image,
                                  selectedImage, data);
}

wxTreeItemId wxDbTreeListCtrl::InsertItem(const wxTreeItemId& parent,
                                        size_t index,
                                        const wxString& text, int image,
                                        int selectedImage,
                                        wxTreeItemData* data)
{
    return m_main_win->InsertItem(parent, index, text, image,
                                  selectedImage, data);
}

wxTreeItemId wxDbTreeListCtrl::AppendItem(const wxTreeItemId& parent,
                                        const wxString& text, int image,
                                        int selectedImage,
                                        wxTreeItemData* data)
{ return m_main_win->AppendItem(parent, text, image, selectedImage, data); }

void wxDbTreeListCtrl::Delete(const wxTreeItemId& item)
{ m_main_win->Delete(item); }

void wxDbTreeListCtrl::DeleteChildren(const wxTreeItemId& item)
{ m_main_win->DeleteChildren(item); }

void wxDbTreeListCtrl::DeleteRoot()
{ m_main_win->DeleteRoot(); }

void wxDbTreeListCtrl::Expand(const wxTreeItemId& item)
{ m_main_win->Expand(item); }

void wxDbTreeListCtrl::ExpandAll(const wxTreeItemId& item)
{ m_main_win->ExpandAll(item); }

void wxDbTreeListCtrl::Collapse(const wxTreeItemId& item)
{ m_main_win->Collapse(item); }

void wxDbTreeListCtrl::CollapseAndReset(const wxTreeItemId& item)
{ m_main_win->CollapseAndReset(item); }

void wxDbTreeListCtrl::Toggle(const wxTreeItemId& item)
{ m_main_win->Toggle(item); }

void wxDbTreeListCtrl::Unselect()
{ m_main_win->Unselect(); }

void wxDbTreeListCtrl::UnselectAll()
{ m_main_win->UnselectAll(); }

bool wxDbTreeListCtrl::SelectItem(const wxTreeItemId& item, const wxTreeItemId& last,
                                bool unselect_others)
{ return m_main_win->SelectItem (item, last, unselect_others); }

void wxDbTreeListCtrl::SelectAll()
{ m_main_win->SelectAll(); }

void wxDbTreeListCtrl::EnsureVisible(const wxTreeItemId& item)
{ m_main_win->EnsureVisible(item); }

void wxDbTreeListCtrl::ScrollTo(const wxTreeItemId& item)
{ m_main_win->ScrollTo(item); }

wxTreeItemId wxDbTreeListCtrl::HitTest(const wxPoint& pos, int& flags, int& column)
{
    wxPoint p = m_main_win->ScreenToClient (ClientToScreen (pos));
    return m_main_win->HitTest (p, flags, column);
}

bool wxDbTreeListCtrl::GetBoundingRect(const wxTreeItemId& item, wxRect& rect,
                                     bool textOnly) const
{ return m_main_win->GetBoundingRect(item, rect, textOnly); }

void wxDbTreeListCtrl::EditLabel (const wxTreeItemId& item, int column)
{ m_main_win->EditLabel (item, column); }

int wxDbTreeListCtrl::OnCompareItems(const wxTreeItemId& item1,
                                   const wxTreeItemId& item2)
{
    // do the comparison here, and not delegate to m_main_win, in order
    // to let the user override it
    //return m_main_win->OnCompareItems(item1, item2);
    return wxStrcmp(GetItemText(item1), GetItemText(item2));
}

void wxDbTreeListCtrl::SortChildren(const wxTreeItemId& item)
{ m_main_win->SortChildren(item); }

wxTreeItemId wxDbTreeListCtrl::FindItem (const wxTreeItemId& item, const wxString& str, int mode)
{ return m_main_win->FindItem (item, str, mode); }

void wxDbTreeListCtrl::SetDragItem (const wxTreeItemId& item)
{ m_main_win->SetDragItem (item); }

bool wxDbTreeListCtrl::SetBackgroundColour(const wxColour& colour)
{
    if (!m_main_win) return false;
    return m_main_win->SetBackgroundColour(colour);
}

bool wxDbTreeListCtrl::SetForegroundColour(const wxColour& colour)
{
    if (!m_main_win) return false;
    return m_main_win->SetForegroundColour(colour);
}

int wxDbTreeListCtrl::GetColumnCount() const
{ return m_main_win->GetColumnCount(); }

void wxDbTreeListCtrl::SetColumnWidth(int column, int width)
{
    m_header_win->SetColumnWidth (column, width);
    m_header_win->Refresh();
}

int wxDbTreeListCtrl::GetColumnWidth(int column) const
{ return m_header_win->GetColumnWidth(column); }

void wxDbTreeListCtrl::SetMainColumn(int column)
{ m_main_win->SetMainColumn(column); }

int wxDbTreeListCtrl::GetMainColumn() const
{ return m_main_win->GetMainColumn(); }

void wxDbTreeListCtrl::SetColumnText(int column, const wxString& text)
{
    m_header_win->SetColumnText (column, text);
    m_header_win->Refresh();
}

wxString wxDbTreeListCtrl::GetColumnText(int column) const
{ return m_header_win->GetColumnText(column); }

void wxDbTreeListCtrl::AddColumn(const wxDbTreeListColumnInfo& colInfo)
{
    m_header_win->AddColumn (colInfo);
    DoHeaderLayout();
}

void wxDbTreeListCtrl::InsertColumn(int before, const wxDbTreeListColumnInfo& colInfo)
{
    m_header_win->InsertColumn (before, colInfo);
    m_header_win->Refresh();
}

void wxDbTreeListCtrl::RemoveColumn(int column)
{
    m_header_win->RemoveColumn (column);
    m_header_win->Refresh();
}

void wxDbTreeListCtrl::SetColumn(int column, const wxDbTreeListColumnInfo& colInfo)
{
    m_header_win->SetColumn (column, colInfo);
    m_header_win->Refresh();
}

const wxDbTreeListColumnInfo& wxDbTreeListCtrl::GetColumn(int column) const
{ return m_header_win->GetColumn(column); }

wxDbTreeListColumnInfo& wxDbTreeListCtrl::GetColumn(int column)
{ return m_header_win->GetColumn(column); }

void wxDbTreeListCtrl::SetColumnImage(int column, int image)
{
    m_header_win->SetColumn (column, GetColumn(column).SetImage(image));
    m_header_win->Refresh();
}

int wxDbTreeListCtrl::GetColumnImage(int column) const
{
    return m_header_win->GetColumn(column).GetImage();
}

void wxDbTreeListCtrl::SetColumnEditable(int column, bool shown)
{
    m_header_win->SetColumn (column, GetColumn(column).SetEditable(shown));
}

void wxDbTreeListCtrl::SetColumnShown(int column, bool shown)
{
    wxASSERT_MSG (column != GetMainColumn(), _T("The main column may not be hidden") );
    m_header_win->SetColumn (column, GetColumn(column).SetShown(GetMainColumn()==column? true: shown));
    m_header_win->Refresh();
}

bool wxDbTreeListCtrl::IsColumnEditable(int column) const
{
    return m_header_win->GetColumn(column).IsEditable();
}

bool wxDbTreeListCtrl::IsColumnShown(int column) const
{
    return m_header_win->GetColumn(column).IsShown();
}

void wxDbTreeListCtrl::SetColumnAlignment (int column, int flag)
{
    m_header_win->SetColumn(column, GetColumn(column).SetAlignment(flag));
    m_header_win->Refresh();
}

int wxDbTreeListCtrl::GetColumnAlignment(int column) const
{
    return m_header_win->GetColumn(column).GetAlignment();
}

void wxDbTreeListCtrl::Refresh(bool erase, const wxRect* rect)
{
    m_main_win->Refresh (erase, rect);
    m_header_win->Refresh (erase, rect);
}

void wxDbTreeListCtrl::SetFocus()
{ m_main_win->SetFocus(); }

wxSize wxDbTreeListCtrl::DoGetBestSize() const
{
    // something is better than nothing...
    return wxSize (200,200); // but it should be specified values! FIXME
}

wxString wxDbTreeListCtrl::OnGetItemText( wxTreeItemData* WXUNUSED(item), long WXUNUSED(column)) const
{
    return wxEmptyString;
}

void wxDbTreeListCtrl::SetToolTip(const wxString& tip) {
    m_header_win->SetToolTip(tip);
    m_main_win->SetToolTip(tip);
}
void wxDbTreeListCtrl::SetToolTip(wxToolTip *tip) {
    m_header_win->SetToolTip(tip);
    m_main_win->SetToolTip(tip);
}

void wxDbTreeListCtrl::SetItemToolTip(const wxTreeItemId& item, const wxString &tip) {
    m_main_win->SetItemToolTip(item, tip);
}
