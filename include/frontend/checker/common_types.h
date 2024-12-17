#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H
#include <string.h>

#define ISTYPE(typedf, tname) (strcmp(typedf->name, tname) == 0)

#define ISINT(typedf) ISTYPE(typedf, "int")
#define ISFLOAT(typedf) ISTYPE(typedf, "decimal")
#define ISSTRING(typedf) ISTYPE(typedf, "string")


#endif //COMMON_TYPES_H
