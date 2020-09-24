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

template<typename TypeName, typename Functor = std::less<TypeName>>
void heapify(TypeName* pointer, int last, Functor cmp_func, int i) 
{ 
    int largest = i; // Initialize largest as root 
    int l = 2*i + 1; // left = 2*i + 1 
    int r = 2*i + 2; // right = 2*i + 2 
  
    // If left child is larger than root 
    if (l < last && cmp_func (pointer[l], pointer[largest])) 
        largest = l; 
  
    // If right child is larger than largest so far 
    if (r < last && cmp_func (pointer[r], pointer[largest])) 
        largest = r; 
  
    // If largest is not root 
    if (largest != i) 
    { 
        std::swap (pointer[i], pointer[largest]); 
  
        // Recursively heapify the affected sub-tree 
        heapify (pointer, last, cmp_func, largest); 
    } 
} 
  
template<typename TypeName, typename Functor = std::less<TypeName>>
void heapSort (TypeName* pointer, int first, int last, Functor cmp_func) 
{ 
    // Build heap (rearrange array) 
    for (int i = last / 2 - 1; i >= 0; i--) 
        heapify<TypeName, Functor> (pointer, last, cmp_func, i); 
  
    // One by one extract an element from heap 
    for (int i = last - 1; i > 0; i--) 
    { 
        // Move current root to end 
        std::swap (pointer[0], pointer[i]); 
  
        // call max heapify on the reduced heap 
        heapify<TypeName, Functor> (pointer, i, cmp_func, 0); 
    } 
}



#endif //DED_SORTS_H
