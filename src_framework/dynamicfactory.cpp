#include "dynamicfactory.h"
#include <QDir>
#include <QStringList>
#include <QDebug>

//------------------------------------------------------------------------------
// Constructor and Destructor

CDynamicFactory::CDynamicFactory()
{

}


//------------------------------------------------------------------------------
// Public Functions

void CDynamicFactory::loadLibraries(QString folder, QString filter, int flags)
{
    // Look in the given dictionary.
    QDir dir(folder);
    // Search only for files.
    dir.setFilter(QDir::Files);
    // Only list files ending with .so
    QStringList filterlist;
    filterlist << filter;
    dir.setNameFilters(filterlist);

    QStringList list = dir.entryList();

    for(auto it = list.begin(); it != list.end(); ++it) {
        QString filename;
        filename = folder + "/" + (*it).toLocal8Bit().constData();
        // Open the library file.
        void *handle = dlopen(filename.toLocal8Bit().constData(), flags);
        if(handle == NULL) {
            qCritical() << "CDynamicFactory::loadLibraries()" << endl
                     << "There was an error loading the library '"
                     << (*it) << "'." << dlerror();
            continue;
        }

        // Let the base class choose what to do with the library.
        addLibrary(handle, filename);
    }
}


//------------------------------------------------------------------------------
// Private Functions
