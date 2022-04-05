#ifndef WASP_H
#define WASP_H

BOOL getPassword(unsigned char *fileBuffer, DWORD fileSize, unsigned char *password , DWORD passLength);
void antiStealth( unsigned char *fileBuffer , DWORD fileSize , HWND hDlg);

#endif