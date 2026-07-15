/*
list.c - логика базового списка.
Янин Ярослав Иванович
Группа МК-101
*/

#include "list.h"

// Функция инициализирует новый элемент списка или голову нового списка
// <entry> - указатель на базовый узел списка
void list_init(ListEntry* entry) {
    entry->next = entry; // закольцованный список
    entry->prev = entry; // закольцованный список
}

// Функция добавляет новый элемент после указанного
// <entry> - элемент, после которого происходит вставка
// <new_entry> - новый вставляемый элемент
ListEntry* list_add(ListEntry* entry, ListEntry* new_entry) {
    new_entry->next = entry->next;      // Новый элемент указывает вперед на того, кто был после entry
    new_entry->prev = entry;              // Новый элемент указывает назад на entry
    entry->next = new_entry;              // entry теперь указывает вперед на новый элемент
    new_entry->next->prev = new_entry;  // Тот, кто стоит после нового, теперь указывает назад на новый
    return new_entry;
}

// Функция исключает элемент из двусвязного списка, перевязывая соседей
// <entry> - элемент, который нужно удалить из цепочки
ListEntry* list_remove(ListEntry* entry) {
    entry->next->prev = entry->prev;  // Элемент справа теперь указывает назад на элемент слева
    entry->prev->next = entry->next;  // Элемент слева теперь указывает вперед на элемент справа
    return entry;
}

// Функция меняет местами два элемента в списке
// <entry1> - первый переставляемый элемент
// <entry2> - второй переставляемый элемент
void list_swap(ListEntry* entry1, ListEntry* entry2) {
    ListEntry* entry1_next, * entry1_prev;
    if (entry1 == entry2) return; // сам с собою элемент не переставляем

    if (entry1->next == entry2) {
        // перстановка соседних элементов, entry1 слева от entry2
        // Аккуратно перевязываем 6 указателей (2 у соседей снаружи, 4 у самих элементов)
        entry1->prev->next = entry2;
        entry2->next->prev = entry1;
        entry2->prev = entry1->prev;
        entry1->next = entry2->next;
        entry1->prev = entry2;
        entry2->next = entry1;

    }
    else if (entry2->next == entry1) {
        // перстановка соседних элементов, entry1 справа от entry2
        list_swap(entry2, entry1);  // Вызываем эту же функцию, поменяв аргументы местами
    }
    else {
        // элементы не граничат друг с другом
        // Сохраняем старых соседей первого элемента
        entry1_next = entry1->next;
        entry1_prev = entry1->prev;

        // Перевязываем соседей так, чтобы они указывали на новые места
        entry1_prev->next = entry2;
        entry1_next->prev = entry2;
        entry2->prev->next = entry1;
        entry2->next->prev = entry1;

        // Меняем указатели самих элементов
        entry1->next = entry2->next;
        entry1->prev = entry2->prev;
        entry2->next = entry1_next;
        entry2->prev = entry1_prev;
    }
}

// Функция возвращает указатель на следующий элемент списка
// <entry> - текущий элемент
ListEntry* list_next(ListEntry* entry) {
    return entry->next;
}

// Функция возвращает указатель на предыдущий элемент списка
// <entry> - текущий элемент
ListEntry* list_prev(ListEntry* entry) {
    return entry->prev;
}