#ifndef LAB3_FILE_DESCRIPTOR_H
#define LAB3_FILE_DESCRIPTOR_H

#include "Entity/FSObject/realisation/fs_object.h"
#include "Entity/File/interface/i_file.h"
#include "Entity/File/interface/i_lockable.h"
#include <string>

/**
 * @brief Класс дескриптора файла файловой системы.
 *
 * Реализует интерфейсы файла и блокировки,
 * предоставляет методы для работы с содержимым файла.
 */
class FileDescriptor : public FileSystemObject, public IFile, public ILockable {
private:
    std::string content;    ///< Содержимое файла
    unsigned int size;      ///< Размер файла в байтах
    Lock mode;              ///< Режим блокировки файла

    /**
     * @brief Обновить размер файла на основе содержимого
     */
    void updateFileSize();

public:
    /**
     * @brief Конструктор файла
     * @param name Имя файла
     * @param parentAddress Адрес родительской директории
     * @param owner Владелец файла
     * @param adr Адрес файла в файловой системе
     */
    FileDescriptor(const std::string &name, unsigned int parentAddress, const User &owner, unsigned int adr);

    /**
     * @brief Записать содержимое в файл с проверкой блокировки
     * @param cont Содержимое для записи
     * @return true если запись успешна, иначе false
     * @throws std::runtime_error если файл не доступен для записи
     */
    bool writeContent(std::string_view cont) override;

    /**
     * @brief Прочитать содержимое файла с проверкой блокировки
     * @return Содержимое файла
     * @throws std::runtime_error если файл не доступен для чтения
     */
    std::string readContent() const override;

    /**
     * @brief Обрезать содержимое файла до указанного индекса
     * @param index Индекс до которого обрезать
     * @return true если операция успешна, иначе false
     * @throws std::runtime_error если файл не доступен для записи
     */
    bool truncateContent(int index) override;

    /**
     * @brief Очистить содержимое файла
     * @throws std::runtime_error если файл не доступен для записи
     */
    void clearContent() override;

    /**
     * @brief Получить размер файла
     * @return Размер файла в байтах
     */
    int getSize() const override;

    /**
     * @brief Установить режим блокировки файла
     * @param m Новый режим блокировки
     */
    void setMode(Lock m) override { mode = m; }

    /**
     * @brief Проверить доступность файла для чтения
     * @return true если файл доступен для чтения, иначе false
     */
    bool isReadable() const override { return mode == Lock::NotLock || mode == Lock::WriteLock; }

    /**
     * @brief Проверить доступность файла для записи
     * @return true если файл доступен для записи, иначе false
     */
    bool isWritable() const override { return mode == Lock::NotLock || mode == Lock::ReadLock; }

    /**
     * @brief Записать содержимое в файл без проверки блокировки
     * @param cont Содержимое для записи
     * @return true если запись успешна
     */
    bool writeContentAlways(std::string_view cont) override;

    /**
     * @brief Прочитать содержимое файла без проверки блокировки
     * @return Содержимое файла
     */
    std::string readContentAlways() const override;
};

#endif