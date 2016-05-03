#include <windows.h>
#include <windowsx.h>
#include "key_acsII.h"

#define ROW_NUM 5    //����
#define COL_NUM 17  //����
#define WM_CHILDWIN  WM_USER+1    //������֪ͨ�Ӵ��ڵ��Զ�����Ϣ

BOOL InitWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
HINSTANCE hInst;                                  //ȫ�̱�������ʵ�����
int        x, y;
TCHAR Childsz[10];

/*typedef struct tagkeyinfo
{
	char upper;
	char lower;
	bool bshow;
}keyinfo, *pkeyinfo;
keyinfo keys[2][2] = {
	{'A','a',TRUE},
	{'B','b',TRUE},
}*/ //���Ͻ��ķ���
int WINAPI WinMain(HINSTANCE hInstance,         //��ǰʵ�����
	HINSTANCE hPrevInstance,     //ǰһ��ʵ�����
	PSTR szCmdLine,              //�������ַ�
	int iCmdShow)                //������ʾ��ʽ
{
	MSG msg;

	if (!InitWindow(hInstance, iCmdShow))
		return FALSE;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

BOOL InitWindow(HINSTANCE hInstance, int iCmdShow)
{
	TCHAR szAppName[] = TEXT("ChildWin");		  //Ӧ�ó�������
	HWND hwnd;                                    //���ھ��
	WNDCLASS wcMainWnd;                           //������ṹ

	wcMainWnd.style = CS_VREDRAW | CS_HREDRAW;
	wcMainWnd.lpfnWndProc = WinProc;
	wcMainWnd.cbClsExtra = 0;
	wcMainWnd.cbWndExtra = 0;
	wcMainWnd.hInstance = hInstance;
	wcMainWnd.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcMainWnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcMainWnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcMainWnd.lpszMenuName = NULL;
	wcMainWnd.lpszClassName = szAppName;

	if (!RegisterClass(&wcMainWnd))
	{
		MessageBox(NULL, TEXT("ע�ᴰ����ʧ�ܣ�"), szAppName,
			MB_ICONERROR);
		return 0;
	}
	hInst = hInstance;
	hwnd = CreateWindow(szAppName, 						//����������
		TEXT("Windows �����"),  //���ڱ���
		WS_OVERLAPPEDWINDOW,				//���ڷ��
		100,								//����λ�õ�x����
		100,								//����λ�õ�y����
		400,								//���ڵĿ��
		300,								//���ڵĸ߶�
		NULL,								//�����ھ��
		NULL,								//�˵����
		hInstance,							//Ӧ�ó���ʵ�����
		NULL);								//���ڴ�������ָ��

	if (!hwnd) return FALSE;

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	return TRUE;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT        rect;
	static	HWND	hWndEdit;

	static HWND hChildWnd[17][ROW_NUM];  //�Ӵ��ھ��
	static int  nXBox, nYBox;                 //�Ӵ��ڿ�Ⱥ͸߶�
	static WORD nChildWin_ID;                 //�����е��Ӵ��ڱ�ʶ
	static int  nRow, nCol;                   //�����е��Ӵ���λ�ã������кţ�

	char		c = NULL;
	WNDCLASS    wcChild;                      //�Ӵ�����
	TCHAR szChildName[] = TEXT ("ChildWin_Child");    //�Ӵ���������

	switch (message)
	{
	case WM_CREATE:
		hWndEdit = CreateWindow(TEXT("edit"),
			NULL,
			WS_CHILD|WS_VISIBLE|
			WS_HSCROLL|WS_VSCROLL|WS_BORDER|ES_LEFT|
			ES_MULTILINE|ES_AUTOHSCROLL|ES_AUTOVSCROLL,
			0,0,0,0, hWnd, (HMENU)1, 
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);

		wcChild.style         = CS_VREDRAW | CS_HREDRAW;
		wcChild.lpfnWndProc   = ChildWndProc;
		wcChild.cbClsExtra    = 0;
		wcChild.cbWndExtra    = 5*sizeof (long);
		wcChild.hInstance     = hInst;
		wcChild.hIcon         = NULL;
		wcChild.hCursor       = LoadCursor (NULL, IDC_ARROW);
		wcChild.hbrBackground = (HBRUSH)GetStockObject (LTGRAY_BRUSH);
		wcChild.lpszMenuName  = NULL;
		wcChild.lpszClassName = szChildName ;

		if (!RegisterClass (&wcChild))
		{
			MessageBox (NULL, TEXT ("ע���Ӵ�����ʧ�ܣ�"), szChildName,
				MB_ICONERROR);
			return 0;
		}

		//�������ơ���ͼ�������л�ͼ�������ĸ������Ӵ���
		for (y=0; y<1; y++)
		{
			for (x=1; x<13; x++)
			{
				nChildWin_ID = y<<4|x;             //�Ӵ��ڱ�ʶֵ
				hChildWnd[x][y] = CreateWindow (szChildName, NULL,
					WS_CHILDWINDOW | WS_DLGFRAME |WS_VISIBLE,
					0,0,0,0, hWnd, (HMENU)(nChildWin_ID),
					hInst, (LPVOID)c);
			}
		}
		for (y=1; y<2; y++)
		{
			for (x=1; x<14; x++)
			{
				nChildWin_ID = y<<4|x;             //�Ӵ��ڱ�ʶֵ
				hChildWnd[x][y] = CreateWindow (szChildName, NULL,
					WS_CHILDWINDOW | WS_DLGFRAME |WS_VISIBLE,
					0,0,0,0, hWnd, (HMENU)(nChildWin_ID),
					hInst, (LPVOID)c);
			}
		}

			for (y=2; y<3; y++)
			{
				for (x=1; x<12; x++)
				{
					nChildWin_ID = y<<4|x;             //�Ӵ��ڱ�ʶֵ
					hChildWnd[x][y] = CreateWindow (szChildName, NULL,
						WS_CHILDWINDOW | WS_DLGFRAME |WS_VISIBLE,
						0,0,0,0, hWnd, (HMENU)(nChildWin_ID),
						hInst, NULL);
				}
			}

			for (y=3; y<4; y++)
			{
				for (x=1; x<11; x++)
				{
					nChildWin_ID = y<<4|x;             //�Ӵ��ڱ�ʶֵ
					hChildWnd[x][y] = CreateWindow (szChildName, NULL,
						WS_CHILDWINDOW | WS_DLGFRAME |WS_VISIBLE,
						0,0,0,0, hWnd, (HMENU)(nChildWin_ID),
						hInst, NULL);
				}
			}

			//��0���Ӵ���Ϊ��ǰ���е��Ӵ���
			nChildWin_ID = 0;
			nRow = 0;
			nCol = 0;
			return 0;

	case WM_SETFOCUS:   
		SetFocus(hWndEdit);   
		return 0;

	case WM_SIZE:	
		nXBox = LOWORD (lParam)/ROW_NUM/3; // ��
		nYBox = HIWORD (lParam)/COL_NUM; // ��

		//�����ڴ�С�ı�ʱ������ȷ�����Ӵ����������ڿͻ�����λ��
		for (y=0; y<1; y++){
			for (x=1; x<13; x++)
			{
				MoveWindow (hChildWnd[x][y], x * nXBox, 
					HIWORD(lParam) * 2 / 3 + y*nYBox, nXBox, nYBox, TRUE);
			}
		}

		for (y=1; y<2; y++)
		{
			for (x=1; x<14; x++)
			{
				MoveWindow (hChildWnd[x][y], x * nXBox, 
					HIWORD(lParam) * 2 / 3 + y*nYBox, nXBox, nYBox, TRUE);
			}
		}
		
		for (y=2; y<3; y++)
			for (x=1; x<12; x++)
			{
				MoveWindow (hChildWnd[x][y], x * nXBox, 
					HIWORD(lParam) * 2 / 3 + y*nYBox, nXBox, nYBox, TRUE);
			}

		for (y=3; y<4; y++)
			for (x=1; x<11; x++)
			{
				MoveWindow (hChildWnd[x][y], x * nXBox, 
					HIWORD(lParam) * 2 / 3 + y*nYBox, nXBox, nYBox, TRUE);
			}
		
		MoveWindow(hWndEdit, 0, 0,
			LOWORD(lParam), HIWORD(lParam) * 2 / 3, TRUE);

			return 0;

	case WM_COMMAND:                //�Ӵ��ڱ����е�֪ͨ��Ϣ
		//���Ӵ��ڱ�����ʱ���ʹ���Ϣ֪ͨ�����ڣ��Ա���������ȷ���Ƿ�������һ���Ӵ��ڣ�
		//���ǣ�������Ϣ֪ͨ�Ӵ��ں����ı�ǰһ�α������Ӵ��ڵ�״̬��
		if (!(LOWORD (wParam) == nChildWin_ID))
		{
			SendMessage ((HWND)hChildWnd[nCol][nRow], WM_CHILDWIN, 0, 0L);

			//��¼�µı����е��Ӵ���
			nChildWin_ID = LOWORD (wParam);
			nRow = nChildWin_ID/16;
			nCol = nChildWin_ID%16;
		}

		if (lParam)        //�༭�ؼ�֪ͨ��Ϣ      
			if ((LOWORD (wParam) == 1) 
				&&(HIWORD(wParam) == EN_ERRSPACE             
				|| HIWORD (wParam) == EN_MAXTEXT)) 
				MessageBox (hWnd,TEXT ("�༭�ؼ��ڴ����"), 
					TEXT ("�˵�ʾ������"), MB_OK|MB_ICONSTOP);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);

} //���� WinProc ����

LRESULT CALLBACK ChildWndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC         hdc;
	PAINTSTRUCT ps;
	RECT        rect;
	LONG l=0;
	static int xy;
	TCHAR win1;

	switch (message)
	{
	case WM_CREATE:
		xy ++;
		//�ø��Ӵ������б�־Ϊ0����0���Ӵ�����Ϊ1��
		if (GetWindowLong (hWnd, GWL_ID) == 0)
			l=SetWindowLong (hWnd, 0, 1);
		else
			l=SetWindowLong (hWnd, 0, 0);

		SetWindowLong(hWnd, 1, print_abc[xy - 1]);
		SetWindowLong(hWnd, 3, abc[xy - 1]);
		return 0;

	case WM_LBUTTONUP:
		//�µ��Ӵ��ڱ�����
		win1 = GetWindowLong(hWnd, 3);
			//�ø��Ӵ������б�־Ϊ1
			//SetWindowLong (hWnd, 0, 1);

			//������Ϣ֪ͨ������
			SendMessage (GetParent (hWnd), WM_COMMAND, 
				GetWindowLong (hWnd, GWL_ID),
				(LPARAM) hWnd);
			//VK_RETURN;
			keybd_event(win1, 0, 0, 0); //�þ��򸸴��ڷ���WM_CHAR��Ϣ
		return 0;

	case WM_PAINT:
		hdc = BeginPaint (hWnd, &ps);

		//������Ʊ����е��Ӵ���
		
		GetClientRect(hWnd, &rect);
		
		wsprintf(Childsz, L"%c", GetWindowLong(hWnd, 1));    //��ʾID
		DrawText(hdc, Childsz, -1, &rect, 
			DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		EndPaint (hWnd, &ps);
		return 0;

	case WM_DESTROY: 
		PostQuitMessage (0);
		return 0;
	}

	return DefWindowProc (hWnd, message, wParam, lParam);

} //���� ChildWinProc ����