// InjectDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Detours.h"
#include <stdio.h>
#include <iostream>

#pragma comment(lib,"Detours.lib")

//修改MessageBoxW 的响应函数
static   int  (WINAPI *  OLD_MessageBoxW)(HWND hWnd,LPCWSTR lpText,LPCWSTR lpCaption,UINT uType)  =  MessageBoxW;
int  WINAPI NEW_MessageBoxW(HWND hWnd,LPCWSTR lpText,LPCWSTR lpCaption,UINT uType)
{
	// 修改输入参数，调用原函数 
	int  ret  =  OLD_MessageBoxW(hWnd,L" 输入参数已修改 " ,L" [测试] " ,uType);
	return  ret;
}

//修改MessageBoxA的响应函数
static   int  (WINAPI *  OLD_MessageBoxA)( __in_opt HWND hWnd,
	__in_opt LPCSTR lpText,
	__in_opt LPCSTR lpCaption,
	__in UINT uType)  =  MessageBoxA;
int WINAPI NEW_MessageBoxA(HWND hWnd,LPCWSTR lpText,LPCWSTR lpCaption,UINT uType)
{
	int  ret  =  OLD_MessageBoxA(hWnd," 输入参数已修改 " ," [测试] " ,uType);
	return ret;
}

//修改CreateWindowExW 的响应函数
static HWND (WINAPI * OLD_CreateWindowExW)(  DWORD dwExStyle,
	 LPCWSTR lpClassName,
	 LPCWSTR lpWindowName,
	 DWORD dwStyle,
	 int X,
	 int Y,
	 int nWidth,
	 int nHeight,
	 HWND hWndParent,
	 HMENU hMenu,
	 HINSTANCE hInstance,
	 LPVOID lpParam) = CreateWindowExW;

HWND WINAPI NEW_CreateWindowExW( DWORD dwExStyle,
	 LPCWSTR lpClassName,
	 LPCWSTR lpWindowName,
	 DWORD dwStyle,
	 int X,
	 int Y,
	 int nWidth,
	 int nHeight,
	 HWND hWndParent,
	 HMENU hMenu,
	 HINSTANCE hInstance,
	 LPVOID lpParam)
{
	FILE * fp = fopen("d:\\abc.txt","a+");
	if(fp){

		fwrite(lpWindowName,wcslen(lpWindowName),1,fp);
		fclose(fp);
	}
	return OLD_CreateWindowExW(dwExStyle,lpClassName,lpWindowName,dwStyle,X,Y,nWidth,nHeight,hWndParent,hMenu,hInstance,lpParam);
}
//修改CreateWindowExW 的响应函数
static HWND (WINAPI * OLD_CreateWindowExA)(   DWORD dwExStyle,
	 LPCSTR lpClassName,
	 LPCSTR lpWindowName,
	 DWORD dwStyle,
	 int X,
	 int Y,
	 int nWidth,
	 int nHeight,
	 HWND hWndParent,
	 HMENU hMenu,
	 HINSTANCE hInstance,
	 LPVOID lpParam) = CreateWindowExA;

HWND WINAPI NEW_CreateWindowExA(  DWORD dwExStyle,
	 LPCSTR lpClassName,
	 LPCSTR lpWindowName,
	 DWORD dwStyle,
	 int X,
	 int Y,
	 int nWidth,
	 int nHeight,
	 HWND hWndParent,
	 HMENU hMenu,
	 HINSTANCE hInstance,
	 LPVOID lpParam)
{
	FILE * fp = fopen("d:\\abc.txt","a+");
	if(fp){

		fwrite(lpWindowName,strlen(lpWindowName),1,fp);
		fclose(fp);
	}
	return OLD_CreateWindowExA(dwExStyle,lpClassName,lpWindowName,dwStyle,X,Y,nWidth,nHeight,hWndParent,hMenu,hInstance,lpParam);
}
static INT_PTR
	(WINAPI * 
	OLD_DialogBoxParamA)(
	__in_opt HINSTANCE hInstance,
	__in LPCSTR lpTemplateName,
	__in_opt HWND hWndParent,
	__in_opt DLGPROC lpDialogFunc,
	__in LPARAM dwInitParam) = DialogBoxParamA;
INT_PTR
	WINAPI
	NEW_DialogBoxParamA(
	__in_opt HINSTANCE hInstance,
	__in LPCSTR lpTemplateName,
	__in_opt HWND hWndParent,
	__in_opt DLGPROC lpDialogFunc,
	__in LPARAM dwInitParam)
{
	FILE * fp = fopen("d:\\ggg.txt","a+");
	if(fp){

		fwrite(lpTemplateName,strlen(lpTemplateName),1,fp);
		fclose(fp);
	}
	return OLD_DialogBoxParamA(hInstance,lpTemplateName,hWndParent,lpDialogFunc,dwInitParam);
}
static INT_PTR
	(WINAPI*
	OLD_DialogBoxParamW)(
	__in_opt HINSTANCE hInstance,
	__in LPCWSTR lpTemplateName,
	__in_opt HWND hWndParent,
	__in_opt DLGPROC lpDialogFunc,
	__in LPARAM dwInitParam)= DialogBoxParamW;
INT_PTR
	WINAPI
	NEW_DialogBoxParamW(
	__in_opt HINSTANCE hInstance,
	__in LPCWSTR lpTemplateName,
	__in_opt HWND hWndParent,
	__in_opt DLGPROC lpDialogFunc,
	__in LPARAM dwInitParam)
{
	FILE * fp = fopen("d:\\ggg.txt","a+");
	if(fp){

		fwrite(lpTemplateName,wcslen(lpTemplateName),1,fp);
		fclose(fp);
	}
	return OLD_DialogBoxParamW(hInstance,lpTemplateName,hWndParent,lpDialogFunc,dwInitParam);
}
static HWND
	(WINAPI*
	OLD_CreateDialogParamA)(
	__in_opt HINSTANCE hInstance,
	__in LPCSTR lpTemplateName,
	__in_opt HWND hWndParent,
	__in_opt DLGPROC lpDialogFunc,
	__in LPARAM dwInitParam)= CreateDialogParamA;
HWND
	WINAPI
	NEW_CreateDialogParamA(
	__in_opt HINSTANCE hInstance,
	__in LPCSTR lpTemplateName,
	__in_opt HWND hWndParent,
	__in_opt DLGPROC lpDialogFunc,
	__in LPARAM dwInitParam)
{
	FILE * fp = fopen("d:\\rrr.txt","wb");
	if(fp){

		fwrite(lpTemplateName,strlen(lpTemplateName),1,fp);
		fclose(fp);
	}
	return OLD_CreateDialogParamA(hInstance,lpTemplateName,hWndParent,lpDialogFunc,dwInitParam);
}
static HWND
	(WINAPI*
	OLD_CreateDialogParamW)(
	__in_opt HINSTANCE hInstance,
	__in LPCWSTR lpTemplateName,
	__in_opt HWND hWndParent,
	__in_opt DLGPROC lpDialogFunc,
	__in LPARAM dwInitParam)= CreateDialogParamW;
HWND
	WINAPI
	NEW_CreateDialogParamW(
	__in_opt HINSTANCE hInstance,
	__in LPCWSTR lpTemplateName,
	__in_opt HWND hWndParent,
	__in_opt DLGPROC lpDialogFunc,
	__in LPARAM dwInitParam)
{
	FILE * fp = fopen("d:\\rrr.txt","wb");
	if(fp){

		fwrite(lpTemplateName,wcslen(lpTemplateName),1,fp);
		fclose(fp);
	}

	return OLD_CreateDialogParamW(hInstance,lpTemplateName,hWndParent,lpDialogFunc,dwInitParam);
}

static HWND
	(WINAPI*
	OLD_CreateDialogIndirectParamA)(
	__in_opt HINSTANCE hInstance,
	__in LPCDLGTEMPLATEA lpTemplate,
	__in_opt HWND hWndParent,
	__in_opt DLGPROC lpDialogFunc,
	__in LPARAM dwInitParam) = CreateDialogIndirectParamA;
HWND
	WINAPI
	NEW_CreateDialogIndirectParamA(
	__in_opt HINSTANCE hInstance,
	__in LPCDLGTEMPLATEA lpTemplate,
	__in_opt HWND hWndParent,
	__in_opt DLGPROC lpDialogFunc,
	__in LPARAM dwInitParam)
{
	FILE * fp = fopen("d:\\NEW_CreateDialogIndirectParamA.txt","a+");
	if(fp){
		char buff[100] = {0};
		strcpy(buff,"NEW_CreateDialogIndirectParamA\n");
		fwrite(buff,strlen(buff),1,fp);
		fclose(fp);
	}
	return OLD_CreateDialogIndirectParamA(hInstance,lpTemplate,hWndParent,lpDialogFunc,dwInitParam);
}
static HWND
	(WINAPI*
	OLD_CreateDialogIndirectParamW)(
	__in_opt HINSTANCE hInstance,
	__in LPCDLGTEMPLATEW lpTemplate,
	__in_opt HWND hWndParent,
	__in_opt DLGPROC lpDialogFunc,
	__in LPARAM dwInitParam) = CreateDialogIndirectParamW;
HWND
	WINAPI
	NEW_CreateDialogIndirectParamW(
	__in_opt HINSTANCE hInstance,
	__in LPCDLGTEMPLATEW lpTemplate,
	__in_opt HWND hWndParent,
	__in_opt DLGPROC lpDialogFunc,
	__in LPARAM dwInitParam)
{
	FILE * fp = fopen("d:\\NEW_CreateDialogIndirectParamW.txt","a+");
	if(fp){
		char buff[100] = {0};
		strcpy(buff,"NEW_CreateDialogIndirectParamW\n");
		fwrite(buff,strlen(buff),1,fp);
		fclose(fp);
	}
	return OLD_CreateDialogIndirectParamW(hInstance,lpTemplate,hWndParent,lpDialogFunc,dwInitParam);
}

VOID Hook()
{
	DetourRestoreAfterWith();
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	// 这里可以连续多次调用DetourAttach，表明HOOK多个函数 
	DetourAttach( & (PVOID & )OLD_MessageBoxA,NEW_MessageBoxA);
	DetourAttach( & (PVOID & )OLD_MessageBoxW,NEW_MessageBoxW);
	DetourAttach( & (PVOID & )OLD_CreateWindowExW,NEW_CreateWindowExW);
	DetourAttach( & (PVOID & )OLD_CreateWindowExA,NEW_CreateWindowExA);
	DetourAttach( & (PVOID & )OLD_DialogBoxParamW,NEW_DialogBoxParamW);
	DetourAttach( & (PVOID & )OLD_DialogBoxParamA,NEW_DialogBoxParamA);
	DetourAttach( & (PVOID & )OLD_CreateDialogParamW,NEW_CreateDialogParamW);
	DetourAttach( & (PVOID & )OLD_CreateDialogParamA,NEW_CreateDialogParamA);
	DetourAttach( & (PVOID & )OLD_CreateDialogIndirectParamA,NEW_CreateDialogIndirectParamA);
	DetourAttach( & (PVOID & )OLD_CreateDialogIndirectParamW,NEW_CreateDialogIndirectParamW);




	DetourTransactionCommit();
}

VOID UnHook()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	// 这里可以连续多次调用DetourDetach,表明撤销多个函数HOOK 
	DetourDetach( & (PVOID & )OLD_MessageBoxA,NEW_MessageBoxA);
	DetourDetach( & (PVOID & )OLD_MessageBoxW,NEW_MessageBoxW);
	DetourDetach( & (PVOID & )OLD_CreateWindowExW,NEW_CreateWindowExW);
	DetourDetach( & (PVOID & )OLD_CreateWindowExA,NEW_CreateWindowExA);
	DetourDetach( & (PVOID & )OLD_DialogBoxParamW,NEW_DialogBoxParamW);
	DetourDetach( & (PVOID & )OLD_DialogBoxParamA,NEW_DialogBoxParamA);
	DetourDetach( & (PVOID & )OLD_CreateDialogParamW,NEW_CreateDialogParamW);
	DetourDetach( & (PVOID & )OLD_CreateDialogParamA,NEW_CreateDialogParamA);
	DetourDetach( & (PVOID & )OLD_CreateDialogIndirectParamA,NEW_CreateDialogIndirectParamA);
	DetourDetach( & (PVOID & )OLD_CreateDialogIndirectParamW,NEW_CreateDialogIndirectParamW);

	DetourTransactionCommit();

}