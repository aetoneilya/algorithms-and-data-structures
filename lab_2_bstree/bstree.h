#ifndef BSTREE_H_
#define BSTREE_H_

#include <functional>
#include <iostream>
#include <memory>
#include <queue>

template <typename T_key, typename T_data>
class Bst {
  class BstIterator;
  class ReverseBstIterator;
  struct Node;

 public:
  using key_type = T_key;
  using value_type = T_data;
  using reference = value_type &;
  using const_reference = value_type const &;
  using iterator = BstIterator;
  using reverse_iterator = ReverseBstIterator;
  using size_type = std::size_t;

 public:
  Bst(){};
  Bst(Bst const &b);
  ~Bst() { delete root_; };

  size_type size() const;       // размер дерева
  void clear();                 // очистка дерева
  bool empty() const noexcept;  // проверка дерева на пустоту

  reference at(key_type k);  // доступ по чтению/записи к данным по ключу
  const reference at(key_type k) const;

  void insert(key_type k, value_type v);  // включение данных с заданным ключом
  void remove(key_type k);  // удаление данных с заданным ключом

  //формирование списка ключей в дереве в порядке обхода узлов по схеме,
  //заданной в варианте задания
  void bf_print() const noexcept;
  void df_print() const noexcept;  //!

  //определение высоты дерева. Трудоёмкость операции – O (n)
  size_type height() const noexcept;

  //запрос прямого итератора, установленного на узел дерева с минимальным
  //ключом
  iterator begin() noexcept;
  //запрос «неустановленного» прямого итератора
  iterator end() noexcept;

  //запрос обратного итератора, установленного на узел дерева с максимальным
  //ключом
  reverse_iterator rbegin() noexcept;
  //запрос «неустановленного» обратного итератора
  reverse_iterator rend() noexcept;

 private:
  struct Node {
    key_type key;
    value_type data;
    Node *left = nullptr;
    Node *right = nullptr;

    Node(key_type k, value_type v) : key(k), data(v) {}
    ~Node() {
      delete left;
      delete right;
    }
  };

  void insert(key_type k, value_type v, Node *&node);
  Node *remove(key_type k, Node *node);
  Node *find(key_type k, Node *node);
  Node *find_min(Node *node);
  Node *find_max(Node *node);

  size_type size_ = 0;
  Node *root_ = nullptr;

  class BstIterator {
   private:
    Node *bst_root_ = nullptr;
    Node *current_ = nullptr;

   public:
    BstIterator(Node *root, Node *n) : bst_root_(root), current_(n){};
    ~BstIterator(){};

    reference operator*() const noexcept { return current_->data; }

    BstIterator &operator++() noexcept {
      current_ = find_next(current_);
      return *this;
    }

    BstIterator operator++(int) noexcept {
      auto it = *this;
      ++(*this);
      return it;
    }

    BstIterator &operator--() noexcept {
      current_ = find_prev(current_);
      return *this;
    }
    BstIterator operator--(int) noexcept {
      auto it = *this;
      (*this);
      return it;
    }

    bool operator==(BstIterator const &other) const noexcept {
      return current_ == other.current_;
    }
    bool operator!=(BstIterator const &other) const noexcept {
      return current_ != other.current_;
    }

   private:
    Node *find_parent(Node *node) { return find_parent(node, bst_root_); }
    Node *find_parent(Node *node, Node *from) {
      if (from == nullptr || node == nullptr) return nullptr;
      if (from->left == node || from->right == node) return from;

      if (node->key > from->key) {
        return find_parent(node, from->right);
      } else if (node->key < from->key) {
        return find_parent(node, from->left);
      }
      return nullptr;
    }

    Node *find_next(Node *node) {
      if (node->right != nullptr) {
        node = node->right;
        while (node->left) node = node->left;
        return node;
      }

      Node *parent = find_parent(node);

      while (parent != nullptr) {
        if (parent->left == node) break;
        node = parent;
        parent = find_parent(node);
      }
      return parent;
    }

    Node *find_prev(Node *node) {
      if (node->left != nullptr) {
        node = node->left;
        while (node->right) node = node->right;
        return node;
      }

      Node *parent = find_parent(node);

      while (parent != nullptr) {
        if (parent->right == node) break;
        node = parent;
        parent = find_parent(node);
      }
      return parent;
    }
  };
  class ReverseBstIterator {
   private:
    Node *bst_root_ = nullptr;
    Node *current_ = nullptr;

   public:
    ReverseBstIterator(Node *root, Node *n) : bst_root_(root), current_(n){};
    ~ReverseBstIterator(){};

    reference operator*() const noexcept { return current_->data; }

    ReverseBstIterator &operator++() noexcept {
      current_ = find_prev(current_);
      return *this;
    }
    ReverseBstIterator operator++(int) noexcept {
      auto it = *this;
      ++(*this);
      return it;
    }

    ReverseBstIterator &operator--() noexcept {
      current_ = find_next(current_);
      return *this;
    }
    ReverseBstIterator operator--(int) noexcept {
      auto it = *this;
      (*this);
      return it;
    }

    bool operator==(ReverseBstIterator const &other) const noexcept {
      return current_ == other.current_;
    }
    bool operator!=(ReverseBstIterator const &other) const noexcept {
      return current_ != other.current_;
    }

   private:
    Node *find_parent(Node *node) { return find_parent(node, bst_root_); }
    Node *find_parent(Node *node, Node *from) {
      if (from == nullptr || node == nullptr) return nullptr;
      if (from->left == node || from->right == node) return from;

      if (node->key > from->key) {
        return find_parent(node, from->right);
      } else if (node->key < from->key) {
        return find_parent(node, from->left);
      }
      return nullptr;
    }

    Node *find_next(Node *node) {
      if (node->right != nullptr) {
        node = node->right;
        while (node->left) node = node->left;
        return node;
      }

      Node *parent = find_parent(node);

      while (parent != nullptr) {
        if (parent->left == node) break;
        node = parent;
        parent = find_parent(node);
      }
      return parent;
    }

    Node *find_prev(Node *node) {
      if (node->left != nullptr) {
        node = node->left;
        while (node->right) node = node->right;
        return node;
      }

      Node *parent = find_parent(node);

      while (parent != nullptr) {
        if (parent->right == node) break;
        node = parent;
        parent = find_parent(node);
      }
      return parent;
    }
  };
};

template <typename T_key, typename T_data>
void Bst<T_key, T_data>::insert(key_type k, value_type v) {
  insert(k, v, root_);
}

template <typename T_key, typename T_data>
void Bst<T_key, T_data>::insert(key_type k, value_type v, Node *&node) {
  if (node == nullptr) {
    node = new Node(k, v);
  } else if (k > node->key) {
    insert(k, v, node->right);
  } else if (k < node->key) {
    insert(k, v, node->left);
  } else {  // if equal
    node->data = v;
  }
  size_++;
}

template <typename T_key, typename T_data>
void Bst<T_key, T_data>::bf_print() const noexcept {
  if (root_ == nullptr) return;

  std::queue<Node *> q;
  q.push(root_);

  while (!q.empty()) {
    Node *node = q.front();
    q.pop();
    if (node->left) q.push(node->left);
    if (node->right) q.push(node->right);
    std::cout << node->data;
  }

  std::cout << std::endl;
}

template <typename T_key, typename T_data>
void Bst<T_key, T_data>::df_print() const noexcept {}

template <typename T_key, typename T_data>
typename Bst<T_key, T_data>::size_type Bst<T_key, T_data>::size() const {
  return size_;
}

template <typename T_key, typename T_data>
void Bst<T_key, T_data>::clear() {
  delete root_;
}

template <typename T_key, typename T_data>
bool Bst<T_key, T_data>::empty() const noexcept {
  return size_ == 0;
}

template <typename T_key, typename T_data>
typename Bst<T_key, T_data>::reference Bst<T_key, T_data>::at(key_type k) {
  return find(k, root_)->data;
}

template <typename T_key, typename T_data>
typename Bst<T_key, T_data>::reference Bst<T_key, T_data>::at(
    key_type k) const {
  return find(k, root_)->data;
}

template <typename T_key, typename T_data>
typename Bst<T_key, T_data>::Node *Bst<T_key, T_data>::find(key_type k,
                                                            Node *node) {
  if (node == nullptr) {
    return nullptr;
  } else if (k > node->key) {
    return find(k, node->right);
  } else if (k < node->key) {
    return find(k, node->left);
  } else {  // equal
    return node;
  }
}

template <typename T_key, typename T_data>
typename Bst<T_key, T_data>::size_type Bst<T_key, T_data>::height()
    const noexcept {
  std::function<size_type(Node *)> lamd_height = [&lamd_height](Node *node) {
    if (node == nullptr) {
      return (size_type)0;
    } else {
      return (size_type)(1 + std::max(lamd_height(node->left),
                                      lamd_height(node->right)));
    }
  };
  return lamd_height(root_);
}

template <typename T_key, typename T_data>
typename Bst<T_key, T_data>::Node *Bst<T_key, T_data>::find_min(Node *node) {
  if (node == nullptr) {
    return nullptr;
  } else if (node->left == nullptr) {
    return node;
  } else {
    return find_min(node->left);
  }
}

template <typename T_key, typename T_data>
typename Bst<T_key, T_data>::Node *Bst<T_key, T_data>::find_max(Node *node) {
  if (node == nullptr) {
    return nullptr;
  } else if (node->right == nullptr) {
    return node;
  } else {
    return find_max(node->right);
  }
}

template <typename T_key, typename T_data>
typename Bst<T_key, T_data>::iterator Bst<T_key, T_data>::begin() noexcept {
  return BstIterator(root_, find_min(root_));
}

template <typename T_key, typename T_data>
typename Bst<T_key, T_data>::iterator Bst<T_key, T_data>::end() noexcept {
  return BstIterator(root_, nullptr);
}

template <typename T_key, typename T_data>
typename Bst<T_key, T_data>::reverse_iterator
Bst<T_key, T_data>::rbegin() noexcept {
  return ReverseBstIterator(root_, find_max(root_));
}

template <typename T_key, typename T_data>
typename Bst<T_key, T_data>::reverse_iterator
Bst<T_key, T_data>::rend() noexcept {
  return ReverseBstIterator(root_, nullptr);
}

template <typename T_key, typename T_data>
void Bst<T_key, T_data>::remove(key_type k) {
  remove(k, root_);
}

template <typename T_key, typename T_data>
typename Bst<T_key, T_data>::Node *Bst<T_key, T_data>::remove(key_type k,
                                                              Node *node) {
  if (node == nullptr) return nullptr;
  if (k < node->key) {
    node->left = remove(k, node->left);
  } else if (k > node->key) {
    node->right = remove(k, node->right);
  } else {
    if (node->left == nullptr) {
      Node *right_node = node->right;
      delete node;
      return right_node;
    }

    if (node->right == nullptr) {
      Node *left_child = node->left;
      delete node;
      return left_child;
    }

    Node *min_node = find_min(node->right);
    node->key = min_node->key;
    node->right = remove(min_node->key, node->right);
  }

  return node;
}

#endif  // BSTREE_H_