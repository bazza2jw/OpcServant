#ifndef MIMICCONTROLS_H
#define MIMICCONTROLS_H
#include "mimicobject.h"
#include "mimicbasetypes.h"
#include "MimicUI.h"
#include "PropertiesEditorDialog.h"
namespace MIMIC
{
/*!
 * \brief The LedObject class
 */
class LedObject : public CircleObject
{
    bool _on = false; // boolean display
    wxColour _currentColour = *wxLIGHT_GREY;
public:
    LedObject(unsigned i = 0, unsigned t = 0) : CircleObject(i,t) {}
    void setOn(bool f) {
        _on = f;
        if(_on)
        {
            _currentColour = MimicObject::colour();
        }
        else
        {
            _currentColour = *wxLIGHT_GREY;
        }
    }
    bool canResize() const {
        return false;
    }

    //
    virtual wxColour & colour()
    {
        return _currentColour;
    }
    //
};

/*!
 * \brief The TrafficLightObject class
 * An icon with four lights showing a state and a value - aggreated data  point - can be the base for a SCADA value etc
 */
class TrafficLightObject : public IconObject
{
    unsigned _state = 0;
    bool _ackOn = false;
    static constexpr int lightSize = 5;
    std::string _text = "TextValue";
public:
    enum
    {
        Off = 0,
        Ok,
        Alert,
        Action
    };
    /*!
     * \brief TrafficLightObject
     * \param i
     * \param t
     */
    TrafficLightObject(unsigned i = 0, unsigned t = 0) : IconObject(i,t) {}
    void draw(wxDC &dc);

    bool canResize() const {
        return false;
    }

    void setState(unsigned v) {
        _state = v;
    }
    void setAckOn(bool f) {
        _ackOn = f;
    }
    void setText(const std::string &s) {
        _text = s;
    }
    /*!
     * \brief load
     * \param dlg
     * \param ns
     * \param p
     */
    virtual void load(PropertiesEditorDialog &dlg,MimicSet &ns,MRL::PropertyPath p)
    {
        IconObject::load(dlg,ns,p);
        dlg.loader().addIntProperty("State","State",ns.data().getValue<int>(p,"STATE"),0,3); // field[5]
        dlg.loader().addBoolProperty("Ack","Ack",ns.data().getValue<bool>(p,"ACK")); // field[6]
        dlg.loader().addStringProperty("Text","Text",_text); // field[7]

    }
    /*!
     * \brief save
     * \param dlg
     * \param ns
     * \param p
     */
    virtual void save(PropertiesEditorDialog &dlg,MimicSet &ns,MRL::PropertyPath p)
    {
        IconObject::save(dlg,ns,p);
        wxVariant v = dlg.loader().fields()[5]->GetValue();
        _state = v.GetInteger();
        ns.data().setValue(p,"STATE",_state);
        //
        v = dlg.loader().fields()[6]->GetValue();
        _ackOn = v.GetBool();
        ns.data().setValue(p,"ACK",_ackOn);
        v = dlg.loader().fields()[7]->GetValue();
        _text = v.GetString();
    }
    /*!
     * \brief fromData
     * \param set
     */
    virtual void fromData(MimicSet *set)
    {
        IconObject::fromData(set);
        MRL::PropertyPath p;
        toPath(p);
        _state = set->data().getValue<int>(p,"STATE");
        _ackOn = set->data().getValue<bool>(p,"ACK");
    }

};


/*!
 * \brief The LedDisplay class
 * A double value display
 */
class LedDisplayObject : public RectObject
{
    wxString _text = "00000.0";
    wxString _format = "%-6.1f";
    int _numberDp = 1;
    bool _nixieMode = false;
    static DigitalLEDBright * _ledImageList;
    static DigitalNixie * _nixieImageList;
    static std::vector<wxBitmap> _ledBitmap;
    static std::vector<wxBitmap> _nixieBitmap;

public:
    /*!
     * \brief LedDisplayObject
     * \param i
     * \param t
     */
    LedDisplayObject(unsigned i = 0, unsigned t = 0);
    /*!
     * \brief canResize
     * \return
     */
    bool canResize() const { return false; }
    /*!
     * \brief setValue
     * \param v
     */
    void setValue(double v)
    {
        _text = wxString::Format(_format,v);
        wxSize sz(_ledBitmap[0].GetSize().GetWidth() * _text.Length(), _ledBitmap[0].GetSize().GetHeight());
        rect().SetSize(sz);
    }
    /*!
     * \brief setFormat
     * \param s
     */
    void setFormat(const wxString &s) {
        _format = s;
    }
    /*!
     * \brief drawNumber
     * \param dc
     * \param l
     * \param p
     * \param c
     */
    void drawNumber(wxDC &dc, std::vector<wxBitmap> &l, wxPoint &p, char c)
    {
        wxBitmap b;
        if((c >= '0') && (c <= '9'))
        {
            b = l.at(c - '0');
        }
        else if(c == '-')
        {
            b = l.at(11);
        }
        else if(c == '.')
        {
            b = l.at(12);
        }
        else
        {
            b = l.at(10);
        }
        dc.DrawBitmap(b,p);
        p.x += b.GetSize().GetWidth();
    }
    /*!
     * \brief draw
     * \param dc
     */
    void draw(wxDC &dc)
    {
        wxPoint pt = rect().GetTopLeft(); // where we draw at
        for(int i = 0; i < _text.Length(); i++ )
        {
            char c;
            _text.at(i).GetAsChar(&c);
            if(_nixieMode)
            {
                drawNumber(dc,_nixieBitmap,pt,c);
            }
            else
            {
                drawNumber(dc,_ledBitmap,pt,c);
            }
        }
    }
    /*!
     * \brief load
     * \param dlg
     * \param ns
     * \param p
     */
    virtual void load(PropertiesEditorDialog &dlg,MimicSet &ns,MRL::PropertyPath p)
    {
        RectObject::load(dlg,ns,p);
        dlg.loader().addStringProperty("Text","Text",ns.data().getValue<std::string>(p,"TEXT")); // field[4]
        dlg.loader().addStringProperty("Format","Format",ns.data().getValue<std::string>(p,"FORMAT")); // field[5]
        dlg.loader().addBoolProperty("Use Nixie","Nixie",ns.data().getValue<bool>(p,"NIXIE")); // field[6]
    }
    /*!
     * \brief save
     * \param dlg
     * \param ns
     * \param p
     */
    virtual void save(PropertiesEditorDialog &dlg,MimicSet &ns,MRL::PropertyPath p)
    {
        RectObject::save(dlg,ns,p);
        wxVariant v = dlg.loader().fields()[4]->GetValue();
        _text = v.GetString();
        if(_text.IsEmpty()) _text = "    0.0";
        v = dlg.loader().fields()[5]->GetValue();
        _format = v.GetString();
        v = dlg.loader().fields()[6]->GetValue();
        _nixieMode = v.GetBool();
        //
        ns.data().setValue(p,"TEXT",_text.ToStdString());
        ns.data().setValue(p,"FORMAT",_format.ToStdString());
        ns.data().setValue(p,"NIXIE",_nixieMode);
        //
    }
    /*!
     * \brief toData
     * \param set
     */
    virtual void toData(MimicSet *set)
    {
        RectObject::toData(set);
        MRL::PropertyPath p;
        toPath(p);
        set->data().setValue(p,"TEXT",_text.ToStdString());
        set->data().setValue(p,"FORMAT",_format.ToStdString());
        set->data().setValue(p,"NIXIE",_nixieMode);
    }
    /*!
     * \brief fromData
     * \param set
     */
    virtual void fromData(MimicSet *set)
    {
        RectObject::fromData(set);
        MRL::PropertyPath p;
        toPath(p);
        _text = wxString(set->data().getValue<std::string>(p,"TEXT"));
        _format = wxString(set->data().getValue<std::string>(p,"FORMAT"));
        _nixieMode = set->data().getValue<bool>(p,"NIXIE");
    }

};




}



#endif // MIMICCONTROLS_H
