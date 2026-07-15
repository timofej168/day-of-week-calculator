#include <stdio.h>
#include <stdbool.h>

#define MIN_YEAR 1
#define MAX_YEAR 9999
#define MIN_MONTH 1
#define MAX_MONTH 12
#define MIN_DAY 1

typedef enum {
    LANG_RU,
    LANG_EN,
} Language;

// Словарь дней недели. Первый индекс — язык (Language), второй — день недели (0-6)
static const char* const DAYS_OF_WEEK[2][7] = {
    [LANG_RU] = {"воскресенье", "понедельник", "вторник", "среда", "четверг", "пятница", "суббота"},
    [LANG_EN] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}
};

// Индексируемые перечислением Language строки интерфейса
static const char* const PROMT_MSG[] = {
    [LANG_RU] = "Введите дату в формате дд.мм.гггг (от 01.01.0001 до 31.12.9999): ",
    [LANG_EN] = "Enter the date in dd.mm.yyyy format (from 01.01.0001 to 31.12.9999): "
};

static const char* const INPUT_ERROR_MSG[] = {
    [LANG_RU] = "Ошибка: данные введены некорректно.\n",
    [LANG_EN] = "Error: data entered incorrectly.\n"
};

static const char* const INVALID_DATE_MSG[] = {
    [LANG_RU] = "Введена несуществующая дата или год не попадает в диапазон [1-9999], перезапустите программу.\n",
    [LANG_EN] = "An invalid date was entered, or the year is outside the [1-9999] range; please restart the program.\n"
};

static inline bool is_leap_year(const int year) {
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

// Вычисляет день недели (0 - воскресенье, 6 - суббота) по алгоритму Судного дня (Doomsday rule)
int calculate_day(const int day, const int month, const int year) {

    // Базовые опорные даты (Судные дни) для каждого месяца.
    // Например, для некорректного года (невисокосного) опорный день в январе — 3-е число, в феврале — 28-е.
    static const int leap_year_doomsdays[12] = {4, 29, 14, 4, 9, 6, 11, 8, 5, 10, 7, 12};
    static const int no_leap_year_doomsdays[12] = {3, 28, 14, 4, 9, 6, 11, 8, 5, 10, 7, 12};

    const int first_part_of_year = year / 100; // Столетие (например, 20 для 2026 года)
    const int second_part_of_year = year % 100; // Год в столетии (например, 26 для 2026 года)

    // Находим опорный день для конкретного столетия (цикл повторяется каждые 400 лет)
    const int doomsday_of_century = ((first_part_of_year % 4) * 5 + 2) % 7;

    // Находим «судный день» для конкретного года внутри столетия
    const int doomsday_of_year = (second_part_of_year + second_part_of_year / 4) % 7 ;

    // Считаем расстояние в днях от введённой даты до ближайшего опорного «судного дня»
    int dif_in_date_and_month_doomsday = (is_leap_year(year))
    ? (day - leap_year_doomsdays[month - 1]) % 7
    : (day - no_leap_year_doomsdays[month - 1]) % 7;

    // Итоговое смещение относительно базового дня недели
    const int temp_res = (dif_in_date_and_month_doomsday + doomsday_of_year + doomsday_of_century) % 7;

    // Корректируем отрицательный остаток от деления в C (например, -1 % 7 должно стать 6)
    return (temp_res < 0) ? temp_res + 7 : temp_res;
}
void print_data(const int day_of_week, const Language language) {

    if (language == LANG_RU) {
        printf("Это %s!\n", DAYS_OF_WEEK[language][day_of_week]);
    } else {
        printf("This is %s!\n", DAYS_OF_WEEK[language][day_of_week]);
    }

}

/**
 * @brief Проверяет корректность введенной даты.
 *
 * Учитывает границы диапазонов месяцев, дней и особенности високосных годов.
 *
 * @param day   День месяца (1-31)
 * @param month Месяц (1-12)
 * @param year  Год (1-9999)
 * @return true если дата корректна, false в противном случае.
 */
bool check_valid_of_data(const int day, const int month, const int year) {
    if (year < MIN_YEAR || year > MAX_YEAR || month < MIN_MONTH || month > MAX_MONTH || day < MIN_DAY || day > 31)  return false;

    static const int days_in_months_in_no_leap_year[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    static const int days_in_months_in_leap_year[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    const int max_days = is_leap_year(year) ? days_in_months_in_leap_year[month - 1] : days_in_months_in_no_leap_year[month - 1];

    return day <= max_days;
}


int interaction(const Language language) {
    int year, month, day;

    printf("%s", PROMT_MSG[language]);

    // Проверяем, что 3 числа введены верно
    if (scanf("%d.%d.%d", &day, &month, &year) != 3) {
        printf("%s", INPUT_ERROR_MSG[language]);
        return 1;
    }
    if (check_valid_of_data(day, month, year)) {
        const int day_of_week = calculate_day(day, month, year);
        print_data(day_of_week, language);
    } else {
        printf("%s", INVALID_DATE_MSG[language]);
        return 1;
    }
    return 0;
}

int main(void) {

    printf("Select language (RU-1, ENG-2): ");

    int temp;

    if (scanf("%d", &temp) != 1) {
        printf("%s", INPUT_ERROR_MSG[LANG_EN]);
        return 1;
    }
    Language language = (temp == 1) ? LANG_RU : LANG_EN;

    return interaction(language);
}
