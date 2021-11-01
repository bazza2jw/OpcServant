#ifndef __ext_type__
#define __ext_type__

#ifdef WIN32
// ќбъ€вить стандартные целые типы, если Win
#define     int8_t      __int8
#define     int16_t     __int16
#define     int32_t     __int32
#define     int64_t     __int64
#define     uint8_t     unsigned __int8
#define     uint16_t    unsigned __int16
#define     uint32_t    unsigned __int32
#define     uint64_t    unsigned __int64
#endif // WIN32

#endif //__ext_type__
