#ifndef HTTPSENDOBJECTMANAGER_H
#define HTTPSENDOBJECTMANAGER_H

#include <Common/objectmanager.h>
namespace MRL {
    class HttpSendObjectManager : public ObjectManager {
        public:
            HttpSendObjectManager();

            virtual bool createRT(const PropertyPath &path);
            // Configuration
            virtual bool create(const PropertyPath &path);
            //
            // Variants for GUI and Web
            virtual bool edit(wxWindow *w, const PropertyPath &s);
            virtual bool edit(Wt::WWidget *w, const PropertyPath &s);
            //
            virtual bool view(wxWindow *parent, const PropertyPath &s);
            virtual bool view(Wt::WWidget *parent, const PropertyPath &s);
            //
            virtual bool update(const PropertyPath &s);  // updated as an object so includes all children
            virtual bool canBeChildOf(const std::string &type) {
                return (type == "Publishers");
            }
            virtual bool createRtObject(unsigned, RtObjectRef &);
            virtual bool canRun() {
                return true; //!< is the object an active runtime object ?
            }
    };
}


#endif // HTTPSENDOBJECTMANAGER_H
