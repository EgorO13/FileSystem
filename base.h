#ifndef LAB3_BASE_H
#define LAB3_BASE_H

#include <string>
#include <vector>
#include <map>
#include <atomic>
#include <future>

/**
 * @brief Тип блокировки файла
 */
enum class Lock {
    NotLock = 0,     ///< Файл не заблокирован
    ReadLock = 1,    ///< Файл заблокирован для чтения
    WriteLock = 2,   ///< Файл заблокирован для записи
    AllLock = 3      ///< Файл полностью заблокирован
};

/**
 * @brief Тип субъекта прав доступа
 */
enum class SubjectType {
    User,            ///< Пользователь
    Group            ///< Группа пользователей
};

/**
 * @brief Тип объекта файловой системы
 */
enum class ObjectType {
    File,            ///< Файл
    Directory        ///< Директория
};

/**
 * @brief Типы разрешений файловой системы
 */
enum class PermissionType {
    Read,               ///< Чтение файла / Просмотр содержимого каталога
    Write,              ///< Запись в файл / Создание объектов в каталоге
    Execute,            ///< Выполнение файла / Проход через каталог
    Modify,             ///< Модификация файла / Модификация каталога
    ModifyMetadata,     ///< Изменение метаданных (имя, владелец и т.д.)
    ChangePermissions,  ///< Изменение прав доступа к объекту
};

/**
 * @brief Эффект разрешения
 */
enum class PermissionEffect {
    Allow,              ///< Разрешить действие
    Deny                ///< Запретить действие
};

/**
 * @brief Результат выполнения команды
 */
struct CommandResult {
    bool success;                     ///< Успешность выполнения команды
    std::vector<std::string> message; ///< Сообщения команды
    std::string error;                ///< Сообщение об ошибке (если есть)
};

/**
 * @brief Информация о файле для отображения
 */
struct FileInfo {
    std::string name;                 ///< Имя файла
    std::string type;                 ///< Тип файла
};

/**
 * @brief Результат операции файловой системы
 */
struct FileSystemResult {
    bool success;                     ///< Успешность выполнения операции
    std::vector<std::string> messages; ///< Сообщения от операции
    std::string error;                ///< Сообщение об ошибке (если есть)

    /**
     * @brief Конструктор результата
     * @param s Флаг успешности операции
     * @param msgs Список сообщений
     * @param err Сообщение об ошибке
     */
    FileSystemResult(bool s = false, const std::vector<std::string>& msgs = {}, const std::string& err = "")
        : success(s), messages(msgs), error(err) {}
};

/**
 * @brief Запись команды для истории
 */
struct CommandEntry {
    std::string commandName;                   ///< Имя команды
    std::vector<std::string> fixedArgs;        ///< Фиксированные аргументы
    std::vector<int> dynamicArgIndices;        ///< Индексы динамических аргументов

    /**
     * @brief Конструктор записи команды
     * @param cmdName Имя команды
     * @param fixed Фиксированные аргументы
     * @param dynamic Индексы динамических аргументов
     */
    CommandEntry(std::string cmdName, const std::vector<std::string>& fixed = {}, const std::vector<int>& dynamic = {})
        : commandName(cmdName), fixedArgs(fixed), dynamicArgIndices(dynamic) {}
};

/**
 * @brief Структура для хранения результатов сбора статистики файловой системы
 *
 * Содержит информацию о типах файлов, размерах, владельцах и времени выполнения.
 */
struct StatisticsResult {
    std::map<std::string, size_t> typeCounters;

    std::atomic<unsigned int> totalSize{0};           ///< Общий размер всех файлов в байтах
    std::atomic<unsigned int> largestFileSize{0};     ///< Размер самого большого файла
    std::string largestFilePath;                      ///< Путь к самому большому файлу

    mutable std::mutex ownersMutex;                   ///< Мьютекс для синхронизации доступа к статистике владельцев
    std::map<std::string, size_t> filesByOwner;       ///< Количество файлов по владельцам

    /**
     * @brief Конструктор по умолчанию
     */
    StatisticsResult() = default;

    StatisticsResult(const StatisticsResult&) = delete;
    StatisticsResult& operator=(const StatisticsResult&) = delete;

    /**
     * @brief Конструктор перемещения
     * @param other Другой объект StatisticsResult
     */
    StatisticsResult(StatisticsResult&& other) noexcept
        : typeCounters(std::move(other.typeCounters)),
          totalSize(other.totalSize.load()),
          largestFileSize(other.largestFileSize.load()),
          largestFilePath(std::move(other.largestFilePath)),
          filesByOwner(std::move(other.filesByOwner)) {}

    /**
     * @brief Оператор перемещения
     * @param other Другой объект StatisticsResult
     * @return Ссылка на текущий объект
     */
    StatisticsResult& operator=(StatisticsResult&& other) noexcept {
        if (this != &other) {
            typeCounters = std::move(other.typeCounters);
            totalSize = other.totalSize.load();
            largestFileSize = other.largestFileSize.load();
            largestFilePath = std::move(other.largestFilePath);
            filesByOwner = std::move(other.filesByOwner);
        }
        return *this;
    }

    /**
     * @brief Обновляет информацию о самом большом файле
     * @param path Путь к файлу
     * @param size Размер файла в байтах
     */
    void updateLargestFile(const std::string& path, unsigned int size) {
        unsigned int current = largestFileSize.load();
        while (size > current) {
            if (largestFileSize.compare_exchange_weak(current, size)) {
                largestFilePath = path;
                break;
            }
        }
    }

    /**
     * @brief Добавляет владельца в статистику
     * @param owner Имя владельца
     */
    void addOwner(const std::string& owner) {
        std::lock_guard<std::mutex> lock(ownersMutex);
        filesByOwner[owner]++;
    }

    /**
     * @brief Объединяет результаты из другого потока
     * @param other Результаты из другого потока
     */
    void merge(StatisticsResult&& other) {
        for (const auto& [type, count] : other.typeCounters) {
            typeCounters[type] += count;
        }
        totalSize += other.totalSize.load();
        unsigned int otherLargest = other.largestFileSize.load();
        if (otherLargest > largestFileSize.load()) {
            largestFileSize = otherLargest;
            largestFilePath = std::move(other.largestFilePath);
        }

        {
            std::lock_guard<std::mutex> lock1(ownersMutex);
            std::lock_guard<std::mutex> lock2(other.ownersMutex);
            for (const auto& [owner, count] : other.filesByOwner) {
                filesByOwner[owner] += count;
            }
        }
    }

    /**
     * @brief Возвращает общее количество объектов
     * @return Общее количество файлов и директорий
     */
    size_t getTotalObjects() const {
        size_t total = 0;
        for (const auto& [type, count] : typeCounters) {
            total += count;
        }
        return total;
    }

    /**
     * @brief Сбрасывает все статистики
     */
    void reset() {
        typeCounters.clear();
        totalSize = 0;
        largestFileSize = 0;
        largestFilePath.clear();
        {
            std::lock_guard<std::mutex> lock(ownersMutex);
            filesByOwner.clear();
        }
    }
};

#endif