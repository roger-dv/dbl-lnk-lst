//
// Created by rogerv on 4/28/24.
//
#include <vector>
#include <algorithm>
#include "some_elm.hpp"
#include "dbl-lnk-lst.hpp"
using cust_coll::dbl_lnk_lst;

int main() {
  srand( time(nullptr) );

  std::vector<some_elm> strs{};
  strs.resize(10/*24*/);

  printf("%lu items in vector\n", strs.size());

  {
    dbl_lnk_lst<some_elm> lst{};

    for (const auto &elm: strs) {
      lst.insert(elm);
      some_elm::count += 1;
    }

    printf("%d items inserted into list; list container size: %lu\n", some_elm::count, lst.size());

    printf("delete items \"%s\", \"%s\", \"%s\":\n", strs[2].s.c_str(), strs[5].s.c_str(), strs[8].s.c_str());

    lst.delete_at(strs[2]);
    lst.delete_at(strs[5]);
    lst.delete_at(strs[8]);

    printf("%lu items remaining in list container\n", lst.size());

    printf("delete items \"%s\", \"%s\":\n", strs[9].s.c_str(), strs[0].s.c_str());

    lst.delete_at(strs[9]);
    lst.delete_at(strs[0]);

    printf("%lu items remaining in list container\n", lst.size());
  }

  printf("%d items in list (after list container destruction)\n", some_elm::count);

  puts("\niterate contents of vector strs:");
  int count = 0;
  std::for_each(strs.begin(), strs.end(), [&count](some_elm &elm){
    putchar('\t');
    puts(elm.s.c_str());
    count++;
  });
  printf("vector strs iterated %d times\n", count);

  {
    dbl_lnk_lst<some_elm> lst{};

    for (auto &elm: strs) {
      lst.append(std::move(elm));
      some_elm::count += 1;
    }

    printf("\n%d items moved into second list; list container size: %lu\n", some_elm::count, lst.size());

    puts("\nforward iterate contents of second list:");
    count = 0;
    std::for_each(lst.begin(), lst.end(), [&count](some_elm &elm){
      putchar('\t');
      puts(elm.s.c_str());
      count++;
    });
    printf("forward iterated %d times\n", count);

    puts("\nbackwards iterate contents of second list:");
    count = 0;
    std::for_each(lst.rbegin(), lst.rend(), [&count](some_elm &elm){
      putchar('\t');
      puts(elm.s.c_str());
      count++;
    });
    printf("backwards iterated %d times\n\n", count);
  }

  printf("\n%d items in second list (after list container destruction)\n", some_elm::count);

  puts("\nvector strs now going out of scope and destructing:");
  return 0;
}