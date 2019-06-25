/**
 * @file:   TP1PractEx.c
 * @author: Даншин Антон П., гр. 515б
 * @date    19.06.2019
 * @brief   Практика
 *
 * Поворот точки на определённый угол.
**/

//Использование математической константы M_PI из <math.h>.
#define _USE_MATH_DEFINES
//Использование функции scanf().
#define _CRT_SECURE_NO_WARNINGS

//Подключение заголовочных файлов.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>

//Структура для точек на плоскости.
typedef struct point 
{
	float position_X; // X - координата точки.
	float position_Y; // Y - координата точки.
} point;

/**
 * Отчистка потока ввода
 * @return 1, когда отчистка завершена */
int clean_stdin();


/**
  * Считывание координат точки
  * @param rp - указатель на точку, которую считывают из потока ввода
  * @return 1 - считывание прошло успешно и в строке есть ещё символы, 0 - считывание прошло успешно и строка закончена, -1 - в строке содержатся ошибки
  */
int scan_point(struct point * rp);

/**
  * Поворот точки на определённый угол
  * @param oldp - указатель на начальную точку
  * @param newp - указатель на конечную точку
  * @param rot - указатель на точку вращения
  * @param ang - угол поворота
  */
void rotating(struct point *oldp, struct point *newp, struct point *rot,float ang);

/**
  * Вычисление длинны целой части числа
  * @param num - число
  * @return кол-во цифр в числе
  */
int num_lenght(long num);

/**
  * Вычисление максимальной длинны целой части всех выводимых чисел
  * @param olds - указатель на массив из начальных точек
  * @param news - указатель на массив из конечных точек
  * @param number - кол-во точек
  * @return кол-во цифр в числе
  */
int find_max_lenght(struct point **olds, struct point **news, int number);

int main()
{
	//Подключение русского языка.
	setlocale(LC_CTYPE, "rus");
	
	struct point **start_points = malloc(sizeof(struct point *)); //Динамический массив, хранящий начальные точки.

	//Ввод начальных координат точек.
	printf(" Введите координаты точек.\n"
		"  * Х - и Y - координаты должны быть разделены \":\"\n"
		"  * Координаты разных точек должны быть разделенны пробелами:\n ");

	//Считывание точек
	int controle; //Переменная для проверки считывания.
	int count = 0; //Переменная для подсчёта кол-ва введённых точек.
	do {
		controle = 1;
		while (controle == 1)
		{
			start_points = realloc(start_points, (count + 1) * sizeof(struct point *));
			start_points[count] = malloc(sizeof(struct point));
			controle = scan_point(start_points[count]);
			count++;
		}
		//В случае, если вводимая трока содержит ошибку.
		if (controle == -1) 
		{
			printf(" Внимание! Строка содержит ошибку! Данные не сохранены. Введите координаты точек: \n ");
			//Отчистка потока ввода.
			clean_stdin();
			//Освобождение памяти, выделенной для точек.
			while (count) 
			{
				free(start_points[--count]);
			}
			//Изменять кол-во выделенной памяти под массив start_points нет смысла, 
			//так как это произойдёт на следующей итерации цикла (т.к. count == 0).
		}
	} while(controle == -1); //Сообщение выводится, пока не будет введена корректная строка.

	float rotate_angle; //Переменная для хранения угла поворота.
	char c;
	//Считывание угла поворота.
	do {
		printf(" Введите угол поворота(в градусах): ");
	} while ((scanf("%f%c", &rotate_angle, &c) != 2 || c != '\n') && clean_stdin());

	struct point *rot_point = malloc(sizeof(struct point)); //Переменная для хранение точки вращения.
	//Считывание точки вращения.
	do {
		printf(" Введите координаты точки, относительно которой происходит вращение: ");
		controle = scan_point(rot_point);
	} while (controle != 0 && clean_stdin()); //Сообщение выводится, пока не будет введена корректная строка.

	//Вычисление координат новых точек.
	struct point **rotated_points = malloc(count * sizeof(struct point *)); //Динамический массив, хранящий соответсвующие повёрнутые точки.
	for (int i = 0; i < count; i++) 
	{
		rotated_points[i] = malloc(sizeof(struct point));
		rotating(start_points[i], rotated_points[i], rot_point, rotate_angle);
	}

	//Вывод результатов.
	
	int max_lint = find_max_lenght(start_points, rotated_points, count);

	//Создание массива пробелов для выравнивания текста внутри таблицы
	char * mass_space = malloc(max_lint+1);
	for (int i = 0; i <= max_lint - 1; mass_space[i++] = ' ');
	mass_space[max_lint - 0] = '\0';

	//Создание массива штрихов для создания контуров таблицы
	char * mass_hatch = malloc(max_lint +  11);
	for (int i = 0; i <= max_lint + 9; mass_hatch[i++] = '-');
	mass_hatch[max_lint + 10] = '\0';
	printf(" +---------+%s+%s+\n", mass_hatch, mass_hatch);
	mass_space[max_lint - 2] = '\0';
	printf(" |  Точки  |  %sНачальные |   %sКонечные |\n", mass_space, mass_space);
	mass_space[max_lint - 2] = ' ';
	printf(" +---------+%s+%s+\n", mass_hatch, mass_hatch);

	for(int i = 0; i < count; i++)
	{
		printf(" | %3i | X | ", i + 1);
		mass_space[max_lint - num_lenght((int)start_points[i]->position_X)] = '\0';
		printf("%s% f", mass_space, start_points[i]->position_X);
		mass_space[max_lint - num_lenght((int)start_points[i]->position_X)] = ' ';
		printf(" | ");

		mass_space[max_lint - num_lenght((int)rotated_points[i]->position_X)] = '\0';
		printf("%s% f", mass_space, rotated_points[i]->position_X);
		mass_space[max_lint - num_lenght((int)rotated_points[i]->position_X)] = ' ';
		printf(" |\n");

		printf(" |     | Y | ", i);
		mass_space[max_lint - num_lenght((int)start_points[i]->position_Y)] = '\0';
		printf("%s% f", mass_space, start_points[i]->position_Y);
		mass_space[max_lint - num_lenght((int)start_points[i]->position_Y, 1)] = ' ';
		printf(" | ");
		mass_space[max_lint - num_lenght((int)rotated_points[i]->position_Y, 1)] = '\0';
		printf("%s% f", mass_space, rotated_points[i]->position_Y);
		mass_space[max_lint - num_lenght((int)rotated_points[i]->position_Y, 1)] = ' ';
		printf(" |\n");
		printf(" +-----+---+%s+%s+\n", mass_hatch, mass_hatch);
	}

	system("pause");
	return 0;
}

int clean_stdin()
{
	while (getchar() != '\n');
	return 1;
}

int scan_point(struct point * rp)
{
	char doublepoint; //Переменная, в которой хранится разделяющий символ между координатами. Используется для проверки.
	char endch; //Переменная, которая хранит символ, следующий аосле координат точки. Используется для проверки.
	char controle = scanf("%f%c%f%c", &(rp->position_X), &doublepoint, &(rp->position_Y), &endch);
	if ((controle != 4) || (doublepoint != ':')|| (endch != ' '&& endch != '\n')) 
	{ //В случае, если в строке присутсвует ошибка ввода.
		return -1;
	}
	else if (endch == '\n') 
	{//В случае, если были считаны координаты последней точки.
		return 0;
	}
	else 
	{//В случае, если координаты точки были считаны корректно и точка не является последней.
		return 1;
	}
}


void rotating(struct point *oldp, struct point *newp, struct point *rot, float ang) 
{
	//Вычисление координат точек.
	newp->position_X = (oldp->position_X - rot->position_X)*cos(M_PI*ang / 180.0) - (oldp->position_Y - rot->position_Y)*sin(M_PI*ang / 180.0) + rot->position_X;
	newp->position_Y = (oldp->position_X - rot->position_X)*sin(M_PI*ang / 180.0) + (oldp->position_Y - rot->position_Y)*cos(M_PI*ang / 180.0) + rot->position_Y;
}
int num_lenght(long num) 
{
	int len = 1; //Переменная для хранения кол-ва цыфр в числе.
	for (; num /= 10; len++);
	return len;
}
int find_max_lenght(struct point **olds, struct point **news, int number)
{
	int max_int = 2;
	while (number-- > 0)
	{
		//Целая часть Х, начальная точка
		max_int = num_lenght((int)olds[number]->position_X) > max_int ? num_lenght((int)olds[number]->position_X) : max_int;
		//Целая часть Y, начальная точка
		max_int = num_lenght((int)olds[number]->position_Y) > max_int ? num_lenght((int)olds[number]->position_Y) : max_int;
		//Целая часть X, конечная точка
		max_int = num_lenght((int)news[number]->position_X) > max_int ? num_lenght((int)news[number]->position_X) : max_int;
		//Целая часть Y, конечная точка
		max_int = num_lenght((int)news[number]->position_Y) > max_int ? num_lenght((int)news[number]->position_Y) : max_int;
	}
	return max_int;
}
