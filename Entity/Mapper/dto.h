#ifndef LAB3_DTO_H
#define LAB3_DTO_H

#include <string>
#include <vector>
#include <map>
#include <chrono>

/**
 * @brief Пространство имен для объектов передачи данных (Data Transfer Objects).
 *
 * Содержит структуры для сериализации и десериализации объектов
 * файловой системы, пользователей, групп и команд.
 */
namespace DTO {

    /**
     * @brief DTO для объектов файловой системы (файлов и директорий).
     *
     * Содержит все необходимые данные для сохранения и восстановления
     * состояния объектов файловой системы.
     */
    struct FileSystemObjectDTO {
        std::string type;                                         ///< Тип объекта: "FILE" или "DIR"
        unsigned int address;                                     ///< Уникальный адрес объекта в файловой системе
        std::string name;                                         ///< Имя объекта
        unsigned int parentAddress;                               ///< Адрес родительской директории
        std::string ownerName;                                    ///< Имя владельца объекта
        unsigned int ownerId;                                     ///< ID владельца объекта
        std::chrono::system_clock::time_point creationTime;      ///< Время создания объекта
        std::chrono::system_clock::time_point lastModifyTime;    ///< Время последнего изменения объекта
        std::map<std::string, std::string> properties;           ///< Дополнительные свойства объекта (содержимое, права доступа и т.д.)
    };

    /**
     * @brief DTO для пользователей файловой системы.
     *
     * Содержит данные пользователя, включая его идентификатор,
     * имя и принадлежность к группам.
     */
    struct UserDTO {
        std::string type = "USER";           ///< Тип
        unsigned int id;                     ///< Уникальный идентификатор пользователя
        std::string name;                    ///< Имя пользователя
        std::vector<unsigned int> groupIds;  ///< Список идентификаторов групп, в которых состоит пользователь
    };

    /**
     * @brief DTO для групп пользователей файловой системы.
     *
     * Содержит данные группы, включая её идентификатор, имя,
     * список пользователей и подгрупп.
     */
    struct GroupDTO {
        std::string type = "GROUP";            ///< Тип
        unsigned int id;                       ///< Уникальный идентификатор группы
        std::string name;                      ///< Имя группы
        std::vector<unsigned int> userIds;     ///< Список идентификаторов пользователей в группе
        std::vector<unsigned int> subgroupIds; ///< Список идентификаторов подгрупп
    };

    /**
     * @brief DTO для записи подкоманды в составной команде.
     *
     * Описывает одну подкоманду в составе составной команды,
     * включая её имя, фиксированные аргументы и индексы динамических аргументов.
     */
    struct SubCommandEntryDTO {
        std::string commandName;                   ///< Имя подкоманды
        std::vector<std::string> fixedArgs;        ///< Список фиксированных аргументов для подкоманды
        std::vector<int> dynamicIndices;           ///< Список индексов динамических аргументов из входных параметров

        SubCommandEntryDTO() = default;

        /**
         * @brief Конструктор записи подкоманды
         * @param name Имя подкоманды
         * @param fixed Фиксированные аргументы (по умолчанию пусто)
         * @param dynamic Индексы динамических аргументов (по умолчанию пусто)
         */
        SubCommandEntryDTO(const std::string& name, const std::vector<std::string>& fixed = {}, const std::vector<int>& dynamic = {})
            : commandName(name), fixedArgs(fixed), dynamicIndices(dynamic) {}
    };

    /**
     * @brief DTO для составных команд.
     *
     * Содержит полное описание составной команды, включая её тип,
     * имя, описание, использование, требование прав администратора
     * и список подкоманд.
     */
    struct CompositeCommandDTO {
        std::string type;                           ///< Тип команды (всегда "composite" для составных команд)
        std::string name;                           ///< Имя составной команды
        std::string description;                    ///< Описание составной команды
        std::string usage;                          ///< Строка использования команды
        bool onlyForAdmin;                          ///< Флаг, указывающий на требование прав администратора
        std::vector<SubCommandEntryDTO> subCommands; ///< Список подкоманд

        CompositeCommandDTO() = default;

        /**
         * @brief Конструктор составной команды
         * @param t Тип команды
         * @param n Имя команды
         * @param desc Описание команды
         * @param u Строка использования команды
         * @param admin Флаг требования прав администратора (по умолчанию false)
         */
        CompositeCommandDTO(const std::string& t, const std::string& n, const std::string& desc, const std::string& u, bool admin = false)
            : type(t), name(n), description(desc), usage(u), onlyForAdmin(admin) {}
    };
}

#endif