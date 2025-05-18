# Modestest
Библиотека Modestest предназначена для тестирования исходного кода на языке c++. 
В основном библиотека ориентирована на модульное тестирования (unit-testing), так как архитектура подразумевает изоляцию тестов, но в целом она может использоваться и для интеграционного тестирования.

## Установка
Для работы потребуется склонировать репозиторий:
```
git clone https://github.com/ssForz/Modestest.git
```
Далее для работы с тестами требуется только подключить заголовочный файл:
```c++
#include <modestest/modestest.h>
```

## Функционал библиотеки
### Тесты
Все тесты, которые создаются разработчиком, должны относиться к тестовому набору. Объявляется он созданием класса, который наследуется от NModestest::Ttesting:
```c++
class MySuite: public NModestest::Ttesting {}
```
Для каждого набора создаётся список тестов:
```c++
INIT_TEST_LIST(MySuite);
//tests
END_TEST_LIST();
```

Чтобы объявить тесты в списке требуется вызвать макросы TEST_START и TEST_END:
```c++
TEST_START(MySuite, "TestA") {
    //statements
}
TEST_END();

TEST_START(MySuite, SMOKE(TestB)) {
    //statements;
}
TEST_END();
```
В данном примере видно, что тесты привязываются к набору и должны иметь своё имя. 

При вызове INIT_TEST_LIST макрос создаёт пространство имён NSuite_<ИМЯ_ТЕСТОВОГО_НАБОРА> и в нём появляется функция RegisterTests(). 
Данную функцию нужно вызывать для записи информации о всех тестах в систему.

Сами тесты запускаются макросом RUN_ALL_TESTS(), который запустить все тесты для всех наборов в файле, или RUN_TESTS(MySuite), который запустит только тестовый набор MySuite.

Библиотека также поддерживает опцию -filter=\*\*, которая позволяет запускать тесты, которые содержат в имени переданную строку.
Для инициализации тестирования требуется вызвать NModestest::Ttesting::InitTesting(argc, argv), куда будет передаваться фильтр, если он указан.

Пример main функции с запуском тестов:
```c++
int main(int argc, char *argv[]) {

    NModestest::Ttesting::InitTesting(argc, argv);

    NSuite_MySuite::RegisterTests();

    RUN_ALL_TESTS();

    // RUN_TESTS(MySuite);
}
```
После сборки и запуска выведется отчёт о тестировании. Например, вот так выглядит отчёт для набора из двух тестов, один из которых проваливается:
```
GLOBAL NOTE: Running with filter: ""

====STARTING GLOBAL TESTING====

====[RUNNING TEST SUITE]====
TEST SUITE: ModuleX
RUNNING 2 tests
[START    ] ModuleX.BaseOK_smoke_test

[      OK ] ModuleX.BaseOK_smoke_test (0 ms)

[START    ] ModuleX.Other_smoke_test

Fail at line: 20
Value of someStatus
Expected: true
Actual: false

[     FAIL] ModuleX.Other_smoke_test (0 ms)

[ FAILED SUITE ] ModuleX (1 ms)
>>>> Passed tests: 1
<<<< Failed tests: 1

[ FAILED SUITE ] ModuleX (1 ms)
>>>> Passed tests: 1

[ FAILED SUITE ] ModuleX (1 ms)


[ FAILED SUITE ] ModuleX (1 ms)
>>>> Passed tests: 1
<<<< Failed tests: 1

====FINISH GLOBAL TESTING (total time 3 ms)====
TOTAL Passed: 1
TOTAL Failed: 1
TOTAL Disabled: 0
```

### Макросы проверки
В библиотеке есть различные макросы проверок, их список:
```c++
FAIL() //- выброс неудачи теста
SUCCESS() //- выброс успеха теста

ASSERT_TRUE(expr) //- строгая проверка истинности выражения
ASSERT_FALSE(expr) //- строгая проверка ложности выражения
ASSERT_EQ(lexpr, rexpr) //- строгая проверка равенства аргументов
ASSERT_NEQ(lexpr, rexpr) //- строгая проверка неравенства аргументов
ASSERT_EX(call) //- строгая проверка выброса любого исключения
ASSERT_THIS_EX(call, ex) //- строгая проверка выброса конкретного исключения

EXPECT_TRUE(expr) //- проверка истинности выражения
EXPECT_FALSE(expr) //- проверка ложности выражения
EXPECT_EQ(lexpr, rexpr) //- проверка равенства аргументов
EXPECT_NEQ(lexpr, rexpr) //- проверка неравенства аргументов
EXPECT_EX(call) //- проверка выброса любого исключения
EXPECT_THIS_EX(call, ex) //- проверка выброса конкретного исключения

WAIT_MS(n) //- ожидание n миллисекунд
WAIT_SEC(n) //- ожидание n секунд
WAIT_MIN(n) //- ожидание n минут

THROW_EX() //- выброс исключения (стандартное исключени std::string("Modestest exception"))
```
Отличия строгой проверки от обычной в том, что строгая проверка завершает выполнение теста в случае провала, а обычная лишь выводит сообщение о провале и помечает статус теста, как FAIL.

### Генерация имён
Для единообразия названий тестов добавлена библиотека генерации имён, в которой реализованы следующие макросы:
```c++
GET_NAME(name) //- делает из name имя name_test
SMOKE(name) //- делает из name имя name_smoke_test
MEDIUM(name) //- делает из name имя name_medium_test
HEAVY(name) //- делает из name имя name_heavy_test
CRITICAL(name) //- делает из name имя name_critical_test
CUSTOM(prefix, name) //- делает из name имя name_prefix_test
```
Эти имена удобны при запуске тестов по фильтрам, например, запуск тестов после каждой сборки с фильтром -filter=\*smoke\* 

Макрос CUSTOM лучше всего использовать при создании своего макроса, например:
```c++
#define DAILY(name) CUSTOM(daily, name)
```
Таким образом мы получили макрос, который будет добавлять приписку daily к тесту.

### Заключение
Библиотека легко подключается в проект и отлично подходит для написания unit-тестов. Она не подразумевает общее хранение данных для тестов, как сделано в gtest, но подобное хранилище можно легко сделать самому просто объявив класс, который будет содержать в себе всю информацию. С этим классом можно работать из самих тестов. 

Исходный код довольно понятный и поэтому легко редактируется, если у пользователя есть свои требования к именам/отчёту или иной логике работы библиотеки. 
