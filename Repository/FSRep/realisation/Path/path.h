#ifndef LAB3_PATH_H
#define LAB3_PATH_H

#include <string>
#include <vector>

/**
 * @brief Вспомогательный класс для работы с путями в файловой системе.
 *
 * Предоставляет статические методы для разбора, нормализации, разрешения и проверки путей файловой системы.
 */
class Path {
public:
    /**
     * @brief Разбить путь на составные части
     * @param path Путь для разбора
     * @return Вектор строк, представляющих части пути
     */
    static std::vector<std::string> splitPath(const std::string& path);

    /**
     * @brief Нормализовать путь (убрать . и .., лишние разделители)
     * @param path Путь для нормализации
     * @return Нормализованный путь
     */
    static std::string normalizePath(const std::string& path);

    /**
     * @brief Разрешить относительный путь относительно базового пути
     * @param basePath Базовый (абсолютный) путь
     * @param relativePath Относительный путь
     * @return Абсолютный путь, полученный комбинацией базового и относительного
     */
    static std::string resolvePath(const std::string& basePath, const std::string& relativePath);

    /**
     * @brief Получить родительскую директорию для указанного пути
     * @param path Путь к файлу или директории
     * @return Путь к родительской директории
     */
    static std::string getParentPath(const std::string& path);

    /**
     * @brief Извлечь имя файла/директории из пути
     * @param path Полный путь
     * @return Имя файла/директории (последний компонент пути)
     */
    static std::string getFileName(const std::string& path);

    /**
     * @brief Проверить валидность пути
     * @param path Проверяемый путь
     * @return true если путь валиден, иначе false
     */
    static bool isValidPath(const std::string& path);

    /**
     * @brief Проверить соответствие имени шаблону (с поддержкой * и ?)
     * @param name Проверяемое имя
     * @param pattern Шаблон для сравнения
     * @return true если имя соответствует шаблону, иначе false
     */
    static bool matchesPattern(const std::string& name, const std::string& pattern);
};

#endif