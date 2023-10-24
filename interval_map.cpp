/******************************************************************************
*                                                                             *
*   Program : Interval map                                                    *
*                                                                             *
*                                                                             *
*                                                                             *
*   Programmer : Nikita Seliverstov (nikselko)                                *
*                                                                             *
*                                                                             *
******************************************************************************/

#include <iostream>
#include <map>

using namespace std;

template<typename K, typename V>
class interval_map {
	friend void IntervalMapTest();
	V m_valBegin;
	std::map<K, V> m_map;
public:
	interval_map(V const& val)
		: m_valBegin(val)
	{}

	void assign(K const& keyBegin, K const& keyEnd, V const& val) { 
		if (!(keyBegin < keyEnd)) return; 

		if (m_map.empty()) {
			if (val == m_valBegin) return;
			m_map.insert({ keyBegin, val });
			m_map.insert({ keyEnd, m_valBegin });
			return;
		}

		K upper_interval = m_map.rbegin()->first;
		K lower_interval = m_map.begin()->first;
		auto map_iter = m_map.upper_bound(keyEnd);

		if (map_iter == m_map.end()) {
			if (upper_interval < keyEnd) {
				m_map.insert(m_map.end(), std::pair<K, V>(keyEnd, m_valBegin));
				map_iter--; map_iter--;
			}
			else {
				map_iter = m_map.lower_bound(keyEnd);
				auto next_val = map_iter->second;
				if (!(next_val == val)) {
					map_iter = m_map.insert_or_assign(map_iter, keyEnd, prev(map_iter)->second);
					map_iter--;
				}
			}

			if (lower_interval > keyBegin) {
				if (!(val == m_valBegin)) m_map.insert_or_assign(m_map.begin(), keyBegin, val);
			}
			else {
				map_iter--;
				auto prev_val = map_iter->second;
				if (!(val == prev_val)) m_map.insert_or_assign(map_iter, keyBegin, val);
			}

			while (keyBegin < map_iter->first) {
				if (map_iter == m_map.begin()) {
					m_map.erase(map_iter);
					break;
				}
				m_map.erase(map_iter--);
			}
		}
	}

	// look-up of the value associated with key
	V const& operator[](K const& key) const {
		auto it = m_map.upper_bound(key);
		if (it == m_map.begin()) {
			return m_valBegin;
		}
		else {
			return (--it)->second;
		}
	}

	void print() {

		std::cout << '\n' << m_valBegin << '\n';
		for (auto it = m_map.begin(); it != m_map.end(); ++it) {
			std::cout << it->first << ", " << it->second << '\n';
		}
	}
};

int main()
{
	//first test case
	interval_map<int, char> fooh{ 'z' };
	fooh.assign(2, 5, 'a');
	fooh.print();
	std::cout << fooh[6] << std::endl << std::endl;

	//second test case
	// expected : z  b  z
	fooh = interval_map<int, char>{ 'z' };
	fooh.assign(1, 4, 'b');
	cout << fooh[0] << " " << fooh[1] << " " << fooh[5] << endl;

	//third test case
	// expected: A
	fooh = interval_map<int, char>{ 'z' };
	fooh.assign(1, 6, 'A');
	fooh.assign(2, 4, 'B');
	cout << fooh[5] << endl;
	fooh.print();


	//forth test case
	fooh = interval_map<int, char>{ 'z' };
	//expected [0,'a'],[1,'z']
	fooh.assign(0, 1, 'a');
	fooh.print();


	//fifth test case
	// expected [0,'f']
	fooh = interval_map<int, char>{ 'z' };
	fooh.assign(1, 2, 'c');
	fooh.assign(2, 3, 'd');
	fooh.assign(3, 4, 'e');
	fooh.assign(4, 15, 'g');
	fooh.assign(0, 10, 'f');
	fooh.print();
	cout << endl;


	//sixth test case
	// expected: 0,'d'  2,'c'  
	fooh = interval_map<int, char>{ 'z' };
	fooh.assign(1, 4, 'c');
	fooh.assign(0, 2, 'd');
	fooh.print();
	cout << endl;
	
}
