#include <windows.h>
#include <stdio.h>


#include "wasp.h"
#include "helper.h"
#include "resource.h"


BOOL getPassword(unsigned char *fileBuffer, DWORD fileSize, unsigned char *password , DWORD passLength){
	
	unsigned char magic_bytes[] = { 0x2B, 0x22, 0x33, 0x18, 0x05, 0x14, 0x15, 0x14, 0x26 };
	unsigned char *address = NULL;

	address = searchbytes(fileBuffer, magic_bytes, fileSize, 9);
	
	if(address == NULL)
	{
		MessageBox(NULL,"Magic bytes not found, cannot decrypt password!","Error",MB_OK | MB_ICONERROR);
		return FALSE;
	}
	
	address+= 0x28;
	decrypter(address, password, passLength);
	
	return TRUE;
}


void antiStealth( unsigned char *fileBuffer , DWORD fileSize , HWND hDlg){
	
	unsigned char magic_bytes[] = { 0x2B, 0x22, 0x33, 0x00, 0x0B, 0x13, 0x0D, 0x06, 0x30, 0x63 };
	unsigned char *addr = NULL;
	char keyname[70];
	unsigned int scanCode = 0;
	char combination[70] = { '\0' };
	
		
	addr = searchbytes(fileBuffer,magic_bytes,fileSize,10);
	
	if(addr == NULL){
		MessageBox(NULL,"Magic bytes not found, cannot discover key combination!","Error",MB_OK | MB_ICONERROR);
		return;
	}
	
	addr+= 0x28;
	
	// get keyboard scancode from virtual key code
	scanCode = MapVirtualKey(*addr, MAPVK_VK_TO_VSC);
	
	scanCode = scanCode << 16;
	
	 switch (*addr)
    {
        case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN: 
        case VK_PRIOR: case VK_NEXT: 
        case VK_END: case VK_HOME:
        case VK_INSERT: case VK_DELETE:
        case VK_DIVIDE: 
		case VK_PAUSE:
		case VK_CANCEL:
        case VK_NUMLOCK:
        {
			// set extended-key bit to 1 to display key name properly
            scanCode |= 0x1000000; 
            break;
        }
    }
	
	GetKeyNameText(scanCode, keyname, 70);
	
	SetDlgItemText(hDlg,IDC_LBLPASS,"Key combo:");
	
	switch(*(addr+1))
	{
		
		case 0x28:
			snprintf(combination, sizeof(combination),"Ctrl + %s",keyname);
		break;
		
		case 0xE8:
			snprintf(combination, sizeof(combination),"Alt + %s",keyname);
		break;
		
		case 0xA8:
			snprintf(combination, sizeof(combination),"Ctrl + Alt + %s",keyname);
		break;
		
		case 0x08:
			snprintf(combination, sizeof(combination),"Ctrl + Shift + %s",keyname);
		break;
		
		case 0x88:
			snprintf(combination, sizeof(combination),"Ctrl + Shift + Alt + %s",keyname);
		break;
		
		case 0xC8:
			snprintf(combination, sizeof(combination),"Alt + Shift + %s",keyname);
		break;
		
		default:
			SetDlgItemText(hDlg,IDC_EDIT,keyname);
			return;

	}
	
	SetDlgItemText(hDlg,IDC_EDIT,combination);
}
