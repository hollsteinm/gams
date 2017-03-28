#pragma once

#include "../igpu_mem_buffer.h"

typedef struct _cl_command_queue* cl_command_queue;
typedef struct _cl_context* cl_context;
typedef struct _cl_mem* cl_mem;
typedef struct _cl_event* cl_event;

namespace gams 
{
    namespace opencl
    {
        class base_opencl_gpu_mem_buffer : public igpu_mem_buffer
        {
        public:
			base_opencl_gpu_mem_buffer(cl_context context, cl_command_queue command_queue, std::size_t size_in_bytes);
            virtual ~base_opencl_gpu_mem_buffer();

			inline cl_mem get_device_memory() { return mem; }

			virtual bool prepare() override;
			virtual void commit() override;

        protected:
			virtual std::size_t write_impl(const char* buffer, std::size_t size) override;
			virtual std::size_t read_impl(char* buffer, std::size_t size) override;

        private:
            cl_context context_;
            cl_command_queue queue_;
            
            cl_mem mem;
        };
    }
}