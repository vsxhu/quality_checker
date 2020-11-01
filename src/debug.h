#ifndef LEETCODE_DEBUG_H
#define LEETCODE_DEBUG_H
//#define DEBUGMSG
//#define DEBUG

#ifdef DEBUGMSG
#define dmesg(argc,argv...) ({printf("[DEBUG]:File:%s:  Func:%s:  Line:%05d\n",__FILE__,__func__,__LINE__);printf(argc,##argv); })
#define emesg(argc,argv...) ({printf("[ERROR]:File:%s:  Func:%s:  Line:%05d\n",__FILE__,__func__,__LINE__);printf(argc,##argv); })
#define imesg(argc,argv...) ({printf("[INFO]:File:%s:  Func:%s:  Line:%05d\n",__FILE__,__func__,__LINE__);printf(argc,##argv); })
#define wmesg(argc,argv...) ({printf("[WARNING]:File:%s:  Func:%s:  Line:%05d\n",__FILE__,__func__,__LINE__);printf(argc,##argv); })
#else
#define dmesg(argc,argv...)
#define emesg(argc,argv...)
#define imesg(argc,argv...)
#define wmesg(argc,argv...)
#endif

#endif
