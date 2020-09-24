#include "research.hpp"


Research::Experiment::Experiment (size_t arr_size, size_t number_of_swaps, size_t number_of_cmp):
        array_size      (arr_size),
        number_of_swaps (number_of_swaps),
        number_of_cmp   (number_of_cmp)
{ }

Experiment Research::Experiment::make_experiment (std::size_t arr_size, sort_function<int, Research::functor<int>> sort) {

    int arr[arr_size];
    for (auto& i : arr) {
        i = rand () % 10001 + 17;
    }

    Research::_number_of_cmp_ = Research::_number_of_swaps_ = 0;
    sort (arr, 0, arr_size, Research::functor<int> ());

    return Experiment (arr_size, Research::_number_of_swaps_, Research::_number_of_cmp_);
}



Research::Research (std::string sort_name, std::size_t number_of_experiments):
    sort_name             (std::move(sort_name)),
    number_of_experiments (number_of_experiments)
{ }

inline const std::vector<Experiment>& Research::get_results () const {
    return research;
}

inline const std::string& Research::get_sort_name () const {
    return sort_name;
}

inline void Research::add (const Experiment& experiment) {
    research.emplace_back (experiment.array_size,
                           experiment.number_of_swaps,
                           experiment.number_of_cmp);

    max_cmp =  std::max (experiment.number_of_cmp, max_cmp);
    max_swap = std::max (max_cmp,                  max_swap);
}

Research Research::make_research (std::size_t number_of_experiments, sort_function<int, Research::functor<int>> sort, const std::string& sort_name) {

    Research   research (sort_name, number_of_experiments);
    Experiment cur_experiment;

    DEBUG (std::cout << "\n***********************\n";)

    for (std::size_t i = 10; i < 10 * (number_of_experiments + 1); i += 10) {
        cur_experiment = Experiment::make_experiment (i, sort);
        research.add (cur_experiment);

        DEBUG (std::cout << "size - " << cur_experiment.array_size << " cmp - " << cur_experiment.number_of_cmp << " swap - " << cur_experiment.number_of_swaps << "\n";)
    }
    research.max_size = number_of_experiments * 10;

    DEBUG (std::cout << "\n***********************\n";)

    return research;
}
