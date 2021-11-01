#ifndef WEBDATAMODEL_H
#define WEBDATAMODEL_H

#include <Wt/WStandardItem.h>
#include <Wt/WStandardItemModel.h>
#include <Wt/WTreeView.h>

namespace MRL {
    /*!
        \brief The WebDataModel class
        Model that handles the navigation tree
    */
    class WebDataModel : public Wt::WStandardItemModel {
        public:
            enum {
                // the columns
                Item = 0, Value,Type, Status,  NumberColumns
            };
        public:
            WebDataModel() :  Wt::WStandardItemModel(0, Value + 1) {
                setHeaderData(Item, Wt::Orientation::Horizontal, std::string("Item"));
                setHeaderData(Value, Wt::Orientation::Horizontal, std::string("Value"));
            }

    };
    typedef std::shared_ptr<WebDataModel> WebDataModelRef;
}
#endif // WEBDATAMODEL_H
