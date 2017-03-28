#pragma once

#include <iosfwd>

namespace gams {
    class igpu_mem_buffer 
    {
    public:
        inline virtual ~igpu_mem_buffer() {}

		std::size_t read(char* out_buffer, std::size_t size);
		std::size_t write(const char* buffer, std::size_t size);

        std::size_t size() const { return size_; }
		inline void reset() { reset_read(); reset_write(); }
		inline void zero() 
		{
			if (rw_ptr != nullptr)
			{
				memset(rw_ptr, 0, size_);
				reset();
			}
		}

		std::size_t remaining_free() const { return get_wr_last() - get_wr_curr(); }
		std::size_t remaining_read() const { return get_rd_last() - get_rd_curr(); }

		igpu_mem_buffer(const igpu_mem_buffer&) = delete;
		igpu_mem_buffer& operator=(const igpu_mem_buffer&) = delete;

		virtual bool prepare() = 0; //tell the device that it must be ready for a read/write from host
		virtual void commit() = 0; //tell the device that it may sync/flush/unsync from host
		
		inline char* get_rd_first() const { return rw_ptr; }
		inline char* get_rd_last() const { return rw_ptr + size_; }
		inline char* get_wr_first() const { return rw_ptr; }
		inline char* get_wr_last() const { return rw_ptr + size_; }
		inline char* get_wr_curr() const { return rw_ptr + wrote_; }
		inline char* get_rd_curr() const { return rw_ptr + read_; }

    protected:
		igpu_mem_buffer(std::size_t size_in_bytes);

		virtual std::size_t read_impl(char* out_buffer, std::size_t size) = 0;
		virtual std::size_t write_impl(const char* buffer, std::size_t size) = 0;

		inline void reset_read() { read_ = 0; }
		inline void reset_write() { wrote_ = 0; }

		char* rw_ptr;

		inline std::size_t get_r_offset() const { return read_; }
		inline std::size_t get_w_offset() const { return wrote_; }

    private:
        std::size_t size_;
		std::size_t wrote_;
		std::size_t read_;
    };
}