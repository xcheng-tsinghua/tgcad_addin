

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for SampleAddin.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0628 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __SampleAddin_i_h__
#define __SampleAddin_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DECLSPEC_XFGVIRT
#if defined(_CONTROL_FLOW_GUARD_XFG)
#define DECLSPEC_XFGVIRT(base, func) __declspec(xfg_virtual(base, func))
#else
#define DECLSPEC_XFGVIRT(base, func)
#endif
#endif

/* Forward Declarations */ 

#ifndef __ASMLOCAddInEventsEx2_FWD_DEFINED__
#define __ASMLOCAddInEventsEx2_FWD_DEFINED__

#ifdef __cplusplus
typedef class ASMLOCAddInEventsEx2 ASMLOCAddInEventsEx2;
#else
typedef struct ASMLOCAddInEventsEx2 ASMLOCAddInEventsEx2;
#endif /* __cplusplus */

#endif 	/* __ASMLOCAddInEventsEx2_FWD_DEFINED__ */


#ifndef __ASMLOCAddInEventsEx_FWD_DEFINED__
#define __ASMLOCAddInEventsEx_FWD_DEFINED__

#ifdef __cplusplus
typedef class ASMLOCAddInEventsEx ASMLOCAddInEventsEx;
#else
typedef struct ASMLOCAddInEventsEx ASMLOCAddInEventsEx;
#endif /* __cplusplus */

#endif 	/* __ASMLOCAddInEventsEx_FWD_DEFINED__ */


#ifndef __ASMLOCAddInEvents_FWD_DEFINED__
#define __ASMLOCAddInEvents_FWD_DEFINED__

#ifdef __cplusplus
typedef class ASMLOCAddInEvents ASMLOCAddInEvents;
#else
typedef struct ASMLOCAddInEvents ASMLOCAddInEvents;
#endif /* __cplusplus */

#endif 	/* __ASMLOCAddInEvents_FWD_DEFINED__ */


#ifndef __ASMLOCApplicationEvents_FWD_DEFINED__
#define __ASMLOCApplicationEvents_FWD_DEFINED__

#ifdef __cplusplus
typedef class ASMLOCApplicationEvents ASMLOCApplicationEvents;
#else
typedef struct ASMLOCApplicationEvents ASMLOCApplicationEvents;
#endif /* __cplusplus */

#endif 	/* __ASMLOCApplicationEvents_FWD_DEFINED__ */


#ifndef __ASMLOCApplicationEventsEx_FWD_DEFINED__
#define __ASMLOCApplicationEventsEx_FWD_DEFINED__

#ifdef __cplusplus
typedef class ASMLOCApplicationEventsEx ASMLOCApplicationEventsEx;
#else
typedef struct ASMLOCApplicationEventsEx ASMLOCApplicationEventsEx;
#endif /* __cplusplus */

#endif 	/* __ASMLOCApplicationEventsEx_FWD_DEFINED__ */


#ifndef __ASMLOCAddInEdgeBarEvents_FWD_DEFINED__
#define __ASMLOCAddInEdgeBarEvents_FWD_DEFINED__

#ifdef __cplusplus
typedef class ASMLOCAddInEdgeBarEvents ASMLOCAddInEdgeBarEvents;
#else
typedef struct ASMLOCAddInEdgeBarEvents ASMLOCAddInEdgeBarEvents;
#endif /* __cplusplus */

#endif 	/* __ASMLOCAddInEdgeBarEvents_FWD_DEFINED__ */


#ifndef __Commands_FWD_DEFINED__
#define __Commands_FWD_DEFINED__

#ifdef __cplusplus
typedef class Commands Commands;
#else
typedef struct Commands Commands;
#endif /* __cplusplus */

#endif 	/* __Commands_FWD_DEFINED__ */


#ifndef __ASMLOCCommand_FWD_DEFINED__
#define __ASMLOCCommand_FWD_DEFINED__

#ifdef __cplusplus
typedef class ASMLOCCommand ASMLOCCommand;
#else
typedef struct ASMLOCCommand ASMLOCCommand;
#endif /* __cplusplus */

#endif 	/* __ASMLOCCommand_FWD_DEFINED__ */


#ifndef __CommandEvents_FWD_DEFINED__
#define __CommandEvents_FWD_DEFINED__

#ifdef __cplusplus
typedef class CommandEvents CommandEvents;
#else
typedef struct CommandEvents CommandEvents;
#endif /* __cplusplus */

#endif 	/* __CommandEvents_FWD_DEFINED__ */


#ifndef __MouseEvents_FWD_DEFINED__
#define __MouseEvents_FWD_DEFINED__

#ifdef __cplusplus
typedef class MouseEvents MouseEvents;
#else
typedef struct MouseEvents MouseEvents;
#endif /* __cplusplus */

#endif 	/* __MouseEvents_FWD_DEFINED__ */


#ifndef __LocateFilterEvents_FWD_DEFINED__
#define __LocateFilterEvents_FWD_DEFINED__

#ifdef __cplusplus
typedef class LocateFilterEvents LocateFilterEvents;
#else
typedef struct LocateFilterEvents LocateFilterEvents;
#endif /* __cplusplus */

#endif 	/* __LocateFilterEvents_FWD_DEFINED__ */


#ifndef __CommandWindowEvents_FWD_DEFINED__
#define __CommandWindowEvents_FWD_DEFINED__

#ifdef __cplusplus
typedef class CommandWindowEvents CommandWindowEvents;
#else
typedef struct CommandWindowEvents CommandWindowEvents;
#endif /* __cplusplus */

#endif 	/* __CommandWindowEvents_FWD_DEFINED__ */


#ifndef __ADDINDocument_FWD_DEFINED__
#define __ADDINDocument_FWD_DEFINED__

#ifdef __cplusplus
typedef class ADDINDocument ADDINDocument;
#else
typedef struct ADDINDocument ADDINDocument;
#endif /* __cplusplus */

#endif 	/* __ADDINDocument_FWD_DEFINED__ */


#ifndef __ADDINDocumentEvents_FWD_DEFINED__
#define __ADDINDocumentEvents_FWD_DEFINED__

#ifdef __cplusplus
typedef class ADDINDocumentEvents ADDINDocumentEvents;
#else
typedef struct ADDINDocumentEvents ADDINDocumentEvents;
#endif /* __cplusplus */

#endif 	/* __ADDINDocumentEvents_FWD_DEFINED__ */


#ifndef __ADDINShortCutMenuEvents_FWD_DEFINED__
#define __ADDINShortCutMenuEvents_FWD_DEFINED__

#ifdef __cplusplus
typedef class ADDINShortCutMenuEvents ADDINShortCutMenuEvents;
#else
typedef struct ADDINShortCutMenuEvents ADDINShortCutMenuEvents;
#endif /* __cplusplus */

#endif 	/* __ADDINShortCutMenuEvents_FWD_DEFINED__ */


#ifndef __ADDINSaveAsTranslatorEvents_FWD_DEFINED__
#define __ADDINSaveAsTranslatorEvents_FWD_DEFINED__

#ifdef __cplusplus
typedef class ADDINSaveAsTranslatorEvents ADDINSaveAsTranslatorEvents;
#else
typedef struct ADDINSaveAsTranslatorEvents ADDINSaveAsTranslatorEvents;
#endif /* __cplusplus */

#endif 	/* __ADDINSaveAsTranslatorEvents_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __SampleAddinLib_LIBRARY_DEFINED__
#define __SampleAddinLib_LIBRARY_DEFINED__

/* library SampleAddinLib */
/* [helpstring][version][uuid] */ 


DEFINE_GUID(LIBID_SampleAddinLib,0xE0BF13EF,0x51B1,0x4E9E,0x8B,0x27,0xFC,0x61,0x86,0x0A,0x82,0xA0);

DEFINE_GUID(CLSID_ASMLOCAddInEventsEx2,0xB813D259,0x4844,0x49E7,0xA3,0xB2,0x7B,0x35,0x9C,0xC3,0xA7,0x8A);

#ifdef __cplusplus

class DECLSPEC_UUID("B813D259-4844-49E7-A3B2-7B359CC3A78A")
ASMLOCAddInEventsEx2;
#endif

DEFINE_GUID(CLSID_ASMLOCAddInEventsEx,0x35F69A4E,0x1681,0x4745,0xB1,0x92,0x0A,0x55,0xB2,0x84,0x3D,0x0F);

#ifdef __cplusplus

class DECLSPEC_UUID("35F69A4E-1681-4745-B192-0A55B2843D0F")
ASMLOCAddInEventsEx;
#endif

DEFINE_GUID(CLSID_ASMLOCAddInEvents,0x6D4144F3,0x2FC2,0x11D3,0x92,0x76,0x00,0xC0,0x4F,0x79,0xBE,0x98);

#ifdef __cplusplus

class DECLSPEC_UUID("6D4144F3-2FC2-11D3-9276-00C04F79BE98")
ASMLOCAddInEvents;
#endif

DEFINE_GUID(CLSID_ASMLOCApplicationEvents,0x104A396A,0xDE35,0x46b6,0x88,0x06,0x75,0x4A,0x7A,0x4C,0xCC,0xB8);

#ifdef __cplusplus

class DECLSPEC_UUID("104A396A-DE35-46b6-8806-754A7A4CCCB8")
ASMLOCApplicationEvents;
#endif

DEFINE_GUID(CLSID_ASMLOCApplicationEventsEx,0x82C7A5E4,0xC5BA,0x4664,0xB7,0x7C,0xE9,0x18,0x8F,0x59,0x08,0xE6);

#ifdef __cplusplus

class DECLSPEC_UUID("82C7A5E4-C5BA-4664-B77C-E9188F5908E6")
ASMLOCApplicationEventsEx;
#endif

DEFINE_GUID(CLSID_ASMLOCAddInEdgeBarEvents,0xD3DEF212,0x1809,0x4aeb,0x8C,0x93,0x97,0x45,0xAA,0x28,0xAC,0xF8);

#ifdef __cplusplus

class DECLSPEC_UUID("D3DEF212-1809-4aeb-8C93-9745AA28ACF8")
ASMLOCAddInEdgeBarEvents;
#endif

DEFINE_GUID(CLSID_Commands,0x6D4144EC,0x2FC2,0x11D3,0x92,0x76,0x00,0xC0,0x4F,0x79,0xBE,0x98);

#ifdef __cplusplus

class DECLSPEC_UUID("6D4144EC-2FC2-11D3-9276-00C04F79BE98")
Commands;
#endif

DEFINE_GUID(CLSID_ASMLOCCommand,0x6D4144ED,0x2FC2,0x11D3,0x92,0x76,0x00,0xC0,0x4F,0x79,0xBE,0x98);

#ifdef __cplusplus

class DECLSPEC_UUID("6D4144ED-2FC2-11D3-9276-00C04F79BE98")
ASMLOCCommand;
#endif

DEFINE_GUID(CLSID_CommandEvents,0x6D4144F0,0x2FC2,0x11D3,0x92,0x76,0x00,0xC0,0x4F,0x79,0xBE,0x98);

#ifdef __cplusplus

class DECLSPEC_UUID("6D4144F0-2FC2-11D3-9276-00C04F79BE98")
CommandEvents;
#endif

DEFINE_GUID(CLSID_MouseEvents,0x6D4144F1,0x2FC2,0x11D3,0x92,0x76,0x00,0xC0,0x4F,0x79,0xBE,0x98);

#ifdef __cplusplus

class DECLSPEC_UUID("6D4144F1-2FC2-11D3-9276-00C04F79BE98")
MouseEvents;
#endif

DEFINE_GUID(CLSID_LocateFilterEvents,0xD0BE7F5A,0xF2C5,0x4a30,0x94,0xC8,0xC9,0x6F,0xED,0x31,0x6D,0xEF);

#ifdef __cplusplus

class DECLSPEC_UUID("D0BE7F5A-F2C5-4a30-94C8-C96FED316DEF")
LocateFilterEvents;
#endif

DEFINE_GUID(CLSID_CommandWindowEvents,0x6D4144F2,0x2FC2,0x11D3,0x92,0x76,0x00,0xC0,0x4F,0x79,0xBE,0x98);

#ifdef __cplusplus

class DECLSPEC_UUID("6D4144F2-2FC2-11D3-9276-00C04F79BE98")
CommandWindowEvents;
#endif

DEFINE_GUID(CLSID_ADDINDocument,0x6347FC9F,0x8E34,0x4f07,0x80,0xC4,0x9F,0xD9,0xDF,0x23,0xCA,0xA7);

#ifdef __cplusplus

class DECLSPEC_UUID("6347FC9F-8E34-4f07-80C4-9FD9DF23CAA7")
ADDINDocument;
#endif

DEFINE_GUID(CLSID_ADDINDocumentEvents,0x1E6DA6E8,0x090A,0x4580,0xA8,0x07,0xF8,0xBF,0x78,0x37,0x86,0xE8);

#ifdef __cplusplus

class DECLSPEC_UUID("1E6DA6E8-090A-4580-A807-F8BF783786E8")
ADDINDocumentEvents;
#endif

DEFINE_GUID(CLSID_ADDINShortCutMenuEvents,0xA3C84CA3,0xBAB2,0x4C1A,0xA2,0x44,0xF9,0x9B,0x01,0x52,0x1A,0x07);

#ifdef __cplusplus

class DECLSPEC_UUID("A3C84CA3-BAB2-4C1A-A244-F99B01521A07")
ADDINShortCutMenuEvents;
#endif

DEFINE_GUID(CLSID_ADDINSaveAsTranslatorEvents,0x0BC20255,0x57FC,0x4BF0,0xBE,0xE4,0xC8,0x84,0x73,0xF9,0xEF,0x8F);

#ifdef __cplusplus

class DECLSPEC_UUID("0BC20255-57FC-4BF0-BEE4-C88473F9EF8F")
ADDINSaveAsTranslatorEvents;
#endif
#endif /* __SampleAddinLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


