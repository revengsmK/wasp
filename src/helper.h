#ifndef HELPER_H
#define HELPER_H

BOOL openFileDialog(HWND hDlg);
DWORD getPasswordLen(unsigned char fileBuffer[], DWORD fileSize);
unsigned char *searchbytes(unsigned char fileBuffer[], unsigned char magic_bytes[] ,DWORD fileSize,int numOfBytes);
unsigned char *LoadFileToMem(DWORD *fileSize, char filePath[]);
void decrypter(unsigned char *input , unsigned char *output, DWORD len);

extern char szFileName[MAX_PATH];

#endif