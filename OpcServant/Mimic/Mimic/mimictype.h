#ifndef MIMICTYPE_H
#define MIMICTYPE_H
#include <MrlLib/mrllib.h>
#include <Mimic/Mimic/mimicobject.h>
namespace MIMIC
{
    /*!
     * \brief The MimicType class
     * factory for MimicObjects
     */
    class MimicType
    {
        std::string _name;
        unsigned _id = 0;
        static std::map<std::string, MimicType *> _map;
        static std::map<unsigned, MimicType *> _mapId;
    public:
        /*!
         * \brief MimicType
         * \param s
         */
        MimicType(const std::string &s) : _name(s)
        {
            _map[s] = this;
            std::hash<std::string> hash_obj;
            _id = unsigned(hash_obj(_name)); // determine the hash to get unique id, probably
            _mapId[_id] = this;
        }
        /*!
         * \brief ~MimicType
         */
        virtual ~MimicType()
        {
            _map.erase(_name);
            _mapId.erase(_id);
        }
        //
        unsigned id() const { return _id;}
        //
        static std::map<std::string, MimicType *> & map() { return _map;}
        static std::map<unsigned, MimicType *> & mapId() { return _mapId;}
        //
        virtual const char * typeClass() const { return "Basic";}
        /*!
         * \brief create
         * \param i
         * \return
         */
        virtual MimicObject * create(unsigned i)
        {
            return new MimicObject(i);
        }
        /*!
         * \brief find
         * \param s
         * \return
         */
        static MimicType * find(const std::string s)
        {
            auto i = _map.find(s);
            if(i != _map.end()) return i->second;
            return nullptr;
        }
        /*!
         * \brief find
         * \param j
         * \return
         */
        static MimicType * find(unsigned j)
        {
            auto i = _mapId.find(j);
            if(i != _mapId.end()) return i->second;
            return nullptr;
        }

        const std::string & name() const { return _name;}
    };

    template <typename T>
    /*!
     * \brief The MimicObjectType class
     */
    class MimicObjectType : public MimicType
    {
    public:
        MimicObjectType(const std::string &s) : MimicType(s) {}
        MimicObject * create(unsigned i) { return new T(i,id());}
    };

}
#endif // MIMICTYPE_H
