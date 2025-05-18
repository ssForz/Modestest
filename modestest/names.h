#ifndef _NAMODESTE_
#define _NAMODESTE_

#define GET_NAME_STRING(name) #name

#define GET_NAME(name) GET_NAME_STRING(name##_test)

#define SMOKE(name) GET_NAME(name##_smoke)

#define MEDIUM(name) GET_NAME(name##_medium)

#define HEAVY(name) GET_NAME(name##_heavy)

#define CRITICAL(name) GET_NAME(name##_critical)

#define CUSTOM(prefix, name) GET_NAME(name##_##prefix)

#endif