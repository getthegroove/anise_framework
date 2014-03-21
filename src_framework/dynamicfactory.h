#ifndef DYNAMICFACTORY_H
#define DYNAMICFACTORY_H

#include <dlfcn.h>
#include <QString>

class CDynamicFactory
{
  public:
    void loadLibraries(QString folder, QString filter, int flags);

  protected:
    // Process a library by its handler. Also receive the file that was used
    // ... to load the library.
    virtual void addLibrary(void *library_handle, QString filename) = 0;

  protected:
    // This is a singleton.
    explicit CDynamicFactory();
};

#endif // DYNAMICFACTORY_H
