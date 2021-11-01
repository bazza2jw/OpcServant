#include "validators.h"

/*!
 * \brief MRL::Validators::validAlphaNumericCharacters
 * \return
 */
wxString * MRL::Validators::validAlphaNumericCharacters() // for names
{
    static wxString s("01234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_");
    return &s;
}

/*!
 * \brief MRL::Validators::validIPCharacters
 * \return
 */
wxString * MRL::Validators::validIPCharacters() // IP address charcaters
{
    static wxString s("01234567890.");
    return &s;
}

/*!
 * \brief MRL::Validators::validNumberCharacters
 * \return
 */
wxString * MRL::Validators::validNumberCharacters() // characters for numbers
{
    static wxString s("01234567890.-");
    return &s;
}

/*!
 * \brief MRL::Validators::validHexCharacters
 * \return
 */
wxString * MRL::Validators::validHexCharacters() // characters for Hex numbers
{
    static wxString s("01234567890ABCDEF");
    return &s;
}

/*!
 * \brief MRL::Validators::AlphaNumeric
 * \return
 */
wxTextValidator &  MRL::Validators::AlphaNumeric()
{
    static wxTextValidator v(wxFILTER_INCLUDE_CHAR_LIST);
    if(v.GetIncludes().Count() < 1) v.SetCharIncludes(*validAlphaNumericCharacters());

    return v;
}

/*!
 * \brief MRL::Validators::IP
 * \return
 */
wxTextValidator &MRL::Validators::IP()
{
    static wxTextValidator v(wxFILTER_INCLUDE_CHAR_LIST);
    if(v.GetIncludes().Count() < 1) v.SetCharIncludes(*validIPCharacters());
    return v;

}

/*!
 * \brief MRL::Validators::Numbers
 * \return
 */
wxTextValidator &  MRL::Validators::Numbers()
{
    static wxTextValidator v(wxFILTER_NUMERIC);

    return v;

}

/*!
 * \brief MRL::Validators::Hex
 * \return
 */
wxTextValidator &MRL::Validators::Hex()
{
    static wxTextValidator v(wxFILTER_INCLUDE_CHAR_LIST);
    if(v.GetIncludes().Count() < 1) v.SetCharIncludes(*validHexCharacters());
    return v;
}
