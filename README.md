# Зайцева Юлия Александровна, БПИ213

# Домашнее задание №1
Использование каналов.

## Вариант задания 20: 
Разработать программу, вычисляющую число вхождений различных цифр в заданной ASCII-строке.

## Реализация программы на 4

a. Были созданы три дочерних процесса: readf - для открытия файла, count - для подсчета кол-ва цифр, out - для вывода результатов в файл. В родительских процессах происходило создание дочерних, а в дочерних - вся основная логика программы;

b. Для передачи информации между процессами использовались неименованные каналы: mem[2] для передачи прочитанного файла от 1 ко 2 процессу, num[2] для передачи информации о кол-ве цифр;

c. Для задания имен входного и выходного файлов используются аргументы командной строки;

d. Ввод и вывод данных при работе с файлами осуществляется через системные вызовы read и write.

e. 5 тестовых файлов и 5 файлов результатов (в названии есть res) представлены;

## Реализация программы на 5

a. Для передачи информации между процессами использовался именованный канал: name для передачи прочитанного файла от 1 ко 2 процессу и для передачи информации о кол-ве цифр. Канал создавался с помощью mknod;

b. Для множественного запуска добавлен флаг O_TRUNC, который очищает содержимое именованного канала;

c. Для задания имен входного и выходного файлов используются аргументы командной строки;

d. Ввод и вывод данных при работе с файлами осуществляется через системные вызовы read и write.

e. 5 тестовых файлов и 5 файлов результатов (в названии есть res) представлены;

## Реализация программы на 6

a. Были созданы два дочерних процесса: readf - для открытия файла в дочернем процессе и вывода результатов в файл в родительском процессе, count - для подсчета кол-ва цифр;

b. Для передачи информации между процессами использовались неименованные каналы: mem[2] для передачи прочитанного файла от 1 ко 2 процессу, num[2] для передачи информации о кол-ве цифр;

c. Для задания имен входного и выходного файлов используются аргументы командной строки;

d. Ввод и вывод данных при работе с файлами осуществляется через системные вызовы read и write.

e. 5 тестовых файлов и 5 файлов результатов (в названии есть res) представлены;

## Реализация программы на 7

a. Были созданы два дочерних процесса: readf - для открытия файла в дочернем процессе и вывода результатов в файл в родительском процессе, count - для подсчета кол-ва цифр;

b. Для передачи информации между процессами использовался именованный канал: name для передачи прочитанного файла от 1 ко 2 процессу и для передачи информации о кол-ве цифр обратно первому процессу. Канал создавался с помощью mknod;

c. Для задания имен входного и выходного файлов используются аргументы командной строки;

d. Ввод и вывод данных при работе с файлами осуществляется через системные вызовы read и write.

e. 5 тестовых файлов и 5 файлов результатов (в названии есть res) представлены;

## Реализация программы на 8

a. Были созданы неродственных процесса: в read.c происходит прочтение файла и запись результата в файл, а в count.c - подсчет кол-ва цифр. Для сигнализирования об окончании подсчетов кол-ва цифр использовалась разделяемая память, в которую записывалось "1", если процесс count.c завершен;

b. Для передачи информации между процессами использовался именованный канал: name для передачи прочитанного файла от 1 ко 2 процессу и для передачи информации о кол-ве цифр обратно первому процессу. Канал создавался с помощью mknod;

c. Для задания имен входного и выходного файлов используются аргументы командной строки;

d. Ввод и вывод данных при работе с файлами осуществляется через системные вызовы read и write.

e. 5 тестовых файлов и 5 файлов результатов (в названии есть res) представлены;

## Реализация программы на 9

a. Были созданы неродственных процесса: в read.c происходит прочтение файла и запись результата в файл, а в count.c - подсчет кол-ва цифр;

b. Для передачи информации между процессами использовался именованный канал: name для передачи прочитанного файла от 1 ко 2 процессу и для передачи информации о кол-ве цифр обратно первому процессу. Канал создавался с помощью mknod;

c. Для осуществления передачи информации ограниченными партиями от процесса read к процессу count использовался следующий метод: происходило чтение порции текста, записывалось в именованный канал, в объект разделяемой памяти помещалось "0". Далее процесс count запускался и после выполнения своей функции помещал в память "1". После окончания чтения первый процесс ждал окончания второго, а затем помещал в память значение "2", чтобы сигнализировать об окончании чтения файла. Далее второй процесс помещал в именованный канал результат подсчетов и помещать в разделяемую память значение "3", после чего запускался первый процесс, записывая результат в выходной файл.

d. Для задания имен входного и выходного файлов используются аргументы командной строки;

e. Ввод и вывод данных при работе с файлами осуществляется через системные вызовы read и write.

f. 5 тестовых файлов и 5 файлов результатов (в названии есть res) представлены, файл 4.txt отличается от файлов на предыдущие оценки;
