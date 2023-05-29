// Generated Code - DO NOT EDIT !!
// generated by 'emugen'
#ifndef __renderControl_server_context_t_h
#define __renderControl_server_context_t_h

#include "renderControl_server_proc.h"

#include "renderControl_types.h"


struct renderControl_server_context_t {

	rcGetRendererVersion_server_proc_t rcGetRendererVersion;
	rcGetEGLVersion_server_proc_t rcGetEGLVersion;
	rcQueryEGLString_server_proc_t rcQueryEGLString;
	rcGetGLString_server_proc_t rcGetGLString;
	rcGetNumConfigs_server_proc_t rcGetNumConfigs;
	rcGetConfigs_server_proc_t rcGetConfigs;
	rcChooseConfig_server_proc_t rcChooseConfig;
	rcGetFBParam_server_proc_t rcGetFBParam;
	rcCreateContext_server_proc_t rcCreateContext;
	rcDestroyContext_server_proc_t rcDestroyContext;
	rcCreateWindowSurface_server_proc_t rcCreateWindowSurface;
	rcDestroyWindowSurface_server_proc_t rcDestroyWindowSurface;
	rcCreateColorBuffer_server_proc_t rcCreateColorBuffer;
	rcOpenColorBuffer_server_proc_t rcOpenColorBuffer;
	rcCloseColorBuffer_server_proc_t rcCloseColorBuffer;
	rcSetWindowColorBuffer_server_proc_t rcSetWindowColorBuffer;
	rcFlushWindowColorBuffer_server_proc_t rcFlushWindowColorBuffer;
	rcMakeCurrent_server_proc_t rcMakeCurrent;
	rcFBPost_server_proc_t rcFBPost;
	rcFBSetSwapInterval_server_proc_t rcFBSetSwapInterval;
	rcBindTexture_server_proc_t rcBindTexture;
	rcBindRenderbuffer_server_proc_t rcBindRenderbuffer;
	rcColorBufferCacheFlush_server_proc_t rcColorBufferCacheFlush;
	rcReadColorBuffer_server_proc_t rcReadColorBuffer;
	rcUpdateColorBuffer_server_proc_t rcUpdateColorBuffer;
	rcOpenColorBuffer2_server_proc_t rcOpenColorBuffer2;
	rcCreateClientImage_server_proc_t rcCreateClientImage;
	rcDestroyClientImage_server_proc_t rcDestroyClientImage;
	rcSelectChecksumHelper_server_proc_t rcSelectChecksumHelper;
	rcCreateSyncKHR_server_proc_t rcCreateSyncKHR;
	rcClientWaitSyncKHR_server_proc_t rcClientWaitSyncKHR;
	rcFlushWindowColorBufferAsync_server_proc_t rcFlushWindowColorBufferAsync;
	rcDestroySyncKHR_server_proc_t rcDestroySyncKHR;
	rcSetPuid_server_proc_t rcSetPuid;
	rcUpdateColorBufferDMA_server_proc_t rcUpdateColorBufferDMA;
	rcCreateColorBufferDMA_server_proc_t rcCreateColorBufferDMA;
	rcWaitSyncKHR_server_proc_t rcWaitSyncKHR;
	virtual ~renderControl_server_context_t() {}
	int initDispatchByName( void *(*getProc)(const char *name, void *userData), void *userData);
};

#endif
