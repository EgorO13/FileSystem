#ifndef LAB3_ITERATOR_H
#define LAB3_ITERATOR_H

#include <iterator>
#include <type_traits>
#include <cstddef>

/**
 * @brief Пара ключ-значение для таблицы.
 *
 * @tparam Key Тип ключа
 * @tparam T Тип значения
 */
template<typename Key, typename T>
struct TablePair {
    const Key key;  ///< Константный Ключ
    T value;        ///< Значение

    /**
     * @brief Конструктор по умолчанию.
     */
    TablePair() noexcept(std::is_nothrow_default_constructible_v<Key> && std::is_nothrow_default_constructible_v<T>)
            : key(Key()), value(T()) {}

    /**
     * @brief Конструктор с копированием ключа и значения.
     * @param k Ключ
     * @param v Значение
     */
    TablePair(const Key& k, const T& v) : key(k), value(v) {}

    /**
     * @brief Конструктор с перемещением ключа и значения.
     * @param k Ключ
     * @param v Значение
     */
    TablePair(Key&& k, T&& v) noexcept(std::is_nothrow_move_constructible_v<Key> && std::is_nothrow_move_constructible_v<T>)
            : key(std::move(k)), value(std::move(v)) {}

    /**
     * @brief Конструктор с копированием ключа и перемещением значения.
     * @param k Ключ
     * @param v Значение
     */
    TablePair(const Key& k, T&& v) : key(k), value(std::move(v)) {}

    /**
     * @brief Конструктор с перемещением ключа и копированием значения.
     * @param k Ключ
     * @param v Значение
     */
    TablePair(Key&& k, const T& v) : key(std::move(k)), value(v) {}

    /**
     * @brief Конструктор из std::pair
     * @param pair std::pair для копирования
     */
    TablePair(const std::pair<Key, T>& pair) : key(pair.first), value(pair.second) {}

    /**
     * @brief Конструктор из std::pair с перемещением
     * @param pair std::pair для перемещения
     */
    TablePair(std::pair<Key, T>&& pair) : key(std::move(pair.first)), value(std::move(pair.second)) {}

    /**
     * @brief Конструктор копирования.
     */
    TablePair(const TablePair&) = default;

    /**
     * @brief Конструктор перемещения.
     * @param other Объект для перемещения
     */
    TablePair(TablePair&& other) noexcept(std::is_nothrow_move_constructible_v<Key> && std::is_nothrow_move_constructible_v<T>)
        : key(std::move(other.key)), value(std::move(other.value)) {}

    auto operator<=>(const TablePair& other) const { return key <=> other.key; }
    TablePair& operator=(const TablePair&) = delete;
    TablePair& operator=(TablePair&&) = delete;
};

/**
 * @brief Итератор для таблицы с поддержкой случайного доступа.
 *
 * @tparam Key Тип ключа
 * @tparam T Тип значения
 * @tparam IsConst Флаг константности итератора
 */
template<typename Key, typename T, bool IsConst>
class TableIterator {
private:
    using PairType = std::conditional_t<IsConst, const TablePair<Key, T>, TablePair<Key, T>>;
    PairType* ptr; ///< Указатель на текущий элемент

public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = std::conditional_t<IsConst, const TablePair<Key, T>, TablePair<Key, T>>;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;
    using element_type = PairType;

    /**
     * @brief Конструктор по умолчанию.
     */
    TableIterator() noexcept : ptr(nullptr) {}

    /**
     * @brief Конструктор с указателем на элемент.
     * @param p Указатель на элемент таблицы
     */
    explicit TableIterator(PairType* p) noexcept : ptr(p) {}

    /**
     * @brief Преобразование неконстантного итератора в константный.
     */
    template<bool WasConst = IsConst, typename = std::enable_if_t<WasConst>>
    TableIterator(const TableIterator<Key, T, false>& other) noexcept : ptr(other.base()) {}

    /**
     * @brief Оператор разыменования.
     * @return Ссылка на текущий элемент
     */
    reference operator*() const noexcept { return *ptr; }

    /**
     * @brief Оператор доступа к членам.
     * @return Указатель на текущий элемент
     */
    pointer operator->() const noexcept { return ptr; }

    /**
     * @brief Получить ключ текущего элемента.
     * @return Константная ссылка на ключ
     */
    const Key& key() const noexcept { return ptr->key; }

    /**
     * @brief Получить базовый указатель.
     * @return Указатель на текущий элемент
     */
    element_type* base() const noexcept { return ptr; }

    /**
     * @brief Префиксный инкремент.
     * @return Ссылка на этот итератор
     */
    TableIterator& operator++() noexcept { ++ptr; return *this; }

    /**
     * @brief Постфиксный инкремент.
     * @return Копия итератора до инкремента
     */
    TableIterator operator++(int) noexcept { auto tmp = *this; ++ptr; return tmp; }

    /**
     * @brief Префиксный декремент.
     * @return Ссылка на этот итератор
     */
    TableIterator& operator--() noexcept { --ptr; return *this; }

    /**
     * @brief Постфиксный декремент.
     * @return Копия итератора до декремента
     */
    TableIterator operator--(int) noexcept { auto tmp = *this; --ptr; return tmp; }

    /**
     * @brief Оператор сдвига вперед.
     * @param n Количество позиций для сдвига
     * @return Ссылка на этот итератор
     */
    TableIterator& operator+=(difference_type n) noexcept { ptr += n; return *this; }

    /**
     * @brief Оператор сдвига назад.
     * @param n Количество позиций для сдвига
     * @return Ссылка на этот итератор
     */
    TableIterator& operator-=(difference_type n) noexcept { ptr -= n; return *this; }

    /**
     * @brief Оператор разности итераторов.
     * @param other Другой итератор
     * @return Расстояние между итераторами
     */
    difference_type operator-(const TableIterator& other) const noexcept { return ptr - other.ptr; }

    /**
     * @brief Оператор индексирования.
     * @param n Смещение от текущей позиции
     * @return Ссылка на элемент по смещению
     */
    reference operator[](difference_type n) const noexcept { return ptr[n]; }

    /**
     * @brief Оператор равенства.
     * @param other Другой итератор
     * @return true если итераторы указывают на один элемент
     */
    bool operator==(const TableIterator& other) const noexcept { return ptr == other.ptr; }

    /**
     * @brief Оператор неравенства.
     * @param other Другой итератор
     * @return true если итераторы указывают на разные элементы
     */
    bool operator!=(const TableIterator& other) const noexcept { return ptr != other.ptr; }

    /**
     * @brief Оператор меньше.
     * @param other Другой итератор
     * @return true если этот итератор предшествует другому
     */
    bool operator<(const TableIterator& other) const noexcept { return ptr < other.ptr; }

    /**
     * @brief Оператор меньше или равно.
     * @param other Другой итератор
     * @return true если этот итератор предшествует другому или совпадает
     */
    bool operator<=(const TableIterator& other) const noexcept { return ptr <= other.ptr; }

    /**
     * @brief Оператор больше.
     * @param other Другой итератор
     * @return true если этот итератор следует за другим
     */
    bool operator>(const TableIterator& other) const noexcept { return ptr > other.ptr; }

    /**
     * @brief Оператор больше или равно.
     * @param other Другой итератор
     * @return true если этот итератор следует за другим или совпадает
     */
    bool operator>=(const TableIterator& other) const noexcept { return ptr >= other.ptr; }

    /**
     * @brief Оператор вычитания.
     * @param n Количество позиций для сдвига назад
     * @return Новый итератор
     */
    TableIterator operator-(difference_type n) const noexcept { return TableIterator(ptr - n); }

    /**
     * @brief Оператор сложения.
     * @param n Количество позиций для сдвига вперед
     * @return Новый итератор
     */
    TableIterator operator+(difference_type n) const noexcept { return TableIterator(ptr + n); }

    /**
     * @brief Оператор сложения (дружественная функция).
     * @param n Количество позиций для сдвига
     * @param it Итератор для сдвига
     * @return Новый итератор
     */
    friend TableIterator operator+(difference_type n, const TableIterator& it) noexcept { return it + n; }
};

static_assert(std::random_access_iterator<TableIterator<int, int, false>>);
static_assert(std::random_access_iterator<TableIterator<int, int, true>>);

#endif