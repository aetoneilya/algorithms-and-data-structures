#ifndef LIST_H_
#define LIST_H_

#include <memory>
#include <utility>

template <typename T>
struct list {
 private:
  struct list_node;
  struct list_iterator;
  struct reverse_list_iterator;

 public:
  using value_type = T;
  using reference = value_type &;
  using const_reference = value_type const &;
  using iterator = list_iterator;
  using reverse_iterator = reverse_list_iterator;
  using size_type = std::size_t;

  /// Конструктор
  list();
  explicit list(size_type n);
  explicit list(std::initializer_list<value_type> const &items);
  /// Конструктор копирования
  list(list const &l);
  list(list &&l);

  /// Деструктор
  ~list();

  //   list &operator=(list &&l);

  /// Опрос размера списка
  size_type size() const noexcept;

  /// Очистка списка
  void clear();

  /// Проверка списка на пустоту
  bool empty() const noexcept;

  /// Опрос наличия заданного значения
  bool contains(const_reference value);

  /// Чтение значения с заданным номером в списке и изменение значения с
  /// заданным номером в списке
  reference at(size_type index);

  /// Получение позиции в списке для заданного значения
  size_type get_position(const_reference value);

  /// Включение нового значения
  void push_front(const_reference value);
  void push_back(const_reference value);
  template <typename... Args>
  void emplace_back(Args &&...args);
  template <typename... Args>
  void emplace_front(Args &&...args);

  // Включение нового значения в позицию с заданным номером
  iterator insert(iterator pos, const_reference value);
  template <typename... Args>
  iterator emplace(size_type pos, Args &&...args);
  template <typename... Args>
  iterator emplace(iterator pos, Args &&...args);

  //Удаление заданного значения из списка
  void remove_value(const_reference value);

  // Удаление значения из позиции с заданным номером
  void erase(size_type ind);
  void erase(iterator pos);

  reference front();
  const_reference front() const;
  reference back();
  const_reference back() const;

  //запрос прямого итератора
  //запрос «неустановленного» прямого итератора end()
  iterator begin() noexcept;
  iterator end() noexcept;

  //запрос обратного итератора rbegin()
  //запрос «неустановленного» обратного итератора rend()
  reverse_iterator rbegin() noexcept;
  reverse_iterator rend() noexcept;

  size_type max_size() const noexcept;

  void pop_back();
  void pop_front();
  void swap(list &other);

 private:
  size_type _size;
  list_node *_head;
  list_node *_tail;

  std::allocator<list_node> _a;
  using _List_node_manager = std::allocator_traits<std::allocator<list_node>>;

  void insert(iterator _pos, list_node *_node = nullptr) noexcept;

  struct list_node {
    list_node *_prev;
    list_node *_next;
    value_type _data = {};

    list_node() noexcept : _prev(this), _next(this) {}

    explicit list_node(const_reference data)
        : _prev(this), _next(this), _data(data) {}

    ~list_node() noexcept {
      _prev->_next = _next;
      _next->_prev = _prev;
    }
  };

  struct list_iterator {
    using _Self = list_iterator;

    list_node *_node;

    explicit list_iterator(list_node *node) noexcept : _node(node) {}

    reference operator*() const noexcept { return _node->_data; }

    _Self &operator++() noexcept {
      _node = _node->_next;
      return *this;
    }

    _Self operator++(int) noexcept {
      auto it = *this;
      ++(*this);
      return it;
    }

    _Self &operator--() noexcept {
      _node = _node->_prev;
      return *this;
    }

    _Self operator--(int) noexcept {
      auto it = *this;
      --(*this);
      return it;
    }

    bool operator==(_Self const &other) const noexcept {
      return _node == other._node;
    }

    bool operator!=(_Self const &other) const noexcept {
      return _node != other._node;
    }
  };

  struct reverse_list_iterator {
    using _Self = reverse_list_iterator;

    list_node *_node;

    explicit reverse_list_iterator(list_node *node) noexcept : _node(node) {}

    reference operator*() const noexcept { return _node->_data; }

    _Self &operator++() noexcept {
      _node = _node->_prev;
      return *this;
    }

    _Self operator++(int) noexcept {
      auto it = *this;
      ++(*this);
      return it;
    }

    _Self &operator--() noexcept {
      _node = _node->_next;
      return *this;
    }

    _Self operator--(int) noexcept {
      auto it = *this;
      --(*this);
      return it;
    }

    bool operator==(_Self const &other) const noexcept {
      return _node == other._node;
    }

    bool operator!=(_Self const &other) const noexcept {
      return _node != other._node;
    }
  };
};

template <typename T>
list<T>::list()
    : _size(0),
      _head(_List_node_manager::allocate(_a, 1)),
      _tail(_List_node_manager::allocate(_a, 1)) {
  _head->_next = _tail;
  _head->_prev = _head;
  _tail->_next = _tail;
  _tail->_prev = _head;
}

template <typename T>
list<T>::list(size_type n) : list() {
  while (n--) {
    auto node = _List_node_manager::allocate(_a, 1);
    _List_node_manager::construct(_a, node);

    insert(end(), node);
  }
}

template <typename T>
list<T>::list(std::initializer_list<value_type> const &items) : list() {
  for (auto it = items.begin(); it != items.end(); ++it) {
    push_back(*it);
  }
}

template <typename T>
list<T>::list(list const &l) : list() {
  for (auto it = l.begin(); it != l.end(); ++it) {
    push_back(*it);
  }
}

template <typename T>
list<T>::list(list &&l) : list() {
  swap(l);
}

template <typename T>
list<T>::~list() {
  clear();

  _List_node_manager::deallocate(_a, _head, 1);
}

// template <typename T>
// list<T> &list<T>::operator=(list &&l) {
//   if (this != &l) {
//     list(std::move(l)).swap(*this);
//   }
//   return *this;
// }

template <typename T>
typename list<T>::reference list<T>::front() {
  return _head->_next->_data;
}

template <typename T>
typename list<T>::const_reference list<T>::front() const {
  return _head->_next->_data;
}

template <typename T>
typename list<T>::reference list<T>::back() {
  return _tail->_prev->_data;
}

template <typename T>
typename list<T>::const_reference list<T>::back() const {
  return _tail->_prev->_data;
}

template <typename T>
typename list<T>::iterator list<T>::begin() noexcept {
  return list_iterator(_head->_next);
}

template <typename T>
typename list<T>::iterator list<T>::end() noexcept {
  return list_iterator(_tail);
}

template <typename T>
typename list<T>::reverse_iterator list<T>::rbegin() noexcept {
  return reverse_list_iterator(_tail->_prev);
}

template <typename T>
typename list<T>::reverse_iterator list<T>::rend() noexcept {
  return reverse_list_iterator(_head);
}

template <typename T>
bool list<T>::empty() const noexcept {
  return _head->_next == _tail;
}

template <typename T>
typename list<T>::size_type list<T>::size() const noexcept {
  return _size;
}

template <typename T>
typename list<T>::size_type list<T>::max_size() const noexcept {
  return _List_node_manager::max_size(_a);
}

template <typename T>
void list<T>::clear() {
  while (!empty()) {
    pop_front();
  }
}

template <typename T>
typename list<T>::iterator list<T>::insert(iterator pos,
                                           const_reference value) {
  auto node = _List_node_manager::allocate(_a, 1);
  _List_node_manager::construct(_a, node, value);

  insert(pos, node);

  return list_iterator(node);
}

template <typename T>
void list<T>::insert(iterator _pos, list_node *_node) noexcept {
  auto pos = _pos._node->_prev;

  _node->_prev = pos;
  _node->_next = pos->_next;

  pos->_next->_prev = _node;
  pos->_next = _node;

  ++_size;
}

template <typename T>
template <typename... Args>
typename list<T>::iterator list<T>::emplace(iterator pos, Args &&...args) {
  ([&]() { insert(pos, args); }(), ...);
  return --pos;
}

template <typename T>
void list<T>::remove_value(const_reference value) {
  for (auto it = begin(); it != end(); it++) {
    if (*it == value) {
      auto next = ++it;
      erase(it);
    }
  }
}

template <typename T>
void list<T>::erase(size_type ind) {
  auto it = begin();
  for (int i = 0; i < ind; i++) {
    it++;
  }
  erase(it);
}

template <typename T>
template <typename... Args>
typename list<T>::iterator list<T>::emplace(size_type ind, Args &&...args) {
  auto pos = at(ind);
  ([&]() { insert(pos, args); }(), ...);
  return --pos;
}

template <typename T>
void list<T>::erase(iterator pos) {
  if (!empty()) {
    _List_node_manager::destroy(_a, pos._node);
    _List_node_manager::deallocate(_a, pos._node, 1);

    --_size;
  }
}

template <typename T>
void list<T>::push_back(const_reference value) {
  insert(end(), value);
}

template <typename T>
template <typename... Args>
void list<T>::emplace_back(Args &&...args) {
  ([&]() { push_back(args); }(), ...);
}

template <typename T>
void list<T>::pop_back() {
  if (!empty()) {
    erase(--end());
  }
}

template <typename T>
void list<T>::push_front(const_reference value) {
  insert(begin(), value);
}

template <typename T>
template <typename... Args>
void list<T>::emplace_front(Args &&...args) {
  ([&]() { push_front(args); }(), ...);
}

template <typename T>
bool list<T>::contains(const_reference value) {
  for (auto it = begin(); it != end(); it++) {
    if (*it == value) return true;
  }
  return false;
};

template <typename T>
typename list<T>::reference list<T>::at(size_type index) {
  if (index > _size) throw std::logic_error("Index out of range");

  auto it = begin();
  for (int i = 0; i < index; i++) it++;

  return *it;
}

template <typename T>
typename list<T>::size_type list<T>::get_position(const_reference value) {
  size_type count = 0;
  for (auto it = begin(); it != end(); it++) {
    if (*it == value) return count;
    count++;
  }
  return -1;
}

template <typename T>
void list<T>::pop_front() {
  erase(begin());
}

template <typename T>
void list<T>::swap(list &other) {
  std::swap(_head, other._head);
  std::swap(_tail, other._tail);
  std::swap(_size, other._size);
}

#endif  // LIST_H_