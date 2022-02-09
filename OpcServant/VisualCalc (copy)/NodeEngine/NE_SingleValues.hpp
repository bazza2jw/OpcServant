#ifndef NE_SINGLEVALUES_HPP
#define NE_SINGLEVALUES_HPP

#include "NE_GenericValue.hpp"
#include "NE_Serializable.hpp"

namespace NE
{



class NumberValue
{
public:
    NumberValue ();
    virtual ~NumberValue ();

    virtual int         ToInteger () const = 0;
    virtual float       ToFloat () const = 0;
    virtual double      ToDouble () const = 0;
    virtual bool        ToBool() const = 0;
    virtual unsigned	ToUnsigned () const = 0;


    static int		ToInteger (const ValueConstPtr& val);
    static int		ToInteger (const ValuePtr& val);
    static int		ToInteger (Value* val);
    static float	ToFloat (const ValueConstPtr& val);
    static float	ToFloat (const ValuePtr& val);
    static float	ToFloat (Value* val);
    static double	ToDouble (const ValueConstPtr& val);
    static double	ToDouble (const ValuePtr& val);
    static double	ToDouble (Value* val);
    static unsigned		ToUnsigned (const ValueConstPtr& val);
    static unsigned		ToUnsigned (const ValuePtr& val);
    static unsigned		ToUnsigned (Value* val);
    static bool		ToBool (const ValueConstPtr& val);
    static bool		ToBool (const ValuePtr& val);
    static bool		ToBool (Value* val);


};


class BooleanValue : public NumberValue,public GenericValue<bool>
{
    DYNAMIC_SERIALIZABLE (BooleanValue);
public:
    BooleanValue ();
    BooleanValue (bool val);
    virtual ~BooleanValue ();
    virtual ValuePtr		Clone () const override;
    virtual std::wstring	ToString (const StringConverter& stringConverter) const override;
    virtual bool            ToBool() const override {
        return GetValue();
    }
    virtual int		ToInteger () const {
        return GetValue()?1:0;
    }
    virtual float	ToFloat () const  {
        return GetValue()?1:0;
    }
    virtual double	ToDouble () const  {
        return GetValue()?1:0;
    }
    virtual unsigned ToUnsigned () const {
        return GetValue()?1:0;
    }

    virtual Stream::Status	Read (InputStream& inputStream) override;
    virtual Stream::Status	Write (OutputStream& outputStream) const override;
};

class IntValue : public NumberValue,
    public GenericValue<int>
{
    DYNAMIC_SERIALIZABLE (IntValue);

public:
    IntValue ();
    IntValue (int val);
    virtual ~IntValue ();

    virtual ValuePtr		Clone () const override;
    virtual std::wstring	ToString (const StringConverter& stringConverter) const override;
    virtual int				ToInteger () const override;
    virtual float			ToFloat () const override;
    virtual double			ToDouble () const override;
    virtual bool            ToBool() const override {
        return GetValue() != 0;
    }
    virtual unsigned ToUnsigned () const {
        union
        {
            int i;
            unsigned u;
        };
        i =  GetValue();
        return u;
    }

    virtual Stream::Status	Read (InputStream& inputStream) override;
    virtual Stream::Status	Write (OutputStream& outputStream) const override;
};


class UnsignedValue : public NumberValue,
        public GenericValue<unsigned>
{
    DYNAMIC_SERIALIZABLE (UnsignedValue);

public:
    UnsignedValue () : UnsignedValue(0) {}
    UnsignedValue (int val) :
        NumberValue (),
        GenericValue<unsigned> (val)
    {

    }
    virtual ~UnsignedValue (){}

    virtual ValuePtr		Clone () const override
    {
        return std::make_shared<UnsignedValue> (val);
    }

    virtual std::wstring	ToString (const StringConverter& stringConverter) const override
    {
        return std::to_wstring (val);
    }

    virtual int				ToInteger () const override
    {
        union
        {
            int i;
            unsigned u;
        };
        u =  GetValue();
        return i;
    }
    virtual float	ToFloat () const override
    {
        return float(ToInteger());
    }
    virtual double			ToDouble () const override
    {
        return double(ToInteger());
    }
    virtual bool            ToBool() const override {
        return GetValue() != 0;
    }
    virtual unsigned ToUnsigned () const override{
        return  GetValue();
    }

    virtual Stream::Status	Read (InputStream& inputStream) override
    {
        ObjectHeader header (inputStream);
        SingleValue::Read (inputStream);
        inputStream.Read (val);
        return inputStream.GetStatus ();
    }
    virtual Stream::Status	Write (OutputStream& outputStream) const override
    {
        ObjectHeader header (outputStream, serializationInfo);
        SingleValue::Write (outputStream);
        outputStream.Write (val);
        return outputStream.GetStatus ();

    }
};


class FloatValue :	public NumberValue,
    public GenericValue<float>
{
    DYNAMIC_SERIALIZABLE (FloatValue);

public:
    FloatValue ();
    FloatValue (float val);
    virtual ~FloatValue ();

    virtual ValuePtr		Clone () const override;
    virtual std::wstring	ToString (const StringConverter& stringConverter) const override;

    virtual int				ToInteger () const override;
    virtual float			ToFloat () const override;
    virtual double			ToDouble () const override;
    virtual bool            ToBool() const override {
        return GetValue() != 0;
    }
    virtual unsigned ToUnsigned () const override{
        return  unsigned(GetValue());
    }

    virtual Stream::Status	Read (InputStream& inputStream) override;
    virtual Stream::Status	Write (OutputStream& outputStream) const override;
};

class DoubleValue : public NumberValue,
    public GenericValue<double>
{
    DYNAMIC_SERIALIZABLE (DoubleValue);

public:
    DoubleValue ();
    DoubleValue (double val);
    virtual ~DoubleValue ();

    virtual ValuePtr		Clone () const override;
    virtual std::wstring	ToString (const StringConverter& stringConverter) const override;

    virtual int				ToInteger () const override;
    virtual float			ToFloat () const override;
    virtual double			ToDouble () const override;

    virtual Stream::Status	Read (InputStream& inputStream) override;
    virtual Stream::Status	Write (OutputStream& outputStream) const override;
    virtual bool            ToBool() const override {
        return GetValue() != 0;
    }
    virtual unsigned ToUnsigned () const override{
        return  unsigned(GetValue());
    }


};

class StringValue : public GenericValue<std::wstring>
{
    DYNAMIC_SERIALIZABLE (StringValue);

public:
    StringValue ();
    StringValue (const std::wstring& val);
    virtual ~StringValue ();

    virtual ValuePtr		Clone () const override;
    virtual std::wstring	ToString (const StringConverter& stringConverter) const override;

    virtual Stream::Status	Read (InputStream& inputStream) override;
    virtual Stream::Status	Write (OutputStream& outputStream) const override;
    virtual bool    ToBool() const {

        return GetValue() == L"true";
    }

};

}

#endif
