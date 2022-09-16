#ifndef TEXTSPIN_H
#define TEXTSPIN_H
#include "VirtualKeypadUI.h"


#include <wx/arrstr.h>
/*!
 * \brief The TextSpin class
 */

class TextSpin : public TextSpinBase
{
   wxArrayString _items;
   int _current = 0;
public:
    TextSpin(wxWindow* parent, wxWindowID id = wxID_ANY);
    virtual ~TextSpin();
    wxArrayString &items() { return _items;}

    void setItems(wxArrayString &a)
    {
        _items = a;
        _current = 0;
    }

    void SetValue(int i)
    {
        if(i < 0) i = 0;
        if(i >= _items.GetCount()) i = _items.GetCount() - 1;
        _current = i;
        setCurrent();
    }

    void SetValue(const wxString &v)
    {
        int i = _items.Index(v);
        if(i >= 0)
        {
            _current = i;
            setCurrent();
        }
    }

    int GetValue() const
    {
        return _current;
    }



protected:
    void setCurrent()
    {
        if(_items.GetCount() > 0)
        {
            GetText()->SetValue(_items[_current]);
        }
        else
        {
            GetText()->Clear();
        }
    }

    virtual void onDown(wxCommandEvent& event)
    {
        _current--;
        if(_current < 0)
        {
            _current = _items.GetCount() - 1;
        }
        setCurrent();
    }
    virtual void onUp(wxCommandEvent& event)
    {
        _current++;
        if(_current >= _items.GetCount()) _current = 0;
        setCurrent();
    }
};

class NumberSpin : public TextSpinBase
{
    int _current = 0;
    int _minimum = 0;
    int _maximum = 10;
    wxString _format = "%d";

    public:
    NumberSpin(wxWindow* parent, wxWindowID id = wxID_ANY): TextSpinBase(parent,id) {}
protected:
    void setCurrent()
    {
        GetText()->SetValue(wxString::Format(_format,_current));
    }

    virtual void onDown(wxCommandEvent& event)
    {
        _current--;
        if(_current < _minimum)
        {
            _current = _maximum;
        }
        setCurrent();
    }
    virtual void onUp(wxCommandEvent& event)
    {
        _current++;
        if(_current >= _maximum) _current = _minimum;
        setCurrent();
    }

    int GetValue() const { return _current;}
    void SetValue(int i){ _current = i; setCurrent();}
    //
    int minimum() const { return _minimum;}
    void setMinimum(int i) { _minimum = i;}
    void setMaximum(int i) {_maximum = i;}
    int maximum() { return _maximum;}
    //
    void setFormat(const wxString &f) { _format = f;}
    //
};



#endif // TEXTSPIN_H
