//
// Created by dunka on 09.09.2020.
//

#ifndef DED_RESEARCH_H
#define DED_RESEARCH_H



#include <string>
#include <utility>
#include <vector>
#include <cassert>
#include "sorts.h"

class Research {
public:

    static std::size_t _number_of_swaps_;
    static std::size_t _number_of_cmp_;

    struct Experiment {

        std::size_t array_size;
        std::size_t number_of_swaps;
        std::size_t number_of_cmp;

        Experiment () = default;

        Experiment (size_t arr_size, size_t number_of_swaps, size_t number_of_cmp):
                array_size      (arr_size),
                number_of_swaps (number_of_swaps),
                number_of_cmp   (number_of_cmp)
        { }
    };


private:

    std::vector<Experiment> research;
    std::string             sort_name;

public:

    std::size_t number_of_experiments = 0;
    std::size_t max_cmp = 0;
    std::size_t max_swap = 0;
    std::size_t max_size = 0;


    explicit Research (std::string sort_name, std::size_t number_of_experiments):
        sort_name             (std::move(sort_name)),
        number_of_experiments (number_of_experiments)
    { }


    inline const std::vector<Experiment>& get_results () const {
        return research;
    }

    inline const std::string& get_sort_name () const {
        return sort_name;
    }

    inline void add (const Experiment& experiment) {
        research.emplace_back (experiment.array_size,
                               experiment.number_of_swaps,
                               experiment.number_of_cmp);
        max_cmp =  std::max (experiment.number_of_cmp, max_cmp);
        max_swap = std::max (max_cmp,                  max_swap);
    }

    template<typename TypeName>
    struct functor {
        bool operator () (const TypeName& x1, const TypeName& x2) {
            _number_of_cmp_++;
            return x1 < x2;
        }
    };
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

template<typename TypeName, typename Functor>
using sort_function = decltype (qsort<TypeName, Functor>);



Experiment make_experiment (std::size_t arr_size, sort_function<int, Research::functor<int>> sort) {

    int arr[arr_size];
    for (auto& i : arr) {
        i = rand () % 10001 + 17;
    }

    Research::_number_of_cmp_ = Research::_number_of_swaps_ = 0;
    sort (arr, 0, arr_size, Research::functor<int> ());

    return Experiment (arr_size, Research::_number_of_swaps_, Research::_number_of_cmp_);
}

Research make_research (std::size_t number_of_experiments, sort_function<int, Research::functor<int>> sort, const std::string& sort_name) {

    Research   research (sort_name, number_of_experiments);
    Experiment cur_experiment;

    DEBUG (std::cout << "\n***********************\n";)

    for (std::size_t i = 10; i < 10 * (number_of_experiments + 1); i += 10) {
        cur_experiment = make_experiment (i, sort);
        research.add (cur_experiment);

        DEBUG (std::cout << "size - " << cur_experiment.array_size << " cmp - " << cur_experiment.number_of_cmp << " swap - " << cur_experiment.number_of_swaps << "\n";)
    }
    research.max_size = number_of_experiments * 10;

    DEBUG (std::cout << "\n***********************\n";)

    return research;
}

#endif //DED_RESEARCH_H
