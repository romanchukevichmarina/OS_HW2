# Романчукевич Марина Александровна БПИ217
## Вариант 29
Задача про экзамен. Преподаватель проводит экзамен у группы студентов. Каждый студент получает свой билет, сообщает его номер и готовит письменный ответ за некоторое случайное время. Подготовив ответ, он передает его преподавателю. Преподаватель некоторое случайное время просматривает ответ и сообщает студенту оценку.
Требуется создать многопроцессное приложение, моделирующее действия преподавателя и студентов. При решении использовать парадигму «клиент–сервер».

## Сценарий:
В начале процесс делится на родительский и дочерний: в родительском процессе идут действия преподавателя, в дочернем -- студента. 
В процессе преподавателя имитируется начало сдачи билета, в этот момент семафор преподавателя уменьшается, чтобы другие студенты ждали пока текущий студент закончит сдавать билет. Процесс засыпает на 1-10 секунд, после студенту ставится оценка, и он уходит, процесс преподавателя засыпает, а семафор студента увеличивается, чтобы следующий студент мог подойти.
В процессе студента запускается создание новых процессов "студентов". Студент "засыпает" на 1-10 секунд, имитируя подготовку билета, далее ожидает, когда семафор студента увеличится. Когда преподаватель освобождается, семафор студента уменьшается, он записывает свой pid в память, и пробуждает преподавателя.

## 4
Множество процессов взаимодействуют с использованием име- нованных POSIX семафоров. Обмен данными ведется через разделяемую память в стандарте POSIX.


Я чувствую потребность в оправдании, так как просрочила дедлайн на неприличный по меркам ПИ срок 🥲:
Я волонтер в приюте для животных, месяц назад туда попало 7 щеночков, и последние недели я плотно занималась их пристройством. Сейчас остался один щеночек, его зовут Соник ☺️
![](Соник.jpg)

