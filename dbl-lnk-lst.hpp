//
// Created by rogerv on 4/28/24.
//
#ifndef DBL_LNK_LST_HPP
#define DBL_LNK_LST_HPP

#include <memory>
#include <concepts>
#include <cassert>

namespace cust_coll {

  template <typename T>
  concept lst_elm_type_constraints = requires {
    requires std::equality_comparable<T>;
    requires std::is_default_constructible_v<T>;
    requires std::copy_constructible<T>;
    requires std::move_constructible<T>;
  };

  /**
   * A classic CS-101 doubly-linked-list container template, but with C++ flair.
   *
   * Supports forward and reverse iteration via begin()/end() and rbegin()/rend().
   *
   * @tparam T type of element contained by container - as constrained by
   *           concept lst_elm_type_constraints (see above)
   */
  template <typename T> requires lst_elm_type_constraints<T>
  class dbl_lnk_lst {
  protected:
    template <typename E> requires lst_elm_type_constraints<E>
    struct lst_node {
      E value{};
      lst_node<E> *prev{nullptr};           // non-owning plain pointer
      std::unique_ptr<lst_node<E>> next{};  // owning smart pointer
      lst_node()  noexcept = delete;
      lst_node(const E &item) noexcept : value{item} {}
      lst_node(E &&item) noexcept : value{std::move(item)} {};
      lst_node& operator=(const E &item) = delete;
      lst_node& operator=(E &&item) = delete;
      ~lst_node() noexcept = default;
    };
  protected:
    std::unique_ptr<lst_node<T>> head{};  // owning smart pointer
    lst_node<T> *tail{nullptr};           // non-owning plain pointer
    size_t count{0};
    void insert_at_head(lst_node<T> *node) noexcept;
    void append_at_tail(lst_node<T> *node) noexcept;
    void insert_at_position(const T&pos, lst_node<T> *node) noexcept;
    void append_at_position(const T&pos, lst_node<T> *node) noexcept;
  public:
    dbl_lnk_lst()  noexcept = default;
    ~dbl_lnk_lst() noexcept { clear(); }
    size_t size() const noexcept { return count; }
    bool is_empty() const noexcept { return count == 0; }
    bool insert(const T& item) noexcept;
    bool insert(T &&item) noexcept;
    bool insert_at(const T& item, const T&pos) noexcept;
    bool insert_at(T &&item, const T&pos) noexcept;
    bool append(const T& item) noexcept;
    bool append(T &&item) noexcept;
    bool append_at(const T& item, const T&pos) noexcept;
    bool append_at(T &&item, const T&pos) noexcept;
    bool delete_at(const T&pos) noexcept;
    void clear() noexcept;

    class iterator {
    public:
      using get_next_node_t = lst_node<T> *(*)(lst_node<T> *) noexcept;
    protected:
      using value_type  = T;
      using pointer     = T*;
      using reference   = T&;
      lst_node<T> *node{nullptr};
      get_next_node_t get_next_node = [](lst_node<T> *) noexcept -> lst_node<T>* { return nullptr; };
    public:
      iterator() noexcept = default;
      iterator(get_next_node_t next_node, lst_node<T> *n) noexcept : node{n}, get_next_node{next_node} {}
      // Prefix increment
      iterator& operator++() { node = get_next_node(node); return *this; }
      // Postfix increment
      iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
      friend bool operator==(const iterator& a, const iterator& b) noexcept { return a.node == b.node; };
      friend bool operator!=(const iterator& a, const iterator& b) noexcept { return a.node != b.node; };
      T& operator*() { return node->value; }
      T* operator->() { return &node->value; }
    };
  protected:
    static lst_node<T> *frwd(lst_node<T> *n) noexcept { return n->next.get(); }
    static lst_node<T> *bkwd(lst_node<T> *n) noexcept { return n->prev; }
  public:
    iterator begin() noexcept { return iterator{frwd, head.get()}; }
    iterator end() noexcept { return iterator{}; }
    iterator rbegin() noexcept { return iterator{bkwd, tail}; }
    iterator rend() noexcept { return iterator{}; }
  };

  template <typename T> requires lst_elm_type_constraints<T>
  inline void dbl_lnk_lst<T>::insert_at_head(lst_node<T> *const node) noexcept {
    if (head) {
      auto *const next_node = head.release(); // transferring ownership (not using assignment via std::move() for reasons)
      next_node->prev = node;
      node->next.reset(next_node); // new node taking ownership of next node
      head.reset(node);  // head taking ownership of the new node
    } else {
      head.reset(node);  // head taking ownership of the new node
      tail = head.get(); // tail set to point at the last node
    }
  }

  template <typename T> requires lst_elm_type_constraints<T>
  inline void dbl_lnk_lst<T>::append_at_tail(lst_node<T> *const node) noexcept {
    if (tail != nullptr) {
      node->prev = tail;
      assert(tail->next.get() == nullptr); // (we trust but verify)
      tail->next.reset(node); // taking ownership of the new node
      tail = node; // newly appended node is now the tail node
    } else {
      insert_at_head(node);
    }
  }

  template <typename T> requires lst_elm_type_constraints<T>
  void dbl_lnk_lst<T>::insert_at_position(const T&pos, lst_node<T> *const node) noexcept {
    for (auto *curr_node = head.get(); curr_node != nullptr; curr_node = curr_node->next.get()) {
      if (curr_node->value == pos) {
        auto *const prev_node = curr_node->prev;
        if (curr_node->prev != nullptr) {
          prev_node->next.release();   // ceases ownership of this pos-matching node without freeing it
          node->next.reset(curr_node); //  takes ownership of this pos-matching node
          node->prev = prev_node;
          prev_node->next.reset(node); // prev_node takes ownership of the newly inserted node
        } else {   // means pos-matching curr_node is the head node
          assert(curr_node == head.get()); // (we trust but verify)
          insert_at_head(node);
        }
        return;
      }
    }
    append_at_tail(node);
  }

  template <typename T> requires lst_elm_type_constraints<T>
  void dbl_lnk_lst<T>::append_at_position(const T&pos, lst_node<T> *const node) noexcept {
    for (auto *curr_node = tail; curr_node != nullptr; curr_node = curr_node->prev) {
      if (curr_node->value == pos) {
        auto *const next_node = curr_node->next.release(); // ceases ownership of next node without freeing it
        if (next_node != nullptr) {
          node->next.reset(next_node); // new node takes ownership of next node
          node->prev = curr_node;
          curr_node->next.reset(node); // curr_node takes ownership of the newly inserted node
          return;
        } else { // means pos-matching curr_node is the tail node
          assert(curr_node == tail);     // (we trust but verify)
        }
      }
    }
    append_at_tail(node);
  }

  /**
   * Inserts copy of item into container where will be at head of list.
   * @tparam T item's type
   * @param item to be copy-inserted
   * @return returns false when fails to allocate memory for new list item
   */
  template <typename T> requires lst_elm_type_constraints<T>
  bool dbl_lnk_lst<T>::insert(const T& item) noexcept {
    auto *const node = new (std::nothrow) lst_node<T>{item};
    if (node != nullptr) {
      insert_at_head(node);
      count++;
      return true;
    }
    return false;
  }

  /**
   * Inserts by moving item into container (thus taking ownership) where will be at head of list.
   * @tparam T item's type
   * @param item to be move-inserted
   * @return returns false when fails to allocate memory for new list item
   */
  template <typename T> requires lst_elm_type_constraints<T>
  bool dbl_lnk_lst<T>::insert(T &&item) noexcept {
    auto *const node = new (std::nothrow) lst_node<T>{std::move(item)};
    if (node != nullptr) {
      insert_at_head(node);
      count++;
      return true;
    }
    return false;
  }

  /**
   * Starting from head of list, iterates list forward, looking to match on the pos specified item;
   * will copy item into the container in front of the found pos item. If the pos item is not found
   * then will fall back to appending the item at the end of the list.
   * @tparam T item's type
   * @param item to be copy-inserted
   * @param pos item to be inserted in front of
   * @return returns false when fails to allocate memory for new list item
   */
  template <typename T> requires lst_elm_type_constraints<T>
  bool dbl_lnk_lst<T>::insert_at(const T& item, const T&pos) noexcept {
    auto *const node = new (std::nothrow) lst_node<T>{item};
    if (node != nullptr) {
      insert_at_position(pos, node);
      count++;
      return true;
    }
    return false;
  }

  /**
   * Starting from head of list, iterates list forward, looking to match on the pos specified item;
   * will move item into the container (thus taking ownership) in front of the found pos item.
   * If the pos item is not found then will fall back to appending the item at the end of the list.
   * @tparam T item's type
   * @param item to be move-inserted
   * @param pos item to be inserted in front of
   * @return returns false when fails to allocate memory for new list item
   */
  template <typename T> requires lst_elm_type_constraints<T>
  bool dbl_lnk_lst<T>::insert_at(T &&item, const T&pos) noexcept {
    auto *const node = new (std::nothrow) lst_node<T>{std::move(item)};
    if (node != nullptr) {
      insert_at_position(pos, node);
      count++;
      return true;
    }
    return false;
  }

  /**
   * Appends copy of item into container where will be at tail of list.
   * @tparam T item's type
   * @param item to be copy-inserted
   * @return returns false when fails to allocate memory for new list item
   */
  template <typename T> requires lst_elm_type_constraints<T>
  bool dbl_lnk_lst<T>::append(const T& item) noexcept {
    auto *const node = new (std::nothrow) lst_node<T>{item};
    if (node != nullptr) {
      append_at_tail(node);
      count++;
      return true;
    }
    return false;
  }

  /**
   * Appends by moving item into container (thus taking ownership) where will be at tail of list.
   * @tparam T item's type
   * @param item to be move-inserted
   * @return returns false when fails to allocate memory for new list item
   */
  template <typename T> requires lst_elm_type_constraints<T>
  bool dbl_lnk_lst<T>::append(T &&item) noexcept {
    auto *const node = new (std::nothrow) lst_node<T>{std::move(item)};
    if (node != nullptr) {
      append_at_tail(node);
      count++;
      return true;
    }
    return false;
  }

  /**
   * Starting from tail of list, iterates list backward, looking to match on the pos specified item;
   * will copy item into the container right after the found pos item. If the pos item is not found
   * then will fall back to appending the item at the end of the list.
   * @tparam T item's type
   * @param item to be copy-inserted
   * @param pos item to be inserted after
   * @return returns false when fails to allocate memory for new list item
   */
  template <typename T> requires lst_elm_type_constraints<T>
  bool dbl_lnk_lst<T>::append_at(const T& item, const T&pos) noexcept {
    auto *const node = new (std::nothrow) lst_node<T>{item};
    if (node != nullptr) {
      append_at_position(pos, node);
      count++;
      return true;
    }
    return false;
  }

  /**
   * Starting from tail of list, iterates list backward, looking to match on the pos specified item;
   * will move item into the container (thus taking ownership) right after the found pos item.
   * If the pos item is not found then will fall back to appending the item at the end of the list.
   * @tparam T item's type
   * @param item to be move-inserted
   * @param pos item to be inserted after
   * @return returns false when fails to allocate memory for new list item
   */
  template <typename T> requires lst_elm_type_constraints<T>
  bool dbl_lnk_lst<T>::append_at(T &&item, const T&pos) noexcept {
    auto *const node = new (std::nothrow) lst_node<T>{std::move(item)};
    if (node != nullptr) {
      append_at_position(pos, node);
      count++;
      return true;
    }
    return false;
  }

  /**
   * Removes a pos specified matched item from list.
   * @tparam T item's type
   * @param pos item to be removed
   * @return returns true if item was matched and removed from list
   */
  template <typename T> requires lst_elm_type_constraints<T>
  bool dbl_lnk_lst<T>::delete_at(const T&pos) noexcept {
    for (auto *curr_node = head.get(); curr_node != nullptr; curr_node = curr_node->next.get()) {
      if (curr_node->value == pos) {
        auto *curr_next_node = curr_node->next.release(); // ceases ownership of next node without freeing it
        if (auto *const prev_node = curr_node->prev; prev_node != nullptr) {
          if (curr_next_node != nullptr)
            curr_next_node->prev = prev_node;
          if (tail == curr_node)
            tail = prev_node;
          prev_node->next.reset(curr_next_node); // transfer ownership (curr_node is also deleted)
        } else {   // means pos-matching curr_node is the head node
          assert(curr_node == head.get()); // (we trust but verify)
          if (curr_next_node != nullptr)
            curr_next_node->prev = nullptr;
          head.reset(curr_next_node); // transfer ownership (curr_node is also deleted)
          if (curr_next_node = head.get(); curr_next_node == nullptr)
            tail = nullptr;
          else if (curr_next_node->next.get() == nullptr)
            tail = curr_next_node;
        }
        count--;
        return true;
      }
    }
    return false;
  }

  /**
   * Removes all items in container, freeing their memory.
   * @tparam T item's type
   */
  template <typename T> requires lst_elm_type_constraints<T>
  void dbl_lnk_lst<T>::clear() noexcept {
    for (auto *node = tail; node != nullptr; node = bkwd(node)) {
      if (node->next) {
        node->next.reset(nullptr);
        count--;
      }
    }
    tail = nullptr;
    if (head) {
      head.reset(nullptr);
      count--;
    }
  }

} // cust_coll

#endif //DBL_LNK_LST_HPP