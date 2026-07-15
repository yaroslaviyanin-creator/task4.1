/*
list.h - логика базового списка.
Янин Ярослав Иванович
Группа МК-101
*/

#ifndef _LIST_H_
#define _LIST_H_

// Вычисляем смещение поля в структуре
// <TypeName> - тип структуры
// <entryName> - имя поля внутри структуры
#define OFFSET_OF(TypeName, entryName)  \
    (unsigned char*)&((TypeName*)0)->entryName  

// Вычитаем смещение из реального адреса, чтобы получить указатель на начало структуры
// <TypeName> - тип структуры
// <entryName> - имя поля внутри структуры
// <addr> - реальный адрес узелка ListEntry
#define GET_CONTENT_RECORD(TypeName, entryName, addr)    \
    ((TypeName*)((unsigned char*)addr - OFFSET_OF(TypeName, entryName)))    

typedef struct _ListEntry {
    struct _ListEntry* next;
    struct _ListEntry* prev;
} ListEntry;

// Функция инициализирует новый элемент списка или голову нового списка
// <head> - указатель на базовый узел списка
void list_init(ListEntry* head);

// Функция добавляет новый элемент строго после указанного
// <entry> - элемент, после которого происходит вставка
// <new_entry> - новый вставляемый элемент
ListEntry* list_add(ListEntry* entry, ListEntry* new_entry);

// Функция исключает элемент из двусвязного списка, перевязывая соседей
// <entry> - элемент, который нужно удалить из цепочки
ListEntry* list_remove(ListEntry* entry);

// Функция меняет местами два элемента в списке
// <entry1> - первый переставляемый элемент
// <entry2> - второй переставляемый элемент
void list_swap(ListEntry* entry1, ListEntry* entry2);

// Функция возвращает указатель на следующий элемент списка
// <entry> - текущий элемент
ListEntry* list_next(ListEntry* entry);

// Функция возвращает указатель на предыдущий элемент списка
// <entry> - текущий элемент
ListEntry* list_prev(ListEntry* entry);

#endif  // _LIST_H_