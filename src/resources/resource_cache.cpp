/*
 * resourcecache.cpp
 *
 *  Created on: May 24, 2013
 *      Author: svp
 */

#include "resource_cache.hpp"

#include <iostream>
#include <boost/log/trivial.hpp>

namespace qts {

template<typename T>
resource_cache<T>::resource_cache() {
}

template<typename T>
resource_cache<T>::~resource_cache() {
}

template<typename T>
const typename T::type& resource_cache<T>::get_resource(const std::string& path) {
	const auto& it = cache.find(path);

	if (cache.end() != it) {
		return it->second;
	} else {
		BOOST_LOG_TRIVIAL(info) << "File \"" << path << "\" was not found in resource cache";

		cache[path] = loader.load(path);
		return cache[path];
	}
}

template<typename T>
void resource_cache<T>::clear()
{
	cache.clear();
}


} /* namespace qts */


#include "mesh_loader.hpp"
#include "bsp_map_loader.hpp"

template class qts::resource_cache<qts::mesh_loader>;
template class qts::resource_cache<qts::bsp_map_loader>;
