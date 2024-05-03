# Classic CS-101 Doubly-Linked-List with some C++ flair

With modern CPU and computer architecture, C++ developers rely on `std::vector<>` for run-of-the-mill container chores - linked list not so much.

Due to memory caching architecture, memory locality can often favor array organized memory over discreet node allocation as employed by list and trees. Even inserting in the middle - a strength of list and a weakness for array can be reasonable for the latter, well, depending on over all element size of the container. As in all things it can be situation dependent.

None-the-less this project is a template class implementation of the classic doubly-linked-list, `cust_coll::dbl_lnk_lst<T>`.

The element type `T` is constrained by the concept `lst_elm_type_constraints` (refer to the source code).

List traversal is via C++ iterator - `begin()/end()` for forward traversal and `rbegin()/rend()` for reversed iteration. Here is example of reversed iteration:

```cpp
    std::for_each(lst.rbegin(), lst.rend(), [](some_elm &elm){
      std::cout << elm << '\n'; // assuming some_elm supports stream i/o
    });
```

All APIs are unit tested using Google GTest - see `dbl-lnk-lst_test.cpp`

The project is built using CMake - GTest as a dependency is managed in CMake.