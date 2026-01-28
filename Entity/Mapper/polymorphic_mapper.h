#ifndef LAB3_POLYMORPHIC_MAPPER_H
#define LAB3_POLYMORPHIC_MAPPER_H

#include "mapper.h"

#include <map>
#include <string>
#include <typeindex>
#include <functional>

/**
 * @brief Шаблонный класс для мапперов конкретных подтипов.
 *
 * Расширяет базовый Mapper, добавляя возможность идентификации
 * конкретного подтипа по ключу и типу.
 *
 * @tparam From Исходный тип (доменная модель)
 * @tparam To Целевой тип (DTO)
 * @tparam FromWrap Тип-обертка для исходного типа (по умолчанию From)
 * @tparam ToWrap Тип-обертка для целевого типа (по умолчанию To)
 * @tparam Key Тип ключа для идентификации подтипа (по умолчанию std::string)
 */
template<typename From, typename To, typename FromWrap = From, typename ToWrap = To, typename Key = std::string>
class SubtypeMapper: public Mapper<From, To, FromWrap, ToWrap> {
public:
    using KeyType = Key; ///< Тип ключа для идентификации подтипа

    /**
     * @brief Получает ключ, идентифицирующий конкретный подтип
     * @return Ключ подтипа
     */
    [[nodiscard]] virtual Key getKey() const = 0;

    /**
     * @brief Получает typeid конкретного подтипа
     * @return typeid подтипа
     */
    [[nodiscard]] virtual std::type_index getType() const = 0;
};

/**
 * @brief Полиморфный маппер, способный обрабатывать несколько подтипов.
 *
 * Содержит коллекцию конкретных мапперов и автоматически выбирает
 * подходящий маппер на основе типа объекта или ключа в DTO.
 *
 * @tparam From Исходный тип (доменная модель)
 * @tparam To Целевой тип (DTO)
 * @tparam FromWrap Тип-обертка для исходного типа (по умолчанию From)
 * @tparam ToWrap Тип-обертка для целевого типа (по умолчанию To)
 * @tparam Key Тип ключа для идентификации подтипа (по умолчанию std::string)
 */
template<typename From, typename To, typename FromWrap = From, typename ToWrap = To, typename Key = std::string>
class PolymorphicMapper: public Mapper<From, To, FromWrap, ToWrap> {
public:
    using Supertype = Mapper<From, To, FromWrap, ToWrap>; ///< Базовый тип маппера
    using Subtype = SubtypeMapper<From, To, FromWrap, ToWrap, Key>; ///< Тип подмаппера

    /**
     * @brief Добавляет конкретный маппер для подтипа
     * @param subMapper Ссылка на добавляемый маппер
     */
    void addSubMapper(const Subtype& subMapper) {
        concreteMappersByKey_.emplace(subMapper.getKey(), std::cref(subMapper));
        concreteMappersByType_.emplace(subMapper.getType(), std::cref(subMapper));
    }

    /**
     * @brief Получает ключ из DTO для определения подтипа
     * @param dto DTO объект
     * @return Ключ подтипа
     */
    [[nodiscard]] virtual Key getKey(const To&) const = 0;

    /**
     * @brief Преобразует объект из доменной модели в DTO
     * @param from Исходный объект доменной модели
     * @return Объект DTO в обертке
     */
    [[nodiscard]] ToWrap mapTo(const From& from) const override {
        std::type_index type = typeid(from);
        auto it = concreteMappersByType_.find(type);
        return it->second.get().mapTo(from);
    }

    /**
     * @brief Преобразует объект из DTO в доменную модель
     * @param to Исходный объект DTO
     * @return Объект доменной модели в обертке
     */
    [[nodiscard]] FromWrap mapFrom(const To& to) const override {
        auto key = getKey(to);
        auto it = concreteMappersByKey_.find(key);
        return it->second.get().mapFrom(to);
    }

private:
    std::map<Key, std::reference_wrapper<const Subtype>> concreteMappersByKey_; ///< Мапперы по ключу
    std::map<std::type_index, std::reference_wrapper<const Subtype>> concreteMappersByType_; ///< Мапперы по типу
};

/**
 * @brief Концепт, проверяющий, что тип является подмаппером.
 *
 * Требует, чтобы тип M имел определенные алиасы и наследовался
 * от соответствующей инстанциации SubtypeMapper.
 */
template<typename M>
concept IsSubmapper = requires(M v) {
    requires std::derived_from<M, SubtypeMapper<typename M::FromType, typename M::ToType, typename M::FromWrapType, typename M::ToWrapType, typename M::KeyType>>;
};

/**
 * @brief Адаптер для приведения конкретного маппера к базовому типу.
 *
 * Позволяет использовать конкретный маппер для подтипа в контексте
 * полиморфного маппера для базового типа.
 *
 * @tparam SuperFrom Базовый тип доменной модели
 * @tparam SuperFromWrap Тип-обертка для базового типа доменной модели
 * @tparam SubMapper Тип конкретного маппера для подтипа
 */
template<typename SuperFrom, typename SuperFromWrap, IsSubmapper SubMapper>
class SubtypeMapperAdapter final : public SubtypeMapper<SuperFrom, typename SubMapper::ToType, SuperFromWrap, typename SubMapper::ToWrapType,typename SubMapper::KeyType> {
    SubMapper sub_; ///< Вложенный конкретный маппер
    using Super = SubtypeMapper<SuperFrom, typename SubMapper::ToType, SuperFromWrap, typename SubMapper::ToWrapType, typename SubMapper::KeyType>;

public:
    /**
     * @brief Конструктор адаптера с perfect forwarding аргументов
     * @tparam Args Типы аргументов конструктора
     * @param args Аргументы для конструктора вложенного маппера
     */
    template<typename ... Args>
    explicit SubtypeMapperAdapter(Args&& ... args) requires std::constructible_from<SubMapper, Args&&...>:
        sub_(std::forward<Args>(args)...) {}

    /**
     * @brief Преобразует объект из доменной модели в DTO
     * @param from Исходный объект доменной модели
     * @return Объект DTO в обертке
     */
    [[nodiscard]] Super::ToWrapType mapTo(const Super::FromType& from) const override {
        return sub_.mapTo(dynamic_cast<const SubMapper::FromType&>(from));
    }

    /**
     * @brief Преобразует объект из DTO в доменную модель
     * @param to Исходный объект DTO
     * @return Объект доменной модели в обертке
     */
    [[nodiscard]] SuperFromWrap mapFrom(const Super::ToType& to) const override {
        return sub_.mapFrom(to);
    }

    /**
     * @brief Получает ключ, идентифицирующий конкретный подтип
     * @return Ключ подтипа
     */
    [[nodiscard]] Super::KeyType getKey() const override {
        return sub_.getKey();
    }

    /**
     * @brief Получает typeid конкретного подтипа
     * @return typeid подтипа
     */
    [[nodiscard]] std::type_index getType() const override {
        return sub_.getType();
    }
};

#endif