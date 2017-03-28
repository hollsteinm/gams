#include "igpu_mem_buffer.h"
#include <algorithm>

gams::igpu_mem_buffer::igpu_mem_buffer(std::size_t size_in_bytes)
    : size_(size_in_bytes)
	, read_(0)
	, wrote_(0)
	, rw_ptr(nullptr)
{
    
}

std::size_t gams::igpu_mem_buffer::read(char* out_buffer, std::size_t size)
{
	std::size_t read = 0;
	const std::size_t num_to_end = get_rd_last() - get_rd_curr();
	size = num_to_end < size ? num_to_end : size;
	read = read_impl(out_buffer, size);
	read_ += read;
	return read;
}

std::size_t gams::igpu_mem_buffer::write(const char* buffer, std::size_t size)
{
	std::size_t wrote = 0;
	const std::size_t num_to_end = get_wr_last() - get_wr_curr();
	size = num_to_end < size ? num_to_end : size;
	wrote = write_impl(buffer, size);
	wrote_ += wrote;
	return wrote;
}