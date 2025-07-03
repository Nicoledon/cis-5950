#define CATCH_CONFIG_MAIN
#include "./catch.hpp"
#include <cstring>  // Does not do our cstring.h this includes C's <string.h>
#include <cstdlib>  // include's C's <stdlib.h>
#include <iostream> // for C++ printing

#include "simple_string.hpp"

using namespace std;

TEST_CASE("Basic split", "[split]") {
  
  simple_string str1("apple,banana,grape");
  simple_string delim1(",");
  auto result1 = str1.split(delim1);
  REQUIRE(result1.size() == 3);
  REQUIRE(result1[0] == simple_string("apple"));
  REQUIRE(result1[1] == simple_string("banana"));
  REQUIRE(result1[2] == simple_string("grape"));

  simple_string str2(",apple,banana,grape");
  simple_string delim2(",");
  auto result2 = str2.split(delim2);
  REQUIRE(result2.size() == 3);
  REQUIRE(result2[0] == simple_string("apple"));
  REQUIRE(result2[1] == simple_string("banana"));
  REQUIRE(result2[2] == simple_string("grape"));

  simple_string str3("apple,banana,grape,");
  simple_string delim3(",");
  auto result3 = str3.split(delim3);
  REQUIRE(result3.size() == 3);
  REQUIRE(result3[0] == simple_string("apple"));
  REQUIRE(result3[1] == simple_string("banana"));
  REQUIRE(result3[2] == simple_string("grape"));

  simple_string str4("apple");
  simple_string delim4(",");
  auto result4 = str4.split(delim4);
  REQUIRE(result4.size() == 1);
  REQUIRE(result4[0] == simple_string("apple"));

  simple_string str5("");
  simple_string delim5(",");
  auto result5 = str5.split(delim5);
  REQUIRE(result5.empty());

  simple_string str6("apple,,banana,,,grape");
  simple_string delim6(",");
  auto result6 = str6.split(delim6);
  REQUIRE(result6.size() == 3);
  REQUIRE(result6[0] == simple_string("apple"));
  REQUIRE(result6[1] == simple_string("banana"));
  REQUIRE(result6[2] == simple_string("grape"));

  simple_string str7(",");
  simple_string delim7(",");
  auto result7 = str7.split(delim7);
  REQUIRE(result7.empty());

  simple_string str8(",,,");
  simple_string delim8(",");
  auto result8 = str8.split(delim8);
  REQUIRE(result8.empty());

  simple_string str9(",apple,banana,grape,");
  simple_string delim9(",");
  auto result9 = str9.split(delim9);
  REQUIRE(result9.size() == 3);
  REQUIRE(result9[0] == simple_string("apple"));
  REQUIRE(result9[1] == simple_string("banana"));
  REQUIRE(result9[2] == simple_string("grape"));

  simple_string str10("singleword");
  simple_string delim10(",");
  auto result10 = str10.split(delim10);
  REQUIRE(result10.size() == 1);
  REQUIRE(result10[0] == simple_string("singleword"));

  simple_string str11("apple , banana , grape");
  simple_string delim11(",");
  auto result11 = str11.split(delim11);
  REQUIRE(result11.size() == 3);
  REQUIRE(result11[0] == simple_string("apple "));
  REQUIRE(result11[1] == simple_string(" banana "));
  REQUIRE(result11[2] == simple_string(" grape"));

  simple_string str12("   apple,banana,grape   ");
  simple_string delim12(",");
  auto result12 = str12.split(delim12);
  REQUIRE(result12.size() == 3);
  REQUIRE(result12[0] == simple_string("   apple"));
  REQUIRE(result12[1] == simple_string("banana"));
  REQUIRE(result12[2] == simple_string("grape   "));

  simple_string str13("apple,  banana,   grape");
  simple_string delim13(",");
  auto result13 = str13.split(delim13);
  REQUIRE(result13.size() == 3);
  REQUIRE(result13[0] == simple_string("apple"));
  REQUIRE(result13[1] == simple_string("  banana"));
  REQUIRE(result13[2] == simple_string("   grape"));

  std::string long_input(10000, 'A');
  simple_string str14(long_input.c_str());
  simple_string delim14(",");
  auto result14 = str14.split(delim14);
  REQUIRE(result14.size() == 1);
  REQUIRE(result14[0] == simple_string(long_input.c_str()));


  std::string long_input_mod(10000, 'A');
  for (size_t i = 999; i < 10000; i += 1000) {
      long_input_mod[i] = ',';
  }
  simple_string str15(long_input_mod.c_str());
  simple_string delim15(",");
  auto result15 = str15.split(delim15);
  REQUIRE(result15.size() == 10);
  for (size_t i = 0; i < 10; ++i) {
      REQUIRE(result15[i].len() == 999);
      REQUIRE(result15[i] == simple_string(std::string(999, 'A').c_str()));
  }
}

TEST_CASE("Complex split", "[split]") {

  simple_string str_empty_delim("abcdef");
  simple_string empty_delim("");
  auto result_empty_delim = str_empty_delim.split(empty_delim);
  REQUIRE(result_empty_delim.size() == 1);
  REQUIRE(result_empty_delim[0] == simple_string("abcdef"));

  simple_string str_repeated_delim("apple::banana:grape::orange");
  simple_string repeated_delim("::");
  auto result_repeated_delim = str_repeated_delim.split(repeated_delim);
  REQUIRE(result_repeated_delim.size() == 4);
  REQUIRE(result_repeated_delim[0] == simple_string("apple"));
  REQUIRE(result_repeated_delim[1] == simple_string("banana"));
  REQUIRE(result_repeated_delim[2] == simple_string("grape"));
  REQUIRE(result_repeated_delim[3] == simple_string("orange"));

  simple_string str_overlap("abcabc");
  simple_string delim_overlap("abc");
  auto result_overlap = str_overlap.split(delim_overlap);
  REQUIRE(result_overlap.size() == 0);

  simple_string str_all_delim(",,,");
  simple_string delim_all(",,,");
  auto result_all_delim = str_all_delim.split(delim_all);
  REQUIRE(result_all_delim.empty());

  simple_string str_single_char("a");
  simple_string delim_single_char("b");
  auto result_single_char = str_single_char.split(delim_single_char);
  REQUIRE(result_single_char.size() == 1);
  REQUIRE(result_single_char[0] == simple_string("a"));

  simple_string str_multi_basic("apple,banana|grape orange");
  simple_string multi_delims(",| ");
  auto result_multi_basic = str_multi_basic.split(multi_delims);
  REQUIRE(result_multi_basic.size() == 4);
  REQUIRE(result_multi_basic[0] == simple_string("apple"));
  REQUIRE(result_multi_basic[1] == simple_string("banana"));
  REQUIRE(result_multi_basic[2] == simple_string("grape"));
  REQUIRE(result_multi_basic[3] == simple_string("orange"));

  simple_string str_multi_extra("  apple, banana | grape , orange | peach  ");
  auto result_multi_extra = str_multi_extra.split(multi_delims);
  REQUIRE(result_multi_extra.size() == 5);
  REQUIRE(result_multi_extra[0] == simple_string("apple"));
  REQUIRE(result_multi_extra[1] == simple_string("banana"));
  REQUIRE(result_multi_extra[2] == simple_string("grape"));
  REQUIRE(result_multi_extra[3] == simple_string("orange"));
  REQUIRE(result_multi_extra[4] == simple_string("peach"));

  simple_string str_multi_front_end("| A , B | C ,| D |");
  auto result_multi_front_end = str_multi_front_end.split(multi_delims);
  REQUIRE(result_multi_front_end.size() == 4);
  REQUIRE(result_multi_front_end[0] == simple_string("A"));
  REQUIRE(result_multi_front_end[1] == simple_string("B"));
  REQUIRE(result_multi_front_end[2] == simple_string("C"));
  REQUIRE(result_multi_front_end[3] == simple_string("D"));

  simple_string str_multi_spaces("apple  ,  banana |  grape , orange  |  peach");
  // Note that here we explicitly pass ",|" as the delimiter string
  auto result_multi_spaces = str_multi_spaces.split(",|");
  REQUIRE(result_multi_spaces.size() == 5);
  REQUIRE(result_multi_spaces[0] == simple_string("apple  "));
  REQUIRE(result_multi_spaces[1] == simple_string("  banana "));
  REQUIRE(result_multi_spaces[2] == simple_string("  grape "));
  REQUIRE(result_multi_spaces[3] == simple_string(" orange  "));
  REQUIRE(result_multi_spaces[4] == simple_string("  peach"));

  simple_string str_every_char("|,| |,|| , ");
  auto result_every_char = str_every_char.split(multi_delims);
  REQUIRE(result_every_char.empty());

  std::string long_input_multi(5000, 'A');
  long_input_multi[1000] = ',';
  long_input_multi[2000] = '|';
  long_input_multi[3000] = ' ';
  long_input_multi[4000] = ',';
  simple_string str_multi_intervals(long_input_multi.c_str());
  auto result_multi_intervals = str_multi_intervals.split(multi_delims);
  REQUIRE(result_multi_intervals.size() == 5);
  REQUIRE(result_multi_intervals[0].len() == 1000);
  REQUIRE(result_multi_intervals[1].len() == 999);
  REQUIRE(result_multi_intervals[2].len() == 999);
  REQUIRE(result_multi_intervals[3].len() == 999);
  REQUIRE(result_multi_intervals[4].len() == 999);

  simple_string str_extreme1(",,,|||   ,,,apple,,,||  banana  ,,,,|grape,, ||| orange   ");
  auto result_extreme1 = str_extreme1.split(multi_delims);
  REQUIRE(result_extreme1.size() == 4);
  REQUIRE(result_extreme1[0] == simple_string("apple"));
  REQUIRE(result_extreme1[1] == simple_string("banana"));
  REQUIRE(result_extreme1[2] == simple_string("grape"));
  REQUIRE(result_extreme1[3] == simple_string("orange"));

  simple_string str_extreme2(",a|b,c d,e f|g,h | i | j , k |");
  auto result_extreme2 = str_extreme2.split(multi_delims);
  REQUIRE(result_extreme2.size() == 11);
  REQUIRE(result_extreme2[0] == simple_string("a"));
  REQUIRE(result_extreme2[1] == simple_string("b"));
  REQUIRE(result_extreme2[2] == simple_string("c"));
  REQUIRE(result_extreme2[3] == simple_string("d"));
  REQUIRE(result_extreme2[4] == simple_string("e"));
  REQUIRE(result_extreme2[5] == simple_string("f"));
  REQUIRE(result_extreme2[6] == simple_string("g"));
  REQUIRE(result_extreme2[7] == simple_string("h"));
  REQUIRE(result_extreme2[8] == simple_string("i"));
  REQUIRE(result_extreme2[9] == simple_string("j"));
  REQUIRE(result_extreme2[10] == simple_string("k"));

  std::string crazy_long(10000, 'A');
  for (size_t i = 2; i < 10000; i += 3) {
    crazy_long[i] = (i % 2 == 0) ? ',' : '|';
  }
  simple_string str_extreme3(crazy_long.c_str());
  auto result_extreme3 = str_extreme3.split(multi_delims);
  REQUIRE(result_extreme3[0].len() == 2);
  REQUIRE(result_extreme3[1].len() == 2);
  REQUIRE(result_extreme3[100].len() == 2);
  REQUIRE(result_extreme3.back().len() == 1);

  simple_string str_extreme4(",,,,,,,||||||       ||||||,,,,,,||||||");
  auto result_extreme4 = str_extreme4.split(multi_delims);
  REQUIRE(result_extreme4.empty());

  simple_string str_extreme5(",| |,|");
  auto result_extreme5 = str_extreme5.split(multi_delims);
  REQUIRE(result_extreme5.empty());

  simple_string str_spaces_only("apple     banana            grape            orange            ");
  simple_string space_delim(" ");
  auto result_spaces_only = str_spaces_only.split(space_delim);
  REQUIRE(result_spaces_only.size() == 4);
  REQUIRE(result_spaces_only[0] == simple_string("apple"));
  REQUIRE(result_spaces_only[1] == simple_string("banana"));
  REQUIRE(result_spaces_only[2] == simple_string("grape"));
  REQUIRE(result_spaces_only[3] == simple_string("orange"));

  simple_string str_tabs_newlines("\t  apple\t\tbanana\n\n\n  grape\t\t orange \n\npear\t\n\tmelon\n");
  simple_string tab_newline_delim("\t\n");
  auto result_tabs_newlines = str_tabs_newlines.split(tab_newline_delim);
  REQUIRE(result_tabs_newlines.size() == 6);
  REQUIRE(result_tabs_newlines[0] == simple_string("  apple"));
  REQUIRE(result_tabs_newlines[1] == simple_string("banana"));
  REQUIRE(result_tabs_newlines[2] == simple_string("  grape"));
  REQUIRE(result_tabs_newlines[3] == simple_string(" orange "));
  REQUIRE(result_tabs_newlines[4] == simple_string("pear"));
  REQUIRE(result_tabs_newlines[5] == simple_string("melon"));

  simple_string str_alt_letters("A,B|C D,E|F G,H|I");
  auto result_alt_letters = str_alt_letters.split(multi_delims);
  REQUIRE(result_alt_letters.size() == 9);
  REQUIRE(result_alt_letters[0] == simple_string("A"));
  REQUIRE(result_alt_letters[1] == simple_string("B"));
  REQUIRE(result_alt_letters[2] == simple_string("C"));
  REQUIRE(result_alt_letters[3] == simple_string("D"));
  REQUIRE(result_alt_letters[4] == simple_string("E"));
  REQUIRE(result_alt_letters[5] == simple_string("F"));
  REQUIRE(result_alt_letters[6] == simple_string("G"));
  REQUIRE(result_alt_letters[7] == simple_string("H"));
  REQUIRE(result_alt_letters[8] == simple_string("I"));

  simple_string str_long_multi("apple,,,|banana|||grape,,,||orange,|peach");
  auto result_long_multi = str_long_multi.split(multi_delims);
  REQUIRE(result_long_multi.size() == 5);
  REQUIRE(result_long_multi[0] == simple_string("apple"));
  REQUIRE(result_long_multi[1] == simple_string("banana"));
  REQUIRE(result_long_multi[2] == simple_string("grape"));
  REQUIRE(result_long_multi[3] == simple_string("orange"));
  REQUIRE(result_long_multi[4] == simple_string("peach"));

  simple_string str_crazy("apple  ,| banana\t\t,| grape  ,\n orange | peach");
  auto result_crazy = str_crazy.split(multi_delims);
  REQUIRE(result_crazy.size() == 6);
  REQUIRE(result_crazy[0] == simple_string("apple"));
  REQUIRE(result_crazy[1] == simple_string("banana\t\t"));
  REQUIRE(result_crazy[2] == simple_string("grape"));
  REQUIRE(result_crazy[3] == simple_string("\n"));
  REQUIRE(result_crazy[4] == simple_string("orange"));
  REQUIRE(result_crazy[5] == simple_string("peach"));

  std::string large_random(5000, 'B');
  for (size_t i = 500; i < 5000; i += 500) {
    large_random[i] = ',';
  }
  simple_string str_rand(large_random.c_str());
  auto result_rand = str_rand.split(multi_delims);
  REQUIRE(result_rand.size() == 10);
  REQUIRE(result_rand[0].len() == 500);
  REQUIRE(result_rand[1].len() == 499);
  REQUIRE(result_rand.back().len() == 499);

  std::string large_pattern;
  char current_char = 'A';
  for (size_t i = 0; i < 10000; ++i) {
    if (i % 250 == 0 && i != 0) {
      char delimiter = (i % 4 == 0) ? ',' : (i % 4 == 1) ? '|' 
                       : (i % 4 == 2) ? ' ' : '\n';
      large_pattern += delimiter;
      current_char = 'A' + ((current_char - 'A' + 1) % 26);
    } else {
      large_pattern += current_char;
    }
  }
  simple_string large_str(large_pattern.c_str());
  auto result_large_str = large_str.split(multi_delims);
  REQUIRE(result_large_str.size() == 40);
  for (size_t i = 0; i < result_large_str.size(); ++i) {
    if (i == 0) {
      REQUIRE(result_large_str[i].len() == 250);
    } else {
      REQUIRE(result_large_str[i].len() == 249);
    }
    REQUIRE(result_large_str[i].data()[0] == char('A' + (i % 26)));
  }
}