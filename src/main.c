#include <windows.h>
#include <stdio.h>

#include "wasp.h"
#include "helper.h"
#include "resource.h"

// Globals

HBRUSH hBrush = NULL;
HICON hIcon;
DWORD sizeOfFile = 0;

char filePath[MAX_PATH];
unsigned char* buffer = NULL;


INT_PTR CALLBACK mainDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam){
	
	switch(message){
		
		case WM_INITDIALOG:
		{
			
			hIcon = LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON,GetSystemMetrics(SM_CXICON),GetSystemMetrics(SM_CYICON),0);
			
			if(hIcon)
				SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
			
			hBrush = CreateSolidBrush(RGB(24, 33, 41));
			
			GetEnvironmentVariable("APPDATA",filePath,MAX_PATH);
			snprintf(filePath, sizeof(filePath),"%s\\WebacamSurveyor\\WSSettings.wss",filePath);
			
			// load current user WSSettings.wss file 
			SendMessage(hDlg, WM_COMMAND, MAKEWPARAM(IDM_LOADCURRENT,0), 0);
			
		}
		break;
		
		
		case WM_CTLCOLORDLG: 
		 return (LONG)hBrush;
		
		case WM_CTLCOLORSTATIC:
		{
			HDC hdc = (HDC)wParam;
			SetBkMode(hdc,TRANSPARENT);
			
			SetTextColor(hdc,RGB(255,255,255));
			return (LONG)hBrush;
		}
		
		case WM_CLOSE:
		{
			
			DeleteObject(hBrush);
			DestroyIcon(hIcon);
			GlobalFree(buffer);
			EndDialog(hDlg,0);
		}
		break;
		
		case WM_COMMAND:
			switch(LOWORD(wParam)){
				
				case IDC_DECRYPT:
				{
					
					unsigned char *pass = NULL;
					DWORD passLen = 0;
					
					if (buffer != NULL)
					{
						passLen = getPasswordLen(buffer, sizeOfFile);
						pass = GlobalAlloc(GPTR, passLen);
					
						if (pass != NULL)
						{
							SetDlgItemText(hDlg,IDC_LBLPASS,"Password:");
						
							if(getPassword(buffer,sizeOfFile,pass,passLen) == TRUE)
								SetDlgItemText(hDlg,IDC_EDIT,(char*)pass);
						 
							GlobalFree(pass);
						}
					}

				}
				break;
				
				case IDC_ANTISTEALTH:
				{
					
					if(buffer != NULL)
						antiStealth(buffer,sizeOfFile,hDlg);
				}
				break;
				
				case IDC_COPY:
				{
					SendMessage(GetDlgItem(hDlg,IDC_EDIT),EM_SETSEL,0,-1);
					SendMessage(GetDlgItem(hDlg,IDC_EDIT),WM_COPY,0,0);
				}
				break;
				
				case IDM_EXIT:
						SendMessage(hDlg,WM_CLOSE,0,0);
				break;
				
				case IDM_OPENFILE:
				{ 
					
					SendMessage(GetDlgItem(hDlg,IDC_EDIT),EM_SETSEL,0,-1);
					SendMessage(GetDlgItem(hDlg,IDC_EDIT),WM_CLEAR,0,0);
					
					if(openFileDialog(hDlg) != FALSE)
					{
						
						GlobalFree(buffer);
						buffer = LoadFileToMem(&sizeOfFile,szFileName);
						
						if(buffer != NULL)
							SetDlgItemText(hDlg,IDC_STATUS,szFileName);
						else
							SetDlgItemText(hDlg,IDC_STATUS,"Failed to load selected WSSettings.wss file!");
					}
				}
				break;
				
				case IDM_LOADCURRENT:
				{
					
					GlobalFree(buffer);
				
					SendMessage(GetDlgItem(hDlg,IDC_EDIT),EM_SETSEL,0,-1);
					SendMessage(GetDlgItem(hDlg,IDC_EDIT),WM_CLEAR,0,0);
					
					buffer = LoadFileToMem(&sizeOfFile,filePath);
					
					if(buffer != NULL)
						SetDlgItemText(hDlg,IDC_STATUS,"Current user WSSettings.wss file loaded...Press Anti-Stealth/Decrypt");
					else
						SetDlgItemText(hDlg,IDC_STATUS,"Failed to load current user WSSettings.wss file!");
				
				}
				break;
				
				case IDM_ABOUT:
						MessageBox(hDlg,"Webcam Surveyor v3.x Anti-Security Program *WASP*\n\nCoded by: Aleksandar"
							  " 'revengsmK'\nLogo by: Aleksandar 'revengsmK'\n\nCoded in C / WinAPI\n\nVisit: github.com/revengsmK","ab0ut",MB_OK | MB_ICONINFORMATION);
				break;
				
			}
		return 0;
		
		default:
			return FALSE;
	}
	
	return TRUE;
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine , int nCmdShow){
	
	return DialogBoxParam(hInstance,MAKEINTRESOURCE(IDD_DIALOG1),NULL,mainDlgProc,0);
	
}