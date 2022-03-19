#include "mimiccanvas.h"
#include "mimictype.h"
#include "mimicset.h"
#include "mimicobject.h"

/*!
 * \brief MimicCanvas::addObject
 * \param x
 * \param y
 * \param key
 */
void MimicCanvas::addObject(int x, int y, const std::string &key)
{
    int xx = 0;
    int yy = 0;
    CalcUnscrolledPosition( x, y, &xx, &yy );

    _objects.addObject(wxPoint(xx,yy),key);
    Refresh();
    Update();

}


void MimicCanvas::setBackground(const std::string &s)
{
    _backgroundImage.LoadFile(s,wxBITMAP_TYPE_ANY);
    Refresh();
}

void MimicCanvas::open()
{
    wxFileDialog  openFileDialog(this, _("Open Mimic File"), "", "", "MIM files (*.mim)|*.mim", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL) return;     // the user changed idea...
    std::string f = openFileDialog.GetPath().ToStdString();
    readSet(f);

}

void MimicCanvas::readSet(const std::string &f)
{
    _fileName = f;
    if(wxFile::Exists(f))
    {
        _objects.clear();
        // proceed loading the file chosen by the user;
        if(_objects.data().load(_fileName))
        {
            _objects.fromData();
        }
        else
        {
            wxLogDebug("Failed to load mim file");
        }
    }
    Refresh();

}

void MimicCanvas::save()
{
    if(_fileName.empty())
    {
        saveAs();
    }
    else
    {
        _objects.toData();
        _objects.data().save(_fileName);
    }

}

void MimicCanvas::saveAs()
{
    wxFileDialog  saveFileDialog(this, _("Save Mimic File"), "", "", "MIM files (*.mim)|*.mim", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_CANCEL) return;     // the user changed idea...
    _fileName = saveFileDialog.GetPath().ToStdString();
    _objects.toData();
    _objects.data().save(_fileName);

}
//
void MimicCanvas::OnDraw (wxDC &dc)
{
    wxRect r = this->GetRect(); // when pan and zoom added change to draw only in region
    if(_backgroundImage.IsOk())
    {
        // add the background image
        dc.DrawBitmap(_backgroundImage,0,0);
    }
    _objects.draw(dc,r);

}
void MimicCanvas::OnCaptureLost(wxMouseCaptureLostEvent& event)
{
    _state = NONE;
    ReleaseMouse();
    Refresh();
}
void MimicCanvas::onChar(wxKeyEvent& event)
{

}
void MimicCanvas::onLeftDown(wxMouseEvent& event)
{
    //
    switch(_state)
    {
    case OBJECT_SELECT:
    case ANCHOR_SELECT:
    {
        if(editMode())
        {

            if(_currentObject)
            {
                int x,y,xx,yy ;
                event.GetPosition(&x,&y);
                CalcUnscrolledPosition( x, y, &xx, &yy );
                _currentpoint = wxPoint( xx, yy ) ;

                _rect = _currentObject->rect();
                if(_currentObject->canResize())
                {
                    _hitPoint = _objects.hitPoint(_currentpoint, _currentObject->rect());
                    //
                    switch(_hitPoint)
                    {
                    case MIMIC::MimicSet::HIT_OBJECT:
                        _state = OBJECT_SELECT;
                        if(!HasCapture())
                        {
                            CaptureMouse() ;
                            SetCursor(*wxCROSS_CURSOR);
                        }
                        break;
                    case MIMIC::MimicSet::HIT_ANCHOR1:
                    case MIMIC::MimicSet::HIT_ANCHOR2:
                    case MIMIC::MimicSet::HIT_ANCHOR3:
                    case MIMIC::MimicSet::HIT_ANCHOR4:
                        _state = ANCHOR_SELECT;
                        if(!HasCapture())
                        {
                            CaptureMouse() ;
                            SetCursor(*wxCROSS_CURSOR);
                        }
                        break;
                    default:
                        _state = NONE;
                        _currentObject->setSelected(false);
                        _currentObject = nullptr;
                        break;
                    }
                }
                else
                {
                    if(_objects.hitPoint(_currentpoint, _currentObject->rect()) == MIMIC::MimicSet::HIT_OBJECT)
                    {
                        _state = OBJECT_SELECT;
                    }
                    else
                    {
                        _state = NONE;
                        _currentObject->setSelected(false);
                        _currentObject = nullptr;
                    }
                }
            }
        }

    }
    break;
    default:
    {

        if(_currentObject) _currentObject->setSelected(false);
        _currentObject = nullptr;
        //
        if(editMode())
        {
            if(_state == NONE)
            {
                int x,y,xx,yy ;
                event.GetPosition(&x,&y);
                CalcUnscrolledPosition( x, y, &xx, &yy );
                _currentpoint = wxPoint( xx, yy ) ;
                MIMIC::MIMICOBJECTPTR &o = _objects.hitTest(_currentpoint);
                if(o)
                {
                    _currentObject = o.get();
                    _currentObject->setSelected(true);
                    _rect = o->rect();
                    _state = OBJECT_SELECT;
                    _currentpoint = o->rect().GetTopLeft();
                    WarpPointer(_currentpoint.x,_currentpoint.y);
                    if(!HasCapture())
                    {
                        CaptureMouse() ;
                        SetCursor(*wxCROSS_CURSOR);
                    }
                    _startpoint = _currentpoint;
                }
            }
            else
            {
                _state = NONE;
            }
        }
        else
        {
            int x,y,xx,yy ;
            event.GetPosition(&x,&y);
            CalcUnscrolledPosition( x, y, &xx, &yy );
            _currentpoint = wxPoint( xx, yy ) ;
            MIMIC::MIMICOBJECTPTR &o = _objects.hitTest(_currentpoint);
            if(o)
            {
                o->onClick(this,_currentpoint,&_objects);
            }
        }
    }
    break;
    }
    Refresh();

}
void MimicCanvas::onLeftUp(wxMouseEvent& event)
{

    if(HasCapture())
    {
        ReleaseMouse();
        SetCursor(*wxSTANDARD_CURSOR);
    }
    if(editMode())
    {
        if (( _state != NONE) && _currentObject)
        {

            {
                wxClientDC dc( this );
                PrepareDC( dc );
                wxDCOverlay overlaydc( _overlay, &dc );
                overlaydc.Clear();
            }
            _overlay.Reset();
            int x,y,xx,yy ;
            event.GetPosition(&x,&y);
            CalcUnscrolledPosition( x, y, &xx, &yy );
            //
            _currentpoint = snapToGrid(wxPoint( xx, yy ));
            //
            if(_currentObject->canResize())
            {
                switch(_state)
                {
                case OBJECT_SELECT:
                    _currentObject->rect().SetPosition(_currentpoint);
                    break;
                case ANCHOR_SELECT:
                {
                    switch(_hitPoint)
                    {
                    case MIMIC::MimicSet::HIT_ANCHOR1:
                    case MIMIC::MimicSet::HIT_ANCHOR2:
                    case MIMIC::MimicSet::HIT_ANCHOR3:
                    case MIMIC::MimicSet::HIT_ANCHOR4:
                        _currentObject->rect() = _rect;
                        break;
                    default:
                        break;
                    }
                }
                break;
                default:
                    break;
                }
            }
            else
            {
                _currentObject->rect().SetPosition(_currentpoint);
            }
            _currentObject->toData(&_objects); // update tree
        }
    }
    Refresh();
    Update();
}
/*!
 * \brief MimicCanvas::onMotion
 * \param event
 */
void MimicCanvas::onMotion(wxMouseEvent& event)
{
    if(editMode() && event.Dragging())
    {
        if ( _state != NONE)
        {
            int x,y, xx, yy ;
            event.GetPosition(&x,&y);
            CalcUnscrolledPosition( x, y, &xx, &yy );
            _currentpoint = wxPoint( xx, yy ) ;
            wxClientDC dc( this ) ;
            PrepareDC( dc ) ;
            wxDCOverlay overlaydc( _overlay, &dc );
            overlaydc.Clear();
            //
            dc.SetPen( wxPen( *wxLIGHT_GREY, 2 ) );
            dc.SetBrush( *wxTRANSPARENT_BRUSH );
            //
            switch(_state)
            {
            case OBJECT_SELECT:
                _rect.SetPosition(_currentpoint);
                break;
            case ANCHOR_SELECT:
            {
                switch(_hitPoint)
                {
                case MIMIC::MimicSet::HIT_ANCHOR1:
                {
                    // move left and top
                    wxRect r(_currentpoint, _rect.GetBottomRight());
                    _rect = r;
                }
                break;
                case MIMIC::MimicSet::HIT_ANCHOR2:
                {
                    // move right and top
                    wxPoint tl(_rect.GetLeft(), _currentpoint.y);
                    wxPoint br(_currentpoint.x,_rect.GetBottom());
                    _rect = wxRect(tl,br);
                }
                break;
                case MIMIC::MimicSet::HIT_ANCHOR3:
                    _rect.SetRightBottom(_currentpoint);
                    break;
                case MIMIC::MimicSet::HIT_ANCHOR4:
                {
                    wxPoint tl(_currentpoint.x, _rect.GetTop());
                    wxPoint br(_rect.GetRight(),_currentpoint.y);
                    _rect = wxRect(tl,br);
                }
                break;
                default:
                    break;
                }
            }
            break;
            default:
                break;
            }
            dc.DrawRectangle( _rect );
        }
    }
}



void MimicCanvas::onPopupClick(wxCommandEvent &evt)
{
    if(_currentObject)
    {
        switch (evt.GetId()) {
        case wxID_EDIT:
            // Open properties for the node
            _currentObject->properties(this, _objects);
            Refresh();
            break;
        case wxID_DELETE:
            // delete the node
            _objects.removeObject(_currentObject->id());
            _currentObject = nullptr;
            _state = NONE;
            Refresh();
            break;
        case wxID_CANCEL:
            break;
        default:
            break;
        }
    }
}

/*!
 * \brief MimicCanvas::onRightDown
 * \param event
 */
void MimicCanvas::onRightDown(wxMouseEvent& event)
{
    if(editMode())
    {
        // context menu
        int x,y,xx,yy ;
        event.GetPosition(&x,&y);
        CalcUnscrolledPosition( x, y, &xx, &yy );
        _currentpoint = wxPoint( xx, yy ) ;
        // find object and put up menu
        MIMIC::MIMICOBJECTPTR &o = _objects.hitTest(_currentpoint);
        if(o)
        {
            _currentObject = o.get();
            // put up the menu
            wxMenu mnu;
            mnu.Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MimicCanvas::onPopupClick), NULL, this);
            mnu.Append(wxID_EDIT,  "Edit...");
            mnu.Append(wxID_DELETE,"Delete");
            mnu.Append(wxID_CANCEL,"Cancel");
            PopupMenu(&mnu);
        }
        else
        {
            _state = NONE;
            if(_currentObject)
            {
                _currentObject->setSelected(false);
                _currentObject = nullptr;
            }
        }
        Refresh();
    }
}

