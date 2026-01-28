#ifndef LAB3_I_LOCKABLE_H
#define LAB3_I_LOCKABLE_H

#include "base.h"

/**
 * @brief Интерфейс блокируемого объекта.
 *
 * Определяет методы для управления блокировками и проверки состояния блокировки.
 */
class ILockable {
public:
    virtual ~ILockable() = default;

    /**
     * @brief Установить режим блокировки
     * @param m Новый режим блокировки
     */
    virtual void setMode(Lock m) = 0;

    /**
     * @brief Проверить доступность объекта для чтения
     * @return true если объект доступен для чтения, иначе false
     */
    virtual bool isReadable() const = 0;

    /**
     * @brief Проверить доступность объекта для записи
     * @return true если объект доступен для записи, иначе false
     */
    virtual bool isWritable() const = 0;
};

#endif