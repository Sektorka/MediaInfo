#ifndef MI_H
#define MI_H

#define _UNICODE 1

#ifdef MI_STATIC
    //#include <MediaInfo/MediaInfo.h>
    //using namespace MediaInfoLib;
    #include <MediaInfo/MediaInfoDLL.h>
    using namespace MediaInfoDLL;
#else
    #include <MediaInfo/MediaInfoDLL.h>
    using namespace MediaInfoDLL;
#endif

#endif // MI_H
