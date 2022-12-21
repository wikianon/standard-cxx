#ifndef _MDR_EXPORT_H_
#define _MDR_EXPORT_H_

#ifdef JMDR_EXPORTS
# define JMDR_API __declspec(dllexport)
#else
# define JMDR_API __declspec(dllimport)
#endif

#endif