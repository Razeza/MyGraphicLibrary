//
// Created by dunka on 09.09.2020.
//

#ifndef DED_SORTS_H
#define DED_SORTS_H


template<typename TypeName, typename Functor = std::less<TypeName>>
void qsort (TypeName* pointer, int first, int last, Functor cmp_func) {

    assert (first >= 0);
    assert (last  >= 0);

    int from_left  = first;
    int from_right = last;

    int cmp = (from_left + from_right) / 2;

    while (from_left <= from_right) {
        while (cmp_func (pointer[from_left], pointer[cmp])) {
            from_left++;
        }

        while (cmp_func (pointer[from_right], pointer[cmp])) {
            from_right--;
        }

        if (from_left <= from_right) {
            std::swap (pointer[from_left], pointer[from_right]);

            from_left++;
            from_right--;
        }
    }

    if (first < from_right) {
        qsort (pointer, first, from_right, cmp_func);
    }

    if (last > from_left) {
        qsort (pointer, from_left, last, cmp_func);
    }
}

template<typename TypeName, typename Functor = std::less<TypeName>>
void bubble_sort (TypeName* pointer, int first, int last, Functor cmp_func)
{
    assert (first >= 0);
    assert (last  >= 0);

    for (int i = 0; i < last; i++) {
        for (int j = 1; j < last - i; j++) {

            if (cmp_func (pointer[j - 1], pointer[j])) {
                std::swap (pointer[j-1], pointer[j]);
            }
        }
    }
}



#endif //DED_SORTS_H
