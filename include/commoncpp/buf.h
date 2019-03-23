
#ifndef __GTL_BUFFER_H_
#define __GTL_BUFFER_H_

#include <stdint.h>
#include <algorithm>
#include <vector>
#include <assert.h>
#include <string.h>
#include <string>

namespace gtl
{
	    /// A buffer class modeled after org.jboss.netty.buffer.ChannelBuffer
		///
		/// @code
		/// +-------------------+------------------+------------------+
		/// | prependable bytes |  readable bytes  |  writable bytes  |
		/// |                   |     (CONTENT)    |                  |
		/// +-------------------+------------------+------------------+
		/// |                   |                  |                  |
		/// 0      <=      readerIndex   <=   writerIndex    <=     size
		/// @endcode
		class Buffer /*: public muduo::copyable*/
		{
		public:
			/*
			设计方案
				kCheapPrepend 预留空间，避免在vector前面插入元素
			*/
			static const uint32_t kCheapPrepend = 0;    
			static const uint32_t kInitialSize = 1024;

			//构造函数
			explicit Buffer(uint32_t initialSize = Buffer::kInitialSize) 
				: buffer_(Buffer::kCheapPrepend + initialSize)
				, readerIndex_(Buffer::kCheapPrepend)
				, writerIndex_(Buffer::kCheapPrepend) 
			{
			}

			//交换缓存
			void swap(Buffer& rhs)
			{
				this->buffer_.swap(rhs.buffer_);
				std::swap(this->readerIndex_, rhs.readerIndex_);
				std::swap(this->writerIndex_, rhs.writerIndex_);
			}

			//可读字节数
			uint32_t readableBytes() const
			{
				return this->writerIndex_ - this->readerIndex_;
			}

			//可写字节数
			uint32_t writableBytes() const
			{
				return this->buffer_.size() - this->writerIndex_;
			}

			//预留字节数
			size_t prependableBytes() const
			{
				return this->readerIndex_;
			}

			//偷看
			const uint8_t * peek() const
			{
				return this->begin() + this->readerIndex_;
			}

			//取出len长度的数据(需要使用peek方法取出数据,该方法只是移动读指针)
			void retrieve(size_t len)
			{
				assert(len <= this->readableBytes());

				/*
				设计亮点
					这里为什么要将小于和等于拆开，而不是直接"this->readerIndex_ += len;"
					因为this->retrieveAll();会同时移动写指针和读指针，优化缓存区空间
				*/
				if (len < this->readableBytes())
				{
					this->readerIndex_ += len;
				}
				else
				{
					this->retrieveAll();
				}
			}

			//取出截止到end长度的数据
			void retrieveUntil(const uint8_t * end)
			{
				assert(peek() <= end);
				assert(end <= this->beginWrite());

				this->retrieve(end - peek());
			}

			//取出所有数据
			void retrieveAll()
			{
				this->readerIndex_ = Buffer::kCheapPrepend;
				this->writerIndex_ = Buffer::kCheapPrepend;
			}

			//追加数据
			void append(const uint8_t* /*restrict*/ data, uint32_t len)
			{
				//确保写缓冲区有空间
				this->ensureWritableBytes(len);
				//将data数据拷贝到写缓冲区的末尾
				std::copy(data, data + len, this->beginWrite());
				//更新写指针位置
				this->hasWritten(len);
			}

			//追加数据
			void append(const void* /*restrict*/ data, uint32_t len)
			{
				this->append(static_cast<const uint8_t*>(data), len);
			}

			//确保写缓冲区有空间
			void ensureWritableBytes(uint32_t len)
			{
				if (this->writableBytes() < len)
				{
					//可写区域小于len，扩充可写区域
					this->makeSpace(len);
				}

				assert(this->writableBytes() >= len);
			}

			//返回写缓冲区的位置
			uint8_t * beginWrite()
			{
				return this->begin() + this->writerIndex_;
			}

			//返回写缓冲区的位置
			const uint8_t * beginWrite() const
			{
				return begin() + this->writerIndex_;
			}

			//更新写指针的位置
			void hasWritten(uint32_t len)
			{
				assert(len <= writableBytes());

				this->writerIndex_ += len;
			}

			//擦除写指针
			void unwrite(uint32_t len)
			{
				assert(len <= readableBytes());

				this->writerIndex_ -= len;
			}

			//调整缓冲区大小到reserve大小--优化缓冲区空间，释放多余的内存空间
			void shrink(uint32_t reserve)
			{
				Buffer other;
				other.ensureWritableBytes(this->readableBytes() + reserve);
				other.append(this->peek(), this->readableBytes());
				this->swap(other);
			}

			//获取当前缓冲区容量
			uint32_t internalCapacity() const
			{
				return this->buffer_.capacity();
			}

		private:
			//缓冲区开始位置
			uint8_t* begin()
			{
				return &*buffer_.begin();
			}

			//缓冲区开始位置
			const uint8_t* begin() const
			{
				return &*buffer_.begin();
			}

			//扩充空间
			void makeSpace(uint32_t len)
			{
				if (this->writableBytes() + this->prependableBytes() < len + Buffer::kCheapPrepend)
				{
					//剩余空间小于需要的长度Len和缓冲区预留空间，直接扩充缓冲区
					//resize会完成对原来缓冲区的拷贝
					this->buffer_.resize(this->writerIndex_ + len);
				}
				else
				{
					//说明缓冲区前面空余一部分空间，需要将可读数据前移
					assert(this->kCheapPrepend < this->readerIndex_);

					uint32_t readable = this->readableBytes();

					//将可读数据域前移
					std::copy(begin() + this->readerIndex_, begin() + this->writerIndex_, this->begin() + Buffer::kCheapPrepend);

					//更新读指针位置
					this->readerIndex_ = Buffer::kCheapPrepend;
					//写指针的位置需要注意
					this->writerIndex_ = this->readerIndex_ + readable;

					assert(readable == readableBytes());
				}
			}

		private:
			std::vector<uint8_t> buffer_;     //缓冲区
			uint32_t readerIndex_;            //读指针位置
			uint32_t writerIndex_;            //写指针位置
		};
}

#endif  // LHN_NET_BUFFER_H
