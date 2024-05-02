//
// Created by rogerv on 4/29/24.
//
#include <vector>
#include <algorithm>
#include <gtest/gtest.h>
#include "some_elm.hpp"
#include "dbl-lnk-lst.hpp"
using cust_coll::dbl_lnk_lst;

static constexpr auto ITEM_NBR = 10;

TEST(DblLnkListAssertions, EmptyConstructedState) {
  some_elm::prnt = false; // setting to true will print test strings to stdout console upon container destruction
  dbl_lnk_lst<some_elm> lst{};
  EXPECT_EQ(lst.size(), 0);
  EXPECT_EQ(lst.size(), 0);
  EXPECT_TRUE(lst.is_empty());
}

TEST(DblLnkListAssertions, InsertCopyAtHead) {
  std::vector<some_elm> strs{ITEM_NBR};
  EXPECT_EQ(strs.size(), ITEM_NBR);
  some_elm::count = 0;
  dbl_lnk_lst<some_elm> lst{};
  for (const auto &elm: strs) {
    auto r = lst.insert(elm);
    EXPECT_TRUE(r);
    some_elm::count += 1;
  }
  EXPECT_EQ(lst.size(), ITEM_NBR);
  EXPECT_EQ(some_elm::count, ITEM_NBR);
  EXPECT_EQ(lst.size(), strs.size());
  auto strs_it = strs.begin();
  auto lst_it = lst.rbegin();
  while (strs_it != strs.end() and lst_it != lst.rend()) {
    auto ve = strs_it++;
    auto le = lst_it++;
    auto r = ve->s.compare(le->s) == 0;
    EXPECT_TRUE(r);
  }
}

TEST(DblLnkListAssertions, MoveInsertAtHead) {
  std::vector<some_elm> strs{ITEM_NBR};
  EXPECT_EQ(strs.size(), ITEM_NBR);
  std::vector<some_elm> sav_strs{strs};
  EXPECT_EQ(sav_strs.size(), ITEM_NBR);
  some_elm::count = 0;
  dbl_lnk_lst<some_elm> lst{};
  for (auto &elm: strs) {
    auto r = lst.insert(std::move(elm));
    EXPECT_TRUE(r);
    some_elm::count += 1;
  }
  EXPECT_EQ(some_elm::count, ITEM_NBR);
  strs.clear();
  EXPECT_EQ(strs.size(), 0);
  EXPECT_EQ(lst.size(), ITEM_NBR);
  auto strs_it = sav_strs.begin();
  auto lst_it = lst.rbegin();
  while (strs_it != sav_strs.end() and lst_it != lst.rend()) {
    auto ve = strs_it++;
    auto le = lst_it++;
    auto r = ve->s.compare(le->s) == 0;
    EXPECT_TRUE(r);
  }
}

TEST(DblLnkListAssertions, InsertCopyAtPos) {
  std::vector<some_elm> strs{ITEM_NBR};
  EXPECT_EQ(strs.size(), ITEM_NBR);
  some_elm::count = 0;
  dbl_lnk_lst<some_elm> lst{};
  for (auto &elm: strs) {
    auto r = lst.insert(elm);
    EXPECT_TRUE(r);
    some_elm::count += 1;
  }
  EXPECT_EQ(some_elm::count, ITEM_NBR);
  auto &trg_pos = strs[2];
  some_elm item{};
  auto r = lst.insert_at(item, trg_pos);
  EXPECT_TRUE(r);
  bool o = r = false;
  std::for_each(lst.begin(), lst.end(), [&r, &o, &item, &trg_pos](some_elm &elm){
    if (not r) {
      if (elm.s.compare(item.s) == 0)
        r = true;
    } else if (elm.s.compare(trg_pos.s) == 0) {
        o = true;
    }
  });
  EXPECT_TRUE(r);
  EXPECT_TRUE(o);
  EXPECT_EQ(lst.size(), ITEM_NBR + 1);
}

TEST(DblLnkListAssertions, MoveInsertAtPos) {
  std::vector<some_elm> strs{ITEM_NBR};
  EXPECT_EQ(strs.size(), ITEM_NBR);
  some_elm::count = 0;
  dbl_lnk_lst<some_elm> lst{};
  for (auto &elm: strs) {
    auto r = lst.insert(elm);
    EXPECT_TRUE(r);
    some_elm::count += 1;
  }
  EXPECT_EQ(some_elm::count, ITEM_NBR);
  auto &trg_pos = strs[2];
  some_elm item{};
  some_elm sav_item{item};
  auto r = lst.insert_at(std::move(item), trg_pos);
  EXPECT_TRUE(r);
  bool o = r = false;
  std::for_each(lst.begin(), lst.end(), [&r, &o, &sav_item, &trg_pos](some_elm &elm){
    if (not r) {
      if (elm.s.compare(sav_item.s) == 0)
        r = true;
    } else if (elm.s.compare(trg_pos.s) == 0) {
        o = true;
    }
  });
  EXPECT_TRUE(r);
  EXPECT_TRUE(o);
  EXPECT_EQ(lst.size(), ITEM_NBR + 1);
}

TEST(DblLnkListAssertions, AppendCopyAtTail) {
  std::vector<some_elm> strs{ITEM_NBR};
  EXPECT_EQ(strs.size(), ITEM_NBR);
  some_elm::count = 0;
  dbl_lnk_lst<some_elm> lst{};
  for (const auto &elm: strs) {
    auto r = lst.append(elm);
    EXPECT_TRUE(r);
    some_elm::count += 1;
  }
  EXPECT_EQ(lst.size(), ITEM_NBR);
  EXPECT_EQ(some_elm::count, ITEM_NBR);
  EXPECT_EQ(lst.size(), strs.size());
  auto strs_it = strs.begin();
  auto lst_it = lst.begin();
  while (strs_it != strs.end() and lst_it != lst.end()) {
    auto ve = strs_it++;
    auto le = lst_it++;
    auto r = ve->s.compare(le->s) == 0;
    EXPECT_TRUE(r);
  }
}

TEST(DblLnkListAssertions, MoveAppendAtTail) {
  std::vector<some_elm> strs{ITEM_NBR};
  EXPECT_EQ(strs.size(), ITEM_NBR);
  std::vector<some_elm> sav_strs{strs};
  EXPECT_EQ(sav_strs.size(), ITEM_NBR);
  some_elm::count = 0;
  dbl_lnk_lst<some_elm> lst{};
  for (auto &elm: strs) {
    auto r = lst.append(std::move(elm));
    EXPECT_TRUE(r);
    some_elm::count += 1;
  }
  EXPECT_EQ(some_elm::count, ITEM_NBR);
  strs.clear();
  EXPECT_EQ(strs.size(), 0);
  EXPECT_EQ(lst.size(), ITEM_NBR);
  auto strs_it = sav_strs.begin();
  auto lst_it = lst.begin();
  while (strs_it != sav_strs.end() and lst_it != lst.end()) {
    auto ve = strs_it++;
    auto le = lst_it++;
    auto r = ve->s.compare(le->s) == 0;
    EXPECT_TRUE(r);
  }
}

TEST(DblLnkListAssertions, AppendCopyAtPos) {
  std::vector<some_elm> strs{ITEM_NBR};
  EXPECT_EQ(strs.size(), ITEM_NBR);
  some_elm::count = 0;
  dbl_lnk_lst<some_elm> lst{};
  for (auto &elm: strs) {
    auto r = lst.insert(elm);
    EXPECT_TRUE(r);
    some_elm::count += 1;
  }
  EXPECT_EQ(some_elm::count, ITEM_NBR);
  auto &trg_pos = strs[2];
  some_elm item{};
  auto r = lst.append_at(item, trg_pos);
  EXPECT_TRUE(r);
  bool o = r = false;
  std::for_each(lst.begin(), lst.end(), [&r, &o, &item, &trg_pos](some_elm &elm){
    if (not o && elm.s.compare(trg_pos.s) == 0) {
      o = true;
    } else if (o and not r) {
      if (elm.s.compare(item.s) == 0)
        r = true;
    }
  });
  EXPECT_TRUE(r);
  EXPECT_TRUE(o);
  EXPECT_EQ(lst.size(), ITEM_NBR + 1);
}

TEST(DblLnkListAssertions, MoveAppendAtPos) {
  std::vector<some_elm> strs{ITEM_NBR};
  EXPECT_EQ(strs.size(), ITEM_NBR);
  some_elm::count = 0;
  dbl_lnk_lst<some_elm> lst{};
  for (auto &elm: strs) {
    auto r = lst.insert(elm);
    EXPECT_TRUE(r);
    some_elm::count += 1;
  }
  EXPECT_EQ(some_elm::count, ITEM_NBR);
  auto &trg_pos = strs[2];
  some_elm item{};
  some_elm sav_item{item};
  auto r = lst.append_at(std::move(item), trg_pos);
  EXPECT_TRUE(r);
  bool o = r = false;
  std::for_each(lst.begin(), lst.end(), [&r, &o, &sav_item, &trg_pos](some_elm &elm){
    if (not o && elm.s.compare(trg_pos.s) == 0) {
      o = true;
    } else if (o and not r) {
      if (elm.s.compare(sav_item.s) == 0)
        r = true;
    }
  });
  EXPECT_TRUE(r);
  EXPECT_TRUE(o);
  EXPECT_EQ(lst.size(), ITEM_NBR + 1);
}

TEST(DblLnkListAssertions, DeleteAtPos) {
  std::vector<some_elm> strs{ITEM_NBR};
  EXPECT_EQ(strs.size(), ITEM_NBR);
  some_elm::count = 0;
  dbl_lnk_lst<some_elm> lst{};
  for (const auto &elm: strs) {
    auto r = lst.insert(elm);
    EXPECT_TRUE(r);
    some_elm::count += 1;
  }
  EXPECT_EQ(lst.size(), ITEM_NBR);
  EXPECT_EQ(some_elm::count, ITEM_NBR);
  EXPECT_EQ(lst.size(), strs.size());
  auto r = lst.delete_at(strs[4]);
  EXPECT_TRUE(r);
  r = lst.delete_at(strs[4]);
  EXPECT_FALSE(r);
  r = lst.delete_at(strs[7]);
  EXPECT_TRUE(r);
  r = lst.delete_at(strs[7]);
  EXPECT_FALSE(r);
  r = lst.delete_at(strs[9]);
  EXPECT_TRUE(r);
  r = lst.delete_at(strs[9]);
  EXPECT_FALSE(r);
  EXPECT_EQ(lst.size(), ITEM_NBR - 3);
}