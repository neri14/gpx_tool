#ifndef NOT_IMPLEMETNED_H
#define NOT_IMPLEMETNED_H

#include <helpers/printer.h>
#include <cstring>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define NOT_IMPLEMENTED LOG_ERROR_STREAM \
    << __func__ << " in file " << __FILENAME__ \
    << " not implemented yet" << std::endl;

#endif /* NOT_IMPLEMETNED_H */