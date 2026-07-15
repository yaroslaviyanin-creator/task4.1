/*
mm.c - менеджер памяти.
Янин Ярослав Иванович
Группа МК-101
*/

#include <stdlib.h>
#include <stdio.h> 
#include <string.h>
#include "mm.h"

MemoryManager* _mm = NULL; // Глобальный указатель на наш менеджер памяти

// Функция инициализирует менеджер памяти
// <size> - размер одного элемента в байтах
// <count> - количество элементов в блоке
// <callback> - указатель на функцию перерасчета адресов при реаллокации
int mm_initialize(int size, int count, void(*callback)(int ofs)) {
    if (_mm != NULL) free(_mm); // Если менеджер уже был создан, удаляем старый

    // Выделяем память под саму структуру MemoryManager
    _mm = (MemoryManager*)malloc(sizeof(MemoryManager));
    if (_mm == NULL) return -1;

    _mm->size = size; // Запоминаем размер одного элемента

#ifdef MM_USE
    _mm->callback = callback; // Сохраняем коллбэк для пересчета указателей
    _mm->delta = count;       // На сколько будем увеличивать массив, если память закончится
    _mm->count = count;       // Сколько элементов вмещает текущий блок

    // Выделяем один большой сплошной кусок памяти (массив байтов)
    _mm->m = calloc(_mm->size, _mm->count);
    if (_mm->m == NULL) {
        free(_mm);    // Удаляем саму структуру менеджера, если не хватило памяти
        _mm = NULL;
        return -1;
    }
    _mm->border_ofs = 0;  // Смещение свободной памяти равно 0 (всё свободно)
    _mm->empty = NULL;    // Стек свободных кусочков изначально пуст
#endif

    return 0;
}

// Функция очищает память, использующуюся менеджером
void mm_finalize(void) {
#ifdef MM_USE
    if (_mm != NULL) free(_mm->m); // Сначала освобождаем большой массив
#endif
    if (_mm != NULL) free(_mm);           // Затем удаляем саму структуру менеджера
    _mm = NULL;
}