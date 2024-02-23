# ifndef LOGGER_H
# define LOGGER_H

# define DEBUG              "\033[36mDEBUG\033[0m"
# define INFO               "\033[34mINFO\033[0m"
# define WARNING            "\033[33mWARNING\033[0m"
# define ERROR              "\033[31mERROR\033[0m"
# define FATAL              "\033[31mFATAL\033[0m"
# define LOGGER(level,formatter,...)  printf("[%s:%d]-[%s] - " formatter "\n",__FILE__,__LINE__,level,## __VA_ARGS__ )
# endif