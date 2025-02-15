// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A6EF1E93_33EB_476A_B556_185C5C5C1935__INCLUDED_)
#define AFX_STDAFX_H__A6EF1E93_33EB_476A_B556_185C5C5C1935__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//////////////////////////////////

#include <afxwin.h>
#include <windows.h>	// Windows的头文件
#include <mmsystem.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <assert.h>
#include <gl\gl.h>		// OpenGL32库的头文件
#include <gl\glu.h>		// GLu32库的头文件
#include <gl\glaux.h>	// GLaux库的头文件

#define FRAND   (((float)rand()-(float)rand())/RAND_MAX)
//////////////////////////////////


#include <windowsx.h>

//采集卡头文件
#include "QCAP.h"
#include "DeviceControl.h"

//	#include "BtnST.h"
class CChannelWnd;
extern bool SnapOk;
extern RECT			 g_rcShowWin ;

extern	DWORD dwCard ;


//保存路径
extern CString RootPath;     //文件根目录

extern CString PicSavePath;    //文件1级目录，即每次登录的日期目录
extern CString RecSavePath;

extern CString PicPath;        //文件2级目录，即日期目下的两个Pic和Video目录
extern CString VideoPath;

extern CString strIniFile;

//保存登录信息
extern CString customsUint;
extern CString address;
extern CString name;
extern int nComboUintIndex;

//串口通信模式(方向)
	#define PASSIVE_REPLY
//	#define ACTIVE_SEND

//功能模块控制宏
	#define USE_SDK               //使用采集卡
//	#define USE_PORT        //使用串口功能
	//	#define PORT_DEBUG     //串口调试
	#define WNDTOPMOST     //总在最前面
	//	#define INITVLC        //初始化vlc
	//	#define USE_3DS_COMPASS  //显示罗盘
	#define USE_SPLASHSCREEN  
	//	#define OPEN_DEV_DEBUG         //打开关闭设备资源调试

//局部功能控制宏

//#define  USE_CHANGE_CHANNEL    //使用改变通道功能
//	#define  USE_INFRARED         //使用红外指示
//#define   SHOW_LEFT_RIGHT_POWER  //显示本体电量
//#define   SHOW_AUTOMODE           //显示自动模式




#define FREE( p ) { if( p ) { free( p ); p = NULL; } }	

// SET INITIAL PARAMETER VALUES
//

#define	SC_DEFAULT_STANDARD                 0		// 0 = NTSC / 1 = PALB

#define SC_DEFAULT_INPUT_SUPPORT			0x00000000	// HDMI

#define DEFAULT_PRODUCT						"StreamCatcher QCAP - PD652"

#define DEFAULT_PRODUCT_VERSION				""



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A6EF1E93_33EB_476A_B556_185C5C5C1935__INCLUDED_)
