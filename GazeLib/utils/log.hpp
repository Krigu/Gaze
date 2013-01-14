#ifndef LOG_HPP_
#define LOG_HPP_

#include <iostream>

/// adds a macro "LOG_D('your message') and LOG_W('your message')"
/// the log_d is only printed if you add the __DEBUG Symbol to eclipses "Symbol and Paths"
#define LOG_W(msg)(std::cout << __FILE__ << "(" << __LINE__  << "): "<< msg << std::endl)
#if __DEBUG == 1
#define LOG_D(msg) LOG_W(msg)
#else
#define LOG_D(msg) // DO NOTHING
#endif

#endif /* LOG_HPP_ */
