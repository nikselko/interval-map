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
#include <cassert>
#include <map>

using std::cout;
using std::endl;

template<typename K, typename V>

class interval_map { 
	friend void IntervalMapTest();
	friend void print();
	V m_valBegin;
	std::map<K, V> m_map;
public:
	
	interval_map(V const& val) : m_valBegin(val) {}

	void assign(K const& keyBegin, K const& keyEnd, V const& val)
	{
		if (not (keyBegin < keyEnd)) return;

		V defaultVal = m_valBegin;
		typename std::map<K, V>::iterator linkMapEnd = m_map.upper_bound(keyEnd);
		auto pair_Begin = m_map.emplace(keyBegin, val);
		auto prev_pair_Begin = pair_Begin.first;

		if (!pair_Begin.second)
			prev_pair_Begin->second = val;

		auto pair_End = m_map.emplace_hint(linkMapEnd, keyEnd, defaultVal);

		assert((std::next(pair_End)) == linkMapEnd);
		bool is_Begin = false;

		if (m_map.begin() != prev_pair_Begin)
			is_Begin = std::next(prev_pair_Begin, -1)->second == val;
		else
			is_Begin = (val == m_valBegin);

		if (val != defaultVal)
			bool is_End = false;

		if (m_map.end() != prev_pair_Begin)
			std::advance(prev_pair_Begin, 1);
		else if (m_map.end() != pair_End)
			std::advance(pair_End, 1);

		m_map.erase(prev_pair_Begin, pair_End);
	}

	V const& operator[](K const& key) const
	{
		auto it = m_map.upper_bound(key);

		if (it == m_map.begin())
			return m_valBegin;
		else
			return (--it)->second;
	}

	void print() 
	{
		cout << endl;

		for (auto it = m_map.begin(); it != m_map.end(); ++it)
			cout << it->first << ", " << it->second << '\n';
	}
};

int main()
{
	interval_map <int, char> test_map{ 'A' };

	test_map.print();
	test_map.assign(3, 6, 'B');
	test_map.print();
}
