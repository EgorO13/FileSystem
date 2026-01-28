#ifndef LAB3_I_FILE_H
#define LAB3_I_FILE_H

#include <string>

/**
 * @brief Интерфейс файла файловой системы.
 *
 * Определяет базовые операции для работы с файлами:
 * чтение, запись, изменение размера и получение информации.
 */
class IFile {
public:
    virtual ~IFile() = default;

    /**
     * @brief Записать содержимое в файл
     * @param cont Содержимое для записи
     * @return true если запись успешна, иначе false
     */
    virtual bool writeContent(std::string_view cont) = 0;

    /**
     * @brief Прочитать содержимое файла
     * @return Содержимое файла
     */
    virtual std::string readContent() const = 0;

    /**
     * @brief Обрезать содержимое файла до указанного индекса
     * @param index Индекс до которого обрезать
     * @return true если операция успешна, иначе false
     */
    virtual bool truncateContent(int index) = 0;

    /**
     * @brief Очистить содержимое файла
     */
    virtual void clearContent() = 0;

    /**
     * @brief Получить размер файла
     * @return Размер файла в байтах
     */
    virtual int getSize() const = 0;

    /**
     * @brief Записать содержимое в файл (альтернативная версия)
     * @param cont Содержимое для записи
     * @return true если запись успешна
     */
    virtual bool writeContentAlways(std::string_view cont) = 0;

    /**
     * @brief Прочитать содержимое файла (альтернативная версия)
     * @return Содержимое файла
     */
    virtual std::string readContentAlways() const = 0;
};

#endif