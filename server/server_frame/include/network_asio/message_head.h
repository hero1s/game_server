
#pragma once

namespace NetworkAsio {
// 4 + 1 + 1 + 4 = 10
	struct message_head {
		message_head() : length_(0), compress_(0), crypto_(0), crc32_(0) {}

		uint32_t length_;
		char compress_;
		char crypto_;
		int32_t crc32_;
	};

	struct MessageBuffer final {
		MessageBuffer() {}

		MessageBuffer(const char *data, uint32_t length)
				: length_(length), data_(new char[length_]) {
			memcpy(data_, data, length_);
		}

		~MessageBuffer() {
			delete data_;
			data_ = nullptr;
		}

		uint32_t length_;
		char *data_;
	};


	inline std::shared_ptr<MessageBuffer> CreateMessage(const char *data, uint32_t length) {
		return std::make_shared<MessageBuffer>(data, length);
	}

	inline std::shared_ptr<MessageBuffer> CreateMessageWithHeader(const char *data, uint32_t length) {
		std::shared_ptr<MessageBuffer> msg = std::make_shared<MessageBuffer>();
		msg->length_ = length + sizeof(message_head);
		msg->data_ = new char[length + sizeof(message_head)];
		message_head *head = (message_head *) msg->data_;
		head->length_ = length;
		head->crc32_ = 0;
		head->crypto_ = 0;
		head->compress_ = 0;
		memcpy(msg->data_ + sizeof(message_head), data, length);

		return msg;
	}

};

