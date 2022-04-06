#include <windows.h>

#include "helper.h"

 /* Webcam Surveyor Anti-Security Program (WASP) helper functions */

char szFileName[MAX_PATH];

BOOL openFileDialog(HWND hDlg){
	
	OPENFILENAME ofn;

	// zero ofn members
	ZeroMemory(&ofn,sizeof(ofn));
	
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrTitle = "Locate WSSettings.wss file";
	ofn.lpstrFilter = "WSSettings.wss\0*.wss\0\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	
	ofn.lpstrDefExt = "wss";
	
	if(GetOpenFileName(&ofn) != 0)
		return TRUE;
	
	
	return FALSE;
	
}

void decrypter(unsigned char *input , unsigned char *output, DWORD len){
	
	const char key[] = "\0hDmpSwrdGZxqlHdgfcIRuHsDHs5Tu";
	short j = 0;
	DWORD i = 0;
	
	while(i < len)
	{
	   *(output+i) = *(input+i) ^ key[j];
			
	    if(j == 30)
	    {
		j = 1;
		continue;
	    }
			
		i++;
		j++;
	}
	
}

DWORD getPasswordLen(unsigned char fileBuffer[], DWORD fileSize){
	
	unsigned char magic_bytes[] = { 0x2B, 0x22, 0x33, 0x18, 0x05, 0x14, 0x15, 0x14, 0x26 };
	unsigned char *address = NULL;
	unsigned char buf[80];
	DWORD passLen = 0;
	
	address = searchbytes(fileBuffer, magic_bytes, fileSize, 9);
	
	if(address == NULL)
	{
	  MessageBox(NULL,"Magic bytes not found, cannot get password length!","Error", MB_OK | MB_ICONERROR);
	  return 0;
	}
		
	address-=0x28;
	
	decrypter(address, buf , 80);
	
	passLen = (buf[79] << 24) + (buf[78] << 16)  + (buf[77] << 8)  + buf[76];
	
	return passLen;
	
}

unsigned char *searchbytes(unsigned char fileBuffer[], unsigned char magic_bytes[], DWORD fileSize, int numOfBytes){
	
	int i;
	
	for (i = 0; i < fileSize; i++){
		
	if(memcmp(magic_bytes,fileBuffer,numOfBytes) == 0)
	   return fileBuffer;
		
		
	   fileBuffer++;
	}
	
	return NULL;
}


unsigned char *LoadFileToMem(DWORD *fileSize, char filePath[]){
	
	HANDLE hFile;
	DWORD bytesRead = 0;
	unsigned char *fileBuffer = NULL;
	
	
	hFile = CreateFile(filePath,GENERIC_READ,FILE_SHARE_READ | FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	
	if(hFile == INVALID_HANDLE_VALUE){
		MessageBox(NULL,"Cannot open WSSettings.wss file!","Error",MB_OK | MB_ICONEXCLAMATION);
		return NULL;
	}
		
		*fileSize = GetFileSize(hFile,NULL);
		
		if(*fileSize != INVALID_FILE_SIZE)
		{
			// reserve memory block to load file
			fileBuffer = GlobalAlloc(GPTR,*fileSize);
		
			// load file content to allocated memory block
			if(fileBuffer != NULL)
				ReadFile(hFile,fileBuffer,*fileSize,&bytesRead,NULL);
			
			
			CloseHandle(hFile);
			return fileBuffer;
			
		}
		
		CloseHandle(hFile);
		return NULL;
}
