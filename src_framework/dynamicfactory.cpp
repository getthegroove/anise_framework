#include "dynamicfactory.h"
#include <dlfcn.h>
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

void CDynamicFactory::loadLibraries(QString folder)
{
    // Look in the given dictionary.
    QDir dir(folder);
    // Search only for files.
    dir.setFilter(QDir::Files);
    // Only list files ending with .so
    QStringList filter;
    filter << "*.so";
    dir.setNameFilters(filter);

    QStringList list = dir.entryList();

    for(auto it = list.begin(); it != list.end(); ++it) {
        QString filename;
        filename = folder + "/" + (*it).toLocal8Bit().constData();
        // Open the library file.
        void *handle = dlopen(filename.toLocal8Bit().constData(), RTLD_NOW);
        if(handle == NULL) {
            qDebug() << "CDynamicFactory::loadLibraries() Error:" << endl
                     << "There was an error loading the library '"
                     << (*it) << "'." << dlerror() << endl;
            continue;
        }

        // Let the base class choose what to do with the library.
        addLibrary(handle, filename);
    }
}


//------------------------------------------------------------------------------
// Private Functions
