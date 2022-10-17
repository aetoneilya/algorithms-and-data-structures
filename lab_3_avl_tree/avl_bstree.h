#ifndef BSTREE_H_
#define BSTREE_H_

#include <algorithm>
#include <iostream>
#include <memory>
#include <queue>

template <typename T_key, typename T_data>
class AvlBst {
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
  AvlBst(){};
  AvlBst(AvlBst const &b);
  ~AvlBst() { delete root_; };

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
    size_type height = 0;
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
  size_type height(Node *node);

  Node *left_rotate(Node *&t);
  Node *right_rotate(Node *&t);
  Node *double_left_rotate(Node *&t);
  Node *double_right_rotate(Node *&t);

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
void AvlBst<T_key, T_data>::insert(key_type k, value_type v) {
  insert(k, v, root_);
  size_++;
}

template <typename T_key, typename T_data>
void AvlBst<T_key, T_data>::insert(key_type k, value_type v, Node *&node) {
  if (node == nullptr) {
    node = new Node(k, v);
  } else if (k < node->key) {
    insert(k, v, node->left);
    if (height(node->left) - height(node->right) == 2) {
      if (k < node->left->key)
        node = right_rotate(node);
      else
        node = double_right_rotate(node);
    }

  } else if (k > node->key) {
    insert(k, v, node->right);
    if (height(node->right) - height(node->left) == 2) {
      if (k > node->right->data)
        node = left_rotate(node);
      else
        node = double_left_rotate(node);
    }
  } else {  // if equal
    node->data = v;
  }
  node->height = std::max(height(node->left), height(node->right)) + 1;
}

template <typename T_key, typename T_data>
void AvlBst<T_key, T_data>::bf_print() const noexcept {
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
void AvlBst<T_key, T_data>::df_print() const noexcept {}

template <typename T_key, typename T_data>
typename AvlBst<T_key, T_data>::size_type AvlBst<T_key, T_data>::size() const {
  return size_;
}

template <typename T_key, typename T_data>
void AvlBst<T_key, T_data>::clear() {
  delete root_;
}

template <typename T_key, typename T_data>
bool AvlBst<T_key, T_data>::empty() const noexcept {
  return size_ == 0;
}

template <typename T_key, typename T_data>
typename AvlBst<T_key, T_data>::reference AvlBst<T_key, T_data>::at(
    key_type k) {
  return find(k, root_)->data;
}

template <typename T_key, typename T_data>
typename AvlBst<T_key, T_data>::reference AvlBst<T_key, T_data>::at(
    key_type k) const {
  return find(k, root_)->data;
}

template <typename T_key, typename T_data>
typename AvlBst<T_key, T_data>::Node *AvlBst<T_key, T_data>::find(key_type k,
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
typename AvlBst<T_key, T_data>::size_type AvlBst<T_key, T_data>::height()
    const noexcept {
  return height(root_);
}

template <typename T_key, typename T_data>
typename AvlBst<T_key, T_data>::size_type AvlBst<T_key, T_data>::height(
    Node *node) {
  return (node == nullptr ? -1 : node->height);
}

template <typename T_key, typename T_data>
typename AvlBst<T_key, T_data>::Node *AvlBst<T_key, T_data>::find_min(
    Node *node) {
  if (node == nullptr) {
    return nullptr;
  } else if (node->left == nullptr) {
    return node;
  } else {
    return find_min(node->left);
  }
}

template <typename T_key, typename T_data>
typename AvlBst<T_key, T_data>::Node *AvlBst<T_key, T_data>::find_max(
    Node *node) {
  if (node == nullptr) {
    return nullptr;
  } else if (node->right == nullptr) {
    return node;
  } else {
    return find_max(node->right);
  }
}

template <typename T_key, typename T_data>
typename AvlBst<T_key, T_data>::iterator
AvlBst<T_key, T_data>::begin() noexcept {
  return BstIterator(root_, find_min(root_));
}

template <typename T_key, typename T_data>
typename AvlBst<T_key, T_data>::iterator AvlBst<T_key, T_data>::end() noexcept {
  return BstIterator(root_, nullptr);
}

template <typename T_key, typename T_data>
typename AvlBst<T_key, T_data>::reverse_iterator
AvlBst<T_key, T_data>::rbegin() noexcept {
  return ReverseBstIterator(root_, find_max(root_));
}

template <typename T_key, typename T_data>
typename AvlBst<T_key, T_data>::reverse_iterator
AvlBst<T_key, T_data>::rend() noexcept {
  return ReverseBstIterator(root_, nullptr);
}

template <typename T_key, typename T_data>
void AvlBst<T_key, T_data>::remove(key_type k) {
  remove(k, root_);
}

template <typename T_key, typename T_data>
typename AvlBst<T_key, T_data>::Node *AvlBst<T_key, T_data>::remove(
    key_type k, Node *node) {
  if (node == nullptr) return nullptr;

  if (k < node->key) {
    node->left = remove(k, node->left);
  } else if (k > node->key) {
    node->right = remove(k, node->right);
  } else {
    if (node->left == nullptr) {
      Node *right_node = node->right;
      node->right = node->left = nullptr;
      delete node;
      return right_node;
    }

    if (node->right == nullptr) {
      Node *left_child = node->left;
      node->right = node->left = nullptr;
      delete node;
      return left_child;
    }

    Node *min_node = find_min(node->right);
    node->key = min_node->key;
    node->data = min_node->data;
    node->right = remove(min_node->key, node->right);
  }

  if (node == nullptr) return node;

  node->height = 1 + std::max(height(node->left), height(node->right));
  int bal = (int)(height(node->left) - height(node->right));
  if (bal > 1) {
    if (height(node->left) >= height(node->right)) {
      return right_rotate(node);
    } else {
      node->left = left_rotate(node->left);
      return right_rotate(node);
    }
  } else if (bal < -1) {
    if (height(node->right) >= height(node->left)) {
      return left_rotate(node);
    } else {
      node->right = right_rotate(node->right);
      return left_rotate(node);
    }
  }
  return node;
}

template <typename T_key, typename T_data>
typename AvlBst<T_key, T_data>::Node *AvlBst<T_key, T_data>::left_rotate(
    Node *&t) {
  if (t->right == nullptr) return t;
  Node *u = t->right;
  t->right = u->left;
  u->left = t;
  t->height = std::max(height(t->left), height(t->right)) + 1;
  u->height = std::max(height(t->right), t->height) + 1;
  return u;
}

template <typename T_key, typename T_data>
typename AvlBst<T_key, T_data>::Node *AvlBst<T_key, T_data>::right_rotate(
    Node *&t) {
  if (t->left == nullptr) return t;
  Node *u = t->left;
  t->left = u->right;
  u->right = t;
  t->height = std::max(height(t->left), height(t->right)) + 1;
  u->height = std::max(height(u->left), t->height) + 1;
  return u;
}

template <typename T_key, typename T_data>
typename AvlBst<T_key, T_data>::Node *AvlBst<T_key, T_data>::double_left_rotate(
    Node *&t) {
  t->right = right_rotate(t->right);
  return left_rotate(t);
}

template <typename T_key, typename T_data>
typename AvlBst<T_key, T_data>::Node *
AvlBst<T_key, T_data>::double_right_rotate(Node *&t) {
  t->left = left_rotate(t->left);
  return right_rotate(t);
}

#endif  // BSTREE_H_