/*
main.c - приложение
Янин Ярослав Иванович
Группа МК-101
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "mm.h"
#include "string_list.h"

#define MM_BLOCKS 2 // количество элементов списка, по умолчанию размещающихся в блоке менеджера памяти
#define TEST_LIST_ENTITIES 100 
#define TEST_BUFFER_MAX_LEN 64

StringListEntry* list;

// Функция перерасчитывает адреса при перевыделении памяти в менеджере памяти
// <ofs> - сдвиг в байтах (новый адрес минус старый)
void realloc_callback(int ofs) {
    ListEntry* head, * currentListEntry, * entry;
    StringListEntry* currentStringListEntry = list; // Начинаем с головы

    if (ofs) { // Если память успешно выделена
        printf("MM reallocating; update addresses...\n");
        currentListEntry = head = &currentStringListEntry->initOrderEntry;
        do {
            // Получаем структуру данных по указателю узла
            currentStringListEntry = GET_CONTENT_RECORD(StringListEntry, initOrderEntry, currentListEntry);
            currentListEntry = list_next(currentListEntry); // Шагаем вперед

            // Прибавляем смещение ко всем указателям во всех 6 списках
            entry = &currentStringListEntry->initOrderEntry;
            (char*)entry->next += ofs;
            (char*)entry->prev += ofs;

            entry = &currentStringListEntry->reverseOrderEntry;
            (char*)entry->next += ofs;
            (char*)entry->prev += ofs;

            entry = &currentStringListEntry->sortByLen;
            (char*)entry->next += ofs;
            (char*)entry->prev += ofs;

            entry = &currentStringListEntry->reverseSortByLen;
            (char*)entry->next += ofs;
            (char*)entry->prev += ofs;

            entry = &currentStringListEntry->quickSort;
            (char*)entry->next += ofs;
            (char*)entry->prev += ofs;

            entry = &currentStringListEntry->reverseQuickSort;
            (char*)entry->next += ofs;
            (char*)entry->prev += ofs;

        } while (currentListEntry != head); // Идем, пока не обойдем весь список

        (char*)list += ofs; // Сдвигаем саму глобальную переменную "голова списка"
    }
    else {
        fprintf(stderr, "MM Error: reallocating failed, cannot reallocate memory\n");
    }
}

// Функция является точкой входа, запускает тесты списков и менеджера памяти
// Аргументы командной строки не используются
int main() {
    char buffer[TEST_BUFFER_MAX_LEN];   // Буфер для генерации случайных строк 
    int i, j, n;
    ListEntry* entry;

    // Вызываем функцию инициализации менеджера памяти
    if (mm_initialize(sizeof(StringListEntry), MM_BLOCKS, &realloc_callback)) {
        fprintf(stderr, "MM Error: Cannot allocate memory!\n");
        return -1;
    }

    // Создание головы списка (NULL вместо текста)
    list = string_list_new(NULL);
    srand((unsigned int)time(NULL));

    // Тест 1: Проверка работы списков и алгоритмов сортировки
    printf("\n---- list test ----\n");
    string_list_add(&list, "test444");
    string_list_add(&list, "test22222");
    string_list_add(&list, "test6");
    string_list_add(&list, "test3333");
    string_list_add(&list, "test111111");
    string_list_add(&list, "test55");
    string_list_add(&list, "test777");
    string_list_add(&list, "test88888");

    string_list_make_reverse_order(list);
    string_list_make_sort_by_len(list);
    string_list_make_reverse_sort_by_len(list);
    string_list_make_quick_sort(list);
    string_list_make_reverse_quick_sort(list);

    printf("Initial order:\n");
    string_list_init_order_print(list);
    printf("Reverse order:\n");
    string_list_reverse_order_print(list);
    printf("Sorted by len:\n");
    string_list_sort_by_len_print(list);
    printf("Reversed sorted by len:\n");
    string_list_reverse_sort_by_len_print(list);
    printf("Quick sorted:\n");
    string_list_quick_sort_print(list);
    printf("Reversed quick sorted:\n");
    string_list_reverse_quick_sort_print(list);

    string_list_clear(list); // Очищаем список перед следующим тестом

    // Тест 2: Проверка работы менеджера памяти под нагрузкой
    printf("\n---- memory manager test ----\n");
    n = TEST_LIST_ENTITIES;
    for (j = 0; j < 3; j++) {
        // Добавляем кучу случайных строк (вызовет нехватку памяти и реаллокацию)
        for (i = 0; i < n; i++) {
            string_list_add(&list, string_list_rnd_str(buffer, TEST_BUFFER_MAX_LEN));
        }

        string_list_make_quick_sort(list);

        // Удаляем половину элементов для проверки фрагментации и стека empty
        for (i = n / 2; i; i--) {
            entry = list_next(&list->quickSort);
            string_list_remove(GET_CONTENT_RECORD(StringListEntry, quickSort, entry));
        }

        n = string_list_count(list);
        printf("\nQuick sorted, ineration %d, count: %d:\n", j, n);
        string_list_quick_sort_print(list);
        n *= 3; // Увеличиваем нагрузку для следующей итерации
    }

    // Финализация приложения
    string_list_free(string_list_clear(list));
    mm_finalize();
    return 0;
}