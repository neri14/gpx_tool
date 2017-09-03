#ifndef PRINTER_H
#define PRINTER_H

#include <iostream>

#define C_RST "\x1B[0m"
#define C_RED "\x1B[31m"
#define C_YEL "\x1B[33m"
#define C_GRN "\x1B[32m"
#define C_BLU "\x1B[34m"

#define F_RED(x) C_RED x C_RST
#define F_YEL(x) C_YEL x C_RST
#define F_GRN(x) C_GRN x C_RST
#define F_BLU(x) C_BLU x C_RST

#define LOG_ERROR_STREAM     std::cerr << F_RED("ERR") << ": "
#define LOG_WARNING_STREAM   std::cerr << F_YEL("WRN") << ": "
#define LOG_INFO_STREAM      std::cout << F_GRN("INF") << ": "
#define LOG_DEBUG_STREAM     std::cout << F_BLU("DBG") << ": "

#define LOG_ERROR(log)     LOG_ERROR_STREAM << log << std::endl
#define LOG_WARNING(log)   LOG_WARNING_STREAM << log << std::endl
#define LOG_INFO(log)      LOG_INFO_STREAM << log << std::endl
#define LOG_DEBUG(log)     LOG_DEBUG_STREAM << log << std::endl

#endif /* PRINTER_H */