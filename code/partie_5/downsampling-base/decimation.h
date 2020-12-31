// header-start
////////////////////////////////////////////////////////////////////////////////
// \file      decimation.cpp
//
// \brief     low level function for image decimation
//
// \legal     Copyright (c) 2020
//
// \author    Bernard 
//
////////////////////////////////////////////////////////////////////////////////
// header-end
#include <iostream>

extern uint nbThread ;

class Decimation{
	private:
	uint nb_thread;
	public:
	Decimation(const uint nb_thread_): nb_thread(nb_thread_) {}
	uint getThread() const{return this->nb_thread;}
	virtual void horizontal_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                           uint dst_width, uint dst_height,
                           uint dst_stride, uint src_stride);
	virtual void vertical_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                    uint dst_width, uint dst_height,
                    uint dst_stride, uint src_stride);
	virtual void do_horizontal_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                           uint dst_width, uint dst_height,
                           uint dst_stride, uint src_stride,uint i_,uint max_thread_);
	virtual void do_vertical_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                    uint dst_width, uint dst_height,
                    uint dst_stride, uint src_stride,uint i_,uint max_thread_);	
	virtual void print_method();
	
};

class SimpleDecimation:public Decimation{
	public:
	explicit SimpleDecimation(const uint nb_thread) : Decimation(nb_thread) {}
	void horizontal_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                           uint dst_width, uint dst_height,
                           uint dst_stride, uint src_stride) override;
	void vertical_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                    uint dst_width, uint dst_height,
                    uint dst_stride, uint src_stride) override;
	void do_horizontal_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                           uint dst_width, uint dst_height,
                           uint dst_stride, uint src_stride,uint i_,uint max_thread_) override;
	void do_vertical_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                    uint dst_width, uint dst_height,
                    uint dst_stride, uint src_stride,uint i_,uint max_thread_) override;				
	void print_method() override;
};

class ConvolutionDecimation:public Decimation{
	public:
	explicit ConvolutionDecimation(const uint nb_thread) : Decimation(nb_thread) {}
	void horizontal_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                           uint dst_width, uint dst_height,
                           uint dst_stride, uint src_stride) override;
	void vertical_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                    uint dst_width, uint dst_height,
                    uint dst_stride, uint src_stride) override;
	
	void do_horizontal_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                           uint dst_width, uint dst_height,
                           uint dst_stride, uint src_stride,uint i_,uint max_thread_) override;
	void do_vertical_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                    uint dst_width, uint dst_height,
                    uint dst_stride, uint src_stride,uint i_,uint max_thread_) override;	
	
	void print_method() override;				
};

class VectorizedDecimation:public Decimation{
	public:
	explicit VectorizedDecimation(const uint nb_thread) : Decimation(nb_thread) {}
	void horizontal_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                           uint dst_width, uint dst_height,
                           uint dst_stride, uint src_stride) override;
	void vertical_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                    uint dst_width, uint dst_height,
                    uint dst_stride, uint src_stride) override;
	
	void do_horizontal_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                           uint dst_width, uint dst_height,
                           uint dst_stride, uint src_stride,uint i_,uint max_thread_) override;
	void do_vertical_decimation(uint16_t *dst_ptr, uint16_t *src_ptr,
                    uint dst_width, uint dst_height,
                    uint dst_stride, uint src_stride,uint i_,uint max_thread_) override;	
	
	void print_method() override;				
};



	
	