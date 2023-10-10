// stdafx.cpp : source file that includes just the standard includes
// Sceleton.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information
//

#include "stdafx.h"

extern HINSTANCE hInst;

LRESULT CALLBACK WFunc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HDC hdc;
	PAINTSTRUCT ps;
	static HPEN hPen1;
	static HBRUSH AppleBrush, SnakeBrush, ThreeBrush;
	static int sx, sy, x, y, appleCount, threeCount, snakeMove, segments = 4, appleTimer = 5000, threeTimer = 8000, snakeTimer = 400;
	static vector <int> AppleVecX, AppleVecY, SnakeVecX, SnakeVecY, ThreeVecX, ThreeVecY;
	static bool flag;
	switch (message)
	{
	case WM_SIZE:
		//размеры поля
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		break;
	case WM_CREATE:
		srand(time(0));
		hdc = GetDC(hWnd);
		//установка таймеров
		SetTimer(hWnd, TIMER_APPLE, appleTimer, 0);
		SetTimer(hWnd, TIMER_THREE, threeTimer, 0);
		//кисти + условный цвет для границ сегментов
		AppleBrush = CreateSolidBrush(RGB(255, 0, 55));
		SnakeBrush = CreateSolidBrush(RGB(0, 255, 55));
		ThreeBrush = CreateSolidBrush(RGB(139, 69, 19));
		hPen1 = CreatePen(PS_SOLID, 1, RGB(100, 100, 255));
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case TIMER_APPLE:
			appleCount++;
			break;
		case TIMER_THREE:
			threeCount++;
			break;
		}
		InvalidateRect(hWnd, 0, 1);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//разбиение на тайлы
		Notebook(hdc, hPen1, sx, sy);
		//генерация яблок
		AppleGeneration(hdc, AppleBrush, AppleVecX, AppleVecY, appleCount, sx, sy);
		//генерация деревьев
		ThreeGeneration(hdc, ThreeBrush, ThreeVecX, ThreeVecY, threeCount, sx, sy);
		//генерация змеи
		if (!flag)
		{
			SnakeGeneration(hdc, SnakeBrush, SnakeVecX, SnakeVecY, segments);
			flag = !flag;
		}
		if (IsSnakeAlive(hWnd, SnakeVecX, SnakeVecY, ThreeVecX, ThreeVecY, sx, sy, threeCount, segments))
			SnakeMoveBody(hdc, SnakeBrush, SnakeVecX, SnakeVecY, segments, snakeMove);
		SnakeGrow(hdc, SnakeBrush, SnakeVecX, SnakeVecY, AppleVecX, AppleVecY, segments, appleCount);
		Harder(appleTimer, threeTimer, snakeTimer, segments, hWnd);
		EndPaint(hWnd, &ps);
		break;
	case WM_HOTKEY:
		switch (LOWORD(wParam))
		{
			//змей влево
		case 1:
			snakeMove = 1;
			SetTimer(hWnd, TIMER_SNAKE, snakeTimer, 0);
			break;
			//змей ввверх
		case 2:
			snakeMove = 2;
			SetTimer(hWnd, TIMER_SNAKE, snakeTimer, 0);
			break;
			//змей вправо
		case 3:
			snakeMove = 3;
			SetTimer(hWnd, TIMER_SNAKE, snakeTimer, 0);
			break;
			//змей вниз
		case 4:
			snakeMove = 4;
			SetTimer(hWnd, TIMER_SNAKE, snakeTimer, 0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0); // Корректная обработка окончания сообщения
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam); // Возврат неотработанного сообщения
	}
	return 0L;
}

void SnakeMoveBody(HDC hdc, HBRUSH SnakeBrush, vector<int>& SnakeVecX, vector<int>& SnakeVecY, int segments, int snakemove)
{
	SelectObject(hdc, SnakeBrush);
	int n;
xyz:switch (snakemove)
	{
	case 1:    //влево
		if (SnakeVecX[segments - 1] == SnakeVecX[segments - 2] + 20 &&
			SnakeVecY[segments - 1] == SnakeVecY[segments - 2])
		{
			snakemove = 0;
			goto xyz;
		}
		for (n = 0; n < segments; n++)
		{
			if (n == segments - 1)
				SnakeVecX[n] -= 20;
			else
			{
				SnakeVecX[n] = SnakeVecX[n + 1];
				SnakeVecY[n] = SnakeVecY[n + 1];
			}
		}
		break;
	case 2:		//вверх
		if (SnakeVecX[segments - 1] == SnakeVecX[segments - 2] &&
			SnakeVecY[segments - 1] == SnakeVecY[segments - 2] + 20)
		{
			snakemove = 0;
			goto xyz;
		}
		for (n = 0; n < segments; n++)
		{
			if (n == segments - 1)
				SnakeVecY[n] -= 20;
			else
			{
				SnakeVecY[n] = SnakeVecY[n + 1];
				SnakeVecX[n] = SnakeVecX[n + 1];
			}
		}
		break;
	case 3:		//вправо
		if (SnakeVecX[segments - 1] == SnakeVecX[segments - 2] - 20 &&
			SnakeVecY[segments - 1] == SnakeVecY[segments - 2])
		{
			snakemove = 0;
			goto xyz;
		}
		for (n = 0; n < segments; n++)
		{
			if (n == segments - 1)
				SnakeVecX[n] += 20;
			else
			{
				SnakeVecX[n] = SnakeVecX[n + 1];
				SnakeVecY[n] = SnakeVecY[n + 1];
			}
		}
		break;
	case 4:		//вниз
		if (SnakeVecX[segments - 1] == SnakeVecY[segments - 2] &&
			SnakeVecY[segments - 1] == SnakeVecY[segments - 2] - 20)
		{
			snakemove = 0;
			goto xyz;
		}
		for (n = 0; n < segments; n++)
		{
			if (n == segments - 1)
				SnakeVecY[n] += 20;
			else
			{
				SnakeVecY[n] = SnakeVecY[n + 1];
				SnakeVecX[n] = SnakeVecX[n + 1];
			}
		}
		break;
	}
	for (n = 0; n < segments; n++)
			Ellipse(hdc, SnakeVecX[n], SnakeVecY[n], SnakeVecX[n] + 20, SnakeVecY[n] + 20);
}

void Notebook(HDC hdc, HPEN hPen1, int wx, int wy)
{
	int a = 0;
	while (a < wx)
	{
		SelectObject(hdc, hPen1);
		a += 20;
		MoveToEx(hdc, a, 0, 0);
		LineTo(hdc, a, wy);
	}
	a = 0;
	while (a < wy)
	{
		SelectObject(hdc, hPen1);
		a += 20;
		MoveToEx(hdc, 0, a, 0);
		LineTo(hdc, wx, a);
	}
	a = 0;
}

void AppleGeneration(HDC hdc, HBRUSH AppleBrush, vector<int>& AppleVecX, vector<int>& AppleVecY, int applecount, int sx, int sy)
{
	int x, y, n;
	x = (rand() % sx + 10) / 20 * 20;
	y = (rand() % sy + 10) / 20 * 20;
	AppleVecX.push_back(x);
	AppleVecY.push_back(y);
	SelectObject(hdc, AppleBrush);
	for (n = 0; n < applecount; n++)
		Ellipse(hdc, AppleVecX[n], AppleVecY[n], AppleVecX[n] + 20, AppleVecY[n] + 20);
}

void ThreeGeneration(HDC hdc, HBRUSH ThreeBrush, vector<int>& ThreeVecX, vector<int>& ThreeVecY, int threecount, int sx, int sy)
{
	int x, y, n;
	x = (rand() % sx + 10) / 20 * 20;
	y = (rand() % sy + 10) / 20 * 20;
	ThreeVecX.push_back(x);
	ThreeVecY.push_back(y);
	SelectObject(hdc, ThreeBrush);
	for (n = 0; n < threecount; n++)
		Ellipse(hdc, ThreeVecX[n], ThreeVecY[n], ThreeVecX[n] + 40, ThreeVecY[n] + 40);
}

void SnakeGeneration(HDC hdc, HBRUSH SnakeBrush, vector<int>& SnakeVecX, vector<int>& SnakeVecY, int segments)
{
	int i = 200, j = 200, n;
	for (n = 0; n < segments; n++)
	{
		SnakeVecX.push_back(i);
		SnakeVecY.push_back(j);
		j += 20;
	}
	SelectObject(hdc, SnakeBrush);
	for (n = 0; n < segments; n++)
		Ellipse(hdc, SnakeVecX[n], SnakeVecY[n], SnakeVecX[n] + 20, SnakeVecY[n] + 20);
	
}

void SnakeGrow(HDC hdc, HBRUSH SnakeBrush, vector<int>& SnakeVecX, vector<int>& SnakeVecY, vector<int>& AppleVecX, vector<int>& AppleVecY, int& segments, int& applecount)
{
	int m, n = segments - 1;
	for (m = 0; m <= applecount; m++)
	{
		if (SnakeVecX[n] == AppleVecX[m] && SnakeVecY[n] == AppleVecY[m])
		{
			segments += 2;
			for (n = 0; n < segments-1; n++)
			{
				if (SnakeVecX[n] == AppleVecX[m] && SnakeVecY[n] == AppleVecY[m])
				{
					SnakeVecX.push_back(AppleVecX[m]);
					SnakeVecY.push_back(AppleVecY[m]);
				}
			}
			applecount--;
			AppleVecX.erase(AppleVecX.begin() + m);
			AppleVecY.erase(AppleVecY.begin() + m);
		}
	}
	for (n = 0; n < segments; n++)
		Ellipse(hdc, SnakeVecX[n], SnakeVecY[n], SnakeVecX[n] + 20, SnakeVecY[n] + 20);
}

bool IsSnakeAlive(HWND hWnd, vector<int> SnakeVecX, vector<int> SnakeVecY, vector<int> ThreeVecX, vector<int>ThreeVecY, int sx, int sy, int threecount, int segments)
{
	int n;
	for (n = 0; n < segments - 3; n++)
	{
		if (SnakeVecX[segments - 1] == SnakeVecX[n] && SnakeVecY[segments - 1] == SnakeVecY[n])
		{
			KillTimer(hWnd, TIMER_SNAKE);
			KillTimer(hWnd, TIMER_APPLE);
			KillTimer(hWnd, TIMER_THREE);
			MessageBox(0, "The snake ate its tail!", "Game over", MB_ICONERROR | MB_OK);
			return false;
		}
	}
	//
	if (SnakeVecX[segments - 1] == -20 || SnakeVecX[segments - 1] == sx||
		SnakeVecY[segments - 1] == -20 || SnakeVecY[segments - 1] == sy)
	{
		KillTimer(hWnd, TIMER_SNAKE);
		KillTimer(hWnd, TIMER_APPLE);
		KillTimer(hWnd, TIMER_THREE);
		MessageBox(0, "The snake got lost!", "Game over", MB_ICONERROR | MB_OK);
		return false;
	}
	//
	for (n = 0; n <= threecount; n++)
	{
		if ((SnakeVecX[segments - 1] == ThreeVecX[n] && SnakeVecY[segments - 1] == ThreeVecY[n]) ||
			(SnakeVecX[segments - 1] == ThreeVecX[n] + 20 && SnakeVecY[segments - 1] == ThreeVecY[n]) ||
			(SnakeVecX[segments - 1] == ThreeVecX[n] && SnakeVecY[segments - 1] == ThreeVecY[n] + 20) ||
			(SnakeVecX[segments - 1] == ThreeVecX[n] + 20 && SnakeVecY[segments - 1] == ThreeVecY[n] + 20))
		{
			KillTimer(hWnd, TIMER_SNAKE);
			KillTimer(hWnd, TIMER_APPLE);
			KillTimer(hWnd, TIMER_THREE);
			MessageBox(0, "The snake crashed into a tree!", "Game over", MB_ICONERROR | MB_OK);
			return false;
		}
	}
	return true;
}

void Harder(int& appleTimer, int& threeTimer, int& snakeTimer, int segments, HWND hWnd)
{
	static int count = 10;
	if (segments == count && count < 50)
	{
		appleTimer += 100;
		threeTimer -= 250;
		snakeTimer -= 50;
		count += 10;
	}
	if (segments == count && count < 90)
	{
		appleTimer += 100;
		threeTimer -= 350;
		snakeTimer -= 25;
		count += 10;
	}
	if (segments == count && count >= 90)
	{
		appleTimer += 100;
		threeTimer -= 450;
		snakeTimer -= 10;
		count += 10;
	}
	sprintf(str, "%d", snakeTimer);
	SetWindowText(hWnd, str);
}
