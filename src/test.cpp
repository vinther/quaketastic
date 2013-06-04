
#define BOOST_ALL_DYN_LINK

#include <boost/log/trivial.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

int main2(int, char**) {
	boost::log::add_file_log("quaketastic.log");

	BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";
	BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";
	BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";
	BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";
}
