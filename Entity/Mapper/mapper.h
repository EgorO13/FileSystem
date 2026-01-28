#ifndef LAB3_MAPPER_H
#define LAB3_MAPPER_H

/**
 * @brief Шаблонный базовый класс для преобразования объектов между моделью и DTO.
 *
 * Определяет интерфейс для мапперов, которые преобразуют объекты из типа From в тип To
 * и обратно, с возможностью использования оберток для управления памятью.
 *
 * @tparam From Исходный тип (доменная модель)
 * @tparam To Целевой тип (DTO)
 * @tparam FromWrap Тип-обертка для исходного типа (по умолчанию From)
 * @tparam ToWrap Тип-обертка для целевого типа (по умолчанию To)
 */
template<typename From, typename To, typename FromWrap = From, typename ToWrap = To>
class Mapper {
public:
    using FromType = From;        ///< Исходный тип
    using ToType = To;            ///< Целевой тип
    using FromWrapType = FromWrap; ///< Тип-обертка для исходного типа
    using ToWrapType = ToWrap;    ///< Тип-обертка для целевого типа

    virtual ~Mapper() = default;

    /**
     * @brief Преобразует объект из доменной модели в DTO
     * @param from Исходный объект доменной модели
     * @return Объект DTO в обертке
     */
    [[nodiscard]] virtual ToWrap mapTo(const From&) const = 0;

    /**
     * @brief Преобразует объект из DTO в доменную модель
     * @param to Исходный объект DTO
     * @return Объект доменной модели в обертке
     */
    [[nodiscard]] virtual FromWrap mapFrom(const To&) const = 0;
};

#endif