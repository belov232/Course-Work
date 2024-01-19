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
        printf("����:\n");
        printf("1. �������� ������\n");
        printf("2. ����� �� ��������\n");
        printf("3. ����� �� ��������\n");
        printf("4. ���������� �� ������� ������ �� ������\n");
        printf("5. �������� � ����\n");
        printf("6. ��������� �� �����\n");
        printf("7. ����� �� ���������\n");
        printf("�������� ����� ����: ");
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
                printf("������� �������� ��� ������: ");
                fgets(searchTitle, sizeof(searchTitle), stdin);
                searchTitle[strcspn(searchTitle, "\n")] = 0;
                searchByTitle(searchTitle);
            }
            else
            {
                printf("���� ������ �����.\n");
            }
            break;

        case 3:
            if (size > 0)
            {
                int maxRating;
                printf("������� ������������ ������� ��� ������: ");
                scanf("%d", &maxRating);
                searchByRating(maxRating);
            }
            else
            {
                printf("���� ������ �����.\n");
            }
            break;

        case 4:
            if (size > 0)
            {
                sortByScore(records, size);

                printf("\n ��������������� ������ �� ������� ������ �� ������ (� ������� ��������):\n");

                for (int i = 0; i < size; i++)
                {
                    printf("��������: %s, ������� ������ �� ������: %.2f\n", records[i].title, records[i].score);
                }
            }
            else
            {
                printf("���� ������ �����.\n");
            }
            break;

        case 5:
            if (size > 0)
            {
                writeToFile(records, size);
            }
            else
            {
                printf("���� ������ �����.\n");
            }
            break;

        case 6:
            if (size > 0)
            {
                readFromFile(records, &size);
                printf("������ ������� ��������� �� �����.\n");
            }
            else
            {
                printf("���� ������ �����. ������ ������ �� �����.\n");
            }
            break;

        case 7:
            printf("��������� ���������.\n");
            break;

        default:
            printf("������������ ����. ����������, �������� ������������ ����� ����.\n");
        }

    } while (choice != 7);

    return 0;
}

int addRecord(struct GameRecord records[], int* size)

{
    if (*size < MAX_SIZE)
    {
        printf("������� ������ ����� ����:\n");
        printf("��������: ");
        fgets(records[*size].title, 50, stdin);
        records[*size].title[strcspn(records[*size].title, "\n")] = 0;

        printf("�������: ");
        scanf("%d", &records[*size].rating);

        while (getchar() != '\n');

        printf("�����: ");
        fgets(records[*size].review, 100, stdin);
        records[*size].review[strcspn(records[*size].review, "\n")] = 0;

        printf("������� ������ �� ������: ");
        scanf("%f", &records[*size].score);

        while (getchar() != '\n');

        printf("������: ");
        fgets(records[*size].version, 20, stdin);
        records[*size].version[strcspn(records[*size].version, "\n")] = 0;

        printf("����: ");
        fgets(records[*size].genre, 10, stdin);
        records[*size].genre[strcspn(records[*size].genre, "\n")] = 0;

        return (*size)++;
    }
    else
    {
        printf("���� ������ �����. ���������� �������� ����� ������.\n");
        return -1;
    }
}

int searchByTitle(const char* searchTitle)

{
    FILE* file = fopen("games.txt", "r");

    if (file == NULL)
    {
        printf("������ ��� �������� ����� ��� ���� �� ����������.\n");
        return 0;
    }

    int found = 0;
    char buffer[1000];

    printf("\n���������� ������ �� �������� \"%s\":\n", searchTitle);

    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        struct GameRecord record;
        sscanf(buffer, "��������: %49[^\n]", record.title);

        if (strstr(record.title, searchTitle) != NULL)
        {
            found = 1;
            printf("%s", buffer);
        }
    }

    fclose(file);

    if (!found)
    {
        printf("���� � ��������� \"%s\" �� �������.\n", searchTitle);
        return -1;
    }
}

int searchByRating(int maxRating)

{
    FILE* file = fopen("games.txt", "r");

    if (file == NULL)
    {
        printf("������ ��� �������� ����� ��� ���� �� ����������.\n");
        return 0;
    }

    int found = 0;
    char buffer[1000];

    printf("\n���������� ������ �� �������� �� %d:\n", maxRating);

    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        struct GameRecord record;
        sscanf(buffer, "�������: %d", &record.rating);

        if (record.rating <= maxRating)
        {
            found = 1;
            printf("%s", buffer);
        }
    }

    fclose(file);

    if (!found)
    {
        printf("���� � ��������� �� %d �� �������.\n", maxRating);
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
        printf("�� ������� ������� ���� ��� ������.\n");
        return 0;
    }

    for (int i = 0; i < size; i++)
    {
        fprintf(file, "��������: %s\n", records[i].title);
        fprintf(file, "�������: %d\n", records[i].rating);
        fprintf(file, "�����: %s\n", records[i].review);
        fprintf(file, "������� ������ �� ������: %.2f\n", records[i].score);
        fprintf(file, "������: %s\n", records[i].version);
        fprintf(file, "����: %s\n", records[i].genre);
        fprintf(file, "***********************************************\n");
    }

    fclose(file);
    printf("������ ������� �������� � ����.\n");
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
        printf("������ ��� �������� �����.\n");
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
        printf("������ ������� ��������� �� �����.\n");
    }
    else
    {
        printf("���� ������ �����. ������ ������ �� �����.\n");
    }
    return -1;
}

