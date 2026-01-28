#ifndef TABLE_H
#define TABLE_H

#include <algorithm>
#include <cstddef>
#include "iterator.h"
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <limits>

/**
 * @brief Ассоциативный контейнер с отсортированными ключами.
 *
 * Реализует интерфейс, аналогичный std::map, но с непрерывным хранением
 * данных для лучшей производительности при итерации.
 *
 * @tparam Key Тип ключа, должен поддерживать операторы сравнения
 * @tparam T Тип значения
 */
template<typename Key, typename T>
class Table {
private:
    TablePair<Key, T>* data_;   ///< Указатель на массив пар ключ-значение
    size_t size_;               ///< Текущее количество элементов
    size_t capacity_;           ///< Выделенная емкость массива

    static constexpr size_t INITIAL_CAPACITY = 16;  ///< Начальная емкость
    static constexpr size_t GROWTH_FACTOR = 2;      ///< Коэффициент роста

    /**
     * @brief Бинарный поиск позиции для ключа.
     * @param key Ключ для поиска
     * @return Позиция, где должен находиться ключ
     */
    size_t binary_search(const Key& key) const noexcept {
        size_t left = 0, right = size_;
        while (left < right) {
            size_t mid = left + (right - left) / 2;
            if (data_[mid].key < key) left = mid + 1;
            else right = mid;
        }
        return left;
    }

    /**
     * @brief Проверить, находится ли ключ на указанной позиции.
     * @param pos Позиция для проверки
     * @param key Ключ для сравнения
     * @return true если ключ найден на позиции, иначе false
     */
    bool key_at_position(size_t pos, const Key& key) const noexcept {
        return pos < size_ && data_[pos].key == key;
    }

    /**
     * @brief Безопасное перераспределение памяти.
     * @param new_capacity Новая емкость
     */
    void safe_reallocate(size_t new_capacity) {
        if (new_capacity == capacity_) return;
        TablePair<Key, T>* new_data = static_cast<TablePair<Key, T>*>(::operator new(new_capacity * sizeof(TablePair<Key, T>)));
        size_t constructed = 0;
        try {
            for (; constructed < size_; constructed++) {
                new (&new_data[constructed]) TablePair<Key, T>(std::move_if_noexcept(data_[constructed]));
            }
        } catch (...) {
            for (size_t i = 0; i < constructed; i++) {
                new_data[i].~TablePair<Key, T>();
            }
            ::operator delete(new_data);
            throw;
        }
        for (size_t i = 0; i < size_; i++) {
            data_[i].~TablePair<Key, T>();
        }
        ::operator delete(data_);
        data_ = new_data;
        capacity_ = new_capacity;
    }

    /**
     * @brief Обеспечить достаточную емкость для новых элементов.
     * @param required Требуемая емкость
     */
    void ensure_capacity(size_t required) {
        if (required > capacity_) {
            size_t new_cap = (capacity_ == 0) ? INITIAL_CAPACITY : capacity_;
            while (new_cap < required) new_cap *= GROWTH_FACTOR;
            safe_reallocate(new_cap);
        }
    }

    /**
     * @brief Сдвинуть элементы вправо начиная с позиции, освобождая место для вставки.
     * @param pos Позиция, с которой начинается сдвиг
     */
    void shift_right(size_t pos) {
        ensure_capacity(size_ + 1);
        for (size_t i = size_; i > pos; --i) {
            new (&data_[i]) TablePair<Key, T>(std::move_if_noexcept(data_[i - 1]));
            data_[i - 1].~TablePair<Key, T>();
        }
    }

    /**
     * @brief Сдвинуть элементы влево начиная с позиции, удаляя элемент.
     * @param pos Позиция, с которой начинается сдвиг
     */
    void shift_left(size_t pos) noexcept {
        data_[pos].~TablePair<Key, T>();
        for (size_t i = pos; i < size_ - 1; i++) {
            new (&data_[i]) TablePair<Key, T>(std::move_if_noexcept(data_[i + 1]));
            data_[i + 1].~TablePair<Key, T>();
        }
        size_--;
    }

public:
    using value_type = TablePair<Key, T>;
    using reference = TablePair<Key, T>&;
    using const_reference = const TablePair<Key, T>&;
    using iterator = TableIterator<Key, T, false>;
    using const_iterator = TableIterator<Key, T, true>;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    /**
     * @brief Конструктор по умолчанию.
     */
    Table() noexcept : data_(nullptr), size_(0), capacity_(0) {}

    /**
     * @brief Конструктор из списка инициализации.
     * @param init Список пар ключ-значение
     */
    Table(std::initializer_list<value_type> init) : data_(nullptr), size_(0), capacity_(0) {
        try {
            reserve(init.size());
            for (const auto& p : init) {
                insert(p);
            }
        } catch (...) {
            clear();
            ::operator delete(data_);
            throw;
        }
    }

    /**
     * @brief Конструктор копирования.
     * @param other Таблица для копирования
     */
    Table(const Table& other) : data_(nullptr), size_(0), capacity_(0) {
        if (other.size_ > 0) {
            try {
                data_ = static_cast<TablePair<Key, T>*>(::operator new(other.size_ * sizeof(TablePair<Key, T>)));
                capacity_ = other.size_;
                for (; size_ < other.size_; size_++) {
                    new (&data_[size_]) TablePair<Key, T>(other.data_[size_]);
                }
            } catch (...) {
                for (size_t i = 0; i < size_; i++) {
                    data_[i].~TablePair<Key, T>();
                }
                ::operator delete(data_);
                throw;
            }
        }
    }

    /**
     * @brief Конструктор перемещения.
     * @param other Таблица для перемещения
     */
    Table(Table&& other) noexcept : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    /**
     * @brief Конструктор из диапазона итераторов.
     * @param first Итератор начала
     * @param last Итератор конца
     */
    template<class InputIt>
    Table(InputIt first, InputIt last) : data_(nullptr), size_(0), capacity_(0) {
        if constexpr (std::is_convertible_v<typename std::iterator_traits<InputIt>::iterator_category, std::random_access_iterator_tag>) {
            reserve(std::distance(first, last));
        } else reserve(8);
        try {
            for (; first != last; first++) {
                insert(value_type(first->first, first->second));
            }
        } catch (...) {
            clear();
            ::operator delete(data_);
            data_ = nullptr;
            capacity_ = 0;
            throw;
        }
    }

    /**
     * @brief Копирующий оператор присваивания.
     * @param other Таблица для копирования
     * @return Ссылка на эту таблицу
     */
    Table& operator=(const Table& other) {
        if (this == &other) return *this;
        Table temp(other);
        swap(temp);
        return *this;
    }

    /**
     * @brief Перемещающий оператор присваивания.
     * @param other Таблица для перемещения
     * @return Ссылка на эту таблицу
     */
    Table& operator=(Table&& other) noexcept {
        if (this == &other) return *this;
        clear();
        ::operator delete(data_);
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
        return *this;
    }

    /**
     * @brief Деструктор.
     */
    ~Table() noexcept {
        clear();
        ::operator delete(data_);
    }

    /**
     * @brief Получить итератор на начало.
     * @return Итератор на первый элемент
     */
    iterator begin() noexcept { return iterator(data_); }

    /**
     * @brief Получить константный итератор на начало.
     * @return Константный итератор на первый элемент
     */
    const_iterator begin() const noexcept { return const_iterator(data_); }

    /**
     * @brief Получить константный итератор на начало.
     * @return Константный итератор на первый элемент
     */
    const_iterator cbegin() const noexcept { return const_iterator(data_); }

    /**
     * @brief Получить итератор на конец.
     * @return Итератор за последним элементом
     */
    iterator end() noexcept { return iterator(data_ + size_); }

    /**
     * @brief Получить константный итератор на конец.
     * @return Константный итератор за последним элементом
     */
    const_iterator end() const noexcept { return const_iterator(data_ + size_); }

    /**
     * @brief Получить константный итератор на конец.
     * @return Константный итератор за последним элементом
     */
    const_iterator cend() const noexcept { return const_iterator(data_ + size_); }

    /**
     * @brief Получить обратный итератор на начало.
     * @return Обратный итератор на первый элемент с конца
     */
    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }

    /**
     * @brief Получить константный обратный итератор на начало.
     * @return Константный обратный итератор на первый элемент с конца
     */
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }

    /**
     * @brief Получить константный обратный итератор на начало.
     * @return Константный обратный итератор на первый элемент с конца
     */
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

    /**
     * @brief Получить обратный итератор на конец.
     * @return Обратный итератор за последним элементом с конца
     */
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }

    /**
     * @brief Получить константный обратный итератор на конец.
     * @return Константный обратный итератор за последним элементом с конца
     */
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }

    /**
     * @brief Получить константный обратный итератор на конец.
     * @return Константный обратный итератор за последним элементом с конца
     */
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

    /**
     * @brief Проверить, пуста ли таблица.
     * @return true если таблица пуста
     */
    bool empty() const noexcept { return size_ == 0; }

    /**
     * @brief Получить размер таблицы.
     * @return Количество элементов
     */
    size_type size() const noexcept { return size_; }

    /**
     * @brief Получить максимально возможный размер.
     * @return Максимальный размер
     */
    size_type max_size() const noexcept {
        return std::numeric_limits<size_type>::max() / sizeof(TablePair<Key, T>);
    }

    /**
     * @brief Очистить таблицу.
     */
    void clear() noexcept {
        for (size_t i = 0; i < size_; i++) {
            data_[i].~TablePair<Key, T>();
        }
        size_ = 0;
    }

    /**
     * @brief Вставить пару ключ-значение.
     * @param value Пара для вставки
     * @return Пара из итератора и флага успеха
     */
    std::pair<iterator, bool> insert(const value_type& value) {
        size_t pos = binary_search(value.key);
        if (key_at_position(pos, value.key)) return {iterator(data_ + pos), false};
        shift_right(pos);
        new (&data_[pos]) TablePair<Key, T>(value.key, value.value);
        size_++;
        return {iterator(data_ + pos), true};
    }

    /**
     * @brief Вставить пару ключ-значение с перемещением.
     * @param value Пара для вставки
     * @return Пара из итератора и флага успеха
     */
    std::pair<iterator, bool> insert(value_type&& value) {
        size_t pos = binary_search(value.key);
        if (key_at_position(pos, value.key)) return {iterator(data_ + pos), false};
        shift_right(pos);
        new (&data_[pos]) TablePair<Key, T>(value.key, std::move(value.value));
        size_++;
        return {iterator(data_ + pos), true};
    }

    /**
     * @brief Вставить элементы из диапазона.
     * @param first Итератор начала
     * @param last Итератор конца
     */
    template<class InputIt>
    void insert(InputIt first, InputIt last) {
        for (; first != last; first++) {
            insert(*first);
        }
    }

    /**
     * @brief Вставить элементы из списка инициализации.
     * @param ilist Список инициализации
     */
    void insert(std::initializer_list<value_type> ilist) {
        for (const auto& val : ilist) {
            insert(val);
        }
    }

    /**
     * @brief Вставить или присвоить значение с перемещением.
     * @param key Ключ
     * @param value Значение
     * @return Пара из итератора и флага вставки
     */
    template<typename K, typename V>
    std::pair<iterator, bool> insert_or_assign(K&& key, V&& value) {
        size_t pos = binary_search(key);
        if (key_at_position(pos, key)) {
            data_[pos].value = std::forward<V>(value);
            return {iterator(data_ + pos), false};
        }
        shift_right(pos);
        new (&data_[pos]) TablePair<Key, T>(std::forward<K>(key), std::forward<V>(value));
        size_++;
        return {iterator(data_ + pos), true};
    }

    /**
     * @brief Создать элемент на месте.
     * @param args Аргументы для конструктора
     * @return Пара из итератора и флага успеха
     */
    template <class... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        TablePair<Key, T> temp(std::forward<Args>(args)...);
        size_t pos = binary_search(temp.key);
        if (key_at_position(pos, temp.key)) return {iterator(data_ + pos), false};
        shift_right(pos);
        try {
            new (&data_[pos]) TablePair<Key, T>(std::move(temp));
            size_++;
        } catch (...) {
            for (size_t i = pos; i < size_; i++) {
                new (&data_[i]) TablePair<Key, T>(std::move_if_noexcept(data_[i + 1]));
                data_[i + 1].~TablePair<Key, T>();
            }
            throw;
        }
        return {iterator(data_ + pos), true};
    }

    /**
     * @brief Попытаться создать элемент с перемещением ключа.
     * @param args Аргументы для конструктора
     * @return Пара из итератора и флага успеха
     */
    template <class... Args>
    std::pair<iterator, bool> try_emplace(Key&& key, Args&&... args) {
        size_t pos = binary_search(key);
        if (key_at_position(pos, key)) return {iterator(data_ + pos), false};
        shift_right(pos);
        try {
            new (&data_[pos]) TablePair<Key, T>(std::move(key), T(std::forward<Args>(args)...));
            size_++;
        } catch (...) {
            for (size_t i = pos; i < size_; i++) {
                new (&data_[i]) TablePair<Key, T>(std::move_if_noexcept(data_[i + 1]));
                data_[i + 1].~TablePair<Key, T>();
            }
            throw;
        }
        return {iterator(data_ + pos), true};
    }

    /**
     * @brief Удалить элемент по константному итератору.
     * @param pos Итератор на элемент
     * @return Итератор на следующий элемент
     */
    iterator erase(const_iterator pos) {
        size_t index = pos.base() - data_;
        if (index >= size_) return end();
        shift_left(index);
        return iterator(data_ + index);
    }

    /**
     * @brief Удалить элемент по итератору.
     * @param pos Итератор на элемент
     * @return Итератор на следующий элемент
     */
    iterator erase(iterator pos) {
        return erase(const_iterator(pos.base()));
    }

    /**
     * @brief Удалить диапазон элементов.
     * @param first Конствнтный итератор начала
     * @param last Константный итератор конца
     * @return Итератор на элемент после удаленного диапазона
     */
    iterator erase(const_iterator first, const_iterator last) {
        if (first == last) return iterator(const_cast<TablePair<Key, T>*>(last.base()));
        size_t start_idx = first.base() - data_;
        size_t end_idx = last.base() - data_;
        size_t count = end_idx - start_idx;
        for (size_t i = start_idx; i < end_idx; i++) {
            data_[i].~TablePair<Key, T>();
        }
        for (size_t i = end_idx; i < size_; i++) {
            new (&data_[i - count]) TablePair<Key, T>(std::move_if_noexcept(data_[i]));
            data_[i].~TablePair<Key, T>();
        }
        size_ -= count;
        return iterator(data_ + start_idx);
    }

    /**
     * @brief Удалить диапазон элементов.
     * @param first Итератор начала
     * @param last Итератор конца
     * @return Итератор на элемент после удаленного диапазона
     */
    iterator erase(iterator first, iterator last) {
        return erase(const_iterator(first), const_iterator(last));
    }

    /**
     * @brief Удалить элемент по ключу.
     * @param key Ключ
     * @return Количество удаленных элементов (0 или 1)
     */
    size_type erase(const Key& key) {
        size_t pos = binary_search(key);
        if (!key_at_position(pos, key)) return 0;
        shift_left(pos);
        return 1;
    }

    /**
     * @brief Обменять содержимое с другой таблицей.
     * @param other Другая таблица
     */
    void swap(Table& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    /**
     * @brief Получить количество элементов с ключом.
     * @param key Ключ
     * @return 0 или 1
     */
    size_type count(const Key& key) const noexcept {
        size_t pos = binary_search(key);
        return key_at_position(pos, key) ? 1 : 0;
    }

    /**
     * @brief Найти элемент по ключу.
     * @param key Ключ
     * @return Итератор на элемент или end()
     */
    iterator find(const Key& key) noexcept {
        size_t pos = binary_search(key);
        return key_at_position(pos, key) ? iterator(data_ + pos) : end();
    }

    /**
     * @brief Найти элемент по ключу (константная версия).
     * @param key Ключ
     * @return Константный итератор на элемент или end()
     */
    const_iterator find(const Key& key) const noexcept {
        size_t pos = binary_search(key);
        return key_at_position(pos, key) ? const_iterator(data_ + pos) : end();
    }

    /**
     * @brief Проверить наличие ключа.
     * @param key Ключ
     * @return true если ключ присутствует
     */
    bool contains(const Key& key) const noexcept {
        size_t pos = binary_search(key);
        return key_at_position(pos, key);
    }

    /**
     * @brief Получить итератор на первый элемент не меньше ключа.
     * @param key Ключ
     * @return Итератор
     */
    iterator lower_bound(const Key& key) noexcept {
        return iterator(data_ + binary_search(key));
    }

    /**
     * @brief Получить константный итератор на первый элемент не меньше ключа.
     * @param key Ключ
     * @return Константный итератор
     */
    const_iterator lower_bound(const Key& key) const noexcept {
        return const_iterator(data_ + binary_search(key));
    }

    /**
     * @brief Получить итератор на первый элемент больше ключа.
     * @param key Ключ
     * @return Итератор
     */
    iterator upper_bound(const Key& key) noexcept {
        size_t pos = binary_search(key);
        if (key_at_position(pos, key)) pos++;
        return iterator(data_ + pos);
    }

    /**
     * @brief Получить константный итератор на первый элемент больше ключа.
     * @param key Ключ
     * @return Константный итератор
     */
    const_iterator upper_bound(const Key& key) const noexcept {
        size_t pos = binary_search(key);
        if (key_at_position(pos, key)) pos++;
        return const_iterator(data_ + pos);
    }

    /**
     * @brief Получить диапазон элементов с ключом.
     * @param key Ключ
     * @return Пара итераторов
     */
    std::pair<iterator, iterator> equal_range(const Key& key) noexcept {
        iterator lb = lower_bound(key);
        iterator ub = (lb != end() && lb->key == key) ? lb + 1 : lb;
        return {lb, ub};
    }

    /**
     * @brief Получить константный диапазон элементов с ключом.
     * @param key Ключ
     * @return Пара константных итераторов
     */
    std::pair<const_iterator, const_iterator> equal_range(const Key& key) const noexcept {
        const_iterator lb = lower_bound(key);
        const_iterator ub = (lb != end() && lb->key == key) ? lb + 1 : lb;
        return {lb, ub};
    }

    /**
     * @brief Доступ к элементу по ключу с проверкой.
     * @param key Ключ
     * @return Ссылка на значение
     * @throws std::out_of_range если ключ не найден
     */
    T& at(const Key& key) {
        size_t pos = binary_search(key);
        if (!key_at_position(pos, key)) throw std::out_of_range("Table::at: key not found");
        return data_[pos].value;
    }

    /**
     * @brief Доступ к элементу по ключу с проверкой (константная версия).
     * @param key Ключ
     * @return Константная ссылка на значение
     * @throws std::out_of_range если ключ не найден
     */
    const T& at(const Key& key) const {
        size_t pos = binary_search(key);
        if (!key_at_position(pos, key)) throw std::out_of_range("Table::at: key not found");
        return data_[pos].value;
    }

    /**
     * @brief Доступ к элементу по ключу с вставкой при отсутствии.
     * @param key Ключ
     * @return Ссылка на значение
     */
    T& operator[](const Key& key) {
        size_t pos = binary_search(key);
        if (key_at_position(pos, key)) return data_[pos].value;
        shift_right(pos);
        new (&data_[pos]) TablePair<Key, T>(key, T());
        size_++;
        return data_[pos].value;
    }

    /**
     * @brief Доступ к элементу по ключу с вставкой при отсутствии (rvalue).
     * @param key Ключ
     * @return Ссылка на значение
     */
    T& operator[](Key&& key) {
        size_t pos = binary_search(key);
        if (key_at_position(pos, key)) return data_[pos].value;
        shift_right(pos);
        new (&data_[pos]) TablePair<Key, T>(std::move(key), T());
        size_++;
        return data_[pos].value;
    }

    /**
     * @brief Зарезервировать память.
     * @param new_cap Новая емкость
     */
    void reserve(size_t new_cap) {
        if (new_cap > capacity_) safe_reallocate(new_cap);
    }

    /**
     * @brief Получить емкость.
     * @return Текущая емкость
     */
    size_type capacity() const noexcept { return capacity_; }

    /**
     * @brief Уменьшить емкость до размера.
     */
    void shrink_to_fit() {
        if (size_ < capacity_) safe_reallocate(size_);
    }

    // /**
    //  * @brief Извлечь значение по ключу.
    //  * @param key Ключ
    //  * @return Значение
    //  * @throws std::out_of_range если ключ не найден
    //  */
    // T extract(const Key& key) { // не соответствует ствндарту, но мб node_type можно так заменить
    //     size_t pos = binary_search(key);
    //     if (!key_at_position(pos, key)) {
    //         throw std::out_of_range("Table::extract: key not found");
    //     }
    //     T value = std::move(data_[pos].value);
    //     shift_left(pos);
    //     return value;
    // }
};

/**
 * @brief Оператор равенства.
 * @param lhs Левая таблица
 * @param rhs Правая таблица
 * @return true если таблицы равны
 */
template<typename Key, typename T>
bool operator==(const Table<Key, T>& lhs, const Table<Key, T>& rhs) noexcept {
    if (lhs.size() != rhs.size()) return false;
    for (auto it1 = lhs.begin(), it2 = rhs.begin(); it1 != lhs.end(); ++it1, ++it2) {
        if (it1->key != it2->key || it1->value != it2->value) return false;
    }
    return true;
}

/**
 * @brief Оператор неравенства.
 * @param lhs Левая таблица
 * @param rhs Правая таблица
 * @return true если таблицы не равны
 */
template<typename Key, typename T>
bool operator!=(const Table<Key, T>& lhs, const Table<Key, T>& rhs) noexcept { return !(lhs == rhs); }

/**
 * @brief Оператор "меньше".
 * Лексикографическое сравнение.
 * @param lhs Левая таблица
 * @param rhs Правая таблица
 * @return true если левая таблица меньше правой
 */
template<typename Key, typename T>
bool operator<(const Table<Key, T>& lhs, const Table<Key, T>& rhs) noexcept {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
        [](const TablePair<Key, T>& a, const TablePair<Key, T>& b) {
            if (a.key < b.key) return true;
            if (b.key < a.key) return false;
            return a.value < b.value;
        }
    );
}

/**
 * @brief Оператор "меньше или равно".
 * @param lhs Левая таблица
 * @param rhs Правая таблица
 * @return true если левая таблица меньше или равна правой
 */
template<typename Key, typename T>
bool operator<=(const Table<Key, T>& lhs, const Table<Key, T>& rhs) noexcept { return !(rhs < lhs); }

/**
 * @brief Оператор "больше".
 * @param lhs Левая таблица
 * @param rhs Правая таблица
 * @return true если левая таблица больше правой
 */
template<typename Key, typename T>
bool operator>(const Table<Key, T>& lhs, const Table<Key, T>& rhs) noexcept { return rhs < lhs; }

/**
 * @brief Оператор "больше или равно".
 * @param lhs Левая таблица
 * @param rhs Правая таблица
 * @return true если левая таблица больше или равна правой
 */
template<typename Key, typename T>
bool operator>=(const Table<Key, T>& lhs, const Table<Key, T>& rhs) noexcept { return !(lhs < rhs); }

/**
 * @brief std::swap для Table.
 */
template<typename Key, typename T>
void swap(Table<Key, T>& lhs, Table<Key, T>& rhs) noexcept { lhs.swap(rhs); }

#endif