/*
Ляпина Наталья Викторовна ИУ7-32Б
Задание №1
Вариант №1

Задача:
Смоделировать операцию деления действительного числа в
форме +-m.n Е +-K, где суммарная длина мантиссы (m+n) - до
30 значащих цифр, а величина порядка K - до 5 цифр, на
целое число длиной до 30 десятичных цифр. Результат
выдать в форме +-0.m1 Е +-K1, где m1 - до 30 значащих цифр,
а K1 - до 5 цифр.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>

#define NMAX 32
#define SRMAX 41
#define SIMAX 32
#define MAX_EXP 99999

#define OK 0
#define ERROR_LEN 1
#define ERROR_SYMBOLS 2
#define ERROR_MANT_LEN 3
#define ERROR_ZERO_FIRST 4
#define ERROR_NO_POINT 5
#define ERROR_ORDER 6
#define ERROR_DIVISION 7

typedef char real_t[SRMAX];
typedef char int_t[SIMAX];
typedef int man_t[NMAX];

typedef struct
{
    char num_sign;
    man_t mantissa;
    char exp_sign;
    int order;
} num_t;

//Функция считывает в структуру действительного числа данные из строки
//ввода и в случае неправильного ввода сообщает об ошибке
int is_real(real_t string, num_t *real, size_t *man_r)
{
    int i = 0;
    int flag = 1;
    long num;
    long buf = 0;
    long exp = 0;

    if (string[i] == '-')
    {
        real->num_sign = '-';
        i++;
    }
    else if (string[i] == '+')
        i++;
    
    //Проверка мантисы до точки
    while (string[i] != '.' && string[i] != '\0' && string[i] != 'E')
    {
        // Символ является чилом
        if (isdigit((int)string[i]) != 0)
        {
            //Получение числа из символа
            num = string[i] - '0';
            if (num > 0)
                flag = 0;
            //Проверка на переполнение мантисы
            if (*man_r == NMAX - 2)
            {
                printf("\nОШИБКА:\nКоличество цифр мантиссы превышает допустимое.\nМаксимальное количество цифр в мантиссе: 30.");
                return ERROR_MANT_LEN;
            }
            //Проверка на нули перед цифрой 01.1
            else if (num == 0 && string[i + 1] != '.' && i == 0)
            {
                printf("\nОШИБКА:\nДействительное число введено неправильно.");
                return ERROR_ZERO_FIRST;
            }
            //Добавление цифры в мантису
            else if(num >= 0 || flag == 0)
            {
                real->mantissa[*man_r] = num;
                *man_r = *man_r + 1;
                buf = buf + 1;
            }
        }
        else
        {
            printf("\nОШИБКА:\nДействительное число введено неправильно (Использованы недопустимые символы).");
            return ERROR_SYMBOLS;
        }
        i++;
    }
    //Проверка на точку
    if (string[i] == '\0')
    {
        printf("\nОШИБКА:\nНеправильный ввод действительного числа.\nДействительное число должно вводиться с точкой.");
        return ERROR_NO_POINT;
    }
    if (string[i] == '.')
        i++;

    //Проверка мантисы после точки
    while (string[i] != 'E' && string[i] != '\0')
    {
        // Является ли цифрой
        if (isdigit((int)string[i]) != 0)
        {
            num = string[i] - '0';
            // Переполнение мантисы
            if (*man_r == NMAX - 2)
            {
                printf("\nОШИБКА:\nКоличество цифр мантиссы превышает допустимое.\nМаксимальное количество цифр в мантиссе: 30.");
                return ERROR_MANT_LEN;
            }
            // Добавление цифры в мантису
            else
            {
                real->mantissa[*man_r] = num;
                *man_r = *man_r + 1;
            }
        }
        else
        {
            printf("\nОШИБКА:\nДействительное число введено неправильно.");
            return ERROR_SYMBOLS;
        }
        i++;
    }
    // целая часть мантисы состоит из 0
    if (flag == 1)
    {
        buf = 0;
        real->order = 0;
    }
    
    //Порядок не указан
    if (string[i] != 'E')
    {
        real->order += buf;
        return OK;
    }

    i++;
    //Знак порядка
    if (string[i] == '-')
    {
        real->exp_sign = '-';
        i++;
    }
    else if (string[i] == '+')
        i++;
    
    //Проверка порядка
    while (string[i] != '\0')
    {
        if (isdigit((int)string[i]) != 0)
        {
            num = string[i] - '0';
            exp = exp *10 + num;
        }
        else
        {
            printf("\nОШИБКА:\nНеправильный порядок действительного числа.");
            return ERROR_ORDER;
        }
        i++;
    }
    //Нет порядка после знака
    if (string[i] == '\0' && isdigit((int)string[i - 1]) == 0)
    {
        printf("\nОШИБКА:\nНеправильный порядок действительного числа.");
        return ERROR_ORDER;
    }
    // Запись порядка в структуру
    if (buf == 0)
        real->exp_sign = '-';

    if (real->exp_sign == '-')
        real->order = real->order - exp;
    else
        real->order = real->order + exp;
    // Проверка переполнения порядка
    if (abs(real->order) > MAX_EXP)
    {
        printf("\nОШИБКА:\nКоличество разрядов порядка действительного числа превышает допустимое.\nМаксимальное количество разрядов в порядке: 5");
        return ERROR_ORDER;
    }
    else
        real->order += buf;
        
    return OK;
}

//Функция проверки ввода действительного числа
int read_real(real_t s)
{
    int ch;
    int i = 0;
    while ((ch = getchar()) != '\n' && ch != EOF)
    {
        if (i < SRMAX)
            s[i++] = ch;
        else
        {
            printf("\nОШИБКА:\nВведенные данные действительного числа слишком длинные.");
            return ERROR_LEN;
        }
    }
    s[i] = '\0';
    return OK;
}
//Функция проверки ввода целого числа
int read_int(int_t s)
{
    int ch;
    int i = 0;
    while ((ch = getchar()) != '\n' && ch != EOF)
    {
        if (i < SIMAX)
            s[i++] = ch;
        else
        {
            printf("\nОШИБКА:\nВведенные данные целого числа слишком длинные.");
            return ERROR_LEN;
        }
    }
    s[i] = '\0';
    return OK;
}

//Функция считывает в структуру целого числа данные из строки
//ввода и в случае неправильного ввода сообщает об ошибке
int is_integer(int_t string, num_t *integer, size_t *man_i)
{
    int i = 0;
    //Проверка на переполнение
    if (strlen(string) > SIMAX - 1)
    {
        printf("\nОШИБКА:\nКоличество цифр целого числа превышает допустимое.\nМаксимальное количество цифр в числе: 30.");
        return ERROR_LEN;
    }
    //Знак числа
    if (string[i] == '-')
    {
        integer->num_sign = '-';
        i++;
    }
    else if (string[i] == '+')
        i++;
    //Порядок числа
    integer->order = strlen(string) - i;

    while (string[i] != '\0')
    {
        //Проверка символов числа
        if (isdigit((int)string[i]) == 0)
        {
            printf("\nОШИБКА:\nЦелое число введено неправильно (Использованы недопустимые символы).");
            return ERROR_SYMBOLS;
        }
        else
        {
            integer->mantissa[*man_i] = string[i] - '0';
            *man_i = *man_i + 1;
        }
        i++;
    }
    return OK;
}

//Функция присваивает нули всем элементам массива структуры
void zeros_in_massive(num_t *num)
{
  for (int i = 0; i < NMAX; i++)
  num->mantissa[i] = 0;
}

//Удаление лидирующих нулей массива
int normalization(num_t *data, size_t *n)
{
    int count = 0;
    while (data->mantissa[0] == 0 && *n > 0)
    {
        for (size_t i = 1; i < *n; i++)
            data->mantissa[i - 1] = data->mantissa[i];
        *n = *n - 1;
        if (count != 0)
            data->order += 1;
        data->mantissa[*n] = 0;
        count += 1;
    }
    return OK;
}

//Функция сдвигает число из начала в конец мантиссы
int move_num_end(num_t *data, size_t *n)
{
    size_t i = NMAX - 1;
    size_t j = *n;
    
    while (j > 0)
    {
        data->mantissa[i] = data->mantissa[j - 1];
        data->mantissa[j - 1] = 0;
        i--;
        j--;
    }
    return OK;
}

//функция двигает данные массива относительно его конца для выравнивания порядков
//и для того, чтобы было возможно произвести операцию вычитания
void move_digits(num_t *num, int size, int offset)
{
    if (offset <= -1)
    {
        offset *= -1;
        for (int i = 0; i < size - offset; i++)
            num->mantissa[i] = num->mantissa[i+offset];
        for (int i = size - offset; i < size; i++)
            num->mantissa[i] = 0;
    }
    else if (offset >= 1)
    {
        for (int i = size - 1; i >= offset; i--)
            num->mantissa[i] = num->mantissa[i-offset];
        for (int i = offset - 1; i >= 0; i--)
            num->mantissa[i] = 0;
    }
}

//Функция поразрядного сравнения чисел
int comparison(num_t *real, num_t *integer)
{
    int i = 0;
    int res = 0;
    while (res == 0 && i < NMAX)
    {
        res = real->mantissa[i] - integer->mantissa[i];
        i++;
    }
    return res;
}

//Функция округления при нахождении 31 цифры мантиссы
void rounding(num_t *result, size_t size, int last_digit)
{
    if (last_digit >= 5)
    {
        result->mantissa[size - 1]++;
        for (int i = size - 1; i >= 0; i--)
        {
            if (result->mantissa[i] == 10)
            {
                result->mantissa[i] -= 10;
                result->mantissa[i - 1]++;
            }
            else
                break;
        }
    }
}

//Функция пораздрядного вычетания цифр массивов
void subtract(num_t *real, num_t *integer, int size)
{
    for (int i = size - 1; i >= 0; i--)
    {
        if (real->mantissa[i] >= integer->mantissa[i])
            real->mantissa[i] -= integer->mantissa[i];
        else
        {
            real->mantissa[i] += 10 - integer->mantissa[i];
            real->mantissa[i-1]--;
        }
    }
}

//Функция, обЪединяющая в себе логику деления
int division(num_t *real, num_t *integer, num_t *result, int size)
{
    size_t current_pos = 0;
    int equal, flag;
    int counter;
    int flag_zero = 1, beggining_flag = 1;

    while (current_pos < NMAX)
    {
        //Разность между разрядами мантисы
        equal = comparison(real, integer);
        //Действительное и целое число равны
        if (equal == 0)
        {
            result->mantissa[current_pos] = 1;
            break;
        }
        else if (equal > 0)
        {
            flag_zero = 0;
            counter = 0;
            //Пока не найдется одинаковых разрядов
            while ((flag = comparison(real, integer)) >= 0)
            {
                if (flag == 0)
                {
                    counter++;
                    break;
                }
                //Вычитание разрядов
                subtract(real, integer, size);
                counter++;
            }
            //Округление при переполнении мантисы
            if (current_pos == NMAX - 2)
            {
                rounding(result, NMAX - 2, counter);
                break;
            }
            //Запись в мантису результата
            else
            {
                result->mantissa[current_pos++] = counter;
                beggining_flag = 0;
                if (flag == 0)
                    break;
            }
        }
        else
        {
            //Разряд мантисы меньше разряда целого
            if (flag_zero)
            {
                if (beggining_flag)
                    result->order--;
                else
                    result->mantissa[current_pos++] = 0;
            }
            flag_zero = 1;
            move_digits(real, size, -1);
        }
    }
    return OK;
}

int main(void)
{
    //"Зануление" всех переменных
    real_t real_s = "12E5";
    int_t integer_s;// = "-125";
    size_t man_r = 0, man_i = 0, man_res = 0;
    num_t real = {.exp_sign = '+', .num_sign = '+', .order = 0};
    num_t integer = {.exp_sign = '+', .num_sign = '+'};
    num_t result = {.exp_sign = '+', .num_sign = '+', .order = 0};
    zeros_in_massive(&real);
    zeros_in_massive(&integer);

    //Отмена буферизации для корректной работы printf
    //Проверка действительного числа
    if (is_real(real_s, &real, &man_r) != OK)
        return ERROR_MANT_LEN;
    printf("\nВведите целое");
    if (read_int(integer_s) != OK)
        return ERROR_LEN;
    //Проверка целого числа
    if (is_integer(integer_s, &integer, &man_i) != OK)
        return ERROR_SYMBOLS;
    
    //Нормализация целого и десятичного чисел
    normalization(&real, &man_r);
    normalization(&integer, &man_i);
    
    //Одно из чисел равно 0
    if (real.mantissa[0] == 0)
    {
        if (integer.mantissa[0] == 0)
        {
            printf("\nОШИБКА:\nРезультат не может быть получен (деление на ноль).");
            return ERROR_ZERO_FIRST;
        }
        else
        {
            printf("\n\nОтвет: 0.0E0");
            return OK;
        }
    }
    if (integer.mantissa[0] == 0)
    {
        printf("\nОШИБКА:\nРезультат не может быть получен (деление на ноль).");
        return ERROR_ZERO_FIRST;
    }
    
    //Сдвигаем числа в конец мантиссы
    move_num_end(&integer, &man_i);
    move_num_end(&real, &man_r);

    //Порядок результата деления
    result.order += (real.order - integer.order + 1);

    //Сдвиг относительно порядков чисел
    size_t diff = man_r - man_i;
    if (diff > 1)
        move_digits(&real, NMAX, diff);

    //Проверка на переполнение
    if (abs(result.order) > MAX_EXP)
    {
        printf("\nОШИБКА:\nВыход за пределы допустимого порядка результирующего числа.");
        return ERROR_ORDER;
    }
    //Получение результирующего знака
    if (result.order > 0)
        result.exp_sign = '+';
    else
        result.exp_sign = '-';
    
    if (real.num_sign == integer.num_sign)
        result.num_sign = '+';
    else
        result.num_sign = '-';

    //Деление чисел
    if (division(&real, &integer, &result, NMAX) == 0)
    {
        //Размер мантисы результата деления
        for (size_t k = 0; k < NMAX; k++)
        {
            if (result.mantissa[k])
                man_res = k + 1;
        }
        
        if (result.mantissa[man_res] != 0)
            man_res++;
        //Печать результата деления
        for (size_t k = 0; k < man_res; k++)
            ;
    }
    else
        return ERROR_DIVISION;
    
    return OK;
}
