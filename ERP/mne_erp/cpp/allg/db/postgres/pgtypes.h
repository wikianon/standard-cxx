#ifndef pgtypes_mne
#define pgtypes_mne

#if defined(__MINGW32__) || defined(__CYGWIN__) || defined(Darwin)
#define BOOLOID         16
#define BYTEAOID        17
#define CHAROID         18
#define NAMEOID         19
#define INT8OID         20
#define INT2OID         21
#define INT2VECTOROID   22
#define INT4OID         23
#define REGPROCOID      24
#define TEXTOID         25
#define OIDOID          26
#define TIDOID      27
#define XIDOID 28
#define CIDOID 29
#define OIDVECTOROID    30
#define PG_TYPE_RELTYPE_OID 71
#define PG_ATTRIBUTE_RELTYPE_OID 75
#define PG_PROC_RELTYPE_OID 81
#define PG_CLASS_RELTYPE_OID 83
#define XMLOID 142
#define POINTOID        600
#define LSEGOID         601
#define PATHOID         602
#define BOXOID          603
#define POLYGONOID      604
#define LINEOID         628
#define FLOAT4OID 700
#define FLOAT8OID 701
#define ABSTIMEOID      702
#define RELTIMEOID      703
#define TINTERVALOID    704
#define UNKNOWNOID      705
#define CIRCLEOID       718
#define CASHOID 790
#define MACADDROID 829
#define INETOID 869
#define CIDROID 650
#define INT4ARRAYOID        1007
#define TEXTARRAYOID        1009
#define FLOAT4ARRAYOID 1021
#define ACLITEMOID      1033
#define CSTRINGARRAYOID     1263
#define BPCHAROID       1042
#define VARCHAROID      1043
#define DATEOID         1082
#define TIMEOID         1083
#define TIMESTAMPOID    1114
#define TIMESTAMPTZOID  1184
#define INTERVALOID     1186
#define TIMETZOID       1266
#define BITOID   1560
#define VARBITOID     1562
#define NUMERICOID      1700
#define REFCURSOROID    1790
#define REGPROCEDUREOID 2202
#define REGOPEROID      2203
#define REGOPERATOROID  2204
#define REGCLASSOID     2205
#define REGTYPEOID      2206
#define REGTYPEARRAYOID 2211
#define TSVECTOROID     3614
#define GTSVECTOROID    3642
#define TSQUERYOID      3615
#define REGCONFIGOID    3734
#define REGDICTIONARYOID    3769
#define RECORDOID       2249
#define RECORDARRAYOID  2287
#define CSTRINGOID      2275
#define ANYOID          2276
#define ANYARRAYOID     2277
#define VOIDOID         2278
#define TRIGGEROID      2279
#define LANGUAGE_HANDLEROID     2280
#define INTERNALOID     2281
#define OPAQUEOID       2282
#define ANYELEMENTOID   2283
#define ANYNONARRAYOID  2776
#define ANYENUMOID      3500
#else
#define PG_PORT_H
#include <c.h>
#include <catalog/pg_type.h>
#endif

#define ACLITEMARRAYOID 1034

#endif /* pgtypes_mne */

