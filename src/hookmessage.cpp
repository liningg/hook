// inject.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <iostream>
#include <map>
using std::map;
using namespace std;
extern HINSTANCE hinst;
#pragma data_seg("SharedDataName")
HHOOK hooker=NULL;
HWND hInterhwnd = NULL;
HWND hActiveWnd = NULL;

#pragma data_seg()
#pragma comment(linker,"/section:SharedDataName,rws")


#define WM_INJECT_CREATE		WM_USER+100
#define WM_INJECT_SHOWWINDOW    WM_USER+101
#define WM_INJECT_CLOSE			WM_USER+102
#define WM_INJECT_DESTROY		WM_USER+103
#define WM_INJECT_ACTIVETE      WM_USER+104
#define WM_INJECT_NOTIFY		WM_USER+105

//注入窗口类型
typedef enum WINDOW_TYPE
{
	WINDOW_TYPE_MFC = 0,
	WINDOW_TYPE_WPF,
	WINDOW_TYPE_QT,
};
static WINDOW_TYPE wndType = WINDOW_TYPE_MFC;
static int nCreateMsgNumber[] = {1,100,100};//当时mfc程序的时候保存createwindow消息就保存第一个create消息，如果是wpf 或者 qt 程序就随意保存，因为create消息之后就是showwindow
static map<int,int> mapHandle;
LRESULT CALLBACK OnWndProc(int code,WPARAM wParam,LPARAM lParam)
{
	 CWPSTRUCT *pCwp = reinterpret_cast<CWPSTRUCT*>(lParam);  
	 switch(pCwp->message)
	 {
	 case WM_CREATE:
		 {
			 break;
		 }
	 case WM_SHOWWINDOW:
		 {
			 HWND hwnd = reinterpret_cast<HWND>((pCwp->hwnd)); 

			 map<int,int>::iterator it = mapHandle.find((int)hwnd);
			 if(it != mapHandle.end()){
				 PostMessage(hInterhwnd,WM_INJECT_SHOWWINDOW,(WPARAM)hwnd,0);
				 LPRECT lpRect;
				 lpRect = (LPRECT)malloc(sizeof(tagRECT));
				 ::GetWindowRect(hwnd,lpRect);
				 FILE * fp = fopen("f:\\abc.txt","a+");
				 if(fp){
					 char buff[1000] = {0};
					 sprintf(buff,"showwindow msg point left = %d,top = %d,right = %d,bottom = %d,handle = 0x%x type = %d map = %d\n",lpRect->left,lpRect->top,lpRect->right,lpRect->bottom,hwnd,nCreateMsgNumber[wndType],mapHandle.size() );
					 fwrite(buff,strlen(buff),1,fp);
					 fclose(fp);
				 }
				 mapHandle.clear();
			 }
			 break;
		 }
	 case WM_CLOSE:
		 {
			 HWND hwnd = reinterpret_cast<HWND>((pCwp->hwnd)); 
			 PostMessage(hInterhwnd,WM_INJECT_CLOSE,(WPARAM)hwnd,0);
			 LPRECT lpRect;
			 lpRect = (LPRECT)malloc(sizeof(tagRECT));
			 ::GetWindowRect(hwnd,lpRect);
			 FILE * fp = fopen("f:\\abc.txt","a+");
			 if(fp){

				 char buff[1000] = {0};
				 sprintf(buff,"close msg point left = %d,top = %d,right = %d,bottom = %d,handle = 0x%x\n",lpRect->left,lpRect->top,lpRect->right,lpRect->bottom,hwnd);
				 fwrite(buff,strlen(buff),1,fp);
				 fclose(fp);
			 }
			 mapHandle.clear();
			 break;
		 }
	 case WM_NCCREATE:
		 {
			 HWND hwnd = reinterpret_cast<HWND>((pCwp->hwnd)); 
			 //mapHandle.insert(make_pair((int)hwnd,0));
			 LPRECT lpRect;
			 lpRect = (LPRECT)malloc(sizeof(tagRECT));
			 ::GetWindowRect(hwnd,lpRect);
			 if(lpRect->right - lpRect->left != 0){
				 if(mapHandle.size() < nCreateMsgNumber[wndType]){
					 mapHandle.insert(make_pair((int)hwnd,0));
					 FILE * fp = fopen("f:\\abc.txt","a+");
					 if(fp){

						 char buff[1000] = {0};
						 sprintf(buff,"createwindow msg point left = %d,top = %d,right = %d,bottom = %d,handle = 0x%x type = %d map = %d\n",lpRect->left,lpRect->top,lpRect->right,lpRect->bottom,hwnd,nCreateMsgNumber[wndType],mapHandle.size() );
						 fwrite(buff,strlen(buff),1,fp);
						 fclose(fp);
					 }
				 }

			 }
			 break;
		 }

	 default:
		 {
			 break;
		 }

	 }
	return CallNextHookEx(hooker,code,wParam,lParam);
}


extern "C" __declspec(dllexport) int SetHook(HWND hwnd,int threadid,int wndtype)
{
	hInterhwnd = hwnd;
	wndType = (WINDOW_TYPE)wndtype;
	mapHandle.clear();
	hooker = SetWindowsHookEx(WH_CALLWNDPROC,OnWndProc,hinst,threadid);
	if(hooker == NULL){
		return 0;
	}
	return 1;
}
extern "C" __declspec(dllexport) int UnHook()
{
	mapHandle.clear();
	UnhookWindowsHookEx(hooker);
	return 1;
}
