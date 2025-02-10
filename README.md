# Message Manager
Message Manager — это проект на C++, который включает в себя динамическую библиотеку для записи сообщений в журнал с разными уровнями важности и приложение, демонстрирующее работу библиотеки. Также в проекте содержатся тесты для проверки библиотеки.

## Состав репозитория

- **src/**
  - `message_manager.cpp`: Исходный файл реализации динамической библиотеки.
  - `message_manager.h`: Заголовочный файл библиотеки.
  - `demo_app.cpp`: Многопоточное приложение для обработки сообщений.
  - `tests.cpp`: Тесты для проверки библиотеки.
- **Makefile**: Сценарий сборки проекта.
- **journal.txt**: Пример выходного файла для хранения сообщений.

---

## Сборка проекта

### Требования
Для сборки проекта потребуется:
- Компилятор с поддержкой C++11 или выше
- Утилита `make`

### Инструкция по сборке

1. Клонируйте репозиторий:
   ```bash
   git clone https://github.com/ivkinivan/message_manager.git
   cd message_manager
   
2. Выполните make:
   ```bash
   make
3. Для запуска приложения:
   ```bash
    ./demo_app
4. Для запуска тестов:
   ```bash
   make run_tests
5. Для очистки от сборочных файлов
   ```bash
   make clean
## Краткая документация
### динамическая библиотека
1. Класс `enum class importance`
   - Нужен для корректной реализации уровней важности. Для него определены 2 функции переводящие `enum` -> `string` и `string` -> `enum`
2. Класс `message`
   - Содержит в себе информацию о сообщениях, необязательность параметра важности реализована при помощи `optional<importance>`, время задается автоматически в конструкторе, при помощи `std::chrono::system_clock::time_point`
3. Класс `message_manager`
   - Содержит в себе информацию о файле вывода, стандартном уровне важности сообщений. Также в нем реализованы методы записи сообщений и изменения стартовых параметров
     
### Многопоточное приложение  
Многопоточное приложение позволяет обрабатывать текстовые сообщения, вводимые пользователем, и записывать их в файл. Сообщения записываются в отдельном потоке.
- Функционал:
    Приложение запрашивает у пользователя имя файла для ввода (в проекте есть файл `journal.txt`, можно использовать его)
    Далее приложение запрашивает у пользователя стандартный уровень важности сообщений  (`low`, `medium`, или `high`)
    Сообщения с уровнем важности ниже стандартного записываться не будут
    Пользователь вводит текст сообщения в стандартный ввод.
    Сообщение помещается в потокобезопасную очередь.
    Поток записи извлекает сообщение из очереди и записывает его в файл
    Приложение управляется при помощи команд. Чтобы их увидеть после запуска надо ввести `-help`
