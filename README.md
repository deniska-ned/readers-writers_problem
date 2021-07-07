# Задача о читателях-писателях

## Задание

Необходимо реализовать схему взаимодействия процессов «читатели-писатели»
под ОС Windows с использованием соответствующих системных вызовов WinAPI
и многопоточности, используя thread, event, mutex.  Приоритет может быть любым.
Потоки должны разделять общее адресное пространство. Количество читателей,
писателей, повторов программы, а также временные задержки, задаются директивами
препроцессора. Обеспечить вывод на экран результат работы читателей
и писателей.


###### P.S.

- вместо WinAPI использовался POSIX