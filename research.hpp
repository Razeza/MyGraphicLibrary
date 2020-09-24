
#ifndef DED_RESEARCH_H
#define DED_RESEARCH_H

#include <string>
#include <utility>
#include <vector>
#include <cassert>
#include "sorts.h"

template<typename TypeName, typename Functor>
using sort_function = decltype (qsort<TypeName, Functor>);

class Research {
public:

    static std::size_t _number_of_swaps_;
    static std::size_t _number_of_cmp_;

    template<typename TypeName>
    struct functor {
        bool operator () (const TypeName& x1, const TypeName& x2) {
            _number_of_cmp_++;
            return x1 < x2;
        }
    };

public:

    struct Experiment {

        std::size_t array_size;
        std::size_t number_of_swaps;
        std::size_t number_of_cmp;

        Experiment () = default;
        Experiment (size_t arr_size, size_t number_of_swaps, size_t number_of_cmp);

        static Experiment make_experiment (std::size_t arr_size, sort_function<int, functor<int>> sort);
    };


private:

    std::vector<Experiment> research;
    std::string             sort_name;

public:

    std::size_t number_of_experiments = 0;
    std::size_t max_cmp               = 0;
    std::size_t max_swap              = 0;
    std::size_t max_size              = 0;


    explicit                              Research      (std::string sort_name, std::size_t number_of_experiments);
    inline const std::vector<Experiment>& get_results   () const;
    inline const std::string&             get_sort_name () const;
    inline void                           add           (const Experiment& experiment);

    static Research make_research (std::size_t number_of_experiments, sort_function<int, Research::functor<int>> sort, const std::string& sort_name);

};

std::size_t Research::_number_of_swaps_ = 0;
std::size_t Research::_number_of_cmp_   = 0;


namespace std {
    /*! Извращение на максимуме */
    /*! Вообще изначально было my_std со своим swapом */
    template<>
    void swap<int> (int& x1, int& x2) noexcept(__and_<is_nothrow_move_constructible<int>, is_nothrow_move_assignable<int> >::value) {

        Research::_number_of_swaps_++;

        int tmp = x1;
        x1      = x2;
        x2      = tmp;
    }
}

using Experiment = Research::Experiment;









#endif //DED_RESEARCH_H
