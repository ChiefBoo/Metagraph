# Metagraph

Программа, которая для заданного аннотируемого метаграфа и агент-функции над ним
выполнит вычисление атрибутов всех элементов.

Вход программы: файл описанного ниже формата.
1. первая строка – пара чисел: количество узлов (NV) и рёбер (NE) графа;
2. пустая строка;
3. NE пар номеров узлов, соответствующих началу и концу ребра;
4. пустая строка;
5. NV + NE правил агент-функции (сначала для узлов, потом для рёбер).
Варианты правил:
a. одно число – значение атрибута.
b. буква и число – значение атрибута копируется из указанного ребра (e) или узла (v).
c. имя функции – вычисление функции
min – определена только для узлов; вычисляет минимальное значение атрибутов рёбер,
входящих в узел;
* – определена только для рёбер; вычисляет произведение значений атрибутов левого
узла и входящих в него рёбер.

Результат работы программы: файл со списком значений атрибутов элементов графа, сначала
узлов, затем рёбер.

Программа должна принимать на вход имя файла с задачей и имя файла для записи результата.

Определения
Аннотируемый метаграф — ориентированный граф, каждый элемент которого описывается
набором атрибутов.
Агент-функция — набор правил преобразования графа и его элементов.


