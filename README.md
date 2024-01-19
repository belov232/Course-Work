#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#define MAX_SIZE 100

struct GameRecord
{
    char title[50];
    int rating;
    char review[100];
    float score;
    char version[20];
    char genre[10];
};

int searchByTitle(const char* searchTitle);
int searchByRating(int maxRating);
void sortByScore(struct GameRecord records[], int size);
int writeToFile(struct GameRecord records[], int size);
int readFromFile(struct GameRecord records[], int* size);
int addRecord(struct GameRecord records[], int* size);

int main()

{
    struct GameRecord records[MAX_SIZE];
    int size = 0;
    int choice;
    setlocale(LC_ALL, "RUS");
    system("chcp 1251");

    do
    {
        printf("Меню:\n");
        printf("1. Добавить запись\n");
        printf("2. Поиск по названию\n");
        printf("3. Поиск по рейтингу\n");
        printf("4. Сортировка по средней оценке по отзыву\n");
        printf("5. Записать в файл\n");
        printf("6. Прочитать из файла\n");
        printf("7. Выход из программы\n");
        printf("Выберите пункт меню: ");
        scanf("%d", &choice);

        getchar();

        switch (choice)
        {
        case 1:
            addRecord(records, &size);
            break;

        case 2:
            if (size > 0)
            {
                char searchTitle[50];
                printf("Введите название для поиска: ");
                fgets(searchTitle, sizeof(searchTitle), stdin);
                searchTitle[strcspn(searchTitle, "\n")] = 0;
                searchByTitle(searchTitle);
            }
            else
            {
                printf("База данных пуста.\n");
            }
            break;

        case 3:
            if (size > 0)
            {
                int maxRating;
                printf("Введите максимальный рейтинг для поиска: ");
                scanf("%d", &maxRating);
                searchByRating(maxRating);
            }
            else
            {
                printf("База данных пуста.\n");
            }
            break;

        case 4:
            if (size > 0)
            {
                sortByScore(records, size);

                printf("\n Отсортированные данные по средней оценке по отзыву (в порядке убывания):\n");

                for (int i = 0; i < size; i++)
                {
                    printf("Название: %s, Средняя оценка по отзыву: %.2f\n", records[i].title, records[i].score);
                }
            }
            else
            {
                printf("База данных пуста.\n");
            }
            break;

        case 5:
            if (size > 0)
            {
                writeToFile(records, size);
            }
            else
            {
                printf("База данных пуста.\n");
            }
            break;

        case 6:
            if (size > 0)
            {
                readFromFile(records, &size);
                printf("Данные успешно прочитаны из файла.\n");
            }
            else
            {
                printf("База данных пуста. Нечего читать из файла.\n");
            }
            break;

        case 7:
            printf("Программа завершена.\n");
            break;

        default:
            printf("Некорректный ввод. Пожалуйста, выберите существующий пункт меню.\n");
        }

    } while (choice != 7);

    return 0;
}

int addRecord(struct GameRecord records[], int* size)

{
    if (*size < MAX_SIZE)
    {
        printf("Введите данные новой игры:\n");
        printf("Название: ");
        fgets(records[*size].title, 50, stdin);
        records[*size].title[strcspn(records[*size].title, "\n")] = 0;

        printf("Рейтинг: ");
        scanf("%d", &records[*size].rating);

        while (getchar() != '\n');

        printf("Отзыв: ");
        fgets(records[*size].review, 100, stdin);
        records[*size].review[strcspn(records[*size].review, "\n")] = 0;

        printf("Средняя оценка по отзыву: ");
        scanf("%f", &records[*size].score);

        while (getchar() != '\n');

        printf("Версия: ");
        fgets(records[*size].version, 20, stdin);
        records[*size].version[strcspn(records[*size].version, "\n")] = 0;

        printf("Жанр: ");
        fgets(records[*size].genre, 10, stdin);
        records[*size].genre[strcspn(records[*size].genre, "\n")] = 0;

        return (*size)++;
    }
    else
    {
        printf("База данных полна. Невозможно добавить новую запись.\n");
        return -1;
    }
}

int searchByTitle(const char* searchTitle)

{
    FILE* file = fopen("games.txt", "r");

    if (file == NULL)
    {
        printf("Ошибка при открытии файла или файл не существует.\n");
        return 0;
    }

    int found = 0;
    char buffer[1000];

    printf("\nРезультаты поиска по названию \"%s\":\n", searchTitle);

    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        struct GameRecord record;
        sscanf(buffer, "Название: %49[^\n]", record.title);

        if (strstr(record.title, searchTitle) != NULL)
        {
            found = 1;
            printf("%s", buffer);
        }
    }

    fclose(file);

    if (!found)
    {
        printf("Игры с названием \"%s\" не найдено.\n", searchTitle);
        return -1;
    }
}

int searchByRating(int maxRating)

{
    FILE* file = fopen("games.txt", "r");

    if (file == NULL)
    {
        printf("Ошибка при открытии файла или файл не существует.\n");
        return 0;
    }

    int found = 0;
    char buffer[1000];

    printf("\nРезультаты поиска по рейтингу до %d:\n", maxRating);

    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        struct GameRecord record;
        sscanf(buffer, "Рейтинг: %d", &record.rating);

        if (record.rating <= maxRating)
        {
            found = 1;
            printf("%s", buffer);
        }
    }

    fclose(file);

    if (!found)
    {
        printf("Игры с рейтингом до %d не найдены.\n", maxRating);
        return -1;
    }
}

void sortByScore(struct GameRecord records[], int size)

{
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size - i - 1; j++)
        {
            if (records[j].score < records[j + 1].score)
            {
                struct GameRecord temp = records[j];
                records[j] = records[j + 1];
                records[j + 1] = temp;
            }
        }
    }
}

int writeToFile(struct GameRecord records[], int size)

{
    FILE* file = fopen("games.txt", "a");

    if (file == NULL)
    {
        printf("Не удалось открыть файл для записи.\n");
        return 0;
    }

    for (int i = 0; i < size; i++)
    {
        fprintf(file, "Название: %s\n", records[i].title);
        fprintf(file, "Рейтинг: %d\n", records[i].rating);
        fprintf(file, "Отзыв: %s\n", records[i].review);
        fprintf(file, "Средняя оценка по отзыву: %.2f\n", records[i].score);
        fprintf(file, "Версия: %s\n", records[i].version);
        fprintf(file, "Жанр: %s\n", records[i].genre);
        fprintf(file, "***********************************************\n");
    }

    fclose(file);
    printf("Данные успешно записаны в файл.\n");
    return -1;
}

int readFromFile(struct GameRecord records[], int* size)

{
    FILE* file;
    char filename[] = "games.txt";
    char buffer[1000];
    int dataRead = 0;

    if ((file = fopen(filename, "r")) == NULL)
    {
        printf("Ошибка при открытии файла.\n");
        return 0;
    }

    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        printf("%s", buffer);
        dataRead = 1; 
    }

    fclose(file);

    if (dataRead)
    {
        printf("Данные успешно прочитаны из файла.\n");
    }
    else
    {
        printf("База данных пуста. Нечего читать из файла.\n");
    }
    return -1;
}
