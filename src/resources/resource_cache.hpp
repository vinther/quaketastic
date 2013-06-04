/*
 * resourcecache.hpp
 *
 *  Created on: May 24, 2013
 *      Author: svp
 */

#ifndef RESOURCE_CACHE_HPP_
#define RESOURCE_CACHE_HPP_

#include <string>
#include <unordered_map>

namespace qts {

template<typename T, typename Key, typename Hash>
class resource_cache {
public:
	resource_cache();
	~resource_cache();

	const typename T::type& get_resource(const std::string& path);
	void clear();

	std::unordered_map<Key, typename T::type> cache;
	T loader;
};

} /* namespace qts */

#endif /* RESOURCE_CACHE_HPP_ */
