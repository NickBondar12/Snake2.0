// Sceleton.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Sceleton.h" // Здесь подключается resource.h

char name[] = "Qwerty"; // Необходимо для регистрации окна

// Указатель на нижнюю границу загрузки приложения
HINSTANCE hInst; // Указатель, нижняя граница, с которой он не грузится в память

int APIENTRY WinMain(HINSTANCE hInstance, // Хендлер приложения
					 HINSTANCE, // Создание копии окна (не задаваемый параметр)
					 LPSTR     lpCmdLine, // Командная строка
					 int       nCmdShow) // Вид окна
{
	hInst = hInstance; // Создание копии хендлера
	// 1. Создание внешнего вида окна
	WNDCLASS win; // Объект для информации об окне
	win.style = CS_HREDRAW | CS_VREDRAW; // Стили перерисовки
	win.lpfnWndProc = WFunc; // Имя (параметр) оконной функции, должно соответствовать имени функции 
	win.cbClsExtra = win.cbWndExtra = 0; // Всегда = 0
	win.hInstance = hInstance; // Имя приложения, к которому привязано окно
	win.hIcon = LoadIcon(NULL, IDI_ASTERISK); // Иконка, идентификатор иконки ID_APPLIKATION
	win.hCursor = LoadCursor(NULL, IDC_ARROW); // Курсор
	win.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // Цвет фона
	win.lpszMenuName = NULL; // Меню отсутствует
	win.lpszClassName = name; // Имя для регистрации = qwerty

	// 2. Регистрация окна в операционной системе
	if (!RegisterClass(&win))
	{
		MessageBox(NULL, "Окно не зарегистрировано", "Ошибка", MB_OK);
		return -1; // Если окно не зарегистрировано
	}

	// 3. Создание окна
	HWND hWnd; // Объявление хендлера окна
	DWORD style = WS_CAPTION | WS_SYSMENU;
	hWnd = CreateWindow(name, "", style,
		50, 50, 456, 458, HWND_DESKTOP, NULL, hInstance, NULL); // Создание хендлера окна

	// HWND_DESKTOP - единственный хендлер рабочего стола

	// 4*. Отображение окна
	ShowWindow(hWnd, nCmdShow); // nCmdShow - для показа окна
	UpdateWindow(hWnd); // Обновить сразу окно - перерисовка

	//5. Регистрация клавиш для перемещения змейки
	RegisterHotKey(hWnd, 1, 0, 'A');
	RegisterHotKey(hWnd, 2, 0, 'W');
	RegisterHotKey(hWnd, 3, 0, 'D');
	RegisterHotKey(hWnd, 4, 0, 'S');

	// 6. Создание насоса сообщений
	MSG msg; // Описание переменной сообщения
	while (GetMessage(&msg, 0, 0, 0)) // Можно ограничить сообщения
	{
		TranslateMessage(&msg); // Разрешает использовать клавиатуру
		DispatchMessage(&msg); // Отправка обработанного сообщения в ОС
	}

	//7. Привязка клавиш к окну 
	UnregisterHotKey(hWnd, 1);
	UnregisterHotKey(hWnd, 2);
	UnregisterHotKey(hWnd, 3);
	UnregisterHotKey(hWnd, 4);

	return msg.wParam; // Корректное закрытие основной программы
}
