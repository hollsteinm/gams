#include "opencl_gpu_mem_buffer.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <CL\opencl.h>
#include <cassert>
#include <mutex>

gams::opencl::base_opencl_gpu_mem_buffer::base_opencl_gpu_mem_buffer(cl_context context, cl_command_queue command_queue, std::size_t size_in_bytes)
	: context_(context)
	, queue_(command_queue)
	, igpu_mem_buffer(size_in_bytes)
{
	cl_int error = CL_SUCCESS;
	mem = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR, size_in_bytes, nullptr, &error);
	if (error != CL_SUCCESS)
	{
		throw std::exception("clCreateBuffer{RW|ALLOC}");
	}
	reset();
}

gams::opencl::base_opencl_gpu_mem_buffer::~base_opencl_gpu_mem_buffer()
{
	clReleaseMemObject(mem);
}

bool gams::opencl::base_opencl_gpu_mem_buffer::prepare()
{
	cl_int error = CL_SUCCESS;
	rw_ptr = static_cast<char*>(clEnqueueMapBuffer(queue_,
		mem,
		CL_BLOCKING,
		CL_MAP_WRITE | CL_MAP_READ,
		0,
		this->size() * sizeof(char),
		0,
		nullptr,
		nullptr,
		&error));
	return error == CL_SUCCESS;
}

void gams::opencl::base_opencl_gpu_mem_buffer::commit()
{
	cl_int error = clEnqueueUnmapMemObject(queue_,
		mem,
		rw_ptr,
		0,
		nullptr,
		nullptr);
	reset();
}

std::size_t gams::opencl::base_opencl_gpu_mem_buffer::write_impl(const char* buffer, std::size_t size)
{
	std::size_t write_iter = get_wr_curr() - get_wr_first();
	for (std::size_t i = 0; i < size; ++i)
	{
		rw_ptr[write_iter++] = buffer[i];
	}
	return size;
}

std::size_t gams::opencl::base_opencl_gpu_mem_buffer::read_impl(char* buffer, std::size_t size)
{
	std::size_t read_iter = get_rd_curr() - get_rd_first();
	for (std::size_t i = 0; i < size; ++i)
	{
		buffer[i] = rw_ptr[read_iter++];
	}
	return size;
}