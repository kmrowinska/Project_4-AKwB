#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <set>
#include <ctime>
#include <sstream>
#include <iterator>
using namespace std;

clock_t start, finish;
int max_ind = 0;
int max_el = 0;
vector<int> multi_set;
vector<int> used_elements;
vector<int> map;

void search(int ind, bool &exists);

void odczyt() {
    fstream file;
    string line;
    file.open("C:\\Users\\kinga\\Desktop\\instancja_test.txt", ios::in);

    if (file.is_open()) {
        cout << "Otwarto pomyslnie plik!\n" << endl;
        multi_set = vector<int>{istream_iterator<int>(file), istream_iterator<int>()};
    } else {
        cout << "Wystapil blad przy zaladowaniu pliku" << endl;
    }

    cout<<"Multizbior: ";
    for(int i=0; i<multi_set.size(); i++){
        cout<<multi_set[i]<<" ";
    }
    cout<<"\n";
    file.close();
}

void checkNewton(int multi_set_size, int k, bool &qualified_for_mapping) {
    double factorial1 = 1;
    for (int i = 1; i <= (k + 2); i++)
        factorial1 = factorial1 * i;

    double factorial2 = 1;
    for (int i = 1; i <= k; i++)
        factorial2 = factorial2 * i;

    int newton_symbol;
    newton_symbol = factorial1 / (2 * factorial2);

    if (newton_symbol == multi_set_size) {
        qualified_for_mapping = true;
    }
}

void find_elements(int &first_el, int &sec_max_el, int &max_el){
    sort(multi_set.begin(), multi_set.end());
    max_el = *max_element(multi_set.begin(), multi_set.end());
    vector<int> copy_multiset = multi_set;
    copy_multiset.pop_back();
    sec_max_el = *max_element(copy_multiset.begin(), copy_multiset.end());
    first_el = max_el - sec_max_el;

}

int main() {
    int first_el, sec_max_el;
    bool qualifiedForMap = false;
    odczyt();
    find_elements(first_el, sec_max_el, max_el);

    int multi_set_size = multi_set.size();
    int k = 0;
    while (qualifiedForMap == false && k < multi_set_size) {
        k++;
        checkNewton(multi_set_size, k, qualifiedForMap);
    }


    if(find(multi_set.begin(), multi_set.end(), first_el) != multi_set.end()){
        map.push_back(first_el);
        used_elements.push_back(first_el);
        if(qualifiedForMap){
            max_ind = k + 1;
            bool mapExists = false;
            start = clock();
            search(1, mapExists);
            finish = clock();
            double time = (double) (finish - start) / (double) (CLOCKS_PER_SEC);
            if (!mapExists){
                cout << "\nDla podanego multizbioru nie istnieje mapa." << endl;
            }
            else{
                cout << "\nRozwiazanie znaleziono w czasie rownym " << time << "s" << endl;
            }
        }
        else{
            cout << "Nie istnieje liczba ciec przekladajaca sie na licznosc multizbioru" << endl;
        }
    }
    else{
        cout<<"Brak liczby w multizbiorze. "<<endl;
        exit(0);
    }

    return 0;
}

void search(int ind, bool &exists) {
    bool found = false;
    if (ind == max_ind) {
        set<int> s1;
        s1.insert(multi_set.begin(), multi_set.end());
        set<int> s2;
        s2.insert(used_elements.begin(), used_elements.end());
        if (s1 == s2) {
            cout << "\nZnaleziono rozwiazanie: ";
            for (int i = 0; i < map.size(); i++)
                cout << map[i] << " ";
            cout << endl;
            exists = true;
            found = true;
        }
    }

    if (!found) {
        int already_checked;
        for (int i = 0; i < multi_set.size(); i++) {
            int multi_set_elem_count = count(multi_set.begin(), multi_set.end(), multi_set[i]);
            int used_elem_count = count(used_elements.begin(), used_elements.end(), multi_set[i]);

            if (multi_set_elem_count <= used_elem_count || multi_set[i] == already_checked)
                continue;

            int current_used_size = used_elements.size();
            map.push_back(multi_set[i]);
            used_elements.push_back(multi_set[i]);
            int sum = multi_set[i];
            bool sum_exists = false;
            bool go_to_next_level = true;

            for (int j = map.size() - 2; j >= 0; j--) {
                sum += map[j];
                int multi_set_sum_count = count(multi_set.begin(), multi_set.end(), sum);
                int used_sum_count = count(used_elements.begin(), used_elements.end(), sum);

                if(multi_set_sum_count > 0){
                    sum_exists = true;
                }

                if (multi_set_sum_count > used_sum_count)
                    used_elements.push_back(sum);

                if (!sum_exists || multi_set_sum_count <= used_sum_count) {
                    go_to_next_level = false;
                    for (int j = used_elements.size() - 1; j >= current_used_size; j--)
                        used_elements.pop_back();
                    break;
                }
            }

            if (go_to_next_level) {
                int end_distance = max_el - sum;
                int multi_set_end_distance_count = count(multi_set.begin(), multi_set.end(), end_distance);
                int used_end_distance_count = count(used_elements.begin(), used_elements.end(), end_distance);

                if (multi_set_end_distance_count > used_end_distance_count && ind < max_ind)
                    search(ind + 1, exists);

                else if (ind == max_ind - 1 && multi_set_end_distance_count == 0 && sum == max_el)
                    search(ind + 1, exists);
            }

            already_checked = map.back();
            map.pop_back();
            for (int j = used_elements.size() - 1; j >= current_used_size; j--)
                used_elements.pop_back();

            if (exists)
                break;
        }
    }
}